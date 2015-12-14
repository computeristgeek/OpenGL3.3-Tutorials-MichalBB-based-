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

Class:	CTexture

Purpose:	Wraps OpenGL texture
			object and performs
			their loading.

********************************/

class CTexture
{
public:
	GLvoid createFromData(GL_UNSIGNED_BYTE* bData, GLint a_iWidth, GLint a_iHeight, GLint a_iBPP, GLenum format, GLboolean bGenerateMipMaps = false);
	GLboolean loadTexture2D(string a_sPath, GLboolean bGenerateMipMaps = false);
	GLvoid bindTexture(GLint iTextureUnit = 0);

	GLvoid setFiltering(GLint a_tfMagnification, GLint a_tfMinification);

	GLvoid setSamplerParameter(GLenum parameter, GLenum value);

	GLint getMinificationFilter();
	GLint getMagnificationFilter();

	GLint getWidth();
	GLint getHeight();
	GLint getBPP();

	GLvoid releaseTexture();

	CTexture();
private:
	GLint iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	GLuint uiTexture; // Texture name
	GLuint uiSampler; // Sampler name
	GLboolean bMipMapsGenerated;

	GLint tfMinification, tfMagnification;

	string sPath;
};

