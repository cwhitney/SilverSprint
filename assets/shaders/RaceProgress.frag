#version 150

in vec2 TexCoord0;
in vec4 vColor;

out vec4 oColor;

uniform vec4 	uBaseColor;
uniform float 	uLeadingEdgePct;
uniform float 	uTailLen;
uniform float 	ciElapsedSeconds;

float map( float value, float inputMin, float inputMax, float outputMin, float outputMax ){
	float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	
	return max(outVal, outputMin);
}

void main()
{
	vec4 finalColor = uBaseColor;

	float le = fract(uLeadingEdgePct);
	float te = le - max(0.05, uTailLen);

	// for testing
	// le = fract(ciElapsedSeconds);
	// te = le - 0.25;

	// calculate alpha fade out for tail
    float dist = le - te;
    float tt = le - dist;
    float colPct = map(fract(TexCoord0.s - tt), 0.0, dist, 0.0, 1.0);

    // line smoothing
    float edgeDist = 0.01;
	float sm = smoothstep(1.0, 0.99, colPct);
	float e1 = smoothstep(1.0, 1.0 - edgeDist, TexCoord0.t);
	float e2 = smoothstep(0.0, edgeDist, TexCoord0.t);

    if(colPct > 1.0){
		// discard;
	}

	finalColor.a = colPct * sm * e1 * e2;
	oColor = finalColor;
} 