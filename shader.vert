#version 150

in vec3 inPosition;
in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{	
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}
