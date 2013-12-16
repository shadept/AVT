#include "Mesh.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Shader.h"

IMPLEMENT_MANAGER(Mesh);

void MeshParser::Load(Mesh* mesh, std::istream& input)
{
	std::vector<Vertice> vertices;
	std::vector<TexCoords> uvs;
	std::vector<Normal> normals;
	std::vector<Normal> tangents;
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
			uv.v = 1.0f - uv.v; // hack
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

Mesh::Vertex::Vertex(Vertice v, TexCoords tc)
{
	position.set(v.x, v.y, v.z);
	U = tc.u;
	V = tc.v;
}

Mesh::Vertex::Vertex(Vertice v, Normal n)
{
	position.set(v.x, v.y, v.z);
	normal.set(n.x, n.y, n.z);
}

Mesh::Vertex::Vertex(Vertice v, Normal n, TexCoords tc)
{
	position.set(v.x, v.y, v.z);
	normal.set(n.x, n.y, n.z);
	U = tc.u;
	V = tc.v;
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
	// generate tangent data
	for (unsigned int i = 0; i < vertexData.size(); i += 3)
	{
		Vertex& v0 = vertexData[i];
		Vertex& v1 = vertexData[i+1];
		Vertex& v2 = vertexData[i+2];

		Vector3 e1 = v1.position - v0.position;
		Vector3 e2 = v2.position - v0.position;

		float dU1 = v1.U - v0.U;
		float dV1 = v1.V - v0.V;
		float dU2 = v2.U - v0.U;
		float dV2 = v2.V - v0.V;

		float f = 1.0f / (dU1 * dV2 - dU2 * dV1);

		Vector3 tangent;
		tangent.X = f * (dV2 * e1.X - dV1 * e2.X);
		tangent.Y = f * (dV2 * e1.Y - dV1 * e2.Y);
		tangent.Z = f * (dV2 * e1.Z - dV1 * e2.Z);

		v0.tangent += tangent;
		v1.tangent += tangent;
		v2.tangent += tangent;
	}

	for (auto& vert : vertexData)
		vert.tangent.normalize();

	assert(mVertexArrayId == 0 && "mesh data already loaded");
	glGenVertexArrays(1, &mVertexArrayId);
	glBindVertexArray(mVertexArrayId);

	glGenBuffers(1, &mVertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
	mCount = vertexData.size();
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), &vertexData[0], GL_STATIC_DRAW);
	checkOpenGLError("Failed to copy vertex data");

	glEnableVertexAttribArray((GLuint)VertexAttributes::POSITION);
	glVertexAttribPointer((GLuint)VertexAttributes::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray((GLuint)VertexAttributes::NORMAL);
	glVertexAttribPointer((GLuint)VertexAttributes::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) sizeof(Vector3));

	glEnableVertexAttribArray((GLuint)VertexAttributes::TANGENT);
	glVertexAttribPointer((GLuint)VertexAttributes::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (2 * (sizeof(Vector3))));

	glEnableVertexAttribArray((GLuint)VertexAttributes::TEXCOORD);
	glVertexAttribPointer((GLuint)VertexAttributes::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (3 * (sizeof(Vector3))));
	checkOpenGLError("Failed to enable vertex attributes");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// find center of mass
	for (auto& v : vertexData)
		mCenterOfMass += v.position;

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
