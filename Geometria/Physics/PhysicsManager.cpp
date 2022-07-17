#include "PhysicsManager.h"
#include "../Physics/Colliders/BoxCollider.h"
#include "Graphics/Cores/Camera/Camera.h"

physx::PxRigidDynamic* dynamicTest;

physx::PxPhysics* PhysicsManager::gPhysics = nullptr;
physx::PxScene* PhysicsManager::gScene = nullptr;
physx::PxMaterial* PhysicsManager::gMaterial = nullptr;

bool PhysicsManager::preUpdate = true;
bool PhysicsManager::foundationCreated = false;
bool PhysicsManager::physicsCreated = false;
bool PhysicsManager::sceneCreated = false;

PhysicsContactListener PhysicsManager::listener;

Vector3 PhysicsManager::gravity(0, -9.81, 0);

std::vector<physx::PxRigidDynamic*> PhysicsManager::allDynamics;
std::vector<physx::PxRigidStatic*> PhysicsManager::allStatics;

void PhysicsManager::OnStartup()
{
	isUniversal = true;
}

void PhysicsManager::OnStart()
{
	std::cout << "Physics Manager Started!" << std::endl;
	if (PhysicsManager::sceneCreated == true && gScene != nullptr)
	{
		gScene->release();
	}

	if (!PhysicsManager::foundationCreated)
	{
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	}

	if (!PhysicsManager::physicsCreated)
	{
		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true);
	}

	physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
	gDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PhysicsManager::contactReportFilterShader;
	sceneDesc.simulationEventCallback = &PhysicsManager::listener;
	gScene = gPhysics->createScene(sceneDesc);

	gMaterial = PhysicsManager::gPhysics->createMaterial(0.5f, 0.5f, 0);

	PhysicsManager::physicsCreated = true;
	PhysicsManager::foundationCreated = true;
	PhysicsManager::sceneCreated = true;
	PhysicsManager::preUpdate = true;
}

void PhysicsManager::OnUpdate()
{
	gScene->simulate(Graphics::DeltaTime());
	gScene->fetchResults(true);

	if (preUpdate)
	{
		for (auto i : PhysicsManager::allDynamics)
		{
			gScene->addActor(*i);
		}

		for (auto i : PhysicsManager::allStatics)
		{
			gScene->addActor(*i);
		}

		std::cout << PhysicsManager::allDynamics.size() << std::endl;

		preUpdate = false;
	}
}

void PhysicsManager::OnDestroy()
{
	PhysicsManager::allDynamics.clear();
	std::vector<physx::PxRigidDynamic*>().swap(PhysicsManager::allDynamics);

	PhysicsManager::allStatics.clear();
	std::vector<physx::PxRigidStatic*>().swap(PhysicsManager::allStatics);
}

void PhysicsManager::SetGravity(Vector3 g)
{
	gravity = g;
	gScene->setGravity(physx::PxVec3(gravity.x, gravity.y, gravity.z));
}

physx::PxRigidStatic* PhysicsManager::CreateStaticBox(BoxCollider& collider, Vector3 position, Vector3 scale)
{
	return CreateStaticBox(collider, position, scale, false);
}

physx::PxRigidStatic* PhysicsManager::CreateStaticBox(BoxCollider& collider, Vector3 position, Vector3 scale, bool isTrigger)
{
	physx::PxShape* boxShape = PhysicsManager::gPhysics->createShape(physx::PxBoxGeometry(scale.x / 2, scale.y / 2, scale.z / 2), *PhysicsManager::gMaterial);

	physx::PxRigidStatic* boxStatic = physx::PxCreateStatic(*PhysicsManager::gPhysics,
		physx::PxTransform(physx::PxVec3(position.x, position.y, position.z)),
		*boxShape);

	PhysicsManager::allStatics.push_back(boxStatic);

	int size = PhysicsManager::allStatics.size() - 1;
	if (!preUpdate && Application::_engineState == Application::Game)
		gScene->addActor(*allStatics[size]);

	return boxStatic;
}

physx::PxRigidDynamic* PhysicsManager::CreateDynamicBox(BoxCollider& collider, Vector3 position, Vector3 scale)
{
	return CreateDynamicBox(collider, position, scale, false);
}

physx::PxRigidDynamic* PhysicsManager::CreateDynamicBox(BoxCollider& collider, Vector3 position, Vector3 scale, bool isTrigger)
{
	physx::PxShape* boxShape = PhysicsManager::gPhysics->createShape(physx::PxBoxGeometry(scale.x / 2, scale.y / 2, scale.z / 2), *PhysicsManager::gMaterial);
	collider.boxShape = boxShape;

	physx::PxRigidDynamic* boxDynamic = physx::PxCreateDynamic(*PhysicsManager::gPhysics,
		physx::PxTransform(physx::PxVec3(position.x, position.y, position.z)),
		*boxShape, 0);

	if(isTrigger)
	{
		boxDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
		boxShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		boxShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	PhysicsManager::allDynamics.push_back(boxDynamic);

	int size = PhysicsManager::allDynamics.size() - 1;
	if (!preUpdate && Application::_engineState == Application::Game)
		gScene->addActor(*allDynamics[size]);

	return boxDynamic;
}

bool PhysicsManager::Raycast(Vector3 origin, Vector3 direction, int maxDistance)
{
	physx::PxRaycastBuffer hit;
	physx::PxVec3 o(origin.x, origin.y, origin.z), d(direction.x, direction.y, direction.z);
	return PhysicsManager::gScene->raycast(o, d, maxDistance, hit);
}

bool PhysicsManager::Raycast(Vector3 origin, Vector3 direction, int maxDistance, RaycastBuffer& buff)
{
	const physx::PxU32 bufferSize = 256;
	physx::PxRaycastHit hitBuffer[bufferSize];
	physx::PxRaycastBuffer hit(hitBuffer, bufferSize);

	physx::PxVec3 o(origin.x, origin.y, origin.z), d(direction.x, direction.y, direction.z);
	bool ray = PhysicsManager::gScene->raycast(o, d, maxDistance, hit);

	buff.origin = origin;
	buff.end = Vector3(
		origin.x + direction.x * maxDistance,
		origin.y + direction.y * maxDistance,
		origin.z + direction.z * maxDistance);

	buff.direction = direction;

	buff.distance = maxDistance;
	
	if(ray)
	{
		for (physx::PxU32 i = 0; i < hit.nbTouches; i++)
		{
		    buff.hitScripts.push_back(reinterpret_cast<ScriptBehaviour*>(hit.touches[i].actor->userData));
		}
		return true;
	}

	return false;
}

bool PhysicsManager::ScreenCameraRaycast(Camera& cam, Vector2 point, int maxDistance, RaycastBuffer& buff)
{
	Vector3 direction = cam.GetDirectionFromScreen(point);
	return PhysicsManager::Raycast(cam.GetCurrentPosition(), direction, maxDistance, buff);
}

bool PhysicsManager::ScreenCameraRaycast(Camera& cam, Vector2 point, int maxDistance)
{
	Vector3 direction = cam.GetDirectionFromScreen(point);
	//std::cout << point.ToString() << " | " << direction.ToString() << " | " << cam.GetCurrentPosition().ToString() << std::endl;
	return PhysicsManager::Raycast(cam.GetCurrentPosition(), direction, maxDistance);
}

physx::PxFilterFlags PhysicsManager::contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
		| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return physx::PxFilterFlag::eDEFAULT;
}

void PhysicsContactListener::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	for (physx::PxU32 i = 0; i < nbPairs; i++)
	{
		const physx::PxContactPair& cp = pairs[i];

		ScriptBehaviour* b1 = reinterpret_cast<ScriptBehaviour*>(pairHeader.actors[0]->userData);
		ScriptBehaviour* b2 = reinterpret_cast<ScriptBehaviour*>(pairHeader.actors[1]->userData);

		if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (b1 != nullptr && b2 != nullptr)
			{
				b1->OnCollisionEnter(*b2);
				b2->OnCollisionEnter(*b1);
			}
		}
		else if(cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (b1 != nullptr && b2 != nullptr)
			{
				b1->OnCollisionExit(*b2);
				b2->OnCollisionExit(*b1);
			}
		}
	}
}

void PhysicsContactListener::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    for(physx::PxU32 i=0; i < count; i++)
    {
        // ignore pairs when shapes have been deleted
        if (pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
            physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
            continue;

        const physx::PxTriggerPair& cp = pairs[i];

		if (pairs[i].otherActor != nullptr && pairs[i].triggerActor != nullptr)
		{
			ScriptBehaviour* b1 = reinterpret_cast<ScriptBehaviour*>(pairs[i].otherActor->userData);
			ScriptBehaviour* b2 = reinterpret_cast<ScriptBehaviour*>(pairs[i].triggerActor->userData);

			//std::cout << "Trigger Touch!" << std::endl;

			if (b1 != nullptr && b2 != nullptr)
			{
				if(!b2->_isTriggerEnter)
				{
					b1->OnTriggerEnter(*b2);
					b2->OnTriggerEnter(*b1);
				}
				else
				{
					b1->OnTriggerExit(*b2);
					b2->OnTriggerExit(*b1);
				}
			}
		}
		//else
		//	std::cout << "Trigger Touch but Actor is nullptr!" << std::endl;
    }
}