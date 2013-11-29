#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "OpenGL.h"
#include "GlobalState.h"
#include "Renderer.h"

class Context
{
public:
	Context();
	~Context();

	void Draw() const;

	void SetDefaultState();
	void SetState(const CullState* state);
	void SetState(const DepthState* state);
	void SetState(const WireframeState* state);
	void SetRenderer(Renderer* renderer);

private:
	Renderer* mRenderer;
};

#endif /* CONTEXT_H_ */
