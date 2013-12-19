#ifndef ICOSPHERE_H_
#define ICOSPHERE_H_

#include <map>
using std::map;

#include <vector>
using std::vector;

#include "Vector3.h"
#include "Mesh.h"

typedef long long Int64;

class IcoSphere
{
private:
	typedef Mesh::Vertice Vertice;
	
	struct Face
	{
		int v1, v2, v3;
	};

	// add vertex to mesh, fix position to be on unit sphere, return index
	static int addVertex(Vertice v, vector<Vertice>& vertices, int& index);
	// return index of point in the middle of p1 and p2
	static int getMiddlePoint(int p1, int p2, vector<Vertice>& vertices, int index, map<Int64, int>& middlePointIndexCache);

public:

	static Mesh* Create(int recursionLevel);
};

#endif /* ICOSPHERE_H_ */
