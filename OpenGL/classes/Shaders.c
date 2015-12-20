#include "Shaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GL/gl.h>

static char* loadFileToString(const char* filename)
{
	char* buffer = NULL;
	FILE* sf = fopen(filename, "r");

	if(sf != NULL)
	{
		fseek(sf, 0, SEEK_END);
		int length = ftell(sf);

		fseek(sf, 0, SEEK_SET);
		buffer = malloc(length);

		fread(buffer, 1, length, sf);
		fclose(sf);
	}
	return buffer;
}

unsigned int loadShaderProgram(const char* vsFilename, const char* fsFilename)
{
	unsigned int id_p, vs, fs;

	const char* fragShader = (const char*)loadFileToString(fsFilename);
	const char* vertShader = (const char*)loadFileToString(vsFilename);

	int compile_ok = 0;

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertShader, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);

	if(compile_ok == 0)
	{
		GLsizei slen = 0;
		GLint blen = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH , &blen);
		fprintf(stderr, "Error in vertex shader:\n");
		GLchar* compiler_log = (GLchar*)malloc(blen);
		glGetShaderInfoLog(vs, blen, &slen, compiler_log);
		fprintf(stderr, "%s\n", compiler_log);
		free(compiler_log);
		exit(1);
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragShader, NULL);
	glCompileShader(fs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);

	if(compile_ok == 0)
	{
		GLsizei slen = 0;
		GLint blen = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH , &blen);
		fprintf(stderr, "Error in fragment shader:\n");
		GLchar* compiler_log = (GLchar*)malloc(blen);
		glGetShaderInfoLog(fs, blen, &slen, compiler_log);
		fprintf(stderr, "%s\n", compiler_log);
		free(compiler_log);
		exit(1);
	}

	id_p = glCreateProgram();
	glAttachShader(id_p, vs);
	glAttachShader(id_p, fs);
	glBindFragDataLocation(id_p, 0, "FragColor");
	glLinkProgram(id_p);

	return id_p;
}

