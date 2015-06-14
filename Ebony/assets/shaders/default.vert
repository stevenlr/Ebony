#version 330

in vec3 iPosition;

void main()
{
	gl_Position = vec4(iPosition, 1);
}