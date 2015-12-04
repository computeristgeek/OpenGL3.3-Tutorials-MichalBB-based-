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

	GLboolean getLinesFromFile(string sFile, GLboolean bIncludePart, vector<string>* vResult);

	GLboolean isLoaded();
	UINT getShaderID();

	CShader();

private:
	UINT uiShader; // ID of shader
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

	UINT getProgramID();


	// Setting vectors
	GLvoid setUniform(string sName, glm::vec2* vVectors, GLint iCount = 1);
	GLvoid setUniform(string sName, const glm::vec2 vVector);
	GLvoid setUniform(string sName, glm::vec3* vVectors, GLint iCount = 1);
	GLvoid setUniform(string sName, const glm::vec3 vVector);
	GLvoid setUniform(string sName, glm::vec4* vVectors, GLint iCount = 1);
	GLvoid setUniform(string sName, const glm::vec4 vVector);

	// Setting floats
	GLvoid setUniform(string sName, float* fValues, GLint iCount = 1);
	GLvoid setUniform(string sName, const GLfloat fValue);

	// Setting 3x3 matrices
	GLvoid setUniform(string sName, glm::mat3* mMatrices, GLint iCount = 1);
	GLvoid setUniform(string sName, const glm::mat3 mMatrix);

	// Setting 4x4 matrices
	GLvoid setUniform(string sName, glm::mat4* mMatrices, GLint iCount = 1);
	GLvoid setUniform(string sName, const glm::mat4 mMatrix);

	// Setting integers
	GLvoid setUniform(string sName, int* iValues, GLint iCount = 1);
	GLvoid setUniform(string sName, const GLint iValue);

	CShaderProgram();

private:
	UINT uiProgram; // ID of program
	GLboolean bLinked; // Whether program was linked and is ready to use
};