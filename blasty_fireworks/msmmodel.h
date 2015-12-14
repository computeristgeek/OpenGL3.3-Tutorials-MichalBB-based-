class CMSMModel
{
public:

	struct MSHeader
	{
		WORD nGroups, nTextures, nMaterials, nAnimations;
	};

	struct CGroup
	{
		GLchar cMat;
		CVector3* vVerts;
		CVector3* vNorms;
		CVector2* vCoords;
		WORD nFaces;
	};

	struct CMaterial
	{
		GLint iTexture;
		GLfloat fAmbient[4], fDiffuse[4], fSpecular[4], fEmmisive[4];
		GL_UNSIGNED_BYTE bShininess;
	};

	MSHeader mHeader;
	CGroup* gGroups;
	CMaterial* mMats;

	GLvoid loadModel(char* strFile);
	GLvoid renderModel();
};