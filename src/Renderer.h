#ifndef RENDERER_H_
#define RENDERER_H_

#include "OpenGL.h"
#include "Shader.h"

#include <list>

#include "Material.h"

// Forward delcaration
class Spatial;
class Light;
class Material;
class Node;
class Geometry;
class Camera;

class Renderer
{
public:
	Renderer();
	Renderer(const Renderer& o) = delete;
	~Renderer();
	Renderer& operator =(const Renderer& rhs) = delete;

	///////////////////////////////////////////////////////////////////////

	int Pick(Node* node, int x, int y);
	Vector3 Unproject(int x, int y);

	////////////////////////// DRAWING SCENE //////////////////////////////

	void SetDebug(bool on);
	void SetCamera(Camera* camera);
	void SetGlobalShader(const Program* shader);
	void SetLighting(bool on);

	void DrawScene(Node* scene);
	void Draw(Geometry* geometry, bool overrideTransparency = false);
	void Draw(const Light* light);

public:
	mutable int _frameCounter;

private:
	void Bind(const Light* light, const Program* shader);
	void Bind(const Material* material, const Program* shader);
	void Bind(const Program* shader);

	// Render specific variables
	Material* mDefaultMaterial;
	Program* mDebugShader;
	Program* mNoLightShader;
	Program* mRealisticShader;
	Program* mSpecularShader;

	// Other variables
	const Program* mShader, *mPickingShader;
	Camera* mCamera;
	const Light* mLight;
	bool mPicking, mLighting;
	bool mNeedLightUpdate;
	bool mDebugging;

	std::vector<Geometry*> mTransparentList;

};

#endif /* RENDERER_H_ */
