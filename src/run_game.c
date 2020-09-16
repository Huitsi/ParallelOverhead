/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <math.h>

#include "common.h"
#include "audio.h"
#include "shufflef.h"

#define LOC_WH 0
#define LOC_POS 1
#define LOC_TEX_AREA 3
#define PI 3.14159265358979323846

#define settings Settings

/**
 * Add a section of rings to the given texture array.
 * @param colors The texture array to add the rings to
 * @param prev_color Previous color to transition from, or NULL if none
 * @param settings Game settings
 * @return The amount of rings added
 */
int generate_rings(float *colors, float* prev_color)
{
	int length = rani(settings.min_color_transition_length, settings.max_color_transition_length);

	float color[3];
	if (prev_color)
	{
		memcpy(color, prev_color, 3*sizeof(float));
	}
	else
	{
		color[0] = 0;
		color[1] = 1;
		color[2] = ranf();
		shufflef(color, 3);
	}

	float next_color[] = {0, ranf(), 1};
	shufflef(next_color, 3);

	float rstep = (next_color[0] - color[0]) / length;
	float gstep = (next_color[1] - color[1]) / length;
	float bstep = (next_color[2] - color[2]) / length;

	for (int r = 0; r < length; r++)
	{
		color[0] += rstep;
		color[1] += gstep;
		color[2] += bstep;
		for (int s = 0; s < settings.sectors; s++)
		{
			float brightness = ranfi(0.5,1);

			colors[(r*settings.sectors + s)*4 + 0] = color[0];
			colors[(r*settings.sectors + s)*4 + 1] = color[1];
			colors[(r*settings.sectors + s)*4 + 2] = color[2];
			colors[(r*settings.sectors + s)*4 + 3] = brightness;
			if (ranf() <= settings.hole_probability)
			{
				colors[(r*settings.sectors + s)*4 + 3] = 0;
			}
		}
	}
	return length;
}

int run_game(SDL_Window *window)
{
	settings.rings = 100;
	settings.sectors = 10;
	settings.amount_of_ships = 2;
	settings.ships_initial_sector_offset = 2;
	settings.min_color_transition_length = 10;
	settings.max_color_transition_length = 40;
	settings.min_tick_time = 16;
	settings.max_tick_time = 24;
	settings.hole_probability = 0.01;
	settings.speed_multiplier = 0.000075;

	int rings = settings.rings;
	int sectors = settings.sectors;
	float sector_angle = 2*PI/sectors;

	struct
	{
		int alive;
		int sector;
	}
	ships[settings.amount_of_ships];
	for (int i = 0; i < settings.amount_of_ships; i++)
	{
		ships[i].alive = 1;
		ships[i].sector = settings.ships_initial_sector_offset + i * (sectors / settings.amount_of_ships);
	}

	//Wall vertices
	const int wall_vertices_start = 3*4;
	GLfloat vertices[wall_vertices_start + 2*3*(sectors+1)];

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

	for (int i = 0; i <= sectors; i++)
	{
		float angle = i*sector_angle;

		vertices[i*6+0+wall_vertices_start] = 1;
		vertices[i*6+1+wall_vertices_start] = angle;
		vertices[i*6+2+wall_vertices_start] = 0;

		vertices[i*6+3+wall_vertices_start] = 1;
		vertices[i*6+4+wall_vertices_start] = angle;
		vertices[i*6+5+wall_vertices_start] = rings;
	}

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint textures[2];
	glGenTextures(2, textures);
	GLuint ship_texture = textures[0];
	GLuint wall_texture = textures[1];

	//Wall texture
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	float wall_texture_data[(rings+settings.max_color_transition_length)*sectors*4*100];
	int rings_generated = generate_rings(wall_texture_data, NULL);
	while (rings_generated < rings)
	{
		rings_generated += generate_rings(&wall_texture_data[rings_generated*sectors*4], &wall_texture_data[rings_generated*sectors*4-4]);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sectors, rings, 0, GL_RGBA, GL_FLOAT, wall_texture_data);

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

	float ship_ring = 0;

	unsigned int last_tick_time = 0;
	unsigned int time_survived = 0;
	while (1)
	{
		unsigned int  tick_start_time =  SDL_GetTicks();
		if (last_tick_time > settings.max_tick_time)
		{
			last_tick_time = settings.max_tick_time;
		}
		else if (last_tick_time < settings.min_tick_time)
		{
			SDL_Delay(settings.min_tick_time - last_tick_time);
			last_tick_time = settings.min_tick_time;
		}
		time_survived += last_tick_time;

		float speed = sqrt(time_survived)*settings.speed_multiplier;

		int ship_sector_delta = 0;

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					return 0;
				case SDL_WINDOWEVENT:
					//Window resize
					SDL_GetWindowSize(window, &w, &h);
					glUniform2f(LOC_WH,w,h);
					glViewport(0,0,w,h);
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
						case SDLK_RIGHT:
							ship_sector_delta++;
							break;
						case SDLK_LEFT:
							ship_sector_delta--;
							break;
					}
					break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		//Update and render walls
		glBindTexture(GL_TEXTURE_2D, wall_texture);
		while(ship_ring > 1)
		{
			memmove(wall_texture_data, &wall_texture_data[sectors*4], sizeof(wall_texture_data) - sectors*4*sizeof(float));
			rings_generated--;
			ship_ring--;

			while (rings_generated < rings)
			{
				rings_generated += generate_rings(&wall_texture_data[rings_generated*sectors*4], &wall_texture_data[rings_generated*sectors*4-4]);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sectors, rings, 0, GL_RGBA, GL_FLOAT, wall_texture_data);
		}

		glUniform3f(LOC_POS, 0, 0, -ship_ring);
		glUniform2f(LOC_TEX_AREA, 2*PI, rings);
		glDrawArrays(GL_TRIANGLE_STRIP, wall_vertices_start/3, (wall_vertices_start + sizeof(vertices))/3);

		//Update and render ships
		ship_ring += last_tick_time*speed;

		glBindTexture(GL_TEXTURE_2D, ship_texture);
		glUniform2f(LOC_TEX_AREA, sector_angle, 1);

		if (ship_sector_delta)
		{
			play_move_sound();
		}

		int ships_alive = 0;
		for (int i = 0; i < settings.amount_of_ships; i++)
		{
			if (ships[i].alive)
			{
				ships[i].sector += ship_sector_delta;

				if (!wall_texture_data[sectors*4 + ships[i].sector*4 + 3])
				{
					ships[i].alive = 0;
					play_death_sound();
					continue;
				}

				ships_alive++;

				glUniform3f(LOC_POS, 0, ships[i].sector * sector_angle, 1);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, wall_vertices_start/3);
			}
		}

		SDL_GL_SwapWindow(window);

		if (!ships_alive)
		{
			break;
		}

		last_tick_time = SDL_GetTicks() - tick_start_time;
	}

	if (report_GL_errors("game"))
	{
		return RET_GL_ERR;
	}
	return 0;
}
