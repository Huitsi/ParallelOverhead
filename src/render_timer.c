/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include <math.h>
#include "common.h"

//Global struct for holding the number graphics and data.
struct
{
	SDL_Surface *surface;
	SDL_Rect rects[12];
}
TimerData;

/**
 * Format and render the given time to the given surface.
 * @param target The surface to render to.
 * @param rect The area of the surface in which to render.
 * @param time_ms The time to render in milliseconds.
 */
void render_time(SDL_Surface *target, SDL_Rect rect, unsigned int time_ms)
{
	int m = time_ms/60000;
	int s = (time_ms/1000) % 60;
	int ms = time_ms % 1000;

	if (m > 99)
	{
		m = 99;
		s = 99;
		ms = 999;
	}

	const unsigned char gap = 1;

	char render_nums[9];

	render_nums[0] = m / 10;
	render_nums[1] = m % 10;
	render_nums[2] = 10; //:
	render_nums[3] = s / 10;
	render_nums[4] = s % 10;
	render_nums[5] = 11; //.
	render_nums[6] = ms / 100;
	render_nums[7] = (ms % 100) / 10;
	render_nums[8] = ms % 10;

	int cut = 0;
	if (!render_nums[0])
	{
		cut = 1;
		if (!render_nums[1])
		{
			cut = 3;
			if (!render_nums[3])
			{
				cut = 4;
			}
		}
	}

	for (int i = cut; i < 9; i++)
	{
		unsigned char num = render_nums[i];
		SDL_BlitSurface(TimerData.surface, &TimerData.rects[num], target, &rect);
		rect.x += TimerData.rects[num].w + gap;
		rect.w -= TimerData.rects[num].w + gap;
	}
}

/**
 * Render the given number to the given surface.
 * @param target The surface to render to.
 * @param rect The area of the surface in which to render.
 * @param distance The number to render.
 */
void render_distance(SDL_Surface *target, SDL_Rect rect, unsigned int distance)
{
	if (distance > 9999999)
	{
		distance = 9999999;
	}

	const unsigned char gap = 1;

	int leading = 1;
	for (int i = 7; i > 0; i--)
	{
		unsigned char num = (distance % (int) pow(10,i+1)) / pow(10,i);
		if (leading && !num)
		{
			continue;
		}
		leading = 0;
		SDL_BlitSurface(TimerData.surface, &TimerData.rects[num], target, &rect);
		rect.x += TimerData.rects[num].w + gap;
		rect.w -= TimerData.rects[num].w + gap;
	}

	unsigned char num = distance % 10;
	SDL_BlitSurface(TimerData.surface, &TimerData.rects[num], target, &rect);
}

/**
 * Render the given time and distance to the given surface.
 * @param target The surface to render to.
 * @param time_ms The time to render in milliseconds.
 * @param distance The distance to render.
 */
void render_time_and_distance(SDL_Surface *target, unsigned int time_ms, unsigned int distance)
{
	if (Settings.options.hide_counters)
	{
		return;
	}
	SDL_Rect time_rect = {0,0,100,11};
	SDL_Rect distance_rect = {0,12,100,11};

	SDL_FillRect(target, NULL, 0);

	render_time(target, time_rect, time_ms);
	render_distance(target, distance_rect, distance);
}

/**
 * Load the number graphics.
 */
void load_nums()
{
	if (Settings.options.hide_counters)
	{
		return;
	}

	TimerData.surface = SDL_LoadBMP("data/nums.bmp");
	if (!TimerData.surface)
	{
		report_SDL_error("Loading data/nums.bmp");
		//return RET_SDL_ERR;
	}

	//                    0  1  2  3  4  5  6  7  8  9  :  .
	const int widths[] = {7, 3, 7, 7, 7, 7, 7, 7, 7, 7, 3, 3};

	int x = 0;
	for (int i = 0; i < 12; i++)
	{
		SDL_Rect rect = {x, 0, widths[i], 11};
		x += rect.w;
		TimerData.rects[i] = rect;
	}
}

/**
 * Free the memory used by the number graphics.
 */
void free_nums()
{
	if (Settings.options.hide_counters)
	{
		return;
	}

	SDL_FreeSurface(TimerData.surface);
}
