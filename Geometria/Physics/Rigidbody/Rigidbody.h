#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

#ifndef PHYSICSMANAGER_H
#include "../PhysicsManager.h"
#endif

#ifndef BOXCOLLIDER_H
#include "../Colliders/BoxCollider.h"
#endif

struct Rigidbody : public ScriptBehaviour
{
	AllowCloning(Rigidbody);

	void OnStart();
	void OnUpdate();
	void OnInspector();

	void OnSave();

	bool forceChange = false;
	Transform forcedTransform;

	BoxCollider* boxC;

	bool freezePositionX = false;
	bool freezePositionZ = false;

	Transform& GetRigidbodyTransform()
	{
		forceChange = true;
		return forcedTransform;
	}

	void SetVelocity(Vector3 add);
};
