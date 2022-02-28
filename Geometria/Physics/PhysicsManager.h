#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#include "PxConfig.h"
#include "PxPhysics.h"
#include "PxPhysicsAPI.h"

struct BoxCollider;

#include <vector>

#define PHYSICSMANAGER_H
struct PhysicsContactListener : public physx::PxSimulationEventCallback
{
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) {}
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
};

class PhysicsManager : public ScriptBehaviour
{
public:

	physx::PxDefaultAllocator gAllocator;
	physx::PxDefaultErrorCallback gErrorCallback;

	physx::PxFoundation* gFoundation = nullptr;
	static physx::PxPhysics* gPhysics;

	physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
	static physx::PxScene* gScene;

	static physx::PxMaterial* gMaterial;

	static std::vector<physx::PxRigidDynamic*> allDynamics;
	static std::vector<physx::PxRigidStatic*> allStatics;

	static bool preUpdate;
	static bool foundationCreated, physicsCreated, sceneCreated;

	static Vector3 gravity;

	void OnStartup();
	void OnStart();
	void OnUpdate();
	void OnDestroy();

	static PhysicsContactListener listener;

	static physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

	static void SetGravity(Vector3 g);

	static physx::PxRigidStatic* CreateStaticBox(BoxCollider& collider, Vector3 position, Vector3 scale);
	static physx::PxRigidDynamic* CreateDynamicBox(BoxCollider& collider, Vector3 position, Vector3 scale);

	static bool Raycast(Vector3 origin, Vector3 direction, int maxDistance);
};