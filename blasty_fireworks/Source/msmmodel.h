class CMSMModel
{
public:

	struct MSHeader
	{
		WORD nGroups, nTextures, nMaterials, nAnimations;
	};

	struct CGroup
	{
		char cMat;
		CVector3* vVerts;
		CVector3* vNorms;
		CVector2* vCoords;
		WORD nFaces;
	};

	struct CMaterial
	{
		int iTexture;
		float fAmbient[4], fDiffuse[4], fSpecular[4], fEmmisive[4];
		BYTE bShininess;
	};

	MSHeader mHeader;
	CGroup* gGroups;
	CMaterial* mMats;

	void loadModel(char* strFile);
	void renderModel();
};