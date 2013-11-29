#include "Context.h"

#include <cassert>
#include <iostream>

GLenum cullFace[CullState::CF_MAX_STATE] =
{
	GL_BACK, GL_FRONT, GL_FRONT_AND_BACK
};

GLenum frontFace[CullState::FF_MAX_STATE] =
{
	GL_CCW, GL_CW
};

GLenum compareFunc[DepthState::CF_MAX_STATE] =
{
	GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS
};

// FIXME GLUT is kinda ugly but we good enough for now
Context::Context() : mRenderer(nullptr)
{
	// fake arguments for glut
	static int argc = 1;
	static char* argv[] = { "AVT" };
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//	glewExperimental = GL_TRUE;
//	GLenum result = glewInit();
//	if (result != GLEW_OK)
//	{
//		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	GLenum err_code = glGetError();
//	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << " " << err_code << std::endl;
}

Context::~Context()
{
	mRenderer = nullptr;
}

void Context::Draw() const
{
}

void Context::SetDefaultState()
{
	GlobalState::InitializeDefaults();
	std::cout << "Setting default contexts states" << std::endl;
	SetState(static_cast<CullState*>(GlobalState::Defaults[GlobalState::CULL].get()));
	SetState(static_cast<DepthState*>(GlobalState::Defaults[GlobalState::DEPTH].get()));
	SetState(static_cast<WireframeState*>(GlobalState::Defaults[GlobalState::WIREFRAME].get()));
}

void Context::SetState(const CullState* state)
{
	assert(state != nullptr);
	if (state->Enabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	glCullFace(cullFace[state->CullFace]);
	glFrontFace(frontFace[state->FrontFace]);
	checkOpenGLError("Failed to set CullState");
}

void Context::SetState(const DepthState* state)
{
	assert(state != nullptr);
	if (state->Enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	glDepthMask(state->Writable);
	glDepthFunc(compareFunc[state->Compare]);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	checkOpenGLError("Failed to set DepthState");
}

void Context::SetState(const WireframeState* state)
{
	assert(state != nullptr);
	if (state->Enabled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	checkOpenGLError("Failed to set WireframeState");
}

void Context::SetRenderer(Renderer* renderer)
{
	mRenderer = renderer;
}
