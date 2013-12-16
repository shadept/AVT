#ifndef LIGHT_H
#define LIGHT_H

#include "Material.h"
#include "Spatial.h"
#include "Vector3.h"

class Light : public Spatial
{
public:
	
	enum class Type
	{
		DIRECTIONAL,
		POINTLIGHT,
		SPOTLGHT
	};

	Light(Type type = Type::POINTLIGHT);
	~Light();

	virtual void OnDraw(Renderer& renderer) const;

// private:
public:
	Type mType;
	Vector3 mAmbientColor;
	Vector3 mDiffuseColor;
	Vector3 mSpecularColor; // uma beka dumb, normalmente tem a cor da luz +- cor do material
	Vector3 mDirection;
	// float mConstanceAtt;
	float mLinearAtt;
	float mQuadraticAtt;
	float mCutoffAngle;
	float mSpotExponent;

private:
	Geometry* mDebugSphere;
	Material* mDebugMaterial;
};

#endif // LIGHT_H
