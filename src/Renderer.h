#ifndef RENDERER_H_
#define RENDERER_H_

#include "OpenGL.h"
#include "Shader.h"

#include <list>


// Forward delcaration
class Spatial;
class Light;
struct Material;
class Node;
class Geometry;
class Camera;
class Texture;
class Cubemap;
class RenderTarget;
class RenderTargetCube;

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
	void BuildEnvironmentMap(Node* scene, const Vector3& center);

	////////////////////////// DRAWING SCENE //////////////////////////////

	enum DebugShader {
		NONE,
		NORMALS,
		DEPTH,
		MAX_DEBUG
	};

	void SetDebug(DebugShader type);
	void SetCamera(Camera* camera);
	void SetGlobalShader(const Program* shader);
	void SetLighting(bool on);

	void DrawScene(Node* scene);
	void Draw(Geometry* geometry, bool overrideTransparency = false);
	void Draw(const Light* light);

private:
	void Bind(const Light* light, const Program* shader);
	void Bind(const Material* material, const Program* shader);

	// Render specific variables
	Material* mDefaultMaterial;
	Program* mDebugShaderNormals, * mDebugShaderDepth;
	Program* mNoLightShader;
	Program* mRealisticShader;
	Program* mSpecularShader;
	Texture* mWhiteTexture;
	RenderTargetCube* mEnvironmentMap;
	Cubemap* mSkybox;

	// Other variables
	const Program* mShader, *mPickingShader;
	Camera* mCamera;
	const Light* mLight;
	bool mPicking, mLighting;
	bool mDebugging;

	std::vector<Geometry*> mTransparentList;

};

#endif /* RENDERER_H_ */
