/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

State game(SDL_Window *window)
{
	int sectors = 8;
	float sector_angle = 2*PI/sectors;
	float corner_angles[sectors];
	for (int i = 0; i < sectors; i++)
	{
		corner_angles[i] = i*sector_angle;
	}

	GLfloat vertices[2*3*(sectors+1)];
	for (int i = 0; i <= sectors; i++)
	{
		vertices[i*6+0] = corner_angles[i];
		vertices[i*6+1] = 1;
		vertices[i*6+2] = 1;

		vertices[i*6+3] = corner_angles[i];
		vertices[i*6+4] = 1;
		vertices[i*6+5] = .02;
	}

	glClearColor(0,0,0,1);

	GLuint vbuf;
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
					//TODO
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

		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices)/3);
		glDisableVertexAttribArray(0);

		if (report_GL_errors("game"))
		{
			return STATE_ERROR;
		}

		SDL_GL_SwapWindow(window);
	}

	return STATE_QUIT;
}
