#version 320 es
/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
precision mediump float;

layout(location = 0) in vec3 vertex_pos_polar;
layout(location = 0) uniform vec2 screen_wh;
layout(location = 1) uniform vec3 object_pos_polar;
out vec3 fragment_pos_polar;

void main()
{
	vec3 pos = vertex_pos_polar + object_pos_polar;
	float ratio = sqrt(screen_wh.x/screen_wh.y);

	gl_Position = vec4(cos(pos.y)*pos.x/ratio, sin(pos.y)*pos.x*ratio, pos.z, pos.z);
	fragment_pos_polar = vertex_pos_polar;
}
