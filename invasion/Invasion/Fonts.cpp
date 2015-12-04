#include "Main.h"

GLuint base;

void BuildFont()								// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord

	base=glGenLists(256);								// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, textures[3]);			// Select Our Font Texture
	for (int loop=0; loop<256; loop++)						// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(base+loop,GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1.0f-cy-0.0625f);		// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1.0f-cy);		// Texture Coord (Top Right)
				glVertex2i(16,16);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,1.0f-cy);				// Texture Coord (Top Left)
				glVertex2i(0,16);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslatef(11,0,0);	//11,0,0			// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}
}

GLvoid KillFont(GLvoid)									// Delete The Font From Memory
{
	glDeleteLists(base,256);							// Delete All 256 Display Lists
}
//////////////////////////////////// GLPRINT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						  	Text printing function.
//
//////////////////////////////////// GLPRINT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

GLvoid glPrint(GLint x, GLint y,int set,float scalex, float scaley, float scalez,float colx, float coly, float colz,const char *string, ...)	
{

	char		text[1024];									// Holds Our String
	va_list		ap;											// Pointer To List Of Arguments

	if (string == NULL)										// If There's No Text
		return;												// Do Nothing

	va_start(ap, string);										// Parses The String For Variables
	    vsprintf(text, string, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);												// Results Are Stored In Text

	if (set>1)												// Did User Choose An Invalid Character Set?
	{
		set=1;												// If So, Select Set 1 (Italic)
	}


	glDisable(GL_DEPTH_TEST);							// Enables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,800,0,600,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
										
	glTranslatef((float)x,(float)y,0);

	glEnable(GL_BLEND);	

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glColor3f(colx,coly,colz);		
	glScalef(scalex,scaley,scalez);
	glListBase(base-32+(128*set));					
	glCallLists(strlen(text),GL_UNSIGNED_BYTE,text);

	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glColor3f(1,1,1);
}