#version 330 core

in vec3 in_Position;

uniform mat4 ModelViewProjectionMatrix;

void main(void)
{
	gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);
}