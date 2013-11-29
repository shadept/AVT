#include "Node.h"


Node::Node()
{
}

Node::~Node()
{
}

void Node::AttachChild(Spatial* child)
{
	if (child->GetParent() != nullptr)
		child->GetParent()->DetachChildren(child);
	mChildren.push_back(child);
	child->SetParent(this);

	// move child from world space to local space
	// TODO inverse transformations?
}

void Node::DetachChildren(Spatial* child)
{
	// move child to world coords
	child->UpdateTransformation();
	child->LocalTransform = child->WorldTransform;

	mChildren.remove(child);
	child->SetParent(nullptr);
}

void Node::OnDraw(Renderer& renderer) const
{
	for (auto sp : mChildren)
		sp->OnDraw(renderer);
}

void Node::OnUpdate(Real delta)
{
	for (auto sp: mChildren)
		sp->OnUpdate(delta);
}
