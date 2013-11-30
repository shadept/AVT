#ifndef RENDERER_H_
#define RENDERER_H_

#include "OpenGL.h"
#include "Shader.h"

#include <list>

#include "Material.h"

// Forward delcaration
class Spatial;
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

	void SetCamera(Camera* camera);

	void Bind(const Program* shader);

	void DrawScene(Node* scene);
	void Draw(const Geometry* geometry);

public:
	mutable int _frameCounter;

private:
	const Program* mShader, *mPickingShader;
	Material* mDefaultMaterial;
	Camera* mCamera;
	bool mPicking;

};

#endif /* RENDERER_H_ */
