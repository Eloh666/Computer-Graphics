#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace chrono;
using namespace graphics_framework;
using namespace glm;

inline mesh createTerrainMesh(const texture &height_map, unsigned int width, unsigned int depth, float height_scale, geometry* grassGeom = nullptr) {

	geometry geom;

	// Contains our position data
	vector<vec3> positions;
	// Contains our normal data
	vector<vec3> normals;
	// Contains our texture coordinate data
	vector<vec2> tex_coords;
	// Contains our texture weights
	vector<vec4> tex_weights;
	// Contains our index data
	vector<unsigned int> indices;

	auto heightMapWidth = height_map.get_width();
	auto heightMapHeight = height_map.get_height();

	// Extract the texture data from the image
	glBindTexture(GL_TEXTURE_2D, height_map.get_id());
	auto data = new vec4[heightMapWidth * heightMapHeight];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, static_cast<void *>(data));

	// Determine ratio of height map to geometry
	float width_point = static_cast<float>(width) / static_cast<float>(heightMapWidth);
	float depth_point = static_cast<float>(depth) / static_cast<float>(heightMapHeight);

	// Point to work on
	vec3 point;
	// Part 1 - Iterate through each point, calculate vertex and add to vector
	for (int x = 0; x < heightMapWidth; ++x) {
		// Calculate x position of point
		point.x = -(width / 2.0f) + (width_point * static_cast<float>(x));

		for (int z = 0; z < heightMapHeight; ++z) {
			// *********************************
			// Calculate z position of point
			point.z = -(depth / 2.0f) + (depth_point * static_cast<float>(z));
			// *********************************
			// Y position based on red component of height map data
			point.y = data[(z * heightMapWidth) + x].y * height_scale;
			// Add point to position data
			positions.push_back(point);
		}
	}

	// Part 1 - Add index data
	for (unsigned int x = 0; x < heightMapWidth - 1; ++x) {
		for (unsigned int y = 0; y < heightMapHeight - 1; ++y) {
			// Get four corners of patch
			unsigned int top_left = (y * heightMapWidth) + x;
			unsigned int top_right = (y * heightMapWidth) + x + 1;
			// *********************************
			unsigned int bottom_left = ((y + 1) * heightMapWidth) + x;
			unsigned int bottom_right = ((y + 1) * heightMapHeight) + x + 1;

			// *********************************
			// Push back indices for triangle 1 (tl,br,bl)
			indices.push_back(top_left);
			indices.push_back(bottom_right);
			indices.push_back(bottom_left);
			// Push back indices for triangle 2 (tl,tr,br)
			// *********************************
			indices.push_back(top_left);
			indices.push_back(top_right);
			indices.push_back(bottom_right);

			// *********************************
		}
	}

	// Resize the normals buffer
	normals.resize(positions.size());

	// Part 2 - Calculate normals for the height map
	for (unsigned int i = 0; i < indices.size() / 3; ++i) {
		// Get indices for the triangle
		auto idx1 = indices[i * 3];
		auto idx2 = indices[i * 3 + 1];
		auto idx3 = indices[i * 3 + 2];

		// Calculate two sides of the triangle
		vec3 side1 = positions[idx1] - positions[idx3];
		vec3 side2 = positions[idx1] - positions[idx2];

		// Normal is normal(cross product) of these two sides
		// *********************************
		vec3 normal = normalize(cross(side2, side1));

		// Add to normals in the normal buffer using the indices for the triangle

		normals[idx1] = normals[idx1] + normal;
		normals[idx2] = normals[idx2] + normal;
		normals[idx3] = normals[idx3] + normal;

		// *********************************
	}

	// Normalize all the normals
	for (auto &n : normals) {
		// *********************************
		n = normalize(n);
		// *********************************
	}

	// Part 3 - Add texture coordinates for geometry
	for (unsigned int x = 0; x < heightMapWidth; ++x) {
		for (unsigned int z = 0; z < heightMapHeight; ++z) {
			tex_coords.push_back(vec2(width_point * x, depth_point * z));
		}
	}

	// Part 4 - Calculate texture weights for each vertex
	for (unsigned int x = 0; x < heightMapWidth; ++x) {
		for (unsigned int z = 0; z < heightMapHeight; ++z) {
			// Calculate tex weight
			vec4 tex_weight(
				clamp(1.0f - abs(data[(heightMapWidth * z) + x].y - 0.1f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(heightMapWidth * z) + x].y - 0.1f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(heightMapWidth * z) + x].y - 0.5f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(heightMapWidth * z) + x].y - 0.9f) / 0.25f, 0.0f, 1.0f)
);

			// *********************************
			// Sum the components of the vector
			auto total = tex_weight.x + tex_weight.y + tex_weight.z + tex_weight.w;
			// Divide weight by sum
			tex_weight = tex_weight / total;
			// Add tex weight to weights
			tex_weights.push_back(tex_weight);
			// *********************************
		}
	}

	// Add necessary buffers to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
	geom.add_index_buffer(indices);

	geom.generate_tb(normals);
	
	if(grassGeom)
	{
		//init rain data
		default_random_engine rand(duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
		
		vector<vec3> filteredPositions = vector<vec3>();

		int randX = (rand() % 2) + 1;
		int currentIndex = 0;
		float minHeight = height_scale * 0.285;
		float maxHeight = height_scale * 0.65;
		while(currentIndex < positions.size())
		{
			vec3 vertex = positions[currentIndex];
			if(vertex.y >= minHeight && vertex.y <= maxHeight)
			{
				vec3 vertexToAdd = vertex * 50.0f;
				filteredPositions.push_back(vertexToAdd);
				for (int i = 0; i < randX; i++)
				{
					filteredPositions.push_back(vertexToAdd + vec3(i*randX, 0, i*randX));
				}
			}
			randX = (rand() % 2) + 1;
			currentIndex += randX;
		}
		
		grassGeom->add_buffer(filteredPositions, BUFFER_INDEXES::POSITION_BUFFER);
	}

	// Delete data
	delete[] data;

	auto terrainMesh = mesh(geom);
	return terrainMesh;
}