#version 110

uniform vec4 baseColor;
uniform float leadingEdgePct;
uniform float trailingEdgePct;

void main(){
	gl_TexCoord[0]	= gl_MultiTexCoord0;
	gl_Position		= ftransform();
} 