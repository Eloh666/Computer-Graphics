#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "meshes/terrainMesh.h"
#include "rendering/debrisTransforms.h"
#include "lights/setupLights.h"
#include "cameras/setupCameras.h"
#include "particles/particles.h"
#include "textures/loadTextures.h"
#include "effects/loadEffects.h"
#include "meshes/loadMeshes.h"

using namespace std;
using namespace chrono;
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
map<string, mesh> specialRenderMeshes;

//effects
map<string, effect> effects;

// textures
map<string, texture> textures;
map<string, texture> normal_maps;
map<string, texture> alpha_maps;
cubemap cube_map;

// transforms and indeces for the debris ring

vec3 gemPosition;
float rotationAngle;

const int rotatingFloaterNumDebris = 2000;

vector<mat4> generalDebrisOriginalTransforms(rotatingFloaterNumDebris);
vector<mat4> generalDebrisRotatingDebris(rotatingFloaterNumDebris);

// water flowing delta
vec2 waterDelta;

// floating amillary
int amillaryRingsNumber = 6;
vector<mat4> amillaryTransforms(amillaryRingsNumber);

// trees
int treesAmount = 40;
vector<mat4> treeTransforms(treesAmount);

// shadows
shadow_map shadow;
effect shadowEff;
bool shouldRenderShadows;

// frame data and post-processing
frame_buffer frames[2];
frame_buffer temp_frame;
unsigned int current_frame = 0;
geometry screen_quad;
float motionBlurCoeff = 0;

// Grass Data
const unsigned long MAX_PARTICLES = 2 << 15;

vec4 positions[MAX_PARTICLES];
vec4 velocitys[MAX_PARTICLES];
GLuint G_Position_buffer, G_Velocity_buffer;
effect rainEffect;
effect compute_eff;
GLuint vao;

// Grass Data
geometry grassGeom;


bool load_content()
{
	// load the meshes
	loadMeshes(meshes);
	loadSpecialRenderMeshes(specialRenderMeshes);

	// loads the textures inside their dictinaries
	loadTextures(textures);
	loadNormalMaps(normal_maps);
	loadAlphaMaps(alpha_maps);

	// loads the effects
	loadEffects(effects);

	// init lights
	initPointLights(points);
	initSpotLights(spots);
	light.set_ambient_intensity(vec4(0, 0, 0, 0));
	light.set_direction(normalize(meshes["boat"].get_transform().position));
	light.set_light_colour(vec4(0, 0, 0, 0));

	// Set camera properties
	setupFreeCam(freeCam);
	setupTargetCameras(targetCameras, meshes);
	setupChaseCamera(chaseCamera, meshes["amillary"]);
	activeCam = &freeCam;

	// initis motion blur required params
	// initFrames
	initScreenQuad(screen_quad);
	// Create 2 frame buffers - use screen width and height
	frames[0] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	frames[1] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create a temp framebuffer
	temp_frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());

	initRainPositions(MAX_PARTICLES, positions, velocitys);
	initRainBuffers(vao, G_Position_buffer, G_Velocity_buffer, MAX_PARTICLES, positions, velocitys);
	compute_eff = createRainComputeShader();
	rainEffect = createBasicRainEffect();

	// amillary ring
	specialRenderMeshes["amillaryRing"].get_material().set_shininess(25.0f);

	// tree positions
	generateTreesTransforms(treeTransforms);

	// Create shadow map- use screen size
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
	shadowEff = createMultiLightEffect();
	shouldRenderShadows = false;

	// Generates the terrain
	auto width = 30;
	auto height = 30;
	auto heightScale = 3.0f;
	const texture height_map("textures/islandHMap.jpg");
	meshes["terrain"] = createTerrainMesh(height_map, width, height, heightScale, &grassGeom);
	meshes["terrain"].get_material().set_specular(vec4(0, 0, 0, 0));
	meshes["terrain"].get_material().set_shininess(25.0f);
	meshes["terrain"].get_transform().scale = vec3(50, 50, 50);

	//setup grass geometry
	grassGeom.set_type(GL_POINTS);
	textures["grass"] = texture("textures/grassPack.dds", false, true);


	// Generates the night skyboxss
	array<string, 6> filenames = { "textures/skybox/starfield_ft.tga", "textures/skybox/starfield_bk.tga", "textures/skybox/starfield_up.tga",
		"textures/skybox/starfield_dn.tga", "textures/skybox/starfield_rt.tga", "textures/skybox/starfield_lf.tga" };
	cube_map = cubemap(filenames);
	specialRenderMeshes["skybox"].get_transform().scale = vec3(1000, 1000, 1000);

	// Setup of the floating spheres dataon = 0.0f;
	gemPosition = meshes["guardian"].get_transform().position + vec3(0, 35, 0);
	createFloatersTransforms(generalDebrisOriginalTransforms, rotatingFloaterNumDebris, gemPosition, 0.15f);
	for (auto i = 0; i < rotatingFloaterNumDebris; i++)
	{
		generalDebrisRotatingDebris[i] = generalDebrisOriginalTransforms[i];
	}

	// Setup cursor
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

void handleUserInput(float delta_time)
{
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

	// activates free cam
	if (glfwGetKey(renderer::get_window(), 'F')) {
		activeCam = &freeCam;
		std::cout << freeCam.get_position().x << endl;
		std::cout << freeCam.get_position().y << endl;
		std::cout << freeCam.get_position().z << endl;
	}
	// activates chase cam
	if (glfwGetKey(renderer::get_window(), 'C')) {
		activeCam = &chaseCamera;
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_O)) {
		shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
		shouldRenderShadows = false;
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_P)) {
		shouldRenderShadows = true;
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

	// handles motion blurStatus
	// none
	if (glfwGetKey(renderer::get_window(), 'B')) {
		motionBlurCoeff = 0;
	}
	// medium
	if (glfwGetKey(renderer::get_window(), 'N')) {
		motionBlurCoeff = 0.65;
	}
	// on drugs
	if (glfwGetKey(renderer::get_window(), 'M')) {
		motionBlurCoeff = 0.9;
	}

}

bool update(float delta_time) {

	// update rain data
	renderer::bind(compute_eff);
	glUniform1f(compute_eff.get_uniform_location("delta_time"), delta_time);
	glUniform3fv(compute_eff.get_uniform_location("max_dims"), 1, value_ptr(vec3(1600.0f, 400.0f, 1600.0f)));

	// userBindings
	handleUserInput(delta_time);

	// Flip frame
	current_frame = (current_frame + 1) % 2;

	// updates the water delta vector to mimick the movement
	waterDelta += vec2(delta_time * 0.05, delta_time * 0.05);

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
	for (auto i = 0; i < rotatingFloaterNumDebris; i++)
	{
		float diff = -1 * ((i % 2) + 1);
		generalDebrisRotatingDebris[i] = translate(generalDebrisOriginalTransforms[i], rotationPosition * diff);
	}
	rotationAngle -= delta_time;

	// Setup and rotation for amillary transforms
	generateAmillaryRings(amillaryTransforms, meshes["amillary"].get_transform().get_transform_matrix(), rotationAngle * 0.5);

	// updates shadows status, temporarily behind a switch
	if (shouldRenderShadows)
	{
		spots[3].set_range(500);
		// updates the projection of the light
		shadow.light_position = spots[3].get_position();
		shadow.light_dir = spots[3].get_direction();
	}
	else
	{
		spots[3].set_range(0);
	}


	return true;
}

void renderSkybox()
{
	glDisable(GL_DEPTH_TEST);

	effect skyboxEffect = effects["skybox"];

	// Bind skybox effect
	renderer::bind(skyboxEffect);

	// Calculate MVP for the skybox

	auto M = specialRenderMeshes["skybox"].get_transform().get_transform_matrix();
	auto V = activeCam->get_view();
	auto P = activeCam->get_projection();
	auto MVP = P * V * M;


	// Set MVP matrix uniform
	glUniformMatrix4fv(skyboxEffect.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

	// Set cubemap uniform
	renderer::bind(cube_map, 0);

	// Render skybox
	renderer::render(specialRenderMeshes["skybox"]);

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

	setupGeneralBindings(model, name, eff);
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

void renderParticleRain()
{
	// Bind Compute Shader
	renderer::bind(compute_eff);
	// Bind data as SSBO
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, G_Position_buffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, G_Velocity_buffer);
	// Dispatch
	glDispatchCompute(MAX_PARTICLES / 128, 1, 1);
	// Sync, wait for completion
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// Bind render effect
	renderer::bind(rainEffect);
	// Create MVP matrix
	mat4 M(1.0f);
	auto V = activeCam->get_view();
	auto P = activeCam->get_projection();
	auto MVP = P * V * M;

	// Set the colour uniform
	renderer::bind(textures["clearWater"], 0);
	glUniform1i(effects["basicTexturing"].get_uniform_location("tex"), 0);
	glUniform3fv(rainEffect.get_uniform_location("cameraPosition"), 1, value_ptr(activeCam->get_position()));
	// Set MVP matrix uniform
	glUniformMatrix4fv(rainEffect.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

	// Bind position buffer as GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, G_Position_buffer);
	//// Setup vertex format
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(nullptr));
	// Render
	glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
	// Tidy up
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void renderGrass(effect eff, geometry geom, texture tex)
{
	glDisable(GL_CULL_FACE);
	// Simply render the points.  All the work done in the geometry shader
	renderer::bind(eff);
	auto V = activeCam->get_view();
	auto P = activeCam->get_projection();
	auto MVP = P * V;
	glUniformMatrix4fv(eff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(eff.get_uniform_location("P"), 1, GL_FALSE, value_ptr(P));
	glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniform1f(eff.get_uniform_location("grassHeight"), 5.0f);
	glUniform1f(eff.get_uniform_location("windStrength"), 4.0f);
	glUniform3fv(eff.get_uniform_location("windDirectionIn"), 1, value_ptr(vec3(1.0f, 0.0f, 1.0f)));
	renderer::bind(tex, 0);
	glUniform1i(eff.get_uniform_location("tex"), 0);

	renderer::render(geom);
	glEnable(GL_CULL_FACE);
}

void renderSceneToTarget()
{
	mat4 lightProjectionMatrix = perspective<float>(90.f, renderer::get_screen_aspect(), 0.1f, 3000.f);
	if (shouldRenderShadows)
	{
		// Render Shadows
		renderShadows(lightProjectionMatrix);
	}
	// select render target
	renderer::set_render_target(temp_frame);
	// clear renderer
	renderer::clear();

	// Render skybox
	renderSkybox();
	renderInstanciatedMesh(
		specialRenderMeshes["crystal"],
		rotatingFloaterNumDebris,
		generalDebrisRotatingDebris,
		effects["multipleInstance"],
		"crystal",
		lightProjectionMatrix
		);
	renderInstanciatedMesh(
		specialRenderMeshes["amillaryRing"],
		amillaryTransforms.size(),
		amillaryTransforms,
		effects["multipleInstance"],
		"amillary",
		lightProjectionMatrix
		);
	renderInstanciatedMesh(
		specialRenderMeshes["trees"],
		treesAmount,
		treeTransforms,
		effects["trees"],
		"tree",
		lightProjectionMatrix
		);
	renderGrass(effects["grass"], grassGeom, textures["grass"]);

	// Render meshes
	for (auto &e : meshes) {
		auto m = e.second;
		auto meshName = e.first;
		effect eff = effects[meshName];
		renderMesh(m, meshName, eff, lightProjectionMatrix);
	}
	//Render rain
	renderParticleRain();
}

void renderScreenBuffer()
{
	effect motionBlurEffect = effects["motionBlur"];
	// Frame pass
	renderer::set_render_target(frames[current_frame]);
	// Clear frame
	renderer::clear();
	// Bind effect
	renderer::bind(motionBlurEffect);
	// MVP is now the identity matrix
	auto MVP = mat4(1.0);
	// Set MVP matrix uniform
	glUniformMatrix4fv(motionBlurEffect.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	// Bind tempframe to TU 0.
	renderer::bind(temp_frame.get_frame(), 0);
	// Bind frames[(current_frame + 1) % 2] to TU 1.
	renderer::bind(frames[(current_frame + 1) % 2].get_frame(), 1);
	// Set tex uniforms
	glUniform1i(motionBlurEffect.get_uniform_location("tex"), 0);
	glUniform1i(motionBlurEffect.get_uniform_location("previous_frame"), 1);
	// Set blend factor (0.9f)
	glUniform1f(motionBlurEffect.get_uniform_location("blend_factor"), motionBlurCoeff);
	// Render screen quad
	renderer::render(screen_quad);

	// Screen pass
	// Set render target back to the screen
	renderer::set_render_target();
	// Clear frame
	renderer::bind(effects["basicTexturing"]);
	// Set MVP matrix uniform
	glUniformMatrix4fv(effects["basicTexturing"].get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	// Bind texture from frame buffer
	renderer::bind(frames[current_frame].get_frame(), 3);
	// Set the uniform
	glUniform1i(effects["basicTexturing"].get_uniform_location("tex"), 3);
	// Render the screen quad
	renderer::render(screen_quad);
}

bool render() {

	renderSceneToTarget();
	renderScreenBuffer();
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