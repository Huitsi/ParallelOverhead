/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <math.h>

#define FULL_ANGLE 2*M_PI

extern struct
{
	int vertex_pos_polar, screen_wh, object_pos_polar, texture_area;
}
Locs;
