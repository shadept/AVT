#ifndef LIGHT_H
#define LIGHT_T

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

	Light(Type type);
	~Light();

// private:
public:
	Type mType;
	Vector3 mAmbientColor;
	Vector3 mDiffuseColor;
	Vector3 mSpecularColor; // uma beka dumb, normalmente tem a cor da luz +- cor do material
	// float mConstanceAtt;
	float mLinearAtt;
	float mQuadraticAtt;
	float mCutoffAngle;
};

#endif // LIGHT_H