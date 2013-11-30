#include "Geometry.h"

Geometry::Geometry(const Mesh* mesh, const Material* material) :
		mMesh(mesh), mMaterial(material)
{
}

void Geometry::OnDraw(Renderer& renderer) const
{
	renderer.Draw(this);
}
