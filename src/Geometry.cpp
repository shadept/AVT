#include "Geometry.h"

Geometry::Geometry(const std::string& name, const Mesh* mesh, const Material* material) :
		Spatial(name), mMesh(mesh), mMaterial(material)
{
}

void Geometry::OnDraw(Renderer& renderer) const
{
	renderer.Draw((Geometry*)this);
}
