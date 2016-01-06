#version 150

in vec2 TexCoord0;

out vec4 oColor;

uniform vec4 baseColor;
uniform float leadingEdgePct;
uniform float trailingEdgePct;

float map( float value, float inputMin, float inputMax, float outputMin, float outputMax ){
	float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	return outVal;
}

void main()
{
	vec4 finalColor = baseColor;
	float colPct = map( TexCoord0.s, trailingEdgePct, leadingEdgePct, 0.0, 1.0 );

	if( colPct > 1.0 || leadingEdgePct == 0.0 ){
		discard;
	}

	finalColor.a = colPct;
	oColor = finalColor;
} 