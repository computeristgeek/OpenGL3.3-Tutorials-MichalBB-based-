#include "mainheader.h"

char* strSndNames[] = 
{
	"ripper_shoot.wav",
	"ripper_bounce.wav",
	"Uzi_Fire12.wav",
	"mg_fire.wav",
	"mg_rotate.wav", // 5
	"mg_rup.wav",
	"mg_rdown.wav",
	"cannon_fire.wav",
	"cannon_bounce.wav",
	"cannon_prepare.wav", // 10
	"flakcannon.wav",
	"shockrifle.wav",
	"explosion01.wav",
	"explosion02.wav",
	"cannon_fire02.wav", //15
	"spider_die.wav",
	"spider_die02.wav",
	"PlasmaRifle_Fire03.wav",
};

FSOUND_SAMPLE** sndData;

GLvoid loadSounds(GLint iNumSnd)
{
	sndData = new FSOUND_SAMPLE*[iNumSnd];
	FOR(i, iNumSnd)
	{
		sndData[i] = FSOUND_Sample_Load(FSOUND_FREE, strSndNames[i], FSOUND_NORMAL, 0, 0);
		if(sndData[i])FSOUND_Sample_SetMode(sndData[i], FSOUND_LOOP_OFF);
	}
}

GLvoid InitFMOD()
{
    if (FSOUND_GetVersion() < FMOD_VERSION)
    {
		GLchar strError[255];
		sprintf(strError, "You are using the wrong FMOD DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
		MessageBox(glAp.hWindow, strError, "Error", MB_ICONERROR | MB_OK);
    }
	if(!FSOUND_Init(44100, 32, 0))
	{
		MessageBox(glAp.hWindow, "Sound couldn't be initialized!", "Error", MB_ICONERROR | MB_OK);
	}
	GLchar strCur[512]; GetCurrentDirectory(512, strCur);
	GLchar strOld[512]; strcpy(strOld, strCur);
	strcat(strCur, "\\Sounds");
	SetCurrentDirectory(strCur);
	loadSounds(18);
	SetCurrentDirectory(strOld);
}