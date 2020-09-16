/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <GLES3/gl32.h>

enum
{
	RET_NO_ERR,
	RET_SDL_ERR,
	RET_GL_ERR
};

struct
{
	struct {int x, y, w, h;} window;

	unsigned short rings;
	unsigned short sectors;

	unsigned short amount_of_ships;
	unsigned short ships_initial_sector_offset;

	unsigned short max_color_transition_length;
	unsigned short min_color_transition_length;

	unsigned short max_tick_time;
	unsigned short min_tick_time;

	float speed_multiplier;

	float hole_probability;
} Settings;

int run_SDL();
int run_GL(SDL_Window *window);
int run_game(SDL_Window *window);

void init_audio();
void free_audio();

void report_SDL_error(char *context);
int report_GL_errors(char *context);
