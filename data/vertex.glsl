#version 320 es
/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
layout(location = 0) in vec3 vertex_pos_polar;
layout(location = 0) uniform vec3 object_pos_polar;
layout(location = 1) uniform vec2 screen_wh;
out vec3 fragment_pos_polar;
void main()
{
	float radius = vertex_pos_polar.x + object_pos_polar.x;
	float angle = vertex_pos_polar.y + object_pos_polar.y;
	float depth = vertex_pos_polar.z + object_pos_polar.z;
	float ratio = sqrt(screen_wh.x/screen_wh.y);

	gl_Position = vec4(cos(angle)*radius/ratio, sin(angle)*radius*ratio, depth, depth);
	fragment_pos_polar = vertex_pos_polar;
}
