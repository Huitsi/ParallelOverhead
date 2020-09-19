/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <string.h>

#include "common.h"

#define VERSION "0.1"

/**
 * Parse arguments and proceed to SDL initialization.
 * @param argc The amount of arguments supplied to the program.
 * @param argv The arguments supplied to the program.
 * @return 0 if the program was ran without errors, a non-zero integer otherwise.
 */
int main(int argc, char **argv)
{
	int print_version = 0;
	int print_help = 0;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp("--seed", argv[i]))
		{
			Settings.options.fixed_seed = 1;
			if (++i >= argc)
			{
				Settings.options.fixed_seed = 0;
				fprintf(stderr, "No seed provided\n");
				break;
			}
			else if (!sscanf(argv[i], "%u", &Settings.options.seed))
			{
				Settings.options.fixed_seed = 0;
				fprintf(stderr, "Invalid seed: %s\n", argv[i]);
			}
		}
		print_version = print_version || !strcmp("--version", argv[i]);
		print_help = print_help || !strcmp("--help", argv[i]);
		Settings.options.mute = Settings.options.mute || !strcmp("--mute", argv[i]);
	}

	if (print_version)
	{
		printf("Parallel Overhead %s\n", VERSION);
	}
	if (print_help)
	{
		printf("An endless runner game.\n");
		printf("Usage: %s [options]\n", argv[0]);
		printf("Options:\n");
		printf("\t--help         Print this help and exit.\n");
		printf("\t--version      Print the version number and exit.\n");
		printf("\t--mute         Do not play any audio.\n");
		printf("\t--seed <seed>  Set the level generator seed to <seed>, a non-negative integer.\n");
	}
	if (print_version || print_help)
	{
		return 0;
	}

	return init_SDL();
}
