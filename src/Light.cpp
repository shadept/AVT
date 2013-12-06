#include "Light.h"

Light::Light(Type type) :
	Spatial("Light"), mType(type)
{
	mLinearAtt = 0.0f;
	mQuadraticAtt = 0.0f;
	mCutoffAngle = 30.0f;
}

Light::~Light()
{

}

void Light::OnDraw(Renderer& renderer) const
{
	renderer.Draw(this);
}
