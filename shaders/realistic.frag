#version 330 core

in vec3 exVertex;
in vec3 exNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

// Light Properties
uniform vec3 LightPosition;
uniform vec3 LightAmbientColor;
uniform vec3 LightDiffuseColor;
uniform vec3 LightSpecularColor;

// Material Properties
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialDiffuseColor;
uniform vec3 MaterialSpecularColor;
uniform float MaterialShininess;

out vec4 FragmentColor;

void main(void)
{
	vec3 V = exVertex;
	vec3 N = normalize(exNormal);
	vec3 Lpos = vec3(ViewMatrix * vec4(LightPosition, 1.0));
	//vec3 Lpos = LightPosition;
	vec3 L = Lpos - V;
	float Ldist = length(L);
	L = normalize(L);
	vec3 E = normalize(-V);
	vec3 H = normalize(L + E);

	vec3 ambient = LightAmbientColor * MaterialAmbientColor;

	float NdotL = clamp(dot(N, L), 0.0, 1.0);
	vec3 diffuse = LightDiffuseColor * MaterialDiffuseColor * NdotL;
	//vec3 diffuse = (0.5 + 0.5 * N) * MaterialDiffuseColor * NdotL;

	float NdotH = clamp(dot(N, H), 0.0, 1.0);
	float Blinn = pow(NdotH, MaterialShininess);
	vec3 specular = LightSpecularColor * MaterialSpecularColor * Blinn;

	vec3 color = ambient + (diffuse + specular);
	FragmentColor = vec4(color, 1.0);
}