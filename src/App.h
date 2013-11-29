#ifndef APP_H_
#define APP_H_

#include "Application.h"
#include "Camera.h"
#include "Geometry.h"
#include "Node.h"

class App: public Application
{
public:
	App();
	~App();

	void OnDraw();
	void OnMouse(int button, int state, int x, int y);
	void OnMouseMotion(int x, int y);
	void OnKeyboard(unsigned char key, int x, int y);
	void OnResize(int w, int h);
	void OnUpdate(const Real delta);

private:
	int mFPS;

	Node mWorld;

	Camera mCamera;
	Vector3 mCameraUp;
	Real mOffsetX, mOffsetY;
	Real mDragOriginX, mDragOriginY;
	Real mCameraH, mCameraV;
	Real mDistance;

	bool mDragging, mFriction;
	bool mRabbit, mDebug;

	Geometry* mGeom;
};

#endif /* APP_H_ */
