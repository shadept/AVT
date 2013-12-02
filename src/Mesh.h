#ifndef MESH_H_
#define MESH_H_

#include <iostream>
#include <vector>

#include "OpenGL.h"

struct Vertice
{
	float x, y, z;
};

struct TexCoords
{
	float u, v;
};

typedef Vertice Normal;

struct Face
{
	unsigned int v[3];
	unsigned int vt[3];
	unsigned int vn[3];
};

struct VertexAttrib
{
	VertexAttrib(Vertice v, Normal n)
	{
		X = v.x;
		Y = v.y;
		Z = v.z;
		NX = n.x;
		NY = n.y;
		NZ = n.z;
		U = V = 0.0f;
	}

	VertexAttrib(Vertice v, Normal n, TexCoords t)
	{
		X = v.x;
		Y = v.y;
		Z = v.z;
		NX = n.x;
		NY = n.y;
		NZ = n.z;
		U = t.u;
		V = t.v;
	}

	float X, Y, Z;
	float NX, NY, NZ;
	float U, V;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Load(std::istream& input);

	int GetCount() const;

	void Bind() const;
	void Unbind() const;

private:
	GLuint mVertexArrayId, mVertexBufferId;
	std::vector<VertexAttrib> mVertices;
	int mCount;
};

#include "Manager.h"

DECLARE_RESOURCE_AND_LOADER(Mesh);
DECLARE_MANAGER(Mesh);

#endif /* MESH_H_ */
