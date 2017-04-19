#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;


//inline vec4 normalizeVec(vec4 plane)
//{
//	vec3 normal(plane.x, plane.y, plane.z);
//	float len = length(normal);
//	return -plane / len;
//}

//bool shouldRenderMesh(mat4 mat, float radius, vec3 center, string meshName)
//{
//	map<string, vec4> planes;
//
//	vec4 firstRow = row(mat, 0);
//	vec4 secondRow = row(mat, 1);
//	vec4 thirdRow = row(mat, 2);
//	vec4 fourthRow = row(mat, 3);
//
//	planes["left"] = (fourthRow + firstRow);
//	planes["right"] = (fourthRow - firstRow);
//	planes["top"] = (fourthRow + secondRow);
//	planes["bottom"] = (fourthRow - secondRow);
//	planes["near"] = (fourthRow + thirdRow);
//	planes["far"] = (fourthRow - thirdRow);
//
//
//	// Normalize, this is not always necessary...
//	for (auto item : planes)
//	{
//		auto plane = item.second;
//		planes[item.first] = normalize(plane);
//	}
//
//
//	for (auto item : planes)
//	{
//		auto plane = item.second;
//		float dist = plane.x * center.x + plane.y * center.y + plane.z * center.z + plane.w ;
//		if (dist > radius)
//		{
//			return false;
//		};
//	}
//	return true;
//}

inline bool shouldRenderMesh(mat4 VP, vec3 center, float radius)
{
	vec4 transformedCentre = VP * vec4(center, 1);
	vec3 PCN = vec3(
		transformedCentre.x / transformedCentre.z,
		transformedCentre.y / transformedCentre.z,
		transformedCentre.z / transformedCentre.z
		);
	if( abs(PCN.x) > 1 || abs(PCN.y) > 1 ||	abs(PCN.y) > 1 )
	{
		return true;
	}
	return true;
}