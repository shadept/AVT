#ifndef SPATIAL_H_
#define SPATIAL_H_

#include "Common.h"
#include "Renderer.h"
#include "Transformation.h"

// Forward Declaration
class Node;

typedef unsigned long long int Identifier;
class IdGenerator
{
public:
	IdGenerator() : mNext(0) {}

	Identifier Generate()
	{
		return mNext++;
	}

private:
	Identifier mNext;
};

// Basic scene object
// Has a position in the world
class Spatial
{
public:
	virtual ~Spatial();

	void Draw(Renderer& renderer) const;
	virtual void OnDraw(Renderer& renderer) const = 0;

	virtual void OnUpdate(Real delta);

	void Enabled(bool b);
	bool IsEnabled() const;

	Identifier GetId() const;
	const std::string& GetName() const {return mName;}
	Node* GetParent() const;
	void SetParent(Node* parent);

	void UpdateTransformation() const;

	Transformation LocalTransform;
	mutable Transformation WorldTransform;

protected:
	// Abstract
	Spatial(const std::string& name);

	bool mEnabled;
	Identifier mId;
	std::string mName;
	Node* mParent;

	static IdGenerator msIdGenerator;
};

#endif /* SPATIAL_H_ */
