#version 330 core

in vec3 in_Position;
in vec3 in_Normal;

out vec4 ex_Color;

uniform mat4 ModelViewProjectionMatrix;

void main(void)
{
	ex_Color = vec4((normalize(in_Normal) + vec3(1.0)) / 2.0, 1.0);
	// ex_Color = vec4((in_Position + vec3(1.0)) / 2.0, 1.0);
	gl_Position = ModelViewProjectionMatrix * vec4(in_Position, 1.0);
}