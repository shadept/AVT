#include "Geometry.h"

Geometry::Geometry(const Mesh* mesh) :
		mMesh(mesh)
{
}

void Geometry::OnDraw(Renderer& renderer) const
{
	renderer.Draw(this);
}
