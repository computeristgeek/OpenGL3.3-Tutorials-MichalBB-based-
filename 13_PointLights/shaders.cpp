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

GLboolean CShader::LoadShader(string sFile, GLint a_iType)
{
	vector<string> sLines;

	if(!getLinesFromFile(sFile, false, &sLines))return false;

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

	return true;
}

/*-----------------------------------------------

Name:    getLinesFromFile

Params:  sFile - path to a file
         bIncludePart - whether to add include part only
         vResult - vector of strings to store result to

Result:  Loads and adds include part.

/*---------------------------------------------*/

GLboolean CShader::getLinesFromFile(string sFile, GLboolean bIncludePart, vector<string>* vResult)
{
	FILE* fp = fopen(sFile.c_str(), "r");
	if(!fp)return false;

	string sDirectory;
	GLint slashIndex = -1;
	RFOR(i, ESZ(sFile)-1)
	{
		if(sFile[i] == '/' || sFile[i] == '/')
		{
			slashIndex = i;
			break;
		}
	}

	sDirectory = sFile.substr(0, slashIndex+1);

	// Get all lines from a file

	GLchar sLine[255];

	GLboolean bInIncludePart = false;

	while(fgets(sLine, 255, fp))
	{
		stringstream ss(sLine);
		string sFirst;
		ss >> sFirst;
		if(sFirst == "#include")
		{
			string sFileName;
			ss >> sFileName;
			if(ESZ(sFileName) > 0 && sFileName[0] == '\"' && sFileName[ESZ(sFileName)-1] == '\"')
			{
				sFileName = sFileName.substr(1, ESZ(sFileName)-2);
				getLinesFromFile(sDirectory+sFileName, true, vResult);
			}
		}
		else if(sFirst == "#include_part")
			bInIncludePart = true;
		else if(sFirst == "#definition_part")
			bInIncludePart = false;
		else if(!bIncludePart || (bIncludePart && bInIncludePart))
			vResult->push_back(sLine);
	}
	fclose(fp);

	return true;
}

/*-----------------------------------------------

Name:	IsLoaded

Params:	none

Result:	True if shader was loaded and compiled.

/*---------------------------------------------*/

GLboolean CShader::IsLoaded()
{
	return bLoaded;
}

/*-----------------------------------------------

Name:	GetShaderID

Params:	none

Result:	Returns ID of a generated shader.

/*---------------------------------------------*/

GLuint CShader::GetShaderID()
{
	return uiShader;
}

/*-----------------------------------------------

Name:	DeleteShader

Params:	none

Result:	Deletes shader and frees memory in GPU.

/*---------------------------------------------*/

GLvoid CShader::DeleteShader()
{
	if(!IsLoaded())return;
	bLoaded = false;
	glDeleteShader(uiShader);
}

CShaderProgram::CShaderProgram()
{
	bLinked = false;
}

/*-----------------------------------------------

Name:	CreateProgram

Params:	none

Result:	Creates a new program.

/*---------------------------------------------*/

GLvoid CShaderProgram::CreateProgram()
{
	uiProgram = glCreateProgram();
}

/*-----------------------------------------------

Name:	AddShaderToProgram

Params:	sShader - shader to add

Result:	Adds a shader (like source file) to
		a program, but only compiled one.

/*---------------------------------------------*/

GLboolean CShaderProgram::AddShaderToProgram(CShader* shShader)
{
	if(!shShader->IsLoaded())return false;

	glAttachShader(uiProgram, shShader->GetShaderID());

	return true;
}

/*-----------------------------------------------

Name:	LinkProgram

Params:	none

Result:	Performs final linkage of OpenGL program.

/*---------------------------------------------*/

GLboolean CShaderProgram::LinkProgram()
{
	glLinkProgram(uiProgram);
	GLint iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return bLinked;
}

/*-----------------------------------------------

Name:	DeleteProgram

Params:	none

Result:	Deletes program and frees memory on GPU.

/*---------------------------------------------*/

GLvoid CShaderProgram::DeleteProgram()
{
	if(!bLinked)return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}

/*-----------------------------------------------

Name:	UseProgram

Params:	none

Result:	Tells OpenGL to use this program.

/*---------------------------------------------*/

GLvoid CShaderProgram::UseProgram()
{
	if(bLinked)glUseProgram(uiProgram);
}

/*-----------------------------------------------

Name:	GetProgramID

Params:	none

Result:	Returns OpenGL generated shader program ID.

/*---------------------------------------------*/

GLuint CShaderProgram::GetProgramID()
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
	glUniformMatrix3fv(iLoc, iCount, GL_FALSE, (GLfloat*)mMatrices);
}

GLvoid CShaderProgram::setUniform(string sName, const glm::mat3 mMatrix)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, 1, GL_FALSE, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

GLvoid CShaderProgram::setUniform(string sName, glm::mat4* mMatrices, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, iCount, GL_FALSE, (GLfloat*)mMatrices);
}

GLvoid CShaderProgram::setUniform(string sName, const glm::mat4 mMatrix)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, 1, GL_FALSE, (GLfloat*)&mMatrix);
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
