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
	mDragging = mFriction = mLightOnCamera = false;
	mRabbit = true;
	mDebug = false;
	mMaterial = 0;

	Handle h = -1;
	h = ShaderManager.Add("./shaders/realistic");
	ShaderManager.Add("./shaders/debug");
	Program* shader = ShaderManager.GetElement(h)->GetRaw();

	Uniform::Bind(*shader, "LightPosition", Vector3(0.0f, 0.0f, 20.0f));
	Uniform::Bind(*shader, "LightAmbientColor", Vector3(0.1f, 0.1f, 0.1f));
	Uniform::Bind(*shader, "LightDiffuseColor", Vector3(1.0f, 1.0f, 1.0f));
	Uniform::Bind(*shader, "LightSpecularColor", Vector3(1.0f, 1.0f, 1.0f));

	mCamera.SetLookAt(Vector3(0.0f, 0.0f, mDistance), Vector3::Zero);
	mCamera.SetFustrum(35.0f, (float) 640 / 480, 1.0f, 20.0f);

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

	mGeom->SetMesh(MeshManager.GetElement(h)->GetRaw());
	mGeom->LocalTransform.SetPosition(Vector3(0.0f, -1.0f, 0.0f));

	Material* porcelainMaterial = new Material();
	porcelainMaterial->mAmbient = Vector3(0.1f, 0.1f, 0.1f);
	porcelainMaterial->mDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	porcelainMaterial->mSpecular = Vector3(1.0f, 1.0f, 1.0f);
	porcelainMaterial->mShininess = 50.0f;
	h = MaterialManager.Add("porcelain", porcelainMaterial);

	Material* goldMaterial = new Material();
	goldMaterial->mAmbient = Vector3(0.24725f, 0.1995f, 0.0745f);
	goldMaterial->mDiffuse = Vector3(0.75164f, 0.60648f, 0.22648f);
	goldMaterial->mSpecular = Vector3(0.628281f, 0.555802f, 0.366065f);
	goldMaterial->mShininess = 0.4f;
	MaterialManager.Add("gold", goldMaterial);

	Material* chromeMaterial = new Material();
	chromeMaterial->mAmbient = Vector3(0.25f, 0.25f, 0.25f);
	chromeMaterial->mDiffuse = Vector3(0.4f, 0.4f, 0.4f);
	chromeMaterial->mSpecular = Vector3(0.774597f, 0.774597f, 0.774597f);
	chromeMaterial->mShininess = 0.6f;
	MaterialManager.Add("chrome", chromeMaterial);

	mGeom->SetMaterial(MaterialManager.GetElement(h)->GetRaw());

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
	if (key == 'm')
	{
		switch(mMaterial)
		{
		case 0: mGeom->SetMaterial(MaterialManager.GetElement("gold")->GetRaw()); break;
		case 1: mGeom->SetMaterial(MaterialManager.GetElement("chrome")->GetRaw()); break;
		case 2: mGeom->SetMaterial(MaterialManager.GetElement("porcelain")->GetRaw()); break;
		default: mGeom->SetMaterial(MaterialManager.GetElement("default")->GetRaw());
		}
		mMaterial = (mMaterial + 1) % 3;
	}

	if (key == 'c')
	{
		if (mRabbit)
		{
			mGeom->SetMesh(MeshManager.GetElement("./models/zerling.obj")->GetRaw());
		}
		else
		{
			mGeom->SetMesh(MeshManager.GetElement("./models/bunny_smooth.obj")->GetRaw());
		}
		mRabbit = !mRabbit;
	}

	if (key == 's')
	{
		if (mDebug)
		{
			mRenderer->Bind(ShaderManager.GetElement("./shaders/realistic")->GetRaw());
		}
		else
		{
			mRenderer->Bind(ShaderManager.GetElement("./shaders/debug")->GetRaw());
		}
		mDebug = !mDebug;
	}

	if (key == 'f')
		mFriction = !mFriction;

	if (key == 'l')
		mLightOnCamera = !mLightOnCamera;

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
	mCamera.SetFustrum(35.0f, ratio, 1.0f, 20.0f);
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

	if (mLightOnCamera)
	{
		Vector3 v = mCamera.LocalTransform.Orientation() * mCamera.LocalTransform.Position();
		v.Z = -v.Z; // FIXME negating Z coord a lot, is this correct?
		Uniform::Bind(*ShaderManager.GetElement("./shaders/realistic")->GetRaw(), "LightPosition", v);
	}

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
