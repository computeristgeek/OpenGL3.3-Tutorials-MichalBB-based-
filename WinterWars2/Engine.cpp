#include "Weapons.h"
#include "MilkshapeModel.h"
#include "Camera.h"
int weapon=Lasergun;
int lsrammo;
int plasma;
int rocket;
Object LaserAmmo[LASERAMMO];
Object RocketLauncher;
Object PlasmaThrower;
Object Plasmaammo[PLASMAAMMO];
Object Rocketammo[ROCKETAMMO];
Object Health[HEALTH];
Object SuperHealth;
Object Armor[ARMOR];
void PrepareAmmo(){
	for(lsrammo=0;lsrammo<LASERAMMO;lsrammo++){
LaserAmmo[lsrammo].active=TRUE;
LaserAmmo[lsrammo].x=float(rand()%100)*1.0f;
LaserAmmo[lsrammo].z=float(rand()%100)*1.0f;
LaserAmmo[lsrammo].active=TRUE;
	}
		for(plasma=0;plasma<PLASMAAMMO;plasma++){
Plasmaammo[plasma].active=TRUE;
Plasmaammo[plasma].x=-float(plasma)*10.0f;
Plasmaammo[plasma].z=-float(rand()%500)*0.1f;
Plasmaammo[plasma].active=TRUE;
	}

		for(rocket=0;rocket<ROCKETAMMO;rocket++){
Rocketammo[rocket].active=TRUE;
Rocketammo[rocket].x=float(rocket)*10.0f;
Rocketammo[rocket].z=float(rand()%500)*0.1f;
Rocketammo[rocket].active=TRUE;
	}
Rocketammo[3].x=-20;
Rocketammo[3].z=-20;
Plasmaammo[4].x=50;
Plasmaammo[4].z=50;
LaserAmmo[5].x=-27;
LaserAmmo[5].z=-22;
LaserAmmo[9].x=27;
LaserAmmo[9].z=-52;
}

void PrepareWeapons(){
RocketLauncher.x=-50;
RocketLauncher.z=-10;
PlasmaThrower.z=80;
PlasmaThrower.x=50;
PlasmaThrower.active=TRUE;
PlasmaThrower.taken=FALSE;
RocketLauncher.active=TRUE;
RocketLauncher.taken=FALSE;

for(int hel=0;hel<HEALTH;hel++){
	Health[hel].x=-100+(rand()%150)*1.4f;
		Health[hel].z=-100+(rand()%150)*1.38f;
			Health[hel].active=true;
}
SuperHealth.x=-20;
SuperHealth.z=0;
SuperHealth.active=true;

for(int arm=0;arm<ARMOR;arm++){
	Armor[arm].x=100-(rand()%150)*1.4f;
		Armor[arm].z=100-(rand()%150)*1.38f;
			Armor[arm].active=true;
}
}

float rot;
Model *pModel[MODEL];
void Drawthem(){


glPushMatrix();
glTranslatef(RocketLauncher.x,0,RocketLauncher.z);
glRotatef(rot,0,1,0);
glScalef(0.1f,0.1f,0.1f);
if(RocketLauncher.active){
pModel[2]->draw();
}
glPopMatrix();

glPushMatrix();
glTranslatef(PlasmaThrower.x,0,PlasmaThrower.z);
glRotatef(rot,0,1,0);
glScalef(0.1f,0.1f,0.1f);
if(PlasmaThrower.active){
pModel[1]->draw();
}
glPopMatrix();

glPushMatrix();
glScalef(0.15f,0.15f,0.15f);
glTranslatef(-90.0f,-0.2f,90.0f);
pModel[4]->draw();
glPopMatrix();
for(int r=0;r<LASERAMMO;r++){
	glPushMatrix();
glTranslatef(LaserAmmo[r].x,0,LaserAmmo[r].z);
glRotatef(rot,0,1,0);
glScalef(0.03f,0.03f,0.03f);
if(LaserAmmo[r].active){
pModel[5]->draw();
}
glPopMatrix();


// Check if player reached ammo
if(camerax-0.6f<LaserAmmo[r].x && camerax+0.6f>LaserAmmo[r].x && cameraz-0.6f<LaserAmmo[r].z && cameraz+0.6f>LaserAmmo[r].z && LaserAmmo[r].active && ammol<500)
// Add some ammo
{ammol+=50; LaserAmmo[r].active=FALSE; Sound("Data/Sounds/Ammotake.wav");}
// Count time if player took lasers
if(!LaserAmmo[r].active){LaserAmmo[r].respawn+=0.01f;}
// Print for a short time
if(LaserAmmo[r].respawn>0 && LaserAmmo[r].respawn<1.5f){  glPrint(250,200,"Lasers +50",0,1,1,1);}
// When Lasers comes back
if(LaserAmmo[r].respawn>8){LaserAmmo[r].active=TRUE;LaserAmmo[r].respawn=0;}
// Check if laser oubnt is not more than 800
if(ammol>500){ammol=500;}
}

// The code above says us, what has to happen when player collides with weapon
if(camerax-0.8f<PlasmaThrower.x && camerax+0.8f>PlasmaThrower.x && cameraz-0.8f<PlasmaThrower.z && cameraz+0.8f>PlasmaThrower.z && PlasmaThrower.active)
{PlasmaThrower.active=false;Sound("Data/Sounds/Weapontake.wav");weapon=Plasmathrow; PlasmaThrower.taken=true;}
if(camerax-0.7f<RocketLauncher.x && camerax+0.7f>RocketLauncher.x && cameraz-0.7f<RocketLauncher.z && cameraz+0.7f>RocketLauncher.z && RocketLauncher.active){RocketLauncher.active=false;Sound("Data/Sounds/Weapontake.wav");weapon=RocketLNCH;RocketLauncher.taken=true;}
if(!PlasmaThrower.active){PlasmaThrower.respawn+=0.01f;}
// Print a text
if(PlasmaThrower.respawn>0 && PlasmaThrower.respawn<2){glPrint(230,300,"XL 23 Electro Plasma Thrower",0,1,1,1);}
if(PlasmaThrower.respawn>15){PlasmaThrower.active=TRUE; PlasmaThrower.respawn=0;}
if(!RocketLauncher.active){RocketLauncher.respawn+=0.01f;}
if(RocketLauncher.respawn>0 && RocketLauncher.respawn<2){glPrint(230,300,"Rocket Launcher",0,1,1,1);}
if(RocketLauncher.respawn>15){RocketLauncher.active=TRUE; RocketLauncher.respawn=0;}
if(GetKeyState('1') & 0x80){weapon=Lasergun;}
if(GetKeyState('2') & 0x80 && RocketLauncher.taken){weapon=RocketLNCH;}
if(GetKeyState('3') & 0x80 && PlasmaThrower.taken){weapon=Plasmathrow;}

for(int p=0;p<PLASMAAMMO;p++){
	glPushMatrix();
glTranslatef(Plasmaammo[p].x,0,Plasmaammo[p].z);
glRotatef(rot,0,1,0);
glScalef(0.5f,0.5f,0.5f);
if(Plasmaammo[p].active){
pModel[6]->draw();
}
glPopMatrix();


// Check if player reaches plasma
if(camerax-0.6f<Plasmaammo[p].x && camerax+0.6f>Plasmaammo[p].x && cameraz-0.6f<Plasmaammo[p].z && cameraz+0.6f>Plasmaammo[p].z && Plasmaammo[p].active && ammoplasma<250)
{// Add some things and deactivate it
ammoplasma+=25; Plasmaammo[p].active=FALSE; Sound("Data/Sounds/Ammotake.wav");
		}
// Count time if ammo is deactivated
if(!Plasmaammo[p].active){Plasmaammo[p].respawn+=0.01f;}
// Print Plasmas
if(Plasmaammo[p].respawn>0 && Plasmaammo[p].respawn<1.5f){glPrint(250,180,"Plasma +25",0,1,1,1);}
// Display plasma again
if(Plasmaammo[p].respawn>12){Plasmaammo[p].active=TRUE;Plasmaammo[p].respawn=0;}
// If plasmas are more than 250, we want to cap it
if(ammoplasma>250){ammoplasma=250;}
	}

for(int rocket=0;rocket<ROCKETAMMO;rocket++){
	glPushMatrix();
glTranslatef(Rocketammo[rocket].x,0,Rocketammo[rocket].z);
glRotatef(rot,0,1,0);
glScalef(0.12f,0.12f,0.12f);
if(Rocketammo[rocket].active){
pModel[0]->draw();
}
glPopMatrix();



if(camerax-0.6f<Rocketammo[rocket].x && camerax+0.6f>Rocketammo[rocket].x && cameraz-0.6f<Rocketammo[rocket].z && cameraz+0.6f>Rocketammo[rocket].z && Rocketammo[rocket].active && ammorocket<100)
{
	// Add some things and deactivate it
ammorocket+=10; Rocketammo[rocket].active=FALSE; Sound("Data/Sounds/Ammotake.wav");
		}
// Count time if ammo is deactivated
if(!Rocketammo[rocket].active){Rocketammo[rocket].respawn+=0.01f;}
// Print a text
if(Rocketammo[rocket].respawn>0 && Rocketammo[rocket].respawn<1.5f){glPrint(250,190,"Rockets +10",0,1,1,1);}
// Display rockets again
if(Rocketammo[rocket].respawn>9){Rocketammo[rocket].active=TRUE;Rocketammo[rocket].respawn=0;}
// If rockets are more than 100, we want to cap it
if(ammorocket>100){ammorocket=100;}
	}

for(int hel=0;hel<HEALTH;hel++){
	glPushMatrix();
glTranslatef(Health[hel].x,0,Health[hel].z);
glRotatef(rot,0,1,0);
glScalef(0.06f,0.06f,0.06f);
if(Health[hel].active){
pModel[9]->draw();
}
glPopMatrix();	
// Check collision between camera and health
if(camerax-0.6f<Health[hel].x && camerax+0.6f>Health[hel].x && cameraz-0.6f<Health[hel].z && cameraz+0.6f>Health[hel].z && Health[hel].active && life>0 && life<=75)
// Deactivate health and add 25 life
{Health[hel].active=false;Sound("Data/Sounds/Health.wav"); life+=25;}
// But what if our life would be more than 100?
else if(life>=75 && life<100 && camerax-0.6f<Health[hel].x && camerax+0.6f>Health[hel].x && cameraz-0.6f<Health[hel].z && cameraz+0.6f>Health[hel].z && Health[hel].active)
{life+=100-life; Health[hel].active=false; Sound("Data/Sounds/Health.wav");}
// Count time until object comes back
if(!Health[hel].active){Health[hel].respawn+=0.01f;}
// Print Health +25 for a short period time
if(Health[hel].respawn>0 && Health[hel].respawn<1.5f){glPrint(250,210,"Health +25",0,1,1,1);}
// When health comes back
if(Health[hel].respawn>6){Health[hel].active=TRUE;Health[hel].respawn=0;}
}

glPushMatrix();
glTranslatef(SuperHealth.x,0,SuperHealth.z);
glRotatef(rot,0,1,0);
glScalef(0.5f,0.5f,0.5f);
if(SuperHealth.active){
pModel[10]->draw();
}
glPopMatrix();
if(camerax-0.6f<SuperHealth.x && camerax+0.6f>SuperHealth.x && cameraz-0.6f<SuperHealth.z && cameraz+0.6f>SuperHealth.z && SuperHealth.active && life<200)
{
	life+=100;SuperHealth.active=false;Sound("Data/Sounds/Superhealth.wav");
}
if(!SuperHealth.active){SuperHealth.respawn+=0.01f;}
if(SuperHealth.respawn>0 && SuperHealth.respawn<2){glPrint(235,250,"SuperHealth, Health +100",0,1,1,1);}
if(SuperHealth.respawn>17){SuperHealth.active=TRUE;SuperHealth.respawn=0;}
if(life>200)life=200;



for(int arm=0;arm<ARMOR;arm++){
	glPushMatrix();
glTranslatef(Armor[arm].x,0,Armor[arm].z);
glRotatef(rot,0,1,0);
glScalef(0.4f,0.4f,0.4f);
if(Armor[arm].active){
pModel[11]->draw();
}
glPopMatrix();	
// Check collision between camera and health
if(camerax-1<Armor[arm].x && camerax+1>Armor[arm].x && cameraz-1<Armor[arm].z && cameraz+1>Armor[arm].z && Armor[arm].active)

{Armor[arm].active=false;Sound("Data/Sounds/Armor.wav");armora+=25;}
// Count time until object comes back
if(!Armor[arm].active){Armor[arm].respawn+=0.01f;}
// Print Health +25 for a short period time
if(Armor[arm].respawn>0 && Armor[arm].respawn<1.5f){glPrint(250,210,"Armor +25",0,1,1,1);}
// When health comes back
if(Armor[arm].respawn>6){Armor[arm].active=TRUE;Armor[arm].respawn=0;}
if(armora>150){armora=150;}
}


}







void LoadTextures()												
{
		pModel[0]->reloadTextures("Data/Models/Rocket.bmp");
	pModel[1]->reloadTextures("Data/Models/HEG.bmp");
		pModel[2]->reloadTextures("Data/Models/RocketLNCH.bmp");
pModel[3]->reloadTextures("Data/Models/Human.bmp");
pModel[4]->reloadTextures("Data/Models/Tree.bmp");
pModel[5]->reloadTextures("Data/Models/Lasers.bmp");
pModel[6]->reloadTextures("Data/Models/Plasma.bmp");
pModel[7]->reloadTextures("Data/Models/Rocket.bmp");
pModel[8]->reloadTextures("Data/Models/Plasmammo.bmp");
pModel[9]->reloadTextures("Data/Models/Health1.bmp");
pModel[10]->reloadTextures("Data/Models/Health2.bmp");
pModel[11]->reloadTextures("Data/Models/Armor.bmp");
}





void LoadModels(){
pModel[0] = new MilkshapeModel();								
pModel[0]->loadModelData("Data/Models/Rockets.ms3d");
pModel[1] = new MilkshapeModel();									// Memory To Hold The Model
pModel[1]->loadModelData("Data/Models/Hyper Electric Gun.ms3d");		// Loads The Model And Checks For Errors
pModel[2] = new MilkshapeModel();									// Memory To Hold The Model
pModel[2]->loadModelData("Data/Models/Rocket launcher.ms3d");		// Loads The Model And Checks For Errors
pModel[3] = new MilkshapeModel();									// Memory To Hold The Model
pModel[3]->loadModelData("Data/Models/Human.ms3d");		// Loads The Model And Checks For Errors
pModel[4] = new MilkshapeModel();									// Memory To Hold The Model
pModel[4]->loadModelData("Data/Models/Tree.ms3d");		// Loads The Model And Checks For Errors
pModel[5] = new MilkshapeModel();									// Memory To Hold The Model
pModel[5]->loadModelData("Data/Models/Laserammo.ms3d");		// Loads The Model And Checrors
pModel[6] = new MilkshapeModel();									// Memory To Hold The Model
pModel[6]->loadModelData("Data/Models/Plasmaammo.ms3d");		// Loads The Model And Checrors
pModel[7] = new MilkshapeModel();									// Memory To Hold The Model
pModel[7]->loadModelData("Data/Models/Rocket.ms3d");		// Loads The Model And Checrors
pModel[8] = new MilkshapeModel();									// Memory To Hold The Model
pModel[8]->loadModelData("Data/Models/Plasma.ms3d");		// Loads The Model And Checrors
pModel[9] = new MilkshapeModel();									// Memory To Hold The Model
pModel[9]->loadModelData("Data/Models/Health1.ms3d");		// Loads The Model And Checrors
pModel[10] = new MilkshapeModel();									// Memory To Hold The Model
pModel[10]->loadModelData("Data/Models/Health2.ms3d");		// Loads The Model And Checrors
pModel[11] = new MilkshapeModel();									// Memory To Hold The Model
pModel[11]->loadModelData("Data/Models/Armor.ms3d");		// Loads The Model And Checrors
}


void drawarocket(){
	pModel[7]->draw();
}


void DrawPlasma(){
glScalef(1.12f,1.12f,1.12f);
pModel[8]->draw();
}

void DrawEnemy(){
	pModel[3]->draw();
}
