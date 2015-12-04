#include "common_header.h"

#include "physicsNode.h"

#ifdef _DEBUG

#pragma comment(lib, "BulletCollision_Debug.lib")
#pragma comment(lib, "BulletDynamics_Debug.lib")
#pragma comment(lib, "BulletFileLoader_Debug.lib")
#pragma comment(lib, "LinearMath_Debug.lib")

#else

#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletFileLoader.lib")
#pragma comment(lib, "LinearMath.lib")

#endif

CPhysicsNode::CPhysicsNode()
{
	_body = NULL;
	_shape = NULL;
}

GLvoid CPhysicsNode::SetPosition(glm::vec3 vInitPosition)
{
	vPositionToSet = vInitPosition;
}

GLvoid CBoxPhysicsNode::CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world)
{
	_shape = new btBoxShape(btVector3(vHalfExtents.x, vHalfExtents.y, vHalfExtents.z));

	btQuaternion rotation;
	rotation.setEulerZYX(0.0f, 0.0f, 0.0f);

	//2
	btVector3 position = btVector3(vPositionToSet.x, vPositionToSet.y, vPositionToSet.z);

	//3
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));

	//4
	btScalar bodyMass = fMass;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(bodyMass, bodyInertia);

	//5
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, _shape, bodyInertia);

	//6
	bodyCI.m_restitution = fRestitution;
	bodyCI.m_friction = fFriction;

	//7
	_body = new btRigidBody(bodyCI);

	//8
	_body->setUserPointer((void*)_body);

	//9
	_body->setLinearFactor(btVector3(1,1,1));

	_body->setLinearVelocity(btVector3(initVelocity.x, initVelocity.y, initVelocity.z));

	_world->addRigidBody(_body);
}

GLvoid CBoxPhysicsNode::SetBoxSize(glm::vec3 a_vHalfExtents)
{
	vHalfExtents = a_vHalfExtents;
}

GLvoid CCapsulePhysicsNode::SetCapsuleSize(GLfloat a_fRadius, GLfloat a_fHeight)
{
	fRadius = a_fRadius;
	fHeight = a_fHeight;
}

GLvoid CCapsulePhysicsNode::CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world)
{
	_shape = new btCapsuleShape(fRadius, fHeight);

	btQuaternion rotation;
	rotation.setEulerZYX(0.0f, 0.0f, 0.0f);

	//2
	btVector3 position = btVector3(vPositionToSet.x, vPositionToSet.y, vPositionToSet.z);

	//3
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));

	//4
	btScalar bodyMass = fMass;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(bodyMass, bodyInertia);

	//5
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, _shape, bodyInertia);

	//6
	bodyCI.m_restitution = fRestitution;
	bodyCI.m_friction = fFriction;

	//7
	_body = new btRigidBody(bodyCI);

	//8
	_body->setUserPointer((void*)_body);

	//9
	_body->setLinearFactor(btVector3(1,1,1));

	_body->setLinearVelocity(btVector3(initVelocity.x, initVelocity.y, initVelocity.z));

	_world->addRigidBody(_body);
}


GLvoid CCapsulePhysicsNodeX::SetCapsuleSize(GLfloat a_fRadius, GLfloat a_fHeight)
{
	fRadius = a_fRadius;
	fHeight = a_fHeight;
}

GLvoid CCapsulePhysicsNodeX::CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world)
{
	_shape = new btCapsuleShapeX(fRadius, fHeight);

	btQuaternion rotation;
	rotation.setEulerZYX(0.0f, 0.0f, 0.0f);

	//2
	btVector3 position = btVector3(vPositionToSet.x, vPositionToSet.y, vPositionToSet.z);

	//3
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));

	//4
	btScalar bodyMass = fMass;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(bodyMass, bodyInertia);

	//5
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, _shape, bodyInertia);

	//6
	bodyCI.m_restitution = fRestitution;
	bodyCI.m_friction = fFriction;

	//7
	_body = new btRigidBody(bodyCI);

	//8
	_body->setUserPointer((void*)_body);

	//9
	_body->setLinearFactor(btVector3(1,1,1));

	_body->setLinearVelocity(btVector3(initVelocity.x, initVelocity.y, initVelocity.z));

	_world->addRigidBody(_body);
}

GLvoid CCapsulePhysicsNodeZ::SetCapsuleSize(GLfloat a_fRadius, GLfloat a_fHeight)
{
	fRadius = a_fRadius;
	fHeight = a_fHeight;
}

GLvoid CCapsulePhysicsNodeZ::CreateNode(GLfloat fMass, glm::vec3 initVelocity, GLfloat fRestitution, GLfloat fFriction, btDiscreteDynamicsWorld* _world)
{
	_shape = new btCapsuleShapeZ(fRadius, fHeight);

	btQuaternion rotation;
	rotation.setEulerZYX(0.0f, 0.0f, 0.0f);

	//2
	btVector3 position = btVector3(vPositionToSet.x, vPositionToSet.y, vPositionToSet.z);

	//3
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));

	//4
	btScalar bodyMass = fMass;
	btVector3 bodyInertia;
	_shape->calculateLocalInertia(bodyMass, bodyInertia);

	//5
	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, _shape, bodyInertia);

	//6
	bodyCI.m_restitution = fRestitution;
	bodyCI.m_friction = fFriction;

	//7
	_body = new btRigidBody(bodyCI);

	//8
	_body->setUserPointer((void*)_body);

	//9
	_body->setLinearFactor(btVector3(1,1,1));

	_body->setLinearVelocity(btVector3(initVelocity.x, initVelocity.y, initVelocity.z));

	_world->addRigidBody(_body);
}