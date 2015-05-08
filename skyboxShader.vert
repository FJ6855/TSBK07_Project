#version 150

in vec3 in_Position;
in vec2 in_Tex_Coord;

out vec2 out_Tex;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform sampler2D texUnit;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);

	out_Tex = in_Tex_Coord;
}
