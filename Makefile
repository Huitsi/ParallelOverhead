#Copyright © 2020 Linus Vanas <linus@vanas.fi>
#SPDX-License-Identifier: MIT

C = $(wildcard src/*.c)
H = $(wildcard src/*.h)
O = $(patsubst src/%.c, tmp/%.o, $C)

.PHONY: all clean

parallel_overhead: $O
	gcc -lSDL2 -lGL -lm $^ -o $@

$O: tmp/%.o: src/%.c $H | tmp
	gcc -Wall -c $< -o $@

tmp:
	mkdir tmp

all: parallel_overhead

clean:
	rm -f parallel_overhead
	rm -fr tmp
