#version 320 es
/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
layout(location = 0) in vec3 vertex_pos_polar;
uniform vec3 object_pos_polar;
out vec3 fragment_pos_polar;
void main()
{
	float angle = vertex_pos_polar.x + object_pos_polar.x;
	float radius = vertex_pos_polar.y + object_pos_polar.y;
	float depth = vertex_pos_polar.z + object_pos_polar.z;

	gl_Position = vec4(cos(angle)*radius, sin(angle)*radius, depth, 1.0);
	fragment_pos_polar = vertex_pos_polar;
}
