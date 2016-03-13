#version 330

in vec3 iPosition;

uniform mat4 uMvp;

out vec3 vPosition;

void main()
{
	gl_Position = uMvp * vec4(iPosition, 0);
	gl_Position.z = gl_Position.w;
	vPosition = iPosition;
}
