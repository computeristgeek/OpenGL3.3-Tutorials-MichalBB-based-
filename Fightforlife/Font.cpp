//************************************** Font.cpp *********************************************
// 
//								 Here are font functions.
//
//************************************** Font.cpp *********************************************

#include "Main.h"

GLuint font[256]; // Here are stored display list characters
GLuint fonts[2];

///////////////////////////////////// CREATETEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This function creates texture GLint GLuint storage.
//
///////////////////////////////////// CREATETEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid CreateTexture(GLuint *storage, LPSTR strFileName)
{
	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	
	AUX_RGBImageRec *pBitmap =auxDIBImageLoad(strFileName);

	if(pBitmap == NULL)									// If we can't load the file, quit!
		return;

	glGenTextures(1, storage);

	glBindTexture(GL_TEXTURE_2D, *storage);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	delete [] pBitmap->data;					// Free the texture data, we don't need it anymore

	delete pBitmap;									// Free the bitmap structure

}

/////////////////////////////////////// BUILDFONT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								Loads font into memory.
//
///////////////////////////////////// CREATETEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid BuildFont()								// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord

	for(GLint loop=0; loop<256; loop++)						// Loop Through All 256 Lists
	{
		font[loop]=glGenLists(1);
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(font[loop],GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2i(-8,-8);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(8,-8);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
				glVertex2i(8,8);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2i(-8,8);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
		glEndList();									// Done Building The Display List
	}

	CreateTexture(&fonts[0],"Data/Textures/Font.bmp");
	CreateTexture(&fonts[1],"Data/Textures/Fontmask.bmp");

}

///////////////////////////////////////// START2D \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//								Sets up an ortho screen.
//
///////////////////////////////////////// START2D \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid Start2D()
{
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,800,0,600,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();
}

////////////////////////////////////////// END2D \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							Restores previous screen settings.
//
////////////////////////////////////////// END2D \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid End2D()
{
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);
}

GLvoid PrintText(CVector2 textpos,GLfloat size,Rgb color, GLint type,GLuint flags, const char*fmt,...)
{
	GLint character=0; // This is list id
	GLchar final[255]={0}; // Final text to display

	va_list arg;
	va_start(arg,fmt);
	vsprintf(final,fmt,arg);
	va_end(arg);

	if(flags==CENTER)
	{
		GLfloat width=strlen(final)*8*size;
		textpos.x=(int)(800-width)/2;
	}

	Start2D(); // Set up ortho screen
	glTranslatef(textpos.x,textpos.y,0); // Translate and scale
	glScalef(size,size,1);
	glEnable(GL_BLEND); // Enable blending
	glBlendFunc(GL_DST_COLOR,GL_ZERO); // Blending function used for masking
	glBindTexture(GL_TEXTURE_2D, fonts[1]); // Select font mask texture
	glPushMatrix(); // Push the matrix
	glColor3f(1,1,1); // Set the color to white
	for(GLint s=0;s<strlen(final);s++)
	{ 
		character=final[s]-32+type*128; // Find a list
		glCallList(font[character]); // Display it
		glTranslated(10,0,0); // Translate into screen
	}
	glPopMatrix(); // Pop the matrix
	glBlendFunc(GL_ONE,GL_ONE); // Another blending mode
	glBindTexture(GL_TEXTURE_2D, fonts[0]); // Select font texture
	glColor3f(color.red,color.green,color.blue);  // Select the color to user's color
	for(s=0;s<strlen(final);s++)
	{
		character=final[s]-32+type*128; // Find a list
		glCallList(font[character]); // Display it
		glTranslated(10,0,0); // Translate into screen
	}
	End2D(); // Destroy ortho screen
	glDisable(GL_BLEND); // Disable blending
}
