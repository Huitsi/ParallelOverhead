/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"
struct settings Settings =
{
	.window = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 480},
	.game = {.rings = 100, .sectors = 10, .ships = 2, .start_sector = 2},
	.transitions = {10, 40},
	.timer = {.sector = 2, .depth = 3},
	.difficulty = {.uncarved_safe_chance = 0.25, .speed = 0.000075, .carvers = 1, .transition = 8},
	.tick_time = {16, 24}
};
