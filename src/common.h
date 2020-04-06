/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <SDL2/SDL.h>
#include <GLES3/gl32.h>

#define VERSION "0.1"

typedef enum
{
	STATE_GAME,
	STATE_QUIT,
	STATE_ERROR,
} State;

typedef struct
{
	unsigned short rings;
	unsigned short sectors;

	unsigned short max_color_transition_length;
	unsigned short min_color_transition_length;

	unsigned short max_tick_time;
	unsigned short min_tick_time;
} Settings;

State game(SDL_Window *window);

int report_GL_errors(char *context);
