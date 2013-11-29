#ifndef MESH_H_
#define MESH_H_

#include <iostream>
#include <vector>

#include "OpenGL.h"
#include "Manager.h"


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
	}

	float X, Y, Z;
	float NX, NY, NZ;
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
	GLuint mVertexArrayId;
	std::vector<VertexAttrib> mVertices;
	int mCount;
};

typedef Manager<Mesh> MeshManager;

#endif /* MESH_H_ */
