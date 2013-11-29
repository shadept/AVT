#include "Shader.h"

#include <cassert>
#include <iostream>

ResourceManager<ShaderResource, ShaderLoader> ShaderManager;

Shader::Shader(ShaderType type)
{
	mType = type;
	mShader = glCreateShader(GLenum(type));
	assert(mShader != 0);
}

Shader::~Shader()
{
	glDeleteShader(mShader);
	mShader = 0;
}

const Shader& Shader::Source(const std::string& src) const
{
	const char * tmp = src.c_str();
	glShaderSource(mShader, 1, &tmp, 0);
	return (*this);
}

const Shader& Shader::Compile() const
{
	glCompileShader(mShader);
	//bool success = checkInfoLog(mShader, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);
	//assert(success == true);
	return (*this);
}

bool Shader::IsCompiled() const
{
	int status;
	glGetShaderiv(mShader, GL_COMPILE_STATUS, &status);
	return status == GL_TRUE;
}

GLuint Program::msCurrentlyInUse = 0;

Program::Program()
{
	mProgram = glCreateProgram();
	assert(mProgram != 0);
}

Program::~Program()
{
	mAttributes.clear();
	mUniforms.clear();
	glDeleteProgram(mProgram);
	mProgram = 0;
}

GLint Program::operator ()(const std::string& attribute) const
{
	assert(IsLinked());
	if (mAttributes.find(attribute) != mAttributes.end())
		return mAttributes.at(attribute);
	else
	{
		GLint location = glGetAttribLocation(mProgram, attribute.c_str());
//		assert(location != -1 && "Uniform not in shader");
		if (location == -1) std::cout << "Warning: Attribute " << attribute << " location not found." << std::endl;
//		else std::cout << "Attribute " << attribute << " location: " << location << std::endl;
		mAttributes[attribute] = location;
		return location;
	}
}

GLint Program::operator [](const std::string& uniform) const
{
	assert(IsLinked());
	if (mUniforms.find(uniform) != mUniforms.end())
		return mUniforms.at(uniform);
	else
	{
		GLint location = glGetUniformLocation(mProgram, uniform.c_str());
//		assert(location != -1 && "Uniform not in shader");
		if (location == -1) std::cout << "Warning: Uniform " << uniform << " location not found." << std::endl;
//		else std::cout << "Uniform " << uniform << " location: " << location << std::endl;
		mUniforms[uniform] = location;
		return location;
	}
}

void Program::BindAttribute(VertexAttributes attrib, const std::string& attribute) const
{
	assert(!IsLinked());
	glBindAttribLocation(mProgram, GLenum(attrib), attribute.data());
}

const Program& Program::AttachShader(const Shader& shader) const
{
	assert(shader.IsCompiled());
	glAttachShader(mProgram, shader.mShader);
	return (*this);
}

const Program& Program::DetachShader(const Shader& shader) const
{
	glDetachShader(mProgram, shader.mShader);
	return (*this);
}

const Program& Program::Link() const
{
	glLinkProgram(mProgram);
	//bool success = checkInfoLog(mProgram, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);
	//assert(success == true);
	return (*this);
}

void Program::Use() const
{
	assert(IsLinked());
	if (msCurrentlyInUse != mProgram)
	{
		msCurrentlyInUse = mProgram;
		glUseProgram(mProgram);
	}
}

void Program::UseNone()
{
	msCurrentlyInUse = 0;
	glUseProgram(0);
}

bool Program::IsLinked() const
{
	int status;
	glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
	return status == GL_TRUE;
}

void Uniform::Bind(const Program& program, const std::string& name, Real value)
{
	GLint location = program[name];
	if (location != -1)
	{
		if (Program::msCurrentlyInUse == program.mProgram)
			glUniform1f(location, (GLfloat) value);
		else
		{
			GLint current;
			glGetIntegerv(GL_CURRENT_PROGRAM, &current);
			program.Use();
			glUniform1f(location, (GLfloat) value);
			glUseProgram(current);
			Program::msCurrentlyInUse = current;
		}
	}
	checkOpenGLError("Failed to bind uniform " + name);
}

void Uniform::Bind(const Program& program, const std::string& name, const Matrix4& value)
{
	GLint location = program[name];
	if (location != -1)
	{
		if (Program::msCurrentlyInUse == program.mProgram)
			glUniformMatrix4fv(location, 1, GL_TRUE, value);
		else
		{
			GLint current;
			glGetIntegerv(GL_CURRENT_PROGRAM, &current);
			program.Use();
			glUniformMatrix4fv(location, 1, GL_TRUE, value);
			glUseProgram(current);
			Program::msCurrentlyInUse = current;
		}
	}
	checkOpenGLError("Failed to bind uniform " + name);
}

void Uniform::Bind(const Program& program, const std::string& name, const Vector3& value)
{
	GLint location = program[name];
	if (location != -1)
	{
		if (Program::msCurrentlyInUse == program.mProgram)
			glUniform3fv(location, 1, value);
		else
		{
			GLint current;
			glGetIntegerv(GL_CURRENT_PROGRAM, &current);
			program.Use();
			glUniform3fv(location, 1, value);
			glUseProgram(current);
			Program::msCurrentlyInUse = current;
		}
	}
	checkOpenGLError("Failed to bind uniform " + name);
}
