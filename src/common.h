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

State game(SDL_Window *window);

int report_GL_errors(char *context);
