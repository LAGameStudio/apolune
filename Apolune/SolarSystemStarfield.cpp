#include "SolarSystemStarfield.h"

#include "Universe.h"

char *space_dust[15] = {
 "data/images/dust/dust1.png",
 "data/images/dust/dust2.png",
 "data/images/dust/dust3.png",
 "data/images/dust/dust4.png",
 "data/images/dust/dust5.png",
 "data/images/dust/dust6.png",
 "data/images/dust/dust7.png",
 "data/images/dust/dust8.png",
 "data/images/dust/dust9.png",
 "data/images/dust/dust10.png",
 "data/images/dust/dust11.png",
 "data/images/dust/dust12.png",
 "data/images/dust/dust13.png",
 "data/images/dust/dust14.png",
 "data/images/dust/dust15.png"
};

char *smoke_images[10] = {
 "data/images/flares/smoke1.png",
 "data/images/flares/smoke2.png",
 "data/images/flares/smoke3.png",
 "data/images/flares/smoke4.png",
 "data/images/flares/smoke5.png",
 "data/images/flares/smoke6.png",
 "data/images/flares/smoke7.png",
 "data/images/flares/smoke8.png",
 "data/images/flares/smoke9.png",
 "data/images/flares/smoke10.png",
};

void StarSimulation::OnLoad() {
 if ( !starting_position ) starting_position=&defaultStartingPosition;
 Anchor(0.0f,0.0f,0.0f,0.0f);
 background.Pick(clear);
 corona=photosphere=wr_corona=wr_photosphere=photosphere2=null;
 float a=(float) uniform();
 float stellarRatio=iratiof(s->star_catalog_no+s->star_catalog_no%(1+system->system_catalog_no),(1+system->system_catalog_no))*5.0f;
 if ( s->type != blackdwarf ) { stellarRatio*=2; if ( stellarRatio < 0.5f ) stellarRatio+=0.5f; } 
 bool wasPulsar=false;
 float photofactor=s->photosphere.rf+s->photosphere.gf+s->photosphere.bf;
 float separationfactor=512.0f;
       ofsx=separationfactor-s->position.x*(float) separationfactor*2.0f-(*starting_position).fx;
       ofsy=separationfactor-s->position.y*(float) separationfactor*2.0f+(*starting_position).fy;
 float radialx=-(float)w/2.0f+ofsx;
 float radialy=-(float)h/2.0f+ofsy;
 float deformx=-(float)w/2.0f+ofsx;
 float deformy=-(float)h/2.0f+ofsy;
 bool systemHasDiscs=( system->nebula==circumstellardisk
   || system->nebula==protoplanetary
   || system->nebula==hIIregion
   || system->nebula==hIregion
   || system->nebula==herbigharo
   || system->nebula==planetarynebula );
 int num_discs=0;
 bool foundThis=false;
 bool discs_before_this=false;
 EACH(system->stars.first,Star,p) if ( p->nebula==circumstellardisk
   || p->nebula==protoplanetary
   || p->nebula==hIIregion
   || p->nebula==hIregion
   || p->nebula==herbigharo
   || p->nebula==planetarynebula ) {
  num_discs++;
  if ( p==s ) foundThis=true;
  if ( !foundThis ) discs_before_this=true;
 }
 bool hasDiscs=( s->nebula==circumstellardisk
   || s->nebula==protoplanetary
   || s->nebula==hIIregion
   || s->nebula==hIregion
   || s->nebula==herbigharo
   || s->nebula==planetarynebula );
 bool isFirst=(s == system->stars.first);
 // Move a star that is too close to its neighbor. (Wasn't this fixed in Universe.h??)
 EACH(stars->first,StarSimulationHandle,p) {
  if ( fdistance(p->p->ofsx,p->p->ofsy,ofsx,ofsy) < 100.0f ) {
   ofsx-=100.0f;
   ofsy-=100.0f;
  }
 }
 EACH(stars->first,StarSimulationHandle,p) {
  if ( fdistance(p->p->ofsx,p->p->ofsy,ofsx,ofsy) < 100.0f ) {
   ofsx-=100.0f;
   ofsy-=100.0f;
  }
 }
 additional=&StarSimulation::NoAdditional;
 if ( s->type == blackdwarf ) {
  VisualAid2 *va2=new VisualAid2;
  va2->w=w;
  va2->h=h;
  va2->image=library.find("data/images/elements/white.png");
  va2->Tint(s->corona);
  va2->blend=multiply;
  va2->duration=10.0f;
  Add(va2);
 }
 /*
 if ( system->stars.count < 3 && options->o_Skyboxes == o_hi ) {
 corona=new fx_Shader2d;
 corona->w=w;
 corona->h=h;
 corona->Select(radial);
 corona->position.Set(radialx,radialy);
 corona->pTex0=library.LoadGLuint("data/images/textures/Camouflage.png",tllto_Tiling);
 corona->pTex1=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
 corona->blend=additive;
 corona->size=256.0f;
 corona->tint.fromCrayon(s->corona);
 Add(corona);
 }
 */
 StarDeformer *temp= new StarDeformer;
 temp->movement.fx=0.0f;
 temp->movement.fy=0.0f;
 photosphere=(fx_Shader2d *) temp;
 photosphere->w=w;
 photosphere->h=h;
 photosphere->Select(deformer);
 photosphere->delta=0.01f;
 if (systemHasDiscs && isFirst) {
  temp->movement.fx=0.0001f;
  temp->movement.fy=0.0001f;
  photosphere->offset.Setf(0.6f,0.5f); // values of interest: 0.5, 0.5001, 0.501, 0.55 
  photosphere->pTex0=library.LoadGLuint("data/images/textures/Rings1.png",tllto_Tiling);
  photosphere->delta/=2.0f;
  photosphere->expired=0.8f*stellarRatio;
 } else if ( s->type == variablestar
          || s->type == pulsar
          || s->type == neutronstar
          || s->type == pulsatingwhitedwarf
          || s->type == slowirregular
          || s->type == sxphoenicis // irregular time sygnatures
          || s->type == deltascuti ) {
  wasPulsar=true;
  switch ( s->star_catalog_no+system->system_catalog_no % 10 ) {
   case 0: photosphere->offset.Setf( 0.501f,  0.5f); break;
   case 1: photosphere->offset.Setf( 0.5001f, 0.5f); break;
   case 2: photosphere->offset.Setf( 0.5001f, 0.5001f); break;
   case 3: photosphere->offset.Setf( 0.5f,    0.5001f); break;
   case 4: photosphere->offset.Setf( 0.55f,   0.5f); break;
   case 5: photosphere->offset.Setf( 0.55f,   0.55f); break;
   case 6: photosphere->offset.Setf( 0.5f,    0.55f); break;
  default: photosphere->offset.Setf( 0.50001f,  0.50001f); break;
  }
  photosphere->pTex0=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
  if ( s->type == slowirregular || s->type == sxphoenicis ) {
   photosphere->delta/=2.0f;
  }
 } else { // add support for glow shader here
  photosphere->offset.Setf( 0.5f,  0.5f);
  photosphere->pTex0=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
  photosphere->delta*=0.0001f;
  photosphere->expired=0.8f*stellarRatio;
  float b=13.0f*stellarRatio;
  float c=0.35f+0.25f*stellarRatio;
  photosphere->Oscillate(b-c,b,b+c,1/(900.0f+(stellarRatio*900.0f)+(stellarRatio*3600.0f)));
 }
 if ( s->type == ydwarf ) photosphere->delta=0.0625f;
 photosphere->position.Set(deformx,deformy);
 photosphere->pTex1=library.LoadGLuint("data/images/textures/Camouflage.png",tllto_Tiling);
 photosphere->blend=additive;
 photosphere->tint.fromCrayon(s->photosphere);
 photosphere->size=256.0f;
 photosphere->time=&fx_Shader2d::March;
 photosphere->expired=0.8f*stellarRatio;
 Add(photosphere);///
 /*
 if ( systemHasDiscs && options->o_Skyboxes > o_lo ) {
  photosphere2=(fx_Shader2d *) new StarDeformer;
  photosphere2->w=w;
  photosphere2->h=h;
  photosphere2->Select(deformer);
  photosphere2->offset.Setf(
   0.5f+xyzs[ s->star_catalog_no+system->system_catalog_no % 100 ]*0.03f,
   0.5f+xyzs[100- s->star_catalog_no+system->system_catalog_no % 100 ]*0.03f);
  photosphere2->pTex0=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
  photosphere2->delta=0.1f;
  photosphere2->expired=0.8f*stellarRatio;
  photosphere2->time=&fx_Shader2d::March;
  float b=13.0f*stellarRatio;
  float c=0.35f+0.25f*stellarRatio;
  photosphere2->Oscillate(b-c,b,b+c,1/(900.0f+(stellarRatio*900.0f)+(stellarRatio*3600.0f)));
  photosphere2->position.Set(deformx,deformy);
  photosphere2->pTex1=library.LoadGLuint("data/images/textures/Camouflage.png",tllto_Tiling);
  photosphere2->blend=additive;
  photosphere2->size=256.0f;
  Add(photosphere2);
 }
 */
 if ( s->type == wolf_rayet || (systemHasDiscs && !isFirst) ) {
  /*
  if ( system->stars.count < 3 && options->o_Skyboxes == o_hi ) {
   wr_corona=new fx_Shader2d;
   wr_corona->w=w;
   wr_corona->h=h;
   wr_corona->Select(radial);
   wr_corona->position.Set(radialx,radialy);
   wr_corona->pTex0=library.LoadGLuint("data/images/textures/Camouflage.png",tllto_Tiling);
   wr_corona->pTex1=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
   wr_corona->blend=additive;
   wr_corona->size=256.0f;
   Add(wr_corona);
  }
  */
  wr_photosphere=new fx_Shader2d;
  wr_photosphere->w=w;
  wr_photosphere->h=h;
  wr_photosphere->Select(deformer);
  wr_photosphere->position.Set(deformx,deformy);
  wr_photosphere->offset.Setf( 0.5f,  0.5f);
  wr_photosphere->pTex0=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
  wr_photosphere->pTex1=library.LoadGLuint("data/images/textures/Camouflage_1.png",tllto_Tiling);
  wr_photosphere->delta=0.0f;
  wr_photosphere->expired=0.6f;
  wr_photosphere->tint.Pick(alabaster);
  Add(wr_photosphere);
  additional=&StarSimulation::MoveWolfRayet;
 }
 Init();
}

// Sets the SolarSystemStarfield "velocity" pointer to the appropriate source based on what
// vehicle the player is currently in.
void SolarSystemStarfield::VelocityPlayerVehicle() {
 /*
 camera=&players.active->profile.active->vessel->map.camera;
 switch ( players.active->profile.active->vesselType ) {
  case spacesuit: { 
                   Astronaut *astronaut=(Astronaut *) (players.active->profile.active->vessel.pointer);
                   velocity=&astronaut->map.velocity;
                  } break;
      case craft: break;
 case spacecraft: { 
                   Starship *ship=(Starship *) (players.active->profile.active->vessel.pointer);
                   velocity=&ship->map.velocity;
                  } break;
 case hovercraft: break;
   case aircraft: break;
 }*/
}

void SolarSystemStarfield::ChangeSystem( int sed ) {
  seed=sed;
  movedx=0;
  movedy=0;
  background.Pick(black);
  tint.Pick(alabaster);
  tint2.fromCrayon(tint);
  tint2.ScaleByAlphaf(0.25f);
  NebulaTypes nebula = (NebulaTypes) (sed % 14);

  if ( nebula != oort ) {
   if ( nebula == reflection || nebula == emission ) { // Reflection / Emission Nebulas
    gasAndDust = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust);
    gasAndDust->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<10; j++ ) {
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta=float_range(1.0f,3.0f) * (1.0f/((float) FPS*3.0f));
     va->delta/=30.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust->wrapLimit);
     va->image=library.find( space_dust[j%15] );
     va->Tints( crayons.Pick(black), tint );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (display->w*2) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust->Add(va);
    }
    gasAndDust->Init();
   } else if ( nebula == planetarynebula ) {
    // Planetary Nebulas?
    gasAndDust = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust);
    gasAndDust->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<20; j++ ) { //was:50
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta/=30.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust->wrapLimit);
     va->image=library.find( smoke_images[(j*3)%10] ); // Needs more variations
     va->Tints( crayons.jami(0,255,64,64,255), crayons.Pick(hotPink) );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (display->w) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust->Add(va);
    }
    gasAndDust->Init();
   } else if ( nebula == protoplanetary ) { // Protoplanetary
    gasAndDust2 = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust2);
    gasAndDust2->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust2->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<20; j++ ) { //was:50
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta=float_range(1.0f,3.0f) * (1.0f/((float) FPS*3.0f));
     va->delta/=30.0f;
     va->deltaAngle-=(float) uniform()/3.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust2->wrapLimit);
     va->image=library.find( space_dust[j%15] );
     va->Tints( crayons.Pick(black), crayons.Pick(alabaster) );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (display->w*2) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust2->Add(va);
    }
    gasAndDust2->Init();
   } else if ( nebula == supernovaremnant && sed % 3 == 1 ) {  // Space-Time Distortions
    gasAndDust2 = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust2);
    gasAndDust2->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust2->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<10; j++ ) {
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta=float_range(1.0f,3.0f) * (1.0f/((float) FPS*3.0f));
     va->delta/=30.0f;
     va->deltaAngle-=(float) uniform()/2.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust2->wrapLimit);
     va->image=library.find( space_dust[j%15] );
     va->blend=solarize;
     va->Tints( crayons.Pick(black), crayons.Pick(cyan) );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (display->w*2) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust2->Add(va);
    }
    gasAndDust2->Init();
   } else if ( nebula == supernovaremnant ) {  // Black Dwarfs (Dead, Superdense Stars)
    gasAndDust2 = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust2);
    gasAndDust2->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust2->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<10; j++ ) {
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta=float_range(1.0f,3.0f) * (1.0f/((float) FPS*3.0f));
     va->delta/=30.0f;
     va->deltaAngle-=(float) uniform()/2.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust2->wrapLimit);
     va->image=library.find( space_dust[j%15] );
     va->blend=additive;
     va->Tints( crayons.Pick(black), crayons.jami(0,0,0,96,255) );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (display->w*2) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust2->Add(va);
    }
    gasAndDust2->Init();
   } else if ( nebula == herbigharo ) { // Herbig-Haro (young stars)
    gasAndDust2 = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust2);
    gasAndDust2->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust2->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<10; j++ ) {
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta=float_range(1.0f,3.0f) * (1.0f/((float) FPS*3.0f));
     va->delta/=3.0f;
     va->deltaAngle-=(float) uniform()/2.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust2->wrapLimit);
     va->image=library.find( space_dust[j%15] );
     va->Tints( crayons.Pick(black), crayons.Pick(gray) );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (2048.0) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust2->Add(va);
    }
    gasAndDust2->Init();
   } else if ( nebula == wrcloud ) { // Wolf-Rayet Cloud
        // Reflection / Emission Nebulas?
    gasAndDust2 = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust2);
    gasAndDust2->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust2->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<30; j++ ) {
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta=float_range(1.0f,3.0f) * (1.0f/((float) FPS*3.0f));
     va->delta/=3.0f;
     va->deltaAngle-=(float) uniform()/2.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust2->wrapLimit);
     va->image=library.find( space_dust[j%15] );
     va->Tints( crayons.Pick(black), crayons.Pick(gray) );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (display->w*2) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust2->Add(va);
    }
    gasAndDust2->Init();
   } else if ( nebula == darknebula ) { // Wolf-Rayet Cloud
        // Reflection / Emission Nebulas?
    gasAndDust2 = new LimitedWrappingProce55Viewer2d;
    AddChild(gasAndDust2);
    gasAndDust2->Anchor(0.0f,0.0f,0.0f,0.0f);
    gasAndDust2->wrapLimit.SetRect(-display->w*2,-display->h*2,display->w*5,display->h*5);
    for ( int j=0; j<5; j++ ) {
     DriftingRotatingVisualAid2 *va = new DriftingRotatingVisualAid2;
     va->delta=float_range(1.0f,3.0f) * (1.0f/((float) FPS*3.0f));
     va->delta/=5.0f;
     va->deltaAngle-=(float) uniform()/2.0f;
     va->deltaAngle*=va->RandomSign();
     va->angle=360.0f -(float) (720.0*uniform());
     va->tinting=(float) uniform();
     va->wrapLimit=&(gasAndDust2->wrapLimit);
     va->image=library.find( space_dust[j%15] );
     va->Tints( crayons.Pick(black), crayons.Pick(gray) );
     va->sx=(int) ( uniform() * (double) ( (-display->w/2)+va->wrapLimit->w ) );
     va->sy=(int) ( uniform() * (double) ( (-display->h/2)+va->wrapLimit->h ) );
     va->height=va->width=display->w+(int) ( uniform() * (display->w*2) );
    // 64+(int) ( uniform() * (1024.0-64.0) );
     va->wanderSpeed=float_range(1.0f,4.0f);
     gasAndDust2->Add(va);
    }
    gasAndDust2->Init();
   }
  }

  attenuate=null;
  switch ( nebula ) {
   case darknebula: /*// Milk of magnesia
     attenBlend=solarize;
     switch ( s->system_catalog_no % 33 % 4 ) {
      case 0: tint2.Pick(alabaster); break;
      case 1: tint2.Pick(red255); break;
      case 2: tint2.Pick(blue); break;
      case 3: tint2.Pick(green); break;
     }
     tint2.ScaleByAlphaf(0.25f);
     attenuate=box;*/
    break;
   case protoplanetary:
   case hIIregion:
   case hIregion:
   case herbigharo:
   case planetarynebula:
     //attenBlend=additive;
     //attenuate=bgatten;
     //background.fromCrayon(tint);
     //background.ScaleByAlphaf(0.5f);
    break;
   case oort: //break;
   case pulsarwindnebula:
   case magneticField: // Blue aura
     attenBlend=additive;
     tint2.Pick(indigo);
     attenuate=bgatten;
    break;
   case reflection:
    break;
   case emission:
     background.Pick(indigo);
     attenuate=box;
     attenBlend=solarize;
    break;
   case wrcloud: 
     background.Pick(crimson);
    break;
   case supernovaremnant: break;
   case circumstellardisk:
     background.fromCrayon(tint2);
    break;
  }

  zoom=1.0f;
  zoomRate=(1.0f/FPS)*2;

  background.Pick(clear); // debug

  // Add the starfield.
 // layers.Add( new SolarSystemStarfieldLayer8 ); // mid-level sprites
  layers.Add( new SolarSystemStarfieldLayer7 );
  layers.Add( new SolarSystemStarfieldLayer6 );
  layers.Add( new SolarSystemStarfieldLayer5 );
  layers.Add( new SolarSystemStarfieldLayer4 );
  layers.Add( new SolarSystemStarfieldLayer3 );
  layers.Add( new SolarSystemStarfieldLayer2 );
  layers.Add( new SolarSystemStarfieldLayer1 );
  //layers.Add( new SolarSystemStarfieldLayer0 );
  EACH(layers.first,SolarSystemStarfieldLayerHandle,p) AddChild(p->p);
 }