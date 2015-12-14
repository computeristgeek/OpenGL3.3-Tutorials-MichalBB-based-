#include "common_header.h"

#include "win_OpenGLApp.h"

#include "shaders.h"
#include "texture.h"
#include "vertexBufferObject.h"

#include "static_geometry.h"

#include "flyingCamera.h"

#include "freeTypeFont.h"

#include "skybox.h"
#include "dirLight.h"

#include "physicsNode.h"

#include "spotLight.h"

CSpotLight slFlashLight;
CVertexBufferObject vboSceneObjects;
GLuint uiVAOSceneObjects;

CFreeTypeFont ftFont;

CSkybox sbMainSkybox;
CFlyingCamera cCamera;

CDirectionalLight dlSun;

/*-----------------------------------------------

Name:    InitScene

Params:  lpParam - Pointer to anything you want.

Result:  Initializes OpenGL features that will
         be used.

/*---------------------------------------------*/



btBroadphaseInterface*                  _broadphase;
btDefaultCollisionConfiguration*        _collisionConfiguration;
btCollisionDispatcher*                  _dispatcher;
btSequentialImpulseConstraintSolver*    _solver;
btDiscreteDynamicsWorld*                _world;

vector<CBoxPhysicsNode> nodes;
vector<CCapsulePhysicsNode> capnodes;
vector<CCapsulePhysicsNodeX> capnodesX;
vector<CCapsulePhysicsNodeZ> capnodesZ;

vector<CCapsulePhysicsNode> grenades;

vector<float> grenadeTimes;


vector<CCapsulePhysicsNode> rockets;


#include <blaze_part_sys_engine.h>
BlazeParticleSystemEngine* psEngine;
#pragma comment(lib, "blaze_part_sys_engine.lib")

GLint iSubSims = 1;

string sItemNames[] = 
{
	"Just A Box",
	"Mainstream Capsule",
	"Grenade",
	"Baguette Launcher"
};

string sMaterials[] = 
{
	"Wood",
	"Metal",
	"Ice",
	"Titanium"
};

GLint iCurrentItem = 0;
GLint iCurrentMaterial = 0;
GLfloat fBoxSize = 5.0f;
GLfloat fRadius = 3.0f;
GLfloat fHeight = 5.0f;
GLint iNumNodes = 16;

GLfloat fMassMultiplier[] = {0.1f, 0.15f, 0.1f, 0.5f};
GLfloat fRestitution[] = {0.1f, 0.05f, 0.1f, 0.04f};
GLfloat fFriction[] = {0.95f, 0.96f, 0.7f, 0.98f};

GLfloat fForcePowers[] = {200, 40, 300, 2};
GLint iRenderOrder[] = {0, 1, 3, 2};

static GLfloat fSimulationSpeed = 3.0f;
GLfloat fImpulseSpeed = 0.1f;
GLfloat fVelLimit = 0.1f;

vector<btRigidBody*> ptrs;

GLvoid InitScene(GLvoid* lpParam)
{
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;
	oglControl->SetVerticalSynchronization(false);
	 _broadphase = new btDbvtBroadphase();
	 _collisionConfiguration = new btDefaultCollisionConfiguration();
	 _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	 _solver = new btSequentialImpulseConstraintSolver();
	 _world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
	 _world->setGravity(btVector3(0, -9.8f, 0));


	 CBoxPhysicsNode newBox;
	 newBox.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	 newBox.SetBoxSize(glm::vec3(500, 1, 500));
	 newBox.CreateNode(0.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1, 1, _world);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if(!PrepareShaderPrograms())
	{
		PostQuitMessage(0);
		return;
	}

	LoadAllStaticObjects(vboSceneObjects, uiVAOSceneObjects);
	
	LoadAllTextures();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);


	slFlashLight = CSpotLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1, 15.0f, 0.017f);
	// Here we load font with pixel size 32 - this means that if we prGLint with size above 32, the quality will be low
	ftFont.LoadSystemFont("arial.ttf", 32);
	ftFont.SetShaderProgram(&spFont2D);
	
	cCamera = CFlyingCamera(glm::vec3(0.0f, 30.0f, 100.0f), glm::vec3(0.0f, 30.0f, 99.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f, 0.1f);
	cCamera.SetMovingKeys('W', 'S', 'A', 'D');

	sbMainSkybox.LoadSkybox("data\\skyboxes\\jajsundown1\\", "jajsundown1_ft.jpg", "jajsundown1_bk.jpg", "jajsundown1_lf.jpg", "jajsundown1_rt.jpg", "jajsundown1_up.jpg", "jajsundown1_dn.jpg");

	dlSun = CDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(sqrt(2.0f)/2, -sqrt(2.0f)/2, 0), 0.2f);

	psEngine = createTransformFeedbackParticleSystem();
	psEngine->initalizeParticleSystem();

	psEngine->setParticleTexture(tTextures[5].GetTextureID());

	GLfloat fBoxSize = 5.0f;
	/*
	FOR(uMat, 4)
	{
		RSFOR(i, 4, 1)
		{
			glm::vec3 vStartPos = glm::vec3(-float(i)*(fBoxSize*2.0f+0.01f)/2.0f, 5.3f+float(6-i)*(fBoxSize*2.0f+0.1f), -50.0f+float(uMat)*40.0f);
			FOR(j, i)
			{
				CBoxPhysicsNode box;
				box.SetPosition(glm::vec3(vStartPos.x+float(j)*(fBoxSize*2.0f+0.01f), vStartPos.y, vStartPos.z));
				box.SetBoxSize(glm::vec3(fBoxSize, fBoxSize, fBoxSize));
				box.CreateNode(300.5f*fMassMultiplier[uMat], glm::vec3(0.0f), fRestitution[uMat], fFriction[uMat], _world);
				box.iMaterialUsed = uMat;

				nodes.push_back(box);
			}
		}
	}*/

	GLint iNumNodes = 20;

	GLfloat fRopeNodeHeight = 1.0f;
	FOR(i, iNumNodes)
	{
		CCapsulePhysicsNode rope;

		rope.SetPosition(glm::vec3(0, 100-i*fRopeNodeHeight, 0));
		rope.SetCapsuleSize(0.1f, fRopeNodeHeight);
		rope.CreateNode(i == 0 ? 0.0f : 5.0f, glm::vec3(0, 0, 0), 0.05f, 0.98f, _world);
		rope.iMaterialUsed = 1;
		rope._body->setAngularFactor(btVector3(0.1f, 0.1f, 0.1f));
		capnodes.push_back(rope);
	}

	FOR(i, iNumNodes-1)
	{
		btPoint2PointConstraint* vstr = new btPoint2PointConstraint(*capnodes[i]._body, *capnodes[i+1]._body, btVector3(0, -fRopeNodeHeight*0.66f, 0), btVector3(0, fRopeNodeHeight*0.66f, 0));
		_world->addConstraint(vstr, true);
	}

	CBoxPhysicsNode box;
	box.SetPosition(glm::vec3(glm::vec3(0, 100-iNumNodes*fRopeNodeHeight, 0)));
	box.SetBoxSize(glm::vec3(fRopeNodeHeight, fRopeNodeHeight, fRopeNodeHeight));
	box.CreateNode(50.5f, glm::vec3(0.0f), 0.05f, 0.98f, _world);
	box._body->setAngularFactor(btVector3(0.3f, 0.3f, 0.3f));
	box.iMaterialUsed = 1;

	nodes.push_back(box);

	btPoint2PointConstraint* vstr =  new btPoint2PointConstraint(*capnodes[ESZ(capnodes)-1]._body, *box._body, btVector3(0, -fRopeNodeHeight*0.66f, 0), btVector3(0, fRopeNodeHeight, 0));
	_world->addConstraint(vstr, true);


	GLfloat fChainNodeHeight = 2.0f;
	GLfloat fChainNodeWidth = 1.0f;
	GLfloat fChainThickness = 0.2f;


	FILE*fp = fopen("settings.ini", "rt");
	fscanf(fp, "%f", &fSimulationSpeed);
	fscanf(fp, "%d", &iSubSims);
	fscanf(fp, "%f", &fImpulseSpeed);
	fscanf(fp, "%f",&fVelLimit);
	fscanf(fp, "%d", &iNumNodes);
	fclose(fp);

	

	FOR(i, iNumNodes)
	{
		GLint j = i/2;

		//if(i%2 == 0)continue;
		GLfloat motac = 0.15f;
		GLfloat chainWeight = 0.2f;
		GLfloat fBaseX = 50.0f;
		GLfloat fBaseY = 100.0f-float(i)*fChainNodeHeight*1.9f;
		GLfloat M_PI = 3.1415f;
		GLfloat fLow = -M_PI/64, fHigh = M_PI/64, fSoftness = 0.0f, fBiasFactor = 0.0f, fRelaxationFactor = 0.1f;


		if(i%2 == 0)
		{
			CBoxPhysicsNode chain;

			chain.SetPosition(glm::vec3(fBaseX-fChainThickness-fChainNodeWidth, fBaseY, 0));
			chain.SetBoxSize(glm::vec3(fChainThickness, fChainNodeHeight, fChainThickness));
			chain.CreateNode(chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chain.iMaterialUsed = 1;
			nodes.push_back(chain);

			CBoxPhysicsNode chain2;

			chain2.SetPosition(glm::vec3(fBaseX+fChainNodeWidth+fChainThickness, fBaseY, 0));
			chain2.SetBoxSize(glm::vec3(fChainThickness, fChainNodeHeight, fChainThickness));
			chain2.CreateNode(chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chain2.iMaterialUsed = 1;
			nodes.push_back(chain2);

			CBoxPhysicsNode chainX;

			chainX.SetPosition(glm::vec3(fBaseX, fBaseY+fChainNodeHeight+fChainThickness, 0));
			chainX.SetBoxSize(glm::vec3(fChainNodeWidth, fChainThickness, fChainThickness));
			chainX.CreateNode(i == 0 ? 0.0f : chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chainX.iMaterialUsed = 1;
			nodes.push_back(chainX);

			CBoxPhysicsNode chainX2;

			chainX2.SetPosition(glm::vec3(fBaseX, fBaseY-fChainNodeHeight-fChainThickness, 0));
			chainX2.SetBoxSize(glm::vec3(fChainNodeWidth, fChainThickness, fChainThickness));
			chainX2.CreateNode(chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chainX2.iMaterialUsed = 1;
			nodes.push_back(chainX2);

			ptrs.push_back(chainX._body);
			ptrs.push_back(chainX2._body);


			btHingeConstraint* vstr = new btHingeConstraint(*chain._body, *chainX._body, btVector3(fChainThickness-motac, fChainNodeHeight-motac, 0), btVector3(-fChainNodeWidth, -fChainThickness, 0.0f), btVector3(0, 0, 1), btVector3(0, 0, 1));

			vstr->setBreakingImpulseThreshold(9999999999.0f);
			vstr->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr, true);

			btHingeConstraint* vstr2 = new btHingeConstraint(*chain2._body, *chainX._body, btVector3(-fChainThickness+motac, fChainNodeHeight-motac, 0), btVector3(fChainNodeWidth, -fChainThickness, 0.0f), btVector3(0, 0, 1), btVector3(0, 0, 1));
			vstr2->setBreakingImpulseThreshold(9999999999.0f);
			vstr2->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr2, true);

			btHingeConstraint* vstr3 = new btHingeConstraint(*chain._body, *chainX2._body, btVector3(fChainThickness-motac, -fChainNodeHeight+motac, 0), btVector3(-fChainNodeWidth, fChainThickness, 0.0f), btVector3(0, 0, 1), btVector3(0, 0, 1));
			vstr3->setBreakingImpulseThreshold(9999999999.0f);
			vstr3->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr3, true);

			btHingeConstraint* vstr4 = new btHingeConstraint(*chain2._body, *chainX2._body, btVector3(-fChainThickness+motac, -fChainNodeHeight+motac, 0), btVector3(fChainNodeWidth, fChainThickness, 0.0f), btVector3(0, 0, 1), btVector3(0, 0, 1));
			vstr4->setBreakingImpulseThreshold(9999999999.0f);
			vstr4->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr4, true);



		}
		else
		{
			CBoxPhysicsNode chain;

			chain.SetPosition(glm::vec3(fBaseX, fBaseY, -fChainNodeHeight/2-fChainThickness));
			chain.SetBoxSize(glm::vec3(fChainThickness, fChainNodeHeight, fChainThickness));
			chain.CreateNode(chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chain.iMaterialUsed = 1;
			nodes.push_back(chain);

			CBoxPhysicsNode chain2;

			chain2.SetPosition(glm::vec3(fBaseX, fBaseY, fChainNodeHeight/2+fChainThickness));
			chain2.SetBoxSize(glm::vec3(fChainThickness, fChainNodeHeight, fChainThickness));
			chain2.CreateNode(chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chain2.iMaterialUsed = 1;
			nodes.push_back(chain2);

			CBoxPhysicsNode chainZ;

			chainZ.SetPosition(glm::vec3(fBaseX, fBaseY+fChainNodeHeight+fChainThickness, 0));
			chainZ.SetBoxSize(glm::vec3(fChainThickness, fChainThickness, fChainNodeWidth));
			chainZ.CreateNode(chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chainZ.iMaterialUsed = 1;
			nodes.push_back(chainZ);

			CBoxPhysicsNode chainZ2;

			chainZ2.SetPosition(glm::vec3(fBaseX, fBaseY-fChainNodeHeight-fChainThickness, 0));
			chainZ2.SetBoxSize(glm::vec3(fChainThickness, fChainThickness, fChainNodeWidth));
			chainZ2.CreateNode(chainWeight, glm::vec3(0, 0, 0), 0.0f, 0.98f, _world);
			chainZ2.iMaterialUsed = 1;
			nodes.push_back(chainZ2);
			ptrs.push_back(chainZ._body);
			ptrs.push_back(chainZ2._body);

			btHingeConstraint* vstr = new btHingeConstraint(*chain._body, *chainZ._body, btVector3(0.0f, fChainNodeHeight-motac, fChainThickness-motac), btVector3(0.0f, -fChainThickness, -fChainNodeWidth), btVector3(1, 0, 0), btVector3(1, 0, 0));
			vstr->setBreakingImpulseThreshold(9999999999.0f);
			vstr->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr, true);

			btHingeConstraint* vstr2 = new btHingeConstraint(*chain2._body, *chainZ._body, btVector3(0.0f, fChainNodeHeight-motac, -fChainThickness+motac), btVector3(0.0f, -fChainThickness, fChainNodeWidth-motac), btVector3(1, 0, 0), btVector3(1, 0, 0));
			vstr2->setBreakingImpulseThreshold(9999999999.0f);
			vstr2->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr2, true);

			btHingeConstraint* vstr3 = new btHingeConstraint(*chain._body, *chainZ2._body, btVector3(0.0f, -fChainNodeHeight+motac, fChainThickness-motac), btVector3(0.0f, fChainThickness, -fChainNodeWidth), btVector3(1, 0, 0), btVector3(1, 0, 0));
			vstr3->setBreakingImpulseThreshold(9999999999.0f);
			vstr3->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr3, true);

			btHingeConstraint* vstr4 = new btHingeConstraint(*chain2._body, *chainZ2._body, btVector3(0.0f, -fChainNodeHeight+motac, -fChainThickness+motac), btVector3(0.0f, fChainThickness, fChainNodeWidth), btVector3(1, 0, 0), btVector3(1, 0, 0));
			vstr4->setBreakingImpulseThreshold(9999999999.0f);
			vstr4->setLimit(fLow, fHigh, fSoftness, fBiasFactor, fRelaxationFactor);
			_world->addConstraint(vstr4, true);	
		}
	}

	SFOR(i, 1, ESZ(ptrs)/2-1)
	{
		btRigidBody* body1 = ptrs[i*2-1];
		btRigidBody* body2 = ptrs[i*2];

		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInB = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(0.0, fChainThickness+0.05, 0.0));
		frameInB.setOrigin(btVector3(0.0, -fChainThickness-0.05, 0.0));

		btSliderConstraint* vstr = new btSliderConstraint(*body1, *body2, frameInA, frameInB, false);

		vstr->setLowerLinLimit(-fChainNodeWidth/2+fChainThickness+0.01f);
		vstr->setUpperLinLimit(fChainNodeWidth/2-fChainThickness-0.01f);

		_world->addConstraint(vstr);
	}

	glm::vec3 vPosition = glm::vec3(-41.56f, 2.74f, 23.47f);

	GLfloat size = 15.0f;
	GLfloat fCornerVertexA[] = {vPosition.x-size/2, vPosition.y, vPosition.z-size/2};
	GLfloat fCornerVertexB[] = {vPosition.x+size/2, vPosition.y, vPosition.z+size/2};
	GLfloat fVelocityMin[] = {-5.0f, 0.0f, -5.0f};
	GLfloat fVelocityMax[] = {5.0f, 25.0f, 5.0f};

	psEngine->createBoxParticleGenerator(fCornerVertexA, fCornerVertexB, fVelocityMin, fVelocityMax, 0.05f, 0.05f, 30, 40, 1.5f, 2.0f, 2.0f);

}

/*-----------------------------------------------

Name:    RenderScene

Params:  lpParam - Pointer to anything you want.

Result:  Renders whole scene.

/*---------------------------------------------*/

vector<GLuint> generators;
vector<float> genTimes;

GLvoid ApplyExplosion(glm::vec3 vPosition, GLfloat fForce, GL_UNSIGNED_BYTE r, GL_UNSIGNED_BYTE g, GL_UNSIGNED_BYTE b)
{
	GLfloat size = 20.0f;
	GLfloat fCornerVertexA[] = {vPosition.x-size/2, vPosition.y, vPosition.z-size/2};
	GLfloat fCornerVertexB[] = {vPosition.x+size/2, vPosition.y, vPosition.z+size/2};
	GLfloat fVelocityMin[] = {-25.0f, -25.0f, -25.0f};
	GLfloat fVelocityMax[] = {25.0f, 25.0f, 25.0f};

	GLuint id = psEngine->createPointParticleGenerator((float*)glm::value_ptr(vPosition), fVelocityMin, fVelocityMax, 0.05f, 0.10f, 30, 40, 1.5f, 1.0f, 1);
	psEngine->setGeneratorColor(id, r, g, b);
	generators.push_back(id);
	genTimes.push_back(1);

	FOR(i, ESZ(nodes))
	{
		btTransform trans = nodes[i]._body->getWorldTransform();
		glm::vec3 vNodePosition(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
		glm::vec3 vDif = vNodePosition-vPosition;
		GLfloat fLen = glm::length(vDif);
		vDif = glm::normalize(vDif);
		GLfloat fAppliedForce = fForce/(pow(fLen, 0.05f)+1.0f);
		nodes[i]._body->applyImpulse(btVector3(vDif.x*fAppliedForce, vDif.y*fAppliedForce, vDif.z*fAppliedForce), btVector3(vPosition.x, vPosition.y, vPosition.z));
	}
	FOR(i, ESZ(capnodes))
	{
		btTransform trans = capnodes[i]._body->getWorldTransform();
		glm::vec3 vNodePosition(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
		glm::vec3 vDif = vNodePosition-vPosition;
		GLfloat fLen = glm::length(vDif);
		vDif = glm::normalize(vDif);
		GLfloat fAppliedForce = fForce/(pow(fLen, 0.05f)+1.0f);
		capnodes[i]._body->applyImpulse(btVector3(vDif.x*fAppliedForce, vDif.y*fAppliedForce, vDif.z*fAppliedForce), btVector3(vPosition.x, vPosition.y, vPosition.z));
	}
}

GLvoid RenderScene(GLvoid* lpParam)
{
	// Typecast lpParam to COpenGLControl pointer
	COpenGLControl* oglControl = (COpenGLControl*)lpParam;
	oglControl->ResizeOpenGLViewportFull();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	spMain.UseProgram();

	spMain.SetUniform("matrices.projMatrix", oglControl->GetProjectionMatrix());
	spMain.SetUniform("matrices.viewMatrix", cCamera.Look());

	spMain.SetUniform("gSampler", 0);

	spMain.SetUniform("matrices.modelMatrix", glm::mat4(1.0));
	spMain.SetUniform("matrices.normalMatrix", glm::mat4(1.0));
	spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 1));

	// This values will set the darkness of whole scene, that's why such name of variable :D
	static GLfloat fAngleOfDarkness = 45.0f;
	// You can play with direction of light with '+' and '-' key
	if(Keys::Key(VK_ADD))fAngleOfDarkness += appMain.sof(90);
	if(Keys::Key(VK_SUBTRACT))fAngleOfDarkness -= appMain.sof(90);
	// Set the directional vector of light
	dlSun.vDirection = glm::vec3(-sin(fAngleOfDarkness*3.1415f/180.0f), -cos(fAngleOfDarkness*3.1415f/180.0f), 0.0f);
	dlSun.SetUniformData(&spMain, "sunLight");

	spMain.SetUniform("matrices.modelMatrix", glm::translate(glm::mat4(1.0), cCamera.vEye));

	spMain.SetUniform("ignoreLights", 1);
	sbMainSkybox.RenderSkybox();
	spMain.SetUniform("ignoreLights", 0);

	glm::vec3 vSpotLightPos = cCamera.vEye;
	glm::vec3 vCameraDir = glm::normalize(cCamera.vView-cCamera.vEye);
	// Move down a little
	vSpotLightPos.y -= 0.2f;
	// Find direction of spotlight
	glm::vec3 vSpotLightDir = (vSpotLightPos+vCameraDir*75.0f)-vSpotLightPos;
	vSpotLightDir = glm::normalize(vSpotLightDir);
	// Find vector of horizontal offset
	glm::vec3 vHorVector = glm::cross(cCamera.vView-cCamera.vEye, cCamera.vUp);
	vSpotLightPos += vHorVector*0.3f;
	// Set it
	slFlashLight.vPosition = vSpotLightPos;
	slFlashLight.vDirection = vSpotLightDir;

	slFlashLight.SetUniformData(&spMain, "slFlashLight");

	spMain.SetUniform("ptLight.vColor", glm::vec3(0.0f, 0.0f, 1.0f));
	spMain.SetUniform("ptLight.vPosition", cCamera.vEye);
	spMain.SetUniform("ptLight.fAmbient", 0.15f);
	static GLfloat fConst = 0.3f, fLineaer = 0.007f, fExp = 0.00008f;

	if(Keys::Key('P'))fConst += appMain.sof(0.2f);
	if(Keys::Key('O'))fConst -= appMain.sof(0.2f);
	if(Keys::Key('L'))fLineaer += appMain.sof(0.01f);
	if(Keys::Key('K'))fLineaer -= appMain.sof(0.01f);
	if(Keys::Key('M'))fExp += appMain.sof(0.0001f);
	if(Keys::Key('N'))fExp -= appMain.sof(0.0001f);

	spMain.SetUniform("ptLight.fConstantAtt", fConst);
	spMain.SetUniform("ptLight.fLinearAtt", fLineaer);
	spMain.SetUniform("ptLight.fExpAtt", fExp);


	spMain.SetUniform("ptFire.vColor", glm::vec3(1.0f, 0.5f, 0.0f));
	spMain.SetUniform("ptFire.vPosition", glm::vec3(-41.56f, 2.74f, 23.47f));
	spMain.SetUniform("ptFire.fAmbient", 0.15f);
	spMain.SetUniform("ptFire.fConstantAtt", fConst);
	spMain.SetUniform("ptFire.fLinearAtt", fLineaer);
	spMain.SetUniform("ptFire.fExpAtt", fExp);

	glBindVertexArray(uiVAOSceneObjects);

	tTextures[0].BindTexture();
	spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", glm::mat4(1.0));
	glDrawArrays(GL_TRIANGLES, 36, 6);

	FOR(mat, 4)
	{
		GLint m = iRenderOrder[mat];
		tTextures[m+1].BindTexture();
		if(m == 2)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			spMain.SetUniform("vColor", glm::vec4(1, 1, 1, 0.5f));
		}
		FOR(i, ESZ(nodes))
		{
			if(nodes[i].iMaterialUsed != m)continue;
			btTransform trans2 = nodes[i]._body->getWorldTransform();
			glm::vec3 vPositionBox(trans2.getOrigin().x(), trans2.getOrigin().y(), trans2.getOrigin().z());
			glm::mat4 mModelFinal = glm::translate(glm::mat4(1.0), vPositionBox);

			btMatrix3x3 rotMatrix = btMatrix3x3(trans2.getRotation());
			GLfloat z,y,x;
			rotMatrix.getEulerZYX(z,y,x);
			btScalar m[16]; FOR(j, 16)m[j] = 0.0f;
			rotMatrix.getOpenGLSubMatrix(m);
			GLfloat aaa[16]; FOR(j, 16)aaa[j] = m[j]; aaa[15] = 1.0f;
			glm::mat4 bbb = glm::make_mat4(aaa);
			mModelFinal = mModelFinal*bbb;

			mModelFinal = glm::scale(mModelFinal, nodes[i].vHalfExtents);

			spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModelFinal);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		FOR(i, ESZ(capnodes))
		{
			if(capnodes[i].iMaterialUsed != m)continue;
			btTransform trans2 = capnodes[i]._body->getWorldTransform();
			glm::vec3 vPositionBox(trans2.getOrigin().x(), trans2.getOrigin().y(), trans2.getOrigin().z());
			glm::mat4 mModelFinal = glm::translate(glm::mat4(1.0), vPositionBox);

			btMatrix3x3 rotMatrix = btMatrix3x3(trans2.getRotation());
			GLfloat z,y,x;
			rotMatrix.getEulerZYX(z,y,x);
			btScalar m[16]; FOR(j, 16)m[j] = 0.0f;
			rotMatrix.getOpenGLSubMatrix(m);
			GLfloat aaa[16]; FOR(j, 16)aaa[j] = m[j]; aaa[15] = 1.0f;
			glm::mat4 bbb = glm::make_mat4(aaa);
			mModelFinal = mModelFinal*bbb;
			mModelFinal = glm::scale(mModelFinal, glm::vec3(capnodes[i].fRadius, capnodes[i].fHeight*2.0f/3.0f, capnodes[i].fRadius));

			spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModelFinal);
			glDrawArrays(GL_TRIANGLES, 42, iCapsuleFaces*3);
		}

		FOR(i, ESZ(capnodesX))
		{
			if(capnodesX[i].iMaterialUsed != m)continue;
			btTransform trans2 = capnodesX[i]._body->getWorldTransform();
			glm::vec3 vPositionBox(trans2.getOrigin().x(), trans2.getOrigin().y(), trans2.getOrigin().z());
			glm::mat4 mModelFinal = glm::translate(glm::mat4(1.0), vPositionBox);

			btMatrix3x3 rotMatrix = btMatrix3x3(trans2.getRotation());
			GLfloat z,y,x;
			rotMatrix.getEulerZYX(z,y,x);
			btScalar m[16]; FOR(j, 16)m[j] = 0.0f;
			rotMatrix.getOpenGLSubMatrix(m);
			GLfloat aaa[16]; FOR(j, 16)aaa[j] = m[j]; aaa[15] = 1.0f;
			glm::mat4 bbb = glm::make_mat4(aaa);

			mModelFinal = mModelFinal*bbb;
			mModelFinal = glm::rotate(mModelFinal, 90.0f, glm::vec3(0, 0, -1));
			mModelFinal = glm::scale(mModelFinal, glm::vec3(capnodesX[i].fRadius, capnodesX[i].fHeight*2.0f/3.0f, capnodesX[i].fRadius));

			spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModelFinal);
			glDrawArrays(GL_TRIANGLES, 42, iCapsuleFaces*3);
		}

		FOR(i, ESZ(capnodesZ))
		{
			if(capnodesZ[i].iMaterialUsed != m)continue;
			btTransform trans2 = capnodesZ[i]._body->getWorldTransform();
			glm::vec3 vPositionBox(trans2.getOrigin().x(), trans2.getOrigin().y(), trans2.getOrigin().z());
			glm::mat4 mModelFinal = glm::translate(glm::mat4(1.0), vPositionBox);

			btMatrix3x3 rotMatrix = btMatrix3x3(trans2.getRotation());
			GLfloat z,y,x;
			rotMatrix.getEulerZYX(z,y,x);
			btScalar m[16]; FOR(j, 16)m[j] = 0.0f;
			rotMatrix.getOpenGLSubMatrix(m);
			GLfloat aaa[16]; FOR(j, 16)aaa[j] = m[j]; aaa[15] = 1.0f;
			glm::mat4 bbb = glm::make_mat4(aaa);

			mModelFinal = mModelFinal*bbb;
			mModelFinal = glm::rotate(mModelFinal, 90.0f, glm::vec3(1, 0, 0));
			mModelFinal = glm::scale(mModelFinal, glm::vec3(capnodesZ[i].fRadius, capnodesZ[i].fHeight*2.0f/3.0f, capnodesZ[i].fRadius));

			spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModelFinal);
			glDrawArrays(GL_TRIANGLES, 42, iCapsuleFaces*3);
		}

		FOR(i, ESZ(grenades))
		{
			if(grenades[i].iMaterialUsed != m)continue;
			btTransform trans2 = grenades[i]._body->getWorldTransform();
			glm::vec3 vPositionGrenade(trans2.getOrigin().x(), trans2.getOrigin().y(), trans2.getOrigin().z());
			glm::mat4 mModelFinal = glm::translate(glm::mat4(1.0), vPositionGrenade);

			btMatrix3x3 rotMatrix = btMatrix3x3(trans2.getRotation());
			GLfloat z,y,x;
			rotMatrix.getEulerZYX(z,y,x);
			btScalar m[16]; FOR(j, 16)m[j] = 0.0f;
			rotMatrix.getOpenGLSubMatrix(m);
			GLfloat aaa[16]; FOR(j, 16)aaa[j] = m[j]; aaa[15] = 1.0f;
			glm::mat4 bbb = glm::make_mat4(aaa);
			mModelFinal = mModelFinal*bbb;
			mModelFinal = glm::scale(mModelFinal, glm::vec3(grenades[i].fRadius, grenades[i].fHeight*2.0f/3.0f, grenades[i].fRadius));

			spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModelFinal);
			glDrawArrays(GL_TRIANGLES, 42, iCapsuleFaces*3);

			grenadeTimes[i] -= appMain.sof(1.0f);
			if(grenadeTimes[i] <= 0.0f)
			{
				grenadeTimes.erase(grenadeTimes.begin()+i);
				_world->removeRigidBody(grenades[i]._body);
				grenades.erase(grenades.begin()+i);
				i--;
				ApplyExplosion(vPositionGrenade, 100.0f, 255, 128, 0);

			}
		}

		FOR(i, ESZ(rockets))
		{
			if(rockets[i].iMaterialUsed != m)continue;
			btTransform trans2 = rockets[i]._body->getWorldTransform();
			glm::vec3 vPositionRocket(trans2.getOrigin().x(), trans2.getOrigin().y(), trans2.getOrigin().z());
			glm::mat4 mModelFinal = glm::translate(glm::mat4(1.0), vPositionRocket);

			btMatrix3x3 rotMatrix = btMatrix3x3(trans2.getRotation());
			GLfloat z,y,x;
			rotMatrix.getEulerZYX(z,y,x);
			btScalar m[16]; FOR(j, 16)m[j] = 0.0f;
			rotMatrix.getOpenGLSubMatrix(m);
			GLfloat aaa[16]; FOR(j, 16)aaa[j] = m[j]; aaa[15] = 1.0f;
			glm::mat4 bbb = glm::make_mat4(aaa);
			mModelFinal = mModelFinal*bbb;
			mModelFinal = glm::scale(mModelFinal, glm::vec3(rockets[i].fRadius, rockets[i].fHeight*2.0f/3.0f, rockets[i].fRadius));

			spMain.SetModelAndNormalMatrix("matrices.modelMatrix", "matrices.normalMatrix", mModelFinal);
			glDrawArrays(GL_TRIANGLES, 42, iCapsuleFaces*3);

		}

		if(m == 2)glDisable(GL_BLEND);
	}



	psEngine->setProjectionMatrix(glm::value_ptr(*oglControl->GetProjectionMatrix()));
	psEngine->setLookAt(cCamera.vEye.x, cCamera.vEye.y, cCamera.vEye.z, cCamera.vView.x, cCamera.vView.y, cCamera.vView.z, cCamera.vUp.x, cCamera.vUp.y, cCamera.vUp.z);
	GLfloat fTimePassed = appMain.sof(1.0f);
	psEngine->updateParticles(fTimePassed);
	psEngine->renderParticles();

	FOR(i, ESZ(generators))
	{
		genTimes[i] -= appMain.sof(1.0f);
		if(genTimes[i] < 0.0f)
		{
			psEngine->deleteParticleGenerator(generators[i]);
			generators.erase(generators.begin()+i);
			genTimes.erase(genTimes.begin()+i);
			i--;
		}
	}

	cCamera.Update();

	// PrGLint something over scene
	
	spFont2D.UseProgram();
	glDisable(GL_DEPTH_TEST);
	spFont2D.SetUniform("matrices.projMatrix", oglControl->GetOrthoMatrix());

	GLint w = oglControl->GetViewportWidth(), h = oglControl->GetViewportHeight();
	
	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	ftFont.Print("www.mbsoftworks.sk", 20, 20, 24);

	ftFont.PrintFormatted(20, h-30, 20, "FPS: %d", oglControl->GetFPS());
	spFont2D.SetUniform("vColor", glm::vec4(0.46f, 0.73f, 1.0f, 1.0f));
	ftFont.PrintFormatted(20, h-80, 20, "Current Item: %s", sItemNames[iCurrentItem].c_str() );
	spFont2D.SetUniform("vColor", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
	ftFont.PrintFormatted(20, h-110, 20, "Material: %s (Press MBUTTON to change)", sMaterials[iCurrentMaterial].c_str() );
	spFont2D.SetUniform("vColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	GLfloat fBoxWeight = fMassMultiplier[iCurrentMaterial]*fBoxSize*fMassMultiplier[iCurrentMaterial]*fBoxSize;
	GLfloat fCapsuleWeight = fMassMultiplier[iCurrentMaterial]*150.0f*fMassMultiplier[iCurrentMaterial]*fHeight;
	if(iCurrentItem == 0)
	{
		ftFont.PrintFormatted(20, h-170, 20, "Size (half extents): %.2f (Press '+' and '-' to change)", fBoxSize);
		ftFont.PrintFormatted(20, h-200, 20, "Approximate weight: %.2f kg", fBoxWeight);
		if(Keys::Key(VK_ADD))fBoxSize += appMain.sof(3.0f);
		if(Keys::Key(VK_SUBTRACT))fBoxSize -= appMain.sof(3.0f);
	}
	else if(iCurrentItem == 1)
	{
		ftFont.PrintFormatted(20, h-170, 20, "Height: %.2f (Press '+' and '-' to change)", fHeight);
		ftFont.PrintFormatted(20, h-200, 20, "Radius: %.2f (Press '*' and '/' to change)", fRadius);
		ftFont.PrintFormatted(20, h-230, 20, "Approximate weight: %.2f kg", fCapsuleWeight);
		if(Keys::Key(VK_ADD))fHeight += appMain.sof(3.0f);
		if(Keys::Key(VK_SUBTRACT))fHeight -= appMain.sof(3.0f);

		if(Keys::Key(VK_DIVIDE))fRadius += appMain.sof(3.0f);
		if(Keys::Key(VK_MULTIPLY))fRadius -= appMain.sof(3.0f);
	}

	//ftFont.PrintFormatted(20, h-260, 20, "ATTENUATIONS:\nConst: %.4f\nLinear: %.4f\nExp: %.6f", fConst, fLineaer, fExp);

	glEnable(GL_DEPTH_TEST);
	if(Keys::Onekey(VK_ESCAPE))PostQuitMessage(0);

	if(Keys::Onekey(VK_MBUTTON))iCurrentMaterial = (iCurrentMaterial+1)&3;
	if(Keys::Onekey(VK_RBUTTON))
	{
		iCurrentItem = (iCurrentItem+1)&3;
	}

	static GLboolean bThrowing = false;
	static GLfloat fInitForce = 0.0f;

	GLint iMaterialToConsider = iCurrentItem == 2 ? 0 : iCurrentMaterial;

	if(Keys::Onekey(VK_LBUTTON) && !bThrowing)
	{
		bThrowing = true;
		fInitForce = 0.0f;
	}
	else if(Keys::Key(VK_LBUTTON) && bThrowing && fInitForce < fForcePowers[iMaterialToConsider])
	{
		fInitForce += appMain.sof(fForcePowers[iCurrentMaterial]);
	}
	else if(bThrowing && (!Keys::Key(VK_LBUTTON) || fInitForce >= fForcePowers[iMaterialToConsider]))
	{
		if(iCurrentItem == 0)
		{
			CBoxPhysicsNode box;
			box.SetPosition(cCamera.vEye+(cCamera.vView-cCamera.vEye)*5.0f);
			box.SetBoxSize(glm::vec3(fBoxSize, fBoxSize, fBoxSize));
			box.CreateNode(fBoxWeight, (cCamera.vView-cCamera.vEye)*fInitForce*fBoxWeight, fRestitution[iMaterialToConsider], fFriction[iMaterialToConsider], _world);
			box.iMaterialUsed = iMaterialToConsider;
			
			nodes.push_back(box);
		}
		else if(iCurrentItem == 1)
		{
			CCapsulePhysicsNode capsule;
			capsule.SetPosition(cCamera.vEye+(cCamera.vView-cCamera.vEye)*5.0f);
			capsule.SetCapsuleSize(fRadius, fHeight);
			capsule.CreateNode(fCapsuleWeight, (cCamera.vView-cCamera.vEye)*fInitForce*fCapsuleWeight/(iCurrentItem <= 2 ? 10.0f : 1.0f), fRestitution[iMaterialToConsider], fFriction[iMaterialToConsider],  _world);
			capsule.iMaterialUsed = iMaterialToConsider;

			capsule._body->setGravity(btVector3(0.0f, -15.0f, 0.0f));

			capnodes.push_back(capsule);
		}
		else if(iCurrentItem == 2)
		{
			CCapsulePhysicsNode grenade;
			grenade.SetPosition(cCamera.vEye+(cCamera.vView-cCamera.vEye)*5.0f);
			grenade.SetCapsuleSize(1, 2);
			grenade.CreateNode(3, (cCamera.vView-cCamera.vEye)*fInitForce, fRestitution[iMaterialToConsider], fFriction[iMaterialToConsider],  _world);
			grenade.iMaterialUsed = 3;

			grenades.push_back(grenade);
			grenadeTimes.push_back(2.5f);
		}

		else if(iCurrentItem == 3)
		{
			CCapsulePhysicsNode rocket;
			rocket.SetPosition(cCamera.vEye+(cCamera.vView-cCamera.vEye)*5.0f);
			rocket.SetCapsuleSize(0.5f, 4);
			rocket.CreateNode(3, (cCamera.vView-cCamera.vEye)*100.0f, fRestitution[iMaterialToConsider], fFriction[iMaterialToConsider],  _world);
			rocket.iMaterialUsed = 3;

			rocket._body->setGravity(btVector3(0, 0, 0));
			rockets.push_back(rocket);
		}
		bThrowing = false;
	}

	GLfloat fA = appMain.sof(fSimulationSpeed);

	FOR(i, 1)
	{
		static GLboolean baba = true;
		if(baba)
		_world->stepSimulation(fA, iSubSims);
		if(Keys::Onekey(' '))baba = !baba;
	}

	GLint numManifolds = _world->getDispatcher()->getNumManifolds();
	set<int> rocketsToExplode;
	for (GLint i=0;i<numManifolds;i++)
	{
		//2
		btPersistentManifold* contactManifold =  _world->getDispatcher()->getManifoldByIndexInternal(i);

		//3
		GLint numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			//5
			const btCollisionObject* obA = contactManifold->getBody0();
			const btCollisionObject* obB = contactManifold->getBody1();

			FOR(j, ESZ(rockets))
			{
				if(rockets[j]._body == (btRigidBody*)obA->getUserPointer() || rockets[j]._body == (btRigidBody*)obB->getUserPointer())
				{
					rocketsToExplode.insert(j);
				}
			}
		}
	}
	for(set<int>::reverse_iterator it = rocketsToExplode.rbegin(); it != rocketsToExplode.rend(); it++)
	{
		GLint ind = *it;
		btTransform trans2 = rockets[ind]._body->getWorldTransform();
		glm::vec3 vPositionRocket(trans2.getOrigin().x(), trans2.getOrigin().y(), trans2.getOrigin().z());
		_world->removeRigidBody(rockets[ind]._body);
		rockets.erase(rockets.begin()+ind);
		ApplyExplosion(vPositionRocket, 40.0f, 0, 153, 255);
	}

	btVector3 rode = nodes[ESZ(nodes)-1]._body->getAngularVelocity();
	rode *= 0.9f;
	nodes[ESZ(nodes)-1]._body->setAngularVelocity(rode);

	if(Keys::Key('B'))ptrs[ESZ(ptrs)-1]->applyImpulse(btVector3(appMain.sof(fImpulseSpeed), 0, 0), btVector3(0, 0, 0));
	if(Keys::Onekey('F'))slFlashLight.bOn = !slFlashLight.bOn;

	oglControl->SwapBuffers();
}

/*-----------------------------------------------

Name:    ReleaseScene

Params:  lpParam - Pointer to anything you want.

Result:  Releases OpenGL scene.

/*---------------------------------------------*/

GLvoid ReleaseScene(GLvoid* lpParam)
{
	FOR(i, NUMTEXTURES)tTextures[i].DeleteTexture();
	sbMainSkybox.DeleteSkybox();

	spMain.DeleteProgram();
	spOrtho2D.DeleteProgram();
	spFont2D.DeleteProgram();
	FOR(i, NUMSHADERS)shShaders[i].DeleteShader();
	ftFont.DeleteFont();

	glDeleteVertexArrays(1, &uiVAOSceneObjects);
	vboSceneObjects.DeleteVBO();
}