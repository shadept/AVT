#version 330 core

in vec3 exVertex;
in vec3 exNormal;
in vec3 exTangent;
in vec2 exTexCoords;

// Matrix Uniforms]
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

// Light Properties
// uniform bool EnableLighting;
uniform vec3 LightPosition;
uniform vec3 LightAmbientColor;
uniform vec3 LightDiffuseColor;
uniform vec3 LightSpecularColor;

// Material Properties
uniform sampler2D MaterialAmbientTexture;
uniform sampler2D MaterialDiffuseTexture;
uniform sampler2D MaterialSpecularTexture;
uniform sampler2D MaterialBumpTexture;
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialDiffuseColor;
uniform vec3 MaterialSpecularColor;
uniform float MaterialShininess;
uniform float MaterialTransparency;

out vec4 FragmentColor;

vec3 AmbientContribution()
{
	vec3 textureContribution = texture(MaterialAmbientTexture, exTexCoords).rgb;
	return LightAmbientColor * MaterialAmbientColor * textureContribution;
}

vec3 DiffuseContribution()
{
	vec3 textureContribution = texture(MaterialDiffuseTexture, exTexCoords).rgb;
	return LightDiffuseColor * MaterialDiffuseColor * textureContribution;
}

vec3 SpecularContribution()
{
	vec3 textureContribution = texture(MaterialSpecularTexture, exTexCoords).rgb;
	return LightSpecularColor * MaterialSpecularColor * textureContribution;
}

void main(void)
{
	vec3 V = exVertex;
	vec3 N = normalize(exNormal);
	// vec3 Lpos = vec3(ViewMatrix * vec4(LightPosition, 1.0));
	vec3 Lpos = LightPosition;
	vec3 L = Lpos - V;
	float Ldist = length(L);
	L = normalize(L);
	vec3 E = normalize(-V);
	vec3 H = normalize(L + E);

	vec3 ambient = AmbientContribution();

	vec3 bumpMapNormal = texture(MaterialBumpTexture, exTexCoords).rgb;
	if (equal(bumpMapNormal, vec3(1.0)) == false)
	{
		vec3 tangent = normalize(exTangent);
		vec3 bitangent = cross(tangent, N);
		bumpMapNormal = normalize(2.0 * bumpMapNormal - vec3(1.0));
		mat3 tangentSpace = mat3(tangent, bitangent, N);
		N = normalize(tangentSpace * bumpMapNormal);
	}

	float NdotL = clamp(dot(N, L), 0.0, 1.0);
	vec3 diffuse = DiffuseContribution() * NdotL;

	float NdotH = clamp(dot(N, H), 0.0, 1.0);
	float angle = acos(NdotH);
	//float specularTerm = pow(NdotH, MaterialShininess);
	float specularTerm = exp(-pow(angle / MaterialShininess, 2.0));
	vec3 specular = SpecularContribution() * specularTerm;

	vec3 color = ambient + (diffuse + specular);
	FragmentColor = vec4(color, MaterialTransparency);
}