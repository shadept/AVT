#ifndef NODE_H_
#define NODE_H_

#include "Spatial.h"

typedef std::list<Spatial*> SpatialList;

class Node: public Spatial
{
public:
	typedef SpatialList::const_iterator const_iterator;

	Node(const std::string& name);
	virtual ~Node();

	const_iterator begin() const { return mChildren.begin(); }
	const_iterator end() const { return mChildren.end(); }

	Spatial* Search(const std::string& path) const;
	Spatial* Find(const std::string& name) const;

	void AttachChild(Spatial* child, bool worldToLocal = false);
	void DetachChildren(Spatial* child, bool localToWorld = false);

	virtual void OnDraw(Renderer& renderer) const;

	virtual void OnUpdate(Real delta);

private:
	SpatialList mChildren;
};

#endif /* NODE_H_ */
