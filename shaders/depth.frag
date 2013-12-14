#version 330 core

// in vec3 exPosition;
in vec3 exVertex;

out vec4 FragmentColor;

void main(void)
{
	float depth = (length(exVertex) - 1.0) / 19.0;;
	FragmentColor = vec4(vec3(depth), 1.0);
}