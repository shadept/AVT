#include "Window.h"

#include <iostream>

Window::Window(const std::string& title, int width, int height)
{
	mWidth = width;
	mHeight = height;
	mContext = new Context();
	mRenderer = nullptr;

	//mContext->Init();
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	mHandle = glutCreateWindow(title.c_str());
	if (mHandle < 1)
	{
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}

	// FIXME this shouldn't be here but GLUT forces this
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << " " << err_code << std::endl;

	mContext->SetDefaultState();
//	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Window::~Window()
{
	//glutDestroyWindow(mHandle);
	mHandle = 0;
	delete mContext;
}

void Window::Touch() const
{
	glutSetWindow(mHandle);
	glutPostRedisplay();
}

void Window::Resize(int width, int height)
{
	//mContext->Bind();
	mWidth = width;
	mHeight = height;
	glViewport(0, 0, width, height);
}

void Window::SetTitle(const std::string& title)
{
	glutSetWindow(mHandle);
	glutSetWindowTitle(title.c_str());
}

void Window::SetRenderer(Renderer* renderer)
{
	mRenderer = renderer;
}

int Window::GetWidth() const
{
	return mWidth;
}

int Window::GetHeight() const
{
	return mHeight;
}

Context* Window::GetContext() const
{
	return mContext;
}
