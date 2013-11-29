#include "Application.h"
#include <chrono>

Application* gCurrentInstance;

extern "C"
void displayCallback() {
	gCurrentInstance->OnDisplay();
}

extern "C"
void idleCallback() {
	gCurrentInstance->OnIdle();
}

extern "C"
void mouseCallback(int button, int state, int x, int y) {
	gCurrentInstance->OnMouse(button, state, x, y);
	gCurrentInstance->UpdateMouse(x, y);
}

extern "C"
void motionCallback(int x, int y) {
	gCurrentInstance->OnMouseMotion(x, y);
	gCurrentInstance->UpdateMouse(x, y);
}

extern "C"
void reshapeCallback(int w, int h) {
	gCurrentInstance->OnReshape(w, h);
}

extern "C"
void keyboardCallback(unsigned char key, int x, int y) {
	gCurrentInstance->OnKeyboard(key, x, y);
}

unsigned long timestamp()
{
//	return glutGet(GLUT_ELAPSED_TIME);
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Application::Application(const std::string& title, int width, int height)
{
	mMouseX = mMouseY = 0;
	mNow = mDelta = 0.0f;
	mLastTime = timestamp();
	mWindow = new Window(title, width, height);
	mRenderer = new Renderer;

	mWindow->GetContext()->SetRenderer(mRenderer);

	gCurrentInstance = this;
//	glutCloseFunc(cleanupCallBack);
	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
}

Application::~Application()
{
	delete mRenderer;
	delete mWindow;
}

void Application::MainLoop()
{
	glutMainLoop();
}

void Application::OnDraw()
{
}

void Application::OnMouse(int button, int state, int x, int y)
{
}

void Application::OnMouseMotion(int x, int y)
{
}

void Application::OnKeyboard(unsigned char key, int x, int y)
{
}

void Application::OnResize(int w, int h)
{
}

void Application::OnUpdate(Real delta)
{
}

unsigned long Application::GetTime() const {
	return mNow;
}

unsigned long Application::GetDeltaTime() const {
	return mDelta;
}

void Application::OnDisplay()
{
	mRenderer->_frameCounter++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OnDraw();

	glutSwapBuffers();
}

void Application::OnIdle()
{
	mNow = timestamp();
	mDelta = mNow - mLastTime;

	OnUpdate(mDelta * 0.001f);

	glutPostRedisplay();
	mLastTime = mNow;
}

void Application::OnReshape(int w, int h)
{
	mWindow->Resize(w, h);
	OnResize(w, h);
}

void Application::UpdateMouse(int x, int y)
{
	mMouseX = x;
	mMouseY = y;
}
