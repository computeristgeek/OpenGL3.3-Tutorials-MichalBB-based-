// file version 1.2 (22.06.2011)

#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

//=========== CFREETYPEFONT =============
//
// Provides printing of ASCII text (no Unicode).
//
//=========== CFREETYPEFONT =============

class CFreeTypeFont
{
public:
	bool loadFont(string sFile, int iPXSize);
	void loadSystemFont(string sName, int iPXSize);
	int getTextWidth(string sText, int iPXSize);
	void print(string sText, int x, int y, int iPXSize, bool bStartOrtho = false);

	void releaseFont();

	CFreeTypeFont();
private:

	void createChar(FT_Face &ftFace, int iIndex);

	UINT uiFontTex[256];
	int iAdvX[256], iAdvY[256];
	int iBearingX[256];
	int iCharW[256], iCharH[256];
	int iLoadedPixelSize, iNewLine;

	bool bLoaded;
};

extern CFreeTypeFont ftMenuFont;
extern CFreeTypeFont ftMainFont;