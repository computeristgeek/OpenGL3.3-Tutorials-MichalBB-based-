void initOpenAL(int numBuf);
void loadSounds();
void releaseALData();
void addSource(ALuint bufID, CVector3 vPos, CVector3 vVel, bool bLoop, bool bRel, bool bPlay, float gain);
void setSourceProperties(ALuint uiSrc, ALuint uiBuf, CVector3 vPos, int iLoop, int iRel, float fGain);

extern ALuint* sndBuf;
extern vector<ALuint> sndSrc;