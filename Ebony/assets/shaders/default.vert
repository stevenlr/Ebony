#version 330

in vec3 iPosition;

uniform mat4 uMvp;

out vec3 vPosition;

void main()
{
	gl_Position = uMvp * vec4(iPosition, 1);
	vPosition = iPosition;
}