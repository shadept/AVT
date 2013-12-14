#version 330 core

in vec3 in_Position;

out vec3 exPosition;

uniform mat4 ModelMatrix;
uniform mat4 ModelViewProjectionMatrix;

void main(void)
{
	// exPosition = vec3(ModelMatrix * vec4(in_Position, 1.0));
	exPosition = in_Position;
	gl_Position = ModelViewProjectionMatrix *  vec4(in_Position, 1.0);
}