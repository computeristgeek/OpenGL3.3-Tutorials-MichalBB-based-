GLvoid initOpenAL(GLint numBuf);
GLvoid loadSounds();
GLvoid releaseALData();
GLvoid addSource(ALuGLint bufID, CVector3 vPos, CVector3 vVel, GLboolean bLoop, GLboolean bRel, GLboolean bPlay, GLfloat gain);
GLvoid setSourceProperties(ALuGLint uiSrc, ALuGLint uiBuf, CVector3 vPos, GLint iLoop, GLint iRel, GLfloat fGain);

extern ALuint* sndBuf;
extern vector<ALuint> sndSrc;