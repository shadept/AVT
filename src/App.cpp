#include "App.h"
#include "Mesh.h"

#include <fstream>
#include <sstream>

App::App(ArgumentList args) :
		Application("Title", 1024, 768), mWorld("world")
{
	mFPS = mTotalFrames = 0;
	mTotalTime = 0.0f;
	mCameraUp = Vector3::AxisY;
	mOffsetX = mOffsetY = 0;
//	mOffsetX = 1.5f;
	mDragOriginX = mDragOriginY = 0.0f;
	mDistance = 5.0f;
	mDragging = mFriction = false;
	mDebug = false;
	mMaterial = 0;

	mLight.mType = Light::Type::DIRECTIONAL;
	mLight.LocalTransform.SetPosition({0.0f, 0.0f, 1.0f}); // this is a direction because it's a directional light
//	mLight.mType = Light::Type::POINTLIGHT;
//	mLight.LocalTransform.SetPosition({0.0f, 0.0f, 5.0f}); // this is a position because it's a point light
	mLight.mAmbientColor = Vector3(1.0f, 1.0f, 1.0f);
	mLight.mDiffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	mLight.mSpecularColor = Vector3(1.0f, 1.0f, 1.0f);

	mWorld.AttachChild(&mLight);
//	mWorld.AttachChild(mLight.mDebugSphere);

	mRenderer->SetLighting(true);
	mRenderer->Draw(&mLight);

	mCamera.SetLookAt(Vector3(0.0f, 0.0f, mDistance), Vector3::Zero);
	mCamera.SetPerspective(35.0f, (float) mWindow->GetWidth() / mWindow->GetHeight(), 1.0f, 50.0f);

	mRenderer->SetCamera(&mCamera);

	mModel = new Geometry("model");

	mCenter = new Node("center");
	mCenter->AttachChild(mModel);

	MeshManager.Load("sphere", "./models/sphere.obj");
	MeshManager.Load("cube", "./models/cube.obj");
	MeshManager.Load("cylinder", "./models/cylinder.obj");
	MeshManager.Load("torus", "./models/torus.obj");
	MeshManager.Load("knot", "./models/knot.obj");
	MeshManager.Load("diamond", "./models/diamond.obj");
	MeshManager.Load("suzanne", "./models/suzanne.obj");
	MeshManager.Load("bunny", "./models/bunny.obj");

	MeshManager.Load("diablo", "./models/Diablo.obj");
	MeshManager.Load("headcrab", "./models/headcrab.obj");
	MeshManager.Load("kaleidoscope", "./models/kaleidoscope.obj");
//	MeshManager.Load("kaleidoscope", "./models/sphere.obj"); // esta esfera nao funca lol, normals ao contrario

	mModel->SetMesh(MeshManager["sphere"]->GetRaw());

	TextureManager.Load("bunny", "./textures/bunny.png");

	TextureManager.Load("kaleidoscope0", "./textures/kaleidoscope0.png");
	TextureManager.Load("kaleidoscope1", "./textures/kaleidoscope1.png");

	Material* bunnyMaterial = new Material();
	bunnyMaterial->mAmbient = Vector3(0.1f, 0.05f, 0.0f);
	bunnyMaterial->mDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	bunnyMaterial->mSpecular = Vector3(1.0f, 0.5f, 0.0f);
	bunnyMaterial->mShininess = 0.1f;
	bunnyMaterial->mDiffuseMap = TextureManager["bunny"]->GetRaw();
	MaterialManager.Add("bunny", bunnyMaterial);

	Material* kaleidoscopeMaterial = new Material();
	kaleidoscopeMaterial->mAmbient = Vector3(0.1f, 0.1f, 0.1f);
	kaleidoscopeMaterial->mDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	kaleidoscopeMaterial->mSpecular = Vector3(0.0f, 0.0f, 0.0f);
	kaleidoscopeMaterial->mShininess = 0.0f;
	kaleidoscopeMaterial->mDiffuseMap = TextureManager["kaleidoscope0"]->GetRaw();
	MaterialManager.Add("kaleidoscope", kaleidoscopeMaterial);

	MaterialManager.Load("diablo_material", "./materials/diablo.mtl");
	MaterialManager.Load("headcrab_material", "./materials/headcrab.mtl");

	mModel->SetMaterial(MaterialManager["chrome"]->GetRaw());

	mWorld.AttachChild(mCenter);

	mSphere = new Geometry("sphere");
	mSphere->SetMesh(MeshManager["kaleidoscope"]->GetRaw());
	mSphere->SetMaterial(MaterialManager["kaleidoscope"]->GetRaw());
	mSphere->LocalTransform.SetPosition({ 0.0f, 0.0f, 0.0f });
	mSphere->LocalTransform.SetScale({ 20.0f, 20.0f, 20.0f });

	mWorld.AttachChild(mSphere);
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
	mTotalFrames++;
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
		static std::string material[5] = {"gold", "chrome", "ruby", "glass", "default"};
		mMaterial = (mMaterial + 1) % 5;
		mModel->SetMaterial(MaterialManager[material[mMaterial]]->GetRaw());
//		switch(mMaterial)
//		{
//		case 0: mModel->SetMaterial(MaterialManager["gold"]->GetRaw()); break;
//		case 1: mModel->SetMaterial(MaterialManager["chrome"]->GetRaw()); break;
//		case 2: mModel->SetMaterial(MaterialManager["ruby"]->GetRaw()); break;
//		case 3: mModel->SetMaterial(MaterialManager["glass"]->GetRaw());break;
//		default: mModel->SetMaterial(MaterialManager["default"]->GetRaw());
//		}
	}

	if (key == 'c')
	{
		static std::string models[8] = {"sphere", "cube", "cylinder", "torus", "knot", "diamond", "suzanne", "bunny"};//, "diablo", "headcrab"};
		static int k = 0;
		k = (k + 1) % 8;
		mModel->SetMesh(MeshManager[models[k]]->GetRaw());
//		switch (k)
//		{
//		case 0:
//		case 1:
//		case 2:
//		case 3:
//		case 4:
//			MaterialManager["glass"]->GetRaw()->mDiffuseMap = nullptr;
//			mModel->SetMesh(MeshManager[models[k]]->GetRaw());
//			break;
//		case 5:
//			mModel->SetMesh(MeshManager["bunny"]->GetRaw());
//			MaterialManager["glass"]->GetRaw()->mDiffuseMap = TextureManager["bunny"]->GetRaw();
//			MaterialManager["glass"]->GetRaw()->mBumpMap = NULL;
//			break;
//		case 6:
//			mModel->SetMesh(MeshManager["diablo"]->GetRaw());
//			MaterialManager["glass"]->GetRaw()->mDiffuseMap = TextureManager["Diablo_diff.png"]->GetRaw();
//			MaterialManager["glass"]->GetRaw()->mBumpMap = TextureManager["Diablo_norm.png"]->GetRaw();
//			break;
//		case 7:
//			mModel->SetMesh(MeshManager["headcrab"]->GetRaw());
//			MaterialManager["glass"]->GetRaw()->mDiffuseMap = TextureManager["headcrab.png"]->GetRaw();
//			MaterialManager["glass"]->GetRaw()->mBumpMap = TextureManager["headcrab_normal.png"]->GetRaw();
//			break;
//		}
	}

	if (key == 'k')
	{
		static int k = 0;
		k = (k + 1) % 3;
		switch (k)
		{
		case 0: mSphere->SetShader(NULL); MaterialManager["kaleidoscope"]->GetRaw()->mDiffuseMap = TextureManager["kaleidoscope0"]->GetRaw(); break;
		case 1: MaterialManager["kaleidoscope"]->GetRaw()->mDiffuseMap = TextureManager["kaleidoscope1"]->GetRaw(); break;
		case 2: mSphere->SetShader(ShaderManager["skybox"]->GetRaw()); break;
		}
	}

	if (key == 's')
	{
		static int debug = 0;
		debug = (debug + 1) % Renderer::MAX_DEBUG;
		mRenderer->SetDebug((Renderer::DebugShader)debug);
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

	static Vector3 lookAt[] = { {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };
	static Vector3 up[] = { {0, 1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}, {0, 1, 0}, {0, 1, 0} };
	static float ratio = (float) mWindow->GetWidth() / mWindow->GetHeight();
	switch(key)
	{
	case '0': mCamera.SetPerspective(35.0f, ratio, 1.0f, 50.0f); mDistance = 5.0f; mCamera.SetLookAt({0.0f, 0.0f, mDistance}, Vector3::Zero); break;
	case '1': mDistance = 0.0f; mCamera.SetPerspective(90.0f, 1.0f, 1.0f, 50.0f); mCamera.SetLookAt(Vector3::Zero, lookAt[0], up[0]); break;
	case '2': mDistance = 0.0f; mCamera.SetPerspective(90.0f, 1.0f, 1.0f, 50.0f); mCamera.SetLookAt(Vector3::Zero, lookAt[1], up[1]); break;
	case '3': mDistance = 0.0f; mCamera.SetPerspective(90.0f, 1.0f, 1.0f, 50.0f); mCamera.SetLookAt(Vector3::Zero, lookAt[2], up[2]); break;
	case '4': mDistance = 0.0f; mCamera.SetPerspective(90.0f, 1.0f, 1.0f, 50.0f); mCamera.SetLookAt(Vector3::Zero, lookAt[3], up[3]); break;
	case '5': mDistance = 0.0f; mCamera.SetPerspective(90.0f, 1.0f, 1.0f, 50.0f); mCamera.SetLookAt(Vector3::Zero, lookAt[4], up[4]); break;
	case '6': mDistance = 0.0f; mCamera.SetPerspective(90.0f, 1.0f, 1.0f, 50.0f); mCamera.SetLookAt(Vector3::Zero, lookAt[5], up[5]); break;
	}
}

void App::OnResize(int w, int h)
{
	float ratio = (float) w / h;
	mCamera.SetViewport(w, h);
	mCamera.SetPerspective(35.0f, ratio, 1.0f, 50.0f);
}

void App::OnUpdate(const Real delta)
{
	static Real counter = 0.0f;
	static Real envMapCounter = 0.0f;
	counter += delta;
	envMapCounter += delta;

	if (envMapCounter > 1.0f/30.0f) // every 1/30 seconds
	{
		mModel->Enabled(false);
		mRenderer->BuildEnvironmentMap(&mWorld, Vector3::Zero);
		// Correction transparent refraction
//		mRenderer->BuildEnvironmentMap(&mWorld, mCamera.LocalTransform.Orientation() * -mCamera.LocalTransform.Position());
		envMapCounter = 0.0f;
		mModel->Enabled(true);
	}

	while (counter >= 1.0f)
	{
		std::stringstream ss;
		ss << "Kaleidoscope thingy @ FPS : " << mFPS;
		mWindow->SetTitle(ss.str());
		counter -= 1.0f;
		mFPS = 0;
		mTotalTime += counter;
	}

	mWorld.OnUpdate(delta);

	Real offX = Math::clamp(mOffsetX * 130.0f, -1000.0f, 1000.0f);
	Real offY = Math::clamp(mOffsetY * 130.0f, -1000.0f, 1000.0f);

//	if (mMovingSphere)
//	{
//		mSphere->LocalTransform.Rotate(Quaternion::fromAxisAngle(mSphere->LocalTransform.Up(), -offX * delta / 200.0f));
//		mSphere->LocalTransform.Rotate(Quaternion::fromAxisAngle(mSphere->LocalTransform.Side(), -offY * delta / 200.0f));
//	}
//	else
//	{
//		mCenter->LocalTransform.Rotate(Quaternion::fromAxisAngle(mCenter->LocalTransform.Up(), offX * delta));
//		mCenter->LocalTransform.Rotate(Quaternion::fromAxisAngle(mCenter->LocalTransform.Side(), offY * delta));
//	}

	mCamera.LocalTransform.SetPosition(Vector3(0.0f, 0.0f, -mDistance));
//	mLight.LocalTransform.SetPosition({0.0f, 0.0f, mDistance});
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
