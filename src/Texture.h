#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OpenGL.h"

class Texture
{
public:
	Texture();
	~Texture();

	void Load(const std::string& filename);

	GLuint GetId() const;
	unsigned GetWidth() const;
	unsigned GetHeight() const;

	void Bind() const;
	void Unbind() const;

private:
	GLuint mTextureId;
	unsigned mWidth = 0;
	unsigned mHeight = 0;
};

#include "Manager.h"

struct TextureResource : public Resource<Texture>
{
public:
	TextureResource(Handle handle, const std::string& filename) : Resource(handle, filename) {};

	friend struct TextureLoader;
};

struct TextureLoader
{
	static bool Load(TextureResource** resource, Handle handle, const std::string& filename)
	{
		*resource = new TextureResource(handle, filename);
		(*resource)->mRaw = new Texture();

		(*resource)->mRaw->Load(filename);

		return true;
	}
};

extern ResourceManager<TextureResource, TextureLoader> TextureManager;

#endif /* TEXTURE_H_ */
