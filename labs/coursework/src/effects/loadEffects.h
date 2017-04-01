#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "multiLightEff.h"
#include "normalMapEff.h"
#include "instanceBasedEff.h"
#include "skyboxEff.h"
#include "movingWaterEff.h"
#include "simpleTexEff.h"
#include "../meshes/grassMesh.h"
#include "../postProcessing/postProcessing.h"


using namespace std;
using namespace graphics_framework;
using namespace glm;



inline void loadEffects(map<string, effect> &effects)
{
	effects["motionBlur"] = createMotionBlurEffect();
	effects["basicTexturing"] = createBasicTexturingEffect();
	effects["terrain"] = createTerrainEffect();
	effects["waterBase"] = createMovingWaterEffect();
	effects["skybox"] = createSkyboxEffect();
	effects["lantern"] = createMultiLightEffect();
	effects["moon"] = createMultiLightEffect();
	effects["buddha"] = createMultiLightEffect();
	effects["boat"] = createNormalMapEffect();
	effects["grave"] = createMultiLightEffect();
	effects["violet"] = createMultiLightRemoveAlphaEffect();
	effects["trees"] = createMultiInstanceRemoveAlphaEffect();
	effects["katana"] = createMultiLightEffect();
	effects["amillary"] = createNormalMapEffect();
	effects["mossyRock"] = createNormalMapEffect();
	effects["guardian"] = createNormalMapEffect();
	effects["multipleInstance"] = createMultiInstanceEffect();
	effects["ruins"] = createMultiLightEffect();
	effects["stoneSword"] = createNormalMapEffect();
	effects["deadTree"] = createNormalMapEffect();
	effects["waterLantern"] = createMultiLightEffect();
	effects["grass"] = createGrassEffect();
}