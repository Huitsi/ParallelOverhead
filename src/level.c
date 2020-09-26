/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <time.h>
#include "common.h"

struct
{
	float previous_color[3];
	unsigned char *carvers;
	unsigned char *safe_path;
}
Level;

/**
 * @return A random number in the range [0,1]
 */
float ranf()
{
	return rand() / (float) RAND_MAX;
}

/**
 * @return A random number in the range [min,max]
 */
float ranfi(float min, float max)
{
	return min + ranf() * ( max - min );
}

/**
 * @return A random integer in the range [min,max]
 */
int rani(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

/**
 * Shuffle an array of floats.
 * @param array The array to shuffle
 * @param len The length of the array
 */
void shufflef(float *array, int len)
{
	for (int i = 0; i < len; i++)
	{
		int j = rani(0,i);
		float tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}

/**
 * Generate a random bright color.
 * @param color The 3-cell float array to store the generated color in.
 */
void generate_color(float *color)
{
	color[0] = 0;
	color[1] = 1;
	color[2] = ranf();
	shufflef(color, 3);
}

/**
 * Reset the level generator (for starting a new game).
 */
void reset_level()
{
	if (Settings.options.fixed_seed)
	{
		srand(Settings.options.seed);
	}

	generate_color(Level.previous_color);

	for (int i = 0; i < Settings.difficulty.carvers; i++)
	{
		Level.carvers[i] = rani(0, Settings.game.sectors - 1);
	}

	for (int i = 0; i < Settings.game.sectors; i++)
	{
		Level.safe_path[i] = Settings.difficulty.transition;
	}
}

/**
 * Initialize the level generator.
 */
void init_level()
{
	srand(time(NULL));
	Level.carvers = malloc(Settings.difficulty.carvers);
	Level.safe_path = malloc(Settings.game.sectors);
	reset_level();
}

/**
 * Free the memory used by level generation data.
 */
void free_level()
{
	free(Level.carvers);
	free(Level.safe_path);
}

/**
 * Add a section of rings to the given texture array.
 * @param colors The texture array to add the rings to
 * @return The amount of rings added
 */
int generate_rings(float *colors)
{
	int sectors_per_ship = (Settings.game.sectors / Settings.game.ships);
	int length = rani(Settings.game.color_transitions.min, Settings.game.color_transitions.max);

	float color[3];
	memcpy(color, Level.previous_color, 3*sizeof(float));

	float next_color[3];
	generate_color(next_color);
	memcpy(Level.previous_color, next_color, 3*sizeof(float));

	float rstep = (next_color[0] - color[0]) / length;
	float gstep = (next_color[1] - color[1]) / length;
	float bstep = (next_color[2] - color[2]) / length;

	for (int r = 0; r < length; r++)
	{
		for (int c = 0; c < Settings.difficulty.carvers; c++)
		{
			for (int s = 0; s < Settings.game.ships; s++)
			{
				int sector = Level.carvers[c] + s * sectors_per_ship;
				Level.safe_path[sector] = Settings.difficulty.transition + 1;
			}

			short sector = (Level.carvers[c] + rani(-1,1)) % sectors_per_ship;
			if (sector < 0)
			{
				sector += sectors_per_ship;
			}
			Level.carvers[c] = sector;


		}

		color[0] += rstep;
		color[1] += gstep;
		color[2] += bstep;
		for (int s = 0; s < Settings.game.sectors; s++)
		{
			int pos = (r * Settings.game.sectors + s) * 4;

			colors[pos + 0] = color[0];
			colors[pos + 1] = color[1];
			colors[pos + 2] = color[2];
			colors[pos + 3] = ranfi(0.6,1);

			if (Level.safe_path[s] || ranf() < Settings.difficulty.uncarved_safe_chance)
			{
				Level.safe_path[s]--;
			}
			else
			{
				colors[pos + 3] = 0;
			}
		}
	}
	return length;
}
