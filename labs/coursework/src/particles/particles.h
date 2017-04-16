#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>


using namespace std;
using namespace chrono;
using namespace graphics_framework;
using namespace glm;


inline void initRainPositions(unsigned long MAX_PARTICLES, vec4 positions[], vec4 velociticies[])
{
	//init rain data
	default_random_engine rand(duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
	// Initilise particles
	for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
		int randX = (rand() % 800);
		int randY = rand() % 800;
		int randZ = (rand() % 800);
		switch (i % 4)
		{
		case 0:
			randX *= -1;
			break;
		case 1:
			randZ *= -1;
			break;
		case 2:
			randX *= -1;
			randZ *= -1;
		}
		positions[i] = vec4(randX, randY, randZ, 0.0f);
		int randSpeedY = -(rand() % 100 + 100);
		int randSpeedX = -(rand() % 100 + 50);
		int randSpeedZ = -(rand() % 10 + 1);
		velociticies[i] = vec4(randSpeedX, randSpeedY, randSpeedZ, 0.0f);
	}
}

inline void initRainBuffers(GLuint &vao, GLuint &G_Position_buffer, GLuint &G_Velocity_buffer, unsigned long MAX_PARTICLES, vec4 positions[], vec4 velociticies[])
{
	// a useless vao, but we need it bound or we get errors.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// *********************************
	//Generate Position Data buffer
	glGenBuffers(1, &G_Position_buffer);
	// Bind as GL_SHADER_STORAGE_BUFFER
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, G_Position_buffer);
	// Send Data to GPU, use GL_DYNAMIC_DRAW
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * MAX_PARTICLES, positions, GL_DYNAMIC_DRAW);

	// Generate Velocity Data buffer
	glGenBuffers(1, &G_Velocity_buffer);
	// Bind as GL_SHADER_STORAGE_BUFFER
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, G_Velocity_buffer);
	// Send Data to GPU, use GL_DYNAMIC_DRAW
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * MAX_PARTICLES, velociticies, GL_DYNAMIC_DRAW);
	// *********************************    
	//Unbind
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}


inline effect createBasicRainEffect()
{
	effect eff;
	eff.add_shader("shaders/rain.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/rain.geom", GL_GEOMETRY_SHADER);
	eff.add_shader("shaders/rain.frag", GL_FRAGMENT_SHADER);
	// Build effect
	eff.build();
	return eff;
}


inline effect createRainComputeShader()
{
	effect eff;
	eff.add_shader("shaders/particle.comp", GL_COMPUTE_SHADER);
	// Build effect
	eff.build();
	return eff;
}