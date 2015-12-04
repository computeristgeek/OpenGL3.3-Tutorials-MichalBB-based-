#include "main.h"
#define PLASMA 30000
#define CRAFT 80
#define BULLET 80
extern int ammop;
extern const float piover180;
class Ammo{
public:
	float x,y;
	float damage;
	int number;
	bool one,sound;
	float xinc, yinc;
	bool active;
	int positive;
};



class Enemy{
public:
float x,y;
float life;
bool one;
float timerexpl;
bool count;
};


class Object{
public:
	void draw(float x, float y);
	float x,y;
	bool active;
	bool sound;
};



void DrawShip();
void DrawEnemy(float x,float y);
extern bool hrot;
extern float rotation,life;
extern float x,y,xinc,yinc;
void Ryby();
void DrawAmmo();
void InitAmmo();
void Craft();
void KeyInput();
void Damagea(float x, float y);
void initd();
void TestCollision();
void Vyvolaj();
void EnemyShot();
void Ortho();

extern float mylife;
extern int count;