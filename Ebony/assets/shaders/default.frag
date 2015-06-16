#version 330

in vec3 vPosition;

uniform sampler2D uTexture;

out vec4 oColor;

void main()
{
	vec4 color = texture(uTexture, vPosition.xy);
	oColor = vec4(color.rgb, 1);
}