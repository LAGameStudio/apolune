#include "TweenBeam.h"

TweenBeam tweenBeam;

#include "TweenBeamProfile.h"

void ShadedTweenBeam( TweenBeamProfile *profile, FBOColor *out ) {
 profile->Apply();
 out->Bind();
 out->QuadYInverted(&tweenBeam);
 out->Unbind();
}