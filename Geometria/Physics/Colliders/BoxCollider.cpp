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

void BoxCollider::OnSave()
{
	SaveInclude(BoxCollider, "Physics/Colliders/BoxCollider.h");
	SaveNewScript(BoxCollider);

	SaveEnd();
}

void BoxCollider::OnUpdate()
{

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
