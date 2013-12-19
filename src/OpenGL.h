#ifndef OPENGL_H_
#define OPENGL_H_

#include <memory>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

bool isOpenGLError();
void checkOpenGLError(std::string error);

typedef void (*glGetObjectiv)(GLuint,GLenum,GLint*);
typedef void (*glGetObjectInfoLog)(GLuint,GLsizei,GLsizei*,GLchar*);

#ifdef _MSC_VER
bool checkInfoLog(GLuint obj, GLenum what, PFNGLGETSHADERIVPROC glGetiv, PFNGLGETSHADERINFOLOGPROC glGetInfoLog);
#else
bool checkInfoLog(GLuint obj, GLenum what, glGetObjectiv glGetiv, glGetObjectInfoLog glGetInfoLog);
#endif // _MSC_VER

#endif /* OPENGL_H_ */
