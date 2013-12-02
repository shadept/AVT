#include "Texture.h"

#include "lodepng.h"
#include <cassert>

ResourceManager<TextureResource, TextureLoader> TextureManager;

Texture::Texture() :
		mTextureId(0)
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
	size_t u2 = 1;
	while (u2 < mWidth)
		u2 *= 2;
	size_t v2 = 1;
	while (v2 < mHeight)
		v2 *= 2;
	// Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
//	double u3 = (double)width / u2;
//	double v3 = (double)height / v2;
	std::vector<unsigned char> image2(u2 * v2 * 4);
	for (size_t y = 0; y < mHeight; y++)
		for (size_t x = 0; x < mWidth; x++)
			for (size_t c = 0; c < 4; c++)
			{
				image2[4 * u2 * y + 4 * x + c] = image[4 * mWidth * y + 4 * x + c];
			}

//	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &mTextureId);
	assert(mTextureId != 0 && "Failed to create opengl texture");

	glBindTexture(GL_TEXTURE_2D, mTextureId);
	checkOpenGLError("Failed to bind texture");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);
	checkOpenGLError("Failed to load texture");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	checkOpenGLError("Failed to set texture parameters");

	glBindTexture(GL_TEXTURE_2D, 0);
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

void Texture::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	checkOpenGLError("Failed to bind texture");
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	checkOpenGLError("Failed to unbind texture");
}
