#include "Material.h"

Color::Color(float r, float b, float g)
{
	R = r;
	G = g;
	B = b;
}

Material::Material() :
		mAmbient(0.1f, 0.1f, 0.1f), mDiffuse(1.0f, 1.0f, 1.0f), mSpecular(0.9f, 0.9f, 0.9f)
{
}

Material::~Material()
{
}
