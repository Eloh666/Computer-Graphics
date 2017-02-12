#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "meshes/katanaMesh.h"
#include "meshes/terrainMesh.h"
#include "effects/terrainEff.h"
#include "effects/katanaEff.h"
#include "meshes/skyboxMesh.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;


target_camera staticCam;
directional_light light;
vector<point_light> points(4);
vector<spot_light> spots(5);
free_camera freeCam;

mesh skybox;
effect skyboxEffect;
cubemap cube_map;

map<string, mesh> meshes;
map<string, texture> textures;
map<string, effect> effects;

bool load_content() {

	// Generates the terrain and loads its textures
	meshes["terrain"] = createTerrainMesh(20, 20, 2.0f);
	textures["water"] = texture("textures/water.jpg");
	textures["sand"] = texture("textures/sand.jpg");
	textures["grass"] = texture("textures/grass.jpg");
	textures["snow"] = texture("textures/snow.jpg");
	effects["terrain"] = createTerrainEffect();

	// Generates the night skybox
	skybox = createSkybox();
	//array<string, 6> filenames = { "textures/skybox/lagoon_ft.tga", "textures/skybox/lagoon_bk.tga", "textures/skybox/lagoon_up.tga",
		//"textures/skybox/lagoon_dn.tga", "textures/skybox/lagoon_rt.tga", "textures/skybox/lagoon_lf.tga" };
	array<string, 6> filenames = { "textures/sahara_ft.jpg", "textures/sahara_bk.jpg", "textures/sahara_up.jpg",
		"textures/sahara_dn.jpg", "textures/sahara_rt.jpg", "textures/sahara_lf.jpg" };
	cube_map = cubemap(filenames);
	skyboxEffect = createTerrainEffect();


	// Generates the katana and loads its textures
	//meshes["katana"] = createKatanaMesh();
	//textures["katana"] = texture("textures/katDiffuse.tga");
	//effects["katana"] = createKatanaEffect();
	
	// Set lighting values
	light.set_ambient_intensity(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	light.set_direction(vec3(1.0f, 1.0f, -1.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	

	// Set camera properties
	freeCam.set_position(vec3(0.0f, 5.0f, 10.0f));
	freeCam.set_target(vec3(0.0f, 0.0f, 0.0f));
	freeCam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

bool update(float delta_time) {
	// The ratio of pixels to rotation - remember the fov
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() * renderer::get_screen_aspect()) / static_cast<float>(renderer::get_screen_height());
	static double cursor_x = 0.0;
	static double cursor_y = 0.0;
	double current_x;
	double current_y;
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios - gets actual change in orientation
	delta_x *= ratio_width;
	delta_y *= ratio_height;
	// Rotate cameras by delta
	freeCam.rotate(delta_x, -delta_y);
	// Use keyboard to move the camera - WASD
	vec3 translation(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(renderer::get_window(), 'W')) {
		translation.z += 5.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		translation.z -= 5.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		translation.x -= 5.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		translation.x += 5.0f * delta_time;
	}
	// Move camera
	freeCam.move(translation);
	// Update the camera
	freeCam.update(delta_time);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
	return true;
}

void renderSkybox()
{
	glDisable(GL_DEPTH_TEST);

	// Bind skybox effect
	renderer::bind(skyboxEffect);

	// Calculate MVP for the skybox

	auto M = skybox.get_transform().get_transform_matrix();
	auto V = freeCam.get_view();
	auto P = freeCam.get_projection();
	auto MVP = P * V * M;


	// Set MVP matrix uniform
	glUniformMatrix4fv(skyboxEffect.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

	// Set cubemap uniform
	renderer::bind(cube_map, 0);
	renderer::bind(cube_map, 0);

	// Render skybox
	renderer::render(skybox);

	// Enable depth test,depth mask,face culling
	glEnable(GL_DEPTH_TEST);
}

bool render() {

	// Render skybox
	renderSkybox();

	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		auto meshName = e.first;
		// Bind effect
		renderer::bind(effects[meshName]);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = freeCam.get_view();
		auto P = freeCam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(effects[meshName].get_uniform_location("MVP"), // Location of uniform
			1,                               // Number of values - 1 mat4
			GL_FALSE,                        // Transpose the matrix?
			value_ptr(MVP));                 // Pointer to matrix data
											 // *********************************
											 // Set M matrix uniform
		glUniformMatrix4fv(effects[meshName].get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

		// Set N matrix uniform - remember - 3x3 matrix
		auto normal = m.get_transform().get_normal_matrix();
		glUniformMatrix3fv(effects[meshName].get_uniform_location("N"),
			1,
			GL_FALSE,
			value_ptr(normal));

		// Bind material
		renderer::bind(m.get_material(), "mat");

		// Bind point lights
		renderer::bind(spots, "spots");

		// Bind spot lights
		renderer::bind(points, "points");

		// Bind env light
		renderer::bind(light, "light");

		if(meshName == "terrain")
		{
			// Bind Tex[0] to TU 0, set uniform
			renderer::bind(textures["water"], 0);
			glUniform1i(effects[meshName].get_uniform_location("tex[0]"), 0);
			// *********************************
			//Bind Tex[1] to TU 1, set uniform
			renderer::bind(textures["sand"], 1);
			glUniform1i(effects[meshName].get_uniform_location("tex[1]"), 1);

			// Bind Tex[2] to TU 2, set uniform
			renderer::bind(textures["grass"], 2);
			glUniform1i(effects[meshName].get_uniform_location("tex[2]"), 2);

			// Bind Tex[3] to TU 3, set uniform
			renderer::bind(textures["snow"], 3);
			glUniform1i(effects[meshName].get_uniform_location("tex[3]"), 3);

		} else
		{
			// Bind texture
			renderer::bind(textures[meshName], 0);

			// Set tex uniform
			glUniform1i(effects[meshName].get_uniform_location("tex"), 0);
		}

		// Set eye position - Get this from active camera
		glUniform3fv(
			effects[meshName].get_uniform_location("eye_pos"),
			1,
			value_ptr(freeCam.get_position())
			);
		// Render mesh
		renderer::render(m);

		// *********************************

		// Render mesh
		renderer::render(m);
	}

	return true;
}

void main() {
	// Create application
	app application("52_Multifile_Shaders");
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}