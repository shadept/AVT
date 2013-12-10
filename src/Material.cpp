#include "Material.h"

#include <fstream>
#include <sstream>

IMPLEMENT_MANAGER(Material);

MaterialParser::MaterialDefinition::MaterialDefinition()
{
	Reset();
}

void MaterialParser::MaterialDefinition::Reset()
{
	Name = "";
	AmbientMap = "";
	DiffuseMap = "";
	SpecularMap = "";
	SpecularCoeffMap = "";
	TransparencyMap = "";
	BumpMap  = "";
	Ambient = { 0.2f, 0.2f, 0.2f};
	Diffuse = { 0.8f, 0.8f, 0.8f};
	Specular = { 1.0f, 1.0f, 1.0f};
	SpecularCoeff = 0.0f;
	Transparency = 1.0f;
	RefractionIndex = 1.0f;
	IlluminationModel = 2;
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
			{
				materials.push_back(material);
				material.Reset();
			}
			ss >> material.Name;
			count++;
		}
		else if (tag == "Ka")
		{
			ss >> material.Ambient.X >> material.Ambient.Y >> material.Ambient.Z;
		}
		else if (tag == "Kd")
		{
			ss >> material.Diffuse.X >> material.Diffuse.Y >> material.Diffuse.Z;
		}
		else if (tag == "Ks")
		{
			ss >> material.Specular.X >> material.Specular.Y >> material.Specular.Z;
		}
		else if (tag == "Ns")
		{
			ss >> material.SpecularCoeff;
		}
		else if (tag == "d" /*|| tag == "Tr"*/)
		{
			ss >> material.Transparency;
		}
		else if (tag == "Ni")
		{
			ss >> material.RefractionIndex;
		}
		else if (tag == "illum")
		{
			ss >> material.IlluminationModel;
		}
		else if (tag == "map_Ka")
		{
			ss >> material.AmbientMap;
		}
		else if (tag == "map_Kd")
		{
			ss >> material.DiffuseMap;
		}
		else if (tag == "map_Ks")
		{
			ss >> material.SpecularMap;
		}
		else if (tag == "map_Bump" || "bump")
		{
			ss >> material.BumpMap;
		}
		else
		{
			Logger::Error << "Invalid tag " << tag << " in material definition" << Logger::endl;
		}
	}

	if (count > 0)
	{
		// insert last material
		materials.push_back(material);
	}
}

bool MaterialLoader::Load(MaterialResource** resource, Handle handle, const std::string& filename)
{
	// no need to actually set resource value

	std::ifstream file(filename);
	if (file.is_open())
	{
		std::vector<MaterialParser::MaterialDefinition> definitions;
		MaterialParser::Load(definitions, file);

		if (definitions.size() == 0)
			return false;

		for (auto& def : definitions)
		{
			Handle h;
			Material* m = new Material();
			m->mAmbient = def.Ambient;
			m->mDiffuse = def.Diffuse;
			m->mSpecular = def.Specular;
			m->mShininess = def.SpecularCoeff;
			m->mTransparency = def.Transparency;
			m->mRefraction = def.RefractionIndex;

			if (!def.AmbientMap.empty())
			{
				h = TextureManager.Load(def.AmbientMap, "./textures/" + def.AmbientMap);
				m->mAmbientMap = TextureManager[h]->GetRaw();
			}
			if (!def.DiffuseMap.empty())
			{
				h = TextureManager.Load(def.DiffuseMap, "./textures/" + def.DiffuseMap);
				m->mDiffuseMap = TextureManager[h]->GetRaw();
			}
			if (!def.SpecularMap.empty())
			{
				h = TextureManager.Load(def.SpecularMap, "./textures/" + def.SpecularMap);
				m->mSpecularMap = TextureManager[h]->GetRaw();
			}
			if (!def.BumpMap.empty())
			{
				h = TextureManager.Load(def.BumpMap, "./textures/" + def.BumpMap);
				m->mBumpMap = TextureManager[h]->GetRaw();
			}

			MaterialManager.Add(def.Name, m);
		}

		return true;
	}

	return false;
}
