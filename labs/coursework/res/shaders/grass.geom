#version 440

uniform mat4 MVP;
uniform mat4 MV;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 12) out;

smooth out vec2 textCoordOut;
smooth out vec3 worldPos;
smooth out vec4 eyeSpacePos;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 vLocalSeed;

// This function returns random number from zero to one
float randZeroOne()
{
    uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;
 
    float fRes =  2.0 - uintBitsToFloat(n);
    vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes  + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);
    return fRes;
}

int randomInt(int min, int max)
{
	float fRandomFloat = randZeroOne();
	return int(float(min)+fRandomFloat*float(max-min));
}

void main()
{
	
	vec3 vGrassFieldPos = gl_in[0].gl_Position.xyz;

	float PIover180 = 3.1415/180.0;
	vec3 vBaseDir[3] = vec3[](
		vec3(1.0, 0.0, 0.0),
		vec3(float(cos(45.0*PIover180)), 0.0f, float(sin(45.0*PIover180))),
		vec3(float(cos(-45.0*PIover180)), 0.0f, float(sin(-45.0*PIover180)))
	);
	
	float fGrassPatchSize = 5.0;
	float fWindStrength = 4.0;
	
	vec3 vWindDirection = vec3(1.0, 0.0, 1.0);
	vWindDirection = normalize(vWindDirection);

	for(int i = 0; i < 3; i++)
	{

		vec3 vBaseDirRotated = (rotationMatrix(vec3(0, 1, 0), sin(i*0.7)*0.1)*vec4(vBaseDir[i], 1.0)).xyz;

		vLocalSeed = vGrassFieldPos*float(i);
		int iGrassPatch = randomInt(0, 3);
		
		float fGrassPatchHeight = 3.5+randZeroOne()*2.0;
	
		float fTCStartX = float(iGrassPatch)*0.25f;
		float fTCEndX = fTCStartX+0.25f;

		float fWindPower = 0.5f+sin(vGrassFieldPos.x/30+vGrassFieldPos.z/30+(1.2f+fWindStrength/20.0f));
		if(fWindPower < 0.0f)
			fWindPower = fWindPower*0.2f;
		else fWindPower = fWindPower*0.3f;
		
		fWindPower *= fWindStrength;
		

		vec3 vTL = vGrassFieldPos - vBaseDirRotated*fGrassPatchSize*0.5f + vWindDirection*fWindPower;
		vTL.y += fGrassPatchHeight; 
		gl_Position = MVP*vec4(vTL, 1.0);
		textCoordOut = vec2(fTCStartX, 1.0);
		worldPos = vTL;
		eyeSpacePos = MV*vec4(vTL, 1.0);
		EmitVertex();
		
		// Grass patch bottom left vertex
		vec3 vBL = vGrassFieldPos - vBaseDir[i]*fGrassPatchSize*0.5f;  
		gl_Position = MVP*vec4(vBL, 1.0);
		textCoordOut = vec2(fTCStartX, 0.0);
		worldPos = vBL;
		eyeSpacePos = MV*vec4(vBL, 1.0);
		EmitVertex();
		                               
		// Grass patch top right vertex
		vec3 vTR = vGrassFieldPos + vBaseDirRotated*fGrassPatchSize*0.5f + vWindDirection*fWindPower;
		vTR.y += fGrassPatchHeight;  
		gl_Position = MVP*vec4(vTR, 1.0);
		textCoordOut = vec2(fTCEndX, 1.0);
		worldPos = vTR;
		eyeSpacePos = MV*vec4(vTR, 1.0);
		EmitVertex();
		
		// Grass patch bottom right vertex
		vec3 vBR = vGrassFieldPos + vBaseDir[i]*fGrassPatchSize*0.5f;  
		gl_Position = MVP*vec4(vBR, 1.0);
		textCoordOut = vec2(fTCEndX, 0.0);
		worldPos = vBR;
		eyeSpacePos = MV*vec4(vBR, 1.0);
		EmitVertex();
		
		EndPrimitive();
	}
}