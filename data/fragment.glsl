#version 320 es
/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
precision mediump float;

uniform sampler2D tex;
layout(location = 3) uniform vec2 texture_area;
in vec3 fragment_pos_polar;
out vec4 color;

void main()
{
	vec2 tex_pos = vec2(fragment_pos_polar.y/texture_area.x, fragment_pos_polar.z/texture_area.y);
	color = texture(tex, tex_pos);
}
