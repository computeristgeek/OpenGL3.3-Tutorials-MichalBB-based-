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

#define CHARS_PER_TEXTURE 1024
#define CHARS_PER_TEXTUREROOT 32

class CFreeTypeFont
{
public:
	GLboolean LoadFont(string sFile, GLint iPXSize, GLint iMaxCharSupport = 128);
	GLboolean LoadSystemFont(string sName, GLint iPXSize, GLint iMaxCharSupport = 128);

	GLint GetTextWidth(string sText, GLint iPXSize);

	GLvoid Print(string sText, GLint x, GLint y, GLint iPXSize = -1);
	GLvoid Print(wstring sText, GLint x, GLint y, GLint iPXSize = -1);
	GLvoid PrintFormatted(GLint x, GLint y, GLint iPXSize, char* sText, ...);
	GLvoid PrintFormatted(GLint x, GLint y, GLint iPXSize, wchar_t* sText, ...);

	GLvoid DeleteFont();

	GLvoid SetShaderProgram(CShaderProgram* a_shShaderProgram);

	CFreeTypeFont();
private:
	GLvoid CreateChar(GLint iIndex, GLubyte* bData);

	vector<CTexture> tCharTextures;
	vector<int> iAdvX, iAdvY;
	vector<int> iBearingX, iBearingY;
	vector<int> iCharWidth, iCharHeight;
	GLint iLoadedPixelSize, iNewLine;
	GLint iOneCharSquareSize;

	GLboolean bLoaded;

	GLuint uiVAO;
	CVertexBufferObject vboData;

	FT_Library ftLib;
	FT_Face ftFace;
	CShaderProgram* shShaderProgram;
};
