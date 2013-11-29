#ifndef NODE_H_
#define NODE_H_

#include "Spatial.h"

typedef std::list<Spatial*> SpatialList;

class Node: public Spatial
{
public:
	typedef SpatialList::const_iterator const_iterator;

	Node();
	virtual ~Node();

	const_iterator begin() const { return mChildren.begin(); }
	const_iterator end() const { return mChildren.end(); }

	void AttachChild(Spatial* child);
	void DetachChildren(Spatial* child);

	virtual void OnDraw(Renderer& renderer) const;

	virtual void OnUpdate(Real delta);

private:
	SpatialList mChildren;
};

#endif /* NODE_H_ */
