#include "Slide.h"

#include "Proce55or.h"
#include "Presentation.h"

Slide::Slide( float d ) : ListItem() {
 fx.Instantiate();
 fullscreen_fx.Instantiate();
 duration=d;
}

Slide::Slide(void) : ListItem() {
 fx.Instantiate();
 fullscreen_fx.Instantiate();
}

void Slide::BetweenFrames() {
 sounds.Update(FRAMETIME);
 fx->Between();
 fullscreen_fx->Between();
}

void Slide::Reset(bool paused) {
 sounds.Reset();
 fx->Reset(paused);
 fullscreen_fx->Reset(paused);
 remaining=duration;
}

void Slide::Redraw() {
 PreProcess();
 fx->Redraw();
 fullscreen_fx->Redraw();
 PostProcess();
}

void Slide::Render() {
 PreProcess();
 fx->Render();
 fullscreen_fx->Render();
 PostProcess();
}

void Slide::Drop( string name ) {
 fx->Drop(name);
 fullscreen_fx->Drop(name);
}

void Slide::OnComplete() {}

bool Slide::OnSlideComplete( ) {
 this->OnComplete();
// if ( this->parent->current == this && !this->next ) this->parent->OnComplete();
 if ( rewind ) {
  parent->Seek(seek);
  Reset(!unpause);
 }
 return !unpause;
}
void Slide::PostProcess( ) { }
void Slide::PreProcess( ) { }
