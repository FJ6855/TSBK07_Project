#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 fragNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{	
	fragNormal = transpose(inverse(mat3(modelMatrix))) * inNormal;

	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}
