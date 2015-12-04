#include "main.h"
#define PLASMA 30000
#define CRAFT 80
#define BULLET 80
extern GLint ammop;
extern const GLfloat piover180;
class Ammo{
public:
	GLfloat x,y;
	GLfloat damage;
	GLint number;
	GLboolean one,sound;
	GLfloat xinc, yinc;
	GLboolean active;
	GLint positive;
};



class Enemy{
public:
GLfloat x,y;
GLfloat life;
GLboolean one;
GLfloat timerexpl;
GLboolean count;
};


class Object{
public:
	GLvoid draw(GLfloat x, GLfloat y);
	GLfloat x,y;
	GLboolean active;
	GLboolean sound;
};



GLvoid DrawShip();
GLvoid DrawEnemy(GLfloat x,GLfloat y);
extern GLboolean hrot;
extern GLfloat rotation,life;
extern GLfloat x,y,xinc,yinc;
GLvoid Ryby();
GLvoid DrawAmmo();
GLvoid InitAmmo();
GLvoid Craft();
GLvoid KeyInput();
GLvoid Damagea(GLfloat x, GLfloat y);
GLvoid initd();
GLvoid TestCollision();
GLvoid Vyvolaj();
GLvoid EnemyShot();
GLvoid Ortho();

extern GLfloat mylife;
extern GLint count;