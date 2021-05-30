/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <GLES2/gl2.h>

#define PATH_MAX 4096

enum
{
	RET_NO_ERR,
	RET_SDL_ERR,
	RET_GL_ERR
};

//Global struct for holding various parameters
struct settings
{
	struct
	{
		char data_dir[PATH_MAX], config_file[PATH_MAX];
		size_t data_dir_len, config_file_len;
	}
	paths;

	struct {char mute:1, quiet:1, hide_counters:1, disable_config_file:1;} flags;

	struct {int x, y, w, h;} window;

	struct
	{
		unsigned char length, sectors;
		float section_length;
		struct {unsigned char min, max;} color_transition_length;
	}
	tunnel;

	struct
	{
		unsigned char amount, depth, sector_offset;
	}
	ships;

	struct {unsigned int seed;} game;

	struct
	{
		float speed, uncarved_safe_chance;
		unsigned char carvers, transition_length;
		struct
		{
			float uncarved_safe_chance, speed;
			short carvers, transition_length;
		} increase;
	}
	difficulty;

	struct {unsigned char sector, depth;} hud;

	struct {unsigned char min, max;} tick_time;
};
extern struct settings Settings;

int init_SDL();
int init_GL(SDL_Window *window);
int init_game(SDL_Window *window);
int run_game(SDL_Window *window, GLfloat vertices[], GLuint textures[], SDL_Surface *timer_surface);

void set_setting(char *setting, char *value);
void override_params(char *file);

void init_audio();
void free_audio();

void report_SDL_error(char *context);
int report_GL_errors(char *context);
