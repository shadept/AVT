#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Renderer.h"
#include "Window.h"

class Application
{
public:
	Application(const std::string& title, int width, int height);
	virtual ~Application();

	void MainLoop();

	virtual void OnCleanup();
	virtual void OnDraw();
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMouseMotion(int x, int y);
	virtual void OnKeyboard(unsigned char key, int x, int y);
	virtual void OnResize(int w, int h);
	virtual void OnUpdate(const Real delta);

	unsigned long GetTime() const;
	unsigned long GetDeltaTime() const;

protected:
	Window* mWindow;
	Renderer* mRenderer;

	int mMouseX, mMouseY;

private:
	mutable unsigned long mNow;
	mutable unsigned long mLastTime;
	mutable unsigned long mDelta;

public:
	// For internal use
	void OnDisplay();
	void OnIdle();
	void OnReshape(int w, int h);
	void UpdateMouse(int x, int y);
};

#endif /* APPLICATION_H_ */
