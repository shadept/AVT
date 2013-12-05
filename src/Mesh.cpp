#include "Mesh.h"

#include <cassert>
#include <fstream>
#include <sstream>

IMPLEMENT_MANAGER(Mesh);

void MeshParser::Load(Mesh* mesh, std::istream& input)
{
	std::vector<Vertice> vertices;
	std::vector<TexCoords> uvs;
	std::vector<Normal> normals;
	std::vector<Face> faces;
	std::string line;

	while (std::getline(input, line))
	{
		std::stringstream ss(line);
		std::string tag;
		ss >> tag;
		if (tag[0] == '#')
		{
			continue;
		}
		else if (tag == "v")
		{
			Vertice v;
			ss >> v.x >> v.y >> v.z;
			vertices.push_back(v);
		}
		else if (tag == "vt")
		{
			TexCoords uv;
			ss >> uv.u >> uv.v;
			uvs.push_back(uv);
		}
		else if (tag == "vn")
		{
			Normal n;
			ss >> n.x >> n.y >> n.z;
			normals.push_back(n);
		}
		else if (tag[0] == 'f')
		{
			Face f;
			char slash;
			for (int i = 0; i < 3; i++)
			{
				ss >> f.v[i] >> slash;
				if (uvs.size() > 0)
					ss >> f.vt[i];
				else
					f.vt[i] = 0;
				ss >> slash;
				if (normals.size() > 0)
					ss >> f.vn[i];
				else
					f.vn[i] = 0;
			}
			faces.push_back(f);
		}
	}

	std::vector<Mesh::Vertex> vertexData;
	vertexData.reserve(faces.size() * 3);
	for (auto& f : faces)
	{
		if (uvs.size() > 0)
		{
			Mesh::Vertex v0(vertices[f.v[0] - 1], normals[f.vn[0] - 1], uvs[f.vt[0] - 1]);
			Mesh::Vertex v1(vertices[f.v[1] - 1], normals[f.vn[1] - 1], uvs[f.vt[1] - 1]);
			Mesh::Vertex v2(vertices[f.v[2] - 1], normals[f.vn[2] - 1], uvs[f.vt[2] - 1]);
			vertexData.push_back(v0);
			vertexData.push_back(v1);
			vertexData.push_back(v2);
		}
		else
		{
			Mesh::Vertex v0(vertices[f.v[0] - 1], normals[f.vn[0] - 1]);
			Mesh::Vertex v1(vertices[f.v[1] - 1], normals[f.vn[1] - 1]);
			Mesh::Vertex v2(vertices[f.v[2] - 1], normals[f.vn[2] - 1]);
			vertexData.push_back(v0);
			vertexData.push_back(v1);
			vertexData.push_back(v2);
		}
	}

	mesh->Load(vertexData);
}

Mesh::Vertex::Vertex(Vertice v, Normal n)
{
	X = v.x;
	Y = v.y;
	Z = v.z;
	NX = n.x;
	NY = n.y;
	NZ = n.z;
	U = 0;
	V = 0;
}

Mesh::Vertex::Vertex(Vertice v, Normal n, TexCoords t)
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

Mesh::Mesh() :
		mVertexArrayId(0), mVertexBufferId(0), mCount(0)
{
}

Mesh::~Mesh()
{
	glBindVertexArray(0);
	if (mVertexArrayId != 0)
		glDeleteVertexArrays(1, &mVertexArrayId);
	mVertexArrayId = 0;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (mVertexBufferId != 0)
		glDeleteBuffers(1, &mVertexBufferId);
	mVertexBufferId = 0;
}

void Mesh::Load(std::vector<Vertex> vertexData)
{
	assert(mVertexArrayId == 0 && "mesh data already loaded");
	glGenVertexArrays(1, &mVertexArrayId);
	glBindVertexArray(mVertexArrayId);

	glGenBuffers(1, &mVertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
	mCount = vertexData.size();
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), &vertexData[0], GL_STATIC_DRAW);
	checkOpenGLError("Failed to copy vertex data");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) sizeof(Vertice));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(Vertice) + sizeof(Normal)));
	checkOpenGLError("Failed to enable vertex attributes");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// find center of mass
	for (auto& v : vertexData)
		mCenterOfMass += Vector3(v.X, v.Y, v.Z);

	mCenterOfMass = mCenterOfMass * (1.0f / mCount);
}

int Mesh::GetCount() const
{
	return mCount;
}

Vector3 Mesh::GetCenterOfMass() const
{
	return mCenterOfMass;
}

void Mesh::Bind() const
{
	assert(mVertexArrayId != 0 && "mesh must have a valid Vertex Array");
	glBindVertexArray(mVertexArrayId);
}

void Mesh::Unbind() const
{
	glBindVertexArray(0);
}

bool MeshLoader::Load(MeshResource** resource, Handle handle, const std::string& filename)
{
	*resource = new MeshResource(handle, filename);

	std::ifstream file(filename);
	if (file.is_open())
	{
		(*resource)->mRaw = new Mesh();
		MeshParser::Load((*resource)->mRaw, file);
		return true;
	}
	return false;
}
