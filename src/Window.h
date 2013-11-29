#ifndef WINDOW_H_
#define WINDOW_H_

#include "Context.h"
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

	void SetRenderer(Renderer* renderer);

	int GetWidth() const;
	int GetHeight() const;

	// use internal use
	Context* GetContext() const;

private:
	int mHandle;
	int mWidth, mHeight;
	Context* mContext;
	Renderer* mRenderer;
};

#endif /* WINDOW_H_ */
