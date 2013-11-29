#ifndef GLOBALSTATE_H_
#define GLOBALSTATE_H_

#include <memory>
#include "GL/glew.h"

class GlobalState
{
public:
	virtual ~GlobalState();

	enum Type
	{
		DEPTH,
		CULL,
		WIREFRAME,
		MAX_STATE
	};

	virtual Type GetType() const = 0;

	static std::shared_ptr<GlobalState> Defaults[MAX_STATE];

	static void InitializeDefaults();

protected:
	GlobalState();

private:
	static bool msIsInitialized;
};

class CullState : public GlobalState
{
public:
	CullState();

	virtual Type GetType() const { return CULL; }

	enum CullFace
	{
		CF_BACK,
		CF_FRONT,
		CF_FRONT_AND_BACK,
		CF_MAX_STATE
	};

	enum FrontFace
	{
		FF_CCW,
		FF_CW,
		FF_MAX_STATE
	};

	bool Enabled;
	CullFace CullFace;
	FrontFace FrontFace;
};

class DepthState : public GlobalState
{
public:
	DepthState();

	virtual Type GetType() const { return DEPTH; }

	enum CompareFunc
	{
		CF_NEVER,
		CF_LESS,
		CF_EQUAL,
		CF_LEQUAL,
		CF_GREATER,
		CF_NOTEQUAL,
		CF_GEQUAL,
		CF_ALWAYS,
		CF_MAX_STATE
	};

	bool Enabled;
	bool Writable;
	CompareFunc Compare;
};

class WireframeState : public GlobalState
{
public:
	WireframeState();

	virtual Type GetType() const { return WIREFRAME; }

	bool Enabled;
};



#endif /* GLOBALSTATE_H_ */
