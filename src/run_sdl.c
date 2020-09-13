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
 * Initialize SDL and then proceed to OpenGL initialization.
 * Cleanup SDL after the game is closed.
 * @param settings Game settings.
 */
int run_SDL(Settings settings)
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

	SDL_Window *window =  SDL_CreateWindow
	(
		"Parallel Overhead",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		480, 480,
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

	int ret = run_GL(settings, window);

	free_audio();

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return ret;
}
