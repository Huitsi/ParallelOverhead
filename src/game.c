/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

#include <stdlib.h>
#include <string.h>

#define LOC_WH 0
#define LOC_POS 1
#define LOC_RINGS 3
#define PI 3.14159265358979323846

/**
 * @return A random number in the range [0,1]
 */
float ranf()
{
	return rand() / (float) RAND_MAX;
}

/**
 * @return A random number in the range [min,max]
 */
float ranfi(float min, float max)
{
	return min + ranf() * ( max - min );
}

/**
 * @return A random integer in the range [min,max]
 */
int rani(int min, int max)
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

/**
 * Shuffle an array of floats.
 * @param array The array to shuffle
 * @param len The length of the array
 */
void shufflef(float *array, int len)
{
	for (int i = 0; i < len; i++)
	{
		int j = rani(0,i);
		float tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}

/**
 * Add a section of rings to the given texture array.
 * @param colors The texture array to add the rings to
 * @param prev_color Previous color to transition from, or NULL if none
 * @param settings Game settings
 * @return The amount of rings added
 */
int generate_rings(float *colors, float* prev_color, Settings settings)
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
		}
	}
	return length;
}

State game(SDL_Window *window)
{
	Settings settings;
	settings.rings = 100;
	settings.sectors = 10;
	settings.min_color_transition_length = 10;
	settings.max_color_transition_length = 40;
	settings.min_tick_time = 16;
	settings.max_tick_time = 24;

	int rings = settings.rings;
	int sectors = settings.sectors;
	float sector_angle = 2*PI/sectors;

	//Wall vertices
	GLfloat vertices[2*3*(sectors+1)];
	for (int i = 0; i <= sectors; i++)
	{
		float angle = i*sector_angle;

		vertices[i*6+0] = 1;
		vertices[i*6+1] = angle;
		vertices[i*6+2] = 0;

		vertices[i*6+3] = 1;
		vertices[i*6+4] = angle;
		vertices[i*6+5] = rings;
	}

	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Wall texture
	GLuint wall_texture;
	glGenTextures(1, &wall_texture);
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	//Create wall texture
	float wall_texture_data[(rings+settings.max_color_transition_length)*sectors*4*100];
	int rings_generated = generate_rings(wall_texture_data, NULL, settings);
	while (rings_generated < rings)
	{
		rings_generated += generate_rings(&wall_texture_data[rings_generated*sectors*4], &wall_texture_data[rings_generated*sectors*4-4], settings);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sectors, rings, 0, GL_RGBA, GL_FLOAT, wall_texture_data);

	glClearColor(0,0,0,1);
	glUniform1f(LOC_RINGS,rings);

	int w,h;
	SDL_GetWindowSize(window, &w, &h);
	glUniform2f(LOC_WH,w,h);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//glDisableVertexAttribArray(0);

	if (report_GL_errors("game init"))
	{
		return STATE_ERROR;
	}

	float ship_ring = 0;

	unsigned int last_tick_time = 0;
	float speed = 0.02;//9;
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

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					return STATE_QUIT;
				case SDL_WINDOWEVENT:
					//Window resize
					SDL_GetWindowSize(window, &w, &h);
					glUniform2f(LOC_WH,w,h);
					glViewport(0,0,w,h);
					break;
				case SDL_KEYDOWN:
					//TODO
					break;
				case SDL_KEYUP:
					//TODO
					break;
			}
		}

		ship_ring += last_tick_time*speed;
		while(ship_ring > 1)
		{
			memmove(wall_texture_data, &wall_texture_data[sectors*4], sizeof(wall_texture_data) - sectors*4*sizeof(float));
			rings_generated--;
			ship_ring--;

			while (rings_generated < rings)
			{
				rings_generated += generate_rings(&wall_texture_data[rings_generated*sectors*4], &wall_texture_data[rings_generated*sectors*4-4], settings);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sectors, rings, 0, GL_RGBA, GL_FLOAT, wall_texture_data);
		}


		glUniform3f(LOC_POS, 0, 0, -ship_ring);

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices)/3);
		SDL_GL_SwapWindow(window);

		last_tick_time = SDL_GetTicks() - tick_start_time;
	}

	if (report_GL_errors("game"))
	{
		return STATE_ERROR;
	}
	return STATE_QUIT;
}
