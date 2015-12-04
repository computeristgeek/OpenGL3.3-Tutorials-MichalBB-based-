//************************************* Soldier.cpp *******************************************
// 
//						Here is all code that works with Soldier class.
//
//************************************* Soldier.cpp *******************************************

#include "Main.h"

//////////////////////////////////// SOLDIER::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//						Renders soldier using Soldier's class data.
//
//////////////////////////////////// SOLDIER::DRAW \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Soldier::Draw()
{
	glColor3f(1,1,1); // Set color to white
	glPushMatrix(); // Push the matrix
	glTranslatef(mvPos.x,mvPos.y,0); // Translate position
	glRotatef(-RotateAfter(mvPos,mvShoot),0,0,1); // Rotate
	glBindTexture(GL_TEXTURE_2D, textures[weapon]); // Bind soldier's texture
	// Render him
	glBegin(GL_QUADS); 
	glTexCoord2f(0,1); glVertex2f(-2,2);
	glTexCoord2f(1,1); glVertex2f(2,2);
	glTexCoord2f(1,0); glVertex2f(2,-2);
	glTexCoord2f(0,0); glVertex2f(-2,-2);
	glEnd();
	// End of rendering
	glPopMatrix(); // Pop the matrix

	glPushMatrix(); // Push the matrix
	glTranslatef(mvShoot.x,mvShoot.y,0);// Translate position
	glDisable(GL_TEXTURE_2D); // Disable texturing
	// Render crosshair
	glBegin(GL_LINES);glVertex2f(0,0.5f);glVertex2f(0,-0.5f);
	glVertex2f(-0.5f,0);glVertex2f(0.5f,0);glEnd();
	// End of rendering
	glPopMatrix(); // Pop the matrix

	for(int s=0;s<bullets.size();s++)
	{
		glPushMatrix(); // Push the matrix
		glTranslatef(bullets[s].mvPos.x,bullets[s].mvPos.y,0);// Translate position
		bullets[s].Draw(); // Render bullets
		glPopMatrix(); // Pop the matrix
		if(bullets[s].life<0)DeleteFromVector(bullets,s); // Delete bullet if its life was under 0
	}

	glLineWidth(2.5f); // Set line width
	for(s=0;s<bolts.size();s++)
	{
		glPushMatrix(); // Push the matrix
		glTranslatef(bolts[s].mvPos.x,bolts[s].mvPos.y,0); // Translate position
		glRotatef(bolts[s].angle,0,0,1); // Rotate
		bolts[s].Draw(); // Render
		glPopMatrix(); // Pop the matrix
		if(bolts[s].life<0)DeleteFromVector(bolts,s); // Delete bolt if its life was under 0
	}
	glLineWidth(1); // Set line width back
	glEnable(GL_TEXTURE_2D); // Enable texturing

	for(s=0;s<rockets.size();s++)
	{
		glPushMatrix(); // Push the matrix
		glTranslatef(rockets[s].mvPos.x,rockets[s].mvPos.y,0); // Translate position
		glRotatef(rockets[s].angle,0,0,1); // Rotate
		rockets[s].Draw(); // Render
		glPopMatrix(); // Pop the matrix
		if(rockets[s].life<0)DeleteFromVector(rockets,s); // Delete rocket if its life was under 0
	}

	if(keys(VK_UP) || keys('W'))mvPos.y+=f(0.1f); // Update soldier's positions with keys
	if(keys(VK_DOWN) || keys('S'))mvPos.y-=f(0.1f);
	if(keys(VK_LEFT) || keys('A'))mvPos.x-=f(0.1f);
	if(keys(VK_RIGHT) || keys('D'))mvPos.x+=f(0.1f);
	shtime+=f(0.01f); // Increase shoot time
	if(onekey(VK_RBUTTON)){weapon++; if(weapon>2)weapon=0;} // Change weapon

	if(weapon==0) // If weapon is chaingun
	{
		if(shtime>0.02f && keys(VK_LBUTTON)) // If it is time to shoot
		{
			shtime=0; // Reset shoottime
			if(ammo[0]>0) // If ammo more than 0
			{
				CVector2 vInc=mvShoot-mvPos; // Create inc vector
				vInc/=Magnitude(vInc); // Normalize
				bullets.push_back(Bullet(mvPos,vInc)); // Create bullet
				FSOUND_PlaySound(FSOUND_FREE,sounds[3]); // Play sound
				ammo[0]-=1; // Subtract 1 ammo
			}
		}
	}
	else if(weapon==1) // If weapon is rocket launcher
	{
		if(shtime>0.1f && keys(VK_LBUTTON)) // If it is time to shoot
		{
			shtime=0; // Reset shoottime
			if(ammo[1]>0) // If ammo more than 0
			{
				CVector2 vInc=mvShoot-mvPos; // Create inc vector
				vInc/=Magnitude(vInc); // Normalize
				vInc/=1.8f; // Slow down a little bit
				rockets.push_back(Rocket(mvPos,vInc,-RotateAfter(mvPos,mvShoot))); // Create rocket
				FSOUND_PlaySound(FSOUND_FREE,sounds[1]); // Play sound
				ammo[1]-=1; // Subtract 1 ammo
			}
		}
	}
	else if(weapon==2) // If weapon is bolt gun
	{
		if(shtime>0.07f && keys(VK_LBUTTON)) // If it is time to shoot
		{
			shtime=0; // Reset shoottime
			if(ammo[2]>0) // If ammo more than 0
			{
				CVector2 vInc=mvShoot-mvPos; // Create inc vector
				vInc/=Magnitude(vInc); // Normalize
				vInc/=1.5f; // Slow down a little bit
				bolts.push_back(Bolt(mvPos,vInc,-RotateAfter(mvPos,mvShoot))); // Create bolt
				FSOUND_PlaySound(FSOUND_FREE,sounds[2]); // Play sound
				ammo[2]-=1; // Subtract 1 ammo
			}
		}
	}
	POINT mp; // Create Mouse Point structure
	GetCursorPos(&mp); // Get position of cursor
	mvShoot.x=-27+(float)mp.x/14.8f; // Calculate crosshair's position
	mvShoot.y=20-(float)mp.y/14.8f;
}

///////////////////////////////// SOLDIER::DISPLAYINFO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//							Renders soldier's data- ammo and life.
//
///////////////////////////////// SOLDIER::DISPLAYINFO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void Soldier::DisplayInfo()
{
	Start2D(); // Start 2D screen
	glDisable(GL_TEXTURE_2D); // Disable texturing
	glColor3f(1,1,1); // Set color to white
	// Render white lines
	glBegin(GL_LINE_STRIP);
	glVertex2f(10,10);glVertex2f(50,10);glVertex2f(50,30);glVertex2f(10,30);glVertex2f(10,10);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(70,10);glVertex2f(130,10);glVertex2f(130,30);glVertex2f(130,30);glVertex2f(70,30);glVertex2f(70,10);
	glEnd();
	// End of white lines rendering

	glEnable(GL_TEXTURE_2D); // Enable texturing
	End2D(); // End 2D screen

	if(life>200)life=200;
	PrintText(CVector2(20,20),1,Rgb(0,1,0),0,NOTHING,"%d",life); // Print life
	PrintText(CVector2(80,20),1,Rgb(1,1,0),0,NOTHING,"%d",ammo[weapon]); // Print ammo
}

////////////////////////////////////// PLAYINTRO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//									Plays intro.
//
////////////////////////////////////// PLAYINTRO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void PlayIntro()
{
	MSG msg;
	bool end=false;
	while(1)											// Do our infinate loop
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 	// Check if there was a message
        { 
			if(msg.message == WM_QUIT)					// If the message wasn't to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{
			UpdateFPS5();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			gluLookAt(0,0,50,0,0,0,0,1,0);
			PrintText(CVector2(0,550),2.5f,Rgb(0,0,1),0,CENTER,"Fight for life");
			PrintText(CVector2(0,500),1,Rgb(1,0,0),0,CENTER,"by Megabyte Softworks");
			PrintText(CVector2(0,450),1,Rgb(0,0.5f,0),0,CENTER,"Unknown gang attacked military base! You must protect it!!!");
			glColor3f(1,1,1);
			glBindTexture(GL_TEXTURE_2D, textures[4]); // Bind soldier's texture
			Start2D();
			glBegin(GL_QUADS); 
			glTexCoord2f(0,1); glVertex2f(10,400);
			glTexCoord2f(1,1); glVertex2f(100,400);
			glTexCoord2f(1,0); glVertex2f(100,310);
			glTexCoord2f(0,0); glVertex2f(10,310);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, textures[5]); // Bind soldier's texture
			glBegin(GL_QUADS); 
			glTexCoord2f(0,1); glVertex2f(10,300);
			glTexCoord2f(1,1); glVertex2f(100,300);
			glTexCoord2f(1,0); glVertex2f(100,210);
			glTexCoord2f(0,0); glVertex2f(10,210);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, textures[10]); // Bind soldier's texture
			glBegin(GL_QUADS); 
			glTexCoord2f(0,1); glVertex2f(10,200);
			glTexCoord2f(1,1); glVertex2f(100,200);
			glTexCoord2f(1,0); glVertex2f(100,110);
			glTexCoord2f(0,0); glVertex2f(10,110);
			glEnd();
			End2D();
			PrintText(CVector2(110,355),1,Rgb(0,0,0),0,NOTHING,"Machine gunner: Weak enemy, that will support you with ammo.");
			PrintText(CVector2(110,255),1,Rgb(0,0,0),0,NOTHING,"Robot: Powerful enemy, that strikes you with rain of bolts.");
			PrintText(CVector2(110,155),1,Rgb(0,0,0),0,NOTHING,"The Can: Most powerful enemy. Heavy armored tank with rocket launcher.");
			PrintText(CVector2(0,80),1,Rgb(0,0,0),0,CENTER,"Press ESCAPE to quit or ENTER to continue.");

			if(onekey(VK_ESCAPE)){quitapp=true;break;}
			if(onekey(VK_RETURN))break;
			SwapBuffers(g_DC);
		}
	}
	glClearColor(0,0.5f,0,1); // Clear color to green
}

////////////////////////////////////// PLAYOUTRO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
//
//									Plays outro.
//
////////////////////////////////////// PLAYOUTRO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void PlayOutro()
{
	glClearColor(1,1,1,1); // Clear color to green
	MSG msg;
	bool end=false;
	while(1)											// Do our infinate loop
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 	// Check if there was a message
        { 
			if(msg.message == WM_QUIT)					// If the message wasn't to quit
				break;
            TranslateMessage(&msg);						// Find out what the message does
            DispatchMessage(&msg);						// Execute the message
        }
		else											// if there wasn't a message
		{
			UpdateFPS5();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			gluLookAt(0,0,50,0,0,0,0,1,0);
			if(win)PrintText(CVector2(0,550),1.5f,Rgb(0,0,1),0,CENTER,"You won! Press ESCAPE to exit!");
			else PrintText(CVector2(0,550),1.5f,Rgb(0,0,1),0,CENTER,"You lost! Press ESCAPE to exit!");
			if(onekey(VK_ESCAPE)){quitapp=true;break;}
			SwapBuffers(g_DC);
		}
	}
}