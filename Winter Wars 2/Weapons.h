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
	float x,y,z;
    bool active;
	float xinc,yinc,zinc;
}Ammo;

typedef class Object{
	public:
	float x,y,z;
	bool active;
	float respawn;
	bool taken;
}Object;

typedef class Player{
	public:
		float x,y,z;
		float life,armor;
}Player;
void LoadTextures();
void LoadModels();
void Drawthem();
void PrepareAmmo();
void PrepareWeapons();
extern float rot;
extern float camerax,cameraz;
extern int weapon;
void drawarocket();
void DisplayLaser();
void InitLaser(); 
void InitAmmo();
GLvoid glPrint(GLint x, GLint y, char *string, int set,float scalex, float scaley, float scalez);
extern int count;
extern int ammorocket;
extern int ammoplasma;
extern int ammol;
extern int lasercount,rocketcount,plasmacount,enemyammo;
extern int life,armora;
void DrawPlasma();
void DrawEnemy();
void DisplayLasers();
void DisplayRockets();
void DisplayPlasmas();
void EnemyAmmo();
void DoWhatYouReSupposedToDo();
extern Player Enemy;
extern Ammo enemolas[100];
extern Object LaserAmmo[LASERAMMO];
extern Object RocketLauncher;
extern Object PlasmaThrower;
extern Object Plasmaammo[PLASMAAMMO];
extern Object Rocketammo[ROCKETAMMO];
extern Object SuperHealth;
extern Object Armor[ARMOR];
extern float timer,inc;
extern int myfrag,compfrag;

extern bool keys[5];
void EnemyMain();
void EnemyTime();
void Menu();
void Shoot(float lasertime,float rockettime, float plasmatime);
