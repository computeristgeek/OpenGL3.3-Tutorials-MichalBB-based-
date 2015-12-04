unit path_of_heroes; 

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, Controls, Graphics, Dialogs,
  ExtCtrls, GL, GLU, OpenGLContext, LCLType, StdCtrls, MMSystem,
  Math3D, UFileFunc, UMBSModel, UTexture, UWorld, UFont, UGLWindowAndForm, Windows, UHero, UParticle;


var sPlayerName : string;
var fBLinkTime : single;

type

  { TForm1 }

  TForm1 = class(TForm)
    OpenGLControl1: TOpenGLControl;
    StaticText1: TStaticText;
    StaticText2: TStaticText;
    Timer1: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure OpenGLControl1Paint(Sender: TObject);
    procedure OpenGLControl1Resize(Sender: TObject);
    procedure StaticText2Click(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end;


var Form1: TForm1;

var prev, cur : LongWord;
var mul : single;
function sof(f:single):single;

implementation



function sof(f:single):single; begin; sof := f*mul; end;

procedure TForm1.FormCreate(Sender: TObject);
var i : integer;
begin
     randomize;
    OpenGLControl1:=TOpenGLControl.Create(Self);
    with OpenGLControl1 do begin
      Name:='OpenGLControl1';
      Parent:=Self;
      SetBounds(0,0,1024, 768);
      OnPaint:=@OpenGLControl1Paint;
      OnResize:=@OpenGLControl1Resize;
    end;
    //MainHero.vPos := Vector3(0, 0, 0);
    OpenGLControl1.MakeCurrent;
    glMatrixMode (GL_PROJECTION);    { prepare for and then }
    glLoadIdentity ();               { define the projection }
    gluPerspective(45, single(1024)/single(768), 0.1, 3000.0);
    glMatrixMode (GL_MODELVIEW);  { back to modelview matrix }
    glViewport (0, 0, 1024, 768);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS);

    tTextures[0].loadBMP('data\textures\grass.bmp', 1);
    tTextures[1].loadBMP('data\textures\fire.bmp', 0);
    tTextures[2].loadBMPColorKey('data\interface\bottom_bar.bmp', 0, 0, 0);
    tTextures[3].loadBMPColorKey('data\interface\pb_edge.bmp', 0, 0, 0);
    tTextures[4].loadBMPColorKey('data\interface\pb_edge_mask.bmp', 0, 0, 0);
    tTextures[5].loadBMPColorKey('data\interface\pb_border.bmp', 0, 0, 0);
    tTextures[6].loadBMPColorKey('data\interface\pb_border_mask.bmp', 0, 0, 0);
    tTextures[7].loadBMP('data\interface\attack_speed.bmp', 0);
    tTextures[8].loadBMP('data\interface\fortitude.bmp', 0);
    tTextures[9].loadBMP('data\interface\intro.bmp', 0);
    tTextures[10].loadBMP('data\interface\outro.bmp', 0);
    mModels[0].loadModel('data\models\hero.mbsm');
    mModels[1].loadModel('data\models\treasure.mbsm');

	tSkybox[0].loadBMP('data/textures/skybox/darkland_front.bmp', 1);
	tSkybox[1].loadBMP('data/textures/skybox/darkland_back.bmp', 1);
	tSkybox[2].loadBMP('data/textures/skybox/darkland_left.bmp', 1);
	tSkybox[3].loadBMP('data/textures/skybox/darkland_right.bmp', 1);
	tSkybox[4].loadBMP('data/textures/skybox/darkland_top.bmp', 1);
	tSkybox[5].loadBMP('data/textures/skybox/darkland_bottom.bmp', 1);
   buildFont;
   loadWorldEntities;
   wCurWorld[0].readFromFile('data\worlds\level1.mbsw');
   wCurWorld[1].readFromFile('data\worlds\level2.mbsw');
   wCurWorld[2].readFromFile('data\worlds\level3.mbsw');
   MainHero.initHeroData;

    Timer1.Enabled:= true;
    prev := timeGetTime; cur := prev;
    randomize;
    fBlinkTime := -0.5;
end;


procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
kp[Key] := true;
end;

procedure TForm1.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
 kp[Key] := false;
end;

procedure TForm1.OpenGLControl1Resize(Sender: TObject);
begin

end;

procedure TForm1.StaticText2Click(Sender: TObject);
begin

end;

procedure TForm1.Timer1Timer(Sender: TObject);
begin
  OpenGLControl1Paint(Sender);
end;

//////// HERO /////////*
//
// Some hero functions.
//
//////// HERO /////////*

procedure renderSkybox;
begin;
	glColor3ub(255, 255, 255);
   glEnable(GL_TEXTURE_2D);
	glDepthMask(0);
	glPushMatrix();
	glTranslatef(MainHero.vPos.x, MainHero.vPos.y, MainHero.vPos.z);
   glScalef(10, 10, 10);

	tSkyBox[0].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 1.0); glVertex3f(-20.0, 20.0, -20.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(20.0, 20.0, -20.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(20.0, -20.0, -20.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-20.0, -20.0, -20.0);
	glEnd();

	tSkyBox[1].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0 , 1.0 ); glVertex3f(-20.0 , 20.0 , 20.0 );
	glTexCoord2f(1.0 , 1.0 ); glVertex3f(20.0 , 20.0 , 20.0 );
	glTexCoord2f(1.0 , 0.0 ); glVertex3f(20.0 , -20.0 , 20.0 );
	glTexCoord2f(0.0 , 0.0 ); glVertex3f(-20.0 , -20.0 , 20.0 );
	glEnd();

	tSkyBox[2].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0 , 1.0 ); glVertex3f(20.0 , 20.0 , -20.0 );
	glTexCoord2f(0.0 , 1.0 ); glVertex3f(20.0 , 20.0 , 20.0 );
	glTexCoord2f(0.0 , 0.0 ); glVertex3f(20.0 , -20.0 , 20.0 );
	glTexCoord2f(1.0 , 0.0 ); glVertex3f(20.0 , -20.0 , -20.0 );
	glEnd();

	tSkyBox[3].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0 , 1.0 ); glVertex3f(-20.0 , 20.0 , -20.0 );
	glTexCoord2f(1.0 , 1.0 ); glVertex3f(-20.0 , 20.0 , 20.0 );
	glTexCoord2f(1.0 , 0.0 ); glVertex3f(-20.0 , -20.0 , 20.0 );
	glTexCoord2f(0.0 , 0.0 ); glVertex3f(-20.0 , -20.0 , -20.0 );
	glEnd();

	tSkyBox[4].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0 , 1.0 ); glVertex3f(-20.0 , 20.0 , -20.0 );
	glTexCoord2f(1.0 , 0.0 ); glVertex3f(20.0 , 20.0 , -20.0 );
	glTexCoord2f(0.0 , 0.0 ); glVertex3f(20.0 , 20.0 , 20.0 );
	glTexCoord2f(0.0 , 1.0 ); glVertex3f(-20.0 , 20.0 , 20.0 );
	glEnd();

	tSkyBox[5].bindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0 , 0.0 ); glVertex3f(-20.0 , -20.0 , -20.0 );
	glTexCoord2f(1.0 , 1.0 ); glVertex3f(20.0 , -20.0 , -20.0 );
	glTexCoord2f(0.0 , 1.0 ); glVertex3f(20.0 , -20.0 , 20.0 );
	glTexCoord2f(0.0 , 0.0 ); glVertex3f(-20.0 , -20.0 , 20.0 );
	glEnd();

	glPopMatrix();
	glDepthMask(1);
end;

procedure renderProgressBar(blx, bly, w : integer; r, g, b : BYTE; cur, max : integer);
var fScale : single;
begin;
glColor3ub(255, 255, 255);
tTextures[3].bindTexture;
glBegin(GL_QUADS);
glTexCoord2d(0, 1); glVertex2d(blx, bly+32);
glTexCoord2d(1, 1); glVertex2d(blx+32, bly+32);
glTexCoord2d(1, 0); glVertex2d(blx+32, bly);
glTexCoord2d(0, 0); glVertex2d(blx, bly);

glTexCoord2d(1, 1); glVertex2d(blx+w-32, bly+32);
glTexCoord2d(0, 1); glVertex2d(blx+w, bly+32);
glTexCoord2d(0, 0); glVertex2d(blx+w, bly);
glTexCoord2d(1, 0); glVertex2d(blx+w-32, bly);
glEnd;

tTextures[5].bindTexture;
glBegin(GL_QUADS);
glTexCoord2d(0, 1); glVertex2d(blx+32, bly+32);
glTexCoord2d(1, 1); glVertex2d(blx+w-32, bly+32);
glTexCoord2d(1, 0); glVertex2d(blx+w-32, bly);
glTexCoord2d(0, 0); glVertex2d(blx+32, bly);
glEnd;

glDisable(GL_TEXTURE_2D);
fScale := single(cur)/single(max);
glBegin(GL_QUADS);
glCOlor3ub(r, g, b);
glVertex2d(blx+3, bly+3);
glVertex2d(blx+3, bly+23);
glVertex2f(single(blx)+3.0+single(w-6)*fScale, single(bly)+23);
glVertex2f(single(blx)+3.0+single(w-6)*fScale, single(bly)+3);

glEnd();

glEnable(GL_TEXTURE_2D);

end;


function getColor(src, dest : integer; iHPCur, iMaximumHP : integer):BYTE;
var fScale : single;
var tmp : integer;
var ok : boolean;
begin;
fScale := 1-single(iHPCur)/single(iMaximumHP);
tmp := src+Round(single(dest-src)*fScale);
getColor := BYTE(tmp);
end;

procedure TForm1.OpenGLControl1Paint(Sender: TObject);
var i:integer;
var s : String;
var ok : boolean;
begin
   cur := TimeGetTime; mul := single(cur-prev)*0.001; prev := cur;
   glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    glLoadIdentity;

    if(iState = 0)then begin;
    s := 'a';
    beginOrthoProjection;
    fBLinkTime += sof(1); if(fBlinkTime > 0.5)then fBlinkTime -= 1.0;
    glEnable(GL_TEXTURE_2D);

    tTextures[9].bindTexture;
    	glBegin(GL_QUADS);
      	glTexCoord2d(0, 1); glVertex2d(0, 768);
         glTexCoord2d(1, 1); glVertex2d(1024, 768);
         glTexCoord2d(1, 0); glVertex2d(1024, 0);
         glTexCoord2d(0, 0); glVertex2d(0, 0);
      glEnd();
          if(fBlinkTime < 0)then
    begin;
    	glDisable(GL_TEXTURE_2D);
    	glCOlor3ub(255, 255, 255);
   	 glBegin(GL_LINES);
    		glVertex2d(635+Length(sPlayerName)*25, 360);
   		 glVertex2d(635+Length(sPlayerName)*25, 320);
    	glEnd();
      glEnable(GL_TEXTURE_2D);
    end;
      glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      print(625, 320, 40, sPlayerName);
      glDisable(GL_BLEND);
    endOrthoProjection;
    	if(onekey(VK_RETURN))then iState := 2;
      if(onekey(VK_BACK)) and (length(sPlayerName) > 0)then Delete(sPlayerName, Length(sPlayerName), 1);
      if(Length(sPlayerName) < 10)then
      begin;
         if(onekey(ord(' ')))then begin s[1] := ' '; AppendStr(sPlayerName, s); end;
      	for i := ord('A') to ord('Z') do begin;
      		if(onekey(i))then
         	begin;
         		 if(kp[VK_SHIFT])then s[1] := chr(i) else s[1] := chr(ord('a')+i-ord('A'));
            	AppendStr(sPlayerName, s);
         	end;
      	end;
      end;
    end else
    if(iState = 1)then begin;
    if(not MainHero.bDead)then
    begin;
    	if(distance(MainHero.vPos, wCurWorld[iCurLevel].vEnd) < 50)then
      begin;
      ok := true;
      for i := 0 to wCurWorld[iCurLevel].iNumEnemies-1 do
      if(wCurWorld[iCurLevel].eEnemies[i].iID = 6) and (wCurWorld[iCurLevel].eEnemies[i].fFade > 0.0) then ok := false;
      if(ok)then begin; if(iCurLevel < 2)then iState := 2 else iState := 3; exit;end;

      end;
    	wCurWorld[iCurLevel].offsetSphere(MainHero.vPos, MainHero.yAcc, MainHero.bStanding, 10, true);
    	tcMain.look(vector3(MainHero.vPos.x, MainHero.vPos.y+20, MainHero.vPos.z));
    	renderSkybox();
    	MainHero.renderHero(sof(1.0));
    	wCurWorld[iCurLevel].renderWorld(sof(1.0));

    	glEnable(GL_TEXTURE_2D);
    	renderParticles(sof(1.0));
    	beginOrthoProjection;

    	glEnable(GL_ALPHA_TEST); tTextures[2].bindtexture();
    	glAlphaFunc(GL_GREATER, 0.0);
    	glColor3ub(255, 255, 255);
    	glBegin(GL_QUADS);
    		glTexCoord2d(0, 1); glVertex2d(0, 128);
    		glTexCoord2d(1, 1); glVertex2d(256, 128);
    		glTexCoord2d(1, 0); glVertex2d(256, 0);
    		glTexCoord2d(0, 0); glVertex2d(0, 0);

    		glTexCoord2f(0, 1); glVertex2d(256, 89);
    		glTexCoord2f(0.2, 1); glVertex2d(1024, 89);
    		glTexCoord2f(0.2, 0); glVertex2d(1024, -39);
    		glTexCoord2f(0, 0); glVertex2d(256, -39);
    	glEnd();

    	renderProgressBar(130, 35, 200, getColor(0, 255, MainHero.iRealHP, MainHero.iRealMaxHP), getColor(192, 0, MainHero.iRealHP, MainHero.iRealMaxHP), 0, MainHero.iRealHP, MainHero.iRealMaxHP);
    	if(MainHero.iLevel < 16)then renderProgressBar(130, 5, 200, 255, 255, 0, MainHero.iExp-iExpLevels[MainHero.iLevel-1], iExpLevels[MainHero.iLevel]-iExpLevels[MainHero.iLevel-1])
      else renderProgressBar(130, 5, 200, 232, 116, 0, 1, 1);
    	if(iLastHit <> -1)then
    	begin;
    		if(wCurWorld[iCurLevel].eEnemies[iLastHit].iHP > 0)then
      	begin;
          	renderProgressBar(730, 5, 200, getColor(0, 255, wCurWorld[iCurLevel].eEnemies[iLastHit].iHP, iMaxHPS[wCurWorld[iCurLevel].eEnemies[iLastHit].iID]), getColor(192, 0, wCurWorld[iCurLevel].eEnemies[iLastHit].iHP, iMaxHPS[wCurWorld[iCurLevel].eEnemies[iLastHit].iID]), 0, wCurWorld[iCurLevel].eEnemies[iLastHit].iHP, iMaxHPS[wCurWorld[iCurLevel].eEnemies[iLastHit].iID]);
      	end;
    	end;

    	glDisable(GL_ALPHA_TEST);
    	glColor3ub(255, 255, 255);
    	glPushMatrix();
    	glTranslated(350, 5, 0); tTextures[7].bindTexture;
    	glBegin(GL_QUADS);
    		glTexCoord2d(0, 1); glVertex2d(0, 64);
    		glTexCoord2d(1, 1); glVertex2d(64, 64);
    		glTexCoord2d(1, 0); glVertex2d(64, 0);
   		glTexCoord2d(0, 0); glVertex2d(0, 0);
    	glEnd();
    	glTranslated(70, 0, 0); tTextures[8].bindTexture;
    	glBegin(GL_QUADS);
    		glTexCoord2d(0, 1); glVertex2d(0, 64);
    		glTexCoord2d(1, 1); glVertex2d(64, 64);
    		glTexCoord2d(1, 0); glVertex2d(64, 0);
    		glTexCoord2d(0, 0); glVertex2d(0, 0);
   	glEnd();
    	glPopMatrix();

    	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    	if(fJustAdvanced > 0)then
    	begin;
    		print(30, 400, 20+25-round(fJustAdvanced*5), 'You have just advanced to level '+IntToStr(MainHero.iLevel)+'!');
      	fJustAdvanced -= sof(1.0);
    	end;
      print(375, 62, 16, '1'); print(445, 62, 16, '2');
    	print(500, 30, 16, 'Skill Points: '+IntToStr(MainHero.iUnAssigned));
    	print(355, 13, 16, IntToStr(MainHero.iSpeed)); print(426, 13, 16, IntToStr(MainHero.iFort));
    	print(5, 80, 32, sPlayerName);
    	print(10, 60, 16, 'Level '+IntToStr(MainHero.iLevel)+' '+sRank[MainHero.iLevel]);
    	print(10, 40, 16, 'HitPoints:'); print(180, 40, 16, IntToStr(MainHero.iRealHP)+'/'+INtToStr(MainHero.iRealMaxHP));
      print(10, 10, 16, 'Experience:');
    	if(MainHero.iLevel < 16)then print(180, 10, 16, IntToStr(MainHero.iExp)+'/'+INtToStr(iExpLevels[MainHero.iLevel]))
      else print(200, 10, 16, IntToStr(MainHero.iExp));
    	print(500, 10, 16, 'Damage: '+IntToStr(iDamage[MainHero.iLevel])+'-'+IntToStr(iDamage[MainHero.iLevel]+iDamage[MainHero.iLevel]div 12));
     	if(fMsgPosition < 50)then begin; print(510, 380+Round(fMsgPosition), 20, sGLobalString); fMsgPosition += sof(25); end;
    	if(iLastHit <> -1)then
    	begin;
    		if(wCurWorld[iCurLevel].eEnemies[iLastHit].iHP > 0)then
     		begin;
      		print(730, 60, 16, sEnemyNames[wCurWorld[iCurLevel].eEnemies[iLastHit].iID]);
          	print(730, 40, 16, 'Hitpoints: '+IntToStr(wCurWorld[iCurLevel].eEnemies[iLastHit].iHP)+'/'+IntToStr(iMaxHPS[wCurWorld[iCurLevel].eEnemies[iLastHit].iID]));
      	end;
    	end;
    	glDisable(GL_BLEND);

    	endOrthoProjection;

   end else
   begin;
   	beginOrthoProjection;
      glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      print(10, 600, 25, 'You didn''t manage to find the treasure. Better luck next time.');
      print(10, 560, 25, 'Press any key to exit...');
      for i := 1 to 256 do if kp[i] then halt;
      glDisable(GL_BLEND);
      endOrthoProjection;
   end;
   end else if(iState = 2)then
   begin;
   	beginOrthoProjection; glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      print(10, 700, 40, sLevelNames[iCurLevel+1]);
      print(615, 20, 25, 'Press enter to continue...');
      glDisable(GL_BLEND);
      endOrthoProjection;
   	if(onekey(VK_RETURN))then
   	begin;
   		iState := 1;
         iCurLevel += 1;
         iLastHit := -1;
         fMsgPosition := 100.0;
         MainHero.vPos := wCurWorld[iCurLevel].vStart;
   	end;
   end else if(iState = 3)then
   begin;
    beginOrthoProjection;
    glEnable(GL_TEXTURE_2D);
    tTextures[10].bindTexture;
    	glBegin(GL_QUADS);
      	glTexCoord2d(0, 1); glVertex2d(0, 768);
         glTexCoord2d(1, 1); glVertex2d(1024, 768);
         glTexCoord2d(1, 0); glVertex2d(1024, 0);
         glTexCoord2d(0, 0); glVertex2d(0, 0);
      glEnd();
      endOrthoProjection;
   	if(onekey(VK_ESCAPE))then Halt;
   end;
   OpenGLControl1.SwapBuffers;
end;

initialization
  {$I path_of_heroes.lrs}

end.

