#include "Window.h"

#include <iostream>

Window::Window(const std::string& title, int width, int height)
{
	mWidth = width;
	mHeight = height;

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	mHandle = glutCreateWindow(title.c_str());
	if (mHandle < 1)
	{
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}

	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << " " << err_code << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Window::~Window()
{
	//glutDestroyWindow(mHandle);
	mHandle = 0;
}

void Window::Touch() const
{
	glutSetWindow(mHandle);
	glutPostRedisplay();
}

void Window::Resize(int width, int height)
{
	mWidth = width;
	mHeight = height;
}

void Window::SetTitle(const std::string& title)
{
	glutSetWindow(mHandle);
	glutSetWindowTitle(title.c_str());
}

int Window::GetWidth() const
{
	return mWidth;
}

int Window::GetHeight() const
{
	return mHeight;
}
