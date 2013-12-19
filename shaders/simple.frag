#version 330 core

in vec3 exVertex;
in vec3 exNormal;
in vec2 exTexCoords;

out vec4 FragmentColor;

struct Environment
{
	samplerCube map;
	samplerCube skybox;
	vec3 ambient;
};

struct Material
{
	sampler2D ambientTexture;
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	sampler2D normalTexture;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float transparency;		// [0 .. 1]
	float refractionIndex;	// [1 .. 2] ?
	float reflectivity;		// [0 .. 1]
};

uniform Environment environment;
uniform Material material;

void main(void)
{
	vec3 ambient = material.ambient * texture(material.ambientTexture, exTexCoords).rgb;
	vec3 diffuse = material.diffuse * texture(material.diffuseTexture, exTexCoords).rgb;

	vec3 color = environment.ambient + ambient + diffuse;
	FragmentColor = vec4(color, material.transparency);
}