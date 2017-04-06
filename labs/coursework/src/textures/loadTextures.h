#pragma once
#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

inline void loadTextures(map<string, texture> &textures)
{
	textures["terrainZero"] = texture("textures/sand.jpg", false, true);
	textures["terrainSand"] = texture("textures/sgrass.jpg", false, true);
	textures["terrainGrass"] = texture("textures/turf-grass.jpg", false, true);
	textures["terrainRock"] = texture("textures/overgrownRocks.jpg", false, true);
	textures["lantern"] = texture("textures/lampDiff.png", false, true);
	textures["moon"] = texture("textures/moonTex.jpg", false, true);
	textures["buddha"] = texture("textures/buddha.jpg", false, true);
	textures["boat"] = texture("textures/boatTex.png", false, true);
	textures["violet"] = texture("textures/violet.png", false, true);
	textures["tree"] = texture("textures/treeDiff.tga", false, true);
	textures["katana"] = texture("textures/katDiffuse.tga", false, true);
	textures["grave"] = texture("textures/grave.png", false, true);
	textures["mossyRock"] = texture("textures/mossyRock.jpg", false, true);
	textures["guardian"] = texture("textures/guardian.psd", false, true);
	textures["crystal"] = texture("textures/crystalDiffuse.jpg", false, true);
	textures["ruins"] = texture("textures/grayStoneWall.jpg", false, true);
	textures["deadTree"] = texture("textures/deadTree.jpg", false, true);
	textures["stoneSword"] = texture("textures/statueStone.jpg", false, true);
	textures["waterLantern"] = texture("textures/lampDiff.png", false, true);
	textures["amillary"] = texture("textures/metal.jpg", false, true);
	textures["clearWater"] = texture("textures/clearWater.jpg", false, true);
	textures["waterBase"] = texture("textures/water2k.jpg", false, true);
	textures["grass"] = texture("textures/grassPack.dds", false, true);
	

}

inline void loadNormalMaps(map<string, texture> &normal_maps)
{
	normal_maps["terrainZero"] = texture("textures/terrainSand0.jpg", false, true);
	normal_maps["terrainSand"] = texture("textures/terrainSandOne.jpg", false, true);
	normal_maps["terrainGrass"] = texture("textures/terrainGrassNorm.png", false, true);
	normal_maps["terrainRock"] = texture("textures/terrainRockNorm.jpg", false, true);
	normal_maps["violet"] = texture("textures/violetNorm.png", false, true);
	normal_maps["buddha"] = texture("textures/buddhaN.jpg", false, true);
	normal_maps["boat"] = texture("textures/woodNorm.jpg", false, true);
	normal_maps["tree"] = texture("textures/treeNorm.tga", false, true);
	normal_maps["mossyRock"] = texture("textures/mossyRockNorm.jpg", false, true);
	normal_maps["guardian"] = texture("textures/guardianNorm.tga", false, true);
	normal_maps["crystal"] = texture("textures/meteorNorm.tga", false, true);
	normal_maps["deadTree"] = texture("textures/deadTreeNorm.jpg", false, true);
	normal_maps["stoneSword"] = texture("textures/bladeNorms.tga", false, true);
	normal_maps["amillary"] = texture("textures/metalNorm.jpg", false, true);
	normal_maps["waterBase"] = texture("textures/waterNormal.jpg", false, true);
}

inline void loadAlphaMaps(map<string, texture> &alpha_maps)
{
	alpha_maps["violet"] = texture("textures/violet_a.jpg", false, true);
	alpha_maps["tree"] = texture("textures/treeAlpha.tga", false, true);
}