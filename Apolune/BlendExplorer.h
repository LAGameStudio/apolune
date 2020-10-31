#pragma once
#include "GLWindow.h"
#include "FastGUI.h"

class BlendExplorer : public GLWindow {
public:
 Zp<FBOColorDepth> fbo1,fbo2;
 Crayon background,fboBG1,fboTint1,fboBG2,fboTint2,t1Tint,t2Tint;
 Strings textures;
 Strings blendOps;
 Strings equationOps;
 Zp<String> source,sourceA,destination,destinationA,equation,equation2,t1,t2;
 Zp<GLImage> texture1,texture2,bg;
 Zbool seperateEquations,separateBlends;
 Zbool drawOverlay;
 void SetDefaultTextures() {
  textures.Add("data/images/elements/85black.png");
  textures.Add("data/images/elements/95black.png");
  textures.Add("data/images/elements/arrowhead.png");
  textures.Add("data/images/elements/atten2D.png");
  textures.Add("data/images/elements/attenOverlay.png");
  textures.Add("data/images/elements/back.png");
  textures.Add("data/images/elements/black.png");
  textures.Add("data/images/elements/buttonglow_1024_0.png");
  textures.Add("data/images/elements/buttonglow_1024_1.png");
  textures.Add("data/images/elements/buttonglow_1024_2.png");
  textures.Add("data/images/elements/buttonglow_1024_3.png");
  textures.Add("data/images/elements/buttonglow_1024_4.png");
  textures.Add("data/images/elements/buttonglow_1024_4b.png");
  textures.Add("data/images/elements/button_1024.png");
  textures.Add("data/images/elements/button_256.png");
  textures.Add("data/images/elements/button_512.png");
  textures.Add("data/images/elements/circle-white.png");
  textures.Add("data/images/elements/colony_overlay.png");
  textures.Add("data/images/elements/crosshair1.png");
  textures.Add("data/images/elements/cube.png");
  t1=textures.Add("data/images/elements/down.png");
  textures.Add("data/images/elements/downarrow.png");
  textures.Add("data/images/elements/fatx.png");
  textures.Add("data/images/elements/filledcircle.png");
  textures.Add("data/images/elements/filledellipse.png");
  textures.Add("data/images/elements/grid32.png");
  textures.Add("data/images/elements/hi.png");
  textures.Add("data/images/elements/horizRamp.png");
  textures.Add("data/images/elements/horizslider.png");
  textures.Add("data/images/elements/invis.png");
  textures.Add("data/images/elements/left.png");
  textures.Add("data/images/elements/leftarrow.png");
  textures.Add("data/images/elements/lo.png");
  textures.Add("data/images/elements/locked.png");
  textures.Add("data/images/elements/longbar.png");
  textures.Add("data/images/elements/med.png");
  textures.Add("data/images/elements/next.png");
  textures.Add("data/images/elements/off.png");
  textures.Add("data/images/elements/orbitpath.png");
  textures.Add("data/images/elements/PM-299_1024.png");
  textures.Add("data/images/elements/PM-Sunglass_1024.png");
  textures.Add("data/images/elements/quitandsave.png");
  textures.Add("data/images/elements/right.png");
  textures.Add("data/images/elements/rightarrow.png");
  textures.Add("data/images/elements/rounded.png");
  textures.Add("data/images/elements/RoundedCorner_64.png");
  textures.Add("data/images/elements/rounded_1024.png");
  textures.Add("data/images/elements/rounded_512.png");
  textures.Add("data/images/elements/rounded_less_1024.png");
  textures.Add("data/images/elements/rounded_less_256.png");
  textures.Add("data/images/elements/rounded_less_512.png");
  textures.Add("data/images/elements/rounded_side.png");
  textures.Add("data/images/elements/rounded_tall.png");
  textures.Add("data/images/elements/rounded_tall_hires.png");
  textures.Add("data/images/elements/rounded_tall_widehires.png");
  textures.Add("data/images/elements/rounded_trans.png");
  textures.Add("data/images/elements/rounder0.png");
  textures.Add("data/images/elements/rounder1.png");
  textures.Add("data/images/elements/rounder3.png");
  textures.Add("data/images/elements/rounder4.png");
  textures.Add("data/images/elements/rounder_box.png");
  textures.Add("data/images/elements/rounder_box256.png");
  t2=textures.Add("data/images/elements/rounder_minigrid.png");
  textures.Add("data/images/elements/Round_Subtractor_1024.png");
  textures.Add("data/images/elements/slider.png");
  textures.Add("data/images/elements/smallcube.png");
  textures.Add("data/images/elements/swab.png");
  textures.Add("data/images/elements/swab2.png");
  textures.Add("data/images/elements/tab.png");
  textures.Add("data/images/elements/transrounded1024black.png");
  textures.Add("data/images/elements/transrounded1024white.png");
  textures.Add("data/images/elements/transrounded256.png");
  textures.Add("data/images/elements/transrounded512.png");
  textures.Add("data/images/elements/transrounded512borderless.png");
  textures.Add("data/images/elements/transroundedouter512.png");
  textures.Add("data/images/elements/unlocked.png");
  textures.Add("data/images/elements/up.png");
  textures.Add("data/images/elements/uparrow.png");
  textures.Add("data/images/elements/vertRamp.png");
  textures.Add("data/images/elements/vertRamp2.png");
  textures.Add("data/images/elements/vertslider.png");
  textures.Add("data/images/elements/white.png");
  textures.Add("data/images/elements/wide.png");
  textures.Add("data/images/elements/xfaded.png");
  textures.Add("data/images/icons/adriano_Hard_Hat.png");
  textures.Add("data/images/icons/alien.png");
  textures.Add("data/images/icons/Andy_Tools_Hammer_Spanner.png");
  textures.Add("data/images/icons/Andy_Trash_Can.png");
  textures.Add("data/images/icons/backpack.png");
  textures.Add("data/images/icons/barretr_Key.png");
  textures.Add("data/images/icons/building.png");
  textures.Add("data/images/icons/commodity.png");
  textures.Add("data/images/icons/computer.png");
  textures.Add("data/images/icons/dagobert83_apply.png");
  textures.Add("data/images/icons/dagobert83_cancel.png");
  textures.Add("data/images/icons/dispenser.png");
  textures.Add("data/images/icons/DONE.png");
  textures.Add("data/images/icons/dtjohnnymonkey_Stopwatch.png");
  textures.Add("data/images/icons/earth.png");
  textures.Add("data/images/icons/eastshores_Warning_Notification.png");
  textures.Add("data/images/icons/EDIT.png");
  textures.Add("data/images/icons/emyller_alert.png");
  textures.Add("data/images/icons/fabricator.png");
  textures.Add("data/images/icons/felipecaparelli_Gears_1.png");
  textures.Add("data/images/icons/female.png");
  textures.Add("data/images/icons/follow.png");
  textures.Add("data/images/icons/foundation.png");
  textures.Add("data/images/icons/fzap_clock_sportstudio_design.png");
  textures.Add("data/images/icons/f_command.png");
  textures.Add("data/images/icons/f_commerce.png");
  textures.Add("data/images/icons/f_culture.png");
  textures.Add("data/images/icons/f_daycare.png");
  textures.Add("data/images/icons/f_education.png");
  textures.Add("data/images/icons/f_factory.png");
  textures.Add("data/images/icons/f_garden.png");
  textures.Add("data/images/icons/f_medical.png");
  textures.Add("data/images/icons/f_military.png");
  textures.Add("data/images/icons/f_office.png");
  textures.Add("data/images/icons/f_reactor.png");
  textures.Add("data/images/icons/f_refinery.png");
  textures.Add("data/images/icons/f_science.png");
  textures.Add("data/images/icons/f_security.png");
  textures.Add("data/images/icons/gatherer.png");
  textures.Add("data/images/icons/gringer_Piggybank-pink.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_10.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_2.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_29.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_3.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_32.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_34.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_4.png");
  textures.Add("data/images/icons/h0us3s_Signs_Hazard_Warning_8.png");
  textures.Add("data/images/icons/hold.png");
  textures.Add("data/images/icons/hull.png");
  textures.Add("data/images/icons/hyoga_Tux_G2.png");
  textures.Add("data/images/icons/hyperdrive.png");
  textures.Add("data/images/icons/jcartier_building.png");
  textures.Add("data/images/icons/jcartier_building_1.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_console.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_cubes.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_cube_green.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_cube_orange.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_cube_red.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_cube_white.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_graphics.png");
  textures.Add("data/images/icons/jean_victor_balin_icon_monitoring.png");
  textures.Add("data/images/icons/johnny_automatic_pliers.png");
  textures.Add("data/images/icons/johnny_automatic_Services_10.png");
  textures.Add("data/images/icons/johnny_automatic_Services_12.png");
  textures.Add("data/images/icons/launcher.png");
  textures.Add("data/images/icons/lemmling_Blank_sticky_note.png");
  textures.Add("data/images/icons/lemmling_Blank_sticky_note_1.png");
  textures.Add("data/images/icons/liftarn_A_pair_of_scissors.png");
  textures.Add("data/images/icons/locomotion.png");
  textures.Add("data/images/icons/Machovka_Hammer_4.png");
  textures.Add("data/images/icons/Machovka_Pipe_wrench.png");
  textures.Add("data/images/icons/Machovka_Screwdriver_5.png");
  textures.Add("data/images/icons/Machovka_Shovel.png");
  textures.Add("data/images/icons/magnifier.png");
  textures.Add("data/images/icons/male.png");
  textures.Add("data/images/icons/martinix_biohazard.png");
  textures.Add("data/images/icons/mcol_open_box.png");
  textures.Add("data/images/icons/molumen_Exclamation_icons_1.png");
  textures.Add("data/images/icons/molumen_Exclamation_icons_2.png");
  textures.Add("data/images/icons/molumen_Exclamation_icons_3.png");
  textures.Add("data/images/icons/molumen_Exclamation_icons_4.png");
  textures.Add("data/images/icons/moveto.png");
  textures.Add("data/images/icons/NEW.png");
  textures.Add("data/images/icons/papapishu_Lab_icon_1.png");
  textures.Add("data/images/icons/papapishu_Lab_icon_3.png");
  textures.Add("data/images/icons/patrol.png");
  textures.Add("data/images/icons/pickup.png");
  textures.Add("data/images/icons/pitr_First_aid_icon.png");
  textures.Add("data/images/icons/planet.png");
  textures.Add("data/images/icons/plussign.png");
  textures.Add("data/images/icons/powersource.png");
  textures.Add("data/images/icons/radacina_cursor_hand.png");
  textures.Add("data/images/icons/random.png");
  textures.Add("data/images/icons/raygun.png");
  textures.Add("data/images/icons/rejon_Hammer.png");
  textures.Add("data/images/icons/richardtallent_Sticky_Note_Pad.png");
  textures.Add("data/images/icons/robot.png");
  textures.Add("data/images/icons/SAVE.png");
  textures.Add("data/images/icons/scanner.png");
  textures.Add("data/images/icons/sea.png");
  textures.Add("data/images/icons/searchlight.png");
  textures.Add("data/images/icons/sigalaflecha_m_sico_hopi.png");
  textures.Add("data/images/icons/Silveira_Neto_Robot_with_a_claw.png");
  textures.Add("data/images/icons/smallcube.png");
  textures.Add("data/images/icons/SnarkHunter_Hammer_and_Nails.png");
  textures.Add("data/images/icons/station.png");
  textures.Add("data/images/icons/Steren_Warning.png");
  textures.Add("data/images/icons/tractorbeam.png");
  textures.Add("data/images/icons/trans_x.png");
  textures.Add("data/images/icons/underwater.png");
  textures.Add("data/images/icons/urwald_Zielscheibe_target_aim.png");
  textures.Add("data/images/icons/vehicles.png");
  textures.Add("data/images/icons/weapon.png");
  textures.Add("data/images/icons/webmichl_light_bulb.png");
  textures.Add("data/images/icons/zeimusu_Warning_notification.png");
  textures.Add("data/images/icons/zeimusu_Warning_sign.png");
 }
 void OnLoad() {
  Fullscreen();
  background.Pick(indigo);
  SetDefaultTextures();
  sourceA=source=blendOps.Add("GL_ZERO",GL_ZERO);
  destinationA=destination=blendOps.Add("GL_ONE", GL_ONE);
  blendOps.Add("GL_SRC_COLOR", GL_SRC_COLOR);
  blendOps.Add("GL_ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR);
  blendOps.Add("GL_SRC_ALPHA", GL_SRC_ALPHA);
  blendOps.Add("GL_ONE_MINUS_SRC_ALPHA",GL_ONE_MINUS_SRC_ALPHA);
  blendOps.Add("GL_DST_ALPHA",GL_DST_ALPHA);
  blendOps.Add("GL_ONE_MINUS_DST_ALPHA",GL_ONE_MINUS_DST_ALPHA);
  blendOps.Add("GL_DST_COLOR",GL_DST_COLOR);
  blendOps.Add("GL_ONE_MINUS_DST_COLOR",GL_ONE_MINUS_DST_COLOR);
  blendOps.Add("GL_SRC_ALPHA_SATURATE",GL_SRC_ALPHA_SATURATE);
  blendOps.Add("GL_CONSTANT_COLOR", GL_CONSTANT_COLOR);
  blendOps.Add("GL_ONE_MINUS_CONSTANT_COLOR", GL_ONE_MINUS_CONSTANT_COLOR);
  blendOps.Add("GL_CONSTANT_ALPHA", GL_CONSTANT_ALPHA);
  blendOps.Add("GL_ONE_MINUS_CONSTANT_ALPHA", GL_ONE_MINUS_CONSTANT_ALPHA);
  equation=equation2=equationOps.Add("GL_FUNC_ADD",GL_FUNC_ADD);
  equationOps.Add("GL_FUNC_SUBTRACT",GL_FUNC_ADD);
  equationOps.Add("GL_FUNC_REVERSE_SUBTRACT",GL_FUNC_ADD);
  fbo1=&fboScreen1;
  fbo2=&fboScreen2;
  fboBG1.Pick(clear);
  fboTint1.Pick(alabaster);
  fboBG2.Pick(clear);
  fboTint2.Pick(alabaster);
  t1Tint.Pick(alabaster);
  t2Tint.Pick(alabaster);
  texture1=library.Load("data/images/elements/smallcube.png");
  texture2=library.Load("data/images/elements/Round_Subtractor_1024.png");
  drawOverlay=true;
 }
 void Between() {
 }
 void Render() {  
  fbo1->background=fboBG1;
  fbo1->Bind();
  glColor3d(1.0,1.0,1.0);
  Blending(transparency);
  t1Tint.gl();
  glEnable(GL_TEXTURE_2D);
  texture1->Use();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,       0);
  glTexCoord2f(1.0f, 1.0f);  glVertex2i(fbo1->w, 0);
  glTexCoord2f(1.0f, 0.0f);  glVertex2i(fbo1->w, fbo1->h);
  glTexCoord2f(0.0f, 0.0f);  glVertex2i(0,       fbo1->h);
  glEnd();
  if ( drawOverlay ) {
   if ( seperateEquations ) {
    glBlendEquationSeparate(equation->integer,equation2->integer);
   } else {
    glBlendEquation(equation->integer);
   }
   if ( separateBlends ) {
    glBlendFuncSeparate(source->integer,destination->integer,sourceA->integer,destinationA->integer);
   } else {
    glBlendFunc(source->integer,destination->integer);
   }
   t2Tint.gl();
   texture2->Use();
   glBegin(GL_QUADS);
   glTexCoord2f(0.0f, 1.0f);  glVertex2i(0,       0);
   glTexCoord2f(1.0f, 1.0f);  glVertex2i(fbo1->w, 0);
   glTexCoord2f(1.0f, 0.0f);  glVertex2i(fbo1->w, fbo1->h);
   glTexCoord2f(0.0f, 0.0f);  glVertex2i(0,       fbo1->h);
   glEnd();
  }
  fbo1->Unbind();
   
  fbo1->tint=fboTint1;
  fbo1->blend=transparency;
  fbo1->RenderUpsideDown(display->w2/2,display->h2/2,display->w2/2+display->w2,display->h2/2+display->h2);
  StretchiNoAlpha(texture1,t1Tint,transparency,
   display->w2/2,
   display->h2/2+display->h2,
   display->w2/2,
   display->h/4
  );
  StretchiNoAlpha(texture2,t2Tint,transparency,
   display->w2/2+display->w2/2,
   display->h2/2+display->h2,
   display->w2/2,
   display->h/4
  );
  int starty=8;
  int startx=8;
  EACH(textures.first,String,s) {
   if ( fast.button(library.Load(s->s.c_str()),startx,starty,32,32) ) {
    t1=s;
    texture1=library.Load(t1->s.c_str());
   }
   if ( fast.button(library.Load(s->s.c_str()),startx+256+32+4,starty,32,32) ) {
    t2=s;
    texture2=library.Load(t2->s.c_str());
   }
   starty+=32+4;
   if ( starty > display->w2-32-4 ) { starty=8; startx+=32+4; }
  }
  int delta=32+16;
  int left=display->w-256;
  int dy=16;
               background=fast.pick("Window BG",  background, left,dy);
  dy+=delta*4-32;   fboTint1=fast.pick("FBO 1 Tint", fboTint1,   left,dy);
  dy+=delta*4-32;     fboBG1=fast.pick("FBO 1 BG",   fboBG1,     left,dy);
  dy+=delta*4-32;   fboTint2=fast.pick("FBO 2 Tint", fboTint2,   left,dy);
  dy+=delta*4-32;     fboBG2=fast.pick("FBO 2 BG",   fboBG2,     left,dy);
  dy+=delta*4-32;     t1Tint=fast.pick("Tex 1 Tint", t1Tint,     left,dy);
  dy+=delta*4-32;     t2Tint=fast.pick("Tex 2 Tint", t2Tint,     left,dy);
  dy=16;
             source=fast.enumerated(&blendOps,"Source",source.pointer,display->w2,dy,64,32);
  dy+=delta; destination=fast.enumerated(&blendOps,"Destination",destination.pointer,display->w2,dy,64,32);
  dy+=delta; separateBlends=fast.toggle("1 & 2","1","Blend Function",separateBlends,display->w2,dy,32,32);
  dy+=delta+8; sourceA=fast.enumerated(&blendOps,"Source A",sourceA.pointer,display->w2,dy,64,32);
  dy+=delta; destinationA=fast.enumerated(&blendOps,"Destination A",destinationA.pointer,display->w2,dy,64,32);
  dy=8;
  dy+=delta; equation=fast.enumerated(&equationOps,"Equation",equation.pointer,display->w2-256,dy,64,32);
  dy+=delta; equation2=fast.enumerated(&equationOps,"Equation2",equation2.pointer,display->w2-256,dy,64,32);
  dy+=delta; seperateEquations=fast.toggle("1 & 2","1","Equation Function",seperateEquations,display->w2-256,dy,32,32);
  dy+=delta+8; drawOverlay=fast.toggle("on","off","Overlay",drawOverlay,display->w2-256,dy,32,32);
 }
};