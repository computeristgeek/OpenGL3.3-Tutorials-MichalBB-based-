//************************************* Enemies.cpp *******************************************
// 
//						Here is all enemies' artificial intelligence.
//
//************************************* Enemies.cpp *******************************************

#include "Main.h"

void Enemy::Draw(){}

//////////////////////////////////// MACHINER::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Renders machine gunner using Machiner's class data.
//
//////////////////////////////////// MACHINER::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Machiner::Draw()
{
	glColor3f(1,1,1); // Set color to white
	glPushMatrix(); // Push the matrix
	glTranslatef(mvPos.x,mvPos.y,0); // Translate position
	glRotatef(-RotateAfter(mvPos,g_soldier.mvPos),0,0,1); // Rotate
	glBindTexture(GL_TEXTURE_2D, textures[4]); // Bind soldier's texture
	// Render him
	glBegin(GL_QUADS); 
	glTexCoord2f(0,1); glVertex2f(-2,2);
	glTexCoord2f(1,1); glVertex2f(2,2);
	glTexCoord2f(1,0); glVertex2f(2,-2);
	glTexCoord2f(0,0); glVertex2f(-2,-2);
	glEnd();
	// End of rendering
	glPopMatrix(); // Pop the matrix
	if(Distance(mvPos,g_soldier.mvPos)>10) // If is far away
	{
		CVector2 vInc=g_soldier.mvPos-mvPos; // Increase soldier's position
		vInc/=Magnitude(vInc);
		vInc/=15;
		mvPos+=v(vInc);
	}

	for(int s=0;s<g_soldier.bullets.size();s++)
	{
		// If collision happens, subtract life and delete bullet
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.bullets[s].mvPos,mvPos))
		{life-=2;DeleteFromVector(g_soldier.bullets,s);}
	}
	for(s=0;s<g_soldier.rockets.size();s++)
	{
		// If collision happens, subtract life and delete rocket
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.rockets[s].mvPos,mvPos))
		{life-=9;DeleteFromVector(g_soldier.rockets,s);}
	}
	for(s=0;s<g_soldier.bolts.size();s++)
	{
		// If collision happens, subtract life and delete bolt
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.bolts[s].mvPos,mvPos))
		{life-=7;DeleteFromVector(g_soldier.bolts,s);}
	}
	glDisable(GL_TEXTURE_2D); // Disable texturing
	for(s=0;s<bullets.size();s++)
	{
		glPushMatrix(); // Push the matrix
		glTranslatef(bullets[s].mvPos.x,bullets[s].mvPos.y,0);// Translate position
		bullets[s].Draw(); // Render bullets
		glPopMatrix(); // Pop the matrix
		// If collision happens, subtract soldier's life and delete bullet
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),bullets[s].mvPos,g_soldier.mvPos))
		{g_soldier.life-=1;DeleteFromVector(bullets,s);}
	}
	glEnable(GL_TEXTURE_2D); // Enable texturing
	shtime+=f(0.01f); // Increase shoottime
	if(shtime>0.2f) // If it is time to shoot
	{
		shtime=0; // Reset shoottime
		CVector2 vInc=g_soldier.mvPos-mvPos; // Create inc vector
		vInc/=Magnitude(vInc); // Normalize
		vInc/=2; // Slow down
		bullets.push_back(Bullet(mvPos,vInc)); // Create bullet
		FSOUND_PlaySound(FSOUND_FREE,sounds[0]); // Play sound
	}
}

////////////////////////////////////// ROBOT::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							Renders robot using Robot's class data.
//
////////////////////////////////////// ROBOT::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Robot::Draw()
{
	glColor3f(1,1,1); // Set color to white
	glPushMatrix(); // Push the matrix
	glTranslatef(mvPos.x,mvPos.y,0); // Translate position
	glRotatef(-RotateAfter(mvPos,g_soldier.mvPos),0,0,1); // Rotate
	glBindTexture(GL_TEXTURE_2D, textures[5]); // Bind soldier's texture
	// Render him
	glBegin(GL_QUADS); 
	glTexCoord2f(0,1); glVertex2f(-2,2);
	glTexCoord2f(1,1); glVertex2f(2,2);
	glTexCoord2f(1,0); glVertex2f(2,-2);
	glTexCoord2f(0,0); glVertex2f(-2,-2);
	glEnd();
	// End of rendering
	glPopMatrix(); // Pop the matrix
	if(Distance(mvPos,g_soldier.mvPos)>10) // If is far away
	{
		CVector2 vInc=g_soldier.mvPos-mvPos; // Increase soldier's position
		vInc/=Magnitude(vInc);
		vInc/=10;
		mvPos+=v(vInc);
	}

	for(int s=0;s<g_soldier.bullets.size();s++)
	{
		// If collision happens, subtract life and delete bullet
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.bullets[s].mvPos,mvPos))
		{life-=2;DeleteFromVector(g_soldier.bullets,s);}
	}
	for(s=0;s<g_soldier.rockets.size();s++)
	{
		// If collision happens, subtract life and delete rocket
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.rockets[s].mvPos,mvPos))
		{life-=9;DeleteFromVector(g_soldier.rockets,s);}
	}
	for(s=0;s<g_soldier.bolts.size();s++)
	{
		// If collision happens, subtract life and delete bolt
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.bolts[s].mvPos,mvPos))
		{life-=7;DeleteFromVector(g_soldier.bolts,s);}
	}
	glDisable(GL_TEXTURE_2D); // Disable texturing
	for(s=0;s<bolts.size();s++)
	{
		glPushMatrix(); // Push the matrix
		glTranslatef(bolts[s].mvPos.x,bolts[s].mvPos.y,0);// Translate position
		glRotatef(bolts[s].angle,0,0,1); // Rotate
		bolts[s].Draw(); // Render bullets
		glPopMatrix(); // Pop the matrix
		// If collision happens, subtract soldier's life and delete bullet
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),bolts[s].mvPos,g_soldier.mvPos))
		{g_soldier.life-=4;DeleteFromVector(bolts,s);}
	}
	glEnable(GL_TEXTURE_2D); // Enable texturing
	shtime+=f(0.01f); // Increase shoottime
	if(shtime>0.3f) // If it is time to shoot
	{
		shtime=0; // Reset shoottime
		CVector2 vInc=g_soldier.mvPos-mvPos; // Create inc vector
		vInc/=Magnitude(vInc); // Normalize
		vInc/=2; // Slow down
		bolts.push_back(Bolt(mvPos,vInc,-RotateAfter(mvPos,g_soldier.mvPos))); // Create bullet
		FSOUND_PlaySound(FSOUND_FREE,sounds[2]); // Play sound
	}
}

/////////////////////////////////////// CAN::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							Renders can using Can's class data.
//
/////////////////////////////////////// CAN::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Can::Draw()
{
	glColor3f(1,1,1); // Set color to white
	glPushMatrix(); // Push the matrix
	glTranslatef(mvPos.x,mvPos.y,0); // Translate position
	glRotatef(-RotateAfter(mvPos,g_soldier.mvPos),0,0,1); // Rotate
	glBindTexture(GL_TEXTURE_2D, textures[10]); // Bind soldier's texture
	// Render him
	glBegin(GL_QUADS); 
	glTexCoord2f(0,1); glVertex2f(-2,2);
	glTexCoord2f(1,1); glVertex2f(2,2);
	glTexCoord2f(1,0); glVertex2f(2,-2);
	glTexCoord2f(0,0); glVertex2f(-2,-2);
	glEnd();
	// End of rendering
	glPopMatrix(); // Pop the matrix
	if(Distance(mvPos,g_soldier.mvPos)>10) // If is far away
	{
		CVector2 vInc=g_soldier.mvPos-mvPos; // Increase soldier's position
		vInc/=Magnitude(vInc);
		vInc/=25;
		mvPos+=v(vInc);
	}

	for(int s=0;s<g_soldier.bullets.size();s++)
	{
		// If collision happens, subtract life and delete bullet
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.bullets[s].mvPos,mvPos))
		{life-=2;DeleteFromVector(g_soldier.bullets,s);}
	}
	for(s=0;s<g_soldier.rockets.size();s++)
	{
		// If collision happens, subtract life and delete rocket
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.rockets[s].mvPos,mvPos))
		{life-=9;DeleteFromVector(g_soldier.rockets,s);}
	}
	for(s=0;s<g_soldier.bolts.size();s++)
	{
		// If collision happens, subtract life and delete bolt
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),g_soldier.bolts[s].mvPos,mvPos))
		{life-=7;DeleteFromVector(g_soldier.bolts,s);}
	}
	for(s=0;s<rockets.size();s++)
	{
		glPushMatrix(); // Push the matrix
		glTranslatef(rockets[s].mvPos.x,rockets[s].mvPos.y,0);// Translate position
		glRotatef(rockets[s].angle,0,0,1); // Rotate
		rockets[s].Draw(); // Render bullets
		glPopMatrix(); // Pop the matrix
		// If collision happens, subtract soldier's life and delete bullet
		if(CBCollide(CVector2(0.1f,0.1f),CVector2(2,2),rockets[s].mvPos,g_soldier.mvPos))
		{g_soldier.life-=9;DeleteFromVector(rockets,s);}
	}
	shtime+=f(0.01f); // Increase shoottime
	if(shtime>0.6f) // If it is time to shoot
	{
		shtime=0; // Reset shoottime
		CVector2 vInc=g_soldier.mvPos-mvPos; // Create inc vector
		vInc/=Magnitude(vInc); // Normalize
		vInc/=2; // Slow down
		rockets.push_back(Rocket(mvPos,vInc,-RotateAfter(mvPos,g_soldier.mvPos))); // Create bullet
		FSOUND_PlaySound(FSOUND_FREE,sounds[1]); // Play sound
	}
}

//////////////////////////////////////// ADDENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						This increases enemies count in random time.
//
//////////////////////////////////////// ADDENEMY \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void AddEnemy()
{
	static float timetoadd=1.2f,time; // Creaate static variables
	static int enpushed;
	time+=f(0.01f); // Increase time
	if(time>timetoadd) // If time has come
	{
		int lucky=rand()%2; // To know where to put enemy
		CVector2 randpos; // Random position
		if(lucky)
		{
			randpos.x=(rand()%2)?-27:27; // Left or right
			randpos.y=-20+rand()%40; // Up and down is random
		}
		else
		{
			randpos.x=-27+(rand()%54); // Left and right is random
			randpos.y=(rand()%2)?-20:20; // Up or down
		}
		timetoadd=0.7f+(float)(rand()%50)/100; // Set time to add enemy
		time=0; // Reset time
		if(enpushed<97) // If there are less than 97 enemies pushed
		{
			enpushed++; // Increase enpushed
			int random=rand()%20; // Create random enemy
			if(random<11)
			machiners.push_back(Machiner(randpos)); // Push machine gunner
			else if(random>10 && random<17)robots.push_back(Robot(randpos)); // Push robot
			else cans.push_back(Can(randpos)); // Push can
		}
		else if(enpushed>96 && enpushed<100){enpushed++;cans.push_back(Can(randpos));} // Push can
	}
}
	

