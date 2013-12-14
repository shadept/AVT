#ifndef SHADER_H_
#define SHADER_H_

#include "OpenGL.h"

#include <map>

enum class VertexAttributes
{
	POSITION = 0,
	NORMAL = 1,
	TANGENT = 2,
	TEXCOORD = 3,
	COLOR = 4
};

enum class ShaderType
{
	VERTEX_SHADER = GL_VERTEX_SHADER, FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
};

class Shader
{
public:
	Shader(ShaderType type);
	Shader(const Shader&) = delete;
	~Shader();

	Shader& operator=(Shader&) = delete;

	const Shader& Source(const std::string& src) const;

	const Shader& Compile() const;

	bool IsCompiled() const;

	friend class Program;

private:
	GLuint mShader;
	ShaderType mType;
};

#define DECL_SHADER_TYPE(Name, Type) struct Name : Shader { Name() : Shader(Type) {} };

DECL_SHADER_TYPE(VertexShader, ShaderType::VERTEX_SHADER);
DECL_SHADER_TYPE(FragmentShader, ShaderType::FRAGMENT_SHADER);

class Program
{
public:
	Program();
	Program(const Program&) = delete;
	~Program();

	Program& operator=(Program&) = delete;

	GLint operator()(const std::string& attribute) const;
	GLint operator[](const std::string& uniform) const;

	void BindAttribute(VertexAttributes attrib, const std::string& attribute) const;

	const Program& AttachShader(const Shader& shader) const;
	const Program& DetachShader(const Shader& shader) const;

	const Program& Link() const;
	void Use() const;
	static void UseNone();

	bool IsLinked() const;

	// for debugging purposes
	GLint temp()
	{
		return mProgram;
	}

	friend struct Uniform;

private:
	GLuint mProgram;
	mutable std::map<std::string, GLint> mAttributes;
	mutable std::map<std::string, GLint> mUniforms;

	static GLuint msCurrentlyInUse;
};

#include "Math.h"

struct Uniform
{
	static void Bind(const Program& program, const std::string& name, GLint value);
	static void Bind(const Program& program, const std::string& name, GLfloat value);
	static void Bind(const Program& program, const std::string& name, const Matrix4& value);
	static void Bind(const Program& program, const std::string& name, const Vector3& value);
	static void Bind(const Program& program, const std::string& name, const Vector4& value);
};

#include "Manager.h"

DECLARE_RESOURCE_(Shader, Program);

struct ShaderLoader
{
	static bool Load(ShaderResource** resource, Handle handle, const std::string& filename1, const std::string& filename2);
};

DECLARE_MANAGER(Shader);

#endif /* SHADER_H_ */
