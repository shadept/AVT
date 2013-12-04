#include "Node.h"

#include "Logger.h"

Node::Node(const std::string& name) :
	Spatial(name)
{
}

Node::~Node()
{
}

Spatial* Node::Search(const std::string& path) const
{
	Logger::Debug << "Searching for " << path << Logger::endl;
	auto p = path.find('/');

	// did not find a slash, then search this level
	if (p == std::string::npos)
		return Find(path);

	std::string name = path.substr(0, p);
	Node* sp = dynamic_cast<Node*>(Find(name));
	if (sp)
	{
		return sp->Search(path.substr(p+1));
	}

	Logger::Error << "Cannot find spatial " << name << " in " << path << Logger::endl;
	return nullptr;
}

Spatial* Node::Find(const std::string& name) const
{
	for(auto* sp : mChildren)
		if (sp->GetName() == name)
			return sp;

	Logger::Error << "Cannot find spatial " << name << " in " << GetName() << Logger::endl;
	return nullptr;
}


void Node::AttachChild(Spatial* child, bool worldToLocal)
{
	if (child->GetParent() != nullptr)
		child->GetParent()->DetachChildren(child);
	mChildren.push_back(child);
	child->SetParent(this);

	// move child from world space to local space
	// TODO test me
	if (worldToLocal)
	{
		child->LocalTransform = this->WorldTransform.Inverse()  * child->LocalTransform;
	}
}

void Node::DetachChildren(Spatial* child, bool localToWorld)
{
	// move child to world coords
	child->UpdateTransformation();
	if (localToWorld)
	{
		child->LocalTransform = child->WorldTransform;
	}

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
