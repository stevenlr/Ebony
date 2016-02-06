#version 330

in vec3 iPosition;
uniform mat4 uMvp;

void main()
{
	gl_Position = uMvp * vec4(iPosition, 1);
}
