#include "App.h"
#include "Mesh.h"

#include "IcoSphere.h"

#include <fstream>
#include <sstream>

App::App(ArgumentList args) :
		Application("Title", 1024, 768), mWorld("world")
{
	mFPS = mTotalFrames = 0;
	mTotalTime = 0.0f;
	mCameraUp = Vector3::AxisY;
	mOffsetX = mOffsetY = 0;
	mOffsetX = 0.02f;
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

	mRenderer->SetLighting(true);
	mRenderer->Draw(&mLight);

	mCamera.SetLookAt(Vector3(0.0f, 0.0f, mDistance), Vector3::Zero);
	mCamera.SetPerspective(35.0f, (float) mWindow->GetWidth() / mWindow->GetHeight(), 1.0f, 50.0f);

	mRenderer->SetCamera(&mCamera);

	MeshManager.Load("sphere", "./models/sphere.obj");

	MeshManager.Load("prism1", "./models/prism1.obj");
	MeshManager.Load("prism2", "./models/prism2.obj");
	MeshManager.Load("prism3", "./models/prism3.obj");
	MeshManager.Load("prism4", "./models/prism4.obj");
	MeshManager.Load("prism5", "./models/prism5.obj");
	MeshManager.Load("prism6", "./models/prism6.obj");
	MeshManager.Load("prism7", "./models/prism7.obj");

	mGems.push_back(new Geometry("prism1", MeshManager["prism1"]->GetRaw(), MaterialManager["glass"]->GetRaw()));
	mGems.push_back(new Geometry("prism2", MeshManager["prism2"]->GetRaw(), MaterialManager["emerald"]->GetRaw()));
	mGems.push_back(new Geometry("prism3", MeshManager["prism3"]->GetRaw(), MaterialManager["turquoise"]->GetRaw()));
	mGems.push_back(new Geometry("prism4", MeshManager["prism4"]->GetRaw(), MaterialManager["jade"]->GetRaw()));
	mGems.push_back(new Geometry("prism5", MeshManager["prism5"]->GetRaw(), MaterialManager["pearl"]->GetRaw()));
	mGems.push_back(new Geometry("prism6", MeshManager["prism6"]->GetRaw(), MaterialManager["ruby"]->GetRaw()));
	mGems.push_back(new Geometry("prism7", MeshManager["prism7"]->GetRaw(), MaterialManager["diamond"]->GetRaw()));

	TextureManager.Load("background1", "./textures/image7.png");
	TextureManager.Load("background2", "./textures/image2-3.png");
	TextureManager.Load("background3", "./textures/image4.png");
	TextureManager.Load("background4", "./textures/image9.png");
	TextureManager.Load("background5", "./textures/image8.png");

	mBackgroundTextures.push_back(TextureManager["background1"]->GetRaw());
	mBackgroundTextures.push_back(TextureManager["background2"]->GetRaw());
	mBackgroundTextures.push_back(TextureManager["background3"]->GetRaw());
	mBackgroundTextures.push_back(TextureManager["background4"]->GetRaw());
	mBackgroundTextures.push_back(TextureManager["background5"]->GetRaw());

	Material* backgroundMaterial = new Material();
	backgroundMaterial->mAmbient = Vector3(0.1f, 0.1f, 0.1f);
	backgroundMaterial->mDiffuse = Vector3(1.0f, 1.0f, 1.0f);
	backgroundMaterial->mSpecular = Vector3(0.0f, 0.0f, 0.0f);
	backgroundMaterial->mShininess = 0.0f;
	backgroundMaterial->mDiffuseMap = TextureManager["background1"]->GetRaw();
	MaterialManager.Add("background", backgroundMaterial);

	// Scene
	mBackground = new Geometry("background", IcoSphere::Create(subDivision), MaterialManager["background"]->GetRaw());
	mBackground->LocalTransform.SetScale({20.f, 20.f, 20.f});
	mWorld.AttachChild(mBackground);

	mCurrentGem = new Geometry("gem", MeshManager["prism1"]->GetRaw(), MaterialManager["glass"]->GetRaw());
	mWorld.AttachChild(mCurrentGem);
}

App::~App()
{
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
	if (key >= '1' && key < '8')
	{
		int gem = key - '1';
		mCurrentGem->SetMesh(mGems[gem]->GetMesh());
		mCurrentGem->SetMaterial(mGems[gem]->GetMaterial());
	}

	if (key == 'k')
	{
		static int background = 0;
		background = (background + 1) % mBackgroundTextures.size();
		MaterialManager["background"]->GetRaw()->mDiffuseMap = mBackgroundTextures[background];
	}

	if (key == 'f')
		mFriction = !mFriction;

	if (key == ' ')
		mMovingSphere = !mMovingSphere;

	if (key == '-')
	{
		subDivision = Math::max(subDivision - 1, 0);
		delete mBackground->GetMesh();
		mBackground->SetMesh(IcoSphere::Create(subDivision));
	}

	if (key == '+')
	{
		subDivision = Math::min(subDivision + 1, 3);
		delete mBackground->GetMesh();
		mBackground->SetMesh(IcoSphere::Create(subDivision));
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
		mCurrentGem->Enabled(false);
		Vector3 position = mCurrentGem->WorldTransform.Position();
		mRenderer->BuildEnvironmentMap(&mWorld, position);
		envMapCounter = 0.0f;
		mCurrentGem->Enabled(true);
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

	Real offX = Math::clamp(mOffsetX * 50.0f, -1000.0f, 1000.0f);
	Real offY = Math::clamp(mOffsetY * 50.0f, -1000.0f, 1000.0f);

	if (mMovingSphere)
	{
		mBackground->LocalTransform.Rotate(Quaternion::fromAxisAngle(mBackground->LocalTransform.Up(), offX * delta));
		mBackground->LocalTransform.Rotate(Quaternion::fromAxisAngle(mBackground->LocalTransform.Side(), offY * delta));
	}
	else
	{
		mCurrentGem->LocalTransform.Rotate(Quaternion::fromAxisAngle(mCurrentGem->LocalTransform.Up(), offX * delta));
		mCurrentGem->LocalTransform.Rotate(Quaternion::fromAxisAngle(mCurrentGem->LocalTransform.Side(), offY * delta));
	}

	mCamera.LocalTransform.SetPosition(Vector3(0.0f, 0.0f, -mDistance));

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
