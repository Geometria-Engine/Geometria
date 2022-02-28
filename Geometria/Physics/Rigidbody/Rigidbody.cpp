#include "Rigidbody.h"
#include "../../Editor/Editor.h"
#include "../Colliders/BoxCollider.h"

VisualAccess(Rigidbody);

void Rigidbody::OnStart()
{
	RequireStaticScript(PhysicsManager);
}

void Rigidbody::OnUpdate()
{
	if (boxC != nullptr)
	{
		if (boxC->boxDynamic != nullptr)
		{
			if (!forceChange)
			{
				physx::PxTransform t = boxC->boxDynamic->getGlobalPose();
				physx::PxVec3 pos = t.p;

				if (freezePositionX)
				{
					boxC->boxDynamic->setGlobalPose(physx::PxTransform(GetTransform().position.x, pos.y, pos.z));
					pos = boxC->boxDynamic->getGlobalPose().p;
				}

				if (freezePositionZ)
				{
					boxC->boxDynamic->setGlobalPose(physx::PxTransform(pos.x, pos.y, GetTransform().position.z));
					pos = boxC->boxDynamic->getGlobalPose().p;
				}

				GetTransform().position = Vector3(pos.x, pos.y, pos.z);
				forcedTransform.position = GetTransform().position;
			}
			else
			{
				GetTransform().position = forcedTransform.position;
				boxC->boxDynamic->setGlobalPose(physx::PxTransform(forcedTransform.position.x, forcedTransform.position.y, forcedTransform.position.z));
				forceChange = false;
			}
		}
	}
	else
	{
		boxC = GetScript<BoxCollider>();
	}
}

void Rigidbody::OnInspector()
{
	VisualAccess_Title(Rigidbody);
}

void Rigidbody::OnSave()
{
	SaveInclude(Rigidbody, "Physics/Rigidbody/Rigidbody.h");
	SaveNewScript(Rigidbody);

	SaveEnd();
}

void Rigidbody::SetVelocity(Vector3 add)
{
	if (boxC != nullptr)
	{
		boxC->boxDynamic->setLinearVelocity(physx::PxVec3(add.x, add.y, add.z));
	}
}
