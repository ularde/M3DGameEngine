#pragma once
#include "Class.h"
#include "Mesh.h"
#include "RigidStatic.h"
#include "Entity.h"
#include "EntityComponent.h"
#include "GeometricShape.h"

class MScriptableObject;
class MRigidStatic;
class MMeshComponent;
class MTriangleMesh;
class MGeometricShape;

struct MGeometryInstance {
	MGeometryInstance(MRigidStatic* owner, MTriangleMesh* shape) {
		mOwner = owner;
		mShape = shape;
	}

	MGeometryInstance(MMeshComponent* owner, MTriangleMesh* shape) {
		mOwner = owner;
		mShape = shape;
	}
	MScriptableObject* mOwner = 0;
	MGeometricShape* mShape = 0;
};