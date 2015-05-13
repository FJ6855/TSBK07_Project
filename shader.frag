#version 150

in vec3 fragNormal;
in vec2 fragTexCoord;
in float fragLightValue;
in vec3 fragPosition;

out vec4 outColor;

uniform sampler2D tex;
uniform vec3 PointLightColor;
uniform vec3 PointLightPos;
uniform vec3 CameraPos;

void main(void)
{
	float distanceFromCamera = length(fragPosition - CameraPos);
	

	//TOTAL LIGHT
	vec3 Light = vec3(0.0, 0.0, 0.0);

	//AMBIENT LIGHT
	vec3 ambientLight = vec3(0.6, 0.6, 0.6);

	//DIRECTIONAL LIGHT (SUN)
	vec3 sunLightIntensity = vec3(0.4, 0.4, 0.4);
	vec3 sunLightDirection = normalize(vec3(-1.0, -1.0, -0.5));
	vec3 norm = normalize(fragNormal);
	float sunLightDiffuse = max(0.0, dot(norm, -sunLightDirection));

	//POINT LIGHT
	vec3 PointLightDirection = normalize(fragPosition - PointLightPos);
	float PointLightDiffuse = clamp(dot(-PointLightDirection, normalize(fragNormal)), 0, 1);
        //Attenuation
	float dist = length(PointLightPos - fragPosition);
    	float attenuation =  1 / (1 + 0.005 * dist * dist);

	//ADD LIGHTS TOGETHER
	Light = ambientLight + sunLightIntensity * sunLightDiffuse
	+attenuation * PointLightDiffuse * PointLightColor;

	Light = Light * fragLightValue;

	outColor = vec4(texture(tex, fragTexCoord)) * vec4(Light,1.0)
	+  vec4(0.8, 0.8, 0.8, 1.0) * clamp(distanceFromCamera *
	distanceFromCamera / 7000, 0, 1);

} 
