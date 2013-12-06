#version 330 core

in vec3 exVertex;
in vec3 exNormal;
in vec2 exTexCoords;

// No Matrix Uniforms

// No Light Properties

// Material Properties
uniform sampler2D MaterialAmbientTexture;
uniform sampler2D MaterialDiffuseTexture;
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialDiffuseColor;
uniform float MaterialTransparency;

out vec4 FragmentColor;

void main(void)
{
	vec3 textureDiffuse = vec3(texture(MaterialDiffuseTexture, exTexCoords).rgb); // y-inverted
	vec3 diffuse = MaterialDiffuseColor * textureDiffuse;
	// vec3 diffuse = vec3(0.0);
	// if (MaterialHasTexture)
	// 	diffuse = vec3(texture(MaterialTexture, vec2(exTexCoords.x, -exTexCoords.y)).rgb);
	// else
	// 	diffuse = MaterialDiffuseColor;

	vec3 color = MaterialAmbientColor + diffuse;
	FragmentColor = vec4(color, MaterialTransparency);
}