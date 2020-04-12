/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
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
		print_version = print_version || !strcmp("--version",argv[i]);
		print_help = print_help || !strcmp("--help",argv[i]);
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
		printf("\t--help    Print this help and exit.\n");
		printf("\t--version Print version and exit.\n");
	}
	if (print_version || print_help)
	{
		return 0;
	}

	Settings settings;
	return run_SDL(settings);
}
