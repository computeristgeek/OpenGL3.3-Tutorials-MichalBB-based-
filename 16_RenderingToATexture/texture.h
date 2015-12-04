#pragma once

enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

/********************************

Class:		CTexture

Purpose:	Wraps OpenGL texture
			object and performs
			their loading.

********************************/

class CTexture
{
public:
	GLvoid CreateEmptyTexture(GLint a_iWidth, GLint a_iHeight, GLint a_iBPP, GLenum format);
	GLvoid CreateFromData(BYTE* bData, GLint a_iWidth, GLint a_iHeight, GLint a_iBPP, GLenum format, GLboolean bGenerateMipMaps = false);
	
	GLboolean LoadTexture2D(string a_sPath, GLboolean bGenerateMipMaps = false);
	GLvoid BindTexture(GLint iTextureUnit = 0);

	GLvoid SetFiltering(GLint a_tfMagnification, GLint a_tfMinification);

	GLvoid SetSamplerParameter(GLenum parameter, GLenum value);

	GLint GetMinificationFilter();
	GLint GetMagnificationFilter();

	GLint GetWidth();
	GLint GetHeight();
	GLint GetBPP();

	UINT GetTextureID();

	GLvoid DeleteTexture();

	CTexture();
private:

	GLint iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	UINT uiTexture; // Texture name
	UINT uiSampler; // Sampler name
	GLboolean bMipMapsGenerated;

	GLint tfMinification, tfMagnification;

	string sPath;
};

