#include "Renderer.h"

#include <iostream>

#include "Camera.h"
#include "Node.h"
#include "Geometry.h"

std::string readFile(const std::string& filename);

Renderer::Renderer() :
		mShader(nullptr), mPickingShader(0), mCamera(nullptr)
{
	_frameCounter = 0;
	mPicking = false;

	mDefaultMaterial = new Material();
	mDefaultMaterial->mAmbient = Vector3(1.0f, 0.0f, 1.0f);
	mDefaultMaterial->mDiffuse = Vector3(1.0f, 0.0f, 1.0f);
	mDefaultMaterial->mSpecular = Vector3(1.0f, 0.0f, 1.0f);
	mDefaultMaterial->mShininess = 0.0f;

	MaterialManager.Add("default", mDefaultMaterial);
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

void Renderer::SetCamera(Camera* camera)
{
	mCamera = camera;
}

void Renderer::Bind(const Program* shader)
{
	shader->Use();
	mShader = shader;
}

void Renderer::DrawScene(Node* scene)
{
	assert(mCamera && "Renderer must have a camera attached");
	if (scene) scene->Draw(*this);
}

void Renderer::Draw(const Geometry* geometry)
{
	assert(mCamera && "Renderer must have a camera attached");
	checkOpenGLError("Failed somewhere else");

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
		mShader->Use();
		const Material* material = geometry->GetMaterial();
		if (material == NULL)
		{
			material = mDefaultMaterial;
		}
		if (material->mTexture != NULL)
		{
			material->mTexture->Bind();
			glUniform1i((*mShader)["MaterialHasTexture"], GL_TRUE);
		}
		else
		{
			glUniform1i((*mShader)["MaterialHasTexture"], GL_FALSE);
		}
		glUniform1i((*mShader)["MaterialTexture"], 0); // GL_TEXTURE0
		Uniform::Bind(*mShader, "MaterialAmbientColor", material->mAmbient);
		Uniform::Bind(*mShader, "MaterialDiffuseColor",  material->mDiffuse);
		Uniform::Bind(*mShader, "MaterialSpecularColor",  material->mSpecular);
		Uniform::Bind(*mShader, "MaterialShininess",  material->mShininess * 128.0f);
		checkOpenGLError("Failed to bind material");


//		checkOpenGLError("Failed to bind shader");
		Uniform::Bind(*mShader, "NormalMatrix", NormalMatrix);
		Uniform::Bind(*mShader, "ModelMatrix", ModelMatrix);
		Uniform::Bind(*mShader, "ViewMatrix", ViewMatrix);
		Uniform::Bind(*mShader, "ProjectionMatrix", ProjectionMatrix);
		Uniform::Bind(*mShader, "ModelViewMatrix", ModelViewMatrix);
		Uniform::Bind(*mShader, "ModelViewProjectionMatrix", ModelViewProjectionMatrix);
		checkOpenGLError("Failed to bind matrixes");
	}

	glDrawArrays(GL_TRIANGLES, 0, geometry->GetMesh()->GetCount());
	checkOpenGLError("Failed to draw");

	geometry->GetMesh()->Unbind();
	checkOpenGLError("Failed to unbind data");
}
