#version 150

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 outColor;

uniform vec3 cameraPos;


void main(void)
{
	//TOTAL LIGHT
	vec3 Light = vec3(0.0, 0.0, 0.0);

	//AMBIENT LIGHT
	vec3 ambientLight = vec3(0.2, 0.2, 0.2);

	//DIRECTIONAL LIGHT (SUN)
	vec3 sunLightIntensity = vec3(0.3, 0.3, 0.3);
	vec3 sunLightDirection = normalize(vec3(-1.0, -1.0, -0.5));
	vec3 norm = normalize(fragNormal);
	float sunLightDiffuse = max(0.0,
	dot(norm,-sunLightDirection))*0.8;

	//SPECULAR LIGHT
	vec3 reflectionDirection = reflect(sunLightDirection, norm);
	vec3 surfaceToCamera = normalize(cameraPos - fragPosition);
	float cosAngle = max(0.0, dot(surfaceToCamera,reflectionDirection));
	float specularCoefficient = pow(cosAngle, 50);
	vec3 LightSpecular = specularCoefficient * vec3(1.0, 1.0,1.0);

	//ADD LIGHTS TOGETHER
	Light = vec3(1.0, 0.0, 0.0) * (ambientLight + sunLightDiffuse
	* sunLightIntensity) +  LightSpecular;

	outColor = vec4(Light, 1.0);
} 
