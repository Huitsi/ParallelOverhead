/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <string.h>

#include "common.h"

#ifndef VERSION
	#define VERSION "(unknown version)"
#endif

/**
 * Parse arguments and proceed to SDL initialization.
 * @param argc The amount of arguments supplied to the program.
 * @param argv The arguments supplied to the program.
 * @return 0 if the program was ran without errors, a non-zero integer otherwise.
 */
int main(int argc, char **argv)
{
	char print_version = 0;
	char print_help = 0;
	char print_settings = 0;

	for (int i = 1; i < argc; i++)
	{
		if(!strcmp("--version", argv[i]))
		{
			print_version = 1;
			continue;
		}

		if (!strcmp("--help", argv[i]))
		{
			print_help = 1;
			continue;
		}

		if (!strcmp("--help-settings", argv[i]))
		{
			print_settings = 1;
			continue;
		}

		if (!strcmp("--mute", argv[i]))
		{
			Settings.flags.mute = 1;
			continue;
		}

		if (!strcmp("--quiet", argv[i]))
		{
			Settings.flags.quiet = 1;
			continue;
		}

		if (!strcmp("--mute", argv[i]))
		{
			Settings.flags.mute = 1;
			continue;
		}

		if (!strcmp("--hide-counters", argv[i]))
		{
			Settings.flags.hide_counters = 1;
			continue;
		}

		if (!strcmp("--disable-config-file", argv[i]))
		{
			Settings.flags.disable_config_file = 1;
			continue;
		}

		int len = strlen(argv[i]);
		char setting[255], value[len];
		if (sscanf(argv[i], "--%255[^=]=%s", setting, value) == 2)
		{
			set_setting(setting, value);
			continue;
		}

		fprintf(stderr, "Unrecognized option: %s\n", argv[i]);
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
		printf(" --help                Print this help and exit.\n");
		printf(" --version             Print the version number and exit.\n");
		printf(" --quiet               Do not print results into standard output.\n");
		printf(" --mute                Do not play any audio.\n");
		printf(" --hide-counters       Do not display time and distance counters.\n");
		printf(" --disable-config-file Do not read from or write to the config file.\n");
		printf(" --<setting>=<value>   Set <setting> to <value> (see below).\n");
		printf(" --help-settings       List available settings (see above) and exit.\n");

	}

	if (print_settings)
	{
		printf
		(
			"Settings:\n"
			" seed (integer) A seed to use for generating the level.\n"
			"       Unless 0, the seed is reset to this value for each run.\n"
			"       When 0 (default), a random (time-based) seed is chosen.\n"
		);
	}

	if (print_version || print_help || print_settings)
	{
		return 0;
	}

	return init_SDL();
}
