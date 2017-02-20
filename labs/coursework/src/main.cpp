#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "meshes/katanaMesh.h"
#include "meshes/terrainMesh.h"
#include "effects/terrainEff.h"
#include "meshes/skyboxMesh.h"
#include "effects/skyboxEff.h"
#include "meshes/grassMesh.h"
#include "effects/normalMapEff.h"
#include "effects/multiLightEff.h"
#include "meshes/moonMesh.h"
#include "meshes/boatMesh.h"
#include "meshes/lampMesh.h"
#include "meshes/graveMesh.h"
#include "meshes/waterMesh.h"
#include "meshes/mossRockMesh.h"
#include "meshes/stoneSwordMesh.h"
#include "meshes/stoneGuardMesh.h"
#include "effects/movingWaterEff.h"
#include "effects/debrisEff.h"
#include "rendering/debrisTransforms.h"
#include "meshes/treeMesh.h"

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
vector<mat4> debrisOffset;

cubemap cube_map;

map<string, mesh> meshes;
map<string, texture> textures;
map<string, texture> normal_maps;
map<string, texture> alpha_maps;
map<string, effect> effects;


const int rotatingFloaterNumCrystal = 150;
const int rotatingFloaterNumAstCrystal = 100;
const int rotatingFloaterNumDebris = 250;

mesh crystal;
mesh asteroidCrystal;
mesh generalDebris;

mesh violet;
effect violetEffect;


vector<mat4> crystalOriginalTransforms(rotatingFloaterNumCrystal);
vector<mat4> crystalRotatingDebris(rotatingFloaterNumCrystal);

vector<mat4> asteroidCrystalOriginalTransforms(rotatingFloaterNumAstCrystal);
vector<mat4> asteroidCrystalRotatingDebris(rotatingFloaterNumAstCrystal);

vector<mat4> generalDebrisOriginalTransforms(rotatingFloaterNumDebris);
vector<mat4> generalDebrisRotatingDebris(rotatingFloaterNumDebris);

effect rotatingDebrisEffect;
vec3 gemPosition;

vec2 waterDelta;

float debrisRotation;

bool load_content() {

	// Generates the terrain and loads its textures
	auto width = 30;
	auto height = 30;
	auto heightScale = 3.0f;
	const texture height_map("textures/islandHMap.jpg");
	meshes["terrain"] = createTerrainMesh(height_map, width, height, heightScale);
	meshes["terrain"].get_material().set_specular(vec4(0, 0, 0, 0));
	meshes["terrain"].get_transform().scale = vec3(50, 50, 50);
	textures["terrainZero"] = texture("textures/sand.jpg", false, true);
	textures["terrainSand"] = texture("textures/sgrass.jpg", false, true);
	textures["terrainGrass"] = texture("textures/turf-grass.jpg", false, true);
	textures["terrainRock"] = texture("textures/overgrownRocks.jpg", false, true);
	normal_maps["terrainZero"] = texture("textures/terrainSand0.jpg", false, true);
	normal_maps["terrainSand"] = texture("textures/terrainSandOne.jpg", false, true);
	normal_maps["terrainGrass"] = texture("textures/terrainGrassNorm.png", false, true);
	normal_maps["terrainRock"] = texture("textures/terrainRockNorm.jpg", false, true);

	effects["terrain"] = createTerrainEffect();

	// Generates water and loads its textures
	meshes["waterBase"] = createWaterMesh();
	textures["waterBase"] = texture("textures/water2.jpg", false, true);
	normal_maps["waterBase"] = texture("textures/waterNormal.jpg", false, true);
	effects["waterBase"] = createMovingWaterEffect();

	// Generates the night skyboxs
	skybox = createSkybox();
	array<string, 6> filenames = { "textures/skybox/starfield_ft.tga", "textures/skybox/starfield_bk.tga", "textures/skybox/starfield_up.tga",
		"textures/skybox/starfield_dn.tga", "textures/skybox/starfield_rt.tga", "textures/skybox/starfield_lf.tga" };
	cube_map = cubemap(filenames);
	skyboxEffect = createSkyboxEffect();
	skybox.get_transform().scale = vec3(1000, 1000, 1000);

	// Generates lamp
	meshes["lamp"] = createLampMesh();
	textures["lamp"] = texture("textures/lampDiff.png", false, true);
	effects["lamp"] = createMultiLightEffect();

	// Generates the moon and loads its textures
	meshes["moon"] = createMoonMesh();
	textures["moon"] = texture("textures/moonTex.jpg", false, true);
	effects["moon"] = createMultiLightEffect();

	// Generates the boat and loads its textures
	meshes["boat"] = createBoatMesh();
	textures["boat"] = texture("textures/boatTex.png", false, true);
	normal_maps["boat"] = texture("textures/woodNorm.jpg", false, true);
	effects["boat"] = createNormalMapEffect();

	// Generates the grave and loads its textures
	meshes["grave"] = createGraveMesh();
	textures["grave"] = texture("textures/grave.png", false, true);
	effects["grave"] = createMultiLightEffect();

	// Generates the statue and loads its textures
	//meshes["statue"] = createStatueMesh();
	//textures["statue"] = texture("textures/buddha.jpg", false, true);
	//effects["statue"] = createMultiLightEffect();

	// Generates the statue and loads its textures
	violet = createTreeMesh();
	textures["violet"] = texture("textures/violet.png", false, true);
	alpha_maps["violet"] = texture("textures/violet_a.jpg", false, true);
	violetEffect = createMultiLightRemoveAlphaEffect();

	// Generates the katana and loads its textures
	meshes["katana"] = createKatanaMesh();
	textures["katana"] = texture("textures/katDiffuse.tga", false, true);
	effects["katana"] = createMultiLightEffect();

	// Generates the basic rock and loads its textures
	//meshes["normalRock"] = createRockMesh();
	//textures["normalRock"] = texture("textures/rock.jpg", false, true);
	//normal_maps["normalRock"] = texture("textures/rockNorm.png", false, true);
	//effects["normalRock"] = createMultiLightEffect();

	// Generates the mossyRock and loads its textures
	meshes["mossyRock"] = createMossyRockMesh();
	textures["mossyRock"] = texture("textures/mossyRock.jpg", false, true);
	normal_maps["mossyRock"] = texture("textures/mossyRockNorm.jpg", false, true);
	effects["mossyRock"] = createNormalMapEffect();

	// Generates the mossyRock and loads its textures
	meshes["guardian"] = createStoneGuardianMesh();
	textures["guardian"] = texture("textures/guardian.psd", false, true);
	normal_maps["guardian"] = texture("textures/guardianNorm.tga", false, true);
	effects["guardian"] = createNormalMapEffect();

	// Setup of the floating spheres dataon = 0.0f;
	crystal = mesh(geometry("models/crystal.obj"));
	asteroidCrystal = mesh(geometry("models/astCrystal.obj"));
	generalDebris = mesh(geometry("models/rock.obj"));

	textures["crystal"] = texture("textures/crystalDiffuse.jpg", false, true);
	normal_maps["crystal"] = texture("textures/chrystalNormal.png", false, true);
	gemPosition = vec3(150, 230, 210);
	createSpheresTransforms(crystalOriginalTransforms, rotatingFloaterNumCrystal, gemPosition, 0.075);
	createSpheresTransforms(asteroidCrystalOriginalTransforms, rotatingFloaterNumAstCrystal, gemPosition, 0.05);
	createSpheresTransforms(generalDebrisOriginalTransforms, rotatingFloaterNumDebris, gemPosition, 0.1);
	for (auto i = 0; i < rotatingFloaterNumCrystal; i++)
	{
		crystalRotatingDebris[i] = crystalOriginalTransforms[i];
	}
	for (auto i = 0; i < rotatingFloaterNumAstCrystal; i++)
	{
		asteroidCrystalRotatingDebris[i] = asteroidCrystalOriginalTransforms[i];
		
	}
	for (auto i = 0; i < rotatingFloaterNumDebris; i++)
	{
		generalDebrisRotatingDebris[i] = generalDebrisOriginalTransforms[i];
	}
	rotatingDebrisEffect = createMultiInstanceEffect();

	// Generates the mossyRock and loads its textures
	//meshes["ruins"] = createRuinsMesh();
	textures["ruins"] = texture("textures/grayStoneWall.jpg", false, true);
	effects["ruins"] = createMultiLightEffect();

	// Generates the mossyRock and loads its textures
	meshes["stoneSword"] = createStoneSwordMesh();
	textures["stoneSword"] = texture("textures/statueStone.jpg", false, true);
	normal_maps["stoneSword"] = texture("textures/bladeNorms.tga", false, true);
	effects["stoneSword"] = createNormalMapEffect();
	
	meshes["deadTree"] = createDeadTreeMesh();
	textures["deadTree"] = texture("textures/deadTree.jpg", false, true);
	normal_maps["deadTree"] = texture("textures/deadTreeNorm.jpg", false, true);
	effects["deadTree"] = createNormalMapEffect();

	// Set lighting values
	light.set_ambient_intensity(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	light.set_direction(normalize(meshes["boat"].get_transform().position));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// boat lantern
	points[0].set_position(meshes["lamp"].get_transform().position + vec3(0, 2, 0));
	points[0].set_light_colour(vec4(1, 0.6, 0, 1));
	points[0].set_range(75);

	// guardian gem point light
	points[1].set_position(vec3(170, 230, 217.5));
	points[1].set_light_colour(vec4(0.4, 0.6, 1, 1));
	points[1].set_range(350);

	// grave candles
	points[2].set_position(vec3(40, 45, 4.3));
	points[2].set_light_colour(vec4(1, 0.6, 0, 1));
	points[2].set_range(15);

	points[3].set_position(vec3(38, 45, 4.3));
	points[3].set_light_colour(vec4(1, 0.6, 0, 1));
	points[3].set_range(15);

	points[4].set_position(vec3(36, 45, 4.3));
	points[4].set_light_colour(vec4(1, 0.6, 0, 1));
	points[4].set_range(15);

	// spot onto the moon surface
	spots[0].set_position(vec3(-300, 365, 290));
	spots[0].set_light_colour(vec4(1, 1, 1, 1));
	spots[0].set_range(4);
	spots[0].set_power(1.0f);
	spots[0].set_direction(vec3(-350, 385, 285));

	// moon general light
	spots[1].set_position(vec3(-350, 385, 285));
	spots[1].set_light_colour(vec4(1, 1, 1, 1));
	spots[1].set_range(15);
	spots[1].set_power(1.0f);
	spots[1].set_direction(vec3(600, 100, -350));

	//guardian blue spot
	spots[2].set_position(vec3(170, 230, 217.5));
	spots[2].set_light_colour(vec4(0.1, 0.95, 0.9, 1));
	spots[2].set_range(5);
	spots[2].set_power(1.0f);
	spots[2].set_direction(vec3(170, 230, -300));

	// Set camera properties
	freeCam.set_position(vec3(400, 100, -300));
	freeCam.set_target(vec3(100, 100, 100));
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
		spots[0].set_position(freeCam.get_position());
	}
	if (glfwGetKey(renderer::get_window(), 'G')) {
		spots[0].set_direction(freeCam.get_position());
	}

	// updates the water delta vector to mimick the movement
	waterDelta += vec2(delta_time * 0.05, delta_time * 0.05);

	// Move camera
	freeCam.move(translation);
	// Update the camera
	freeCam.update(delta_time);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;

	auto positionVector = vec3(cos(debrisRotation)*100.5f, 0, sin(debrisRotation)*100.5f);
	for (int i = 0; i < rotatingFloaterNumCrystal; i++)
	{
		crystalRotatingDebris[i] = translate(crystalOriginalTransforms[i], positionVector * 2.0f + gemPosition);
	}
	positionVector = vec3(cos(debrisRotation)*10.5f, 0, sin(debrisRotation)*10.5f);
	for (int i = 0; i < rotatingFloaterNumAstCrystal; i++)
	{
		asteroidCrystalRotatingDebris[i] = translate(asteroidCrystalOriginalTransforms[i], positionVector + gemPosition);
	}
	for (int i = 0; i < rotatingFloaterNumDebris; i++)
	{
		generalDebrisRotatingDebris[i] = translate(generalDebrisOriginalTransforms[i], (positionVector * 3.0f) + gemPosition);
	}
	debrisRotation -= 1.0 * delta_time;

	// *********************************

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

void setupGeneralBindings(mesh &m, string meshName, effect &eff)
{
	// Sets uniform for water movement
	if (meshName == "waterBase")
	{
		glUniform2fv(eff.get_uniform_location("waterDelta"), 1, value_ptr(waterDelta));
	}

	// Set N matrix uniform - remember - 3x3 matrix
	auto normal = m.get_transform().get_normal_matrix();
	glUniformMatrix3fv(eff.get_uniform_location("N"),
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

	if (normal_maps.count(meshName) != 0)
	{
		renderer::bind(normal_maps[meshName], 1);
		glUniform1i(eff.get_uniform_location("normal_map"), 1);
	}
	if (alpha_maps.count(meshName) != 0)
	{
		glDisable(GL_CULL_FACE);
		renderer::bind(alpha_maps[meshName], 1);
		glUniform1i(eff.get_uniform_location("blendMap"), 1);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}
	if (meshName == "terrain")
	{
		// Bind Tex[0] to TU 0, set uniform
		renderer::bind(textures["terrainZero"], 0);
		glUniform1i(eff.get_uniform_location("tex[0]"), 0);
		// *********************************
		//Bind Tex[1] to TU 1, set uniform
		renderer::bind(textures["terrainSand"], 1);
		glUniform1i(eff.get_uniform_location("tex[1]"), 1);

		// Bind Tex[2] to TU 2, set uniform
		renderer::bind(textures["terrainGrass"], 2);
		glUniform1i(eff.get_uniform_location("tex[2]"), 2);

		// Bind Tex[3] to TU 3, set uniform
		renderer::bind(textures["terrainRock"], 3);
		glUniform1i(eff.get_uniform_location("tex[3]"), 3);

		// Bind normalMaps[0] to TU 0, set uniform
		renderer::bind(normal_maps["terrainZero"], 4);
		glUniform1i(eff.get_uniform_location("normal_maps[0]"), 4);
		// *********************************
		//Bind normal_maps[1] to TU 1, set uniform
		renderer::bind(normal_maps["terrainSand"], 5);
		glUniform1i(eff.get_uniform_location("normal_maps[1]"), 5);

		// Bind normal_maps[2] to TU 2, set uniform
		renderer::bind(normal_maps["terrainGrass"], 6);
		glUniform1i(eff.get_uniform_location("normal_maps[2]"), 6);

		// Bind normal_maps[3] to TU 3, set uniform
		renderer::bind(normal_maps["terrainRock"], 7);
		glUniform1i(eff.get_uniform_location("normal_maps[3]"), 7);

	}
	else
	{
		// Bind texture
		renderer::bind(textures[meshName], 0);

		// Set tex uniform
		glUniform1i(eff.get_uniform_location("tex"), 0);
	}
	glUniform3fv(
		eff.get_uniform_location("eye_pos"),
		1,
		value_ptr(freeCam.get_position())
		);
}

void renderMesh(mesh &m, string meshName, effect &eff)
{
	renderer::bind(eff);

	// Create MVP matrix
	auto M = m.get_transform().get_transform_matrix();
	auto V = freeCam.get_view();
	auto P = freeCam.get_projection();
	auto MVP = P * V * M;
	// Set MVP matrix uniform
	glUniformMatrix4fv(eff.get_uniform_location("MVP"),
		1,
		GL_FALSE,
		value_ptr(MVP));
	glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));

	setupGeneralBindings(m, meshName, eff);

	// Render mesh
	renderer::render(m);
}

void renderFloatingDebris(mesh &model, int amount, vector<mat4> &transforms, effect eff)
{
	// sets up the buffer
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &transforms[0], GL_STATIC_DRAW);

	GLuint VAO = model.get_geometry().get_array_object();
	glBindVertexArray(VAO);

	// sets up the other thingies
	
	renderer::bind(eff);

	auto V = freeCam.get_view();
	glUniformMatrix4fv(eff.get_uniform_location("view"), 1, GL_FALSE, value_ptr(V));
	auto P = freeCam.get_projection();
	glUniformMatrix4fv(eff.get_uniform_location("projection"), 1, GL_FALSE, value_ptr(P));

	setupGeneralBindings(model, "crystal", eff);
	renderer::render_instancieted(model, amount);
}

bool render() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render skybox
	renderSkybox();
	renderFloatingDebris(crystal, rotatingFloaterNumCrystal, crystalRotatingDebris, rotatingDebrisEffect);
	renderFloatingDebris(asteroidCrystal, rotatingFloaterNumAstCrystal, asteroidCrystalRotatingDebris, rotatingDebrisEffect);
	renderFloatingDebris(generalDebris, rotatingFloaterNumDebris, generalDebrisRotatingDebris, rotatingDebrisEffect);

	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		auto meshName = e.first;
		effect eff = effects[meshName];
		renderMesh(m, meshName, eff);
	}
	renderMesh(violet, "violet", violetEffect);
	return true;
}

void main() {
	// Create application
	app application("Coursework");
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}