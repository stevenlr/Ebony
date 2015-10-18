#version 330

in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uSampler;

out vec4 oColor;

void main()
{	
	oColor = texture(uSampler, vUV.st);
}
