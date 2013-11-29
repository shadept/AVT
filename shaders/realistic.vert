#version 330 core

in vec3 in_Position;
in vec3 in_Normal;

uniform mat4 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

out vec3 exVertex;
out vec3 exNormal;

void main(void)
{
	exVertex = vec3(ModelViewMatrix * vec4(in_Position, 1.0));
	exNormal = vec3(NormalMatrix * vec4(in_Normal, 1.0)); // FIXME this is bad

	gl_Position = ModelViewProjectionMatrix *  vec4(in_Position, 1.0);
}