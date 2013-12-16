#include "Light.h"

#include "Geometry.h"
#include "Material.h"

Light::Light(Type type) :
	Spatial("Light"), mType(type)
{
	mLinearAtt = 0.1f;
	mQuadraticAtt = 0.0f;
	mCutoffAngle = 30.0f;
	mSpotExponent = 1.0f;
	MeshManager.Load("sphere", "./models/sphere.obj");
	mDebugSphere = new Geometry("LightDebug");
	mDebugMaterial = new Material();
	mDebugSphere->SetMesh(MeshManager["sphere"]->GetRaw());
	mDebugSphere->SetMaterial(mDebugMaterial);
	mDebugSphere->SetShader(ShaderManager["nolight"]->GetRaw());
}

Light::~Light()
{
	delete mDebugMaterial;
	delete mDebugSphere;

	mDebugMaterial = nullptr;
	mDebugSphere = nullptr;
}

void Light::OnDraw(Renderer& renderer) const
{
	// Debug stuff
//	mDebugSphere->WorldTransform = this->WorldTransform;
//	mDebugSphere->LocalTransform = this->LocalTransform;
//	mDebugSphere->LocalTransform.SetScale({0.1f, 0.1f, 0.1f});
//
//	mDebugMaterial->mAmbient = this->mAmbientColor;
//	mDebugMaterial->mDiffuse = this->mDiffuseColor;
//	mDebugMaterial->mSpecular = this->mSpecularColor;
//	renderer.Draw(mDebugSphere);
}
