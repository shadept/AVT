#version 330 core

in vec3 exPosition;

out vec4 FragmentColor;

struct Environment
{
	samplerCube map;
	samplerCube skybox;
	vec3 ambient;
};

uniform Environment environment;

void main(void)
{
	FragmentColor = texture(environment.skybox, exPosition);
}