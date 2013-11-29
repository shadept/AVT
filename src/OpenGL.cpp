#include "OpenGL.h"

#include <iostream>

bool isOpenGLError()
{
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR)
	{
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if (isOpenGLError())
	{
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

bool checkInfoLog(GLuint obj, GLenum what, glGetObjectiv glGetiv, glGetObjectInfoLog glGetInfoLog)
{
	GLint status;
	glGetiv(obj, what, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetInfoLog(obj, infoLogLength, NULL, strInfoLog);
		std::cerr << strInfoLog << std::endl;
		delete[] strInfoLog;
		return false;
	}
	return true;
}
