/* Copyright © 2020 Linus Vanas <linus@vanas.fi>
 * SPDX-License-Identifier: MIT
 */
#include "common.h"

/**
 * Print the latest OpenGL errors, if any.
 * @param context A context string for the error.
 * @return The amount of erros reported.
 */
int report_GL_errors(char *context)
{
	int amount = -1;
	GLenum err;
	do
	{
		err = glGetError();
		amount++;
	}
	while (err != GL_NO_ERROR);

	if (amount)
	{
		fprintf(stderr, "%s: %d OpenGL error(s) encountered.\n", context, amount);
	}

	return amount;
}

/**
 * Load the contents of a file into a string.
 * @param path The path of the file in the filesystem.
 * @return A string with the contents of the file. free() after use.
 */
char *load_file(char *path)
{
	//Open the file.
	FILE *f = fopen(path,"r");
	if (f == NULL)
	{
		perror(path);
		return NULL;
	}

	//Find the lenght of the file.
	if(fseek(f, 0L, SEEK_END))
	{
		perror(path);
		return NULL;
	}

	long len = ftell(f);

	if (len < 0L)
	{
		perror(path);
		return NULL;
	}

	//Read the file into a string.
	rewind(f);

	char *s = malloc(sizeof (char) * (len+1));
	if(!fread(s,len,1,f))
	{
		fclose(f);
		perror(path);
		return NULL;
	}

	fclose(f);

	//Null-terminate and return the string.
	s[len] = 0;
	return s;
}

/**
 * Load from a file and compile the given shader.
 * @param shader_file_path The path of the shader file in the filesystem.
 * @param type The type of the shader, eg. GL_VERTEX_SHADER.
 * @return A GLuint representing the shader, or 0 on failure.
 */
GLuint make_shader(char *shader_file_path, GLenum type)
{
	char *code = load_file(shader_file_path);
	const GLchar *source[1] = {code};

	if (source == NULL)
	{
		free(code);
		return 0;
	}

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, source, NULL);
	free(code);
	glCompileShader(shader);

	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status)
	{
		return shader;
	}

	GLint len = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

	GLchar *log = malloc(sizeof (GLchar) * len);
	glGetShaderInfoLog(shader, len, NULL, log);

	report_GL_errors("make_shader");
	fprintf(stderr, "%s:\n", shader_file_path);
	fprintf(stderr, log);
	free(log);
	return 0;
}

/**
 * Load from files, compile and link the given shaders into a program.
 * @param vertex_shader_file_path The path of the vertex shader file in the filesystem.
 * @param fragment_shader_file_path The path of the fragment shader file in the filesystem.
 * @return A GLuint representing the program, or 0 on failure.
 */
GLuint make_program(char *vertex_shader_file_path, char *fragment_shader_file_path)
{
	GLuint vertex_shader = make_shader(vertex_shader_file_path, GL_VERTEX_SHADER);
	if (!vertex_shader)
	{
		return 0;
	}

	GLuint fragment_shader = make_shader(fragment_shader_file_path, GL_FRAGMENT_SHADER);
	if (!fragment_shader)
	{
		return 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status)
	{
		return program;
	}

	GLint len = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

	GLchar *log = malloc(sizeof (GLchar) * len);
	glGetProgramInfoLog(program, len, NULL, log);

	report_GL_errors("make_program");
	fprintf(stderr, log);
	free(log);
	return 0;
}

struct
{
	GLint vertex_pos_polar, screen_wh, object_pos_polar, texture_area;
}
Locs;

/**
 * Initialize OpenGL and then proceed to the game.
 * Cleanup OpenGL after the game is closed.
 * @param window The game window.
 */
int init_GL(SDL_Window *window)
{
	char vertex_path[Settings.paths.data_dir_len + sizeof "vertex.glsl"];
	snprintf(vertex_path, sizeof vertex_path, "%s%s", Settings.paths.data_dir, "vertex.glsl");

	char fragment_path[Settings.paths.data_dir_len + sizeof "fragment.glsl"];
	snprintf(fragment_path, sizeof fragment_path, "%s%s", Settings.paths.data_dir, "fragment.glsl");

	GLuint program = make_program(vertex_path, fragment_path);
	if(!program)
	{
		return RET_GL_ERR;
	}
	glUseProgram(program);
	if (report_GL_errors("make_program"))
	{
		return RET_GL_ERR;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	if (report_GL_errors("eanble blend"))
	{
		return RET_GL_ERR;
	}

	Locs.vertex_pos_polar = glGetAttribLocation(program, "vertex_pos_polar");
	Locs.screen_wh = glGetUniformLocation(program, "screen_wh");
	Locs.object_pos_polar = glGetUniformLocation(program, "object_pos_polar");
	Locs.texture_area = glGetUniformLocation(program, "texture_area");

	int ret = init_game(window);

	glDeleteProgram(program);

	return ret;
}
