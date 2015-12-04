#include "main.h"

UINT a[2];


void BuildAmmo(){
a[1]=glGenLists(1);
glNewList(a[1],GL_COMPILE);
	glBegin(GL_QUADS);
	glColor3f(0,1,0);
glTexCoord2f(0,1);	glVertex3f(-0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,0.18f);
glTexCoord2f(1,0);	glVertex3f(0.02f,-0.02f,0.18f);
glTexCoord2f(0,0);	glVertex3f(-0.02f,-0.02f,0.18f);
	//''''''''''''''''''''''''''''
glTexCoord2f(0,1);	glVertex3f(-0.02f,0.02f,-0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,-0.18f);
glTexCoord2f(0,1);	glVertex3f(0.02f,-0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(-0.02f,0.02f,-0.18f);
	//''''''''''''''''''
glTexCoord2f(1,1);	glVertex3f(-0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(-0.02f,0.02f,-0.18f);
glTexCoord2f(0,1);	glVertex3f(-0.02f,-0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(-0.02f,-0.02f,0.18f);
	//;;;;;;;;;;;;;;;;;;;;;;;;;
glTexCoord2f(0,1);	glVertex3f(0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(0.02f,-0.02f,-0.18f);
glTexCoord2f(0,1);	glVertex3f(0.02f,-0.02f,0.18f);
	//''''''''''''''''''''''''''''''''
glTexCoord2f(0,0);	glVertex3f(-0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(-0.02f,0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(0.02f,0.02f,-0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,0.18f);
//'''''''''''''''''''''''''''''''''''''''
glTexCoord2f(0,1);	glVertex3f(-0.02f,-0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(-0.02f,-0.02f,-0.18f);
glTexCoord2f(1,0);	glVertex3f(0.02f,-0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(0.02f,-0.02f,0.18f);
	glEnd();
	glColor3f(1,1,1);
glEndList();

a[2]=glGenLists(1);
glNewList(a[2],GL_COMPILE);
	glBegin(GL_QUADS);
	glColor3f(1,0,0);
glTexCoord2f(0,1);	glVertex3f(-0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,0.18f);
glTexCoord2f(1,0);	glVertex3f(0.02f,-0.02f,0.18f);
glTexCoord2f(0,0);	glVertex3f(-0.02f,-0.02f,0.18f);
	//''''''''''''''''''''''''''''
glTexCoord2f(0,1);	glVertex3f(-0.02f,0.02f,-0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,-0.18f);
glTexCoord2f(0,1);	glVertex3f(0.02f,-0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(-0.02f,0.02f,-0.18f);
	//''''''''''''''''''
glTexCoord2f(1,1);	glVertex3f(-0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(-0.02f,0.02f,-0.18f);
glTexCoord2f(0,1);	glVertex3f(-0.02f,-0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(-0.02f,-0.02f,0.18f);
	//;;;;;;;;;;;;;;;;;;;;;;;;;
glTexCoord2f(0,1);	glVertex3f(0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(0.02f,-0.02f,-0.18f);
glTexCoord2f(0,1);	glVertex3f(0.02f,-0.02f,0.18f);
	//''''''''''''''''''''''''''''''''
glTexCoord2f(0,0);	glVertex3f(-0.02f,0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(-0.02f,0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(0.02f,0.02f,-0.18f);
glTexCoord2f(1,1);	glVertex3f(0.02f,0.02f,0.18f);
//'''''''''''''''''''''''''''''''''''''''
glTexCoord2f(0,1);	glVertex3f(-0.02f,-0.02f,0.18f);
glTexCoord2f(1,1);	glVertex3f(-0.02f,-0.02f,-0.18f);
glTexCoord2f(1,0);	glVertex3f(0.02f,-0.02f,-0.18f);
glTexCoord2f(0,0);	glVertex3f(0.02f,-0.02f,0.18f);
	glEnd();
	glColor3f(1,1,1);
glEndList();

}

void Sound(char*name){
	PlaySound(name,NULL,SND_ASYNC);
}

