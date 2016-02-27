#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

//uniform sampler2D uTexture;

out vec4 oColor;

void main()
{
	float f = max(dot(vNormal, normalize(vec3(0.2, -1, 1))), 0.1f);
	
	oColor = vec4(vec3(f), 1);
}
