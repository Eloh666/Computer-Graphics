#version 440

uniform mat4 VP;
uniform mat3 N;
uniform mat4 lightVP;
uniform float grassHeight;
uniform float windStrength;
uniform vec3 windDirectionIn;
uniform vec3 eyePos;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 16) out;

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 transformed_normal;
layout(location = 2) out vec2 tex_coord;
layout(location = 9) out vec4 light_space_pos;

bool shouldRenderMesh(mat4 VP, vec4 center);

// Disclamer: the 3 following functions are the result of following a tutorial, they have been modified,
// but for this reason I only deserve part of the credit
mat4 getRotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float u = 1.0 - c;
    
    return mat4(u * axis.x * axis.x + c,           u * axis.x * axis.y - axis.z * s,  u * axis.z * axis.x + axis.y * s,  0.0,
                u * axis.x * axis.y + axis.z * s,  u * axis.y * axis.y + c,           u * axis.y * axis.z - axis.x * s,  0.0,
                u * axis.z * axis.x - axis.y * s,  u * axis.y * axis.z + axis.x * s,  u * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                             1.0);
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

vec3 getFaceNormal(vec3 a, vec3 b, vec3 c){
	// Calculate two vectors in the plane of the input triangle
	vec3 ab = a - b;
	vec3 ac = a - c;
	vec3 normal = normalize(cross(ab, ac));
	return normal;
}

void main()
{

	bool shouldRender = shouldRenderMesh(VP, gl_in[0].gl_Position);
	bool fallBack = distance(eyePos, gl_in[0].gl_Position.xyz) < 150;
	if(shouldRender || fallBack){
		vec3 basePoint = gl_in[0].gl_Position.xyz;

		float PIover180 = 3.1415/180.0;
		vec3 baseRotationVectors[4] = vec3[](
			vec3(1.0, 0.0, 0.0),
			vec3(float(cos(45.0*PIover180)), 0.0f, float(sin(45.0*PIover180))),
			vec3(float(cos(-45.0*PIover180)), 0.0f, float(sin(-45.0*PIover180))),
			vec3(float(cos(90.0*PIover180)), 0.0f, float(sin(90.0*PIover180)))
		);
	
		vec3 windDirection = normalize(windDirectionIn);

		for(int i = 0; i < 4; i++)
		{

			vec3 baseRotationVectorsRotated = (getRotationMatrix(vec3(0, 1, 0), sin(i*0.7)*0.1)*vec4(baseRotationVectors[i], 1.0)).xyz;

			vLocalSeed = basePoint*float(i);
			int iGrassPatch = randomInt(0, 3);
		
			float fGrassPatchHeight = 3.5+randZeroOne()*2.0;
	
			float fTCStartX = float(iGrassPatch)*0.25f;
			float fTCEndX = fTCStartX+0.25f;

			float fWindPower = 0.5f+sin(basePoint.x/30+basePoint.z/30+(1.2f+abs(windStrength)/40.0f));
			if(fWindPower < 0.0f)
				fWindPower = fWindPower*0.2f;
			else fWindPower = fWindPower*0.3f;
		
			fWindPower *= abs(windStrength) / 2;
		
		
			// Grass patch top left vertex
			vec3 topLeftVert = basePoint - baseRotationVectorsRotated*grassHeight*0.5f + windDirection*fWindPower;
			topLeftVert.y += fGrassPatchHeight; 
			// Grass patch bottom left vertex
			vec3 bottomLeftVert = basePoint - baseRotationVectors[i]*grassHeight*0.5f;  
			// Grass patch top right vertex
			vec3 topRightVert = basePoint + baseRotationVectorsRotated*grassHeight*0.5f + windDirection*fWindPower;
			topRightVert.y += fGrassPatchHeight;  
			// Grass patch bottom right vertex
			vec3 bottomRightVert = basePoint + baseRotationVectors[i]*grassHeight*0.5f; 

			vec3 triangleNormal = N * getFaceNormal(topLeftVert, bottomLeftVert, topRightVert);


			// Grass patch top left vertex
			gl_Position = VP*vec4(topLeftVert, 1.0);
			tex_coord = vec2(fTCStartX, 1.0);
			transformed_normal = triangleNormal;
			position = topLeftVert;
			light_space_pos = lightVP * vec4(position, 1);
			EmitVertex();
		
			// Grass patch bottom left vertex
			gl_Position = VP*vec4(bottomLeftVert, 1.0);
			tex_coord = vec2(fTCStartX, 0.0);
			transformed_normal = triangleNormal;
			position = bottomLeftVert;
			light_space_pos = lightVP * vec4(position, 1);
			EmitVertex();
		                               
			// Grass patch top right vertex
			gl_Position = VP*vec4(topRightVert, 1.0);
			tex_coord = vec2(fTCEndX, 1.0);
			transformed_normal = triangleNormal;
			position = topRightVert;
			light_space_pos = lightVP * vec4(position, 1);
			EmitVertex();
		
			// Grass patch bottom right vertex
			gl_Position = VP*vec4(bottomRightVert, 1.0);
			tex_coord = vec2(fTCEndX, 0.0);
			transformed_normal = triangleNormal;
			position = bottomRightVert;
			light_space_pos = lightVP * vec4(position, 1);
			EmitVertex();

			EndPrimitive();
		}
	}
}