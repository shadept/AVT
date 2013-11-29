#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "Spatial.h"

#include "Mesh.h"
#include "Shader.h"

class Geometry: public Spatial
{
public:
	Geometry(const Mesh* mesh = NULL);
	virtual ~Geometry() {};

	virtual void OnDraw(Renderer& renderer) const;

	void SetMesh(const Mesh* mesh) { mMesh = mesh; }
	const Mesh* GetMesh() const { return mMesh; }

private:
	const Mesh* mMesh;

public:
	Vector3 color;
};

#endif /* GEOMETRY_H_ */
