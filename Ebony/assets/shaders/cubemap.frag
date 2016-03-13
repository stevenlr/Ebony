#version 330

in vec3 vPosition;

uniform samplerCube uCubemap;

out vec4 oColor;

void main()
{
	vec3 pos = normalize(vPosition.xyz);
	vec3 posr = vec3(pos.x, pos.z, -pos.y);
	
	oColor = vec4(texture(uCubemap, posr).rgb, 1);
}
