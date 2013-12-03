#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <string>
#include <vector>

#include "Texture.h"
#include "Vector3.h"

class Material;

struct MaterialParser
{
	struct MaterialDefinition
	{
		MaterialDefinition();

		void Reset();

		std::string Name; // newmtl
		std::string AmbientMap; // map_Ka
		std::string DiffuseMap; // map_Kd
		std::string SpecularMap; // map_Ks
		std::string SpecularCoeffMap; // map_Ns
		std::string TransparencyMap; // map_d or map_Tr
		std::string BumpMap; // map_bump or bump
		float Ambient[3]; // Ka
		float Diffuse[3]; // Kd
		float Specular[3]; // Ks
		float SpecularCoeff; // Ns
		float Transparency; // d or Tr
		float RefractionIndex; // Ni
		int IlluminationModel; // illum (0-no lighting applied, 1-normal lighting, 2-specular)
	};

	static void Load(std::vector<MaterialDefinition>& materials, std::istream& input);

};

class Material
{
public:
	Material();
	virtual ~Material();

public:
	Vector3 mAmbient;
	Vector3 mDiffuse;
	Vector3 mSpecular;
	Texture* mTexture;
	float mShininess;
};

#include "Manager.h"

DECLARE_RESOURCE_AND_LOADER(Material);
DECLARE_MANAGER(Material);

#endif /* MATERIAL_H_ */
