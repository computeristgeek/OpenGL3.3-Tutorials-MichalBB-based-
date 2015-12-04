#pragma once

#include <btBulletDynamicsCommon.h>
#include <LinearMath/btHashMap.h>


class CPhysicsNode
{
public:
	btRigidBody* _body;
	btCollisionShape* _shape;

	GLint iMaterialUsed;

	virtual GLvoid CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world) = NULL;
	GLvoid SetPosition(glm::vec3 vInitPosition);

	CPhysicsNode();
protected:
	glm::vec3 vPositionToSet;
};


class CBoxPhysicsNode : public CPhysicsNode
{
public:
	GLvoid CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world);
	GLvoid SetBoxSize(glm::vec3 a_vHalfExtents);

	glm::vec3 vHalfExtents;
};

class CCapsulePhysicsNode : public CPhysicsNode
{
public:
	GLvoid CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world);
	GLvoid SetCapsuleSize(GLfloat a_fRadius, GLfloat a_fHeight);

	GLfloat fHeight;
	GLfloat fRadius;
};

class CCapsulePhysicsNodeX : public CPhysicsNode
{
public:
	GLvoid CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world);
	GLvoid SetCapsuleSize(GLfloat a_fRadius, GLfloat a_fHeight);

	GLfloat fHeight;
	GLfloat fRadius;
};


class CCapsulePhysicsNodeZ : public CPhysicsNode
{
public:
	GLvoid CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world);
	GLvoid SetCapsuleSize(GLfloat a_fRadius, GLfloat a_fHeight);

	GLfloat fHeight;
	GLfloat fRadius;
};
