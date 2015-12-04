#include "common_header.h"

#include "shaders.h"

#include <glm/gtc/type_ptr.hpp>

CShader::CShader()
{
	bLoaded = false;
}

CShader shShaders[NUMSHADERS];
CShaderProgram spMain, spOrtho2D, spFont2D, spColor;

/*-----------------------------------------------

Name:	PrepareShaderPrograms

Params:	none

Result:	Loads all shaders and creates shader programs.

/*---------------------------------------------*/

GLboolean PrepareShaderPrograms()
{
	// Load shaders and create shader program

	string sShaderFileNames[] = {"main_shader.vert", "main_shader.geom", "main_shader.frag", "ortho2D.vert",
		"ortho2D.frag", "font2D.frag", "dirLight.frag", "pointLight.frag", "fog.frag", "color.vert", "color.frag",
		"spotLight.frag"
	};

	FOR(i, NUMSHADERS)
	{
		string sExt = sShaderFileNames[i].substr(ESZ(sShaderFileNames[i])-4, 4);
		GLint iShaderType = sExt == "vert" ? GL_VERTEX_SHADER : (sExt == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER);
		shShaders[i].LoadShader("data\\shaders\\"+sShaderFileNames[i], iShaderType);
	}

	// Create shader programs

	spMain.CreateProgram();
	spMain.AddShaderToProgram(&shShaders[0]);
	spMain.AddShaderToProgram(&shShaders[1]);
	spMain.AddShaderToProgram(&shShaders[2]);
	spMain.AddShaderToProgram(&shShaders[6]);
	spMain.AddShaderToProgram(&shShaders[7]);
	spMain.AddShaderToProgram(&shShaders[11]); // NEW: Spotlight shader
	spMain.LinkProgram();

	if(!spMain.LinkProgram())return false;

	spOrtho2D.CreateProgram();
	spOrtho2D.AddShaderToProgram(&shShaders[3]);
	spOrtho2D.AddShaderToProgram(&shShaders[4]);
	spOrtho2D.LinkProgram();

	spFont2D.CreateProgram();
	spFont2D.AddShaderToProgram(&shShaders[3]);
	spFont2D.AddShaderToProgram(&shShaders[5]);
	spFont2D.LinkProgram();

	spColor.CreateProgram();
	spColor.AddShaderToProgram(&shShaders[9]);
	spColor.AddShaderToProgram(&shShaders[10]);
	spColor.LinkProgram();

	return true;
}

/*-----------------------------------------------

Name:    LoadShader

Params:  sFile - path to a file
         a_iType - type of shader (fragment, vertex, geometry)

Result:	Loads and compiles shader.

/*---------------------------------------------*/

GLboolean CShader::LoadShader(string sFile, GLint a_iType)
{
	vector<string> sLines;

	if(!GetLinesFromFile(sFile, false, &sLines))return false;

	const char** sProgram = new const char*[ESZ(sLines)];
	FOR(i, ESZ(sLines))sProgram[i] = sLines[i].c_str();
	
	uiShader = glCreateShader(a_iType);

	glShaderSource(uiShader, ESZ(sLines), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;

	GLint iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if(iCompilationStatus == GL_FALSE)
	{
		char sInfoLog[1024];
		char sFinalMessage[1536];
		GLint iLogLength;
		glGetShaderInfoLog(uiShader, 1024, &iLogLength, sInfoLog);
		sprintf(sFinalMessage, "Error! Shader file %s wasn't compiled! The compiler returned:\n\n%s", sFile.c_str(), sInfoLog);
		MessageBox(NULL, sFinalMessage, "Error", MB_ICONERROR);
		return false;
	}
	iType = a_iType;
	bLoaded = true;

	return true;
}

/*-----------------------------------------------

Name:    GetLinesFromFile

Params:  sFile - path to a file
         bIncludePart - whether to add include part only
         vResult - vector of strings to store result to

Result:  Loads and adds include part.

/*---------------------------------------------*/

GLboolean CShader::GetLinesFromFile(string sFile, GLboolean bIncludePart, vector<string>* vResult)
{
	FILE* fp = fopen(sFile.c_str(), "rt");
	if(!fp)return false;

	string sDirectory;
	GLint slashIndex = -1;
	RFOR(i, ESZ(sFile)-1)
	{
		if(sFile[i] == '\\' || sFile[i] == '/')
		{
			slashIndex = i;
			break;
		}
	}

	sDirectory = sFile.substr(0, slashIndex+1);

	// Get all lines from a file

	char sLine[255];

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
				GetLinesFromFile(sDirectory+sFileName, true, vResult);
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

Name:	UniformSetters

Params:	yes, there are :)

Result:	These set of functions set most common
		types of uniform variables.

/*---------------------------------------------*/

// Setting floats

GLvoid CShaderProgram::SetUniform(string sName, float* fValues, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}

GLvoid CShaderProgram::SetUniform(string sName, const GLfloat fValue)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

GLvoid CShaderProgram::SetUniform(string sName, glm::vec2* vVectors, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}

GLvoid CShaderProgram::SetUniform(string sName, const glm::vec2 vVector)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

GLvoid CShaderProgram::SetUniform(string sName, glm::vec3* vVectors, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

GLvoid CShaderProgram::SetUniform(string sName, const glm::vec3 vVector)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

GLvoid CShaderProgram::SetUniform(string sName, glm::vec4* vVectors, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

GLvoid CShaderProgram::SetUniform(string sName, const glm::vec4 vVector)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 3x3 matrices

GLvoid CShaderProgram::SetUniform(string sName, glm::mat3* mMatrices, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, iCount, FALSE, (GLfloat*)mMatrices);
}

GLvoid CShaderProgram::SetUniform(string sName, const glm::mat3 mMatrix)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, 1, FALSE, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

GLvoid CShaderProgram::SetUniform(string sName, glm::mat4* mMatrices, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, iCount, FALSE, (GLfloat*)mMatrices);
}

GLvoid CShaderProgram::SetUniform(string sName, const glm::mat4 mMatrix)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, 1, FALSE, (GLfloat*)&mMatrix);
}

// Setting integers

GLvoid CShaderProgram::SetUniform(string sName, int* iValues, GLint iCount)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1iv(iLoc, iCount, iValues);
}

GLvoid CShaderProgram::SetUniform(string sName, const GLint iValue)
{
	GLint iLoc = glGetUniformLocation(uiProgram, sName.c_str());
	glUniform1i(iLoc, iValue);
}
