#version 330

in vec3 iPosition;
in vec3 iNormal;

uniform mat4 uMvp;

out vec3 vPosition;
out vec3 vNormal;

void main()
{
	gl_Position = uMvp * vec4(iPosition, 1);
	vNormal = iNormal;
	vPosition = iPosition;
}