#ifndef WINDOW_H_
#define WINDOW_H_

#include "Renderer.h"

class Window
{
public:
	Window(const std::string& title, int width = 640, int height = 480);
	~Window();

	// causes windows to redisplay
	void Touch() const;

	void Resize(int width, int height);
	void SetTitle(const std::string& title);

	int GetWidth() const;
	int GetHeight() const;

private:
	int mHandle;
	int mWidth, mHeight;
};

#endif /* WINDOW_H_ */
