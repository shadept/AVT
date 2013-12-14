#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "Spatial.h"

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"

class Geometry: public Spatial
{
public:
	Geometry(const std::string& name, const Mesh* mesh = NULL, const Material* material = NULL);
	virtual ~Geometry() {};

	virtual void OnDraw(Renderer& renderer) const;

	void SetMaterial(const Material* material) { mMaterial = material; }
	const Material* GetMaterial() const { return mMaterial; }

	void SetMesh(const Mesh* mesh) { mMesh = mesh; }
	const Mesh* GetMesh() const { return mMesh; }

	void SetShader(const Program* shader) { mShader = shader; }
	const Program* GetShader() const { return mShader; }

private:
	const Mesh* mMesh = NULL;
	const Material* mMaterial = NULL;
	const Program* mShader = NULL;

public:
	Vector3 color;
};

#endif /* GEOMETRY_H_ */
