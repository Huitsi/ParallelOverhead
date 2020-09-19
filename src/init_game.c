/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <math.h>
#include <time.h>

#include "common.h"
#include "game.h"

int init_game(SDL_Window *window)
{
	if (Settings.options.fixed_seed)
	{
		srand(Settings.options.seed);
	}
	else
	{
		srand(time(NULL));
	}
	Settings.tick_time.min = 16;
	Settings.tick_time.max = 24;

	Settings.game.rings = 100;
	Settings.game.sectors = 10;
	Settings.game.ships = 2;
	Settings.game.start_sector_offset = 2;

	Settings.transitions.min = 10;
	Settings.transitions.max = 40;

	Settings.difficulty.hole_density = 0.01;
	Settings.difficulty.speed = 0.000075;

	//Wall vertices
	float sector_angle = 2*PI/Settings.game.sectors;
	GLfloat vertices[(4 + 3*Settings.game.sectors + 1)*3];

	vertices[0] = 1;
	vertices[2] = 0;
	vertices[3] = (1 - sector_angle)/2;

	vertices[3] = 1;
	vertices[4] = 0;
	vertices[5] = 1 - (1 - sector_angle)/2;

	vertices[6] = 1;
	vertices[7] = sector_angle;
	vertices[8] = (1 - sector_angle)/2;

	vertices[9] = 1;
	vertices[10] = sector_angle;
	vertices[11] = 1 - (1 - sector_angle)/2;

	for (int i = 0; i <= Settings.game.sectors; i++)
	{
		float angle = i*sector_angle;

		vertices[12 + i*6 + 0] = 1;
		vertices[12 + i*6 + 1] = angle;
		vertices[12 + i*6 + 2] = 0;

		vertices[12 + i*6 + 3] = 1;
		vertices[12 + i*6 + 4] = angle;
		vertices[12 + i*6 + 5] = Settings.game.rings;
	}

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint textures[2];
	glGenTextures(2, textures);
	GLuint ship_texture = textures[0];
	//GLuint wall_texture = textures[1];

	//Ship texture
	glBindTexture(GL_TEXTURE_2D, ship_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	SDL_Surface *ship_surface = SDL_LoadBMP("data/ship.bmp");
	if (!ship_surface)
	{
		return RET_SDL_ERR;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ship_surface->w, ship_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ship_surface->pixels);

	glClearColor(0,0,0,1);

	int w,h;
	SDL_GetWindowSize(window, &w, &h);
	glUniform2f(LOC_WH,w,h);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//glDisableVertexAttribArray(0);

	if (report_GL_errors("game init"))
	{
		return RET_GL_ERR;
	}

	return run_game(window, vertices, textures);
}
