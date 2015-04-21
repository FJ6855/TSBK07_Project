#version 150

in vec3 fragNormal;

out vec4 outColor;

void main(void)
{	
	vec3 sunLight = vec3(0.7, 0.7, 0.7);
	vec3 sunDirection = normalize(vec3(-0.5, -1.0, -2.0));	
	vec3 norm = normalize(fragNormal);
	float diffuseShade = max(0.0, dot(norm, -sunDirection));

	vec3 sunDiffuse = sunLight * diffuseShade;
	
	outColor = vec4(sunDiffuse, 1.0);
}
