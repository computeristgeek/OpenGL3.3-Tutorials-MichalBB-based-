//************************************** Ammo.cpp *********************************************
// 
//						 Here are functions that works with ammo.
//
//************************************** Ammo.cpp *********************************************

#include "Main.h"

void Ammo::Draw();

////////////////////////////////////// BULLET::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//									Renders a bullet.
//
////////////////////////////////////// BULLET::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Bullet::Draw()
{
	glColor3f(0.8f,0.8f,0.8f); // Set grey color
	glBegin(GL_QUADS); // Render
	glVertex2f(-0.1f,0.1f);
	glVertex2f(0.1f,0.1f);
	glVertex2f(0.1f,-0.1f);
	glVertex2f(-0.1f,-0.1f);
	glEnd();
	mvPos+=v(mvInc); // Update position
	life-=f(0.01f); // Subtract life
}

////////////////////////////////////// ROCKET::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//									Renders a rocket.
//
////////////////////////////////////// ROCKET::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Rocket::Draw()
{
	glColor3f(1,1,1); // Set color to white
	glBindTexture(GL_TEXTURE_2D, textures[3]); // Bind rocket texture
	glBegin(GL_QUADS); // Render
	glTexCoord2f(0,1); glVertex2f(-0.25f,0.4f);
	glTexCoord2f(1,1); 	glVertex2f(0.25f,0.4f);
	glTexCoord2f(1,0); 	glVertex2f(0.25f,-0.4f);
	glTexCoord2f(0,0); 	glVertex2f(-0.25f,-0.4f);
	glEnd();
	mvPos+=v(mvInc); // Update position
	life-=f(0.01f); // Subtract life
}

/////////////////////////////////////// BOLT::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//									Renders a bolt.
//
/////////////////////////////////////// BOLT::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Bolt::Draw()
{
	glColor3f(0,1,1); // Set bright blue color
	glBegin(GL_LINE_STRIP); // Render
	glVertex2f(0,-0.3f);
	glVertex2f(-0.2f,-0.1f);
	glVertex2f(0.2f,0.1f);
	glVertex2f(0,0.3f);
	glEnd();
	mvPos+=v(mvInc); // Update position
	life-=f(0.01f); // Subtract life
}
