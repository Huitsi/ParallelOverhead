/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

struct settings Settings =
{
	.paths = {.data_dir_len = 0, .config_file_len = 0},
	.window = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 480},
	.tunnel =
	{
		.length = 255, .sectors = 10, .section_length = 1.0,
		.color_transition_length = {12, 60}
	},
	.ships =
	{
		.amount = 2, .depth = 1, .sector_offset = 2,
	},
	.difficulty =
	{
		.speed = 0.000075, .uncarved_safe_chance = 0.5, .carvers = 2, .transition_length = 8,
		.increase = {.speed = 0, .uncarved_safe_chance = -0.25, .carvers = -1, .transition_length = 0}
	},
	.hud = {.sector = 2, .depth = 3},
	.tick_time = {16, 24}
};

char try_set_uchar(char *setting_name, unsigned char *setting, char *setting_str,  char *value_str)
{
	if (strcmp(setting_name, setting_str))
	{
		return 0;
	}
	unsigned char value;
	if (sscanf(value_str, "%hhud", &value))
	{
		*setting = value;
		return 1;
	}
	fprintf(stderr, "Invalid %s: %s\n", setting_str, value_str);
	return 1;
}

char try_set_float(char *setting_name, float *setting, char *setting_str,  char *value_str)
{
	if (strcmp(setting_name, setting_str))
	{
		return 0;
	}
	float value;
	if (sscanf(value_str, "%f", &value))
	{
		*setting = value;
		return 1;
	}
	fprintf(stderr, "Invalid %s: %s\n", setting_str, value_str);
	return 1;
}

void set_setting(char *setting, char *value)
{
	if (!strcmp("seed", setting))
	{
		unsigned int seed;
		if (sscanf(value, "%ud", &seed))
		{
			Settings.game.seed = seed;
			return;
		}
		fprintf(stderr, "Invalid seed: %s\n", value);
		return;
	}

	if (try_set_uchar("ships", &(Settings.ships.amount), setting, value)) return;
	if (try_set_uchar("ship-depth", &(Settings.ships.depth), setting, value)) return;
	if (try_set_uchar("ship-sector-offset", &(Settings.ships.sector_offset), setting, value)) return;

	if (try_set_uchar("tunnel-sectors", &(Settings.tunnel.sectors), setting, value)) return;
	if (try_set_uchar("tunnel-length", &(Settings.tunnel.length), setting, value)) return;
	if (try_set_float("tunnel-section-length", &(Settings.tunnel.section_length), setting, value)) return;
	if (try_set_uchar("min-color-transition-length", &(Settings.tunnel.color_transition_length.min), setting, value)) return;
	if (try_set_uchar("max-color-transition-length", &(Settings.tunnel.color_transition_length.max), setting, value)) return;

	fprintf(stderr, "Unrecognized setting: %s\n", setting);
}

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

	fclose(f);
}
