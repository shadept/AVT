#version 330 core

in vec3 exVertex;
in vec3 exNormal;
in vec3 exTangent;
in vec2 exTexCoords;

in vec3 model_vertex;

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
//	vec3 bumpMapNormal = texture(material.normalTexture, texCoords).rgb;
//	if (equal(bumpMapNormal, vec3(1.0)) == false)
//	{
//		_normal = normalFromTexture(_normal, normalize(tangent), texCoords);
//	}

	return _normal;
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

	//		   | Air  | material
	//         \/	  \/
	float eta = 1.0 / material.refractionIndex;
	float r0 = pow(1.0 - eta, 2.0) / pow(1.0 + eta, 2.0);
	float fresnel = r0 + (1.0 - r0) * pow(1.0 - dot(view, normal), 5.0);


	// float l = length(environment.ambient + ambient);
	// float val = min(NdotL * l + (1.0 - l), 1.0);
	vec3 view_world = vec3(inverseViewMatrix * vec4(view, 0.0));
	vec3 worldNormal = vec3(inverseViewMatrix * vec4(normal, 0.0));
	if (material.transparency < 1.0)
	{
		vec3 vReflected = reflect(-view_world, worldNormal);

		// Chromatic Aberration
		vec3 vRefractedR = refract(-view_world, worldNormal, 1 / material.refractionIndex);
		vec3 vRefractedG = refract(-view_world, worldNormal, 1 / material.refractionIndex * 0.99);
		vec3 vRefractedB = refract(-view_world, worldNormal, 1 / material.refractionIndex * 0.98);

		vec3 cRefracted;
		cRefracted.r = texture(environment.map, vRefractedR).r;
		cRefracted.g = texture(environment.map, vRefractedG).g;
		cRefracted.b = texture(environment.map, vRefractedB).b;
		vec3 cReflected = texture(environment.map, normalize(vReflected + model_vertex)).rgb;

		// color = mix(color, cReflected, fresnel);
		// color = mix(cRefracted, color, material.transparency);
		float reflectivity = min(material.reflectivity + fresnel, 1.0);
		vec3 newColor = mix(cRefracted, cReflected, reflectivity);
		diffuse = mix(newColor, diffuse, material.transparency);
	}
	else if (material.reflectivity > 0.0)
	{
		vec3 vReflected = reflect(-view_world, worldNormal);
		vec4 reflectedColor = texture(environment.map, normalize(vReflected + model_vertex));
		diffuse = mix(diffuse, reflectedColor.rgb, material.reflectivity);
	}

	vec3 color = environment.ambient + (ambient + diffuse + fresnel * specular) * attenuation;

	FragmentColor = vec4(color, 1.0);
	//FragmentColor = vec4(exTexCoords.s, exTexCoords.t, 0.0, 1.0);
}