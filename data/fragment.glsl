#version 320 es
/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#define PI 3.14159265358979323846
precision mediump float;

uniform sampler2D tex;
layout(location = 3) uniform float rings;
in vec3 fragment_pos_polar;
out vec4 color;

void main()
{
	vec2 tex_pos = vec2(fragment_pos_polar.y/(2.0*PI), fragment_pos_polar.z/rings);
	color = texture(tex, tex_pos);
}
