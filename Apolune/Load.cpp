/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#define VERSION_STRING "APOLUNE\n1.0"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // for joystick info
#include <direct.h>    // for getcwd()
#include <stdio.h>
#include <stdlib.h>    // for rand functions
#include <math.h>

#include "macros.h"

#include "CommandLine.h"
#include "Load.h"

#include "Audio.h"
#include "Display.h"
#include "Interface.h"
#include "GLWindow.h"
#include "GLImage.h"

#include "DJ.h"
#include "TextureLibrary.h"
#include "FreeType.h"
#include "TexFont.h"
#include "Atlas.h"

#include "HUD.h"

#include "Introduction.h"
#include "TitleSequence.h"
#include "TitleMenu.h"
#include "GameOptions.h"

#include "HeightMap.h"
#include "Skybox.h"

#include "Scene.h"

#include "Shader.h"
#include "Material.h"
#include "WorldShader.h"

#include "Cube.h"
#include "Sphere.h"
#include "Toroid.h"
#include "Primitives2d.h"
#include "Primitives3d.h"
#include "ModelLibrary.h"
#include "TTFLibrary.h"
#include "TTFEx.h"
#include "FastGUI.h"

#include "Polygon.h"

#include "Model.h"
#include "Camera.h"

#include "UglyFont.h"
#include "LoadingSequence.h"

#include "BitBrush.h" // InitBitBrushes();
#include "HeightBrush.h" // InitHeightBrushes();

#include "Image2Model.h" // LoadImageModelsAndPointFilms()
#include "InterpolatedPointFilm.h"

#include "BitVoxelTest.h"
#include "LithophaneTest.h"
#include "LithophaneTest2.h"
#include "PagedLithophaneTest.h"
#include "NormalTest.h"
#include "STLViewer.h"
#include "BinaryFileTest.h"

#include "Curve2dTest.h"
#include "LatheTest.h"
#include "PointInTriangleTest.h"
#include "FreetypeTest.h"
#include "TTFCoreTest.h"
#include "TTFLibraryBrowser.h"
#include "TTFExLibraryBrowser.h"

#include "STLLibraryBrowser.h"
#include "Billboard.h"

#include "FastGUITest.h"

#if defined(DEBUG)
// Tests we are taking
//#include "GravitronTest.h"
#include "Box2DTest.h"

#include "GLFAFTester.h"

#endif

#include "ScreenPosition.h"
#include "TouchTest.h"
#include "TouchGestureDetectorTest.h"

//#define NOLOAD_TEXTURES 1

extern Display display;
extern Display *displayPointer;
extern Interface input;

extern ShaderLibrary shaders;
extern Materials materials;

gCamera camera1;

ImmediateModel model;

void MainLoadingSequenceUnthreaded::Load() {
  InitBitBrushes();
  InitHeightBrushes();
  LoadFonts();
  LoadTextures();
  LoadFilms();
  InitializeGlobals();   // Init Zdisposables, SafeMediaLookups (loads some textures)
  pregenerateCubeToSphereMap();
  LoadPrimitives();
  LoadAudio();  
  pthreads.Lock();
  pthreads.Unlock();
  LoadMaterials();
//  LoadScaffolds();
  LoadScripts();
  mainmenu->stayClosed=true;
  if ( CmdLine.Option("-chute") ) {
//   LoadOrCreateTestCharacter();
   pthreads.Lock();
   mainmenu->visible=false;
   mainmenu->Repopulate();
   mainmenu->stayClosed=true;
   titles->visible=false;
   titles->Rewind(true);
   CreateHUD();
   pthreads.Unlock();
  } else {
   titles->visible=true;
  }
  mainmenu->stayClosed=false;
 }

MainSequenceLoadingThread::MainSequenceLoadingThread( MainLoadingSequence *mls )
 : LoadingSequenceThread() { sequence=mls; /*locking=true;*/ }

void MainSequenceLoadingThread::Advance() {
 pthreads.Lock();
 sequence->AdvanceCounter();
 pthreads.Unlock();
}

void MainLoadingSequence::AdvanceCounter() {
 {
  wasCounter=counter;
  counter++;
  switch ( counter.value ) {
  case 1: output+=string("Big bang..."); break;
  case 2: output+=string("complete.\nLoading geometry"); break;
  case 4: output+=string("...complete.\nLoading textures"); break;
  case 5: output+=string("...complete.\nWatching films"); break;
  case 6: output+=string("...complete.\nAssembling May 1999 compliant smilies\n"); break;
  case 7: output+=string("Initializing AI DJ\n"); break;
  case 8: output+=string("Scanning nebulae\n"); break;
  case 9: output+=string("Turning on the radio\n"); break;
  case 10: output+=string("Material sciences\n"); break;
  case 11: output+=string("Scanning exoplanets\n"); break;
  case 12: output+=string("Pre-loading in-flight movie...please wait"); break;
  case 14: output+=string("...complete.\nPlay"); break;
  default: break;
  }
 }
}

bool MainSequenceLoadingThread::Unlocked() {
  LoadFonts();
  Advance();//1
  pregenerateCubeToSphereMap();
  pthreads.Lock();
  pthreads.Unlock();
  Advance();//2
  LoadPrimitives();
  InitBitBrushes();
  InitHeightBrushes();
//  LoadScaffolds();
  Advance();//3
  LoadTextures();
  Advance();//4
  LoadFilms();
  Advance();//5
  InitializeGlobals();   // Init Zdisposables, SafeMediaLookups (loads some textures)
  Advance();//6
  LoadAudio();
  Advance();//7
  Advance();//8
//  universe->sectors.LoadNebulaImages();
  Advance();//9
//  LoadRadio();
  Advance();//10
  LoadMaterials();
  Advance();//11
  Advance();//12
//  LoadPlayerProfileData();
  Advance();//13
  LoadScripts();
  mainmenu->stayClosed=true;
  Advance();//14
  if ( CmdLine.Option("-chute") ) {
   pthreads.Lock();
   mainmenu->visible=false;
   mainmenu->Repopulate();
   mainmenu->stayClosed=true;
   titles->visible=false;
   titles->Rewind(true);
   CreateHUD();
   hud->visible=true; 
   pthreads.Unlock();
  } else {
   titles->visible=true;
  }
  pthreads.Lock();
  mainmenu->stayClosed=false;
  sequence->destroyMe=true;
  pthreads.Unlock();
  return true;
 }

void MainLoadingSequence::Between() {
}

void MainLoadingSequence::Render() {
 Text("Uplinking\nPlease Wait",display->wd/2.0,display->hd-80.0,15,15,10,true);
 Text("Uplinking\nPlease Wait",display->wd/2.0+1.0,display->hd-80.0,15,15,10,true);
 glColor3d(0.6,0.6,0.6);
 Text("G R B O S",20,20,12,14,false);
 Text("G R B O S",21,20,12,14,false);
 Text("G R B O S",22,20,12,14,false);
 Text("G R B O S",23,20,12,14,false);
 Text("G R B O S",24,20,12,14,false);
 Text("^Empire^ Version 1.0 (c) 2007-2013 H. Elwood Gilliland III",20,36,6,7,false);
 Text(output,20,display->hd/2.0,6,7,10,false);
}

void DrawLoading( GLImage *loading, HDC *game_dc, HWND *game_window, int w, int h ) {

#if defined(GL_DEBUG)
 glReportError( glGetError() );
 GL_Assert( "DrawLoading" );
#endif
 
 Blending(none);
 art.Element(loading->texture,crayons.Pick(alabaster),art.stash.filledRect,0,0,w,h);

 Crayon c; c.Int(64,128,64,255); Blending(additive); c.gl();
 Text(VERSION_STRING,crayons.Pick(green),16.0,32.0,10.0,14.0);

}

//-------------------------------- Fonts

#include "ATFont.h"
void LoadFonts() {
 
  //freetype.Load("data/freetype.txt");
  //ttf.Load("data/ttflib.txt");
  //ttfex.Binary("data/ttfex-bin.txt");
  
  LoadTexFonts();
  LoadATexFonts();
  LoadAtlasFonts();

  // We can now init FastGUI because we have presumably loaded data/ttfex/elements/roundrect.txt
  fast.Init();

}

//-------------------------------- Textures

// Films
void LoadFilms() {
 OUTPUT("Loading films.\n");
 vault.Load("data/films.txt");
}

// Spritesheets
void LoadSpriteSheets() {
}

void LoadTextures() {
#if !defined(NOLOAD_TEXTURES)
 // Load default textures;
 OUTPUT("Loading default textures.\n");
 invis=invisiblePNG=library.Load("data/images/elements/invis.png",tllto_Tiling);

 OUTPUT("Loading textures.\n");
 Strings file;
 file.Load("data/textures.txt");
 EACH(file.first,String,s) {
  string first;
  string_argument((char *)s->s.c_str(),&first);
  if ( first.length() < 1 ) continue; // ignore blank lines
  const char *p=first.c_str();
  if ( *p=='/' && *(p+1) == '/' ) continue;
  if ( *p=='#' ) continue;
  p=s->s.c_str();
  while ( *p == ' ' ) p++;
  if ( file_exists(p) ) library.Load(p);
  else OUTPUT("LoadTextures: Could not find file `%s`\n", p );
 }
// for ( int i=0; i<39; i++ ) {
//  library.Load(library.find(weapon_flares[i]),&ip_BlackBackgroundToTransparent,false);
// }
 LoadFilms();
#endif
}

//-------------------------------- Materials

void LoadMaterials() {
  // Create duplicates of the base materials.
 //substances.Add(materials.Add("Wire Frame", WireFrame));
 substances.Add(materials.Add("Solid Grey",crayons.Pick(gray)));
 substances.Add(materials.Flat("Flat Orange",crayons.Pick(orange)));
 //substances.Add(materials.Add("Wire Vertex Color", WireVertexShaded));
 //substances.Add(materials.Add("Vertex Color", VertexColor));
 substances.Add(materials.Add(Textured,"Textured", library.Load("data/images/elements/grid32.png")));
 substances.Add(materials.Add(Textured,"Textured Wrapped", library.Load("data/images/elements/grid32.png",tllto_Tiling)));
  // Create Solid, Wire (Did not work.. this feature may not work as expected?)
  // Texture-Only Materials
}

//-------------------------------- Load 3D Primitives

void LoadPrimitives() {
 OUTPUT("Loading primitives.\n");
 Strings file;
 file.Load("data/primitives.txt");
 EACH(file.first,String,s) {
  string first;
  string_argument((char *)s->s.c_str(),&first);
  if ( first.length() < 1 ) continue; // ignore blank lines
  const char *p=first.c_str();
  if ( *p=='/' && *(p+1) == '/' ) continue;
  if ( *p=='#' ) continue;
  p=s->s.c_str();
  while ( *p == ' ' ) p++;
  if ( file_exists(p) ) lowPLYprimitives.Load(p);
  else OUTPUT("LoadPrimitives: Could not find file `%s`\n", p );
 }
}

//-------------------------------- Audio Buffers

void LoadAudio() {
#if !defined(NOLOAD_SOUNDS)
 OUTPUT("Loading DJ Tracks...");
 DJTracks tracks;
 tracks.fromString(file_as_string("data/tracks.txt").c_str());
 dJ.Merge(&tracks);
 OUTPUT("Loading audio samples.\n");
 Strings file;
 file.Load("data/audio.txt");
 EACH(file.first,String,s) {
  string first;
  string_argument((char *)s->s.c_str(),&first);
  if ( first.length() < 1 ) continue; // ignore blank lines
  const char *p=first.c_str();
  if ( *p=='/' && *(p+1) == '/' ) continue;
  if ( *p=='#' ) continue;
  p=s->s.c_str();
  while ( *p == ' ' ) p++;
  if ( file_exists(p) ) audio.Load(p);
  else OUTPUT("LoadAudio: Could not find file `%s`\n", p );
 }
#endif
}

//-------------------------------- HUD

void CreateHUD() {
 /*
 if ( hud ) {
  windows.Remove(hud);
  hud.Delete();
 }
 NEW(hud,HUD); 
 hud->manager=&windows;
 windows.Add(hud);
 windows.BringToFront(hud);
 hud->visible=true;
 windows.send=hud;
 */
}

//-------------------------------- Image2Model operations and InterpolatedPointFilms

void LoadImageModelsAndPointFilms() {
}

//-------------------------------- Title Sequence

#include "GravitronTest.h"
#include "SparksTest.h"

#include "Scripting.h"
void LoadScripts() {
 consulate.Load("data/script.txt");
 consulate.ShowErrors();

 //windows.Add(new GravitronTest);
// windows.Add(windows.send=new SparksTest);
}


#include "AssetLoader.h"
#include "VirtualWindow.h"

void Load( GLWindowManager *windows, Audio *audio, Display *display, Interface *input ) {
 options.SetGlobalPointer();
 OUTPUT("Load() called.\n");
 InitializeGlobals(); 
 pregenerateCubeToSphereMap();
 //NEW(options,GameOptions); windows->Add(options); 
 LoadPrimitives();  
 LoadMaterials();
// LoadScaffolds();
 LoadFonts();
 virtualStyleDefault.Defaults();
 LoadTextures();
 LoadScripts();
 LoadAudio();
 LoadWindows();
} // end void Load(void)

#include "Player.h"
#include "GameMenu.h"

#include "Art2d.h"
#include "BestRectTest.h"
#include "AtlasfontViewer.h"

#include "SmokeTest.h"

#include "ShaderLab.h"
#include "ATFontTest.h"
#include "Box2DTest.h"

void LoadWindows() {
// windows.Add(new TwoDeeElementEditor); return;
// windows.Add(new BinaryFileTest);
// windows.Add(new BestRectTest); return;
// windows.Add(new Art2dTest);
// windows.Add(new AtlasFontViewer); return;
// windows.Add(new DeathPanel("^OYour starship was incinerated.")); return;
// windows.Add(new SmokeTest); return;
 //windows.Add(new ShaderLab);

// windows.Add(new ATFontTest);
 windows.Add(new GameMenu);
}