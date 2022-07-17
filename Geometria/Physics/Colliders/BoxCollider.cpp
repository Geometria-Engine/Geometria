#include "BoxCollider.h"
#include "../Rigidbody/Rigidbody.h"
#include "../../Editor/Editor.h"

VisualAccess(BoxCollider);

void BoxCollider::OnStart()
{
	RequireStaticScript(PhysicsManager);

	if (GetScript<Rigidbody>() == nullptr)
	{
		if (boxStatic == nullptr)
		{
			boxStatic = PhysicsManager::CreateStaticBox(*this, GetTransform().position, GetTransform().scale * size);
			boxStatic->userData = this;
		}

		GetTransform().position.AddEventFunction([this]{ OnTransformChange(0); });
		GetTransform().rotation.AddEventFunction([this]{ OnTransformChange(1); });
		GetTransform().scale.AddEventFunction([this]{ OnTransformChange(2); });
	}
	else
	{
		if (boxDynamic == nullptr)
		{
			boxDynamic = PhysicsManager::CreateDynamicBox(*this, GetTransform().position, GetTransform().scale * size);
			boxDynamic->userData = this;
			std::cout << "Dynamic Box Set!" << std::endl;
		}
	}
}

void BoxCollider::OnInspector()
{
	VisualAccess_Title(BoxCollider);
}

void BoxCollider::OnTransformChange(int value)
{
	switch(value)
	{
		case 0:
			if(boxStatic != nullptr)
			{
				boxStatic->setGlobalPose(physx::PxTransform(GetTransform().position.x, GetTransform().position.y, GetTransform().position.z));
			}
			break;

		case 1:
			if(boxStatic != nullptr)
			{
				physx::PxTransform t = boxStatic->getGlobalPose();
				t.rotate(physx::PxVec3(GetTransform().rotation.x, GetTransform().rotation.y, GetTransform().rotation.z));
			}
			break;

		case 2:
			SetScale(this->size);
			break;
	}
}

void BoxCollider::OnSave()
{
	SaveInclude(BoxCollider, "Physics/Colliders/BoxCollider.h");
	SaveNewScript(BoxCollider);

	SaveEnd();
}

void BoxCollider::OnUpdate()
{
	if(_setTriggerOnUpdate)
	{
		SetTrigger(_isTrigger);
		_setTriggerOnUpdate = false;
	}
}

void BoxCollider::OnDestroy()
{
	if(boxDynamic != nullptr)
	{
		boxDynamic->detachShape(*boxShape);
		boxDynamic->release();
		boxDynamic = nullptr;
	}

	if(boxStatic != nullptr)
	{
		boxStatic->detachShape(*boxShape);
		boxStatic->release();
		boxStatic = nullptr;
	}

	if (boxShape != nullptr)
	{
		boxShape->release();
		boxShape = nullptr;
	}

	//delete this;
}

void BoxCollider::SetScale(Vector3 size)
{
	this->size = size;

	if (boxDynamic != nullptr)
	{
		if (boxShape != nullptr)
		{
			boxDynamic->detachShape(*boxShape);
			boxShape->release();
			boxShape = nullptr;
		}

		boxShape = PhysicsManager::gPhysics->createShape(physx::PxBoxGeometry((GetTransform().scale.x * size.x) / 2,
			(GetTransform().scale.y * size.y) / 2,
			(GetTransform().scale.z * size.z) / 2),
			*PhysicsManager::gMaterial);

		boxDynamic->attachShape(*boxShape);
	}
	else if (boxStatic != nullptr)
	{
		if (boxShape != nullptr)
		{
			boxStatic->detachShape(*boxShape);
			boxShape->release();
			boxShape = nullptr;
		}

		boxShape = PhysicsManager::gPhysics->createShape(physx::PxBoxGeometry((GetTransform().scale.x * size.x) / 2,
			(GetTransform().scale.y * size.y) / 2,
			(GetTransform().scale.z * size.z) / 2),
			*PhysicsManager::gMaterial);

		boxStatic->attachShape(*boxShape);
	}
}

void BoxCollider::SetTrigger(bool t)
{
	_isTrigger = t;
	if(PhysicsManager::sceneCreated)
	{
		if(GetScript<Rigidbody>() != nullptr && boxDynamic != nullptr)
		{
			boxDynamic->detachShape(*boxShape);
			boxShape->release();
			boxShape = nullptr;

			boxDynamic->release();
			boxDynamic = nullptr;

			if(t)
			{
				boxDynamic = PhysicsManager::CreateDynamicBox(*this, GetTransform().position, GetTransform().scale * size, true);
				boxDynamic->userData = this;
			}
			else
			{
				boxDynamic = PhysicsManager::CreateDynamicBox(*this, GetTransform().position, GetTransform().scale * size, false);
				boxDynamic->userData = this;
			}
		}
	}
	else
		_setTriggerOnUpdate = true;
}