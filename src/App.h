#ifndef APP_H_
#define APP_H_

#include "Application.h"
#include "Camera.h"
#include "Geometry.h"
#include "Node.h"
#include "Light.h"

typedef std::vector<std::string> ArgumentList;

class App: public Application
{
public:
	App(ArgumentList args);
	~App();

	void OnDraw();
	void OnMouse(int button, int state, int x, int y);
	void OnMouseMotion(int x, int y);
	void OnKeyboard(unsigned char key, int x, int y);
	void OnResize(int w, int h);
	void OnUpdate(const Real delta);

private:
	int mFPS, mTotalFrames;
	float mTotalTime;

	Node mWorld;

	Camera mCamera;
	Vector3 mCameraUp;
	Real mOffsetX, mOffsetY;
	Real mDragOriginX, mDragOriginY;
	Real mDistance;

	bool mDragging, mFriction;
	bool mDebug;
	bool mMovingSphere = true;
	int mMaterial, subDivision = 1;

	std::vector<Geometry*> mGems;
	std::vector<Texture*> mBackgroundTextures;
	Geometry* mBackground, *mCurrentGem;
	Light mLight;
};

#endif /* APP_H_ */
