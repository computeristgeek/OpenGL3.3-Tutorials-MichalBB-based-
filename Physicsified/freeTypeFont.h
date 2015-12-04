#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.h"
#include "shaders.h"
#include "vertexBufferObject.h"

/********************************

Class:		CFreeTypeFont

Purpose:	Wraps FreeType fonts and
			their usage with OpenGL.

********************************/

class CFreeTypeFont
{
public:
	GLboolean LoadFont(string sFile, GLint iPXSize);
	GLboolean LoadSystemFont(string sName, GLint iPXSize);

	GLint GetTextWidth(string sText, GLint iPXSize);

	GLvoid Print(string sText, GLint x, GLint y, GLint iPXSize = -1);
	GLvoid PrintFormatted(GLint x, GLint y, GLint iPXSize, char* sText, ...);

	GLvoid DeleteFont();

	GLvoid SetShaderProgram(CShaderProgram* a_shShaderProgram);

	CFreeTypeFont();
private:
	GLvoid CreateChar(GLint iIndex);

	CTexture tCharTextures[256];
	GLint iAdvX[256], iAdvY[256];
	GLint iBearingX[256], iBearingY[256];
	GLint iCharWidth[256], iCharHeight[256];
	GLint iLoadedPixelSize, iNewLine;

	GLboolean bLoaded;

	GLuint uiVAO;
	CVertexBufferObject vboData;

	FT_Library ftLib;
	FT_Face ftFace;
	CShaderProgram* shShaderProgram;
};
