#include "Texture.h"

#include "lodepng.h"
#include <cassert>
#include <sstream>

IMPLEMENT_MANAGER(Texture);

Texture::Texture()
{
}

Texture::~Texture()
{
	if (mTextureId != 0)
	{
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
	}
}

void Texture::Load(const std::vector<unsigned char>& image, int width, int height)
{
	glGenTextures(1, &mTextureId);
	assert(mTextureId != 0 && "Failed to create opengl texture");

	glBindTexture(GL_TEXTURE_2D, mTextureId);
	checkOpenGLError("Failed to bind texture");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	checkOpenGLError("Failed to load texture");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkOpenGLError("Failed to set texture parameters");

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Load(const std::string& filename)
{
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, mWidth, mHeight, filename);

	if (error != 0)
	{
		std::cout << "ERROR " << error << ": " << lodepng_error_text(error) << std::endl;
		assert(0 && "Failed to load texture image");
		return;
	}

	// make power of two image
//	size_t u2 = 1;
//	while (u2 < mWidth)
//		u2 *= 2;
//	size_t v2 = 1;
//	while (v2 < mHeight)
//		v2 *= 2;
//	std::vector<unsigned char> image2(u2 * v2 * 4);
//	for (size_t y = 0; y < mHeight; y++)
//		for (size_t x = 0; x < mWidth; x++)
//			for (size_t c = 0; c < 4; c++)
//			{
//				image2[4 * u2 * y + 4 * x + c] = image[4 * mWidth * y + 4 * x + c];
//			}

//	glEnable(GL_TEXTURE_2D);
//	Load(image2, u2, v2);
	Load(image, mWidth, mHeight);
}

GLuint Texture::GetId() const
{
	return mTextureId;
}

unsigned Texture::GetWidth() const
{
	return mWidth;
}

unsigned Texture::GetHeight() const
{
	return mHeight;
}

void Texture::Bind(int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
//	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	checkOpenGLError("Failed to bind texture");
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	checkOpenGLError("Failed to unbind texture");
}

Cubemap::Cubemap()
{
}

Cubemap::~Cubemap()
{
}

void Cubemap::Load(int width, int height)
{
	glGenTextures(1, &mTextureId);
	assert(mTextureId != 0 && "Failed to create opengl texture");

	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
	checkOpenGLError("Failed to bind texture");

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Load(const std::string& filename)
{
    // GL_TEXTURE_CUBE_MAP_POSITIVE_X​
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_X​
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Y​
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y​
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Z​
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z​

	glGenTextures(1, &mTextureId);
	assert(mTextureId != 0 && "Failed to create opengl texture");

	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
	checkOpenGLError("Failed to bind texture");

	// cubemap has six textures
	std::vector<std::vector<unsigned char> > images;

	for(int i = 0; i < 6; i++)
	{
		unsigned width = 0, height = 0;
		std::stringstream ss;
		ss << filename << i << ".png";
		Logger::Debug << "Loading cubemap face " << ss.str() << Logger::endl;
		images.push_back(std::vector<unsigned char>{});
		unsigned error = lodepng::decode(images[i], width, height, ss.str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(images[i])[0]);

		if (error != 0)
		{
			std::cout << "ERROR " << error << ": " << lodepng_error_text(error) << std::endl;
			assert(0 && "Failed to load texture image");
			return;
		}
	}
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	// checkOpenGLError("Failed to load texture");

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	checkOpenGLError("Failed to set texture parameters");

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Bind(int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
//	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
	checkOpenGLError("Failed to unbind texture cubemap");
}

void Cubemap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	checkOpenGLError("Failed to unbind texture cubemap");
}

bool TextureLoader::Load(TextureResource** resource, Handle handle, const std::string& filename)
{
	*resource = new TextureResource(handle, filename);
	(*resource)->mRaw = new Texture();

	(*resource)->mRaw->Load(filename);

	return true;
}
