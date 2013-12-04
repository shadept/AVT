#include "Renderer.h"

#include <iostream>

#include "Camera.h"
#include "Node.h"
#include "Geometry.h"
#include "Light.h"

std::string readFile(const std::string& filename);

Renderer::Renderer() :
		mShader(nullptr), mPickingShader(0), mCamera(nullptr), mLight(nullptr)
{
	_frameCounter = 0;
	mPicking = false;
	mLighting = true;
	mNeedLightUpdate = true;

	mDefaultMaterial = new Material();
	mDefaultMaterial->mAmbient = Vector3(1.0f, 0.0f, 1.0f);
	mDefaultMaterial->mDiffuse = Vector3(1.0f, 0.0f, 1.0f);
	mDefaultMaterial->mSpecular = Vector3(1.0f, 0.0f, 1.0f);
	mDefaultMaterial->mShininess = 0.0f;

	MaterialManager.Add("default", mDefaultMaterial);

	Handle debug = ShaderManager.Load("debug", "./shaders/debug.vert", "./shaders/debug.frag");
	Handle noLight = ShaderManager.Load("noligth", "./shaders/simple.vert", "./shaders/simple.frag");
	Handle realistic = ShaderManager.Load("realistic", "./shaders/simple.vert", "./shaders/realistic.frag");
	Handle specular = ShaderManager.Load("specular", "./shaders/simple.vert", "./shaders/specular.frag");

	mDebugShader = ShaderManager[debug]->GetRaw();
	mNoLightShader = ShaderManager[noLight]->GetRaw();
	mRealisticShader = ShaderManager[realistic]->GetRaw();
	mSpecularShader = ShaderManager[specular]->GetRaw();


	mDebugging = false;
	mShader = mRealisticShader;
}

Renderer::~Renderer()
{
}

int Renderer::Pick(Node* node, int x, int y)
{
	mPicking = true;
	if (!mPickingShader)
	{
		VertexShader vs;
		vs.Source(readFile("shaders/debug.vert"));
		vs.Compile();

		FragmentShader fs;
		fs.Source(readFile("shaders/picking.frag"));
		fs.Compile();

		mPickingShader = new Program();
		mPickingShader->AttachShader(vs).AttachShader(fs);
		mPickingShader->BindAttribute(VertexAttributes::POSITION, "in_Position");
		mPickingShader->Link();
	}

//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawScene(node);

	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLubyte data[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	mPicking = false;
	return data[0] + data[1] * 256 + data[2] * 256 * 256;
}

Vector3 Renderer::Unproject(int x, int y)
{
	assert(mCamera && "Renderer must have a camera attached");
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	mCamera->UpdateTransformation();
	Matrix4 mv = mCamera->WorldTransform;
	mv.transpose();
	Matrix4 p = mCamera->GetProjection().transposed();
	for (int i = 0; i < 16; i++)
	{
		modelview[i] = mv[i];
		projection[i] = p[i];
	}
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float) x;
	winY = (float) y;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
//	std::cout << "ZBuffer value: " << winZ << std::endl;
	checkOpenGLError("Failed to readpixels");

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return Vector3(posX, posY, posZ);
}

////////////////////////// DRAWING SCENE //////////////////////////////

void Renderer::SetDebug(bool on)
{
	mDebugging = on;
	if (on)
	{
		mShader = mDebugShader;
	}
	else
	{
		mShader = (mLighting ? mRealisticShader : mNoLightShader);
		mNeedLightUpdate = true;
	}
}

void Renderer::SetCamera(Camera* camera)
{
	mCamera = camera;
}

void Renderer::SetGlobalShader(const Program* shader)
{
	shader->Use();
	mShader = shader;
}

void Renderer::SetLighting(bool on)
{
	mLighting = on;
	mNeedLightUpdate = true;
	if (!mDebugging)
	{
		if (!on)
			mShader = mNoLightShader;
		else
			mShader = mRealisticShader;
	}
}

void Renderer::DrawScene(Node* scene)
{
	assert(mCamera && "Renderer must have a camera attached");
	if (scene) {
		// mLight = nullptr;
		//TODO create list of object to draw, and sort them from back to front
		scene->Draw(*this);

		if (mTransparentList.size() > 0 && !mDebugging)
		{
			bool lighting = mLighting;
			SetLighting(false);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Logger::Debug << "Drawing transparent objects" << Logger::endl;
			// TODO order from back to front

			// fill depth buffer
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			for (auto* geo : mTransparentList)
				Draw(geo, true);

			// with the depth buffer filled, polygon sorting is free
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			for (auto* geo : mTransparentList)
				Draw(geo, true);

			SetLighting(lighting);

			// TODO specular hightlight
			glDepthFunc(GL_EQUAL); // TODO move to context
			for (auto* geo : mTransparentList)
			{
				const Material* mat = geo->GetMaterial();
				Material specularMat = *mat;
				specularMat.mAmbient = {};
				specularMat.mDiffuse = {};
				geo->SetMaterial(&specularMat);
				Draw(geo, true);
				geo->SetMaterial(mat);
			}

			glDepthFunc(GL_LEQUAL);
			glDisable(GL_BLEND);
		}
		mTransparentList.clear();
	}
}

void Renderer::Draw(Geometry* geometry, bool overrideTransparency)
{
	assert(mCamera && "Renderer must have a camera attached");
	checkOpenGLError("Failed somewhere else");

	const Material* material = geometry->GetMaterial() ? geometry->GetMaterial() : mDefaultMaterial;
	if (material->mTransparency < 1.0f && overrideTransparency == false && !mDebugging)
	{
		// Logger::Debug << "Transparent material, saving for later" << Logger::endl;
		mTransparentList.push_back(geometry);
		return;
	}

	geometry->GetMesh()->Bind();
	checkOpenGLError("Failed to bind mesh");

	geometry->UpdateTransformation();
	mCamera->UpdateTransformation();

	Matrix4 ModelMatrix = geometry->WorldTransform;
	Matrix4 ViewMatrix = mCamera->WorldTransform;
	Matrix4 ProjectionMatrix = mCamera->GetProjection();
	Matrix4 ModelViewMatrix = ViewMatrix * ModelMatrix;
	Matrix4 NormalMatrix = ModelViewMatrix.inverted().transpose();
	Matrix4 ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;

	if (mPicking)
	{
		int r = (geometry->GetId() & 0x000000FF) >> 0;
		int g = (geometry->GetId() & 0x0000FF00) >> 8;
		int b = (geometry->GetId() & 0x00FF0000) >> 16;
		mPickingShader->Use();
		Uniform::Bind(*mPickingShader, "ModelViewProjection", ModelViewProjectionMatrix);
		Uniform::Bind(*mPickingShader, "PickingColor", Vector3(r / 255.0f, g / 255.0f, b / 255.0f));
	}
	else
	{
		if (mShader == NULL)
			Logger::Fatal << "Cannot bind NULL shader" << Logger::endl;

		mShader->Use();
		Uniform::Bind(*mShader, "NormalMatrix", NormalMatrix);
		Uniform::Bind(*mShader, "ModelMatrix", ModelMatrix);
		Uniform::Bind(*mShader, "ViewMatrix", ViewMatrix);
		Uniform::Bind(*mShader, "ProjectionMatrix", ProjectionMatrix);
		Uniform::Bind(*mShader, "ModelViewMatrix", ModelViewMatrix);
		Uniform::Bind(*mShader, "ModelViewProjectionMatrix", ModelViewProjectionMatrix);
		checkOpenGLError("Failed to bind matrixes");

		Bind(geometry->GetMaterial(), mShader);

		if (mNeedLightUpdate)
		{
			// Logger::Debug << "Updating Lighting" << Logger::endl;
			Bind(mLight, mShader);
			mNeedLightUpdate = false;
		}
	}

	glDrawArrays(GL_TRIANGLES, 0, geometry->GetMesh()->GetCount());
	checkOpenGLError("Failed to draw");

	geometry->GetMesh()->Unbind();
	checkOpenGLError("Failed to unbind data");
}

void Renderer::Draw(const Light* light)
{
	mLight = light;
	mNeedLightUpdate = true;
}

void Renderer::Bind(const Light* light, const Program* shader)
{
	if (mLighting == false) {
		return;
	}

	assert(light != NULL && "cannot bind NULL light");

	light->UpdateTransformation();
	const Vector3& position = light->LocalTransform.Position();
	Uniform::Bind(*shader, "LightPosition", position);
	Uniform::Bind(*shader, "LightAmbientColor", light->mAmbientColor);
	Uniform::Bind(*shader, "LightDiffuseColor", light->mDiffuseColor);
	Uniform::Bind(*shader, "LightSpecularColor", light->mSpecularColor);
}

void Renderer::Bind(const Material* material, const Program* shader)
{
	if (material->mTexture != NULL)
	{
		material->mTexture->Bind();
		Uniform::Bind(*shader, "MaterialHasTexture", GL_TRUE);
	}
	else
	{
		Uniform::Bind(*shader, "MaterialHasTexture", GL_FALSE);
	}
	Uniform::Bind(*shader, "MaterialTexture", 0); // GL_TEXTURE0
	Uniform::Bind(*shader, "MaterialAmbientColor", material->mAmbient);
	Uniform::Bind(*shader, "MaterialDiffuseColor",  material->mDiffuse);
	Uniform::Bind(*shader, "MaterialSpecularColor",  material->mSpecular);
	Uniform::Bind(*shader, "MaterialShininess",  material->mShininess * 128.0f);
	Uniform::Bind(*shader, "MaterialTransparency", material->mTransparency);
	checkOpenGLError("Failed to bind material");
}