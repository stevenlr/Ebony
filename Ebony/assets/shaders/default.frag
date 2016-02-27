#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

uniform samplerCube uCubemap;

out vec4 oColor;

void main()
{
	float f = max(dot(vNormal, normalize(vec3(0.2, -1, 1))), 0.1f);
	vec3 pos = normalize(vPosition.xyz);
	vec3 posr = vec3(pos.x, pos.z, -pos.y);
	
	oColor = vec4(texture(uCubemap, posr).rgb, 1);
}
