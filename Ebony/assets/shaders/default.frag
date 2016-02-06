#version 330

uniform vec3 uColor;
out vec4 oColor;

void main()
{
	oColor = vec4(uColor, 1);
}
