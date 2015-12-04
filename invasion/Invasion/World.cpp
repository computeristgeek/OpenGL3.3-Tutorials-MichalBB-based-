#include "Main.h"


UINT skybox[6];
UINT textures[MAXTEXTURES];

void DisplaySkyBox(float size)
{
	glDepthMask(0);
	glPushMatrix();

	glTranslatef(Main.m_vPosition.x,Main.m_vPosition.y,Main.m_vPosition.z);

	// Top

	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex3f(-size,size,size);
	glTexCoord2f(1,1); glVertex3f(size,size,size);
	glTexCoord2f(1,0); glVertex3f(size,size,-size);
	glTexCoord2f(0,0); glVertex3f(-size,size,-size);
	glEnd();

	// Down

	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex3f(-size,-size,-size);
	glTexCoord2f(1,1); glVertex3f(size,-size,-size);
	glTexCoord2f(1,0); glVertex3f(size,-size,size);
	glTexCoord2f(0,0); glVertex3f(-size,-size,size);
	glEnd();

	// Left

		glBindTexture(GL_TEXTURE_2D, skybox[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex3f(-size,size,size);
	glTexCoord2f(1,1); glVertex3f(-size,size,-size);
	glTexCoord2f(1,0); glVertex3f(-size,-size,-size);
	glTexCoord2f(0,0); glVertex3f(-size,-size,size);
	glEnd();

	// Right

	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex3f(size,size,-size);
	glTexCoord2f(1,1); glVertex3f(size,size,size);
	glTexCoord2f(1,0); glVertex3f(size,-size,size);
	glTexCoord2f(0,0); glVertex3f(size,-size,-size);
	glEnd();

	// Front

	glBindTexture(GL_TEXTURE_2D, skybox[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex3f(-size,size,-size);
	glTexCoord2f(1,1); glVertex3f(size,size,-size);
	glTexCoord2f(1,0); glVertex3f(size,-size,-size);
	glTexCoord2f(0,0); glVertex3f(-size,-size,-size);
	glEnd();

	// Back

	glBindTexture(GL_TEXTURE_2D, skybox[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex3f(size,size,size);
	glTexCoord2f(1,1); glVertex3f(-size,size,size);
	glTexCoord2f(1,0); glVertex3f(-size,-size,size);
	glTexCoord2f(0,0); glVertex3f(size,-size,size);
	glEnd();


	glPopMatrix();
	glDepthMask(1);

}

void DrawWorld()
{
	DisplaySkyBox(200);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	// Floor
	glTexCoord2f(0,200); glVertex3f(-400,0,-400);
	glTexCoord2f(200,200); glVertex3f(400,0,-400);
	glTexCoord2f(200,0); glVertex3f(400,0,400);
	glTexCoord2f(0,0); glVertex3f(-400,0,400);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	// Front
	glTexCoord2f(0,5); glVertex3f(-400,20,-400);
	glTexCoord2f(200,5); glVertex3f(400,20,-400);
	glTexCoord2f(200,0); glVertex3f(400,0,-400);
	glTexCoord2f(0,0); glVertex3f(-400,0,-400);
	// Back
	glTexCoord2f(0,5); glVertex3f(-400,20,400);
	glTexCoord2f(200,5); glVertex3f(400,20,400);
	glTexCoord2f(200,0); glVertex3f(400,0,400);
	glTexCoord2f(0,0); glVertex3f(-400,0,400);
	// Left
	glTexCoord2f(0,5); glVertex3f(-400,20,400);
	glTexCoord2f(200,5); glVertex3f(-400,20,-400);
	glTexCoord2f(200,0); glVertex3f(-400,0,-400);
	glTexCoord2f(0,0); glVertex3f(-400,0,400);
	// Right
	glTexCoord2f(0,5); glVertex3f(400,20,400);
	glTexCoord2f(200,5); glVertex3f(400,20,-400);
	glTexCoord2f(200,0); glVertex3f(400,0,-400);
	glTexCoord2f(0,0); glVertex3f(400,0,400);

	glEnd();


};

/////////////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//					This creates a texture in OpenGL that we can texture map.
//
/////////////////////////////////////// CREATE TEXTURE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	
	// We need to load the texture data, so we use a cool API that the glaux.lib offers.
	
	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	
	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);


	// Bind the texture to the texture arrays index and init the texture
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

void LoadFont()
{
	CreateTexture(textures,"Data/Textures/Font.bmp",3);
}

void LoadAllTextures()
{
	// Load Skybox
	CreateTexture(skybox,"Data/Skybox/top.bmp",0);
	CreateTexture(skybox,"Data/Skybox/down.bmp",1);
	CreateTexture(skybox,"Data/Skybox/left.bmp",2);
	CreateTexture(skybox,"Data/Skybox/right.bmp",3);
	CreateTexture(skybox,"Data/Skybox/front.bmp",4);
	CreateTexture(skybox,"Data/Skybox/back.bmp",5);
	// Load other textures
	CreateTexture(textures,"Data/Textures/Floor.bmp",0);
	CreateTexture(textures,"Data/Textures/Wall.bmp",1);
	CreateTexture(textures,"Data/Textures/Explosion.bmp",2);
	CreateTexture(textures,"Data/Textures/Items.bmp",4);
}

void Start2D()
{
	glDisable(GL_DEPTH_TEST);							// Enables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,800,0,600,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();
}

void End2D()
{
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);
}


void CrossHair(){
	glBegin(GL_LINE_STRIP);
	glVertex2f(400,290);
	glVertex2f(400,310);
	glVertex2f(410,300);
	glVertex2f(400,290);
	glVertex2f(390,300);
	glVertex2f(400,310);
	glVertex2f(390,300);
	glVertex2f(410,300);
	glEnd();
}


void WeaponPicture()
{
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	if(weapon==0){
	glTexCoord2f(0,1); glVertex2f(300,60);
	glTexCoord2f(0.5f,1); glVertex2f(350,60);
	glTexCoord2f(0.5f,0.5f); glVertex2f(350,10);
	glTexCoord2f(0,0.5f); glVertex2f(300,10);
	}
	else if(weapon==1)
	{
	glTexCoord2f(0.5f,1); glVertex2f(300,60);
	glTexCoord2f(1,1); glVertex2f(350,60);
	glTexCoord2f(1,0.5f); glVertex2f(350,10);
	glTexCoord2f(0.5f,0.5f); glVertex2f(300,10);
	}
	else if(weapon==2)
	{
	glTexCoord2f(0,0.5f); glVertex2f(300,60);
	glTexCoord2f(0.5f,0.5f); glVertex2f(350,60);
	glTexCoord2f(0.5f,0); glVertex2f(350,10);
	glTexCoord2f(0,0); glVertex2f(300,10);
	}
	glEnd();
}

void DrawAllLines()
{
	glBegin(GL_LINE_STRIP);
	glVertex2f(10,60);
	glVertex2f(130,60);
	glVertex2f(130,10);
	glVertex2f(10,10);
	glVertex2f(10,60);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex2f(350,60);
	glVertex2f(550,60);
	glVertex2f(550,10);
	glVertex2f(350,10);
	glVertex2f(350,60);
	glEnd();

}

int life=100;
int missile=50,mbs;

int whentoadd=200;

void OrthoMode()
{
	Start2D();
	CrossHair();
	DrawAllLines();
	WeaponPicture();
	End2D();
	glPrint(5,12,0,3,3,3,1,0,0,"%d",life);
	if(weapon==0)
	glPrint(365,12,0,3,3,3,0,0,1,"INF");
	else if(weapon==1)
	glPrint(365,12,0,3,3,3,1,1,0,"%d",missile);
	else if(weapon==2)
	glPrint(365,12,0,3,3,3,1,1,0,"%d",mbs);

	if(allkills>whentoadd){whentoadd+=200;mbs+=1;}

}
