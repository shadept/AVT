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
		Vector3 Ambient; // Ka
		Vector3 Diffuse; // Kd
		Vector3 Specular; // Ks
		float SpecularCoeff; // Ns
		float Transparency; // d or Tr
		float RefractionIndex; // Ni
		int IlluminationModel; // illum (0-no lighting applied, 1-normal lighting, 2-specular)
	};

	static void Load(std::vector<MaterialDefinition>& materials, std::istream& input);

};

struct Material
{
	Vector3 mAmbient = {0.2f, 0.2f, 0.2f};
	Vector3 mDiffuse = {0.8f, 0.8f, 0.8f};
	Vector3 mSpecular = {1.0f, 1.0f, 1.0f};
	Texture* mAmbientMap = nullptr;
	Texture* mDiffuseMap = nullptr;
	Texture* mSpecularMap = nullptr;
	Texture* mBumpMap = nullptr; // or normalmap
	float mShininess = 0.0f;
	float mTransparency = 1.0f;
	float mRefraction = 1.0;
};

#include "Manager.h"

DECLARE_RESOURCE_AND_LOADER(Material);
DECLARE_MANAGER(Material);

#endif /* MATERIAL_H_ */
