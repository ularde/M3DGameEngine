#pragma once
#include "Class.h"
#include "TriangleMesh.h"
#include "RigidStatic.h"
#include "Entity.h"
#include "EntityComponent.h"
#include "RenderObject.h"

class MScriptableObject;
class MRigidStatic;
class MMeshComponent;
class MTriangleMesh;
class MRenderObject;

struct MRenderInstance {
	MRenderInstance(MRigidStatic* owner, MTriangleMesh* shape) {
		mOwner = owner;
		mShape = shape;
	}

	MRenderInstance(MMeshComponent* owner, MTriangleMesh* shape) {
		mOwner = owner;
		mShape = shape;
	}
	MScriptableObject* mOwner = 0;
	MRenderObject* mShape = 0;
};