#include "Renderer.h"

#include <iostream>

#include "Camera.h"
#include "Node.h"
#include "Geometry.h"
#include "Material.h"
#include "Light.h"

std::string readFile(const std::string& filename);

Renderer::Renderer() :
		mShader(nullptr), mPickingShader(0), mCamera(nullptr), mLight(nullptr)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	mPicking = false;
	mLighting = true;
	mNeedLightUpdate = true;

	// load default materials
	MaterialManager.Load("default_material", "./materials/default.mtl");
	mDefaultMaterial = MaterialManager["default"]->GetRaw();

	Handle debugNormals = ShaderManager.Load("_normals", "./shaders/debug.vert", "./shaders/debug.frag");
	Handle debugDepth = ShaderManager.Load("_depth", "./shaders/simple.vert", "./shaders/depth.frag");
	Handle noLight = ShaderManager.Load("noligth", "./shaders/simple.vert", "./shaders/simple.frag");
	Handle realistic = ShaderManager.Load("realistic", "./shaders/simple.vert", "./shaders/realistic.frag");
	Handle specular = ShaderManager.Load("specular", "./shaders/simple.vert", "./shaders/specular.frag");
	Handle skybox = ShaderManager.Load("skybox", "./shaders/skybox.vert", "./shaders/skybox.frag");

	mDebugShaderNormals = ShaderManager[debugNormals]->GetRaw();
	mDebugShaderDepth = ShaderManager[debugDepth]->GetRaw();
	mNoLightShader = ShaderManager[noLight]->GetRaw();
	mRealisticShader = ShaderManager[realistic]->GetRaw();
	mSpecularShader = ShaderManager[specular]->GetRaw();

	mDebugging = false;
	mShader = mRealisticShader;

	std::vector<unsigned char> white(4);
	white[0] = white[1] = white[2] = white[3] = 0xFF;
	mWhiteTexture = new Texture();
	mWhiteTexture->Load(white, 1, 1);

	mSkybox = new Cubemap();
	mSkybox->Load("./textures/skybox");
	Logger::Debug << "Loaded cube map" << Logger::endl;

	mCubemap = new Cubemap();
	mCubemap->Load(1024,1024);
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

void Renderer::BuildEnvironmentMap(Node* scene, const Vector3& center)
{
	assert(scene != NULL && "cannot built environment map from null scene");

	Camera camera = *mCamera;
	mCamera->UpdateTransformation();
	mCamera->SetPerspective(90.0f, 1.0f, 1.0f, 500.0f);
	mCamera->SetViewport(1024, 1024);

	GLuint framebuffer = 0, depthbuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, mCubemap->ID(), 0);

	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE && "failed to create framebuffer");

	static Vector3 lookAt[] = { {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };
	static Vector3 up[] = { {0, -1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}, {0, -1, 0}, {0, -1, 0} };

	bool lighting = mLighting;
	SetLighting(false);
	for (int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mCubemap->ID(), 0);
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(status == GL_FRAMEBUFFER_COMPLETE && "failed to create framebuffer");
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		mCamera->SetLookAt(center, center + lookAt[i], up[i]);
		scene->Draw(*this);
		mTransparentList.clear();
	}
	SetLighting(lighting);

	*mCamera = camera;

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteRenderbuffers(1, &depthbuffer);
	glDeleteFramebuffers(1, &framebuffer);

	mCamera->SetViewport(mCamera->GetWidth(), mCamera->GetHeight());
	glClear(GL_DEPTH_BUFFER_BIT);
}

////////////////////////// DRAWING SCENE //////////////////////////////

void Renderer::SetDebug(DebugShader type)
{
	mDebugging = (type == 0 ? false : true);
	switch (type)
	{
	case 0:
		mShader = (mLighting ? mRealisticShader : mNoLightShader);
		mNeedLightUpdate = true;
		break;
	case 1:
		mShader = mDebugShaderNormals;
		break;
	case 2:
		mShader = mDebugShaderDepth;
		break;
	default:
		break;
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
	if (scene)
	{
		//TODO create list of object to draw, and sort them from back to front
		scene->Draw(*this);

		if (mTransparentList.size() > 0 && !mDebugging)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			for (auto* geo : mTransparentList)
				Draw(geo, true);

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
//		Logger::Debug << "Transparent material, saving for later" << Logger::endl;
		mTransparentList.push_back(geometry);
		return;
	}

	const Program* shader = (geometry->GetShader() != NULL ? geometry->GetShader() : mShader);
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
		if (shader == NULL)
			Logger::Fatal << "Cannot bind NULL shader" << Logger::endl;

		shader->Use();
//		Uniform::Bind(*shader, "CameraPosition", mCamera->WorldTransform.Orientation() * mCamera->WorldTransform.Position());
		Uniform::Bind(*shader, "NormalMatrix", NormalMatrix);
		Uniform::Bind(*shader, "ModelMatrix", ModelMatrix);
		Uniform::Bind(*shader, "ViewMatrix", ViewMatrix);
		Uniform::Bind(*shader, "ProjectionMatrix", ProjectionMatrix);
		Uniform::Bind(*shader, "ModelViewMatrix", ModelViewMatrix);
		Uniform::Bind(*shader, "ModelViewProjectionMatrix", ModelViewProjectionMatrix);
		mCubemap->Bind(4);
		mSkybox->Bind(5);
		Uniform::Bind(*shader, "environment.map", 4); // GL_TEXTURE4
		Uniform::Bind(*shader, "environment.skybox", 5); // GL_TEXTURE5
		checkOpenGLError("Failed to bind matrixes");

		Bind(geometry->GetMaterial(), shader);

		if (mNeedLightUpdate)
		{
			// Logger::Debug << "Updating Lighting" << Logger::endl;
			Bind(mLight, shader);
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
	if (mLighting == false)
	{
		return;
	}

	assert(light != NULL && "cannot bind NULL light");

	light->UpdateTransformation();
	const Vector3& position = light->LocalTransform.Position();
	Uniform::Bind(*shader, "lightSource.position", Vector4(position.X, position.Y, position.Z,
			(light->mType == Light::Type::DIRECTIONAL ? 0.0f : 1.0f)));
	Uniform::Bind(*shader, "lightSource.ambient", light->mAmbientColor);
	Uniform::Bind(*shader, "lightSource.diffuse", light->mDiffuseColor);
	Uniform::Bind(*shader, "lightSource.specular", light->mSpecularColor);
	Uniform::Bind(*shader, "lightSource.spotDirection", light->mDirection);
	Uniform::Bind(*shader, "lightSource.spotExponent", light->mSpotExponent);
	Uniform::Bind(*shader, "lightSource.spotCutoff", light->mCutoffAngle);
	Uniform::Bind(*shader, "lightSource.constantAttenuation", 1);
	Uniform::Bind(*shader, "lightSource.linearAttenuation", light->mLinearAtt);
	Uniform::Bind(*shader, "lightSource.quadraticAttenuation", light->mQuadraticAtt);
}

void Renderer::Bind(const Material* material, const Program* shader)
{
	if (material->mAmbientMap != NULL)
		material->mAmbientMap->Bind(0);
	else
		mWhiteTexture->Bind(0);
	if (material->mDiffuseMap != NULL)
		material->mDiffuseMap->Bind(1);
	else
		mWhiteTexture->Bind(1);
	if (material->mSpecularMap != NULL)
		material->mSpecularMap->Bind(2);
	else
		mWhiteTexture->Bind(2);
	if (material->mBumpMap != NULL)
		material->mBumpMap->Bind(3);
	else
		mWhiteTexture->Bind(3);

	Uniform::Bind(*shader, "material.ambientTexture", 0); // GL_TEXTURE0
	Uniform::Bind(*shader, "material.diffuseTexture", 1); // GL_TEXTURE1
	Uniform::Bind(*shader, "material.specularTexture", 2); // GL_TEXTURE2
	Uniform::Bind(*shader, "material.normalTexture", 3); // GL_TEXTURE3

	Uniform::Bind(*shader, "material.ambient", material->mAmbient);
	Uniform::Bind(*shader, "material.diffuse", material->mDiffuse);
	Uniform::Bind(*shader, "material.specular", material->mSpecular);
//	Uniform::Bind(*shader, "MaterialShininess",  material->mShininess * 128.0f);
	Uniform::Bind(*shader, "material.shininess", material->mShininess);
	Uniform::Bind(*shader, "material.transparency", material->mTransparency);
	Uniform::Bind(*shader, "material.refractionIndex", material->mRefraction);
	Uniform::Bind(*shader, "material.reflectivity", material->mReflectivity);

	checkOpenGLError("Failed to bind material");

//	glActiveTexture(GL_TEXTURE0);
//	glDisable(GL_TEXTURE_2D);
//	glActiveTexture(GL_TEXTURE1);
//	glDisable(GL_TEXTURE_2D);
//	glActiveTexture(GL_TEXTURE2);
//	glDisable(GL_TEXTURE_2D);
//	glActiveTexture(GL_TEXTURE3);
//	glDisable(GL_TEXTURE_2D);
}
