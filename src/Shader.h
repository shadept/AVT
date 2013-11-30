#ifndef SHADER_H_
#define SHADER_H_

#include "OpenGL.h"

#include <map>

enum class VertexAttributes
{
	POSITION = 0,
	NORMAL = 1,
	TEXCOORD = 2,
	COLOR = 3
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
	static void Bind(const Program& program, const std::string& name, GLfloat value);
	static void Bind(const Program& program, const std::string& name, const Matrix4& value);
	static void Bind(const Program& program, const std::string& name, const Vector3& value);
};

#include "Manager.h"

struct ShaderResource: public Resource<Program>
{
public:
	ShaderResource(Handle handle, const std::string& filename) : Resource(handle, filename) {};

	friend struct ShaderLoader;
};

#include <fstream>

std::string readFile(const std::string& filename);

struct ShaderLoader
{
	static bool Load(ShaderResource** resource, Handle handle, const std::string& filename)
	{
		*resource = new ShaderResource(handle, filename);
		Program* shader = new Program();
		(*resource)->mRaw = shader;

		VertexShader vs;
		vs.Source(readFile(filename+".vert"));
		vs.Compile();

		FragmentShader fs;
		fs.Source(readFile(filename+".frag"));
		fs.Compile();

		shader->AttachShader(vs).AttachShader(fs);
		shader->BindAttribute(VertexAttributes::POSITION, "in_Position");
		shader->BindAttribute(VertexAttributes::NORMAL, "in_Normal");
		shader->Link();

		return true;
	}
};

extern ResourceManager<ShaderResource, ShaderLoader> ShaderManager;

#endif /* SHADER_H_ */
