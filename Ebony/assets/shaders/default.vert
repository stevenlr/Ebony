#version 330

in vec3 iPosition;
in vec3 iNormal;
in vec3 iTangent;
in vec2 iUV;

uniform mat4 uMvp;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vUV;

void main()
{
	gl_Position = uMvp * vec4(iPosition, 1);
	vNormal = normalize(iNormal);
	vUV = iUV;
	vPosition = iPosition;
}
