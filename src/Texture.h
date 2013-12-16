#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OpenGL.h"

#include <vector>

class Texture;

class Texture
{
public:
	Texture();
	~Texture();

	void Load(const std::vector<unsigned char>& image, int width, int height);
	void Load(const std::string& filename);

	GLuint GetId() const;
	unsigned GetWidth() const;
	unsigned GetHeight() const;

	void Bind(int unit) const;
	void Unbind() const;

private:
	GLuint mTextureId = 0;
	unsigned mWidth = 0;
	unsigned mHeight = 0;
};

class Cubemap
{
public:
	Cubemap();
	~Cubemap();

	void Create(int width, int height);
	void Load(const std::string& filename);

	void Bind(int unit) const;
	void Unbind() const;

	GLuint ID() const { return mTextureId; }

private:
	GLuint mTextureId = 0;
};

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	void Create(int width, int height);

	void BindToWrite();
	void BindToRead(int unit);
	void Unbind();

private:
	GLuint mTextureId = 0;
	GLuint mFramebuffer = 0;
	GLuint mDepthBuffer = 0;
	unsigned mWidth = 0;
	unsigned mHeight = 0;
};

class RenderTargetCube
{
public:
	RenderTargetCube();
	~RenderTargetCube();

	void Create(int width, int height);

	void BindToWrite(int face);
	void BindToRead(int unit);
	void Unbind();

private:
	GLuint mTextureId = 0;
	GLuint mFramebuffer = 0;
	GLuint mDepthBuffer = 0;
	unsigned mWidth = 0;
	unsigned mHeight = 0;
};

#include "Manager.h"

DECLARE_RESOURCE_AND_LOADER(Texture);
DECLARE_MANAGER(Texture);

#endif /* TEXTURE_H_ */
