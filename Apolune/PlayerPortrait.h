/*******************************************************************************************************************************\
| 88888888888          88                  LOST ASTRONAUT'S                 * Lost Astronaut Game Development Framework         |
| 88                   ""                                                   * Copyright (c) 2007-2017 H. Elwood Gilliland III   |
| 88aaaaa  8b,dPPYba,  88  8b,dPPYba,    ,adPPYb,d8   ,adPPYba,  ,adPPYba,  ****************************************************|
| 88UUUUU  88P'   "Y8  88  88P'   `"8a  a8"    `Y88  a8P_____88  I8[    ""  * This software is copyrighted software.            |
| 88       88          88  88       88  8b       88  8PP"""""""   `"Y8ba,   * Use of this code is given only with permission    |
| 88       88          88  88       88  "8a,   ,d88  "8b,   ,aa  aa    ]8I  * to parties who have been granted such permission  |
| 88       88          88  88       88   `"YbbdP"Y8   `"Ybbd8"'  `"YbbdP"'  * by its author, Herbert Elwood Gilliland III.      |
|    O  F      T H E     E M P I R E     aa,    ,88                         * All rights reserved.                              |
|_______________________________________  "Y8bbdP"   __________________________________________________________________________*/
#pragma once

#include "TextureLibrary.h"
#include "FastGUI.h"

extern Zint stashed_PortraitBackground;

ONE(PlayerPortrait,{
 eyewears.Add("data/images/portraits/020.png");
 eyewears.Add("data/images/portraits/022.png");
 eyewears.Add("data/images/portraits/023.png");
 eyewears.Add("data/images/portraits/025.png");
 eyewears.Add("data/images/portraits/026.png");
 eyewears.Add("data/images/elements/invis.png");
 heads.Add("data/images/portraits/111.png");   hairs.Add("data/images/portraits/089.png");  suits.Add("data/images/portraits/132.png");
 heads.Add("data/images/portraits/110.png");   hairs.Add("data/images/portraits/088.png");  suits.Add("data/images/portraits/131.png");
 heads.Add("data/images/portraits/109.png");   hairs.Add("data/images/portraits/087.png");  suits.Add("data/images/portraits/130.png");
 heads.Add("data/images/portraits/108.png");   hairs.Add("data/images/portraits/086.png");  suits.Add("data/images/portraits/129.png");
 heads.Add("data/images/portraits/107.png");   hairs.Add("data/images/portraits/085.png");  suits.Add("data/images/portraits/128.png");
 hairs.Add("data/images/portraits/106.png");   hairs.Add("data/images/portraits/084.png");  suits.Add("data/images/portraits/127.png");
 hairs.Add("data/images/portraits/105.png");   hairs.Add("data/images/portraits/083.png");  suits.Add("data/images/portraits/126.png");
 hairs.Add("data/images/portraits/104.png");   hairs.Add("data/images/portraits/082.png");  suits.Add("data/images/portraits/125.png");
 hairs.Add("data/images/portraits/103.png");   hairs.Add("data/images/portraits/081.png");  suits.Add("data/images/portraits/124.png");
 hairs.Add("data/images/portraits/102.png");   hairs.Add("data/images/portraits/080.png");  suits.Add("data/images/portraits/123.png");
 hairs.Add("data/images/portraits/101.png");   hairs.Add("data/images/portraits/079.png");  suits.Add("data/images/portraits/122.png");
 hairs.Add("data/images/portraits/100.png");   hairs.Add("data/images/portraits/078.png");  suits.Add("data/images/portraits/121.png");
 hairs.Add("data/images/portraits/099.png");   hairs.Add("data/images/portraits/077.png");  suits.Add("data/images/portraits/120.png");
 hairs.Add("data/images/portraits/098.png");   hairs.Add("data/images/portraits/076.png");  suits.Add("data/images/portraits/119.png");
 hairs.Add("data/images/portraits/097.png");   hairs.Add("data/images/portraits/075.png");  suits.Add("data/images/portraits/118.png");
 hairs.Add("data/images/portraits/096.png");   hairs.Add("data/images/portraits/074.png");  suits.Add("data/images/portraits/117.png");
 hairs.Add("data/images/portraits/095.png");   hairs.Add("data/images/portraits/073.png");  suits.Add("data/images/portraits/116.png");
 hairs.Add("data/images/portraits/094.png");   hairs.Add("data/images/portraits/072.png");  suits.Add("data/images/portraits/115.png");
 hairs.Add("data/images/portraits/093.png");   hairs.Add("data/images/portraits/071.png");  suits.Add("data/images/portraits/114.png");
 hairs.Add("data/images/portraits/092.png");   hairs.Add("data/images/portraits/070.png");  suits.Add("data/images/portraits/112.png");
 hairs.Add("data/images/portraits/091.png");   hairs.Add("data/images/portraits/069.png");
 hairs.Add("data/images/portraits/090.png");   hairs.Add("data/images/portraits/068.png");                                              
 hairs.Add("data/images/elements/invis.png");
 browses.Add("data/images/portraits/024.png");
 browses.Add("data/images/portraits/046.png");
 browses.Add("data/images/portraits/048.png");
 browses.Add("data/images/portraits/049.png");
 browses.Add("data/images/portraits/050.png");
 browses.Add("data/images/elements/invis.png");
 eyeses.Add("data/images/portraits/030.png");
 eyeses.Add("data/images/portraits/031.png");
 eyeses.Add("data/images/portraits/032.png");
 eyeses.Add("data/images/portraits/033.png");
 noses.Add("data/images/portraits/027.png");
 noses.Add("data/images/portraits/052.png");
 noses.Add("data/images/elements/invis.png");
 lipses.Add("data/images/portraits/021.png");  beards.Add("data/images/portraits/067.png");  
 lipses.Add("data/images/portraits/029.png");  beards.Add("data/images/portraits/066.png"); 
 lipses.Add("data/images/portraits/035.png");  beards.Add("data/images/portraits/065.png"); 
 lipses.Add("data/images/portraits/036.png");  beards.Add("data/images/portraits/064.png"); 
 lipses.Add("data/images/portraits/037.png");  beards.Add("data/images/portraits/063.png"); 
 lipses.Add("data/images/portraits/038.png");  beards.Add("data/images/portraits/062.png"); 
 lipses.Add("data/images/portraits/039.png");  beards.Add("data/images/portraits/061.png");
 lipses.Add("data/images/portraits/040.png");  beards.Add("data/images/portraits/060.png");
 lipses.Add("data/images/portraits/041.png");  beards.Add("data/images/portraits/059.png");
 lipses.Add("data/images/portraits/042.png");  beards.Add("data/images/portraits/058.png");
 lipses.Add("data/images/portraits/043.png");  beards.Add("data/images/portraits/057.png");
 lipses.Add("data/images/portraits/044.png");  beards.Add("data/images/elements/invis.png");
 lipses.Add("data/images/portraits/045.png");  
 lipses.Add("data/images/portraits/056.png");
 tint_eyewear.Pick(alabaster);
 tint_head.Pick(alabaster);   
 tint_hair.Pick(alabaster);   
 tint_brows.Pick(alabaster);  
 tint_eyes.Pick(alabaster);   
 tint_nose.Pick(alabaster);   
 tint_lips.Pick(alabaster);   
 tint_beard.Pick(alabaster);  
 tint_suit.Pick(alabaster);
 bubble=library.Load("data/images/portraits/bubble.png");
})
 Strings eyewears,heads,hairs,browses,eyeses,noses,lipses,beards,suits;
 Zp<GLImage> eyewear;  Zint selected_eyewear;  Crayon tint_eyewear;
 Zp<GLImage> head;     Zint selected_head;     Crayon tint_head;   
 Zp<GLImage> hair;     Zint selected_hair;     Crayon tint_hair;   
 Zp<GLImage> brows;    Zint selected_brows;    Crayon tint_brows;  
 Zp<GLImage> eyes;     Zint selected_eyes;     Crayon tint_eyes;   
 Zp<GLImage> nose;     Zint selected_nose;     Crayon tint_nose;   
 Zp<GLImage> lips;     Zint selected_lips;     Crayon tint_lips;   
 Zp<GLImage> beard;    Zint selected_beard;    Crayon tint_beard;  
 Zp<GLImage> suit;     Zint selected_suit;     Crayon tint_suit;   
 Zp<GLImage> bubble;
 void Set( PlayerPortrait *p ) {
  eyewear=p->eyewear;  selected_eyewear=p->selected_eyewear;  tint_eyewear=p->tint_eyewear;
  head   =p->head;     selected_head   =p->selected_head;     tint_head=   p->tint_head;   
  hair   =p->hair;     selected_hair   =p->selected_hair;     tint_hair=   p->tint_hair;   
  brows  =p->brows;    selected_brows  =p->selected_brows;    tint_brows=  p->tint_brows;  
  eyes   =p->eyes;     selected_eyes   =p->selected_eyes;     tint_eyes=   p->tint_eyes;   
  nose   =p->nose;     selected_nose   =p->selected_nose;     tint_nose=   p->tint_nose;   
  lips   =p->lips;     selected_lips   =p->selected_lips;     tint_lips=   p->tint_lips;   
  beard  =p->beard;    selected_beard  =p->selected_beard;    tint_beard=  p->tint_beard;  
  suit   =p->suit;     selected_suit   =p->selected_suit;     tint_suit=   p->tint_suit;   
  bubble =p->bubble;
 }
 KEYWORDS({
  IMGWORD("eyewear",eyewear)  NUMWORD("s_eyewear",selected_eyewear)  TINTWORD("teyewear",tint_eyewear) 
  IMGWORD("head",head)        NUMWORD("s_head",selected_head)        TINTWORD("thead",tint_head)       
  IMGWORD("hair",hair)        NUMWORD("s_hair",selected_hair)        TINTWORD("thair",tint_hair)       
  IMGWORD("brows",brows)      NUMWORD("s_brows",selected_brows)      TINTWORD("tbrows",tint_brows)     
  IMGWORD("eyes",eyes)        NUMWORD("s_eyes",selected_eyes)        TINTWORD("teyes",tint_eyes)       
  IMGWORD("nose",nose)        NUMWORD("s_nose",selected_nose)        TINTWORD("tnose",tint_nose)       
  IMGWORD("lips",lips)        NUMWORD("s_lips",selected_lips)        TINTWORD("tlips",tint_lips)       
  IMGWORD("beard",beard)      NUMWORD("s_beard",selected_beard)      TINTWORD("tbeard",tint_beard)     
  IMGWORD("suit",suit)        NUMWORD("s_suit",selected_suit)        TINTWORD("tsuit",tint_suit)       
 })
 WORDKEYS({
  IMGKEY("eyewear",eyewear)  NUMBKEY("s_eyewear",selected_eyewear)   TINTKEY("teyewear",tint_eyewear) 
  IMGKEY("head",head)        NUMBKEY("s_head",selected_head)         TINTKEY("thead",tint_head)       
  IMGKEY("hair",hair)        NUMBKEY("s_hair",selected_hair)         TINTKEY("thair",tint_hair)       
  IMGKEY("brows",brows)      NUMBKEY("s_brows",selected_brows)       TINTKEY("tbrows",tint_brows)     
  IMGKEY("eyes",eyes)        NUMBKEY("s_eyes",selected_eyes)         TINTKEY("teyes",tint_eyes)       
  IMGKEY("nose",nose)        NUMBKEY("s_nose",selected_nose)         TINTKEY("tnose",tint_nose)       
  IMGKEY("lips",lips)        NUMBKEY("s_lips",selected_lips)         TINTKEY("tlips",tint_lips)       
  IMGKEY("beard",beard)      NUMBKEY("s_beard",selected_beard)       TINTKEY("tbeard",tint_beard)     
  IMGKEY("suit",suit)        NUMBKEY("s_suit",selected_suit)         TINTKEY("tsuit",tint_suit)       
 })
 void Eyewear(int i) { selected_eyewear=i; eyewear=library.Load(eyewears.AnyChar(i)); }
 void Head(int i) { selected_head=i; head=library.Load(heads.AnyChar(i)); }
 void Hair(int i) { selected_hair=i; hair=library.Load(hairs.AnyChar(i)); }
 void Brows(int i) { selected_brows=i; brows=library.Load(browses.AnyChar(i)); }
 void Eyes(int i) { selected_eyes=i; eyes=library.Load(eyeses.AnyChar(i)); }
 void Nose(int i) { selected_nose=i; nose=library.Load(noses.AnyChar(i)); }
 void Lips(int i) { selected_lips=i; lips=library.Load(lipses.AnyChar(i)); }
 void Beard(int i) { selected_beard=i; beard=library.Load(beards.AnyChar(i)); }
 void Suit(int i) { selected_suit=i; suit=library.Load(suits.AnyChar(i)); }
 void NoBeard() { selected_beard=-1; beard=null; }
 void Randomize( bool male ) {
  const char *path="data/images/portraits/";
  Eyewear(number_range(0,1000));
  Head(number_range(0,1000));
  Hair(number_range(0,1000));
  Brows(number_range(0,1000));
  Eyes(number_range(0,1000));
  Nose(number_range(0,1000));
  Lips(number_range(0,1000));
  if ( male ) Beard(number_range(0,1000));
  else NoBeard();
  Suit(number_range(0,1000));
 }
 Zint seed,suitseed;
 void Seed( int s, int t, bool male ) {
  seed=s;
  suitseed=t;
  const char *path="data/images/portraits/";
  Eyewear(s+s+s); 
  Head(s+s);
  Hair(s+11); tint_hair.Vivids(s);
  Brows(s+12);
  Eyes(s+123);
  Nose(s+1234);
  Lips(s*s+s);
  if ( male ) Beard(s);
  else NoBeard();
  Suit(t);
 }
 void Render( int x, int y, int w=384, int h=384, bool spacesuit=false, bool drawMouth=false ) {
  if ( suit    ) StretchiNoAlpha(suit,    tint_suit   ,transparency,x,y,w,h);
  if ( head    ) StretchiNoAlpha(head,    tint_head   ,transparency,x,y,w,h);
  if ( hair    ) StretchiNoAlpha(hair,    tint_hair   ,transparency,x,y,w,h);
  if ( eyes    ) StretchiNoAlpha(eyes,    tint_eyes   ,transparency,x,y,w,h);
  if ( brows   ) StretchiNoAlpha(brows,   tint_brows  ,transparency,x,y,w,h);
  if ( nose    ) StretchiNoAlpha(nose,    tint_nose   ,transparency,x,y,w,h);
  if ( beard   ) StretchiNoAlpha(beard,   tint_beard  ,transparency,x,y,w,h);
  if ( lips    && drawMouth ) StretchiNoAlpha(lips,    tint_lips   ,transparency,x,y,w,h);
  if ( eyewear ) StretchiNoAlpha(eyewear, tint_eyewear,transparency,x,y,w,h);
  if ( spacesuit ) StretchiNoAlpha(bubble, crayons.Pick(alabaster), transparency,x,y,w,h);
 }
 void RenderHeadOnly( int x, int y, int w=384, int h=384, bool spacesuit=false, bool drawMouth=false ) {
  if ( head    ) StretchiNoAlpha(head,    tint_head   ,transparency,x,y,w,h);
  if ( hair    ) StretchiNoAlpha(hair,    tint_hair   ,transparency,x,y,w,h);
  if ( eyes    ) StretchiNoAlpha(eyes,    tint_eyes   ,transparency,x,y,w,h);
  if ( brows   ) StretchiNoAlpha(brows,   tint_brows  ,transparency,x,y,w,h);
  if ( nose    ) StretchiNoAlpha(nose,    tint_nose   ,transparency,x,y,w,h);
  if ( beard   ) StretchiNoAlpha(beard,   tint_beard  ,transparency,x,y,w,h);
  if ( lips    && drawMouth ) StretchiNoAlpha(lips,    tint_lips   ,transparency,x,y,w,h);
  if ( eyewear ) StretchiNoAlpha(eyewear, tint_eyewear,transparency,x,y,w,h);
  if ( spacesuit ) StretchiNoAlpha(bubble, crayons.Pick(alabaster), transparency,x,y,w,h);
 }
 void RenderXReversed( int x, int y, int w=384, int h=384, bool spacesuit=false, bool drawMouth=false ) {
  if ( suit    ) StretchiNoAlphaXReversed(suit,    tint_suit   ,transparency,x,y,w,h);
  if ( head    ) StretchiNoAlphaXReversed(head,    tint_head   ,transparency,x,y,w,h);
  if ( hair    ) StretchiNoAlphaXReversed(hair,    tint_hair   ,transparency,x,y,w,h);
  if ( eyes    ) StretchiNoAlphaXReversed(eyes,    tint_eyes   ,transparency,x,y,w,h);
  if ( brows   ) StretchiNoAlphaXReversed(brows,   tint_brows  ,transparency,x,y,w,h);
  if ( nose    ) StretchiNoAlphaXReversed(nose,    tint_nose   ,transparency,x,y,w,h);
  if ( beard   ) StretchiNoAlphaXReversed(beard,   tint_beard  ,transparency,x,y,w,h);
  if ( lips    && drawMouth ) StretchiNoAlphaXReversed(lips,    tint_lips   ,transparency,x,y,w,h);
  if ( eyewear ) StretchiNoAlphaXReversed(eyewear, tint_eyewear,transparency,x,y,w,h);
  if ( spacesuit ) StretchiNoAlpha(bubble, crayons.Pick(alabaster), transparency,x,y,w,h);
 }
 void ShowHelpInfo( int sx, int sy, bool hoveringColor ) {
  fast.boxedtext(
   ( !hoveringColor ) ? 
      "Click/Hold Left = Tune +\n"
      "Click Middle = Full\n"
      "Wheel Up/Down = Fine Tune +/-\n"
      "Click/Hold Right = Tune -\n"
    : "Click Left = Randomize\n"
      "Click Middle = White\n"
      "Wheel Up/Down = Bright/Dark\n"
      "Click Right = Black",
   sx+64,sy,false,
   crayons.Pick(alabaster),crayons.Pick(gray),crayons.Pick(rgb0007),4,true
  );
 }
 void RenderUI( int sx, int sy ) {
  Zbool hoveredRGBA;
  if ( stashed_PortraitBackground == 0 ) stashed_PortraitBackground=art.stash.GenRoundRect(0.3,0.3,crayons.Pick(alabaster),false,false,32);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,art.stash.rr_Button,sx,sy,384,384+4);
  Render(sx,sy,384,384,false,true);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*0+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*0,48,48) ) Head(selected_head+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*1+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*1,48,48) ) Hair(selected_hair+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*2+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*2,48,48) ) Brows(selected_brows+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*3+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*3,48,48) ) Eyewear(selected_eyewear+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*4+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*4,48,48) ) Eyes(selected_eyes+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*5+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*5,48,48) ) Nose(selected_nose+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*6+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*6,48,48) ) Lips(selected_lips+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*7+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*7,48,48) ) Beard(selected_beard+1);
  art.Element2(crayons.Pick(indigo),crayons.Pick(blue255),none,stashed_PortraitBackground,sx+384+16+128+8+4,sy+(48+2)*8+4,40,40); if ( fast.button(fast.rotateRight,sx+384+16+128+8,sy+(48+2)*8,48,48) ) Suit(selected_suit+1);
  tint_head    = fast.colorPickerTiny(tint_head,   sx+384+16,sy+(48+2)*0,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_hair    = fast.colorPickerTiny(tint_hair,   sx+384+16,sy+(48+2)*1,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_brows   = fast.colorPickerTiny(tint_brows,  sx+384+16,sy+(48+2)*2,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_eyewear = fast.colorPickerTiny(tint_eyewear,sx+384+16,sy+(48+2)*3,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_eyes    = fast.colorPickerTiny(tint_eyes,   sx+384+16,sy+(48+2)*4,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_nose    = fast.colorPickerTiny(tint_nose,   sx+384+16,sy+(48+2)*5,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_lips    = fast.colorPickerTiny(tint_lips,   sx+384+16,sy+(48+2)*6,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_beard   = fast.colorPickerTiny(tint_beard,  sx+384+16,sy+(48+2)*7,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  tint_suit    = fast.colorPickerTiny(tint_suit,   sx+384+16,sy+(48+2)*8,128,48); hoveredRGBA=( fast.rgbPalHovering || fast.cPalHovering );
  fast.boxedtext("HEAD",         sx+384+16+128+8+48+4,sy+(48+2)*0+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("HAIR",         sx+384+16+128+8+48+4,sy+(48+2)*1+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("BROWS",        sx+384+16+128+8+48+4,sy+(48+2)*2+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("EYEWEAR",      sx+384+16+128+8+48+4,sy+(48+2)*3+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("EYES",         sx+384+16+128+8+48+4,sy+(48+2)*4+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("NOSE",         sx+384+16+128+8+48+4,sy+(48+2)*5+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("LIPS & TEETH", sx+384+16+128+8+48+4,sy+(48+2)*6+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("BEARD",        sx+384+16+128+8+48+4,sy+(48+2)*7+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  fast.boxedtext("SUIT",         sx+384+16+128+8+48+4,sy+(48+2)*8+16+8,false,crayons.Pick(brightTurquoise),crayons.Pick(alabaster),crayons.Pick(black));
  if ( hoveredRGBA ) {
   ShowHelpInfo(sx,sy+(48+2)*8+4+40,fast.cPalHovering);
  }
 }
MANY(PlayerPortrait,PlayerPortraitHandle,PlayerPortraitHandles,"PlayerPortrait",PlayerPortraits,{})
DONE(PlayerPortrait)