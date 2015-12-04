#pragma once

/********************************

Class:		CShader

Purpose:	Wraps OpenGL shader loading
			and compiling.

********************************/

class CShader
{
public:
	GLboolean LoadShader(string sFile, GLint a_iType);
	GLvoid DeleteShader();

	GLboolean GetLinesFromFile(string sFile, GLboolean bIncludePart, vector<string>* vResult);

	GLboolean IsLoaded();
	GLuint GetShaderID();

	CShader();

private:
	GLuint uiShader; // ID of shader
	GLint iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	GLboolean bLoaded; // Whether shader was loaded and compiled
};

/********************************

Class:		CShaderProgram

Purpose:	Wraps OpenGL shader program
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

	GLuint GetProgramID();

	// Setting vectors
	GLvoid SetUniform(string sName, glm::vec2* vVectors, GLint iCount = 1);
	GLvoid SetUniform(string sName, const glm::vec2 vVector);
	GLvoid SetUniform(string sName, glm::vec3* vVectors, GLint iCount = 1);
	GLvoid SetUniform(string sName, const glm::vec3 vVector);
	GLvoid SetUniform(string sName, glm::vec4* vVectors, GLint iCount = 1);
	GLvoid SetUniform(string sName, const glm::vec4 vVector);

	// Setting floats
	GLvoid SetUniform(string sName, float* fValues, GLint iCount = 1);
	GLvoid SetUniform(string sName, const GLfloat fValue);

	// Setting 3x3 matrices
	GLvoid SetUniform(string sName, glm::mat3* mMatrices, GLint iCount = 1);
	GLvoid SetUniform(string sName, const glm::mat3 mMatrix);

	// Setting 4x4 matrices
	GLvoid SetUniform(string sName, glm::mat4* mMatrices, GLint iCount = 1);
	GLvoid SetUniform(string sName, const glm::mat4 mMatrix);

	// Setting integers
	GLvoid SetUniform(string sName, int* iValues, GLint iCount = 1);
	GLvoid SetUniform(string sName, const GLint iValue);

	CShaderProgram();

private:
	GLuint uiProgram; // ID of program
	GLboolean bLinked; // Whether program was linked and is ready to use
};