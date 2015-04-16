#version 150

in vec3 fragNormal;

out vec4 outColor;

void main(void)
{	vec3 light = vec3(0.7, 0.7, 0.7);
	
	vec3 norm = normalize(fragNormal);

	float diffuseShade = dot(norm, light);

	diffuseShade = max(0.0, diffuseShade);

	float shade = 1.0 * diffuseShade;
	
	outColor = vec4(shade, shade, shade, 1.0);
}
