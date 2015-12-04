#include "common_header.h"

#include "shaders.h"

#include <glm/gtc/type_ptr.hpp>

CShader::CShader()
{
	bLoaded = false;
}

/*-----------------------------------------------

Name:	loadShader

Params:	sFile - path to a file
		a_iType - type of shader (fragment, vertex, geometry)

Result:	Loads and compiles shader.

/*---------------------------------------------*/

GLboolean CShader::loadShader(string sFile, GLint a_iType)
{
	FILE* fp = fopen(sFile.c_str(), "rt");
	if(!fp)return false;

	// Get all lines from a file

	vector<string> sLines;
	char sLine[255];
	while(fgets(sLine, 255, fp))sLines.push_back(sLine);
	fclose(fp);

	const char** sProgram = new const char*[ESZ(sLines)];
	FOR(i, ESZ(sLines))sProgram[i] = sLines[i].c_str();
	
	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader, ESZ(sLines), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;

	GLint iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if(iCompilationStatus == GL_FALSE)return false;
	iType = a_iType;
	bLoaded = true;

	return 1;
}

/*-----------------------------------------------

Name:	isLoaded

Params:	none

Result:	True if shader was loaded and compiled.

/*---------------------------------------------*/

GLboolean CShader::isLoaded()
{
	return bLoaded;
}

/*-----------------------------------------------

Name:	getShaderID

Params:	none

Result:	Returns ID of a generated shader.

/*---------------------------------------------*/

UINT CShader::getShaderID()
{
	return uiShader;
}

/*-----------------------------------------------

Name:	deleteShader

Params:	none

Result:	Deletes shader and frees memory in GPU.

/*---------------------------------------------*/

GLvoid CShader::deleteShader()
{
	if(!isLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}

CShaderProgram::CShaderProgram()
{
	bLinked = false;
}

/*-----------------------------------------------

Name:	createProgram

Params:	none

Result:	Creates a new program.

/*---------------------------------------------*/

GLvoid CShaderProgram::createProgram()
{
	uiProgram = glCreateProgram();
}

/*-----------------------------------------------

Name:	addShaderToProgram

Params:	sShader - shader to add

Result:	Adds a shader (like source file) to
		a program, but only compiled one.

/*---------------------------------------------*/

GLboolean CShaderProgram::addShaderToProgram(CShader* shShader)
{
	if(!shShader->isLoaded())return false;

	glAttachShader(uiProgram, shShader->getShaderID());

	return true;
}

/*-----------------------------------------------

Name:	linkProgram

Params:	none

Result:	Performs final linkage of OpenGL program.

/*---------------------------------------------*/

GLboolean CShaderProgram::linkProgram()
{
	glLinkProgram(uiProgram);
	GLint iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}

/*-----------------------------------------------

Name:	deleteProgram

Params:	none

Result:	Deletes program and frees memory on GPU.

/*---------------------------------------------*/

GLvoid CShaderProgram::deleteProgram()
{
	if(!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}

/*-----------------------------------------------

Name:	useProgram

Params:	none

Result:	Tells OpenGL to use this program.

/*---------------------------------------------*/

GLvoid CShaderProgram::useProgram()
{
	if(bLinked)glUseProgram(uiProgram);
}

/*-----------------------------------------------

Name:		getProgramID

Params:	none

Result:	Returns OpenGL generated shader program ID.

/*---------------------------------------------*/

UINT CShaderProgram::getProgramID()
{
	return uiProgram;
}

/*-----------------------------------------------

Name:		uniformSetters

Params:	yes, there are :)

Result:	These set of functions set most common
			uniform variables.

/*---------------------------------------------*/

// Setting floats

GLvoid CShaderProgram::setUniform(string sName, float* fValues, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}

GLvoid CShaderProgram::setUniform(string sName, const GLfloat fValue)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

GLvoid CShaderProgram::setUniform(string sName, glm::vec2* vVectors, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}

GLvoid CShaderProgram::setUniform(string sName, const glm::vec2 vVector)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

GLvoid CShaderProgram::setUniform(string sName, glm::vec3* vVectors, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

GLvoid CShaderProgram::setUniform(string sName, const glm::vec3 vVector)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

GLvoid CShaderProgram::setUniform(string sName, glm::vec4* vVectors, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

GLvoid CShaderProgram::setUniform(string sName, const glm::vec4 vVector)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 3x3 matrices

GLvoid CShaderProgram::setUniform(string sName, glm::mat3* mMatrices, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, iCount, FALSE, (GLfloat*)mMatrices);
}

GLvoid CShaderProgram::setUniform(string sName, const glm::mat3 mMatrix)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, 1, FALSE, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

GLvoid CShaderProgram::setUniform(string sName, glm::mat4* mMatrices, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, iCount, FALSE, (GLfloat*)mMatrices);
}

GLvoid CShaderProgram::setUniform(string sName, const glm::mat4 mMatrix)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, 1, FALSE, (GLfloat*)&mMatrix);
}

// Setting integers

GLvoid CShaderProgram::setUniform(string sName, int* iValues, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1iv(iLoc, iCount, iValues);
}

GLvoid CShaderProgram::setUniform(string sName, const GLint iValue)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1i(iLoc, iValue);
}