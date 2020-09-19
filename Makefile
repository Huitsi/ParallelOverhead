#Copyright © 2020 Linus Vanas <linus@vanas.fi>
#SPDX-License-Identifier: MIT

C = $(wildcard src/*.c)
H = $(wildcard src/*.h)
O = $(patsubst src/%.c, tmp/%.o, $C)
D = $(patsubst src/%.c, tmp/%.dbg.o, $C)

.PHONY: all clean

po.debug: $D
	gcc -lSDL2 -lGL -lm $^ -o $@

parallel_overhead: $O
	gcc -lSDL2 -lGL -lm $^ -o $@

$O: tmp/%.o: src/%.c $H | tmp
	gcc -O3 -Wall -c $< -o $@

$D: tmp/%.dbg.o: src/%.c $H | tmp
	gcc -ggdb -Wall -c $< -o $@

tmp:
	mkdir tmp

all: parallel_overhead

clean:
	rm -fr parallel_overhead po.debug tmp
