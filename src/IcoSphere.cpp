#include "IcoSphere.h"

int IcoSphere::addVertex(Vertice v, vector<Vertice>& vertices, int& index)
{
	float length = Math::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x /= length; v.y /= length; v.z /= length;
	vertices.push_back(v);
	return index++;
}

// return index of point in the middle of p1 and p2
int IcoSphere::getMiddlePoint(int v1, int v2, vector<Vertice>& vertices, int index, map<Int64, int>& middlePointIndexCache)
{
	// first check if we have it already
	Int64 smallerIndex = v1 < v2 ? v1 : v2;
	Int64 greaterIndex = v1 < v2 ? v2 : v1;
	Int64 key = (smallerIndex << 32) + greaterIndex;

	map<Int64, int>::iterator ret = middlePointIndexCache.find(key);
	if (ret != middlePointIndexCache.end())
	{
		return ret->second;
	}

	// not in cache, calculate it
	Vertice point1 = vertices[v1];
	Vertice point2 = vertices[v2];
	Vertice middle = { (point1.x + point2.x) / 2.0f, (point1.y + point2.y) / 2.0f, (point1.z + point2.z) / 2.0f };

	// add vertex makes sure point is on unit sphere
	int i = addVertex(middle, vertices, index);

	// store it, return index
	middlePointIndexCache[key] = i;
	return i;
}

Mesh* IcoSphere::Create(int recursionLevel)
{
	map<Int64, int>* middlePointIndexCache = new map<Int64, int>();
	vector<Vertice>* vertices = new vector<Vertice>();
	int index = 0;

	// create 12 vertices of a icosahedron
	Real t = (1.0f + Math::sqrt(5.0f)) / 2.0f;

	addVertex(Vertice({ -1.0f, t, 0.0f }), *vertices, index);
	addVertex(Vertice({ 1.0f, t, 0.0f }), *vertices, index);
	addVertex(Vertice({ -1.0f, -t, 0.0f }), *vertices, index);
	addVertex(Vertice({ 1.0f, -t, 0.0f }), *vertices, index);

	addVertex(Vertice({ 0.0f, -1.0f, t }), *vertices, index);
	addVertex(Vertice({ 0.0f, 1.0f, t }), *vertices, index);
	addVertex(Vertice({ 0.0f, -1.0f, -t }), *vertices, index);
	addVertex(Vertice({ 0.0f, 1.0f, -t }), *vertices, index);

	addVertex(Vertice({ t, 0.0f, -1.0f }), *vertices, index);
	addVertex(Vertice({ t, 0.0f, 1.0f }), *vertices, index);
	addVertex(Vertice({ -t, 0.0f, -1.0f }), *vertices, index);
	addVertex(Vertice({ -t, 0.0f, 1.0f }), *vertices, index);


	// create 20 triangles of the icosahedron
	vector<Face>* faces = new vector<Face>();

	// 5 faces around point 0
	faces->push_back(Face({ 0, 11, 5 }));
	faces->push_back(Face({ 0, 5, 1 }));
	faces->push_back(Face({ 0, 1, 7 }));
	faces->push_back(Face({ 0, 7, 10 }));
	faces->push_back(Face({ 0, 10, 11 }));

	// 5 adjacent faces 
	faces->push_back(Face({ 1, 5, 9 }));
	faces->push_back(Face({ 5, 11, 4 }));
	faces->push_back(Face({ 11, 10, 2 }));
	faces->push_back(Face({ 10, 7, 6 }));
	faces->push_back(Face({ 7, 1, 8 }));

	// 5 faces around point 3
	faces->push_back(Face({ 3, 9, 4 }));
	faces->push_back(Face({ 3, 4, 2 }));
	faces->push_back(Face({ 3, 2, 6 }));
	faces->push_back(Face({ 3, 6, 8 }));
	faces->push_back(Face({ 3, 8, 9 }));

	// 5 adjacent faces 
	faces->push_back(Face({ 4, 9, 5 }));
	faces->push_back(Face({ 2, 4, 11 }));
	faces->push_back(Face({ 6, 2, 10 }));
	faces->push_back(Face({ 8, 6, 7 }));
	faces->push_back(Face({ 9, 8, 1 }));


	// refine triangles
	for (int i = 0; i < recursionLevel; i++)
	{
		vector<Face>* faces2 = new vector<Face>();
		for (auto tri : *faces)
		{
			// replace triangle by 4 triangles
			int a = getMiddlePoint(tri.v1, tri.v2, *vertices, index, *middlePointIndexCache);
			int b = getMiddlePoint(tri.v2, tri.v3, *vertices, index, *middlePointIndexCache);
			int c = getMiddlePoint(tri.v3, tri.v1, *vertices, index, *middlePointIndexCache);

			faces2->push_back(Face({ tri.v1, a, c }));
			faces2->push_back(Face({ tri.v2, b, a }));
			faces2->push_back(Face({ tri.v3, c, b }));
			faces2->push_back(Face({ a, b, c }));
		}
		faces = faces2;
	}

	static Mesh::Normal normal = { 0.0f, 1.0f, 0.0f };
	vector<Mesh::Vertex> vertexData;

	for (auto tri : *faces)
	{
		vertexData.push_back({ (*vertices)[tri.v1], normal });
		vertexData.push_back({ (*vertices)[tri.v2], normal });
		vertexData.push_back({ (*vertices)[tri.v3], normal });
	}

	Mesh* m = new Mesh();
	m->Load(vertexData);
	return m;
}
