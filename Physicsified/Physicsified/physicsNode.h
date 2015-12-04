#pragma once

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btHashMap.h>


class CPhysicsNode
{
public:
	btRigidBody* _body;
	btCollisionShape* _shape;

	int iMaterialUsed;

	virtual void CreateNode(float fMass, glm::vec3 initVelocity, float fRestitution, float fFriction, btDiscreteDynamicsWorld* _world) = NULL;
	void SetPosition(glm::vec3 vInitPosition);

	CPhysicsNode();
protected:
	glm::vec3 vPositionToSet;
};


class CBoxPhysicsNode : public CPhysicsNode
{
public:
	void CreateNode(float fMass, glm::vec3 initVelocity, float fRestitution, float fFriction, btDiscreteDynamicsWorld* _world);
	void SetBoxSize(glm::vec3 a_vHalfExtents);

	glm::vec3 vHalfExtents;
};

class CCapsulePhysicsNode : public CPhysicsNode
{
public:
	void CreateNode(float fMass, glm::vec3 initVelocity, float fRestitution, float fFriction, btDiscreteDynamicsWorld* _world);
	void SetCapsuleSize(float a_fRadius, float a_fHeight);

	float fHeight;
	float fRadius;
};

class CCapsulePhysicsNodeX : public CPhysicsNode
{
public:
	void CreateNode(float fMass, glm::vec3 initVelocity, float fRestitution, float fFriction, btDiscreteDynamicsWorld* _world);
	void SetCapsuleSize(float a_fRadius, float a_fHeight);

	float fHeight;
	float fRadius;
};


class CCapsulePhysicsNodeZ : public CPhysicsNode
{
public:
	void CreateNode(float fMass, glm::vec3 initVelocity, float fRestitution, float fFriction, btDiscreteDynamicsWorld* _world);
	void SetCapsuleSize(float a_fRadius, float a_fHeight);

	float fHeight;
	float fRadius;
};
