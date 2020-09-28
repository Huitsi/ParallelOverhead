/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

struct settings Settings =
{
	.window = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 480},
	.game =
	{
		.rings = 100, .sectors = 10, .ships = 2, .ship_depth = 1, .start_sector = 2,
		.color_transitions = {12, 60}
	},
	.difficulty =
	{
		.speed = 0.000075, .uncarved_safe_chance = 0.5, .carvers = 2, .transition_length = 8,
		.increase = {.speed = 0, .uncarved_safe_chance = -0.25, .carvers = -1, .transition_length = 0}
	},
	.hud = {.sector = 2, .depth = 3},
	.tick_time = {16, 24}
};

/**
 * Load game parameter overrides from the given file.
 * @param file The path of the override file.
 */
void override_params(char *file)
{
	FILE *f = fopen(file, "r");
	if (f == NULL)
	{
		perror(file);
		return;
	}

	int sum = 0;
	sum += fscanf(f, "%hhud", &Settings.game.rings);
	sum += fscanf(f, "%hhud", &Settings.game.sectors);
	sum += fscanf(f, "%hhud", &Settings.game.ships);
	sum += fscanf(f, "%hhud", &Settings.game.ship_depth);
	sum += fscanf(f, "%hhud", &Settings.game.start_sector);
	sum += fscanf(f, "%hhud", &Settings.game.color_transitions.min);
	sum += fscanf(f, "%hhud", &Settings.game.color_transitions.max);

	sum += fscanf(f, "%f", &Settings.difficulty.speed);
	sum += fscanf(f, "%f", &Settings.difficulty.uncarved_safe_chance);
	sum += fscanf(f, "%hhud", &Settings.difficulty.carvers);
	sum += fscanf(f, "%hhud", &Settings.difficulty.transition_length);

	sum += fscanf(f, "%f", &Settings.difficulty.increase.uncarved_safe_chance);
	sum += fscanf(f, "%f", &Settings.difficulty.increase.speed);
	sum += fscanf(f, "%hd", &Settings.difficulty.increase.carvers);
	sum += fscanf(f, "%hd", &Settings.difficulty.increase.transition_length);

	sum += fscanf(f, "%hhud", &Settings.hud.sector);
	sum += fscanf(f, "%hhud", &Settings.hud.depth);

	sum += fscanf(f, "%hhud", &Settings.tick_time.min);
	sum += fscanf(f, "%hhud", &Settings.tick_time.max);

	if (sum < 19)
	{
		fprintf(stderr, "Override file was incomplete or invalid.\n");
	}

	fclose(f);
}
