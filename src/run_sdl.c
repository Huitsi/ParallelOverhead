/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

/**
 * Print the  latest SDL error.
 * @param context A context string for the error.
 */
void report_SDL_error(char *context)
{
	fprintf(stderr, "%s: %s\n", context, SDL_GetError());
}

/**
 * Load into settings the window size and position
 * from a file created on the previous execution.
 */
void load_window_size()
{
	Settings.window.x = SDL_WINDOWPOS_UNDEFINED;
	Settings.window.y = SDL_WINDOWPOS_UNDEFINED;
	Settings.window.w = 480;
	Settings.window.h = 480;

	FILE *f = fopen("window.dat","r");
	if (f == NULL)
	{
		return;
	}

	int x, y, w, h;

	if (fscanf(f, "%d %d %d %d", &x, &y, &w, &h ) < 4)
	{
		fclose(f);
		fprintf(stderr, "Invalid window.dat\n");
		return;
	}

	fclose(f);

	Settings.window.x = x;
	Settings.window.y = y;
	Settings.window.w = w;
	Settings.window.h = h;
}

/**
 * Load the current window size and position to a file.
 */
void save_window_size(SDL_Window *window)
{
	int x, y, w, h;
	SDL_GetWindowPosition(window, &x, &y);
	SDL_GetWindowSize(window, &w, &h);

	FILE *f = fopen("window.dat","w");
	if (f == NULL)
	{
		return;
	}

	fprintf(f, "%d %d %d %d", x, y, w, h);
	fclose(f);
}

/**
 * Initialize SDL and then proceed to OpenGL initialization.
 * Cleanup SDL after the game is closed.
 */
int run_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		report_SDL_error("SDL_Init");
		SDL_Quit();
		return RET_SDL_ERR;
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES))
	{
		report_SDL_error("SDL_GL_SetAttribute");
		SDL_Quit();
		return RET_SDL_ERR;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3))
	{
		report_SDL_error("SDL_GL_SetAttribute");
		SDL_Quit();
		return RET_SDL_ERR;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2))
	{
		report_SDL_error("SDL_GL_SetAttribute");
		SDL_Quit();
		return RET_SDL_ERR;
	}

	load_window_size();

	SDL_Window *window =  SDL_CreateWindow
	(
		"Parallel Overhead",
		Settings.window.x,
		Settings.window.y,
		Settings.window.w,
		Settings.window.h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (window == NULL)
	{
		report_SDL_error("SDL_CreateWindow");
		SDL_Quit();
		return RET_SDL_ERR;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (context == NULL)
	{
		report_SDL_error("SDL_GL_CreateContext");
		SDL_Quit();
		return RET_SDL_ERR;
	}

	init_audio();

	int ret = run_GL(window);

	save_window_size(window);

	free_audio();

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return ret;
}
