#version 330 core

in vec3 exVertex;
in vec3 exNormal;
in vec3 exTangent;
in vec2 exTexCoords;

uniform sampler2D MaterialBumpTexture;

out vec4 FragmentColor;

void main(void)
{
	vec3 normal = normalize(exNormal);
	vec3 bumpMapNormal = texture(MaterialBumpTexture, exTexCoords).rgb;
	if (equal(bumpMapNormal, vec3(1.0)) == false)
	{
		vec3 tangent = normalize(exTangent);
		// tangent = normalize(tangent - dot(tangent, N) * N);
		vec3 bitangent = cross(tangent, normal);
		bumpMapNormal = normalize(2.0 * bumpMapNormal - vec3(1.0));
		mat3 tangentSpace = mat3(tangent, bitangent, normal);
		normal = normalize(tangentSpace * bumpMapNormal);
	}

	FragmentColor = vec4(normal / 2.0 + 0.5, 1.0);
	// FragmentColor = vec4(normal, 1.0);
}