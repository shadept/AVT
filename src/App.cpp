#include "App.h"
#include "Mesh.h"

#include <fstream>
#include <sstream>

App::App() :
		Application("Title", 640, 480), mWorld("world")
{
	mFPS = 0;
	mCameraUp = Vector3::AxisY;
	mCameraH = mCameraV = 0;
	mOffsetX = mOffsetY = 0;
	mDragOriginX = mDragOriginY = 0.0f;
	mDistance = 5.0f;
	mDragging = mFriction = false;
	mDebug = false;
	mMaterial = 0;

	mLight.LocalTransform.SetPosition({0.0f, 0.0f, 20.0f});
	mLight.mAmbientColor = Vector3(0.1f, 0.1f, 0.1f);
	mLight.mDiffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	mLight.mSpecularColor = Vector3(1.0f, 1.0f, 1.0f);

	mRenderer->SetLighting(true);
	mRenderer->Draw(&mLight);

	mCamera.SetLookAt(Vector3(0.0f, 0.0f, mDistance), Vector3::Zero);
	mCamera.SetFustrum(35.0f, (float) 640 / 480, 1.0f, 20.0f);

	mRenderer->SetCamera(&mCamera);

	mModel = new Geometry("model");

	mCenter = new Node("center");
	mCenter->AttachChild(mModel);

	Handle h = -1;

	// verts: 2503
	// faces: 4968
	h = MeshManager.Load("bunny", "./models/bunny_smooth.obj");

	// verts: 2503
	// faces: 4968
//	MeshManager.Add("./models/bunny_flat.obj");

	// verts: 242841
	// faces: 483744
//	MeshManager.Load("zerling", "./models/zerling.obj");

	MeshManager.Load("diablo", "./models/Diablo.obj");
	MeshManager.Load("headcrab", "./models/headcrab.obj");
	MeshManager.Load("kaleidoscope", "./models/kaleidoscope.obj");

	mModel->SetMesh(MeshManager[h]->GetRaw());
	mModel->LocalTransform.SetPosition(Vector3(0.0f, -1.0f, 0.0f));

	h = TextureManager.Load("bunny", "./textures/bunny.png");

	TextureManager.Load("kaleidoscope0", "./textures/kaleidoscope0.png");
	TextureManager.Load("kaleidoscope1", "./textures/kaleidoscope1.png");

	TextureManager.Load("diablo", "./textures/Diablo_diff.png");
	TextureManager.Load("headcrab", "./textures/headcrab.png");

	Material* bunnyMaterial = new Material();
	bunnyMaterial->mAmbient = Vector3(0.1f, 0.05f, 0.0f);
	bunnyMaterial->mDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	bunnyMaterial->mSpecular = Vector3(1.0f, 0.5f, 0.0f);
	bunnyMaterial->mShininess = 0.1f;
	bunnyMaterial->mDiffuseMap = TextureManager[h]->GetRaw();
	h = MaterialManager.Add("bunny", bunnyMaterial);

	Material* kaleidoscopeMaterial = new Material();
	kaleidoscopeMaterial->mAmbient = Vector3(0.1f, 0.1f, 0.1f);
	kaleidoscopeMaterial->mDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	kaleidoscopeMaterial->mSpecular = Vector3(0.0f, 0.0f, 0.0f);
	kaleidoscopeMaterial->mShininess = 0.0f;
	kaleidoscopeMaterial->mDiffuseMap = TextureManager["kaleidoscope0"]->GetRaw();
	MaterialManager.Add("kaleidoscope", kaleidoscopeMaterial);

//	Material* diabloMaterial = new Material();
//	diabloMaterial->mAmbient = Vector3(0.0f, 0.0f, 0.0f);
//	diabloMaterial->mDiffuse = Vector3(0.64f, 0.64f, 0.64f);
//	diabloMaterial->mSpecular = Vector3(0.0f, 0.0f, 0.0f);
////	diabloMaterial->mShininess = 96.078431f;
//	diabloMaterial->mDiffuseMap = TextureManager["diablo"]->GetRaw();
//	MaterialManager.Add("diablo", diabloMaterial);
	MaterialManager.Load("diablo_material", "./materials/diablo.mtl");

//	Material* headcrabMaterial = new Material();
//	headcrabMaterial->mAmbient = Vector3(0.1f, 0.1f, 0.1f);
//	headcrabMaterial->mDiffuse = Vector3(0.6f, 0.6f, 0.6f);
//	headcrabMaterial->mSpecular = Vector3(0.9f, 0.9f, 0.9f);
//	headcrabMaterial->mShininess = 0.0f;
//	headcrabMaterial->mDiffuseMap = TextureManager["headcrab"]->GetRaw();
//	MaterialManager.Add("headcrab", headcrabMaterial);
	MaterialManager.Load("headcrab_material", "./materials/headcrab.mtl");

	mModel->SetMaterial(MaterialManager[h]->GetRaw());

	mWorld.AttachChild(mCenter);

	mSphere = new Geometry("sphere");
	mSphere->SetMesh(MeshManager["kaleidoscope"]->GetRaw());
	mSphere->SetMaterial(MaterialManager["kaleidoscope"]->GetRaw());
	mSphere->LocalTransform.SetPosition({ 0.0f, 0.0f, 0.0f });
	mSphere->LocalTransform.SetScale({ 10.0f, 10.0f, 10.0f });

	mWorld.AttachChild(mSphere);

	//mCenter->UpdateTransformation();
	//Vector3 center = mGeom->WorldTransform * mCenter->GetMesh()->GetCenterOfMass();
	//Logger::Debug << "Bunny center of mass in world coords " << center << Logger::endl;
}

App::~App()
{
	delete mCenter;
	delete mSphere;
	delete mModel;
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
		mDistance = Math::max(mDistance, 0.0f);
	}

	if (button == 4 && state == GLUT_DOWN)
	{
		mDistance += 0.3f;
		mDistance = Math::min(mDistance, 10.0f);
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
	static bool lighting = true;

	if (key == 'l')
	{
		lighting = !lighting;
		mRenderer->SetLighting(lighting);
	}

	if (key == 'm')
	{
		switch(mMaterial)
		{
		case 0: mModel->SetMaterial(MaterialManager["gold"]->GetRaw()); break;
		case 1: mModel->SetMaterial(MaterialManager["chrome"]->GetRaw()); break;
		case 2: mModel->SetMaterial(MaterialManager["ruby"]->GetRaw()); break;
		case 3: mModel->SetMaterial(MaterialManager["glass"]->GetRaw()); break;
		default: mModel->SetMaterial(MaterialManager["default"]->GetRaw());
		}
		mMaterial = (mMaterial + 1) % 4;
	}

	if (key == 'c')
	{
		static int k = 0;
		k = (k + 1) % 3;
		switch (k)
		{
		case 0:
			mModel->SetMesh(MeshManager["bunny"]->GetRaw());
			MaterialManager["glass"]->GetRaw()->mDiffuseMap = TextureManager["bunny"]->GetRaw();
			mModel->SetMaterial(MaterialManager["bunny"]->GetRaw());
			break;
		case 1:
			mModel->SetMesh(MeshManager["diablo"]->GetRaw());
			MaterialManager["glass"]->GetRaw()->mDiffuseMap = TextureManager["diablo"]->GetRaw();
			mModel->SetMaterial(MaterialManager["diablo"]->GetRaw());
			break;
		case 2:
			mModel->SetMesh(MeshManager["headcrab"]->GetRaw());
			MaterialManager["glass"]->GetRaw()->mDiffuseMap = TextureManager["headcrab"]->GetRaw();
			mModel->SetMaterial(MaterialManager["headcrab"]->GetRaw());
			break;
		}
		mMaterial = 0;
	}

	if (key == 'k')
	{
		static int k = 0;
		k = (k + 1) % 2;
		switch (k)
		{
		case 0: MaterialManager["kaleidoscope"]->GetRaw()->mDiffuseMap = TextureManager["kaleidoscope0"]->GetRaw(); break;
		case 1: MaterialManager["kaleidoscope"]->GetRaw()->mDiffuseMap = TextureManager["kaleidoscope1"]->GetRaw(); break;
		}
	}

	if (key == 's')
	{
		static bool debug = false;
		debug = !debug;
		mRenderer->SetDebug(debug);
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

	if (key == ' ')
		mMovingSphere = !mMovingSphere;
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
		ss << "SUPER HYPER MEGA SUCH TRIP WOW. Kiss, Diablo ;-) @ FPS : " << mFPS;
		mWindow->SetTitle(ss.str());
		counter = 0.0f;
		mFPS = 0;
	}

	mWorld.OnUpdate(delta);

	Real offX = Math::clamp(mOffsetX * 130.0f, -1000.0f, 1000.0f);
	Real offY = Math::clamp(mOffsetY * 130.0f, -1000.0f, 1000.0f);

//	if (mMovingSphere){
//		mSphere->LocalTransform.Rotate(Quaternion::fromAxisAngle(mSphere->LocalTransform.Up(), -offX * delta / 200.0f));
//		mSphere->LocalTransform.Rotate(Quaternion::fromAxisAngle(mSphere->LocalTransform.Side(), -offY * delta / 200.0f));
//	}
//	else{
//		mCenter->LocalTransform.Rotate(Quaternion::fromAxisAngle(mCenter->LocalTransform.Up(), offX * delta));
//		mCenter->LocalTransform.Rotate(Quaternion::fromAxisAngle(mCenter->LocalTransform.Side(), offY * delta));
//	}

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
