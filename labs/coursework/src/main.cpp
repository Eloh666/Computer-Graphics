#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "meshes/katanaMesh.h"
#include "meshes/terrainMesh.h"
#include "effects/terrainEff.h"
#include "meshes/skyboxMesh.h"
#include "effects/skyboxEff.h"
#include "meshes/grassMesh.h"
#include "effects/normalMapEff.h"
#include "meshes/treeMesh.h"
#include "effects/multiLightEff.h"
#include "meshes/moonMesh.h"
#include "meshes/boatMesh.h"
#include "meshes/lampMesh.h"
#include "meshes/graveMesh.h"
#include "meshes/statueMesh.h"
#include "meshes/waterMesh.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;


target_camera staticCam;
directional_light light;
vector<point_light> points(5);
vector<spot_light> spots(5);
free_camera freeCam;

mesh skybox;
mesh grass;
effect skyboxEffect;
cubemap cube_map;

map<string, mesh> meshes;
map<string, texture> textures;
map<string, texture> normal_maps;
map<string, texture> alpha_maps;
map<string, effect> effects;

bool load_content() {

	// Generates the terrain and loads its textures
	auto width = 30;
	auto height = 30;
	auto heightScale = 3.0f;
	const texture height_map("textures/islandHMap.jpg");
	meshes["terrain"] = createTerrainMesh(height_map, width, height, heightScale);
	meshes["terrain"].get_material().set_specular(vec4(0, 0, 0, 0));
	meshes["terrain"].get_transform().scale = vec3(50, 50, 50);
	textures["water"] = texture("textures/sand.jpg", false, true);
	textures["terrainSand"] = texture("textures/sgrass.jpg", false, true);
	textures["terrainGrass"] = texture("textures/turf-grass.jpg", false, true);
	textures["rock"] = texture("textures/grass.jpg", false, true);
	
	effects["terrain"] = createTerrainEffect();


	// Generates water and loads its textures
	meshes["waterBase"] = createWaterMesh();
	textures["waterBase"] = texture("textures/water.jpg", false, true);
	normal_maps["waterBase"] = texture("textures/watNorm.png", false, true);
	//effects["waterBase"] = createNormalMapEffect();
	effects["waterBase"] = createTerrainEffect();

	// Generates the night skyboxs
	skybox = createSkybox();
	array<string, 6> filenames = { "textures/skybox/starfield_ft.tga", "textures/skybox/starfield_bk.tga", "textures/skybox/starfield_up.tga",
		"textures/skybox/starfield_dn.tga", "textures/skybox/starfield_rt.tga", "textures/skybox/starfield_lf.tga" };
	cube_map = cubemap(filenames);
	skyboxEffect = createSkyboxEffect();

	skybox.get_transform().scale = vec3(1000, 1000, 1000);
	//skybox.get_transform().translate(meshes["terrain"].get_transform().position + vec3(0, 0, 0));

	// Generates lampw
	meshes["lamp"] = createLampMesh();
	textures["lamp"] = texture("textures/lampDiff.png", false, true);
	//normal_maps["lamp"] = texture("textures/lampNorm.png");
	effects["lamp"] = createMultiLightEffect();

	// Generates the moon and loads its textures
	meshes["moon"] = createMoonMesh();
	textures["moon"] = texture("textures/moonTex.jpg", false, true);
	effects["moon"] = createMultiLightEffect();

	// Generates the boat and loads its textures
	meshes["boat"] = createBoatMesh();
	textures["boat"] = texture("textures/boatTex.png", false, true);
	effects["boat"] = createMultiLightEffect();

	// Generates the grave and loads its textures
	meshes["grave"] = createGraveMesh();
	textures["grave"] = texture("textures/grave.png", false, true);
	effects["grave"] = createMultiLightEffect();

	// Generates the statue and loads its textures
	meshes["statue"] = createStatueMesh();
	textures["statue"] = texture("textures/buddha.jpg", false, true);
	//normal_maps["statue"] = texture("textures/buddhaN.jpg");
	effects["statue"] = createMultiLightEffect();

	// Generates the katana and loads its textures
	meshes["katana"] = createKatanaMesh();
	textures["katana"] = texture("textures/katDiffuse.tga", false, true);
	//normal_maps["katana"] = texture("textures/katBump.tga");
	effects["katana"] = createMultiLightEffect();

	// Generates the tree and loads its textures
	//meshes["tree"] = createTree();
	//textures["tree"] = texture("textures/palm.tga", false, true);
	//alpha_maps["tree"] = texture("textures/palmA.jpeg", false, true);
	//normal_maps["tree"] = texture("textures/treeNorm.tga");
	//effects["tree"] = createMultiLightEffect();

	// Set lighting values
	light.set_ambient_intensity(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	light.set_direction(normalize(meshes["boat"].get_transform().position));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//points[0].set_position(vec3(37, 43.5, 0));
	points[0].set_position(vec3(158, 29, -90));
	points[0].set_light_colour(vec4(1, 0.6, 0, 1));
	points[0].set_range(75);

	//points[1].set_position(meshes["moon"].get_transform().position);
	//points[1].set_light_colour(vec4(1, 1, 1, 1));
	//points[1].set_range(250);

	points[2].set_position(vec3(40, 45, 4.3));
	points[2].set_light_colour(vec4(1, 0.6, 0, 1));
	points[2].set_range(15);

	points[3].set_position(vec3(38, 45, 4.3));
	points[3].set_light_colour(vec4(1, 0.6, 0, 1));
	points[3].set_range(15);

	points[4].set_position(vec3(36, 45, 4.3));
	points[4].set_light_colour(vec4(1, 0.6, 0, 1));
	points[4].set_range(15);


	spots[0].set_position(vec3(281, 324, 205));
	spots[0].set_light_colour(vec4(1, 1, 1, 1));
	spots[0].set_range(5);
	spots[0].set_power(1.0f);
	spots[0].set_direction(vec3(350, 362, 242));

	spots[1].set_position(vec3(231, 274, 205));
	spots[1].set_light_colour(vec4(1, 1, 1, 1));
	spots[1].set_range(15);
	spots[1].set_power(1.0f);
	spots[1].set_direction(vec3(-400, 0, -400));

	// Set camera properties
	freeCam.set_position(vec3(-100, 145, -500));
	freeCam.set_target(normalize(meshes["katana"].get_transform().position));
	freeCam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 3000.0f);
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
		translation.z += 50.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		translation.z -= 50.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		translation.x -= 50.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		translation.x += 50.0f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), '1')) {
		spots[1].move(vec3(5.0f * delta_time, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), '2')) {
		spots[1].move(vec3(-5.0f * delta_time, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), '3')) {
		spots[1].move(vec3(0.0f, 5.0f * delta_time, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), '4')) {
		spots[1].move(vec3(0.0f, -5.0f * delta_time, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), '5')) {
		spots[1].move(vec3(0.0f, 0.0f, 5.0f * delta_time));
	}
	if (glfwGetKey(renderer::get_window(), '6')) {
		spots[1].move(vec3(0.0f, 0.0f, -5.0f * delta_time));
	}
	if (glfwGetKey(renderer::get_window(), 'Q')) {
		cout << spots[1].get_position().x << endl;
		cout << spots[1].get_position().y << endl;
		cout << spots[1].get_position().z << endl;
	}
	if (glfwGetKey(renderer::get_window(), 'M')) {
		cout << freeCam.get_position().x << endl;
		cout << freeCam.get_position().y << endl;
		cout << freeCam.get_position().z << endl;
	}
	if (glfwGetKey(renderer::get_window(), 'N')) {
		spots[1].set_position(freeCam.get_position());
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

	// Render skybox
	renderer::render(skybox);

	// Enable depth test,depth mask,face culling
	glEnable(GL_DEPTH_TEST);
}

void renderMesh(mesh &m, string meshName)
{
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

	if (normal_maps.count(meshName) != 0 )
	{
		renderer::bind(normal_maps[meshName], 1);
		glUniform1i(effects[meshName].get_uniform_location("normal_map"), 1);
	}
	if (alpha_maps.count(meshName) != 0)
	{
		glDisable(GL_CULL_FACE);
		renderer::bind(alpha_maps[meshName], 1);
		glUniform1i(effects[meshName].get_uniform_location("blendMap"), 1);
	} else
	{
		glEnable(GL_CULL_FACE);
	}
	if (meshName == "terrain")
	{
		// Bind Tex[0] to TU 0, set uniform
		renderer::bind(textures["water"], 0);
		glUniform1i(effects[meshName].get_uniform_location("tex[0]"), 0);
		// *********************************
		//Bind Tex[1] to TU 1, set uniform
		renderer::bind(textures["terrainSand"], 1);
		glUniform1i(effects[meshName].get_uniform_location("tex[1]"), 1);

		// Bind Tex[2] to TU 2, set uniform
		renderer::bind(textures["terrainGrass"], 2);
		glUniform1i(effects[meshName].get_uniform_location("tex[2]"), 2);

		// Bind Tex[3] to TU 3, set uniform
		renderer::bind(textures["rock"], 3);
		glUniform1i(effects[meshName].get_uniform_location("tex[3]"), 3);

	}
	else
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

bool render() {
	// Render skybox
	renderSkybox();

	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		auto meshName = e.first;
		renderMesh(m, meshName);
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