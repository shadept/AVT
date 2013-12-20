#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_TexCoords;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewProjectionMatrix;

out vec3 exVertex;
out vec3 exNormal;
out vec3 exTangent;
out vec2 exTexCoords;

out vec3 model_vertex;

void main(void)
{	
	exVertex = (ModelViewMatrix * vec4(in_Position, 1.0)).xyz;
	exNormal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	exTangent = (NormalMatrix * vec4(in_Tangent, 0.0)).xyz;
	exTexCoords = in_TexCoords;

	model_vertex = in_Position;

	gl_Position = ModelViewProjectionMatrix *  vec4(in_Position, 1.0);
}