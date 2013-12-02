#include "Material.h"

ResourceManager<MaterialResource, MaterialLoader> MaterialManager;

Material::Material() :
		mAmbient(0.1f, 0.1f, 0.1f), mDiffuse(1.0f, 1.0f, 1.0f), mSpecular(0.9f, 0.9f, 0.9f), mTexture(0), mShininess(0.0f)
{
}

Material::~Material()
{
}
