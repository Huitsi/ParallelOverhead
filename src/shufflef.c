/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <stdlib.h>

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
