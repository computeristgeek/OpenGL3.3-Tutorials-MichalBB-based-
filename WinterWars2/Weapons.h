#include "main.h"

#define MODEL 12

#define Lasergun 1
#define Plasmathrow 3
#define RocketLNCH 2
#define LASERAMMO 10
#define PLASMAAMMO 5
#define ROCKETAMMO 7
#define HEALTH 8
#define ARMOR 8 
typedef class Ammo{
	public:
	GLfloat x,y,z;
    GLboolean active;
	GLfloat xinc,yinc,zinc;
}Ammo;

typedef class Object{
	public:
	GLfloat x,y,z;
	GLboolean active;
	GLfloat respawn;
	GLboolean taken;
}Object;

typedef class Player{
	public:
		GLfloat x,y,z;
		GLfloat life,armor;
}Player;
GLvoid LoadTextures();
GLvoid LoadModels();
GLvoid Drawthem();
GLvoid PrepareAmmo();
GLvoid PrepareWeapons();
extern GLfloat rot;
extern GLfloat camerax,cameraz;
extern GLint weapon;
GLvoid drawarocket();
GLvoid DisplayLaser();
GLvoid InitLaser(); 
GLvoid InitAmmo();
GLGLvoid glPrint(GLGLint x, GLGLint y, GLchar *string, GLint set,GLfloat scalex, GLfloat scaley, GLfloat scalez);
extern GLint count;
extern GLint ammorocket;
extern GLint ammoplasma;
extern GLint ammol;
extern GLint lasercount,rocketcount,plasmacount,enemyammo;
extern GLint life,armora;
GLvoid DrawPlasma();
GLvoid DrawEnemy();
GLvoid DisplayLasers();
GLvoid DisplayRockets();
GLvoid DisplayPlasmas();
GLvoid EnemyAmmo();
GLvoid DoWhatYouReSupposedToDo();
extern Player Enemy;
extern Ammo enemolas[100];
extern Object LaserAmmo[LASERAMMO];
extern Object RocketLauncher;
extern Object PlasmaThrower;
extern Object Plasmaammo[PLASMAAMMO];
extern Object Rocketammo[ROCKETAMMO];
extern Object SuperHealth;
extern Object Armor[ARMOR];
extern GLfloat timer,inc;
extern GLint myfrag,compfrag;

extern GLboolean keys[5];
GLvoid EnemyMain();
GLvoid EnemyTime();
GLvoid Menu();
GLvoid Shoot(GLfloat lasertime,GLfloat rockettime, GLfloat plasmatime);
