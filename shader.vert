#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;
in float inLightValue;

out vec3 fragNormal;
out float fragLightValue;
out vec2 fragTexCoord;
out vec3 fragPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{	
	fragNormal = mat3(modelMatrix) * inNormal;
        fragLightValue = inLightValue;
	fragTexCoord = inTexCoord;
	fragPosition = vec3(modelMatrix * vec4(inPosition, 1.0));

	gl_Position =  projectionMatrix * viewMatrix * modelMatrix *vec4(inPosition, 1.0);
	

}
