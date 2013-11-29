#include "App.h"
#include "Mesh.h"

#include <fstream>
#include <sstream>

std::string readFile(const std::string& filename);

Program* p;

App::App() :
		Application("Title", 640, 480)
{
	mFPS = 0;
	mCameraUp = Vector3::AxisY;
	mCameraH = mCameraV = 0;
	mOffsetX = mOffsetY = 0;
	mDragOriginX = mDragOriginY = 0.0f;
	mDragging = mFriction = false;
	mRabbit = true;

	VertexShader vs;
	vs.Source(readFile("shaders/realistic.vert"));
	vs.Compile();

	FragmentShader fs;
	fs.Source(readFile("shaders/realistic.frag"));
	fs.Compile();

	ProgramPtr shader(new Program());
	shader->AttachShader(vs).AttachShader(fs);
	shader->BindAttribute(VertexAttributes::POSITION, "in_Position");
	shader->BindAttribute(VertexAttributes::NORMAL, "in_Normal");
	shader->Link();

	p = shader.get();

	Uniform::Bind(*shader, "LightPosition", Vector3(0.0f, 0.0f, 20.0f));
	Uniform::Bind(*shader, "LightAmbientColor", Vector3(0.1f, 0.1f, 0.1f));
	Uniform::Bind(*shader, "LightDiffuseColor", Vector3(0.9f, 0.9f, 0.9f));
	Uniform::Bind(*shader, "LightSpecularColor", Vector3(0.9f, 0.9f, 0.9f));

	Uniform::Bind(*shader, "MaterialAmbientColor", Vector3(0.1f, 0.1f, 0.1f));
	Uniform::Bind(*shader, "MaterialDiffuseColor", Vector3(0.9f, 0.9f, 0.9f));
	Uniform::Bind(*shader, "MaterialSpecularColor", Vector3(0.9f, 0.9f, 0.9f));
	Uniform::Bind(*shader, "MaterialShininess", 256.0f);

	mCamera.SetLookAt(Vector3(0.0f, 0.0f, 5.0f), Vector3::Zero);
	mCamera.SetFustrum(40.0f, (float) 640 / 480, 1.0f, 20.0f);

	mRenderer->Bind(shader);
	mRenderer->SetCamera(&mCamera);

	mGeom = new Geometry();
	mGeom->SetMesh(MeshManager::Get("models/bunny_smooth.obj"));
	mGeom->LocalTransform.SetPosition(Vector3(0.0f, -1.0f, 0.0f));

	mWorld.AttachChild(mGeom);
}

App::~App()
{
	delete mGeom;
}

void App::OnDraw()
{
	mRenderer->DrawScene(&mWorld);
	mFPS++;
}

void App::OnMouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mDragging = true;
			mFriction = false;
			mDragOriginX = x;
			mDragOriginY = y;
			mOffsetX = mOffsetY = 0;
		}
		else
		{
			mDragging = false;
		}
	}
}

void App::OnMouseMotion(int x, int y)
{
	if (mDragging)
	{
		mOffsetX = (x - mDragOriginX);
		mOffsetY = (y - mDragOriginY);
		mDragOriginX = x;
		mDragOriginY = y;
	}
}

void App::OnKeyboard(unsigned char key, int x, int y)
{
	if (key == 'c')
	{
		if (mRabbit)
		{
			mGeom->SetMesh(MeshManager::Get("models/cube.obj"));
			mGeom->LocalTransform.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			mGeom->SetMesh(MeshManager::Get("models/bunny_smooth.obj"));
			mGeom->LocalTransform.SetPosition(Vector3(0.0f, -1.0f, 0.0f));
		}
		mRabbit = !mRabbit;
	}

	if (key == 'f')
		mFriction = !mFriction;

	if (key == 'i')
	{
		std::cout << "Position: " << mCamera.LocalTransform.Position();
		Vector3 v = mCamera.LocalTransform.Orientation() * mCamera.LocalTransform.Position();
		std::cout << "Transformed: " << v;
		std::cout << "Length: " << v.length() << std::endl;
	}
}

void App::OnResize(int w, int h)
{
	float ratio = (float) w / h;
	mCamera.SetFustrum(30.0f, ratio, 1.0f, 20.0f);
}

void App::OnUpdate(const Real delta)
{
	static Real counter = 0.0f;
	counter += delta;

	if(counter >= 1.0f)
	{
		std::stringstream ss;
		ss << "Title @ FPS : " << mFPS;
		mWindow->SetTitle(ss.str());
		counter = 0.0f;
		mFPS = 0;
	}

	mWorld.OnUpdate(delta);

	Real offX =	mOffsetX * 130.0f;
	Real offY = mOffsetY * 130.0f;

	mCamera.LocalTransform.Rotate(
		Quaternion::fromAxisAngle(mCamera.LocalTransform.Up(), offX * delta));
	mCamera.LocalTransform.Rotate(
		Quaternion::fromAxisAngle(mCamera.LocalTransform.Side(), offY * delta));

	// friction baby!
	if (!mDragging) {
		if (mFriction)
		{
			if (Math::abs(mOffsetX) > Math::epsilon)
				mOffsetX *= Math::pow(0.09f, delta);
			else
				mOffsetX = 0.0f;
			if (Math::abs(mOffsetY) > Math::epsilon)
				mOffsetY *= Math::pow(0.09f, delta);
			else
				mOffsetY = 0.0f;

			if (mOffsetY == 0.0f && mOffsetX == 0.0f)
				mFriction = false;
		}
	} else {
		mOffsetX = mOffsetY = 0;
	}
}
