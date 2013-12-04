#include "Light.h"

Light::Light(Type type) :
	Spatial("Light")
{

}

Light::~Light()
{

}

void Light::OnDraw(Renderer& renderer) const
{
	renderer.Draw(this);
}