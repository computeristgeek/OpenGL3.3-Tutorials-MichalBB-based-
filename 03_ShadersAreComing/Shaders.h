#pragma once

/********************************

Class:	CShader

Purpose:	Wraps OpenGL shader loading
			and compiling.

********************************/

class CShader
{
public:
	GLboolean LoadShader(string sFile, GLint a_iType);
	GLvoid DeleteShader();

	GLboolean IsLoaded();
	UINT GetShaderID();

	CShader();

private:
	UINT uiShader; // ID of shader
	GLint iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	GLboolean bLoaded; // Whether shader was loaded and compiled
};

/********************************

Class:	CShaderProgram

Purpose: Wraps OpenGL shader program
		and make its usage easy.

********************************/

class CShaderProgram
{
public:
	GLvoid CreateProgram();
	GLvoid DeleteProgram();

	GLboolean AddShaderToProgram(CShader* shShader);
	GLboolean LinkProgram();

	GLvoid UseProgram();

	UINT GetProgramID();

	CShaderProgram();

private:
	UINT uiProgram; // ID of program
	GLboolean bLinked; // Whether program was linked and is ready to use
};