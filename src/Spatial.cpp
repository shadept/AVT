#include "Spatial.h"

#include "Node.h"
#include <iostream>

IdGenerator Spatial::msIdGenerator;

Spatial::Spatial() :
		mId(msIdGenerator.Generate()), mParent(nullptr)
{
}

Spatial::~Spatial()
{
}

void Spatial::Draw(Renderer& renderer) const
{
	// do culling here
	OnDraw(renderer);
}

void Spatial::OnUpdate(Real delta)
{
}

Identifier Spatial::GetId() const
{
	return mId;
}

Node* Spatial::GetParent() const
{
	return mParent;
}

void Spatial::SetParent(Node* parent)
{
	assert((parent != nullptr || parent == nullptr) && "Should detach this spatial from it's parent before assigning a new parent");
	mParent = parent;
}

void Spatial::UpdateTransformation() const
{
	if (LocalTransform.IsDirty())
	{
		if (mParent)
		{
			mParent->UpdateTransformation();
			WorldTransform = mParent->WorldTransform * LocalTransform;
		}
		else
		{
			WorldTransform = LocalTransform;
		}
	}
}
