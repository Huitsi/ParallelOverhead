/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

#define LOC_WH 0
#define LOC_POS 1
#define LOC_RINGS 3
#define PI 3.14159265358979323846

State game(SDL_Window *window)
{
	int rings = 2;

	int sectors = 6;
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

	//TODO Move to loop
	float texdata[] = {1,0,0, 0,1,0, 0,0,1, 1,1,0, 0,1,1, 1,0,1,  .75,0,0, 0,.75,0, 0,0,.75, .75,.75,0, 0,.75,.75, .75,0,.75};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sectors, rings, 0, GL_RGB, GL_FLOAT, texdata);

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

	while (1)
	{
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

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices)/3);
		SDL_GL_SwapWindow(window);
	}

	if (report_GL_errors("game init"))
	{
		return STATE_ERROR;
	}
	return STATE_QUIT;
}
