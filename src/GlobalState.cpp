#include "GlobalState.h"

std::shared_ptr<GlobalState> GlobalState::Defaults[MAX_STATE];
bool GlobalState::msIsInitialized = false;

GlobalState::~GlobalState()
{
}

GlobalState::GlobalState()
{
	InitializeDefaults();
}

void GlobalState::InitializeDefaults()
{
	if (!msIsInitialized)
	{
		msIsInitialized = true; // avoid infinite recursion :-)
		Defaults[CULL].reset(new CullState);
		Defaults[DEPTH].reset(new DepthState);
		Defaults[WIREFRAME].reset(new WireframeState);
	}
}

CullState::CullState()
{
	Enabled = true;
	CullFace = CF_BACK;
	FrontFace = FF_CCW;
}

DepthState::DepthState()
{
	Enabled = true;
	Writable = true;
	Compare = CF_LEQUAL;
}

WireframeState::WireframeState()
{
	Enabled = false;
}
