unit UWorld;

{$mode objfpc}{$H+}

interface

uses
  Classes, Dialogs, SysUtils,GL, UFileFunc, UMBSModel, Math3D, UTexture, UHero, UParticle;

var level2Special, level3Special :  boolean;
var fTreasureAngle : single;

type CPlatform = object
iTex : integer;
vPts : array[0..3] of CVector3;
vCoords :array[0..3] of CVector2;
vNorm : CVector3;
end;

type CEntity = object
vPos : CVector3;
iModel : integer;
fAngles : array[0..2] of single;
end;

type CGameObject = object
vPos : CVector3;
iID : integer;
end;

var iMaxHPS : array[0..6] of integer;
var iExpGains : array[0..6] of integer;
var iDamages : array[0..6] of integer;
var iLastHit : integer;
var sEnemyNames : array[0..6] of string;
var sLevelNames : array[0..2] of String;
var cR, cB, cG : array[0..6] of GL_UNSIGNED_BYTE;

type CEnemy = object
vPos : CVector3;
iHP, iID : integer;
fFade : single;
adData : CAnimData;
bStanding : boolean;
yAcc : single;
procedure renderEnemy(fTime : Single);

end;

type CWorld = object
pPlatforms : array of CPlatform;
iNumPlat : integer;
tTextures: array of CTexture;
eEntities:  array of CEntity;
iNumEnt : integer;

eEnemies : array of CEnemy; iNumEnemies : integer;
iNumObjects : integer;

tLocalTextures : array of CTexture;
iNumTextures : integer;

mLocalModels : array of CMBSMOdel;
iNumModels : integer;

vStart, vEnd : CVector3;

procedure readFromFile(sPath : string);
procedure renderWorld(fTime : single);

procedure offsetSphere(var vPos : CVector3; var yAcc : single; var bStanding : boolean; radius : single;bHeroUpdate : boolean);

end;
var wCurWorld : array [0..2] of CWorld;
procedure loadWorldEntities;
var fMsgPosition : single;
implementation

procedure loadWorldEntities;
begin;
level2Special := false; level3Special := false;
iCurLevel := -1; sLevelNames[0] := 'Seaside Mountain'; sLevelNames[1] := 'Lava Caves'; sLevelNames[2] := 'Frost Cave';
mEnemies[0].loadModel('data\models\treant.mbsm');
iMaxHPs[0] := 120; iDamages[0] := 30; iExpGains[0] := 100; sEnemyNames[0] := 'Treant'; cr[0] := 100; cG[0] := 50; cB[0] := 0;
mEnemies[1].loadModel('data\models\giant_crab.mbsm');
iMaxHPs[1] := 270; iDamages[1] := 40; iExpGains[1] := 250; sEnemyNames[1] := 'Giant Crab'; cr[1] := 40; cG[1] := 40; cB[1] := 40;
mEnemies[2].loadModel('data\models\lava_elemental.mbsm');
iMaxHPs[2] := 470; iDamages[2] := 55; iExpGains[2] := 350; sEnemyNames[2] := 'Lava Elemental'; cr[2] := 255; cG[2] := 128; cB[2] := 0;
mEnemies[3].loadModel('data\models\golem.mbsm');
iMaxHPs[3] := 750; iDamages[3] := 75; iExpGains[3] := 550; sEnemyNames[3] := 'Golem'; cr[3] := 128; cG[3] := 128; cB[3] := 128;
mEnemies[4].loadModel('data\models\ice_wasp.mbsm');
iMaxHPs[4] := 950; iDamages[4] := 65; iExpGains[4] := 650; sEnemyNames[4] := 'Ice Wasp'; cr[4] := 0; cG[4] := 128; cB[4] := 255;
mEnemies[5].loadModel('data\models\ice_warrior.mbsm');
iMaxHPs[5] := 1400; iDamages[5] := 90; iExpGains[5] := 840; sEnemyNames[5] := 'Ice Warrior'; cr[5] := 0; cG[5] := 128; cB[5] := 255;
mEnemies[6].loadModel('data\models\ice_warrior.mbsm');
iMaxHPs[6] := 5000; iDamages[6] := 150; iExpGains[6] := 4000; sEnemyNames[6] := 'Ice Warrior Leader'; cr[6] := 0; cG[6] := 128; cB[6] := 255;
end;

procedure CWorld.readFromFile(sPath : string);
var stream : TMemoryStream;
var i, j, k:integer;
var ints : array[1..7] of integer;
var vBox : array[0..7] of CVector3;
var oObject : CGameObject;
var line,s : string;
var pPlat : CPlatform;
begin;
stream := LoadFileToMemStream(sPath);

for i:=1 to 7 do stream.read(ints[i], sizeof(integer));

iNumPlat := ints[1]+ints[5]*4;
SetLength(pPlatforms, iNumPlat);
for i := 0 to ints[1]-1 do
begin;
	stream.read(pPlatforms[i].iTex, sizeof(integer));
	for j := 0 to 3 do stream.read(pPlatforms[i].vPts[j], sizeof(CVector3));
	for j := 0 to 3 do stream.read(pPlatforms[i].vCoords[j], sizeof(CVector2));
   pPlatforms[i].vNorm := Cross(vectorMinus(pPlatforms[i].vPts[0], pPlatforms[i].vPts[1]), vectorMinus(pPlatforms[i].vPts[2], pPlatforms[i].vPts[1]));
   Normalize(pPlatforms[i].vNorm);
end;

iNumEnt := ints[2];
SetLength(eEntities, iNumEnt);
for i :=0 to iNumEnt-1 do
begin;
	stream.read(eEntities[i].vPos, sizeof(CVector3));
   stream.read(eEntities[i].iModel, sizeof(integer));
   for j:=0 to 2 do stream.read(eEntities[i].fAngles[j], sizeof(single));
end;

iNumEnemies := ints[3]; SetLength(eEnemies, iNumEnemies);
for i := 0 to iNumEnemies-1 do begin;
stream.read(eEnemies[i].vPos, sizeof(CVector3));
stream.read(eEnemies[i].iID, sizeof(integer));
eEnemies[i].iHP:= iMaxHPs[eEnemies[i].iID];
eEnemies[i].adData.nullize; eEnemies[i].fFade := 1.0;
mEnemies[eEnemies[i].iID].startAnimation(eEnemies[i].adData, 'idle');
end;

iNumObjects := ints[4];
for i := 0 to iNumObjects-1 do
begin;
	stream.read(oObject.vPos, sizeof(CVector3));
	stream.read(oObject.iID, sizeof(integer));
end;

for i:=0 to ints[5]-1 do
begin;
	for j:=0 to 7 do stream.read(vBox[j], sizeof(CVector3));
   pPlat.iTex:=-1;
   pPlat.vPts[0] := vBox[4]; pPlat.vPts[1] := vBox[5]; pPlat.vPts[2] := vBox[1]; pPlat.vPts[3] := vBox[0];
   pPlat.vNorm := Cross(vectorMinus(pPlat.vPts[0], pPlat.vPts[1]), vectorMinus(pPlat.vPts[2], pPlat.vPts[1]));
   Normalize(pPlat.vNorm);
   pPlatforms[ints[1]+i*4] := pPlat;

   pPlat.vPts[0] := vBox[6]; pPlat.vPts[1] := vBox[7]; pPlat.vPts[2] := vBox[3]; pPlat.vPts[3] := vBox[2];
   pPlat.vNorm := Cross(vectorMinus(pPlat.vPts[0], pPlat.vPts[1]), vectorMinus(pPlat.vPts[2], pPlat.vPts[1]));
   Normalize(pPlat.vNorm);
   pPlatforms[ints[1]+i*4+1] := pPlat;

   pPlat.vPts[0] := vBox[7]; pPlat.vPts[1] := vBox[4]; pPlat.vPts[2] := vBox[0]; pPlat.vPts[3] := vBox[3];
   pPlat.vNorm := Cross(vectorMinus(pPlat.vPts[0], pPlat.vPts[1]), vectorMinus(pPlat.vPts[2], pPlat.vPts[1]));
   Normalize(pPlat.vNorm);
   pPlatforms[ints[1]+i*4+2] := pPlat;

   pPlat.vPts[0] := vBox[5]; pPlat.vPts[1] := vBox[6]; pPlat.vPts[2] := vBox[2]; pPlat.vPts[3] := vBox[1];
   pPlat.vNorm := Cross(vectorMinus(pPlat.vPts[0], pPlat.vPts[1]), vectorMinus(pPlat.vPts[2], pPlat.vPts[1]));
   Normalize(pPlat.vNorm);
   pPlatforms[ints[1]+i*4+3] := pPlat;
end;

iNumTextures := ints[6];
SetLength(tLocalTextures, iNumTextures);
for i:=0 to iNumTextures-1 do
begin;
	line := 'data\worlds\textures\';s := 'a';
	while true do begin; stream.read(s[1], 1); if(ord(s[1])<15)then break; appendSTR(line, s); end;
	tLocalTextures[i].loadBMP(line, 2);
	stream.read(tLocalTextures[i].bLiquid, sizeof(GL_UNSIGNED_BYTE));
   stream.read(tLocalTextures[i].fAddU, sizeof(single));
   stream.read(tLocalTextures[i].fAddV, sizeof(single));
	tLocalTextures[i].fCurAn := 0.0;
   tLocalTextures[i].fCurU := 0.0;
   tLocalTextures[i].fCurV := 0.0;
end;

iNumModels := ints[7];
SetLength(mLocalModels, iNumModels);
for i:=0 to iNumModels-1 do
begin;
	line := 'data\worlds\models\'; s := 'a';
	while true do begin; stream.read(s[1], 1); if(ord(s[1])<15)then break; appendSTR(line, s); end;
	mLocalModels[i].loadModel(line);
end;

stream.read(vStart, sizeof(CVector3)); stream.read(vEnd, sizeof(CVector3));

end;

procedure sphereSphereCollision(var vCent1, vCent2 : CVector3; rad1, rad2 : single);
var vDir : CVector3;
var fDist : Single;
begin;
fDist := distance(vCent1, vCent2);

if(fDist < rad1+rad2)then begin;
vDir := vectorMinus(vCent2,vCent1); normalize(vDir);
vCent1 := vectorMinus(vCent1, vectorScale(vDir, rad1+rad2-fDist+0.01));
end;

end;

procedure CWorld.renderWorld(fTime: single);
var i, j, k:integer;
var emptyData : CAnimData;
var sine : single;
var vMid, vDir, vPt : CVector3;
begin;
emptyData.nullize;
if(iCurLevel = 2)then
begin;
glPushMatrix();
glTranslatef(vEnd.x, vEnd.y, vEnd.z);
glRotatef(fTreasureAngle, 0, 1, 0); fTreasureAngle += fTIme*180;
if(fTreasureAngle > 360)then
begin;
	fTreasureAngle -= 360;
   if(level3Special)then for i := 1 to 150 do AddParticle(Vector3(vEnd.x, vEnd.y+10, vEnd.z),
               				Vector3(frand(-10, 10), frand(-10, 10), frand(-10, 10)),
                                           1.5, 255, 255, 0);
end;
mModels[1].renderModel(emptyData);
glPopMatrix();
end;
for i := 0 to iNumPlat-1 do
begin;
	if(iCurLevel = 1) and (level2Special) and (i = 10) then
   begin;
   	vMid := Vector3(0, 0, 0); for j := 0 to 3 do vMid := vectorPlus(vMid, pPlatforms[i].vPts[j]);
   	vMid := vectorScale(vMid, 0.25);
   	for j := 0 to 3 do
      begin;
      	vPt := vectorPlus(wCurWorld[iCurLevel].vEnd, vectorMinus(pPlatforms[i].vPts[j], vMid));
         vDir := vectorMinus(vPt, pPlatforms[i].vPts[j]); normalize(vDir);
         pPlatforms[i].vPts[j] := vectorPlus(pPlatforms[i].vPts[j], vectorScale(vDir, fTime*80));
         MainHero.vPos := vectorPlus(MainHero.vPos, vectorScale(vDir, fTime*20));
      end;
   end;
	if(iCurLevel = 2) and (level3Special) and (i = 35) and(pPlatforms[i].vPts[0].y < 360) then
   begin;
   	for j := 0 to 3 do
      begin;
         pPlatforms[i].vPts[j].y += fTime*60;
         MainHero.vPos.y += fTime*15;
      end;
   end;
	if(pPlatforms[i].iTex = -1)then continue;
   tLocalTextures[pPlatforms[i].iTex].bindTexture;
	glBegin(GL_QUADS);
	for j := 0 to 3 do
	begin;
		glTexCoord2f(pPlatforms[i].vCoords[j].x+tLocalTextures[pPlatforms[i].iTex].fCurU, pPlatforms[i].vCoords[j].y+tLocalTextures[pPlatforms[i].iTex].fCurV);
  		 glVertex3f(pPlatforms[i].vPts[j].x, pPlatforms[i].vPts[j].y, pPlatforms[i].vPts[j].z);
	end;
	glEnd();
end;


for i:=0 to iNumTextures-1 do begin;
	if(tLocalTextures[i].bLiquid = 0)then continue;
   sine := sin(tLocalTextures[i].fCurAn*PI/180.0);
   tLocalTextures[i].fCurU += tLocalTextures[i].fAddU*sine*fTime;
   tLocalTextures[i].fCurV += tLocalTextures[i].fAddV*sine*fTime;
	tLocalTextures[i].fCurAn += 115.0*fTime;
end;

for i:=0 to iNumEnt-1 do begin;
glPushmatrix();
glTranslatef(eEntities[i].vPos.x, eEntities[i].vPos.y, eEntities[i].vPos.z);
glRotatef(eEntities[i].fAngles[0], 1, 0, 0);
glRotatef(eEntities[i].fAngles[1], 0, 1, 0);
glRotatef(eEntities[i].fAngles[2], 0, 0, 1);
mLocalModels[eEntities[i].iModel].renderModel(emptyData);
glPopMatrix();
if(iCurLevel = 2) and (level3Special) and (i = 21) and(pPlatforms[35].vPts[0].y < 360) then
eEntities[i].vPos.y += fTime*60;
end;

for i := 0 to iNumEnemies-1 do
begin;
	if(eEnemies[i].fFade <= 0)then continue;
	eEnemies[i].renderEnemy(fTime);
	for j := i+1 to iNumEnemies-1 do
   begin;
		if(eEnemies[j].fFade <= 0)then continue;
		sphereSphereCollision(eEnemies[i].vPos, eEnemies[j].vPos, 10, 10);
	end;
	sphereSphereCollision(eEnemies[i].vPos, MainHero.vPos, 10, 10);
   for j := 1 to 2 do
   begin;
   	if(mModels[0].curKeyFrame(MainHero.adData, 'attack'+IntToStr(j)) = 2) and (mModels[0].animTimes(MainHero.adData, 'attack'+IntToStr(j)) = 0)then
   	begin;
   		if (distance(eEnemies[i].vPos, MainHero.vPos) < 28) and(eEnemies[i].iHP > 0) and(AngleBetweenVectors(Vector3(eEnemies[i].vPos.x-MainHero.vPos.x, 0, eEnemies[i].vPos.z-MainHero.vPos.z), Vector3(-tcMain.vDir.x, 0, -tcMain.vDir.z)) < PI/6)then
      	begin;
      		eEnemies[i].iHP -= iDamage[MainHero.iLevel]+random(iDamage[MainHero.iLevel] div 12 + 1);
         	mModels[0].alterTimes(MainHero.adData, 'attack'+IntToStr(j), 10);
            iLastHit := i;
            for k := 0 to iDamage[MainHero.iLevel] div 2 do AddParticle(Vector3(eEnemies[i].vPos.x, eEnemies[i].vPos.y+20, eEnemies[i].vPos.z),
               										Vector3(frand(-10, 10), frand(-10, 10), frand(-10, 10)),
                                               1.5, cR[eEnemies[i].iID], cG[eEnemies[i].iID], cB[eEnemies[i].iID]);
     			if(eEnemies[i].iHP <= 0)then
         	begin;
         		mEnemies[eEnemies[i].iID].startAnimation(eEnemies[i].adData, 'die');
               MainHero.iEXP += iExpGains[eEnemies[i].iID];
               while(MainHero.iEXP >= iExpLevels[MainHero.iLevel]) do
               begin;
               	if(MainHero.iLevel = 16)then break;
               	MainHero.iLevel += 1;
           			 for k := 0 to 399 do AddParticle(Vector3(MainHero.vPos.x, MainHero.vPos.y+20, MainHero.vPos.z),
               				Vector3(frand(-10, 10), frand(-10, 10), frand(-10, 10)),
                                           1.5, 255, 255, 0);
                   fJustAdvanced := 5.0;
                   MainHero.iMaxHP := Round(single(MainHero.iMaxHP)*105/100);
                   MainHero.recalcHP;
                   MainHero.iUnAssigned += 1;
               end;
               fMsgPosition := 0.0;
               sGlobalString := '+'+IntToStr(iExpGains[eEnemies[i].iID])+' EXP';
         		break;
         	end;
     		end;
   	end;
   end;
end;


end;


procedure CWorld.offsetSphere(var vPos : CVector3; var yAcc : single; var bStanding : boolean; radius : single; bHeroUpdate : boolean);
var i : integer;
var vOffset, vInt : CVector3;
var fDistance : single;
var iClass : integer;
begin;
bStanding := False;
for i := 0 to iNumPlat-1 do
begin;
	iClass := ClassifySphere(vPos, pPlatforms[i].vNorm, pPlatforms[i].vPts[0], radius, fDistance);
	if(iClass = 0)then
   begin;
   	vOffset := vectorScale(pPlatforms[i].vNorm, fDistance);
		vInt := vectorMinus(vPos, vOffset);
		if(InsidePolygon(vInt, pPlatforms[i].vPts, 4) ) or
			   (EdgeSphereCollision(vPos, pPlatforms[i].vPts, 4, radius/2.0) )then
			begin;

				vOffset := GetCollisionOffset(pPlatforms[i].vNorm, radius, fDistance);

				vPos := vectorPlus(vPos, vOffset);
            if(AngleBetweenVectors(pPlatforms[i].vNorm, Vector3(0, 1, 0)) <= PI/4)then
            begin;
            	if(iCurLevel = 1)then if(pPlatforms[i].iTex <> -1)then if(tLocalTextures[pPlatforms[i].iTex].bLiquid=1)then if(bHeroUpdate)then MainHero.iRealHP := 0;
         		bStanding := true;
               yAcc := 0.0;
               if(iCurLevel = 1)then begin;
               	if(i = 29)then yAcc := 271;
               	if(i = 10)then level2Special := true;
               end;
               if(iCurLevel = 2)then
               begin;
               	if(i = 35)then level3Special := true;
               end;
            end;

         end;
	end;

end;


end;



procedure CEnemy.renderEnemy(fTime : single);
var vDir : CVector3;
var fDist : single;
var i : integer;
begin;
if(vPos.y < -200)then begin; iHP := 0; yacc := 0; end;
vDir := vectorMinus(MainHero.vPos, vPos);
glPushMatrix();
glTranslatef(vPos.x, vPos.y-10, vPos.z);
if(iID = 6)then glScalef(1.2, 1.2, 1.2);
fDist := AngleBetweenVectors(vDir, Vector3(0, 0, 1)); fDist *= 180/PI;
if(MainHero.vPos.x > vPos.x)then fDist := 360-fDist;
glRotatef(-fDist, 0, 1, 0);
if(iHP <= 0)then begin; glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glColor4f(1, 1, 1, fFade); end;
mEnemies[iID].renderModel(adData);
if(iHP <= 0)then glDisable(GL_BLEND);
adData.updateData(fTime);
yacc -= 70*fTime;
wCurWorld[iCurLevel].offsetSphere(vPos, yacc, bStanding, 10, false);
vPos.y += yacc*fTime;
glPopMatrix();
if(iHP > 0)then
begin;
	fDist := distance(vPos, MainHero.vPos);
   if(fDist > 100) or (MainHero.iRealHP = 0)then
   begin;
   	mEnemies[iID].stopAnimation(adData, 'walk');
      if(not mEnemies[iID].isAnimPresent(adData, 'idle')) then mEnemies[iID].startAnimation(adData, 'idle');
   end else
	if(fDist < 100)and (not mEnemies[iID].isAnimPresent(adData, 'attack')) then
	begin;
   	mEnemies[iID].stopAnimation(adData, 'idle');
  		if(not mEnemies[iID].isAnimPresent(adData, 'walk'))then mEnemies[iID].startAnimation(adData, 'walk');
		normalize(vDir);
		vPos := vectorPlus(vPos, vectorScale(vDir, fTime*40.0));
	end;
	if(mEnemies[iID].animTimes(adData, 'attack') mod 10 = 1)then mEnemies[iID].stopAnimation(adData, 'attack');
	if(fDist < 22) and (MainHero.iRealHP > 0)then
   begin;
		mEnemies[iID].stopAnimation(adData, 'walk');
		if(not mEnemies[iID].isAnimPresent(adData, 'attack'))then mEnemies[iID].startAnimation(adData, 'attack');
   	if(mEnemies[iID].curKeyFrame(adData, 'attack') = 1) and (mEnemies[iID].animTimes(adData, 'attack') = 0)then
   	begin;
          MainHero.iRealHP -= iDamages[iID]; if(MainHero.iRealHP < 0)then MainHero.iRealHP := 0;
          mEnemies[iID].alterTimes(adData, 'attack', 10);
          for i := 0 to iDamages[iID]div 2 do AddParticle(Vector3(MainHero.vPos.x, MainHero.vPos.y+20, MainHero.vPos.z),
        																 Vector3(frand(-10, 10), frand(-10, 10), frand(-10, 10)),
                                          							 1.5, 255, 0, 0);
   	end;
   end;
end else
begin;
	mEnemies[iID].stopAnimation(adData, 'attack');
   mEnemies[iID].stopAnimation(adData, 'walk');
   if(mEnemies[iID].curKeyFrame(adData, 'die') = 2)then
   begin;
   	mEnemies[iID].pauseAnimation(adData, 'die');
      fFade -= fTime*0.5;
   end;
end;

end;

end.

