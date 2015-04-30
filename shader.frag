#version 150

in vec3 fragNormal;
in vec2 fragTexCoord;
in float fragLightValue;

out vec4 outColor;

uniform sampler2D tex;

void main(void)
{	
	vec3 Light = vec3(0.0, 0.0, 0.0);
        vec3 ambientLight = vec3(0.6, 0.6, 0.6);

	vec3 sunLightIntensity = vec3(0.4, 0.4, 0.4);
	vec3 sunLightDirection = normalize(vec3(-0.5, -1.0, -2.0));	
	vec3 norm = normalize(fragNormal);
	float sunLightDiffuse = max(0.0, dot(norm, -sunLightDirection));

	Light = ambientLight + (sunLightIntensity * sunLightDiffuse);
	Light = Light * fragLightValue;
	outColor = vec4(texture(tex, fragTexCoord)) * vec4(Light,1.0);
}	
