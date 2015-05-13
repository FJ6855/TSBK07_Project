#version 150

in vec3 inPosition;
in vec3 inNormal;

out vec3 fragNormal;
out float fragLightValue;
out vec3 fragPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{	
	fragNormal = mat3(modelMatrix) * inNormal;

	fragPosition = vec3(modelMatrix * vec4(inPosition, 1.0));

	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}
