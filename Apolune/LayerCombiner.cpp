#include "LayerCombiner.h"

LayerCombiner layerCombiner;

#include "LayerCombinerProfile.h"

void ShadedLayerCombiner( LayerCombinerProfile *in, FBOColor *out ) {
 out->background.Pick(clear);
 out->Bind();
 in->Apply();
 out->QuadYInverted(&layerCombiner);
 out->Unbind();
}