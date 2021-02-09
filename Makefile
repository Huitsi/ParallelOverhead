#Copyright © 2020 Linus Vanas <linus@vanas.fi>
#SPDX-License-Identifier: MIT

prefix = ./local
bindir = $(prefix)/games
datadir = $(prefix)/share

CFLAGS = -Wall -O3

C = $(wildcard src/*.c)
O = $(patsubst src/%.c, tmp/%.o, $C)

version := $(shell git describe --tags)

ifdef version
	VERSION = -DVERSION=\"$(version)\"
endif

.PHONY: all clean install uninstall

all: parallel_overhead

clean:
	rm -fr parallel_overhead tmp

install: parallel_overhead
	install -D -t$(DESTDIR)$(bindir) parallel_overhead
	find data -type f |\
	 xargs install -m644 -D -t$(DESTDIR)$(datadir)/parallel_overhead

uninstall:
	rm -f $(DESTDIR)$(bindir)/parallel_overhead
	rm -fr $(DESTDIR)$(datadir)/parallel_overhead

parallel_overhead: $O
	$(CC) $(LDFLAGS) $^ -lSDL2 -lGLESv2 -lm -o $@

tmp:
	mkdir tmp

tmp/o.makefile: $C | tmp
	echo -n > $@
	for f in $^;\
	 do echo -n 'tmp/' >> $@;\
	 $(CC) -MM $$f >> $@;\
	 echo '	$$(CC) $$(CPPFLAGS) $$(CFLAGS) $(VERSION) -c $$< -o $$@' >> $@;\
	 echo >> $@;\
	done

include tmp/o.makefile
