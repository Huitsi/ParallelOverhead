#version 320 es
/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
precision mediump float;
layout(location = 2) in vec3 fragment_pos_polar;
layout(location = 0) out vec4 color;
void main()
{
	color = vec4(1.0,0.0,0.0,1.0);
}
