#include "common_header.h"

#include "texture.h"

#include <FreeImage.h>

#pragma comment(lib, "FreeImage.lib")

CTexture::CTexture()
{
	bMipMapsGenerated = false;
}

/*-----------------------------------------------

Name:	CreateEmptyTexture

Params:	a_iWidth, a_iHeight - dimensions
		format - format of data

Result:	Creates texture from provided data.

/*---------------------------------------------*/

GLvoid CTexture::CreateEmptyTexture(GLint a_iWidth, GLint a_iHeight, GLenum format)
{
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if(format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	// We must handle this because of internal format parameter
	else if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	
	glGenSamplers(1, &uiSampler);
}

/*-----------------------------------------------

Name:	CreateFromData

Params:	a_sPath - path to the texture
		format - format of data
		bGenerateMipMaps - whether to create mipmaps

Result:	Creates texture from provided data.

/*---------------------------------------------*/

GLvoid CTexture::CreateFromData(BYTE* bData, GLint a_iWidth, GLint a_iHeight, GLint a_iBPP, GLenum format, GLboolean bGenerateMipMaps)
{
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	if(format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	// We must handle this because of internal format parameter
	else if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	if(bGenerateMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
	glGenSamplers(1, &uiSampler);

	sPath = "";
	bMipMapsGenerated = bGenerateMipMaps;
	iWidth = a_iWidth;
	iHeight = a_iHeight;
	iBPP = a_iBPP;
}

/*-----------------------------------------------

Name:	LoadTexture2D

Params:	a_sPath - path to the texture
		bGenerateMipMaps - whether to create mipmaps

Result:	Loads texture from a file, supports most
		graphics formats.

/*---------------------------------------------*/

GLboolean CTexture::LoadTexture2D(string a_sPath, GLboolean bGenerateMipMaps)
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

	GLenum format = 0;
	GLint bada = FreeImage_GetBPP(dib);
	if(FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
	if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;
	if(format != 0)
	CreateFromData(bDataPointer, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), FreeImage_GetBPP(dib), format, bGenerateMipMaps);
	
	FreeImage_Unload(dib);

	if(format == 0)
		return false;

	sPath = a_sPath;

	return true; // Success
}

GLvoid CTexture::SetSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(uiSampler, parameter, value);
}

/*-----------------------------------------------

Name:	SetFiltering

Params:	tfMagnification - mag. filter, must be from
							ETextureFiltering enum
		tfMinification - min. filter, must be from
							ETextureFiltering enum

Result:	Sets magnification and minification
			texture filter.

/*---------------------------------------------*/

GLvoid CTexture::SetFiltering(GLint a_tfMagnification, GLint a_tfMinification)
{
	glBindSampler(0, uiSampler);

	// Set magnification filter
	if(a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	tfMinification = a_tfMinification;
	tfMagnification = a_tfMagnification;
}

/*-----------------------------------------------

Name:	BindTexture

Params:	iTextureUnit - texture unit to bind texture to

Result:	Guess what it does :)

/*---------------------------------------------*/

GLvoid CTexture::BindTexture(GLint iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBindSampler(iTextureUnit, uiSampler);
}

/*-----------------------------------------------

Name:	DeleteTexture

Params:	none

Result:	Frees all memory used by texture.

/*---------------------------------------------*/

GLvoid CTexture::DeleteTexture()
{
	glDeleteSamplers(1, &uiSampler);
	glDeleteTextures(1, &uiTexture);
}

/*-----------------------------------------------

Name:	Getters

Params:	none

Result:	... They get something :D

/*---------------------------------------------*/

GLint CTexture::GetMinificationFilter()
{
	return tfMinification;
}

GLint CTexture::GetMagnificationFilter()
{
	return tfMagnification;
}

GLint CTexture::GetWidth()
{
	return iWidth;
}

GLint CTexture::GetHeight()
{
	return iHeight;
}

GLint CTexture::GetBPP()
{
	return iBPP;
}

GLuint CTexture::GetTextureID()
{
	return uiTexture;
}

string CTexture::GetPath()
{
	return sPath;
}

GLboolean CTexture::ReloadTexture()
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(sPath.c_str(), 0); // Check the file signature and deduce its format

	if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(sPath.c_str());
	
	if(fif == FIF_UNKNOWN) // If still unknown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
		dib = FreeImage_Load(fif, sPath.c_str());
	if(!dib)
		return false;

	BYTE* bDataPointer = FreeImage_GetBits(dib); // Retrieve the image data

	// If somehow one of these failed (they shouldn't), return failure
	if(bDataPointer == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
		return false;

	GLenum format;
	GLint bada = FreeImage_GetBPP(dib);
	if(FreeImage_GetBPP(dib) == 32)format = GL_RGBA;
	if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
	if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iWidth, iHeight, format, GL_UNSIGNED_BYTE, bDataPointer);
	if(bMipMapsGenerated)glGenerateMipmap(GL_TEXTURE_2D);
	
	FreeImage_Unload(dib);

	return true; // Success
}

GLvoid CTexture::SetWrap()
{
	glBindSampler(0, uiSampler);
	glSamplerParameteri(uiSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(uiSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

CTexture tTextures[NUMTEXTURES];

GLvoid LoadAllTextures()
{
	// Load textures

	string sTextureNames[] = {"dirt_seamless.jpg", "metal_plate.jpg"};

	FOR(i, NUMTEXTURES)
	{
		tTextures[i].LoadTexture2D("data\\textures\\"+sTextureNames[i], true);
		tTextures[i].SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
	}
}