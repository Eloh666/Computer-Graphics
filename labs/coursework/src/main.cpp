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
#include "rendering/debrisTransforms.h"
#include "meshes/treeMesh.h"
#include "lights/setupLights.h"
#include "cameras/setupCameras.h"
#include "meshes/amillaryMesh.h"
#include "meshes/statueMesh.h"
#include "meshes/ruinsMesh.h"
#include "effects/instanceBasedEff.h"
#include "postProcessing/postProcessing.h"
#include "particles/particles.h"

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

mesh trees;
mesh skybox;
mesh crystal;

//effects
map<string, effect> effects;

effect skyboxEffect;
effect treeEffect;
effect multiIstanceNormalEffect;
effect motionBlurEffect;
effect basicTextureEffect;
effect grassEffect;

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
mesh amillaryRing;
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

// rainData
const unsigned long MAX_PARTICLES = 2 << 13;

vec4 positions[MAX_PARTICLES];
vec4 velocitys[MAX_PARTICLES];
GLuint G_Position_buffer, G_Velocity_buffer;
effect rainEffect;
effect compute_eff;
GLuint vao;

// grassData
vector<mat4> grassTransforms(MAX_PARTICLES);

bool load_content() {
	renderer::setClearColour(0, 0, 0);
	// initis motion blur required params
	// initFrames
	initScreenQuad(screen_quad);
	// Create 2 frame buffers - use screen width and height
	frames[0] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	frames[1] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create a temp framebuffer
	temp_frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	motionBlurEffect = createMotionBlurEffect();
	basicTextureEffect = createBasicTexturingEffect();

	textures["clearWater"] = texture("textures/clearWater.jpg", false, true);


	//init rain data
	default_random_engine rand(duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
	// Initilise particles
	for (unsigned int i = 0; i < MAX_PARTICLES; ++i) {
		int randX = (rand() % 800);
		int randY = rand() % 800;
		int randZ = (rand() % 800);
		switch(i%4)
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
		velocitys[i] = vec4(0, randSpeedY, 0, 0.0f);
	}

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
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * MAX_PARTICLES, velocitys, GL_DYNAMIC_DRAW);
	// *********************************    
	//Unbind
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	compute_eff = createRainComputeShader();
	rainEffect = createBasicRainEffect();


	

	// amillary ring
	amillaryRing = mesh(geometry("models/amillaryRing.obj"));
	amillaryRing.get_material().set_shininess(25.0f);

	// tree positions
	generateTreesTransforms(treeTransforms);

	// Create shadow map- use screen size
	shadow = shadow_map(renderer::get_screen_width(), renderer::get_screen_height());
	shadowEff = createMultiLightEffect();
	shouldRenderShadows = false;

	// Generates the terrain and loads its textures
	auto width = 30;
	auto height = 30;
	auto heightScale = 3.0f;
	const texture height_map("textures/islandHMap.jpg");
	meshes["terrain"] = createTerrainMesh(height_map, width, height, heightScale);
	//meshes["terrain"].get_material().set_specular(vec4(0.57, 0.3, 0.1, 0.3));
	meshes["terrain"].get_material().set_specular(vec4(0, 0, 0, 0));
	meshes["terrain"].get_material().set_shininess(25.0f);
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
	normal_maps["violet"] = texture("textures/violetNorm.png", false, true);

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

	meshes["waterLantern"] = createLampMesh();
	textures["waterLantern"] = texture("textures/lampDiff.png", false, true);
	effects["waterLantern"] = createMultiLightEffect();

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
	glUniform1i(basicTextureEffect.get_uniform_location("tex"), 0);
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
	//Render rain
	//renderParticleRain();
}

void renderScreenBuffer()
{
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
		renderer::bind(basicTextureEffect);
		// Set MVP matrix uniform
		glUniformMatrix4fv(basicTextureEffect.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Bind texture from frame buffer
		renderer::bind(frames[current_frame].get_frame(), 3);
		// Set the uniform
		glUniform1i(basicTextureEffect.get_uniform_location("tex"), 3);
		// Render the screen quad
		renderer::render(screen_quad);	
}

void renderGrass(effect eff, geometry geom)
{
	// Simply render the points.  All the work done in the geometry shader
	renderer::bind(eff);
	auto V = activeCam->get_view();
	auto P = activeCam->get_projection();
	auto MVP = P * V;
	glUniformMatrix4fv(eff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(eff.get_uniform_location("P"), 1, GL_FALSE, value_ptr(P));
	glUniform1f(eff.get_uniform_location("point_size"), 2.0f);
	renderer::bind(textures["grass"], 0);
	glUniform1i(eff.get_uniform_location("tex"), 0);

	renderer::render(geom);
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