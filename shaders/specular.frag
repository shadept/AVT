#version 330 core

in vec3 exVertex;
in vec3 exNormal;
in vec2 exTexCoords;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

// Light Properties
uniform bool EnableLighting;
uniform vec3 LightPosition;
uniform vec3 LightAmbientColor;
uniform vec3 LightDiffuseColor;
uniform vec3 LightSpecularColor;

// Material Properties
uniform bool MaterialHasTexture;
uniform sampler2D MaterialTexture;
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialDiffuseColor;
uniform vec3 MaterialSpecularColor;
uniform float MaterialShininess;
uniform float MaterialTransparency;

out vec4 FragmentColor;

void main(void)
{
	vec3 V = exVertex;
	vec3 N = normalize(exNormal);
	//vec3 Lpos = vec3(ViewMatrix * vec4(LightPosition, 1.0));
	vec3 Lpos = LightPosition;
	vec3 L = Lpos - V;
	float Ldist = length(L);
	L = normalize(L);
	vec3 E = normalize(-V);
	vec3 H = normalize(L + E);

	float NdotH = clamp(dot(N, H), 0.0, 1.0);
	float specularTerm = pow(NdotH, MaterialShininess);
	//float angle = acos(NdotH);
	//float specularTerm = exp(-pow(angle / MaterialShininess, 2.0));
	vec3 specular = LightSpecularColor * MaterialSpecularColor * specularTerm;

	vec3 color = specular;

	float transparency = length(color.rgb);
	FragmentColor = vec4(color, transparency);
}