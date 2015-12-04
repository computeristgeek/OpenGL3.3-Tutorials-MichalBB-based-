unit UMBSModel;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Math3D, UTexture, UFileFunc, GL, GLU;


type CAnimData = object
	nAnims:integer;
	iAnim : array[0..9] of integer;
	iKF : array[0..9] of integer;
	fTime : array[0..9] of single;
   bPaused : array[0..9] of byte;
   iTimes : array [0..9] of integer;
	procedure nullize; // :-)
   procedure updateData(fVal : single);
end;

type CMBSMHeader = object
	nGroups, nTextures, nMats, nAnims : Word;
end;

type CGroup = object
	sName:string;
	cMat, cFaceNorm: char;
   vVerts, vFNorms, vVNorms :  array of CVector3;
   vCoords : array of CVector2;
	iNFaces : Word;
end;

type CMaterial = object
	iTex:integer;
	fAmbient, fDiffuse, fSpecular, fEmmisive : array[1..4] of real;
	bShininess : BYTE;
end;

type CAnimation = object
	bNumKF : byte;
   sName : string;
	fDelay : array of single;
	bGBF : array of array of BYTE; // Animated groups bitfield (double pointer)
   bFBF : array of array of array of BYTE; // Animated faces bitfield (triple pointer)
   iOffSet : array of array of integer;

	viGrAnim : array of boolean; // Indices of groups affected by this animation
   iVGASize, iGroupOffset : Integer; // Sizeof of the former array
	gGroups : array of array of CGroup; // [KF][G]
end;


type CMBSModel = object
	bLoad : boolean;
	mbsmHeader : CMBSMHeader;
	gGroups : array of CGroup;
	tTexturesL : array of CTexture;
	mMats : array of CMaterial;
	aAnims : array of CAnimation;
   aaa : array of array of integer;


	procedure loadModel(sFile : string);
   procedure renderModel(var adData : CAnimData);
	// Animation functions
   procedure startAnimation(var adData : CAnimData; sAnName : string);
   procedure stopAnimation(var adData : CAnimData; sAnName : string);
   procedure pauseAnimation(var adData : CAnimData; sAnName : string);
   procedure continueAnimation(var adData : CAnimData; sAnName : string);
   procedure alterTimes(var adData : CAnimData; sAnName : string; by : integer);
   function curKeyFrame(var adData : CAnimData; sAnName : string) : integer;
   function isAnimPresent(var adData : CAnimData; sAnName : string) : boolean;
   function animTimes(var adData : CAnimData; sAnName : string) : integer;
   procedure updateDataSpecial(var adData : CanimData; fVal1, fVal2 : single; sName : string);
   constructor init;
end;


var mModels : array[0..50] of CMBSModel;
var mEnemies : array[0..6] of CMBSModel;
var GlobalString : string;

implementation

constructor CMBSModel.init;
begin;
bLoad := false;
end;

procedure CMBSModel.loadModel(sFile : string);
var MemStream: TMemoryStream;
var c :char;
var s, line:string;
var tm:LongWord;
var i,j,k,l,m, iNGBF, iNFBF, iKF:integer;

begin;
	s := 'a';
	memStream:=LoadFileToMemStream(sFile);
   for i:=1 to 10 do memStream.Read(c, 1);
   memStream.Read(mbsmHeader, sizeof(CMBSMHeader));
   SetLength(gGroups, mbsmheader.nGroups);

	// Read groups
   for i:=0 to mbsmHeader.nGroups-1 do
   begin;
  		while true do begin; memStream.read(c, 1); if(ord(c)<15)then break; end; // Skip name of group, we won't need it

      memStream.read(gGroups[i].cMat, sizeof(char));
		memStream.read(gGroups[i].cFaceNorm, sizeof(char));
		memStream.read(gGroups[i].iNFaces, sizeof(Word));
		SetLength(gGroups[i].vVerts, gGroups[i].iNFaces*3);
      SetLength(gGroups[i].vCoords, gGroups[i].iNFaces*3);
		for j:=0 to gGroups[i].iNFaces-1 do
      begin;
      	memStream.Read(gGroups[i].vVerts[j*3], sizeof(CVector3)*3);
         memStream.Read(gGroups[i].vCoords[j*3], sizeof(CVector2)*3);
      end;
	end;

	// Read textures
   SetLength(tTexturesL, mbsmHeader.nTextures);
	for i:=0 to mbsmheader.nTextures-1 do
   begin;
   	line := 'data\models\textures\';
		while true do begin; memstream.read(s[1], 1); if(ord(s[1])<15)then break; appendSTR(line, s); end;
		tTexturesL[i].loadBMP(line, 1);
   end;

// Read materials
	SetLength(mMats, mbsmHeader.nMats);
	for i:=0 to mbsmHeader.nMats-1 do
   begin;
		while true do begin; memStream.read(c, 1); if(ord(c)<15)then break; end;
		memStream.read(c, 1);
		mMats[i].iTex := integer(c);
      for j:=1 to 17 do memStream.read(c, 1);
	end;

	// Read animations
	SetLength(aAnims, mbsmHeader.nAnims);
	iNGBF := (mbsmHeader.nGroups+7) div 8; // Number of group bitfields

	for i:=0 to mbsmheader.nAnims-1 do
   begin;
		aAnims[i].sName := '';
		while true do begin; memstream.read(s[1], 1); if(ord(s[1])<15)then break; appendSTR(aAnims[i].sName, s); end;
     // ShowMessage(aAnims[i].sname);
      memStream.read(aAnims[i].bNumKF, sizeof(BYTE));
      SetLength(aAnims[i].fDelay, aAnims[i].bNumKF);
		SetLength(aAnims[i].bGBF, aAnims[i].bNumKF);
      SetLength(aAnims[i].bFBF, aAnims[i].bNumKF);
     	SetLength(aAnims[i].gGroups, aAnims[i].bNumKF);
      SetLength(aAnims[i].iOffSet, aAnims[i].bNumKF);
      Setlength(aAnims[i].viGrAnim, mbsmHeader.nGroups); for j := 0 to mbsmHeader.nGroups-1 do aAnims[i].viGrAnim[j] := false;
      for j:=0 to aAnims[i].bNumKF-1 do
      begin;
			memStream.read(tm, sizeof(LongWord)); aAnims[i].fDelay[j] := single(tm)/1000.0;
        // ShowMessage(n2s('time',tm));
         SetLength(aAnims[i].gGroups[j], mbsmHeader.nGroups);
         SetLength(aAnims[i].bFBF[j], mbsmHeader.nGroups);
         SetLength(aAnims[i].iOffSet[j], mbsmHeader.nGroups);
         SetLength(aAnims[i].bGBF[j], iNGBF);
			SetLength(aAnims[i].bFBF[j], mbsmHeader.nGroups);
			for k:=0 to iNGBF-1 do MemStream.read(aAnims[i].bGBF[j][k], sizeof(BYTE));
			for k:=0 to mbsmHeader.nGroups-1 do
			begin;
				aAnims[i].gGroups[j][k].cFaceNorm := gGroups[k].cFaceNorm; // It doesn't matter
				aAnims[i].gGroups[j][k].iNFaces := gGroups[k].iNFaces;
				memStream.read(aAnims[i].gGroups[j][k].cMat, sizeof(char));
				iNFBF := (gGroups[k].iNFaces+7) div 8; // Number of face bitfield
				SetLength(aAnims[i].bFBF[j][k], sizeof(BYTE)*iNFBF);
				// Is k-th group animated?
				if((aAnims[i].bGBF[j][k div 8] and (1 shl (k mod 8))) = 0)then
				begin;
            	if j = 0 then aAnims[i].iOffSet[j][k] := -1 else aAnims[i].iOffSet[j][k] := aAnims[i].iOffSet[j-1][k];
            	//aAnims[i].gGroups[j][k].vVerts = j == 0 ? gGroups[k].vVerts : aAnims[i].gGroups[j-1][k].vVerts;
					for l:=0 to iNFBF-1 do aAnims[i].bFBF[j][k][l] := 0;
					continue;
				end;
      		aAnims[i].iOffSet[j][k] := j;
            aAnims[i].viGrAnim[k] := true;
            //ShowMessage(n2s('offset', aAnims[i].iOffSet[j][k]));
            for l:=0 to iNFBF-1 do begin; memStream.read(aAnims[i].bFBF[j][k][l], 1); end;

				setLength(aAnims[i].gGroups[j][k].vVerts, gGroups[k].iNFaces*3);
				for l:=0 to gGroups[k].iNFaces-1 do
            begin;
					if((aAnims[i].bFBF[j][k][l div 8] and (1 shl (l mod 8))) = 0) then
               begin;
               	for m:=0 to 2 do
                  begin;
                  	if(j = 0)then aAnims[i].gGroups[j][k].vVerts[3*l+m] := gGroups[k].vVerts[3*l+m]
                     else begin;
                     iKF := aAnims[i].iOffSet[j-1][k];
                   // ShowMessage(n2s(iKF));
                     if(iKF <> -1)then aAnims[i].gGroups[j][k].vVerts[3*l+m] := aAnims[i].gGroups[iKF][k].vVerts[3*l+m]
                     else aAnims[i].gGroups[j][k].vVerts[3*l+m] := gGroups[k].vVerts[3*l+m];
                     end;
                  end;
               end
					else for m:=0 to 2 do memStream.read(aAnims[i].gGroups[j][k].vVerts[3*l+m], sizeof(CVector3));

				end;
			end;
		end;
   end;
   bLoad := true;
end;



procedure CMBSModel.renderModel(var adData: CAnimData);
var i, j, f, k, v, iMat, n, a, iNKF, iOff:integer;
var iKF: ^Integer;
var fScale : single;
var vSrc, vDest, vTar : Cvector3;
var viDone : array of boolean;
begin;
	if(not bLoad)then exit;
 //  GlobalString := n2s('nic', adData.nAnims);
	if(adData.nAnims = 0)then
   begin;

		for i :=0 to mbsmHeader.nGroups-1 do
      begin;
			iMat := integer(gGroups[i].cMat);
			if(iMat <> -1)then
         begin;
				glEnable(GL_TEXTURE_2D);
				if(mMats[iMat].iTex <> -1)then tTexturesL[mMats[iMat].iTex].bindTexture();
			end
			else glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
			for j := 0 to gGroups[i].iNFaces-1 do for k:=0 to 2 do
         begin;
				glTexCoord2f(gGroups[i].vCoords[j*3+k].x, gGroups[i].vCoords[j*3+k].y);
            glVertex3f(gGroups[i].vVerts[j*3+k].x, gGroups[i].vVerts[j*3+k].y, gGroups[i].vVerts[j*3+k].z);
			end;
			glEnd();
		end;
	end else
	begin
   	SetLength(viDone, mbsmheader.nGroups); for i := 0 to mbsmHeader.nGroups-1 do viDone[i] := false;
		for i:=0 to adData.nAnims-1 do
      begin;
 			a := adData.iAnim[i];
			iKF := @adData.iKF[i];

			while true do
			begin
				if(adData.fTime[i] >= aAnims[a].fDelay[iKF^]) then
            begin;
					adData.fTime[i] -= aAnims[a].fDelay[iKF^];
					iKF^ := (iKF^+1) mod integer (aAnims[a].bNumKF);
               if(iKF^ = 0)then adData.iTimes[i]+=1;
				end else break;
			end;
			fScale := adData.fTime[i]/aAnims[a].fDelay[iKF^];
			iNKF := (iKF^+1) mod integer(aAnims[a].bNumKF);
			for j := 0 to mbsmHeader.nGroups-1 do
			begin;
         	if(not aAnims[a].viGrAnim[j])then continue;
            if(viDone[j])then continue;
         	if((iNKF <> 0) and ((aAnims[a].bGBF[iNKF][j div 8] and (1 shl (j mod 8) ) ) = 0))then continue;
            viDone[j] := true;
				iMat := integer(aAnims[a].gGroups[iKF^][j].cMat);
				if(iMat <> -1)then
				begin;
					glEnable(GL_TEXTURE_2D);
					if(mMats[iMat].iTex <> -1)then tTexturesL[mMats[iMat].iTex].bindTexture();
				end
				else glDisable(GL_TEXTURE_2D);
				glBegin(GL_TRIANGLES);
				for f := 0 to gGroups[j].iNFaces-1 do
				begin;
					for v := 0 to 2 do
               begin;
						glTexCoord2f(gGroups[j].vCoords[f*3+v].x, gGroups[j].vCoords[f*3+v].y);
						iOff := aAnims[a].iOffSet[iKF^][j];
						if(iOff = -1)then vSrc := gGroups[j].vVerts[f*3+v]
                  else vSrc := aAnims[a].gGroups[iOff][j].vVerts[f*3+v];
                  iOff := aAnims[a].iOffSet[(iKF^+1) mod integer(aAnims[a].bNumKF)][j];
                  if(iOff = -1)then vDest := gGroups[j].vVerts[f*3+v]
						else vDest := aAnims[a].gGroups[iOff][j].vVerts[f*3+v];
						if((iNKF = 0) or ( (aAnims[a].bFBF[iNKF][j][f div 8] and (1 shl (f mod 8)) ) <> 0) )then
                  begin;
							vTar := vectorPlus(vSrc, vectorScale(vectorMinus(vDest, vSrc), fScale));
                     glVertex3f(vTar.x, vTar.y, vTar.z);
                  end
                  else glVertex3f(vSrc.x, vSrc.y, vSrc.z);
					end;
				end;
				glEnd();
			end;
		end;
		for i := 0 to mbsmHeader.nGroups-1 do
      begin;
			if(viDone[i])then continue;
			iMat := integer(gGroups[i].cMat);
			if(iMat <> -1)then
			begin;
				glEnable(GL_TEXTURE_2D);
				if(mMats[iMat].iTex <> -1)then tTexturesL[mMats[iMat].iTex].bindTexture();
			end
			else glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
			for j := 0 to gGroups[i].iNFaces-1 do
			for k := 0 to 2 do
         begin;
				glTexCoord2f(gGroups[i].vCoords[j*3+k].x, gGroups[i].vCoords[j*3+k].y);
				glVertex3f(gGroups[i].vVerts[j*3+k].x, gGroups[i].vVerts[j*3+k].y, gGroups[i].vVerts[j*3+k].z);
			end;
			glEnd();
		end;
	end;
end;


procedure CMBSModel.startAnimation(var adData : CAnimData; sAnName : string);
var a, i, ind:integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if(a = -1)then exit;
   ind := adData.nAnims;
	for i:= 0 to adData.nAnims-1 do if(adData.iAnim[i] = a) or (adData.iAnim[i] = -1) then begin; ind := i; break; end;
	adData.iAnim[ind] := a; adData.fTime[ind] := 0.0;
	adData.iKF[ind] := 0;
   adData.bPaused[ind] := 0;
   adData.iTimes[ind] := 0;
   if(ind = adData.nAnims)then adData.nAnims+=1;
end;

procedure CMBSModel.stopAnimation(var adData : CAnimData; sAnName : string);
var a, i, ind, l:integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if(a = -1)then exit;
   ind := -1;
	for i:= 0 to adData.nAnims-1 do if(adData.iAnim[i] = a) then begin; ind := i; break; end;
   if(ind = -1)then exit;
   l := adData.nAnims-1;
   adData.iAnim[ind] := adData.iAnim[l];
   adData.iKF[ind] := adData.iKF[l];
   adData.fTime[ind] := adData.fTime[l];
   adData.iTimes[ind] := adData.iTimes[l];
   adData.nAnims -= 1;
end;


procedure CAnimData.nullize;
var i : integer;
begin;
nAnims := 0;
for i := 0 to 9 do begin;
iAnim[i] := -1;
fTime[i] := 0.0;
bPaused[i] := 0;
iKF[i] := 0;
end;

end;

procedure CAnimData.updateData(fVal : single);
var i : integer;
begin;
for i := 0 to nAnims-1 do if (bPaused[i] = 0) then fTime[i] += fVal;
end;

procedure CMBSModel.updateDataSpecial(var adData : CanimData; fVal1, fVal2 : single; sName : string);
var i : integer;
begin;
for i := 0 to adData.nAnims-1 do
begin;
	if(adData.bPaused[i] = 1) then continue;
   adData.fTime[i] += fVal1;
   if(aAnims[adData.iAnim[i]].sName = sName)then adData.fTime[i] += fVal2;
end;
end;

procedure CMBSModel.pauseAnimation(var adData : CAnimData; sAnName : string);
var a, i, ind : integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if(a = -1)then exit;
   ind := -1; for i := 0 to adData.nAnims-1 do if(adData.iAnim[i] = a)then begin; ind := i; break; end;
   if(ind = -1)then exit;
   adData.bPaused[ind] := 1;
end;

procedure CMBSModel.continueAnimation(var adData : CAnimData; sAnName : string);
var a, i, ind : integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if(a = -1)then exit;
   ind := -1; for i := 0 to adData.nAnims-1 do if(adData.iAnim[i] = a)then begin; ind := i; break; end;
   if(ind = -1)then exit;
   adData.bPaused[ind] := 0;
end;

function CMBSModel.curKeyFrame(var adData : CAnimData; sAnName : string) : integer;
var a, i, ind : integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if(a = -1)then begin; curKeyFrame := -1; exit; end;
   ind := -1; for i := 0 to adData.nAnims-1 do if(adData.iAnim[i] = a)then begin; ind := i; break; end;
   if(ind = -1)then begin; curKeyFrame := -1; exit; end;
   curKeyFrame := adData.iKF[ind];
end;

function CMBSModel.isAnimPresent(var adData : CAnimData; sAnName : string) : boolean;
var a, i, ind : integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if a = -1 then begin; isAnimPresent := false; exit; end;
   ind := -1; for i := 0 to adData.nAnims-1 do if(adData.iAnim[i] = a)then begin; ind := i; break; end;
   if(ind = -1)then begin; isAnimPresent := false; exit; end;
   isAnimPresent := true;
end;

function CMBSModel.animTimes(var adData : CAnimData; sAnName : string) : integer;
var a, i, ind : integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if(a = -1)then begin; animTimes := -1; exit; end;
   ind := -1; for i := 0 to adData.nAnims-1 do if(adData.iAnim[i] = a)then begin; ind := i; break; end;
   if(ind = -1)then begin; animTimes := -1; exit; end;
   animTimes := adData.iTimes[ind];
end;

procedure CMBSModel.alterTimes(var adData : CAnimData; sAnName : string; by : integer);
var a, i, ind : integer;
begin;
	a := -1;
	for i := 0 to mbsmHeader.nAnims-1 do if(aAnims[i].sName = sAnName)then begin; a := i; break; end;
	if(a = -1)then begin; exit; end;
   ind := -1; for i := 0 to adData.nAnims-1 do if(adData.iAnim[i] = a)then begin; ind := i; break; end;
   if(ind = -1)then begin; exit; end;
   adData.iTimes[ind] += by;
end;

end.

