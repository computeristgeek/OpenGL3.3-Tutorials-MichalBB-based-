#include "main.h"
#include "Weapons.h"
GLuint		base;


GLuint	loop;			

GLfloat	cnt1;				// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2;				// 2nd Counter Used To Move Text & For Coloring


GLvoid BuildFont(GLvoid)								// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord

	base=glGenLists(256);								// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, g_Texture[5]);			// Select Our Font Texture
	for (loop=0; loop<256; loop++)						// Loop Through All 256 Lists
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
			glTranslatef(11,0,0);	//10,0,0			// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built
}

GLvoid KillFont(GLvoid)									// Delete The Font From Memory
{
	glDeleteLists(base,256);							// Delete All 256 Display Lists
}


GLvoid glPrintFast(GLint x, GLint y, char *string, int set)	
{
	if (set>1)set=1;
	glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);	

	glBindTexture(GL_TEXTURE_2D, g_Texture[5]);		

	glLoadIdentity();									
	glTranslatef((float)x,(float)y,0);		
	glScalef(3.0f,3.0f,0.0f);
	glListBase(base-32+(128*set));					
	glCallLists(strlen(string),GL_BYTE,string);			
glDisable(GL_BLEND);

}

GLvoid glPrint(GLint x, GLint y, char *string, int set,float scalex, float scaley, float scalez)	
{
		
	glDisable(GL_DEPTH_TEST);							// Enables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,640,0,480,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);	

	glBindTexture(GL_TEXTURE_2D, g_Texture[5]);		

	glLoadIdentity();									
	glTranslatef((float)x,(float)y,0);		
	glScalef(scalex,scaley,scalez);
	glListBase(base-32+(128*set));					
	glCallLists(strlen(string),GL_BYTE,string);			
glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}