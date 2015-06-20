#version 330

in vec3 vPosition;
in vec3 vNormal;

uniform sampler2D uTexture;

out vec4 oColor;

void main()
{
	vec3 sunDir = normalize(vec3(1, 1, -1));
	vec3 sunColor = vec3(0.9, 0.85, 0.85);
	vec3 ambientColor = vec3(0, 0, 0.03);
	
	float sunContrib = max(dot(vNormal, -sunDir), 0);

	oColor = vec4(ambientColor + sunContrib * sunColor, 1);
}