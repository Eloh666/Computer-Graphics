#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace graphics_framework;
using namespace glm;


// Generates tangents and binormals for geometry
void generate_tb(geometry &geom, const std::vector<glm::vec3> &normals) {
	// Declare tangent and binormal buffers
	std::vector<glm::vec3> tangent_data;
	std::vector<glm::vec3> binormal_data;
	// Iterate through each normal and generate
	for (unsigned int i = 0; i < geom.get_vertex_count(); ++i) {
		// Determine if tangent value.  Get orthogonal with forward and up vectors
		// Orthogonal to forward vector
		glm::vec3 c1 = glm::cross(normals[i], glm::vec3(0.0f, 0.0f, 1.0f));
		// Orthogonal to up vector
		glm::vec3 c2 = glm::cross(normals[i], glm::vec3(0.0f, 1.0f, 0.0f));
		// Determine which vector has greater length.  This will be the tangent
		if (glm::length(c1) > glm::length(c2))
			tangent_data.push_back(glm::normalize(c1));
		else
			tangent_data.push_back(glm::normalize(c2));

		// Generate binormal from tangent and normal
		binormal_data.push_back(glm::normalize(glm::cross(normals[i], tangent_data[i])));
	}

	// Add the new buffers to the geometry
	geom.add_buffer(tangent_data, BUFFER_INDEXES::TANGENT_BUFFER);
	geom.add_buffer(binormal_data, BUFFER_INDEXES::BINORMAL_BUFFER);
}