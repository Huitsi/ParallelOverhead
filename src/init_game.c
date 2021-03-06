/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <time.h>

#include "common.h"
#include "game.h"
#include "level.h"
#include "hud.h"

int init_game(SDL_Window *window)
{
	float sector_angle = FULL_ANGLE/Settings.tunnel.sectors;
	GLfloat vertices[(4 + 2*Settings.tunnel.sectors + 2)*3];

	//Ship vertices
	vertices[0] = 1;
	vertices[1] = 0;
	vertices[2] = 0;

	vertices[3] = 1;
	vertices[4] = 0;
	vertices[5] = sector_angle;

	vertices[6] = 1;
	vertices[7] = sector_angle;
	vertices[8] = 0;

	vertices[9] = 1;
	vertices[10] = sector_angle;
	vertices[11] = sector_angle;

	//Wall vertices
	for (int i = 0; i <= Settings.tunnel.sectors; i++)
	{
		int pos = 12 + i * 6;
		float angle = i * sector_angle;

		vertices[pos + 0] = 1;
		vertices[pos + 1] = angle;
		vertices[pos + 2] = 0;
		vertices[pos + 3] = 1;
		vertices[pos + 4] = angle;
		vertices[pos + 5] = Settings.tunnel.length * Settings.tunnel.section_length;
	}

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint textures[3];
	glGenTextures(3, textures);
	GLuint ship_texture = textures[0];
	GLuint wall_texture = textures[1];
	GLuint timer_texture = textures[2];

	//Ship texture
	glBindTexture(GL_TEXTURE_2D, ship_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	char ship_path[Settings.paths.data_dir_len + sizeof "ship.bmp"];
	snprintf(ship_path, sizeof ship_path, "%s%s", Settings.paths.data_dir, "ship.bmp");

	SDL_Surface *ship_surface = SDL_LoadBMP(ship_path);
	if (!ship_surface)
	{
		report_SDL_error("Loading data/ship.bmp");
		return RET_SDL_ERR;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ship_surface->w, ship_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ship_surface->pixels);

	//Timer
	init_hud();
	SDL_Surface *timer_surface = SDL_CreateRGBSurface(0, 7*7+2*3+7*1, 2*11+1, 32, 0, 0, 0, 0);
	glBindTexture(GL_TEXTURE_2D, timer_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glClearColor(0,0,0,1);

	int w,h;
	SDL_GetWindowSize(window, &w, &h);
	glUniform2f(Locs.screen_wh, w, h);

	glVertexAttribPointer(Locs.vertex_pos_polar, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Locs.vertex_pos_polar);

	//Wall texture
	init_level();
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	if (report_GL_errors("game init"))
	{
		return RET_GL_ERR;
	}

	while(run_game(window, vertices, textures, timer_surface));

	free_hud();
	free_level();
	SDL_FreeSurface(timer_surface);

	if (report_GL_errors("game"))
	{
		return RET_GL_ERR;
	}
	return 0;
}
