#include "Mesh.h"

#include <cassert>
#include <sstream>

ResourceManager<MeshResource, MeshLoader> MeshManager;

void LoadMesh(std::istream& input, std::vector<Vertice>& vertices, std::vector<TexCoords>& uvs, std::vector<Normal>& normals, std::vector<Face>& faces)
{
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

void Mesh::Load(std::istream& input)
{
	if (mVertexArrayId == 0)
		glGenVertexArrays(1, &mVertexArrayId);
	checkOpenGLError("Failed to generate VAO");
	assert(mVertexArrayId != 0);
	glBindVertexArray(mVertexArrayId);

	std::vector<Vertice> vertices;
	std::vector<TexCoords> uvs;
	std::vector<Normal> normals;
	std::vector<Face> faces;

	LoadMesh(input, vertices, uvs, normals, faces);
	mCount = faces.size() * 3;

	for (auto& f : faces)
	{
		if (uvs.size() > 0)
		{
			VertexAttrib v0(vertices[f.v[0] - 1], normals[f.vn[0] - 1], uvs[f.vt[0] - 1]);
			VertexAttrib v1(vertices[f.v[1] - 1], normals[f.vn[1] - 1], uvs[f.vt[1] - 1]);
			VertexAttrib v2(vertices[f.v[2] - 1], normals[f.vn[2] - 1], uvs[f.vt[2] - 1]);
			mVertices.push_back(v0);
			mVertices.push_back(v1);
			mVertices.push_back(v2);
		}
		else
		{
			VertexAttrib v0(vertices[f.v[0] - 1], normals[f.vn[0] - 1]);
			VertexAttrib v1(vertices[f.v[1] - 1], normals[f.vn[1] - 1]);
			VertexAttrib v2(vertices[f.v[2] - 1], normals[f.vn[2] - 1]);
			mVertices.push_back(v0);
			mVertices.push_back(v1);
			mVertices.push_back(v2);
		}
	}

	glGenBuffers(1, &mVertexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(VertexAttrib), &mVertices[0], GL_STATIC_DRAW);
	checkOpenGLError("Failed to copy vertex data");

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttrib), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttrib), (GLvoid*) sizeof(Vertice));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttrib), (GLvoid*)(sizeof(Vertice) + sizeof(Normal)));
	checkOpenGLError("Failed to enable vertex attributes");

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int Mesh::GetCount() const
{
	return mCount;
}

void Mesh::Bind() const
{
	assert(mVertexArrayId != 0 && "ERROR: must have a valid Vertex Array");
	glBindVertexArray(mVertexArrayId);
}

void Mesh::Unbind() const
{
	glBindVertexArray(0);
}
