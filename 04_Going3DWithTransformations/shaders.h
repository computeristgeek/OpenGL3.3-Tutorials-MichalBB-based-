#pragma once

/********************************

Class:	CShader

Purpose:	Wraps OpenGL shader loading
			and compiling.

********************************/

class CShader
{
public:
	GLboolean loadShader(string sFile, GLint a_iType);
	GLvoid deleteShader();

	GLboolean isLoaded();
	GLuint getShaderID();

	CShader();

private:
	GLuint uiShader; // ID of shader
	GLint iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	GLboolean bLoaded; // Whether shader was loaded and compiled
};

/********************************

Class:	CShaderProgram

Purpose:	Wraps OpenGL shader program
			and make its usage easy.

********************************/

class CShaderProgram
{
public:
	GLvoid createProgram();
	GLvoid deleteProgram();

	GLboolean addShaderToProgram(CShader* shShader);
	GLboolean linkProgram();

	GLvoid useProgram();

	GLuint getProgramID();

	CShaderProgram();

private:
	GLuint uiProgram; // ID of program
	GLboolean bLinked; // Whether program was linked and is ready to use
};