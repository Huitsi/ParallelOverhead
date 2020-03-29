#Copyright © 2020 Linus Vanas <linus@vanas.fi>
#SPDX-License-Identifier: MIT

SRC = $(shell find ./src/ -type f)

.PHONY: all clean

parallel_overhead: $(SRC)
	gcc -Wall -lSDL2 -lGL -o $@ $^

all: parallel_overhead

clean:
	rm -f parallel_overhead
