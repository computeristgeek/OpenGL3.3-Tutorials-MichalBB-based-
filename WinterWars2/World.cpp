#include "main.h"
#include "Weapons.h"
GLvoid CreateTexture(GLuint textureArray[], LPSTR strFileName, GLint textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	

	
	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	
	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	glGenTextures(1, &textureArray[textureID]);


	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}




GLvoid CreateWorld(){
	glBindTexture(GL_TEXTURE_2D, g_Texture[0]);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 50.0f);	glVertex3f(-100, -1, -100);
		glTexCoord2f(50.0f, 50.0f);   glVertex3f(100, -1, -100);
		glTexCoord2f(50.0f, 0.0f);	glVertex3f(100, -1, 100);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(-100, -1, 100);

	glEnd();
			glBindTexture(GL_TEXTURE_2D, g_Texture[1]);
//Vpravo
	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 1.0f);	glVertex3f(100, 100, -100);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(100, -1, -100);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(100, -1, 100);
		glTexCoord2f(1.0f, 1.0f);   glVertex3f(100, 100, 100);
//Vpredu
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(-100, 100, 100);
		glTexCoord2f(1.0f, 1.0f);   glVertex3f(100, 100, 100);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(100, -1, 100);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(-100,-1, 100);
//Vzadu het
		glTexCoord2f(1.0f, 1.0f);	glVertex3f(-100, 100, -100);
		glTexCoord2f(0.0f, 1.0f);   glVertex3f(100, 100, -100);
		glTexCoord2f(0.0f, 0.0f);	glVertex3f(100, -1, -100);
		glTexCoord2f(1.0f, 0.0f);   glVertex3f(-100,-1, -100);
glEnd();


		glBindTexture(GL_TEXTURE_2D, g_Texture[2]);
			glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f(-100, 100, -100);
		glTexCoord2f(1.0f, 1.0f);   glVertex3f(100, 100, -100);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(100, 100, 100);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(-100, 100, 100);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, g_Texture[3]);
			glBegin(GL_QUADS);
// Vlavo
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(-100, 100, 100);
		glTexCoord2f(1.0f, 1.0f);   glVertex3f(-100, 100, -100);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(-100, -1, -100);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(-100,-1, 100);
	glEnd();
}

GLvoid Textures(){
	
CreateTexture(g_Texture,"Data/Textures/Snow.bmp",0);
CreateTexture(g_Texture,"Data/SkyBox/Mountains.bmp",1);
CreateTexture(g_Texture,"Data/SkyBox/Top.bmp",2);
CreateTexture(g_Texture,"Data/SkyBox/Sun.bmp",3);
CreateTexture(g_Texture,"Data/Textures/Objects.bmp",4);
CreateTexture(g_Texture,"Data/Textures/Font.bmp",5);
CreateTexture(g_Texture,"Data/Textures/Laser.bmp",6);
CreateTexture(g_Texture,"Data/Textures/Objects2.bmp",7);
}

GLvoid CrossHair(){
	glBegin(GL_LINE_STRIP);
glVertex2f(310,240);
glVertex2f(320,250);
glVertex2f(330,240);
glVertex2f(320,230);
glVertex2f(310,240);
glVertex2f(330,240);
glVertex2f(320,250);
glVertex2f(320,230);
glEnd();
}


GLvoid LifeLines(){
	glBegin(GL_LINE_STRIP);
	glVertex2f(50,95);
	glVertex2f(150,95);
	glVertex2f(150,55);
	glVertex2f(50,55);
	glVertex2f(150,55);
	glVertex2f(150,15);
	glVertex2f(50,15);
	glEnd();
		glBegin(GL_LINE_STRIP);
	glVertex2f(200,55);
    glVertex2f(245,55);
	glVertex2f(245,15);
	glVertex2f(200,15);
	glVertex2f(200,55);
	glVertex2f(350,55);
	glVertex2f(350,15);
	glVertex2f(200,15);
	glEnd();
}
GLvoid Objects(){
		glBindTexture(GL_TEXTURE_2D, g_Texture[4]);
glBegin(GL_QUADS);
//Armor
glTexCoord2f(0.5f,0.5f);	glVertex2f(5.0f,15.0f);
glTexCoord2f(1.0f,0.5f);   glVertex2f(50.0f,15.0f);
glTexCoord2f(1.0f,1.0f);	glVertex2f(50.0f,55.0f);
glTexCoord2f(0.5f,1.0f); glVertex2f(5.0f,55.0f);
//Weapon
if(weapon==Lasergun){
glTexCoord2f(0.0f,0.5f);	glVertex2f(200.0f,55.0f);
glTexCoord2f(0.5f,0.5f);   glVertex2f(245.0f,55.0f);
glTexCoord2f(0.5f,0.0f);	glVertex2f(245.0f,15.0f);
glTexCoord2f(0.0f,0.0f); glVertex2f(200.0f,15.0f);
}
else if(weapon==RocketLNCH){
glTexCoord2f(0.5f,0.5f);	glVertex2f(200.0f,55.0f);
glTexCoord2f(1,0.5f);   glVertex2f(245.0f,55.0f);
glTexCoord2f(1,0);	glVertex2f(245.0f,15.0f);
glTexCoord2f(0.5f,0); glVertex2f(200.0f,15.0f);
}
else if(weapon==Plasmathrow){
glTexCoord2f(0.0f,1);	glVertex2f(200.0f,55.0f);
glTexCoord2f(0.5f,1);   glVertex2f(245.0f,55.0f);
glTexCoord2f(0.5f,0.5f);	glVertex2f(245.0f,15.0f);
glTexCoord2f(0,0.5f); glVertex2f(200.0f,15.0f);
}
glEnd();
	glBindTexture(GL_TEXTURE_2D, g_Texture[7]);
glBegin(GL_QUADS);
glTexCoord2f(0.0f,0.5f);	glVertex2f(5.0f,55.0f);
glTexCoord2f(0.5f,0.5f);   glVertex2f(50.0f,55.0f);
glTexCoord2f(0.5,1.0f);	glVertex2f(50.0f,95.0f);
glTexCoord2f(0.0f,1.0f); glVertex2f(5.0f,95.0f);
glEnd();
CrossHair();
// Draw a progressbar of player's ammo
// Lasers

glBegin(GL_QUADS);
if(ammol<150 && ammol>0){glColor3f(1,0,0);}else if(ammol>=150 && ammol<350){glColor3f(1,0.5f,0);}else if(ammol>=350){glColor3f(0,1,0.5f);}else{glColor3f(0.5f,0,0);}
glTexCoord2f(0.0f,0.5f);	glVertex2f(450,480);
glTexCoord2f(0.5f,0.5f);   glVertex2f(490,480);
glTexCoord2f(0.5,0.0f);	glVertex2f(490,440);
glTexCoord2f(0.0f,0.0f); glVertex2f(450,440);

glVertex2f(465,442);glVertex2f(465,(float(ammol/13)+442));glVertex2f(475,float(ammol/13)+442);glVertex2f(475,442);
// Rockets
if(ammorocket<25 && ammorocket>0){glColor3f(1,0,0);}else if(ammorocket>=25 && ammorocket<55){glColor3f(1,0.5f,0);}else if(ammorocket>=55){glColor3f(0,1,0.5f);}else{glColor3f(0.5f,0,0);}
glTexCoord2f(0.5f,0.5f);glVertex2f(490,480);
glTexCoord2f(1,0.5f);   glVertex2f(530,480);
glTexCoord2f(1,0);	    glVertex2f(530,440);
glTexCoord2f(0.5f,0);   glVertex2f(490,440);
glVertex2f(505,442);glVertex2f(505,(float(ammorocket/2.6f)+442));glVertex2f(515,float(ammorocket/2.6f)+442);glVertex2f(515,442);
// Plasma
if(ammoplasma<35 && ammoplasma>0){glColor3f(1,0,0);}else if(ammoplasma>=35 && ammoplasma<125){glColor3f(1,0.5f,0);}else if(ammoplasma>=125){glColor3f(0,1,0.5f);}else{glColor3f(0.5f,0,0);}
glTexCoord2f(0.5f,1);glVertex2f(530,480);
glTexCoord2f(1,1);   glVertex2f(570,480);
glTexCoord2f(1,0.5f);	    glVertex2f(570,440);
glTexCoord2f(0.5f,0.5f);   glVertex2f(530,440);
glVertex2f(545,442);glVertex2f(545,(float(ammoplasma/6.5f)+442));glVertex2f(555,float(ammoplasma/6.5f)+442);glVertex2f(555,442);
glEnd();


}
GLint life=101;
GLint ammol=50;
GLint ammorocket=20;
GLint ammoplasma=20;
GLint armora=-1;
char ammoll[1];
char armor[1];
char text[1];
char ammorocketer[1];
char ammoplas[1];
GLvoid Ortho(){
	
	glDisable(GL_DEPTH_TEST);							// Enables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,640,0,480,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
Objects();
LifeLines();
glColor3f(0.0f,1,1);
gcvt( life, 3, text);

	glPrintFast(40,50,text,0);
	glPushMatrix();
	if(weapon==Lasergun){
	gcvt( ammol, 3, ammoll);
	glColor3f(1.0f,1,0);
	glPrintFast(240,12,ammoll,0);
	}
	else if(weapon==RocketLNCH){
	gcvt(ammorocket, 3, ammorocketer);
	glColor3f(1,0.5f,1);
	glPrintFast(240,12,ammorocketer,0);
	}
	else if(weapon==Plasmathrow){
	gcvt(ammoplasma, 3, ammoplas);
	glColor3f(1,0.5f,0.5f);
	glPrintFast(240,12,ammoplas,0);
	
	}
	glPopMatrix();
	glPushMatrix();
	gcvt( armora, 3, armor);
	glColor3f(1.0f,1,1);
	glPrintFast(40,10,armor,0);
	glPopMatrix();
glColor3f(1,1,1);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}



AUX_RGBImageRec *LoadBMP(const char *Filename)						// Loads A Bitmap Image
{

	FILE *File=NULL;												// File Handle

	if (!Filename)													// Make Sure A Filename Was Given
	{
		return NULL;												// If Not Return NULL
	}

	File=fopen(Filename,"r");										// Check To See If The File Exists

	if (File)														// Does The File Exist?
	{
		fclose(File);												// Close The Handle
		return auxDIBImageLoad(Filename);							// Load The Bitmap And Return A Pointer
	}

	return NULL;													// If Load Failed Return NULL
}

GLuGLint LoadGLTexture( const char *filename )						// Load Bitmaps And Convert To Textures
{
	AUX_RGBImageRec *pImage;										// Create Storage Space For The Texture
	GLuGLint texture = 0;												// Texture ID

	pImage = LoadBMP( filename );									// Loads The Bitmap Specified By filename

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if ( pImage != NULL && pImage->data != NULL )					// If Texture Image Exists
	{
		glGenTextures(1, &texture);									// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		free(pImage->data);											// Free The Texture Image Memory
		free(pImage);												// Free The Image Structure
	}

	return texture;													// Return The Status
}


