/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
void init_hud();
void free_hud();

void render_time_and_distance(SDL_Surface *target, unsigned int time_ms, unsigned int distance);
