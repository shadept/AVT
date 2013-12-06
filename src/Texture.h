#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "OpenGL.h"

#include <vector>

class Texture;

struct TextureParser
{
	struct TextureDefinition
	{

	};

	static void Load(Texture* mesh, std::istream& input);
};

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

#include "Manager.h"

DECLARE_RESOURCE_AND_LOADER(Texture);
DECLARE_MANAGER(Texture);

#endif /* TEXTURE_H_ */
