#include "main.h"
#include "Weapons.h"
Object Damage;
void DrawShip(){
		glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,g_Texture[0]);
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
glTexCoord2f(0,1); glVertex3f(-10.0f,20,-5.0f);
glTexCoord2f(1,1); glVertex3f(40.0f,20,-5.0f);
glTexCoord2f(1,0); glVertex3f(40.0f,-20,-5.0f);
glTexCoord2f(0,0); glVertex3f(-10.0f,-20,-5.0f);
glEnd();

glPopMatrix();

glPushMatrix();
glTranslatef(x,y,0);
glRotatef(rotation,0,0,1);
	glBegin (GL_TRIANGLES);	
	glColor3f(0,0,0);
		glVertex2f(-0.1f, 0.05f);
		glColor3f(1,1,1);
		glVertex2f(0.1f, 0.05f);
		glColor3f(0,0,0);
		glVertex2f(0.0f,0.35f);	
		glColor3f(255, 0, 0);						
		glVertex3f(0, 0.7f, 0);						
		glColor3f(0, 255, 0);							
		glVertex3f(-0.3f, 0, 0);						
		glColor3f(0, 0, 255);							
		glVertex3f(0.3f, 0, 0);	

	glEnd();										
	if(hrot){
		glBegin(GL_LINES);
			glColor3f(1.0f,0.5f,0);
		glVertex2f(0,0);
		glVertex2f(-0.2f,-0.5f);
		glVertex2f(0,0);
		glVertex2f(-0.1f,-0.5f);
		glVertex2f(0,0);
		glVertex2f(0.0f,-0.5f);
		glVertex2f(0,0);
		glVertex2f(0.1f,-0.5f);
		glVertex2f(0,0);
		glVertex2f(0.2f,-0.5f);
		glEnd();
	}
glPopMatrix();
}

void DrawEnemy(float x,float y){
	glPushMatrix();
	glTranslatef(x,y,0);
	glBegin(GL_TRIANGLES);

	glColor3f(1,0.5f,0);
    glVertex2f(0,0.5f);
	glColor3f(0,0.5f,0.5f);
	glVertex2f(0.5f,-0.5f);
	glColor3f(1,0.0f,0);
	glVertex2f(-0.5f,-0.5f);
	glEnd();

	glPopMatrix();
}