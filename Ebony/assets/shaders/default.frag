#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

uniform sampler2D uTexture;

out vec4 oColor;

void main()
{
	vec4 texColor = texture(uTexture, vUV.st * vec2(1, -1) + vec2(0, 1));
	texColor.rgb = vec3(1, 0.2, 0.5);
	texColor.a /= 5;
	oColor = vec4(texColor.rgb * (1 - texColor.a / 2), texColor.a / (2 - texColor.a));
}
