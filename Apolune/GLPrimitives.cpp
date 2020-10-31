#include "GLPrimitives.h"

DisplayList fullscreenQuad;

void InitGLPrimitives() {
 GLRect *screen=new GLRect( 0.0f, 0.0f, display.wf, display.hf );
 fullscreenQuad.StartRecording();
 screen->Draw();
 fullscreenQuad.StopRecording();
 delete screen;
}