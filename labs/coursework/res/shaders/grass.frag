#version 440

layout(location = 0) in vec2 tex_coord;

out vec4 outputColor;

uniform sampler2D tex;

void main()
{
	float fAlphaTest = 0.25f;
	float fAlphaMultiplier = 1.5f;

	vec4 vTexColor = texture2D(tex, tex_coord);
	float fNewAlpha = vTexColor.a*fAlphaMultiplier;               
	if(fNewAlpha < fAlphaTest)
		discard;

	vec4 vMixedColor = vTexColor*vec4(0, 0.43, 0, 1); 
	
	outputColor = vec4(vMixedColor.zyx, 1);
}