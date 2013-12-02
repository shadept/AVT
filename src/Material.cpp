#include "Material.h"

IMPLEMENT_MANAGER(Material);

Material::Material() :
		mAmbient(0.1f, 0.1f, 0.1f), mDiffuse(1.0f, 1.0f, 1.0f), mSpecular(0.9f, 0.9f, 0.9f), mTexture(0), mShininess(0.0f)
{
}

Material::~Material()
{
}

bool MaterialLoader::Load(MaterialResource** resource, Handle handle, const std::string& filename)
{
	*resource = new MaterialResource(handle, filename);
	(*resource)->mRaw = new Material();

	return true;
}
