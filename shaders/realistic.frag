#version 330 core

in vec3 exVertex;
in vec3 exNormal;
in vec3 exTangent;
in vec2 exTexCoords;

out vec4 FragmentColor;

// Matrix Uniforms
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

// struct Camera
// {
// 	vec3 position;
// 	float near;
// 	float far;
// };

struct Environment
{
	samplerCube map;
	samplerCube skybox;
	vec3 ambient;
};

struct Light
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 spotDirection;
	float spotExponent;
	float spotCutoff;		// [0 .. 90]
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
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
uniform Light lightSource;
uniform Material material;

vec3 AmbientContribution()
{
	vec3 textureContribution = texture(material.ambientTexture, exTexCoords).rgb;
	return lightSource.ambient * material.ambient * textureContribution;
}

vec3 DiffuseContribution()
{
	vec3 textureContribution = texture(material.diffuseTexture, exTexCoords).rgb;
	return lightSource.diffuse * material.diffuse * textureContribution;
}

vec3 SpecularContribution()
{
	vec3 textureContribution = texture(material.specularTexture, exTexCoords).rgb;
	return lightSource.specular * material.specular * textureContribution;
}

vec3 normalFromTexture(vec3 normal, vec3 tangent, vec2 texCoords)
{
	vec3 bumpMapNormal = texture(material.normalTexture, texCoords).rgb;
	vec3 bitangent = cross(tangent, normal);
	bumpMapNormal = normalize(2.0 * bumpMapNormal - vec3(1.0));
	mat3 tangentSpace = mat3(tangent, bitangent, normal);
	return normalize(tangentSpace * bumpMapNormal);
}

vec3 getNormal(vec3 normal, vec3 tangent, vec2 texCoords)
{
	vec3 _normal = normalize(normal);
	vec3 bumpMapNormal = texture(material.normalTexture, texCoords).rgb;
	if (equal(bumpMapNormal, vec3(1.0)) == false)
	{
		_normal = normalFromTexture(_normal, normalize(tangent), texCoords);
	}

	return _normal;
}

float fresnelReflectance(float VdotN, float r0)
{
	return r0 + (1.0 - r0) * pow(1.0 - VdotN, 5.0);
}

void main(void)
{
	mat4 inverseViewMatrix = inverse(ViewMatrix);
	vec3 vertex = exVertex;
	vec3 normal = getNormal(exNormal, exTangent, exTexCoords);
	vec3 view = normalize(-vertex);

	// vec3 viewWC = vec3(inverseViewMatrix * vec4(view, 1.0));
	// vec3 normalWC = normalize(vec3(vec4(normal, 0.0) * ViewMatrix));

	bool direction = lightSource.position.w == 0.0;
	vec4 lightPosition = lightSource.position;
	// vec4 lightPosition = ViewMatrix * lightSource.position;
	vec3 light = vec3(0.0);
	float lightDistance = 0;
	if (direction)
		light = lightPosition.xyz;
	else
		light = lightPosition.xyz - vertex;
	lightDistance = length(light);
	light = normalize(light);

	vec3 half_vector = normalize(light + view);

	float NdotL = clamp(dot(normal, light), 0.0, 1.0);
	float NdotH = clamp(dot(normal, half_vector), 0.0, 1.0);
	float specularTerm = exp(-pow(acos(NdotH) / material.shininess, 2.0));

	vec3 ambient = AmbientContribution();
	vec3 diffuse = DiffuseContribution() * NdotL;
	vec3 specular = SpecularContribution() * specularTerm;

	// float spotEffect = dot(normalize(lightSource.spotDirection), normalize(-light));
	float attenuation = 1.0;
	if (!direction)
	{
		attenuation = 1.0 / (lightSource.constantAttenuation +
			lightSource.linearAttenuation * lightDistance +
			lightSource.quadraticAttenuation * lightDistance * lightDistance);
	}


	vec3 color = environment.ambient + (ambient + diffuse + specular) * attenuation;

	if (material.transparency < 1.0)
	{
		//		   | Air  | material
		//         \/	  \/
		float rr = 1.0 / material.refractionIndex;
		float r0 = pow(1.0 - rr, 2.0) / pow(1.0 + rr, 2.0);
		float fresnel = fresnelReflectance(dot(view, normal), r0);

		diffuse = DiffuseContribution() * (NdotL * 0.5 + 0.5);
		vec3 vRefracted = refract(-view, normal, rr);
		vec3 vReflected = reflect(-view, normal);
		// Both view and normal were in eye coords, so we need to transform them to world coords
		vRefracted = vec3(inverseViewMatrix * vec4(vRefracted, 0.0));
		vReflected = vec3(inverseViewMatrix * vec4(vReflected, 0.0));

		vec3 cRefracted = texture(environment.map, vRefracted).rgb;
		vec3 cReflected = texture(environment.map, vReflected).rgb;

		color = mix(color, cReflected, fresnel);
		color = mix(cRefracted, color, material.transparency);
	}
	else if (material.reflectivity > 0.0)
	{
		float l = length(environment.ambient + ambient);
		float val = min(NdotL * l + (1.0 - l), 1.0);
		vec3 vReflected = reflect(-view, normal);
		// Same as above
		vReflected = vec3(inverseViewMatrix * vec4(vReflected, 0.0));
		vec3 cReflected = texture(environment.map, vReflected).rgb * val;
		color = mix(color, cReflected, material.reflectivity);
	}

	FragmentColor = vec4(color, 1.0);
}