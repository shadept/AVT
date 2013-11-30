#ifndef MATERIAL_H_
#define MATERIAL_H_

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
	float mShininess;
};

#include "Manager.h"

struct MaterialResource : public Resource<Material>
{
public:
	MaterialResource(Handle handle, const std::string& filename) : Resource(handle, filename) {};

	friend struct MaterialLoader;
};

struct MaterialLoader
{
	static bool Load(MaterialResource** resource, Handle handle, const std::string& filename)
	{
		*resource = new MaterialResource(handle, filename);
		(*resource)->mRaw = new Material();

		return true;
	}
};

extern ResourceManager<MaterialResource, MaterialLoader> MaterialManager;

#endif /* MATERIAL_H_ */
