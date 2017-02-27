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
#include "meshes/statueMesh.h"
#include "meshes/ruinsMesh.h"
#include "shadows/setupShadows.h"

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
vector<target_camera> targetCameras(6);
chase_camera chaseCamera;


// meshes
map<string, mesh> meshes;

mesh trees;
mesh skybox;
mesh crystal;

//effects
map<string, effect> effects;

effect skyboxEffect;
effect treeEffect;
effect multiIstanceNormalEffect;

// textures
map<string, texture> textures;
map<string, texture> normal_maps;
map<string, texture> alpha_maps;
cubemap cube_map;

// transforms and indeces for the debris ring

vec3 gemPosition;
float rotationAngle;

const int rotatingFloaterNumDebris = 1000;

vector<mat4> generalDebrisOriginalTransforms(rotatingFloaterNumDebris);
vector<mat4> generalDebrisRotatingDebris(rotatingFloaterNumDebris);

// water flowing delta
vec2 waterDelta;

// shadows
shadow_map shadow;
effect shadowEff;
effect generalEffect;

// floating amillary
int amillaryRingsNumber = 6;
mesh amillaryRing;
vector<mat4> amillaryTransforms(amillaryRingsNumber);

// trees
int treesAmount = 40;
vector<mat4> treeTransforms(treesAmount);

bool load_content() {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Create shadow map- use screen size
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
	shadowEff = createMultiLightEffect();
	generalEffect = createMultiLightEffect();

	// amillary ring
	amillaryRing = mesh(geometry("models/amillaryRing.obj"));
	amillaryRing.get_material().set_shininess(25.0f);

	// tree positions
	generateTreesTransforms(treeTransforms);

	// Generates the terrain and loads its textures
	auto width = 30;
	auto height = 30;
	auto heightScale = 3.0f;
	const texture height_map("textures/islandHMap.jpg");
	meshes["terrain"] = createTerrainMesh(height_map, width, height, heightScale);
	//meshes["terrain"].get_material().set_specular(vec4(0.57, 0.3, 0.1, 0.3));
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
	textures["waterBase"] = texture("textures/water2k.jpg", false, true);
	normal_maps["waterBase"] = texture("textures/waterNormal.jpg", false, true);
	effects["waterBase"] = createMovingWaterEffect();

	// Generates the night skyboxss
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

	// Generates the moon and loads its textures
	meshes["buddha"] = createStatueMesh();
	textures["buddha"] = texture("textures/buddha.jpg", false, true);
	normal_maps["buddha"] = texture("textures/buddhaN.jpg", false, true);
	effects["buddha"] = createMultiLightEffect();

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
	meshes["violet"] = createVioletTreeMesh();
	textures["violet"] = texture("textures/violet.png", false, true);
	alpha_maps["violet"] = texture("textures/violet_a.jpg", false, true);
	effects["violet"] = createMultiLightRemoveAlphaEffect();

	trees = createTreeMesh();
	textures["tree"] = texture("textures/treeDiff.tga", false, true);
	alpha_maps["tree"] = texture("textures/treeAlpha.tga", false, true);
	normal_maps["tree"] = texture("textures/treeNorm.tga", false, true);
	treeEffect = createMultiInstanceRemoveAlphaEffect();

	// Generates the katana and loads its textures
	meshes["katana"] = createKatanaMesh();
	textures["katana"] = texture("textures/katDiffuse.tga", false, true);
	effects["katana"] = createMultiLightEffect();

	// Generates the basic rock and loads its textures
	meshes["amillary"] = createAmillaryMesh();
	textures["amillary"] = texture("textures/metal.jpg", false, true);
	normal_maps["amillary"] = texture("textures/metalNorm.jpg", false, true);
	effects["amillary"] = createNormalMapEffect();

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
	crystal = mesh(geometry("models/meteor.obj"));

	textures["crystal"] = texture("textures/crystalDiffuse.jpg", false, true);
	normal_maps["crystal"] = texture("textures/meteorNorm.tga", false, true);
	gemPosition = meshes["guardian"].get_transform().position + vec3(0, 35, 0);
	createFloatersTransforms(generalDebrisOriginalTransforms, rotatingFloaterNumDebris, gemPosition, 0.15f);
	for (auto i = 0; i < rotatingFloaterNumDebris; i++)
	{
		generalDebrisRotatingDebris[i] = generalDebrisOriginalTransforms[i];
	}
	multiIstanceNormalEffect = createMultiInstanceEffect();

	// Generates the mossyRock and loads its textures
	meshes["ruins"] = createRuinsMesh();
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
	light.set_ambient_intensity(vec4(0, 0, 0, 0));
	light.set_direction(normalize(meshes["boat"].get_transform().position));
	light.set_light_colour(vec4(0, 0, 0, 0));

	initPointLights(points);
	initSpotLights(spots);

	// Set camera properties

	setupFreeCam(freeCam);
	setupTargetCameras(targetCameras, meshes);
	setupChaseCamera(chaseCamera, meshes["amillary"]);
	activeCam = &freeCam;


	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}

bool update(float delta_time) {

	// updates the water delta vector to mimick the movement
	waterDelta += vec2(delta_time * 0.05, delta_time * 0.05);

	// updates the projection of the light
	shadow.light_position = spots[1].get_position();
	shadow.light_dir = spots[1].get_direction();

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
	if (glfwGetKey(renderer::get_window(), 'M')) {
		cout << freeCam.get_position().x << endl;
		cout << freeCam.get_position().y << endl;
		cout << freeCam.get_position().z << endl;
	}

	// activates free cam
	if (glfwGetKey(renderer::get_window(), '1')) {
		spots[1].rotate(vec3(1, 0, 0) * delta_time);
	}
	// activates free cam
	if (glfwGetKey(renderer::get_window(), '2')) {
		spots[1].rotate(-vec3(1, 0, 0) * delta_time);	}
	// activates free cam
	if (glfwGetKey(renderer::get_window(), '3')) {
		spots[1].rotate(vec3(0, 1, 0) * delta_time);	}
	// activates free cam
	if (glfwGetKey(renderer::get_window(), '4')) {
		spots[1].rotate(-vec3(0, 1, 0) * delta_time);	}
	// activates free cam
	if (glfwGetKey(renderer::get_window(), '5')) {
		spots[1].rotate(vec3(0, 0, 1) * delta_time);
	}
	// activates free cam
	if (glfwGetKey(renderer::get_window(), '6')) {
		spots[1].rotate(-vec3(0, 0, 1) * delta_time);
	}

	// activates free cam
	if (glfwGetKey(renderer::get_window(), 'F')) {
		activeCam = &freeCam;
	}
	// activates chase cam
	if (glfwGetKey(renderer::get_window(), 'C')) {
		activeCam = &chaseCamera;
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_L)) {
		light.set_light_colour(vec4(1, 1, 1, 1));
		light.set_ambient_intensity(vec4(0.1, 0.1, 0.1, 1));
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_K)) {
		light.set_light_colour(vec4(0, 0, 0, 0));
		light.set_ambient_intensity(vec4(0, 0, 0, 0));
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
		points[5].set_light_colour(vec4(0.4, 0.6, 1, 1));
		meshes["amillary"].get_material().set_emissive(vec4(0.1, 0.95, 0.9, 1));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F8)) {
		points[5].set_light_colour(vec4(1, 0.85, 0, 1));
		meshes["amillary"].get_material().set_emissive(vec4(1, 0.85, 0, 1));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F9)) {
		points[5].set_light_colour(vec4(1, 0, 0, 1));
		meshes["amillary"].get_material().set_emissive(vec4(1, 0, 0, 1));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_F10)) {
		points[5].set_light_colour(vec4(0, 1, 0, 1));
		meshes["amillary"].get_material().set_emissive(vec4(0, 1, 0, 1));
	}

	// handles free camera movement if active
	if (activeCam == &freeCam)
	{
		freeCam.set_target(activeCam->get_target());
		handleFreeCameraMovement(freeCam, delta_time, translation);
	}
	if (activeCam == &chaseCamera)
	{
		handleChaseCameraMovement(chaseCamera, delta_time, meshes["amillary"]);
	}

	// Update the camera
	activeCam->update(delta_time);

	// Rotates the amillary around the statue
	float amillaryRotation = sin(rotationAngle) * 0.01f;
	auto rotationPosition = vec3(cos(rotationAngle * 0.35f)*155.0f, 0, sin(rotationAngle * 0.35f)*155.0f);


	meshes["amillary"].get_transform().rotate(vec3(amillaryRotation, amillaryRotation, amillaryRotation));
	meshes["amillary"].get_transform().position = rotationPosition + gemPosition;
	points[5].set_position(meshes["amillary"].get_transform().position);

	// Debris cloud
	// Small floaters
	rotationPosition = vec3(cos(rotationAngle)*15.0f, 0, sin(rotationAngle)*15.0f);
	for (int i = 0; i < rotatingFloaterNumDebris; i++)
	{
		float diff = -1 * ((i % 2) + 1);
		generalDebrisRotatingDebris[i] = translate(generalDebrisOriginalTransforms[i], rotationPosition * diff);
	}
	rotationAngle -= delta_time;

	// Setup and rotation for amillary transforms
	generateAmillaryRings(amillaryTransforms, meshes["amillary"].get_transform().get_transform_matrix(), rotationAngle * 0.5);

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
		renderer::bind(alpha_maps[meshName], 2);
		glUniform1i(eff.get_uniform_location("blendMap"), 2);
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

	// Bind eye position
	glUniform3fv(
		eff.get_uniform_location("eye_pos"),
		1,
		value_ptr(activeCam->get_position())
		);

	// Bind shadow map texture - using available index
	renderer::bind(shadow.buffer->get_depth(), 15);
	glUniform1i(eff.get_uniform_location("shadow_map"), 15);
}

void renderMesh(mesh &m, string meshName, effect &eff, mat4 lightProjectionMatrix)
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

	auto lightMVP = lightProjectionMatrix * shadow.get_view() * M;
	// Set light transform
	glUniformMatrix4fv(eff.get_uniform_location("lightMVP"),
		1,
		GL_FALSE,
		value_ptr(lightMVP));

	setupGeneralBindings(m, meshName, eff);

	// Render mesh
	renderer::render(m);
}

void renderInstanciatedMesh(mesh &model, int amount, vector<mat4> &transforms, effect eff, string name, mat4 lightProjectionMatrix)
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

	auto lightMVPPartial = lightProjectionMatrix * shadow.get_view();
	// Set light transform
	glUniformMatrix4fv(eff.get_uniform_location("lightMVPPartial"),
		1,
		GL_FALSE,
		value_ptr(lightMVPPartial));

	setupGeneralBindings(model, name, eff);
	renderer::render_instancieted(model, amount);
}

void renderShadows(mat4 lightProjectionMatrix)
{
	// Set render target to shadow map
	renderer::set_render_target(shadow);
	// Clear depth buffer bit
	glClear(GL_DEPTH_BUFFER_BIT);
	// Set render mode to cull face
	glCullFace(GL_FRONT);

	// Bind shader
	renderer::bind(shadowEff);
	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		// *********************************
		// View matrix taken from shadow map
		auto V = shadow.get_view();
		// *********************************
		auto MVP = lightProjectionMatrix * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(shadowEff.get_uniform_location("MVP"), // Location of uniform
			1,                                      // Number of values - 1 mat4
			GL_FALSE,                               // Transpose the matrix?
			value_ptr(MVP));                        // Pointer to matrix data
													// Render mesh
		renderer::render(m);
	}
	// Set render target back to the screen
	renderer::set_render_target();
	// Set cull face to back
	glCullFace(GL_BACK);
}

bool render() {

	mat4 lightProjectionMatrix = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 50000.f);

	// Render Shadows
	renderShadows(lightProjectionMatrix);
	// Render skybox
	renderSkybox();
	renderInstanciatedMesh(crystal, rotatingFloaterNumDebris, generalDebrisRotatingDebris, multiIstanceNormalEffect, "crystal", lightProjectionMatrix);
	renderInstanciatedMesh(amillaryRing, amillaryTransforms.size(), amillaryTransforms, multiIstanceNormalEffect, "amillary", lightProjectionMatrix);
	renderInstanciatedMesh(trees, treesAmount, treeTransforms, treeEffect, "tree", lightProjectionMatrix);

	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		auto meshName = e.first;
		effect eff = effects[meshName];
		renderMesh(m, meshName, eff, lightProjectionMatrix);
	}
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