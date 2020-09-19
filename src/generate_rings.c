/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <stdlib.h>

#include "common.h"

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
 * Add a section of rings to the given texture array.
 * @param colors The texture array to add the rings to
 * @param prev_color Previous color to transition from, or NULL if none
 * @param settings Game settings
 * @return The amount of rings added
 */
int generate_rings(float *colors, float* prev_color)
{
	int length = rani(Settings.transitions.min, Settings.transitions.max);

	float color[3];
	if (prev_color)
	{
		memcpy(color, prev_color, 3*sizeof(float));
	}
	else
	{
		color[0] = 0;
		color[1] = 1;
		color[2] = ranf();
		shufflef(color, 3);
	}

	float next_color[] = {0, ranf(), 1};
	shufflef(next_color, 3);

	float rstep = (next_color[0] - color[0]) / length;
	float gstep = (next_color[1] - color[1]) / length;
	float bstep = (next_color[2] - color[2]) / length;

	for (int r = 0; r < length; r++)
	{
		color[0] += rstep;
		color[1] += gstep;
		color[2] += bstep;
		for (int s = 0; s < Settings.game.sectors; s++)
		{
			float brightness = ranfi(0.5,1);

			colors[(r * Settings.game.sectors + s) * 4 + 0] = color[0];
			colors[(r * Settings.game.sectors + s) * 4 + 1] = color[1];
			colors[(r * Settings.game.sectors + s) * 4 + 2] = color[2];
			colors[(r * Settings.game.sectors + s) * 4 + 3] = brightness;

			if (ranf() <= Settings.difficulty.hole_density)
			{
				colors[(r*Settings.game.sectors + s)*4 + 3] = 0;
			}
		}
	}
	return length;
}
