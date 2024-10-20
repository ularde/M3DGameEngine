#pragma once
#include <string>
#include <Python.h>
#include <jemalloc/jemalloc.h>
#include <sol/sol.hpp>

#include "Platforms.h"

class MBasicPlatform;

class MScriptableObject {
public:
	std::string mClassName;
	std::string mPrototypeName;
	std::string mName;
	std::string mLuaScriptPath, mLuaScriptDir, mLuaScriptName;
	virtual void InitializeLuaInstance() = 0;
	MBasicPlatform* GetPlatform() { return gPlatform; }
	void* operator new(size_t size) { return je_malloc(size); }
	void operator delete[](void* ptr) {return je_free(ptr); }
		bool IsRigidBodyPhysicsProxy() {
		return (mClassName == "RigidBodyPhysicsProxySphere" ||
			mClassName == "RigidBodyPhysicsProxyBox" ||
			mClassName == "CharacterController");
	}
	bool IsMeshComponent() {
		return (mClassName == "StaticMeshComponent" ||
			mClassName == "SkeletonMesh");
	}
	bool IsCameraComponent() {
		return (mClassName == "FirstPersonCamera" ||
			mClassName == "CinematicCamera");
	}
	bool IsStaticMeshComponent() { return mClassName == "StaticMeshComponent"; }
	bool IsRigidStatic() { return (mClassName == "RigidStatic"); }
	bool IsEntityComponent() { return (mPrototypeName == "EntityComponent"); }
	bool IsFirstPersonCamera() { return (mClassName == "FirstPersonCamera"); }
	bool IsCinematicCamera() { return (mClassName == "CinematicCamera"); }
	bool IsRigidBodyPhysicsProxySphere() { return (mClassName == "RigidBodyPhysicsProxySphere"); }
	bool IsRigidBodyPhysicsProxyBox() { return (mClassName == "RigidBodyPhysicsProxyBox"); }
	bool IsCharacterController() { return (mClassName == "CharacterController"); }
	bool IsContainer() { return (mPrototypeName == "Container"); }
	bool IsEntity() { return (mPrototypeName == "Entity"); }
	bool IsScene() { return (mClassName == "Scene"); }
	bool IsValidatedGeometryObjectOwner() { return (IsMeshComponent() || IsRigidStatic()); }
protected:
	MBasicPlatform* gPlatform = NULL;
};

class MUnscriptableObject {
public:
	MBasicPlatform* GetPlatform() { return gPlatform; }
	void* operator new(size_t size) { return je_malloc(size); }
	void operator delete[](void* ptr) {return je_free(ptr); }
protected:
	MBasicPlatform* gPlatform = NULL;
};