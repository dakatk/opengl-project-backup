#version 130
in vec4 position;
in vec2 texcoord;
in vec3 normal;
out vec2 Texcoord;
out vec3 Normal;
out vec3 WorldPos;
uniform mat4 modelViewMatrix;
uniform mat4 projMatrix;
void main()
{
	Texcoord = texcoord;
	Normal = (modelViewMatrix * vec4(normal, 0.0)).xyz;
	WorldPos = (modelViewMatrix * position).xyz;
	gl_Position = modelViewMatrix * projMatrix * position;
}
