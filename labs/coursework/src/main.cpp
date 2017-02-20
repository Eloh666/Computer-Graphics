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
#include "lights/setupLights.h"
#include "cameras/setupCameras.h"
#include "meshes/amillaryMesh.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;


// lights
directional_light light;
vector<point_light> points(6);
vector<spot_light> spots(5);

// cameras
free_camera freeCam;
camera *activeCam;
vector<target_camera> targetCameras(7);
chase_camera chaseCamera;


// meshes
map<string, mesh> meshes;

mesh violet;
mesh skybox;
mesh crystal;
mesh asteroidCrystal;
mesh generalDebris;

//effects
map<string, effect> effects;

effect skyboxEffect;
effect violetEffect;
effect rotatingDebrisEffect;

// textures
map<string, texture> textures;
map<string, texture> normal_maps;
map<string, texture> alpha_maps;
cubemap cube_map;

// transforms and indeces for the debris ring

vec3 gemPosition;
float rotationAngle;

const int rotatingFloaterNumDebris = 500;

vector<mat4> generalDebrisOriginalTransforms(rotatingFloaterNumDebris);
vector<mat4> generalDebrisRotatingDebris(rotatingFloaterNumDebris);

// water flowing delta

vec2 waterDelta;

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

	// Generates lantern
	meshes["lantern"] = createLampMesh();
	textures["lantern"] = texture("textures/lampDiff.png", false, true);
	effects["lantern"] = createMultiLightEffect();

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
	violet = createTreeMesh();
	textures["violet"] = texture("textures/violet.png", false, true);
	alpha_maps["violet"] = texture("textures/violet_a.jpg", false, true);
	violetEffect = createMultiLightRemoveAlphaEffect();

	// Generates the katana and loads its textures
	meshes["katana"] = createKatanaMesh();
	textures["katana"] = texture("textures/katDiffuse.tga", false, true);
	effects["katana"] = createMultiLightEffect();

	// Generates the basic rock and loads its textures
	meshes["amillary"] = createAmillaryMesh();
	textures["amillary"] = texture("textures/metal.jpg", false, true);
	normal_maps["amillary"] = texture("textures/metalNorm.jpg", false, true);
	effects["amillary"] = createMultiLightEffect();

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
	createSpheresTransforms(generalDebrisOriginalTransforms, rotatingFloaterNumDebris, gemPosition, 0.05f);
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

	// Set sets up lightning values
	light.set_ambient_intensity(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	light.set_direction(normalize(meshes["boat"].get_transform().position));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	initPointLights(points);
	initSpotLights(spots);

	// Set camera properties

	setupFreeCam(freeCam);
	setupTargetCameras(targetCameras, meshes);
	activeCam = &freeCam;

	
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

bool update(float delta_time) {

	// updates the water delta vector to mimick the movement
	waterDelta += vec2(delta_time * 0.05, delta_time * 0.05);



	// The ratio of pixels to rotation - remember the fov
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
	if (glfwGetKey(renderer::get_window(), 'B')) {
		cout << activeCam->get_position().x << endl;
		cout << activeCam->get_position().y << endl;
		cout << activeCam->get_position().z << endl;
	}

	// activates free cam
	if (glfwGetKey(renderer::get_window(), 'F')) {
		activeCam = &freeCam;
	}
	// activates chase cam
	if (glfwGetKey(renderer::get_window(), 'C')) {
		activeCam = &chaseCamera;
	}

	// activates static target cameras
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F1)) {
		activeCam = &targetCameras[0];
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F2)) {
		activeCam = &targetCameras[1];
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F3)) {
		activeCam = &targetCameras[2];
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F4)) {
		activeCam = &targetCameras[3];
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F5)) {
		activeCam = &targetCameras[4];
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F6)) {
		activeCam = &targetCameras[5];
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F7)) {
		activeCam = &targetCameras[6];
	}

	// handles free camera movement if active
	if(activeCam == &freeCam)
	{
		freeCam.set_target(activeCam->get_target());
		handleFreeCameraMovement(freeCam, delta_time, translation);
	}

	// Update the camera
	activeCam->update(delta_time);


	// Rotates the amillary around the statue
	float amillaryRotation = rotationAngle * 0.001f;
	auto rotationPosition = vec3(cos(rotationAngle * 0.5f)*140.0f, 0, sin(rotationAngle * 0.5f)*140.0f);


	meshes["amillary"].get_transform().rotate(vec3(amillaryRotation, amillaryRotation, amillaryRotation));
	meshes["amillary"].get_transform().position = rotationPosition + gemPosition + vec3(0, 25, 0);
	points[5].set_position(meshes["amillary"].get_transform().position);

	// Debris cloud
	// Small floaters
	rotationPosition = vec3(cos(rotationAngle)*5.0f, -50, sin(rotationAngle)*5.0f);
	for (int i = 0; i < rotatingFloaterNumDebris; i++)
	{
		generalDebrisRotatingDebris[i] = translate(generalDebrisOriginalTransforms[i], rotationPosition  + meshes["amillary"].get_transform().position);
	}
	rotationAngle -= 1.0 * delta_time;

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
	auto V = activeCam->get_view();
	auto P = activeCam->get_projection();
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
		value_ptr(activeCam->get_position())
		);
}

void renderMesh(mesh &m, string meshName, effect &eff)
{
	renderer::bind(eff);

	// Create MVP matrix
	auto M = m.get_transform().get_transform_matrix();
	auto V = activeCam->get_view();
	auto P = activeCam->get_projection();
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

	auto V = activeCam->get_view();
	glUniformMatrix4fv(eff.get_uniform_location("view"), 1, GL_FALSE, value_ptr(V));
	auto P = activeCam->get_projection();
	glUniformMatrix4fv(eff.get_uniform_location("projection"), 1, GL_FALSE, value_ptr(P));

	setupGeneralBindings(model, "crystal", eff);
	renderer::render_instancieted(model, amount);
}

bool render() {

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render skybox
	renderSkybox();
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