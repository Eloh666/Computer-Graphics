#version 440

bool shouldRenderMesh(mat4 VP, vec4 center)
{
	vec4 transformedCentre = VP * center;
	vec3 PCN = vec3(
		transformedCentre.x / transformedCentre.z,
		transformedCentre.y / transformedCentre.z,
		transformedCentre.z / transformedCentre.z
		);
	if( abs(PCN.x) > 1.0 || abs(PCN.y) > 1.0 ||	abs(PCN.y) > 1.0 )
	{
		return false;
	}
	return true;
}