#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_TexCoords;

uniform mat4 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

out vec3 exVertex;
out vec3 exNormal;
out vec3 exTangent;
out vec2 exTexCoords;

void main(void)
{
	exVertex = vec3(ModelViewMatrix * vec4(in_Position, 1.0));
	// exNormal = vec3(NormalMatrix * vec4(in_Normal, 0.0));
	exNormal = in_Normal;
	// exTangent = vec3(NormalMatrix * vec4(in_Tangent, 0.0));
	exTangent = in_Tangent;
	exTexCoords = in_TexCoords;

	gl_Position = ModelViewProjectionMatrix *  vec4(in_Position, 1.0);
}