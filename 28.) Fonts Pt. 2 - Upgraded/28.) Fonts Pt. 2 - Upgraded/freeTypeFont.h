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
	bool LoadFont(string sFile, int iPXSize, int iMaxCharSupport = 128);
	bool LoadSystemFont(string sName, int iPXSize, int iMaxCharSupport = 128);

	int GetTextWidth(string sText, int iPXSize);

	void Print(string sText, int x, int y, int iPXSize = -1);
	void Print(wstring sText, int x, int y, int iPXSize = -1);
	void PrintFormatted(int x, int y, int iPXSize, char* sText, ...);
	void PrintFormatted(int x, int y, int iPXSize, wchar_t* sText, ...);

	void DeleteFont();

	void SetShaderProgram(CShaderProgram* a_shShaderProgram);

	CFreeTypeFont();
private:
	void CreateChar(int iIndex, GLubyte* bData);

	vector<CTexture> tCharTextures;
	vector<int> iAdvX, iAdvY;
	vector<int> iBearingX, iBearingY;
	vector<int> iCharWidth, iCharHeight;
	int iLoadedPixelSize, iNewLine;
	int iOneCharSquareSize;

	bool bLoaded;

	UINT uiVAO;
	CVertexBufferObject vboData;

	FT_Library ftLib;
	FT_Face ftFace;
	CShaderProgram* shShaderProgram;
};
