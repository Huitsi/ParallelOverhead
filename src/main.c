/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <GLES3/gl32.h>

#define VERSION "0.1"

void fail(char *fun)
{
	fprintf(stderr, "%s failed: %s\n", fun, SDL_GetError());
	SDL_Quit();
}

int main(int argc, char **argv)
{
	int print_version = 0;
	int print_help = 0;
	for (int i = 1; i < argc; i++)
	{
		print_version = print_version || !strcmp("--version",argv[i]);
		print_help = print_help || !strcmp("--help",argv[i]);
	}

	if (print_version)
	{
		printf("Parallel Overhead %s\n", VERSION);
	}
	if (print_help)
	{
		printf("Usage: %s [options]\n", argv[0]);
		printf("Options:\n");
		printf("\t--help    Print this help and exit.\n");
		printf("\t--version Print version and exit.\n");
	}
	if (print_version || print_help)
	{
		return 0;
	}

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		fail("SDL_Init");
		return 1;
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES))
	{
		fail("SDL_GL_SetAttribute");
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3))
	{
		fail("SDL_GL_SetAttribute");
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2))
	{
		fail("SDL_GL_SetAttribute");
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
		fail("SDL_CreateWindow");
		return 1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (context == NULL)
	{
		fail("SDL_GL_CreateContext");
		return 1;
	}

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);

	SDL_Delay(2000);

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}
