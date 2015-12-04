#include "common_header.h"

#include "texture.h"

#include <FreeImage.h>

#pragma comment(lib, "FreeImage.lib")

CTexture::CTexture()
{
	bMipMapsGenerated = false;
}

/*-----------------------------------------------

Name:	createFromData

Params:	a_sPath - path to the texture
				format - format of data
				bGenerateMipMaps - whether to create mipmaps

Result:	Creates texture from provided data.

/*---------------------------------------------*/

void CTexture::createFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	iWidth = a_iWidth;
	iHeight = a_iHeight;
	iBPP = a_iBPP;

	glEnable(GL_TEXTURE_2D);
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	if(format == GL_RGB || format == GL_BGR) // We must handle this because of internal format parameter
	{
		BYTE* bData2 = new BYTE[iWidth*iHeight*3];
		FOR(i, iHeight)FOR(j, iWidth)
		{
			bData2[(i*iWidth+j)*3+FI_RGBA_RED] = bData[(i*iWidth+j)*3];
			bData2[(i*iWidth+j)*3+FI_RGBA_GREEN] = bData[(i*iWidth+j)*3+1];
			bData2[(i*iWidth+j)*3+FI_RGBA_BLUE] = bData[(i*iWidth+j)*3+2];
		}

		if(!bGenerateMipMaps)glTexImage2D(GL_TEXTURE_2D, 0, 3, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bData2);
		else gluBuild2DMipmaps(GL_TEXTURE_2D, 3, iWidth, iHeight, GL_RGB, GL_UNSIGNED_BYTE, bData2);

		delete[] bData2;
	}
	else
	{
		if(!bGenerateMipMaps)glTexImage2D(GL_TEXTURE_2D, 1, format, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
		else gluBuild2DMipmaps(GL_TEXTURE_2D, 1, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bData);
	}

	sPath = "";
	bMipMapsGenerated = bGenerateMipMaps;

}

void CTexture::createLuminanceFromData(BYTE* bData, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	iWidth = a_iWidth;
	iHeight = a_iHeight;
	iBPP = a_iBPP;

	glEnable(GL_TEXTURE_2D);
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);

	if(format == GL_RGB || format == GL_BGR) // We must handle this because of internal format parameter
	{
		BYTE* bNewData = new BYTE[iWidth*iHeight*2];
		FOR(i, iHeight)FOR(j, iWidth)
			bNewData[ (i*iWidth+j)*2 ] = bNewData[ (i*iWidth+j)*2 + 1] = bData[(i*iWidth+j)*3];

		if(!bGenerateMipMaps)glTexImage2D(GL_TEXTURE_2D, 0, 2, iWidth, iHeight, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, bNewData);
		else gluBuild2DMipmaps(GL_TEXTURE_2D, 2, iWidth, iHeight, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, bNewData);

		delete[] bNewData;
	}
	else
	{
		if(!bGenerateMipMaps)glTexImage2D(GL_TEXTURE_2D, 0, 2, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
		else gluBuild2DMipmaps(GL_TEXTURE_2D, 2, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bData);
	}

	sPath = "";
	bMipMapsGenerated = bGenerateMipMaps;

}


/*-----------------------------------------------

Name:	loadTexture2D

Params:	a_sPath - path to the texture
				bGenerateMipMaps - whether to create mipmaps

Result:	Loads texture from a file, supports most
		graphics formats.

/*---------------------------------------------*/

bool CTexture::loadTexture2D(string a_sPath, bool bGenerateMipMaps, bool bLoadLuminance)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(a_sPath.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(a_sPath.c_str());
	
	if(fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, a_sPath.c_str());
	if(!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	// If somehow one of these failed (they shouldn't), return failure
	if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	GLenum format = FreeImage_GetBPP(dib) == 24 ? GL_BGR : FreeImage_GetBPP(dib) == 8 ? GL_LUMINANCE : 0;
	if(bLoadLuminance)createLuminanceFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);
	else createFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);
	
	FreeImage_Unload(dib);

	sPath = a_sPath;

	return true; // Success
}

void CTexture::setSamplerParameter(GLenum parameter, GLenum value)
{
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glTexParameteri(GL_TEXTURE_2D, parameter, value);
}

/*-----------------------------------------------

Name:	setFiltering

Params:	tfMagnification - mag. filter, must be from
									ETextureFiltering enum
			tfMinification - min. filter, must be from
									ETextureFiltering enum

Result:	Sets magnification and minification
			texture filter.

/*---------------------------------------------*/

void CTexture::setFiltering(int a_tfMagnification, int a_tfMinification)
{
	if(a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	// Set minification filter
	if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

}

/*-----------------------------------------------

Name:		bindTexture

Params:	iTextureUnit - texture unit to bind texture to

Result:	Guess what it does :)

/*---------------------------------------------*/

void CTexture::bindTexture(int iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
}

/*-----------------------------------------------

Name:		releaseTexture

Params:	none

Result:	Frees all memory used by texture.

/*---------------------------------------------*/

void CTexture::releaseTexture()
{
	glDeleteTextures(1, &uiTexture);
}

int CTexture::getMinificationFilter()
{
	return tfMinification;
}

int CTexture::getMagnificationFilter()
{
	return tfMagnification;
}

int CTexture::getWidth()
{
	return iWidth;
}

int CTexture::getHeight()
{
	return iHeight;
}

int CTexture::getBPP()
{
	return iBPP;
}

UINT CTexture::getTextureID()
{
	return uiTexture;
}