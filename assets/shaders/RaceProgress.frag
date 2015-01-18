#version 110

uniform vec4 baseColor;
uniform float leadingEdgePct;
uniform float trailingEdgePct;

float map( float value, float inputMin, float inputMax, float outputMin, float outputMax ){
	float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	return outVal;
}

void main() {
	vec4 finalColor = baseColor;
	float colPct = map( gl_TexCoord[0].s, trailingEdgePct, leadingEdgePct, 0.0, 1.0 );

	if( colPct > 1.0 || leadingEdgePct == 0.0 ){
		discard;
	}

	finalColor.a = colPct;
	gl_FragColor = finalColor;
} 