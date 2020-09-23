/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <GLES3/gl32.h>

enum
{
	RET_NO_ERR,
	RET_SDL_ERR,
	RET_GL_ERR
};

//Global struct for holding various parameters
struct
{
	struct {char mute, fixed_seed, hide_counters; unsigned int seed;} options;

	struct {int x, y, w, h;} window;

	struct {unsigned short rings, sectors, ships, start_sector;} game;

	struct {unsigned short min, max;} transitions;

	struct {unsigned short sector, depth;} timer;

	struct {float hole_density, speed;} difficulty;

	struct {unsigned short min, max;} tick_time;
}
Settings;

int init_SDL();
int init_GL(SDL_Window *window);
int init_game(SDL_Window *window);
int run_game(SDL_Window *window, GLfloat vertices[], GLuint textures[], SDL_Surface *timer_surface);

void init_audio();
void free_audio();

void report_SDL_error(char *context);
int report_GL_errors(char *context);
