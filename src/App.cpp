#include "App.h"
#include "Mesh.h"

#include <fstream>
#include <sstream>

App::App() :
		Application("Title", 640, 480)
{
	mFPS = 0;
	mCameraUp = Vector3::AxisY;
	mCameraH = mCameraV = 0;
	mOffsetX = mOffsetY = 0;
	mDragOriginX = mDragOriginY = 0.0f;
	mDistance = 5.0f;
	mDragging = mFriction = false;
	mRabbit = true;
	mDebug = false;

	Handle h = -1;
	h = ShaderManager.Add("./shaders/realistic");
	ShaderManager.Add("./shaders/debug");
	Program* shader = ShaderManager.GetElement(h)->GetShader();

	Uniform::Bind(*shader, "LightPosition", Vector3(0.0f, 0.0f, 20.0f));
	Uniform::Bind(*shader, "LightAmbientColor", Vector3(0.3f, 0.3f, 0.3f));
	Uniform::Bind(*shader, "LightDiffuseColor", Vector3(0.9f, 0.9f, 0.9f));
	Uniform::Bind(*shader, "LightSpecularColor", Vector3(0.9f, 0.9f, 0.9f));

	Uniform::Bind(*shader, "MaterialAmbientColor", Vector3(0.3f, 0.3f, 0.3f));
	Uniform::Bind(*shader, "MaterialDiffuseColor", Vector3(0.9f, 0.9f, 0.9f));
	Uniform::Bind(*shader, "MaterialSpecularColor", Vector3(0.9f, 0.9f, 0.9f));
	Uniform::Bind(*shader, "MaterialShininess", 256.0f);

	mCamera.SetLookAt(Vector3(0.0f, 0.0f, mDistance), Vector3::Zero);
	mCamera.SetFustrum(40.0f, (float) 640 / 480, 1.0f, 20.0f);

	mRenderer->Bind(shader);
	mRenderer->SetCamera(&mCamera);

	mGeom = new Geometry();

	// verts: 2503
	// faces: 4968
	h = MeshManager.Add("./models/bunny_smooth.obj");

	// verts: 2503
	// faces: 4968
	MeshManager.Add("./models/bunny_flat.obj");

	// verts: 242841
	// faces: 483744
	MeshManager.Add("./models/zerling.obj");

	mGeom->SetMesh(MeshManager.GetElement(h)->GetMesh());
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

	if (button == 3 && state == GLUT_DOWN)
	{
		mDistance -= 0.3f;
	}

	if (button == 4 && state == GLUT_DOWN)
	{
		mDistance += 0.3f;
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
			mGeom->SetMesh(MeshManager.GetElement("./models/zerling.obj")->GetMesh());
		}
		else
		{
			mGeom->SetMesh(MeshManager.GetElement("./models/bunny_smooth.obj")->GetMesh());
		}
		mRabbit = !mRabbit;
	}

	if (key == 's')
	{
		if (mDebug)
		{
			mRenderer->Bind(ShaderManager.GetElement("./shaders/realistic")->GetShader());
		}
		else
		{
			mRenderer->Bind(ShaderManager.GetElement("./shaders/debug")->GetShader());
		}
		mDebug = !mDebug;
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

	if (counter >= 1.0f)
	{
		std::stringstream ss;
		ss << "Title @ FPS : " << mFPS;
		mWindow->SetTitle(ss.str());
		counter = 0.0f;
		mFPS = 0;
	}

	mWorld.OnUpdate(delta);

	Real offX = mOffsetX * 130.0f;
	Real offY = mOffsetY * 130.0f;

	mCamera.LocalTransform.SetPosition(Vector3(0.0f, 0.0f, -mDistance));
	mCamera.LocalTransform.Rotate(Quaternion::fromAxisAngle(mCamera.LocalTransform.Up(), offX * delta));
	mCamera.LocalTransform.Rotate(Quaternion::fromAxisAngle(mCamera.LocalTransform.Side(), offY * delta));

	// friction baby!
	if (!mDragging)
	{
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
	}
	else
	{
		mOffsetX = mOffsetY = 0;
	}
}
