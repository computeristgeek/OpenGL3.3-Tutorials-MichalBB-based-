#include "mgbsoft.h"

GLint iNumBuf;
ALuint* sndBuf;
vector<ALuint> sndSrc;
ALCdevice* g_pDevice;
ALCcontext* g_pContext;

char* sndNames[] = 
{
	"FlakCannon.WAV",
	"FIREBEAM.wav",
	"Uzi.wav",
	"Sounds\\Fire.wav",
	"Sounds\\RotateUp.wav",
	"Sounds\\RotateDown.wav",
	"Sounds\\Rotate.wav"
	"Sounds\\Serious_firepower.wav"
	"Sounds\\Cannon\\Bounce.wav"
};

//====================

GLvoid initOpenAL(GLint numBuf)
{
	g_pDevice = alcOpenDevice(NULL);
	if(g_pDevice == NULL)
	{
		MessageBox(NULL, "There was an error while opening a sound device!", "Error", MB_ICONERROR | MB_OK);
	}
    g_pContext = alcCreateContext(g_pDevice, NULL);
    alcMakeContextCurrent(g_pContext);
	sndBuf = new ALuint[numBuf];
	alGenBuffers(numBuf, sndBuf);
	iNumBuf = numBuf;
}

//====================

GLvoid loadSounds()
{
	FOR(i, iNumBuf)
	{
		ALenum format;
		ALsizei size;
		ALvoid* data;
		ALsizei freq;
		ALboolean loop;
		alutLoadWAVFile(sndNames[i], &format, &data, &size, &freq, &loop);
		alBufferData(sndBuf[i], format, data, size, freq);
		alutUnloadWAV(format, data, size, freq);
	}
}

//====================

GLvoid releaseALData()
{
	FOR(i, ESZ(sndSrc))alDeleteSources(1, &sndSrc[i]);

	alDeleteBuffers(iNumBuf, sndBuf);

    alcDestroyContext(g_pContext);
    alcCloseDevice(g_pDevice);
}

//====================

GLvoid addSource(ALuGLint bufID, CVector3 vPos, CVector3 vVel, GLboolean bLoop, GLboolean bRel, GLboolean bPlay, GLfloat gain)
{
	ALuGLint srcNew;
	alGenSources(1, &srcNew);

	alSourcei(srcNew, AL_BUFFER, bufID);
	alSourcef(srcNew, AL_PITCH, 1.0f);
	alSourcef(srcNew, AL_GAIN, gain);
	if(!bRel)
	{
		alSource3f(srcNew, AL_POSITION, vPos.x, vPos.y, vPos.z);
		alSource3f(srcNew, AL_VELOCITY, vVel.x, vVel.y, vVel.z);
	}
	else alSourcei(srcNew, AL_SOURCE_RELATIVE, AL_TRUE);
	if(bLoop)alSourcei(srcNew, AL_LOOPING, AL_TRUE);
	if(bPlay)alSourcePlay(srcNew);
	sndSrc.push_back(srcNew);
}

GLvoid setSourceProperties(ALuGLint uiSrc, ALuGLint uiBuf, CVector3 vPos, GLint iLoop, GLint iRel, GLfloat fGain)
{
	alSourcei(uiSrc, AL_BUFFER, uiBuf);
	alSourcef(uiSrc, AL_PITCH, 1.0f);
	alSourcef(uiSrc, AL_GAIN, fGain);
	alSource3f(uiSrc, AL_POSITION, vPos.x, vPos.y, vPos.z);
	alSourcei(uiSrc, AL_SOURCE_RELATIVE, iRel);
	alSourcei(uiSrc, AL_LOOPING, iLoop);
}