/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"
#include "game.h"
#include "audio.h"
#include "level.h"

int run_game(SDL_Window *window, GLfloat vertices[], GLuint textures[], SDL_Surface *timer_surface)
{
	GLuint ship_texture = textures[0];
	GLuint wall_texture = textures[1];
	GLuint timer_texture = textures[2];
	const float sector_angle = FULL_ANGLE/Settings.game.sectors;
	struct
	{
		int alive;
		int sector;
	}
	ships[Settings.game.ships];
	for (int i = 0; i < Settings.game.ships; i++)
	{
		ships[i].alive = 1;
		ships[i].sector = Settings.game.start_sector + i * (Settings.game.sectors / Settings.game.ships);
	}

	reset_level();
	//Wall texture
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	float wall_texture_data[(Settings.game.rings + Settings.transitions.max) * Settings.game.sectors * 4];
	int rings_generated = generate_rings(wall_texture_data);
	while (rings_generated < Settings.game.rings)
	{
		rings_generated += generate_rings
		(
			&wall_texture_data[rings_generated * Settings.game.sectors * 4]
		);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Settings.game.sectors, Settings.game.rings, 0, GL_RGBA, GL_FLOAT, wall_texture_data);

	reset_music();

	float ship_ring = 0;
	unsigned int last_tick_time = 0;
	unsigned int time_survived = 0;
	unsigned int rings_survived = 0;
	unsigned int pauses = 0;
	char paused = 1;
	int ships_alive = 0;

	while (1)
	{
		unsigned int  tick_start_time =  SDL_GetTicks();
		if (last_tick_time > Settings.tick_time.max)
		{
			last_tick_time = Settings.tick_time.max;
		}
		else if (last_tick_time < Settings.tick_time.min)
		{
			SDL_Delay(Settings.tick_time.min - last_tick_time);
			last_tick_time = Settings.tick_time.min;
		}

		int w,h, ship_sector_delta = 0;
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
						case SDLK_RETURN:
						case SDLK_SPACE:
							paused = !paused;
							pauses++;
							if (ships_alive)
							{
								break;
							}
						case SDLK_BACKSPACE:
							return 1;
						case SDLK_ESCAPE:
							return 0;

					}
					break;
			}
		}

		float speed = sqrt(time_survived)*Settings.difficulty.speed;

		if (paused || !ships_alive)
		{
			ship_sector_delta = 0;
			pause_music(1);
		}
		else
		{
			time_survived += last_tick_time;
			ship_ring += last_tick_time*speed;
			pause_music(0);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		//Update and render walls
		glBindTexture(GL_TEXTURE_2D, wall_texture);
		while(ship_ring > 1)
		{
			memmove
			(
				wall_texture_data,
				&wall_texture_data[Settings.game.sectors * 4],
				sizeof(wall_texture_data) - Settings.game.sectors*4*sizeof(float)
			);
			rings_generated--;
			ship_ring--;
			rings_survived++;

			while (rings_generated < Settings.game.rings)
			{
				rings_generated += generate_rings
				(
					&wall_texture_data[rings_generated * Settings.game.sectors * 4]
				);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Settings.game.sectors, Settings.game.rings, 0, GL_RGBA, GL_FLOAT, wall_texture_data);
		}

		glUniform3f(LOC_POS, 0, 0, -ship_ring);
		glUniform2f(LOC_TEX_AREA, FULL_ANGLE,  Settings.game.rings);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4+1+3*Settings.game.sectors);

		//Update and render ships
		glBindTexture(GL_TEXTURE_2D, ship_texture);
		glUniform2f(LOC_TEX_AREA, sector_angle, sector_angle);

		if (ship_sector_delta)
		{
			play_move_sound();
		}

		ships_alive = 0;
		for (int i = 0; i < Settings.game.ships; i++)
		{
			if (ships[i].alive)
			{
				//ships[i].sector = (ships[i].sector + ship_sector_delta) % Settings.game.sectors;
				ships[i].sector += ship_sector_delta;
				ships[i].sector %= Settings.game.sectors;

				if (!wall_texture_data[Settings.game.sectors*4 + ships[i].sector*4 + 3])
				{
					ships[i].alive = 0;
					play_death_sound();
					continue;
				}

				ships_alive++;

				glUniform3f(LOC_POS, 0, ships[i].sector * sector_angle, 1);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}

		//Render timer
		render_time_and_distance(timer_surface, time_survived, rings_survived);
		glBindTexture(GL_TEXTURE_2D, timer_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timer_surface->w, timer_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, timer_surface->pixels);

		glUniform3f(LOC_POS, 1, Settings.timer.sector * sector_angle, Settings.timer.depth);
		glUniform2f(LOC_TEX_AREA, -sector_angle, sector_angle);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		SDL_GL_SwapWindow(window);

		last_tick_time = SDL_GetTicks() - tick_start_time;
	}

	printf("Survived for %f s (%u rings). Paused %u times.\n", time_survived/1000., rings_survived, pauses/2);

	return 1;
}
