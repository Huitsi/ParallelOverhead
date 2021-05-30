/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"
#include "game.h"
#include "audio.h"
#include "level.h"
#include "hud.h"

int run_game(SDL_Window *window, GLfloat vertices[], GLuint textures[], SDL_Surface *timer_surface)
{
	GLuint ship_texture = textures[0];
	GLuint wall_texture = textures[1];
	GLuint timer_texture = textures[2];
	const float sector_angle = FULL_ANGLE/Settings.tunnel.sectors;

	reset_level();
	reset_music();

	struct
	{
		int alive;
		int sector;
	}
	ships[Settings.ships.amount];
	for (int i = 0; i < Settings.ships.amount; i++)
	{
		ships[i].alive = 1;
		ships[i].sector = Settings.ships.sector_offset + i * (Settings.tunnel.sectors / Settings.ships.amount);
	}

	//Wall texture
	glBindTexture(GL_TEXTURE_2D, wall_texture);
	unsigned char wall_texture_data[(Settings.tunnel.length + Settings.tunnel.color_transition_length.max) * Settings.tunnel.sectors * 4];
	int rings_generated = generate_rings(wall_texture_data);
	while (rings_generated < Settings.tunnel.length)
	{
		rings_generated += generate_rings
		(
			&wall_texture_data[rings_generated * Settings.tunnel.sectors * 4]
		);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Settings.tunnel.sectors, Settings.tunnel.length, 0, GL_RGBA, GL_UNSIGNED_BYTE, wall_texture_data);

	float ship_pos = 0;
	unsigned int last_tick_time = 0;
	unsigned int time_survived = 0;
	unsigned int rings_survived = 0;
	char paused = 1;
	int ships_alive = Settings.ships.amount;

	if (!Settings.flags.quiet)
	{
		printf("Starting a new run.\n");
	}

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
					glUniform2f(Locs.screen_wh, w, h);
					glViewport(0,0,w,h);
					break;
				case SDL_KEYDOWN:
					if (e.key.repeat)
					{
						break;
					}
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
			ship_pos += last_tick_time*speed;
			pause_music(0);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		//Update and render walls
		glBindTexture(GL_TEXTURE_2D, wall_texture);
		while(ship_pos > Settings.tunnel.section_length)
		{
			memmove
			(
				wall_texture_data,
				&wall_texture_data[Settings.tunnel.sectors * 4],
				sizeof(wall_texture_data) - Settings.tunnel.sectors*4*sizeof(float)
			);
			rings_generated--;
			ship_pos -= Settings.tunnel.section_length;
			rings_survived++;

			while (rings_generated < Settings.tunnel.length)
			{
				rings_generated += generate_rings
				(
					&wall_texture_data[rings_generated * Settings.tunnel.sectors * 4]
				);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Settings.tunnel.sectors, Settings.tunnel.length, 0, GL_RGBA, GL_UNSIGNED_BYTE, wall_texture_data);
		}

		glUniform3f(Locs.object_pos_polar, 0, 0, -ship_pos);
		glUniform2f(Locs.texture_area, FULL_ANGLE,  Settings.tunnel.length * Settings.tunnel.section_length);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, (2*Settings.tunnel.sectors + 2));

		//Update and render ships
		glBindTexture(GL_TEXTURE_2D, ship_texture);
		glUniform2f(Locs.texture_area, sector_angle, sector_angle);

		if (ship_sector_delta)
		{
			play_move_sound();
		}

		ships_alive = 0;
		for (int i = 0; i < Settings.ships.amount; i++)
		{
			if (ships[i].alive)
			{
				ships[i].sector += ship_sector_delta;
				ships[i].sector %= Settings.tunnel.sectors;
				if (ships[i].sector < 0)
				{
					ships[i].sector += Settings.tunnel.sectors;
				}

				if (!wall_texture_data[Settings.tunnel.sectors*4*Settings.ships.depth + ships[i].sector*4 + 3])
				{
					ships[i].alive = 0;
					increase_difficulty();
					play_death_sound();
					if (!Settings.flags.quiet)
					{
						printf("Ship lost after %.3f s (a distance of %u rings).\n", time_survived/1000., rings_survived);
					}
					continue;
				}

				ships_alive++;

				glUniform3f(Locs.object_pos_polar, 0, ships[i].sector * sector_angle, Settings.ships.depth * Settings.tunnel.section_length);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}

		//Render timer
		render_time_and_distance(timer_surface, time_survived, rings_survived);
		glBindTexture(GL_TEXTURE_2D, timer_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timer_surface->w, timer_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, timer_surface->pixels);

		glUniform3f(Locs.object_pos_polar, 1, Settings.hud.sector * sector_angle, Settings.hud.depth);
		glUniform2f(Locs.texture_area, -sector_angle, sector_angle);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		SDL_GL_SwapWindow(window);

		last_tick_time = SDL_GetTicks() - tick_start_time;
	}

	return 1;
}
