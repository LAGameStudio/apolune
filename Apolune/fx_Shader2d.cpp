#include "fx_Shader2d.h"

#include "ColorShader.h"
#include "ThermalShader.h"
#include "Tunnel2Shader.h"
#include "SwirlShader.h"
//#include "Sphere2Shader.h"
#include "SniperLensShader.h"
#include "RippleShader.h"
#include "DreamBlurShader.h"
#include "PixelateShader.h"
#include "FrostedShader.h"
#include "Frosted2Shader.h"
#include "GammaShader.h"
#include "ShockwaveShader.h"
#include "Nebula4LightShader.h"
#include "NightvisionShader.h"
#include "CrosshatchShader.h"
#include "ConvolverShader.h"
#include "ImplodeShader.h"
#include "BloomShader.h"
#include "FBMShader.h"
#include "BendShader.h"
#include "BlendShader.h"
#include "ParallaxShader.h"
#include "ProtoShader.h"
#include "BillboardShader.h"
#include "StarNestShader.h"
#include "SpaceShaders.h"
#include "ASCIIShader.h"
#include "TexShader.h"

#include "Art2d.h"
#include "UglyGlyphs.h"

void InitShaders() {
 glReportError(glGetError());
 art.Init();
 CreateUglyFont();
 asciiShader.Init();
 alphaavgShader.Init();
 alphaavgthresholdShader.Init();
 basictextureShader.Init(); // this is a test shader
 //bendShader.Init();
 //bendRShader.Init();
 //bendGShader.Init();
 //bendBShader.Init();
 //bendRGShader.Init();
 //bendGBShader.Init();
 //bendBRShader.Init();
 //blendtransparentShader.Init();
 //blendtransparentfactorShader.Init();
 //fboblendtransparentShader.Init();
 //fboblendtransparentfactorShader.Init();
 //fboblendtransparentfactorwhiteclearShader.Init();
 //fboblendtransparentfactorblacketchShader.Init();
 //blend3addShader.Init();
 //blend3avgShader.Init();
 //blendmultiplyalphaShader.Init();
 //blendalphaShader.Init();
 //blendalphamultShader.Init();
 //blendalphainvShader.Init();
 //blendalpha0Shader.Init();
 //blend2wayShader.Init();
 //blend3wayShader.Init();
 //blend4wayShader.Init();
 //blend5wayShader.Init();
 //blend6wayShader.Init();
 //blend7wayShader.Init();
 //blend8wayShader.Init();
 //bloomShader.Init();
 //billboardShader.Init();
 //clodShader.Init();
 //color2dShader.Init();
 //coloraboveShader.Init();
 //coloraboveBShader.Init();
 //colorbelowShader.Init();
 //colorbelowBShader.Init();
 //colorbelowABShader.Init();
 //colorabovebelowShader.Init();
 //coverageShader.Init();
 //crosshatchShader.Init();
 //deformerShader.Init();
 //dreamblurShader.Init();
 //dreamblurrgbShader.Init();
 //edgedetectShader.Init();
 //embossShader.Init();
 //embossconvolveAShader.Init();
 //embosspreserveAShader.Init();
 //fbmShader.Init();
 //fbmcheapshadowShader.Init();
 //flyingShader.Init();
 //frosted2Shader.Init();
 //frostedShader.Init();
 //gaussianblurShader.Init();
 //gammaShader.Init();
 //glasscaveShader.Init();
 //hq4xShader.Init();
 //implodeShader.Init();
 //layerCombiner.Init();
// luxShader.Init();
 //monjoriShader.Init();
 //motionShader.Init();
 //nautilusShader.Init();
 //nebula4Shader.Init();
 //nightvisionShader.Init();
 //normalmapShader.Init();
 //normalmappingShader.Init();
 //normalmappingalphaShader.Init();
 //normalmapping3lightShader.Init();
 //normalmappingalpha3lightShader.Init();
 //normalmappingalpha3lightalmShader.Init();
 //parallaxpointShader.Init();
 //parallaxprotoShader.Init();
 //parallaxglossprotoShader.Init();
 phongShader.Init();
 p2xShader.Init();
 pixelateShader.Init();
 plasmaShader.Init();
 //pulseShader.Init();
 //radialShader.Init();
 //rippleShader.Init();
 //shadows2dShader.Init();
 //shadow2dShader.Init();
 //multishadow2dShader.Init();
 // light2dShader.Init();
 // lights2dShader.Init();
 // lightsnoradius2dShader.Init();
 // lightsnocolor2dShader.Init();
 // lightspulsing2dShader.Init();
 //sharpnessShader.Init();
 //shockwaveShader.Init();
 //sigmaV9gaussianShader.Init();
 //sigmaH9gaussianShader.Init();
 //sigmaV7gaussianShader.Init();
 //sigmaH7gaussianShader.Init();
 //sigmaV5gaussianShader.Init();
 //sigmaH5gaussianShader.Init();
 //sigmaVgaussianShader.Init();
 //sigmaHgaussianShader.Init();
 //simplefrostShader.Init();
 //sniperlensShader.Init();
 //spherizeShader.Init();
 //spherizeShader2.Init();
 //starNestShader.Init();
 //starNest2Shader.Init();
 //starNestSimpleShader.Init();
 //starNestSimple2Shader.Init();
 //stencilShader.Init();
 // stencil2Shader.Init();
 // stencil3Shader.Init();
 // stencil4Shader.Init();
 // stencilAddShader.Init();
 // stencilAdd2Shader.Init();
 // stencilAdd3Shader.Init();
 // stencilAdd4Shader.Init();
 // stencilAvg2Shader.Init();
 // stencilAvg3Shader.Init();
 // stencilAvg4Shader.Init();
 // stencilMult2Shader.Init();
 // stencilMult3Shader.Init();
 // stencilMult4Shader.Init();
 // stencilMult2AlphaShader.Init();
 //swirlShader.Init();
 texShader.Init();
  vcShader.Init();
  v2Shader.Init();
  v3Shader.Init(); 
  texTCShader.Init();
 //thermalShader.Init();
 //tunnel2Shader.Init();
 //tunnelShader.Init();
 tvShader.Init();
 tweenBeam.Init();
 vertexdisplaceShader.Init();
 //vortexShader.Init();
 // wireframeShader.Init();  obsolete
 // SpaceShaders 
 waterandgroundShader.Init();
 //emissioncloudsShader.Init();
 electrocloudsShader.Init();
 electroncloudsShader.Init();
 //nebulazShader.Init();
 toontroidShader.Init();
 texturedplanetShader.Init();
 earthlikeShader.Init();
 //accretionShader.Init();
 //blackholeShader.Init();
 //ringedjovianShader.Init();
 electroplanetShader.Init();
 atmosphericplanetShader.Init();
 earthlike2Shader.Init();
 ringedjovian2Shader.Init();
 lunarplanetShader.Init();
 flaringstarShader.Init();
 oldtvShader.Init();
 //classicplanetShader.Init();
}