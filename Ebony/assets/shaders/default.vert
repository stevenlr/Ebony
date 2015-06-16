#version 330

in vec3 iPosition;

out vec3 vPosition;

void main()
{
	gl_Position = vec4(iPosition, 1);
	vPosition = gl_Position.xyz;
}