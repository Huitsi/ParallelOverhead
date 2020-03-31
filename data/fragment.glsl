#version 320 es
/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
precision mediump float;
in vec3 fragment_pos_polar;
out vec4 color;
void main()
{
	color = vec4(fragment_pos_polar.x,fragment_pos_polar.y/6.28,fragment_pos_polar.z,1.0);
}
