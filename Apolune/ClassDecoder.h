/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "Strings.h"

// Classes
#include "GLImage.h"
#include "GLWindow.h"
#include "Presentation.h"
#include "VisualAid.h"
#include "fx_Buttons.h"
#include "fx_Glow.h"
#include "fx_IconSelect.h"
#include "fx_MixedBag.h"
#include "fx_Numbers.h"
#include "fx_RGBAPal.h"
#include "fx_Sliders.h"
#include "fx_TexFont.h"
#include "fx_Tweakables.h"
#include "fx_Tween.h"

#include "ClassDeployer.h"

class ClassDescription : public String {
public:
 Strings subclasses;  // lists valid sub-classes
 ClassDeployer *deployer;
};


class ClassDescriptions : public LinkedList {
public:
 CLEARLISTRESET(ClassDescription);
 ~ClassDescriptions() {
  Clear();
 }
};

class ClassDecoder
{
public:
 Strings basic,atoms,lists,containers,scaffolds,proce55ors,perpetuators;

 String 
  // basic forms
  *_display, *_glwindow,

  // atoms
  *_integer, *_number, *_float, *_string, *_vertex, *_matrix, *_font, *_model, *_keyvalue, *_film, *_filmstrip, *_image,

  // lists
  *_integers, *_numbers, *_floats, *_strings, *_vertices, *_matrices, *_fonts, *_models, *_keyvalues, *_films, *_filmstrips, *_images,

  // presenters
  *_presentation, *_proce55viewer2d, *_parallax2d, *_parallaxtilemap2d, *_gameengine2d,

  // scaffolds
  *_panes, *_hudoverlay,

  // fx (proce55ors)
  *_visualaid, *_visualaidadditive,
  *_fxcoolbutton, *_fxcooltoggle,
  *_fxglow, *_fximage, *_fxiconselect, *_fxintegerselect, *_fxfloatselect, *_fxfadein, *_fxfadeout,
  *_fxmovingline, *_fxbox, *_fxlightning, *_fxlightningfield, *_fxmatrixtext, *_fxwritetext,
  *_fxcoolrgbapal4f, *_fxlabeledslider, *_fxverticalslider, *_fxrangeselector, *_fxtexfont,
  *_fxtexfontbox, *_fxtexnumbertoggle, *_fxxfade, *_fxxypad, *_fxknob, *_fxtabview, *_fxtweenselect,
  
  // fx (perpetuations)
  *_fxglow2,
  *_visualaid2,
  *_fxwobblingvisualaid2;

 matrix2d<String *> classTable;
                                      
 String *FindClassNameByName( const char *s ) {
  String *result;
  result = basic.findCaseless(s);
  if ( result ) return result;
  result = atoms.findCaseless(s);
  if ( result ) return result;
  result = lists.findCaseless(s);
  if ( result ) return result;
  result = containers.findCaseless(s);
  if ( result ) return result;
  result = scaffolds.findCaseless(s);
  if ( result ) return result;
  result = proce55ors.findCaseless(s);
  if ( result ) return result;
  result = perpetuators.findCaseless(s);
  /*if ( result )*/ return result;
  /*return null;*/
 }

 String *FindClassNameByName( string s ) {
  String *result;
  result = basic.findCaseless(s);
  if ( result ) return result;
  result = atoms.findCaseless(s);
  if ( result ) return result;
  result = lists.findCaseless(s);
  if ( result ) return result;
  result = containers.findCaseless(s);
  if ( result ) return result;
  result = scaffolds.findCaseless(s);
  if ( result ) return result;
  result = proce55ors.findCaseless(s);
  if ( result ) return result;
  result = perpetuators.findCaseless(s);
  /*if ( result )*/ return result;
  /*return null;*/
 }

 String *FindClassNameByNumber( int i ) {
  int set=i/1000;
  int idx=i-(set*1000);
  return classTable(set,idx);
 }

 ClassDeployer *FindDeployer( String *className ) {
  int i=className->integer;
  int set=i/1000;
  int idx=i-(set*1000);
 }

 ClassDecoder(void) {

  classTable.resize(10,50);
  for ( int i=0; i<10; i++ ) for ( int j=0; j<10; j++ ) classTable(i,j)=null;

  // Basic Forms = 1
  classTable(1, 2)=_glwindow=basic.PushAndGet( "Window" );                              _glwindow->integer             =1002;  
                                                                                                                       
  // Atoms = 2                                                                                                         
  classTable(2, 1)=_integer=basic.PushAndGet( "Integer" );                              _integer->integer              =2001;
  classTable(2, 2)=_float=basic.PushAndGet( "Float" );                                  _float->integer                =2002;
  classTable(2, 3)=_number=basic.PushAndGet( "Number" );                                _number->integer               =2003;
  classTable(2, 4)=_string=basic.PushAndGet( "String" );                                _string->integer               =2004;
  classTable(2, 5)=_vertex=basic.PushAndGet( "Vertex" );                                _vertex->integer               =2005;
  classTable(2, 6)=_matrix=basic.PushAndGet( "Matrix" );                                _matrix->integer               =2006;
  classTable(2, 7)=_font=basic.PushAndGet( "Font" );                                    _font->integer                 =2007;
  classTable(2, 8)=_model=basic.PushAndGet( "Model" );                                  _model->integer                =2008;
  classTable(2, 9)=_keyvalue=basic.PushAndGet( "KeyValue" );                            _model->integer                =2009;
  classTable(2,10)=_film=basic.PushAndGet( "Film" );                                    _model->integer                =2010;
  classTable(1, 1)=_image=basic.PushAndGet( "Image" );                                  _image->integer                =2011;
                                                                                                                       
  // Lists = 3                                                                                                         
  classTable(3, 1)=_integers=basic.PushAndGet( "Integers" );                            _integers->integer             =3001;
  classTable(3, 2)=_floats=basic.PushAndGet( "Floats" );                                _floats->integer               =3002;
  classTable(3, 3)=_numbers=basic.PushAndGet( "Numbers" );                              _numbers->integer              =3003;
  classTable(3, 4)=_strings=basic.PushAndGet( "Strings" );                              _strings->integer              =3004;
  classTable(3, 5)=_vertices=basic.PushAndGet( "Vertices" );                            _vertices->integer             =3005;
  classTable(3, 6)=_matrices=basic.PushAndGet( "Matrices" );                            _matrices->integer             =3006;
  classTable(3, 7)=_fonts=basic.PushAndGet( "Fonts" );                                  _fonts->integer                =3007;
  classTable(3, 8)=_model=basic.PushAndGet( "Model" );                                  _models->integer               =3008;
  classTable(3, 9)=_keyvalues=basic.PushAndGet( "KeyValues" );                          _models->integer               =3009;
  classTable(3,10)=_films=basic.PushAndGet( "Films" );                                  _model->integer                =3010;
  classTable(3,11)=_images=basic.PushAndGet( "Image" );                                 _images->integer               =3011;
                                                                                                                       
  // Proce55or Containers = 4                                                                                          
  classTable(4, 1)=_presentation=containers.PushAndGet( "Presentation" );               _presentation->integer         =4001;
  classTable(4, 2)=_proce55viewer2d=containers.PushAndGet( "Viewer" );                  _proce55viewer2d->integer      =4002;
  classTable(4, 3)=_parallax2d=containers.PushAndGet( "Parallax2d" );                   _parallax2d->integer           =4003;
  classTable(4, 4)=_parallaxtilemap2d=containers.PushAndGet( "ParallaxTileMap2d" );     _parallaxtilemap2d->integer    =4004;
  classTable(4, 5)=_gameengine2d=containers.PushAndGet( "GameEngine2d" );               _gameengine2d->integer         =4005;
                                                                                                                       
  // Design Scaffolds = 5                                                                                              
  classTable(5, 1)=_panes=basic.PushAndGet( "Panes" );                                  _panes->integer                =5001;
  classTable(5, 2)=_hudoverlay=basic.PushAndGet( "HUDOverlay" );                        _hudoverlay->integer           =5002;
                                                                                                                       
  // Proce55ors = 6                                                                                                    
  classTable(6, 1)=_visualaid        =proce55ors.PushAndGet( "VisualAid" );             _visualaid->integer            =6001;
  classTable(6, 2)=_visualaidadditive=proce55ors.PushAndGet( "VisualAidAdditive" );     _visualaidadditive->integer    =6002;
  classTable(6, 3)=_fxcoolbutton     =proce55ors.PushAndGet( "CoolButton" );            _fxcoolbutton->integer         =6003;
  classTable(6, 4)=_fxcooltoggle     =proce55ors.PushAndGet( "CoolToggle" );            _fxcooltoggle->integer         =6004;
  classTable(6, 5)=_fxglow           =proce55ors.PushAndGet( "Glow" );                  _fxglow->integer               =6005;
  classTable(6, 6)=_fximage          =proce55ors.PushAndGet( "Image" );                 _fximage->integer              =6006;
  classTable(6, 7)=_fxiconselect     =proce55ors.PushAndGet( "IconSelect" );            _fxiconselect->integer         =6007;
  classTable(6, 8)=_fxintegerselect  =proce55ors.PushAndGet( "IntegerSelect" );         _fxintegerselect->integer      =6008;
  classTable(6, 9)=_fxfloatselect    =proce55ors.PushAndGet( "FloatSelect" );           _fxfloatselect->integer        =6009;
  classTable(6,10)=_fxfadein         =proce55ors.PushAndGet( "FadeIn" );                _fxfadein->integer             =6010;
  classTable(6,11)=_fxfadeout        =proce55ors.PushAndGet( "FadeOut" );               _fxfadeout->integer            =6011;
  classTable(6,12)=_fxmovingline     =proce55ors.PushAndGet( "MovingLine" );            _fxmovingline->integer         =6012;
  classTable(6,13)=_fxbox            =proce55ors.PushAndGet( "Box" );                   _fxbox->integer                =6013;
  classTable(6,14)=_fxlightning      =proce55ors.PushAndGet( "Lightning" );             _fxlightning->integer          =6014;
  classTable(6,15)=_fxlightningfield =proce55ors.PushAndGet( "LightningField" );        _fxlightningfield->integer     =6015;
  classTable(6,16)=_fxmatrixtext     =proce55ors.PushAndGet( "MatrixText" );            _fxmatrixtext->integer         =6016;
  classTable(6,17)=_fxwritetext      =proce55ors.PushAndGet( "WriteText" );             _fxwritetext->integer          =6017;
  classTable(6,18)=_fxcoolrgbapal4f  =proce55ors.PushAndGet( "CoolRGBAPal4f" );         _fxcoolrgbapal4f->integer      =6018;
  classTable(6,19)=_fxlabeledslider  =proce55ors.PushAndGet( "LabeledSlider" );         _fxlabeledslider->integer      =6019;
  classTable(6,20)=_fxverticalslider =proce55ors.PushAndGet( "VerticalSlider" );        _fxverticalslider->integer     =6020; 
  classTable(6,21)=_fxrangeselector  =proce55ors.PushAndGet( "RangeSelector" );         _fxrangeselector->integer      =6021;
  classTable(6,22)=_fxtexfont        =proce55ors.PushAndGet( "TexFont" );               _fxtexfont->integer            =6022;
  classTable(6,23)=_fxtexfontbox     =proce55ors.PushAndGet( "TexFontBox" );            _fxtexfontbox->integer         =6023;
  classTable(6,24)=_fxtexnumbertoggle=proce55ors.PushAndGet( "TexNumberToggle" );       _fxtexnumbertoggle->integer    =6024;
  classTable(6,25)=_fxxfade          =proce55ors.PushAndGet( "XFade" );                 _fxxfade->integer              =6025;
  classTable(6,26)=_fxxypad          =proce55ors.PushAndGet( "XYPad" );                 _fxxypad->integer              =6026;
  classTable(6,27)=_fxknob           =proce55ors.PushAndGet( "Knob" );                  _fxknob->integer               =6027;
  classTable(6,28)=_fxtabview        =proce55ors.PushAndGet( "TabView" );               _fxtabview->integer            =6028;
  classTable(6,29)=_fxtweenselect    =proce55ors.PushAndGet( "TweenSelect" );           _fxtweenselect->integer        =6029;
  
  // Perpetuated = 7
  classTable(7, 1)=_visualaid2            = proce55ors.PushAndGet( "VisualAid2" );      _visualaid2->integer           =7001;
  classTable(7, 2)=_fxglow2               = proce55ors.PushAndGet( "Glow2" );           _fxglow2->integer              =7002;
  classTable(7, 3)=_fxwobblingvisualaid2  = proce55ors.PushAndGet( "Wobbler" );         _fxwobblingvisualaid2->integer =7003;
  
 }

 ~ClassDecoder(void) {}
};

extern ClassDecoder classdecoder;