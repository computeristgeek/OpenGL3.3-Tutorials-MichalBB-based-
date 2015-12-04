unit UHero;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Math3D, UMBSModel, GL,LCLType, UFont;

var iState : integer;  // Current game state (0 = intro, 1 = game, 2 = between levels, 3 = end)
var sGlobalString : string;

type CHero = object
vPos : CVector3;
yAcc : single;
iMaxHP, iExp, iLevel, iRealHP, iRealMaxHP, iFort, iSpeed, iUnAssigned : integer;
fAngle : single;
adData : CAnimData;
bStanding, bDead : boolean;
fIdleTime : single;
fRegenVal : single;
fDeadTime : single;
procedure renderHero(fTime : single);
procedure initHeroData;
procedure recalcHP;
end;

var kp, onekp: array[1..500] of boolean;
var MainHero: CHero;
var iExpLevels : array[1..16] of integer;
var iDamage : array[1..16] of integer;
var sRank : array[1..16] of string;
var fJustAdvanced : single;
function onekey(Key : Word) : boolean;
var iCurLevel : integer;
implementation


procedure CHero.initHeroData;
var i : integer;
begin;
    adData.nullize; IeXP := 0;
    iMaxHP := 250; iRealHP := 250; iRealMaxHP:=250;
    iLevel := 1;
    iExpLevels[0] := 0; for i := 1 to 16 do iExpLevels[i] := iExpLevels[i-1]+(250*i);
    sRank[1] := 'Apprentice'; sRank[2] := 'Apprentice'; sRank[3] := 'Apprentice';
    sRank[4] := 'Young Warrior'; sRank[5] := 'Young Warrior'; sRank[6] := 'Young Warrior';
    sRank[7] := 'Knight'; sRank[8] := 'Knight'; sRank[9] := 'Knight';
    sRank[10] := 'Elite Knight'; sRank[11] := 'Elite Knight'; sRank[12] := 'Elite Knight';
    sRank[13] := 'SwordsMaster'; sRank[14] := 'SwordsMaster'; sRank[15] := 'SwordsMaster';
    sRank[16] := 'Swords GrandMaster';

    iDamage[1] := 30; iDamage[2] := 37; iDamage[3] := 44; iDamage[4] := 52; iDamage[5] := 61;
    iDamage[6] := 73; iDamage[7] := 83; iDamage[8] := 92; iDamage[9] := 102; iDamage[10] := 114;
    iDamage[11] := 128; iDamage[12] := 142; iDamage[13] := 157; iDamage[14] := 172; iDamage[15] := 195;
    iDamage[16] := 225;

    tcMain.vDir := vector3(0, 15, 60);
    iUnAssigned:=1;
    bDead := false;
end;


procedure CHero.renderHero(fTime : single);
var i, iIs : integer;
var anPres : boolean;
var sHelp : string;
var s : single;
begin;
	glPushMatrix();
	glTranslatef(vPos.x, vPos.y-5, vPos.z);
	glRotatef(fAngle-180.0, 0, -1, 0);
	mModels[0].renderModel(adData);
   mModels[0].updateDataSpecial(adData, fTime, fTime*(single(iSpeed*3)/100.0), 'attack1');
   mModels[0].updateDataSpecial(adData, 0.0, fTime*(single(iSpeed*3)/100.0), 'attack2');
	glPopMatrix();
	if(iRealHP > 0)then
   begin;
		fRegenVal += fTime*single(iRealMaxHP)/100.0;
	   iRealHP += trunc(fRegenVal); fRegenVal -= single(trunc(fRegenVal));
	   if(iRealHP > iRealMaxHP)then iRealHP := iRealMaxHP;
		iIs := 0;
	   fIdleTime += 1.0*fTime;
		if(kp[VK_LEFT])then begin; fIdleTime := 0.0; fAngle -= 150.0*fTime; tcMain.rotate(-150.0*fTime); end;
		if(kp[VK_RIGHT])then begin; fIdleTime := 0.0;  tcMain.rotate(150.0*fTime); fAngle += 150.0*fTime; end;
		if(kp[VK_UP]) or (kp[VK_DOWN])then
		begin;
	   	s := 1; if(kp[VK_DOWN])then s := -1;
			vPos.x += sin(fAngle*PI/180.0)*50.0*s*fTime;
			vPos.z -= cos(fAngle*PI/180.0)*50.0*s*fTime;
	      fIdleTime := 0.0;
	      mModels[0].stopAnimation(adData, 'idle');
			if(not mModels[0].isAnimPresent(adData, 'attack1')) and
	      (not mModels[0].isAnimPresent(adData, 'attack2')) and
	      (not mModels[0].isAnimPresent(adData, 'walk_upper')) and
	      (not mModels[0].isAnimPresent(adData, 'walk_legs')) then
			begin;
				mModels[0].startAnimation(adData, 'walk_legs');
				mModels[0].startAnimation(adData, 'walk_upper');
			end;
			iIs := 1;
		end;
	   if(mModels[0].animTimes(adData, 'attack1') mod 10 = 1)then mModels[0].stopAnimation(adData, 'attack1')
	   else if(mModels[0].animTimes(adData, 'attack2') mod 10 = 1)then mModels[0].stopAnimation(adData, 'attack2');
	   if(kp[VK_CONTROL])then
      begin;
		   fIdleTime := 0.0;
		   mModels[0].stopAnimation(adData, 'idle');
	   	if(not mModels[0].isAnimPresent(adData, 'attack1')) and (not mModels[0].isAnimPresent(adData, 'attack2'))then
	  		begin;
	      	sHelp := 'attack'; sHelp += inttostr(random(2)+1);
	  			mModels[0].startAnimation(adData, sHelp);
	   		mModels[0].stopAnimation(adData, 'walk_legs');
   		mModels[0].stopAnimation(adData, 'walk_upper');
   		end;

		end;
		if(iIs = 0)then
		begin;
   		mModels[0].stopAnimation(adData, 'walk_legs');
      	mModels[0].stopAnimation(adData, 'walk_upper');
		end;

   	if((kp[ord(' ')]) and bStanding)then yacc := 30.0;
   	if(bStanding)then mModels[0].stopAnimation(adData, 'jump_legs');
   	anPres := mModels[0].isAnimPresent(adData, 'jump_legs');
   	if(not anPres) and (fabsf(yacc) > 15)then begin;
   	mModels[0].startAnimation(adData, 'jump_legs');
   	mModels[0].stopAnimation(adData, 'walk_legs');
   	mModels[0].stopAnimation(adData, 'walk_upper');
   	end;
   	if(fIdleTime > 3.5) and (not mModels[0].isAnimPresent(adData, 'idle'))then mModels[0].startAnimation(adData, 'idle');
   	yacc -= 70*fTime;
   	MainHero.vPos.y += yacc*fTime;
   	if(iUnAssigned > 0)then
   	begin;
   		if(onekey(ord('1'))) or (onekey(VK_NUMPAD1)) then begin;iUnAssigned-=1; iSpeed += 1; end;
      	if(onekey(ord('2'))) or (onekey(VK_NUMPAD2)) then begin;iUnAssigned-=1; iFort += 1; recalcHP;end;
		end;
	end else
   begin;
   	mModels[0].stopAnimation(adData, 'walk_legs');
   	mModels[0].stopAnimation(adData, 'walk_upper');
   	mModels[0].stopAnimation(adData, 'idle');
   	mModels[0].stopAnimation(adData, 'attack1');
   	mModels[0].stopAnimation(adData, 'attack2');
   	mModels[0].stopAnimation(adData, 'jump_legs');
      if (not mModels[0].isAnimPresent(adData, 'die')) then mModels[0].startAnimation(adData, 'die');
      if(mModels[0].curKeyFrame(adData, 'die') = 4)then
      begin;
      	mModels[0].pauseAnimation(adData, 'die');
         fDeadTime += fTime;
         if(fDeadTime > 2.0)then bDead := true;
      end;
   end;
   if(vPos.y < -200)then iRealHP := 0;
end;

procedure CHero.recalcHP;
var fRatio : single;
begin;
fRatio := single(iRealHP)/single(iRealMaxHP);
iRealMaxHP := Round(single(iMaxHP)*(1.0+(single(iFort)*7)/100.0));
iRealHP := Round(fRatio*single(iRealMaxHP));
end;

function onekey(Key : Word) : boolean;
begin;
if(kp[Key]) and (not onekp[Key])then
begin;
	onekp[Key] := true;
   onekey := true;
end else if(kp[key]) and (onekp[Key]) then onekey := false
else begin; onekp[Key] := false; onekey := false; end;
end;

end.

