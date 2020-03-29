/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

State game(SDL_Window *window)
{
	GLfloat vertices[] = {0,1,0, 0.5*PI,1,0, 0,0,0};

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

		glDrawArrays(GL_TRIANGLES, 0, 9);
		glDisableVertexAttribArray(0);

		if (report_GL_errors("game"))
		{
			return STATE_ERROR;
		}

		SDL_GL_SwapWindow(window);
	}

	return STATE_QUIT;
}
