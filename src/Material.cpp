#include "Material.h"

#include <sstream>

IMPLEMENT_MANAGER(Material);

MaterialParser::MaterialDefinition::MaterialDefinition()
{
	Ambient[0] = Ambient[1] = Ambient[2];
	Diffuse[0] = Diffuse[1] = Diffuse[2];
	Specular[0] = Specular[1] = Specular[2];
	SpecularCoeff = Transparency = 0.0f;
	IlluminationModel = 0;
}

void MaterialParser::Load(std::vector<MaterialDefinition>& materials, std::istream& input)
{
	std::string line;
	MaterialDefinition material;
	int count = 0;

	while (std::getline(input, line))
	{
		std::stringstream ss(line);
		std::string tag;
		ss >> tag;
		if (tag[0] == '#')
		{
			continue;
		}
		else if (tag == "newmtl")
		{
			if (count > 0)
				materials.push_back(material);
			ss >> material.Name;
			count++;
		}
		else if (tag == "Ka")
		{
			ss >> material.Ambient[0] >> material.Ambient[1] >> material.Ambient[2];
		}
		else if (tag == "Kd")
		{
			ss >> material.Diffuse[0] >> material.Diffuse[1] >> material.Diffuse[2];
		}
		else if (tag == "Ks")
		{
			ss >> material.Specular[0] >> material.Specular[1] >> material.Specular[2];
		}
		else if (tag == "Ns")
		{
			ss >> material.SpecularCoeff;
		}
	}
}

Material::Material() :
		mAmbient(0.1f, 0.1f, 0.1f), mDiffuse(1.0f, 1.0f, 1.0f), mSpecular(0.9f, 0.9f, 0.9f), mTexture(0), mShininess(0.0f)
{
}

Material::~Material()
{
}

bool MaterialLoader::Load(MaterialResource** resource, Handle handle, const std::string& filename)
{
	*resource = new MaterialResource(handle, filename);
	(*resource)->mRaw = new Material();

	return true;
}
