
#include "common_header.h"
#include "win_OpenGLApp.h"

#include "FreeTypeFont.h"

#ifdef _DEBUG
#pragma comment(lib, "freetype249_d.lib")
#else
#pragma comment(lib, "freetype249.lib")
#endif

CFreeTypeFont::CFreeTypeFont()
{
	bLoaded = false;
}

//====================

inline int next_p2(int n){int res = 1; while(res < n)res <<= 1; return res;}

/////////////// CREATECHAR \\\\\\\\\\\\\\\\\*
//
// Creates OpenGL 2D texture with a character
//
/////////////// CREATECHAR \\\\\\\\\\\\\\\\\*

void CFreeTypeFont::createChar(FT_Face &ftFace, int iIndex)
{
	FT_Load_Glyph(ftFace, FT_Get_Char_Index(ftFace, iIndex), FT_LOAD_DEFAULT);

	FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);
	FT_Bitmap* pBitmap = &ftFace->glyph->bitmap;

	int iW = pBitmap->width, iH = pBitmap->rows;
	int iTW = next_p2(iW), iTH = next_p2(iH);

	GLubyte* data = new GLubyte[iTW*iTH*2];
	// Copy glyph data
	FOR(ch, iTH)FOR(cw, iTW)
		data[2*(ch*iTW+cw)] = 
		data[2*(ch*iTW+cw)+1] = (ch >= iH || cw >= iW) ? 0 : pBitmap->buffer[(iH-ch-1)*iW+cw];
	// And create a texture from it
	glGenTextures(1, &uiFontTex[iIndex]);
	glBindTexture(GL_TEXTURE_2D, uiFontTex[iIndex]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, iTW, iTH, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
	// Calculate glyph data
	iAdvX[iIndex] = ftFace->glyph->advance.x>>6;
	iBearingX[iIndex] = ftFace->glyph->metrics.horiBearingX>>6;
	iAdvY[iIndex] = (ftFace->glyph->metrics.height - ftFace->glyph->metrics.horiBearingY)>>6;
	iNewLine = max(iNewLine, int(ftFace->glyph->metrics.height>>6));
	iCharW[iIndex] = iTW; iCharH[iIndex] = iTH;

	delete [] data;
}

void CFreeTypeFont::loadSystemFont(string sName, int iPXSize)
{
	char buf[512]; GetWindowsDirectory(buf, 512);
	string sPath = buf;
	sPath += "\\Fonts\\";
	sPath += sName;

	loadFont(sPath, iPXSize);
}

/////////////// LOADFONT \\\\\\\\\\\\\\\\\*
//
// Loads whole TrueType font from a file.
//
/////////////// LOADFONT \\\\\\\\\\\\\\\\\*

bool CFreeTypeFont::loadFont(string sFile, int iPXSize)
{
	FT_Library ftLib;
	FT_Face ftFace;
	BOOL bError = FT_Init_FreeType(&ftLib);
	
	bError = FT_New_Face(ftLib, sFile.c_str(), 0, &ftFace);
	if(bError)return false;
	FT_Set_Pixel_Sizes(ftFace, iPXSize, iPXSize);
	iLoadedPixelSize = iPXSize;
	FOR(i, 128)createChar(ftFace, i);
	bLoaded = true;

	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLib);

	return true;
}

void beginOrthoProjection()
{
	RECT rRect; GetClientRect(appMain.hWnd, &rRect);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, rRect.right, 0, rRect.bottom, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDepthFunc(GL_ALWAYS);
}

//////////// ENDORTHOPROJECTION \\\\\\\\\\\\\\*
//
// Guess what it does.
//
//////////// ENDORTHOPROJECTION \\\\\\\\\\\\\\*

void endOrthoProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDepthFunc(GL_LESS);
}

////////////////// PRINT \\\\\\\\\\\\\\\\\\*
//
// Prints text on x, y screen coordinates, 
// multiple lines allowed.
//
////////////////// PRINT \\\\\\\\\\\\\\\\\\*

void CFreeTypeFont::print(string sText, int x, int y, int iPXSize, bool bStartOrtho)
{
	if(!bLoaded)return;
	if(bStartOrtho)beginOrthoProjection();
	bool bEnabled = glIsEnabled(GL_BLEND);
	if(!bEnabled)glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int iCurX = x, iCurY = y;
	if(iPXSize == -1)iPXSize = iLoadedPixelSize;
	FOR(i, ESZ(sText))
	{
		if(sText[i] == '\n')
		{
			iCurX = x;
			iCurY -= iNewLine*iPXSize/iLoadedPixelSize;
			continue;
		}
		int iIndex = int(sText[i]);
		iCurX += iBearingX[iIndex]*iPXSize/iLoadedPixelSize;
		glBindTexture(GL_TEXTURE_2D, uiFontTex[iIndex]);
		glBegin(GL_QUADS);
			glTexCoord2d(0, 1); glVertex2d(iCurX, iCurY+(iCharH[iIndex]-iAdvY[iIndex])*iPXSize/iLoadedPixelSize);
			glTexCoord2d(1, 1); glVertex2d(iCurX+iCharW[iIndex]*iPXSize/iLoadedPixelSize, iCurY+(iCharH[iIndex]-iAdvY[iIndex])*iPXSize/iLoadedPixelSize);
			glTexCoord2d(1, 0); glVertex2d(iCurX+iCharW[iIndex]*iPXSize/iLoadedPixelSize, iCurY-iAdvY[iIndex]*iPXSize/iLoadedPixelSize);
			glTexCoord2d(0, 0); glVertex2d(iCurX, iCurY-iAdvY[iIndex]*iPXSize/iLoadedPixelSize);
		glEnd();
		iCurX -= iBearingX[iIndex]*iPXSize/iLoadedPixelSize;
		iCurX += iAdvX[iIndex]*iPXSize/iLoadedPixelSize;
	}
	if(!bEnabled)glDisable(GL_BLEND);
	if(bStartOrtho)endOrthoProjection();
}

////////// GETTEXTWIDTH \\\\\\\\\\*
//
// Guess what it does.
//
////////// GETTEXTWIDTH \\\\\\\\\\*

int CFreeTypeFont::getTextWidth(string sText, int iPXSize)
{
	if(iPXSize == -1)iPXSize = iLoadedPixelSize;
	int iRes = 0;
	FOR(i, ESZ(sText))iRes += iAdvX[int(sText[i])]*iPXSize/iLoadedPixelSize;
	return iRes;
}
