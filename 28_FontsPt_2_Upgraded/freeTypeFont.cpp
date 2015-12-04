#include "common_header.h"

#include "FreeTypeFont.h"

#include <glm/gtc/matrix_transform.hpp>

#ifdef _DEBUG
#pragma comment(lib, "freetype249_D.lib")
#else
#pragma comment(lib, "freetype249.lib")
#endif

using namespace std;

CFreeTypeFont::CFreeTypeFont()
{
	bLoaded = false;
}

/*-----------------------------------------------

Name:	CreateChar

Params:	iIndex - character index in Unicode.

Result:	Creates one single character (its
		texture).

/*---------------------------------------------*/

inline GLint next_p2(GLint n){GLint res = 1; while(res < n)res <<= 1; return res;}

GLvoid CFreeTypeFont::CreateChar(GLint iIndex, GLubyte* bData)
{
	FT_Load_Glyph(ftFace, FT_Get_Char_Index(ftFace, iIndex), FT_LOAD_DEFAULT);

	FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);
	FT_Bitmap* pBitmap = &ftFace->glyph->bitmap;

	GLint iW = pBitmap->width, iH = pBitmap->rows;

	// Some characters when rendered, are somehow just bigger than our desired pixel size
	// In this case, I just ignore them - another solution is to set iOneCharSquareSize in LoadFont function
	// to twice the size (just multiply by 2 and you're safe)
	if(iW > iOneCharSquareSize)
		return;
	if(iH > iOneCharSquareSize)
		return;

	GLint iRow = (iIndex%CHARS_PER_TEXTURE)/CHARS_PER_TEXTUREROOT;
	GLint iCol = (iIndex%CHARS_PER_TEXTURE)%CHARS_PER_TEXTUREROOT;
	GLint iOneTextureByteRowSize = CHARS_PER_TEXTUREROOT*iOneCharSquareSize;

	// Copy glyph data
	FOR(ch, iH)memcpy(bData+iRow*iOneTextureByteRowSize*iOneCharSquareSize + iCol*iOneCharSquareSize + ch*iOneTextureByteRowSize, pBitmap->buffer + (iH-ch-1)*iW, iW);

	// Calculate glyph data
	iAdvX[iIndex] = ftFace->glyph->advance.x>>6;
	iBearingX[iIndex] = ftFace->glyph->metrics.horiBearingX>>6;
	iCharWidth[iIndex] = ftFace->glyph->metrics.width>>6;

	iAdvY[iIndex] = (ftFace->glyph->metrics.height - ftFace->glyph->metrics.horiBearingY)>>6;
	iBearingY[iIndex] = ftFace->glyph->metrics.horiBearingY>>6;
	iCharHeight[iIndex] = ftFace->glyph->metrics.height>>6;

	iNewLine = max(iNewLine, int(ftFace->glyph->metrics.height>>6));

	glm::vec2 vQuad[] =
	{
		glm::vec2(0.0f, float(-iAdvY[iIndex]+iOneCharSquareSize)),
		glm::vec2(0.0f, float(-iAdvY[iIndex])),
		glm::vec2(float(iOneCharSquareSize), float(-iAdvY[iIndex]+iOneCharSquareSize)),
		glm::vec2(float(iOneCharSquareSize), float(-iAdvY[iIndex]))
	};
	GLfloat fOneStep = 1.0f/(float(CHARS_PER_TEXTUREROOT));
	// Texture coordinates change depending on character index, which determines its position in the texture
	glm::vec2 vTexQuad[] =
	{
		glm::vec2(float(iCol)*fOneStep, float(iRow+1)*fOneStep),
		glm::vec2(float(iCol)*fOneStep, float(iRow)*fOneStep),
		glm::vec2(float(iCol+1)*fOneStep, float(iRow+1)*fOneStep),
		glm::vec2(float(iCol+1)*fOneStep, float(iRow)*fOneStep)
	};

	// Add this char to VBO
	FOR(i, 4)
	{
		vboData.AddData(&vQuad[i], sizeof(glm::vec2));
		vboData.AddData(&vTexQuad[i], sizeof(glm::vec2));
	}
}

/*-----------------------------------------------

Name:	LoadFont

Params:	sFile - path to font file
		iPXSize - desired font pixel size

Result:	Loads whole font.

/*---------------------------------------------*/

GLboolean CFreeTypeFont::LoadFont(string sFile, GLint iPXSize, GLint iMaxCharSupport)
{
	BOOL bError = FT_Init_FreeType(&ftLib);
	
	bError = FT_New_Face(ftLib, sFile.c_str(), 0, &ftFace);
	if(bError)return false;
	FT_Set_Pixel_Sizes(ftFace, iPXSize, iPXSize);
	iLoadedPixelSize = iPXSize;
	iOneCharSquareSize = next_p2(iLoadedPixelSize);

	// Neat trick - we need to calculate ceil(iMaxCharSupport/CHARS_PER_TEXTURE) and that calculation does it, more in article
	GLint iNumTextures = (iMaxCharSupport+CHARS_PER_TEXTURE-1)/CHARS_PER_TEXTURE;

	// One texture will store up to CHARS_PER_TEXTURE characters
	GLubyte** bTextureData = new GLubyte*[iNumTextures];

	tCharTextures.resize(iNumTextures);

	FOR(i, iNumTextures)
	{
		GLint iTextureDataSize = iOneCharSquareSize*iOneCharSquareSize*CHARS_PER_TEXTURE;
		bTextureData[i] = new GLubyte[iTextureDataSize];
		memset(bTextureData[i], 0, iTextureDataSize);
	}

	iAdvX.resize(iMaxCharSupport); iAdvY.resize(iMaxCharSupport);
	iBearingX.resize(iMaxCharSupport); iBearingY.resize(iMaxCharSupport);
	iCharWidth.resize(iMaxCharSupport); iCharHeight.resize(iMaxCharSupport);

	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);
	vboData.CreateVBO();
	vboData.BindVBO();

	FOR(i, iMaxCharSupport)CreateChar(i, bTextureData[i/CHARS_PER_TEXTURE]);
	bLoaded = true;

	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLib);

	FOR(i, iNumTextures)
	{
		tCharTextures[i].CreateFromData(bTextureData[i], iOneCharSquareSize*CHARS_PER_TEXTUREROOT, iOneCharSquareSize*CHARS_PER_TEXTUREROOT, 8, GL_DEPTH_COMPONENT, false);
		tCharTextures[i].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

		tCharTextures[i].SetSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		tCharTextures[i].SetSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	
	vboData.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, (void*)(sizeof(glm::vec2)));

	FOR(i, iNumTextures)
		delete[] bTextureData[i];

	delete[] bTextureData;

	return true;
}

/*-----------------------------------------------

Name:	LoadSystemFont

Params:	sName - system font name
		iPXSize - desired font pixel size

Result:	Loads system font (from system Fonts
		directory).

/*---------------------------------------------*/

GLboolean CFreeTypeFont::LoadSystemFont(string sName, GLint iPXSize, GLint iMaxCharSupport)
{
	char buf[512]; GetWindowsDirectory(buf, 512);
	string sPath = buf;
	sPath += "\\Fonts\\";
	sPath += sName;

	return LoadFont(sPath, iPXSize, iMaxCharSupport);
}

/*-----------------------------------------------

Name:	GetTextWidth

Params:	sText - text to get width of
		iPXSize - it's printed size

Result:	Returns width as number of pixels the
		text will occupy.

/*---------------------------------------------*/

GLint CFreeTypeFont::GetTextWidth(string sText, GLint iPXSize)
{
	GLint iResult = 0;
	FOR(i, ESZ(sText))iResult += iAdvX[sText[i]];

	return iResult*iPXSize/iLoadedPixelSize;
}

/*-----------------------------------------------

Name:	Print

Params:	sText - text to print
		x, y - 2D position
		iPXSize - printed text size

Result:	Prints text at specified position
		with specified pixel size.

/*---------------------------------------------*/

GLvoid CFreeTypeFont::Print(string sText, GLint x, GLint y, GLint iPXSize)
{
	if(!bLoaded)return;
	GLint iLastBoundTexture = -1;

	glBindVertexArray(uiVAO);
	shShaderProgram->SetUniform("gSampler", 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLint iCurX = x, iCurY = y;
	if(iPXSize == -1)iPXSize = iLoadedPixelSize;
	GLfloat fScale = float(iPXSize)/float(iLoadedPixelSize);
	FOR(i, ESZ(sText))
	{
		if(sText[i] == '\n')
		{
			iCurX = x;
			iCurY -= iNewLine*iPXSize/iLoadedPixelSize;
			continue;
		}
		GLint iIndex = int(sText[i]);
		GLint iTextureNeeded = iIndex/CHARS_PER_TEXTURE;
		if(iTextureNeeded != iLastBoundTexture)
		{
			iLastBoundTexture = iTextureNeeded;
			tCharTextures[iTextureNeeded].BindTexture();
		}
		iCurX += iBearingX[iIndex]*iPXSize/iLoadedPixelSize;
		if(sText[i] != ' ')
		{
			glm::mat4 mModelView = glm::translate(glm::mat4(1.0f), glm::vec3(float(iCurX), float(iCurY), 0.0f));
			mModelView = glm::scale(mModelView, glm::vec3(fScale));
			shShaderProgram->SetUniform("matrices.modelViewMatrix", mModelView);
			// Draw character
			glDrawArrays(GL_TRIANGLE_STRIP, iIndex*4, 4);
		}

		iCurX += (iAdvX[iIndex]-iBearingX[iIndex])*iPXSize/iLoadedPixelSize;
	}
	glDisable(GL_BLEND);
}

GLvoid CFreeTypeFont::Print(wstring sText, GLint x, GLint y, GLint iPXSize)
{
	if(!bLoaded)return;
	GLint iLastBoundTexture = -1;

	glBindVertexArray(uiVAO);
	shShaderProgram->SetUniform("gSampler", 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLint iCurX = x, iCurY = y;
	if(iPXSize == -1)iPXSize = iLoadedPixelSize;
	GLfloat fScale = float(iPXSize)/float(iLoadedPixelSize);
	FOR(i, ESZ(sText))
	{
		if(sText[i] == '\n')
		{
			iCurX = x;
			iCurY -= iNewLine*iPXSize/iLoadedPixelSize;
			continue;
		}
		GLint iIndex = int(sText[i]);
		GLint iTextureNeeded = iIndex/CHARS_PER_TEXTURE;
		if(iTextureNeeded < 0 || iTextureNeeded >= ESZ(tCharTextures))
			continue;
		if(iTextureNeeded != iLastBoundTexture)
		{
			iLastBoundTexture = iTextureNeeded;
			tCharTextures[iTextureNeeded].BindTexture();
		}
		iCurX += iBearingX[iIndex]*iPXSize/iLoadedPixelSize;
		if(sText[i] != ' ')
		{
			glm::mat4 mModelView = glm::translate(glm::mat4(1.0f), glm::vec3(float(iCurX), float(iCurY), 0.0f));
			mModelView = glm::scale(mModelView, glm::vec3(fScale));
			shShaderProgram->SetUniform("matrices.modelViewMatrix", mModelView);
			// Draw character
			glDrawArrays(GL_TRIANGLE_STRIP, iIndex*4, 4);
		}

		iCurX += (iAdvX[iIndex]-iBearingX[iIndex])*iPXSize/iLoadedPixelSize;
	}
	glDisable(GL_BLEND);
}

/*-----------------------------------------------

Name:	PrintFormatted

Params:	x, y - 2D position
		iPXSize - printed text size
		sText - text to print

Result:	Prints formatted text at specified position
		with specified pixel size.

/*---------------------------------------------*/

GLvoid CFreeTypeFont::PrintFormatted(GLint x, GLint y, GLint iPXSize, char* sText, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, sText);
	vsprintf(buf, sText, ap);
	va_end(ap);
	Print(buf, x, y, iPXSize);
}

GLvoid CFreeTypeFont::PrintFormatted(GLint x, GLint y, GLint iPXSize, wchar_t* sText, ...)
{
	wchar_t buf[512];
	va_list ap;
	va_start(ap, sText);
	vswprintf(buf, sText, ap);
	va_end(ap);
	Print(buf, x, y, iPXSize);
}

/*-----------------------------------------------

Name:	DeleteFont

Params:	none

Result:	Deletes all font textures.

/*---------------------------------------------*/

GLvoid CFreeTypeFont::DeleteFont()
{
	FOR(i, ESZ(tCharTextures))
		tCharTextures[i].DeleteTexture();
	vboData.DeleteVBO();
	glDeleteVertexArrays(1, &uiVAO);
}

/*-----------------------------------------------

Name:	SetShaderProgram

Params:	a_shShaderProgram - shader program

Result:	Sets shader program that font uses.

/*---------------------------------------------*/

GLvoid CFreeTypeFont::SetShaderProgram(CShaderProgram* a_shShaderProgram)
{
	shShaderProgram = a_shShaderProgram;
}