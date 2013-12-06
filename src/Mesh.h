#ifndef MESH_H_
#define MESH_H_

#include <iostream>
#include <vector>

#include "OpenGL.h"
#include "Vector3.h"

class Mesh;

struct MeshParser
{
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

	static void Load(Mesh* mesh, std::istream& input);
};

class Mesh
{
public:
	typedef MeshParser::Vertice Vertice;
	typedef MeshParser::Normal Normal;
	typedef MeshParser::Normal Tangent;
	typedef MeshParser::TexCoords TexCoords;

	struct Vertex
	{
		Vertex(Vertice v, Normal n);
		Vertex(Vertice v, Normal n, TexCoords tc);

//		float X, Y, Z;
//		float NX, NY, NZ;
//		float TX, TY, TZ;
//		float U, V;
		Vector3 position;
		Vector3 normal;
		Vector3 tangent = {0.0f, 0.0f, 0.0f};
		float U = 0.0f;
		float V = 0.0f;
	};

	Mesh();
	~Mesh();

	void Load(std::vector<Vertex> vertexData);

	int GetCount() const;
	Vector3 GetCenterOfMass() const;

	void Bind() const;
	void Unbind() const;

private:
	GLuint mVertexArrayId, mVertexBufferId;
	Vector3 mCenterOfMass;
//	std::vector<Vertex> mVertices;
	int mCount;
};

#include "Manager.h"

DECLARE_RESOURCE_AND_LOADER(Mesh);
DECLARE_MANAGER(Mesh);

#endif /* MESH_H_ */
