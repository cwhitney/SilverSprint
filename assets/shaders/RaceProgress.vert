#version 150

in vec4 ciPosition;
in vec2 ciTexCoord0;

out vec2 TexCoord0;

uniform mat4 ciModelViewProjection;

uniform vec4 baseColor;
uniform float leadingEdgePct;
uniform float trailingEdgePct;

void main()
{
	TexCoord0	= ciTexCoord0;
	gl_Position	= ciModelViewProjection * ciPosition;
} 