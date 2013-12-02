#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Texture.h"
#include "Vector3.h"

class Material
{
public:
	Material();
	virtual ~Material();

public:
	Vector3 mAmbient;
	Vector3 mDiffuse;
	Vector3 mSpecular;
	Texture* mTexture;
	float mShininess;
};

#include "Manager.h"

DECLARE_RESOURCE_AND_LOADER(Material);
DECLARE_MANAGER(Material);

#endif /* MATERIAL_H_ */
