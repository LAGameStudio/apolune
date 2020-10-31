
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/

#include "Crayon.h"
#include <math.h>
#include "BinaryFile.h"
#include "GLSetup.h"
#include "Art.h"

char *color_keywords[TOTAL_COLORS_AND_GLASS] = {
 
 "cyan", "chocolate", "white", "black", "clear", "red255", "green255", "blue255",
 "pink", "aliceblue", "blue", "green", "cadetblue", "orangered", "brown", "indigo",
 "thistle", "darkred", "darksalmon", "salmon", "purple", "deepskyblue", "steelblue", "slateblue", "yellow",
 "orange", "bloodred", "skyblue", "rosybrown", "violet", "darkchocolate", "crimson", "chartreuse",
 "lightskyblue", "beige", "bisque", "firebrick", "gold", "oldlace", "linen", "turquoise",
 "gray", "cornflowerblue", "charcoal",
  /* Browns and yellows */
 "cornsilk", "beiger", "cream", "lemonchiffon", "palegoldenrod", "wheat", "dandelion", "mustard", "mediumgoldenrod",
 "corn", "lemonyellow", "goldenyellow", "schoolbusyellow", "golden", "sunglow", "tangerineyellow", "saffron",
 "goldenpoppy", "amber", "selectiveyellow", "macncheese", "sandybrown", "desertsand",
 "palegold", "brass", "californiagold", "vegasgold", "oldgold", "metallicgold", "goldenrod",
 "satinsheengold", "buddhagold", "peru", "rosegold", "ochre",
 "darkgoldenrod", "sandytaupe", "goldenbrown", "rawumber",
  /* Pinks */
 "palepink", "pigpink", "pink", "cottoncandy", "babypink", "mediumpink",
 "cherryblossompink", "nadashikopink", "carnationPink", "ticklemepink",
 "raspberrypink", "darkpink", "brinkpink", "puce", "pastelpink",
 "lightsalmonpink", "salmonpink", "mediumsalmon", "salmon2",
 "darksalmon2", "coralpink", "lightcoral", "lightcarminepink",
 "carminepink", "deepcarminepink", "vermilion",
  /* Tans, Oranges and Reds */
 "lightchocolate", "terracotta", "tomato", "coralred", "carminered", "redorange2", "electricvermilion",
 "orangered3", "coquelicot", "scarlet", "red2", "pigmentred", "lust", "crimson2", "ruby", "raspberry",
 "razzmatazz", "cerise", "alizarincrimson", "electriccrimson", "americanrose", "radicalred", "amaranth",
 "richcarmine", "fireenginered", "harvardcrimson", "cardinal",
  /* Purply Reds */
 "brightmaroon", "richmaroon", "raspberryrose", "raspberryglace", "deepcarmine", "crimsonglory",
 "brighttyrianpurple", "mediumcarmine", "mahogany", "venetianred", "persianred", "brickred",
 "darkterracotta", "chestnut",
  /* Browns, Reds, Burgundies and Auburns */
 "fuzzywuzzybrown", "deepchestnut", "redwood", "mediumchocolate", "darkchestnut", "cafeaulait", "russet",
 "rust", "burntumber", "vividburgundy", "redbrown", "firebrick2", "carnelian", "upsdellred", "rufous",
 "cognac", "teared", "sangria", "vividauburn", "burgundy", "mediumtyrianpurple", "carmine", "darkred2",
 "falured", "maroon", "tuscanred", "bole", "chocolate3", "darkchocolate2", "auburn", "persianplum",
 "darkraspberry", "tyrianpurple", "caputmortuum", "coffee",
  /* Greens */
 "olivinegreen", "olivedrab", "olive3", "darkolivegreen", "lavendermist",
  /* Magentas, Purples, Lilacs */
 "lightmauve", "mauve", "thistle2", "lilac", "lightorchid", "paleplum",
 "lavendermagenta", "orchid", "heliotrope", "psychedelicpurple", "electricpurple",
 "pontiff", "x11purple", "darkorchid", "deeppurple", "internetpurple",
  /* Blues, Indigos, Violets */
 "deepindigo", "purpleheart", "royalpurple", "genericpurple", "pigmentindigo",
 "darkindigo", "vulgarpurple", "persianindigo", "ultramarine", "persianblue",
 "electricultramarine", "hanpurple", "westminster", "indigo2", "violet2", "pigmentviolet",
 "violetpurple", "lavender", "purplemountainmajesty", "purpleYam", "mediumpurple",
 "amethyst", "deeplilac", "mediumorchid", "deepfuchsia", "operamauve",
 "mauvetaupe", "mulberry", "paleredviolet", "cabaret", "royalfuchsia",
 "byzantine", "fandango", "mediumredviolet", "redviolet", "hibiscus",
 "brighttyrianpurple2", "mediumtyrianpurple2", "amaranthpurple", "nightshadz",
 "royalheath", "jazzberryjam", "amaranthdeeppurple", "redvioleteggplant", "disco",
 "plum", "violeteggplant", "purple4", "byzantium", "palatinatepurple", "tokyopurple",
 "halayaube", "pansypurple", "nightclub", "tyrianpurple2", "cosmic", "persianplum2",
 "eggplant", "darkbyzantium", "purpletaupe", "blackberry",
  /* Beiges */
 "beige3", "blanchedalmond", "bananamania", "moccasin", "peachyellow", "wheat2",
 "bisque2", "navajowhite", "lightkhaki", "palegoldenrod2", "buff", "flax", "zinnwaldite",
 "desertsand2", "ecru", "tan2", "khaki", "darkkhaki", "rosybrown2", "terracotta2",
 "bittersweet", "sandybrown2", "goldenrod2", "brass2", "paletaupe", "antiquebrass",
 "bronze", "ochre2", "copper", "darkgoldenrod2", "sandytaupe2", "darktan", "shadow",
 "beaver", "frenchbeige", "palebrown", "peru2", "cocoabean", "tenne", "burntorange",
 "mahogany2", "darkterracotta2", "chestnut2", "fuzzywuzzybrown2", "deepchestnut2", "darkchestnut2",
 "mediumbrown", "sienna", "brown4", "saddlebrown", "rusty", "idaho", "burntumber2",
 "redbrown2", "burgundy2", "maroon2", "auburn2", "rawumber2", "pullmanbrown", "darkbrown",
 "sepia", "bole2", "mediumtaupe", "taupe", "cafe", "liver", "bistre", "sealbrown",
 "seashell", "oldlace", "antiquewhite", "blanchedalmond2", "champagne",
  /* Others (Orange/Yellow/Brown/Pink) */
 "bananamania2", "moccasin2", "bisque3", "navajowhite", "peachpuff", "peach", "deeppeach", "peachorange",
 "brightapricot", "apricot", "melon", "macaroniandcheese", "sandybrown3", "lightsalmon",
 "darksalmon3", "mediumsalmon2", "coral", "salmon3", "coralpink2", "lightcoral2", "bittersweet2",
 "terracotta3", "tearose", "vividtangerine", "atomictangerine", "orangepeel", "orange2",
 "deepsaffron", "darkorange", "tangerine", "orange4", "automotiveamber", "pumpkin",
 "richorange", "safetyorange", "cadmiumorange", "persimmon", "tangelo", "vermilion2",
 "redorange3", "electricvermilion2", "orangered2", "internationalorange", "portlandorange",
 "ultraorange", "cherry", "deepcarrotorange", "carrotorange", "neoncarrot", "gamboge",
 "ochre3", "bronze2", "copper2", "palebrown2", "antiquebrass2", "peru3", "cocoa",
 "tenne2", "burntorange2", "mahogany3", "darkTerracotta3", "darkcoral", "mediumbrown2",
 "sienna2", "brown2", "saddlebrown2", "rust2", "russet2", "redbrown3", "cognacbrown",
 "burntumber3", "pullmanbrown2", "darkbrown2", "mediumtaupe2", "taupe2",
  /* Greenish blues */
 "electricultramarine2",
 "mediumnavyblue", "mediumblue", "azure", "pigmentblue", "ultramarine2", "darkblue",
 "navy", "persianindigo2", "stpattyblue", "midnightblue", "sapphire", "palesilver",
 "silver", "aliceblue2", "bubbles", "lightcyan", "paleturquoise", "palerobineggblue",
 "aquamarine", "aquamarineblue", "babyblue", "electricblue", "cyan2", "turquoiseblue",
 "brightturquoise", "turquoise2", "mayablue", "capri", "processcyan", "brightcerulean",
 "deepturquoise", "robineggblue", "mediumturquoise", "mediumaquamarine", "lightseagreen",
 "cadetblue2", "viridian", "pinegreen", "darkcyan", "teal", "tealblue", "bondiblue",
 "richelectricblue", "cerulean", "steelblue2", "mediumelectricblue", "darkelectricblue",
 "midnightgreen", "palepink2", "pigpink2", "pink4", "cottoncandy2", "babypink2", "mediumpink2",
 "cherryblossompink2", "nadashikopink2", "carnationpink2", "lavenderpink", "lavenderrose",
 "lightfuchsiapink", "persianpink", "ticklemepink2", "mauvelous", "amaranthpink",
 "paleamaranthpink", "pastelpink2", "puce2", "palevioletred", "lightcarminepink2",
 "lightcoral3", "coralpink3", "salmon4", "lightsalmonpink2", "salmonpink2", "pinkorange", "coral2",
 "carnation", "carminepink2", "deepCarminepink2", "wildwatermelon", "brinkpink2", "darkpink2",
 "cranberry", "frenchrose", "violetred", "hotpink2", "brilliantrose", "raspberrypink2",
 "wildstrawberry", "brightpink", "deeppink", "cerisepink", "cerise2", "tyrianpink",
 "vividcerise", "amaranthcerise", "deepcerise", "hollywoodcerise", "persianrose",
 "shockingpink", "razzledazzlerose", "fuchsiapink", "ultrapink", "pinkflamingo", "magenta",
 "hotmagenta", "amaranthmagenta", "skymagenta", "fandango2",
 /* ANSI, mono terminal and C64 colors */
 "ansigrey", "ansired", "ansigreen", "ansiamber", "ansiblue", "ansimagenta", "ansicyan",
 "ansiboldgrey", "ansiboldred", "ansiboldgreen", "ansiboldblue", "ansiboldmagenta",
 "ansiwhite",
 "ansibackcyan", "ansibackred", "ansibackgrey",
 "ansimutedgreen", "ansimutedyellow", "ansimutedred", "ansimutedblue", "ansimutedgreen2",
 "ansialtred", "ansialtmagenta", "ansialtyellow", "ansialtblue", "ansialtwhite", "ansialtbrown", "ansialtgreen", "ansialtgrey", "ansialtblack", "ansialtcyan",
 "ansimonogreen", "ansimonoamber",
 "ansimsxamber",
 "c64blue", "c64cyan",
 "pullmangreen", "omahaorange", "imitationgold", "dupontgray", "vermillion3",
 "mineralred", "glaciergreen", "wfeyellow", "bigskyblue", "gunmetalgray",
  /* Glasses */
 "redglass", "greenglass", "blueglass", "magentaglass"
};

CrayonBox crayons;

void CrayonBox::gl( Colors c ) {
 Crayon crayon;
 crayon.Pick(c);
 glColor4fv(crayon.floats);
}

int CrayonBox::fansi( Crayon c ) {
 for ( int i=0; i<256; i++ ) { if ( c.Near(fansi(i),2) ) return i; }
 return -1;
}
Crayon CrayonBox::fansi( int i ) {
 switch ( i % 256 ) {
  default:
  case 0: return crayons.Pick(black);
  case 1:   { Crayon c; c.Int( 128, 0, 0, 255 );      return c; } break;
  case 2:   { Crayon c; c.Int( 0, 128, 0, 255 );      return c; } break;
  case 3:   { Crayon c; c.Int( 128, 128, 0, 255 );    return c; } break;
  case 4:   { Crayon c; c.Int( 0, 0, 128, 255 );      return c; } break;
  case 5:   { Crayon c; c.Int( 128, 0, 128, 255 );    return c; } break;
  case 6:   { Crayon c; c.Int( 0, 128, 128, 255 );    return c; } break;
  case 7:   { Crayon c; c.Int( 192, 192, 192, 255 );  return c; } break;
  case 8:   { Crayon c; c.Int( 128, 128, 128, 255 );  return c; } break;
  case 9:   { Crayon c; c.Int( 255, 0, 0, 255 );      return c; } break;
  case 10:  { Crayon c; c.Int( 0, 255, 0, 255 );      return c; } break;
  case 11:  { Crayon c; c.Int( 255, 255, 0, 255 );    return c; } break;
  case 12:  { Crayon c; c.Int( 0, 0, 255, 255 );      return c; } break;
  case 13:  { Crayon c; c.Int( 255, 0, 255, 255 );    return c; } break;
  case 14:  { Crayon c; c.Int( 0, 255, 255, 255 );    return c; } break;
  case 15:  { Crayon c; c.Int( 255, 255, 255, 255 );  return c; } break;
  case 16:  { Crayon c; c.Int( 0, 0, 0, 255 );        return c; } break;
  case 17:  { Crayon c; c.Int( 0, 0, 95, 255 );       return c; } break;
  case 18:  { Crayon c; c.Int( 0, 0, 135, 255 );      return c; } break;
  case 19:  { Crayon c; c.Int( 0, 0, 175, 255 );      return c; } break;
  case 20:  { Crayon c; c.Int( 0, 0, 215, 255 );      return c; } break;
  case 21:  { Crayon c; c.Int( 0, 0, 255, 255 );      return c; } break;
  case 22:  { Crayon c; c.Int( 0, 95, 0, 255 );       return c; } break;
  case 23:  { Crayon c; c.Int( 0, 95, 95, 255 );      return c; } break;
  case 24:  { Crayon c; c.Int( 0, 95, 135, 255 );     return c; } break;
  case 25:  { Crayon c; c.Int( 0, 95, 175, 255 );     return c; } break;
  case 26:  { Crayon c; c.Int( 0, 95, 215, 255 );     return c; } break;
  case 27:  { Crayon c; c.Int( 0, 95, 255, 255 );     return c; } break;
  case 28:  { Crayon c; c.Int( 0, 135, 0, 255 );      return c; } break;
  case 29:  { Crayon c; c.Int( 0, 135, 95, 255 );     return c; } break;
  case 30:  { Crayon c; c.Int( 0, 135, 135, 255 );    return c; } break;
  case 31:  { Crayon c; c.Int( 0, 135, 175, 255 );    return c; } break;
  case 32:  { Crayon c; c.Int( 0, 135, 215, 255 );    return c; } break;
  case 33:  { Crayon c; c.Int( 0, 135, 255, 255 );    return c; } break;
  case 34:  { Crayon c; c.Int( 0, 175, 0, 255 );      return c; } break;
  case 35:  { Crayon c; c.Int( 0, 175, 95, 255 );     return c; } break;
  case 36:  { Crayon c; c.Int( 0, 175, 135, 255 );    return c; } break;
  case 37:  { Crayon c; c.Int( 0, 175, 175, 255 );    return c; } break;
  case 38:  { Crayon c; c.Int( 0, 175, 215, 255 );    return c; } break;
  case 39:  { Crayon c; c.Int( 0, 175, 255, 255 );    return c; } break;
  case 40:  { Crayon c; c.Int( 0, 215, 0, 255 );      return c; } break;
  case 41:  { Crayon c; c.Int( 0, 215, 95, 255 );     return c; } break;
  case 42:  { Crayon c; c.Int( 0, 215, 135, 255 );    return c; } break;
  case 43:  { Crayon c; c.Int( 0, 215, 175, 255 );    return c; } break;
  case 44:  { Crayon c; c.Int( 0, 215, 215, 255 );    return c; } break;
  case 45:  { Crayon c; c.Int( 0, 215, 255, 255 );    return c; } break;
  case 46:  { Crayon c; c.Int( 0, 255, 0, 255 );      return c; } break;
  case 47:  { Crayon c; c.Int( 0, 255, 95, 255 );     return c; } break;
  case 48:  { Crayon c; c.Int( 0, 255, 135, 255 );    return c; } break;
  case 49:  { Crayon c; c.Int( 0, 255, 175, 255 );    return c; } break;
  case 50:  { Crayon c; c.Int( 0, 255, 215, 255 );    return c; } break;
  case 51:  { Crayon c; c.Int( 0, 255, 255, 255 );    return c; } break;
  case 52:  { Crayon c; c.Int( 95, 0, 0, 255 );       return c; } break;
  case 53:  { Crayon c; c.Int( 95, 0, 95, 255 );      return c; } break;
  case 54:  { Crayon c; c.Int( 95, 0, 135, 255 );     return c; } break;
  case 55:  { Crayon c; c.Int( 95, 0, 175, 255 );     return c; } break;
  case 56:  { Crayon c; c.Int( 95, 0, 215, 255 );     return c; } break;
  case 57:  { Crayon c; c.Int( 95, 0, 255, 255 );     return c; } break;
  case 58:  { Crayon c; c.Int( 95, 95, 0, 255 );      return c; } break;
  case 59:  { Crayon c; c.Int( 95, 95, 95, 255 );     return c; } break;
  case 60:  { Crayon c; c.Int( 95, 95, 135, 255 );    return c; } break;
  case 61:  { Crayon c; c.Int( 95, 95, 175, 255 );    return c; } break;
  case 62:  { Crayon c; c.Int( 95, 95, 215, 255 );    return c; } break;
  case 63:  { Crayon c; c.Int( 95, 95, 255, 255 );    return c; } break;
  case 64:  { Crayon c; c.Int( 95, 135, 0, 255 );     return c; } break;
  case 65:  { Crayon c; c.Int( 95, 135, 95, 255 );    return c; } break;
  case 66:  { Crayon c; c.Int( 95, 135, 135, 255 );   return c; } break;
  case 67:  { Crayon c; c.Int( 95, 135, 175, 255 );   return c; } break;
  case 68:  { Crayon c; c.Int( 95, 135, 215, 255 );   return c; } break;
  case 69:  { Crayon c; c.Int( 95, 135, 255, 255 );   return c; } break;
  case 70:  { Crayon c; c.Int( 95, 175, 0, 255 );     return c; } break;
  case 71:  { Crayon c; c.Int( 95, 175, 95, 255 );    return c; } break;
  case 72:  { Crayon c; c.Int( 95, 175, 135, 255 );   return c; } break;
  case 73:  { Crayon c; c.Int( 95, 175, 175, 255 );   return c; } break;
  case 74:  { Crayon c; c.Int( 95, 175, 215, 255 );   return c; } break;
  case 75:  { Crayon c; c.Int( 95, 175, 255, 255 );   return c; } break;
  case 76:  { Crayon c; c.Int( 95, 215, 0, 255 );     return c; } break;
  case 77:  { Crayon c; c.Int( 95, 215, 95, 255 );    return c; } break;
  case 78:  { Crayon c; c.Int( 95, 215, 135, 255 );   return c; } break;
  case 79:  { Crayon c; c.Int( 95, 215, 175, 255 );   return c; } break;
  case 80:  { Crayon c; c.Int( 95, 215, 215, 255 );   return c; } break;
  case 81:  { Crayon c; c.Int( 95, 215, 255, 255 );   return c; } break;
  case 82:  { Crayon c; c.Int( 95, 255, 0, 255 );     return c; } break;
  case 83:  { Crayon c; c.Int( 95, 255, 95, 255 );    return c; } break;
  case 84:  { Crayon c; c.Int( 95, 255, 135, 255 );   return c; } break;
  case 85:  { Crayon c; c.Int( 95, 255, 175, 255 );   return c; } break;
  case 86:  { Crayon c; c.Int( 95, 255, 215, 255 );   return c; } break;
  case 87:  { Crayon c; c.Int( 95, 255, 255, 255 );   return c; } break;
  case 88:  { Crayon c; c.Int( 135, 0, 0, 255 );      return c; } break;
  case 89:  { Crayon c; c.Int( 135, 0, 95, 255 );     return c; } break;
  case 90:  { Crayon c; c.Int( 135, 0, 135, 255 );    return c; } break;
  case 91:  { Crayon c; c.Int( 135, 0, 175, 255 );    return c; } break;
  case 92:  { Crayon c; c.Int( 135, 0, 215, 255 );    return c; } break;
  case 93:  { Crayon c; c.Int( 135, 0, 255, 255 );    return c; } break;
  case 94:  { Crayon c; c.Int( 135, 95, 0, 255 );     return c; } break;
  case 95:  { Crayon c; c.Int( 135, 95, 95, 255 );    return c; } break;
  case 96:  { Crayon c; c.Int( 135, 95, 135, 255 );   return c; } break;
  case 97:  { Crayon c; c.Int( 135, 95, 175, 255 );   return c; } break;
  case 98:  { Crayon c; c.Int( 135, 95, 215, 255 );   return c; } break;
  case 99:  { Crayon c; c.Int( 135, 95, 255, 255 );   return c; } break;
  case 100: { Crayon c; c.Int( 135, 135, 0, 255 );    return c; } break;
  case 101: { Crayon c; c.Int( 135, 135, 95, 255 );   return c; } break;
  case 102: { Crayon c; c.Int( 135, 135, 135, 255 );  return c; } break;
  case 103: { Crayon c; c.Int( 135, 135, 175, 255 );  return c; } break;
  case 104: { Crayon c; c.Int( 135, 135, 215, 255 );  return c; } break;
  case 105: { Crayon c; c.Int( 135, 135, 255, 255 );  return c; } break;
  case 106: { Crayon c; c.Int( 135, 175, 0, 255 );    return c; } break;
  case 107: { Crayon c; c.Int( 135, 175, 95, 255 );   return c; } break;
  case 108: { Crayon c; c.Int( 135, 175, 135, 255 );  return c; } break;
  case 109: { Crayon c; c.Int( 135, 175, 175, 255 );  return c; } break;
  case 110: { Crayon c; c.Int( 135, 175, 215, 255 );  return c; } break;
  case 111: { Crayon c; c.Int( 135, 175, 255, 255 );  return c; } break;
  case 112: { Crayon c; c.Int( 135, 215, 0, 255 );    return c; } break;
  case 113: { Crayon c; c.Int( 135, 215, 95, 255 );   return c; } break;
  case 114: { Crayon c; c.Int( 135, 215, 135, 255 );  return c; } break;
  case 115: { Crayon c; c.Int( 135, 215, 175, 255 );  return c; } break;
  case 116: { Crayon c; c.Int( 135, 215, 215, 255 );  return c; } break;
  case 117: { Crayon c; c.Int( 135, 215, 255, 255 );  return c; } break;
  case 118: { Crayon c; c.Int( 135, 255, 0, 255 );    return c; } break;
  case 119: { Crayon c; c.Int( 135, 255, 95, 255 );   return c; } break;
  case 120: { Crayon c; c.Int( 135, 255, 135, 255 );  return c; } break;
  case 121: { Crayon c; c.Int( 135, 255, 175, 255 );  return c; } break;
  case 122: { Crayon c; c.Int( 135, 255, 215, 255 );  return c; } break;
  case 123: { Crayon c; c.Int( 135, 255, 255, 255 );  return c; } break;
  case 124: { Crayon c; c.Int( 175, 0, 0, 255 );      return c; } break;
  case 125: { Crayon c; c.Int( 175, 0, 95, 255 );     return c; } break;
  case 126: { Crayon c; c.Int( 175, 0, 135, 255 );    return c; } break;
  case 127: { Crayon c; c.Int( 175, 0, 175, 255 );    return c; } break;
  case 128: { Crayon c; c.Int( 175, 0, 215, 255 );    return c; } break;
  case 129: { Crayon c; c.Int( 175, 0, 255, 255 );    return c; } break;
  case 130: { Crayon c; c.Int( 175, 95, 0, 255 );     return c; } break;
  case 131: { Crayon c; c.Int( 175, 95, 95, 255 );    return c; } break;
  case 132: { Crayon c; c.Int( 175, 95, 135, 255 );   return c; } break;
  case 133: { Crayon c; c.Int( 175, 95, 175, 255 );   return c; } break;
  case 134: { Crayon c; c.Int( 175, 95, 215, 255 );   return c; } break;
  case 135: { Crayon c; c.Int( 175, 95, 255, 255 );   return c; } break;
  case 136: { Crayon c; c.Int( 175, 135, 0, 255 );    return c; } break;
  case 137: { Crayon c; c.Int( 175, 135, 95, 255 );   return c; } break;
  case 138: { Crayon c; c.Int( 175, 135, 135, 255 );  return c; } break;
  case 139: { Crayon c; c.Int( 175, 135, 175, 255 );  return c; } break;
  case 140: { Crayon c; c.Int( 175, 135, 215, 255 );  return c; } break;
  case 141: { Crayon c; c.Int( 175, 135, 255, 255 );  return c; } break;
  case 142: { Crayon c; c.Int( 175, 175, 0, 255 );    return c; } break;
  case 143: { Crayon c; c.Int( 175, 175, 95, 255 );   return c; } break;
  case 144: { Crayon c; c.Int( 175, 175, 135, 255 );  return c; } break;
  case 145: { Crayon c; c.Int( 175, 175, 175, 255 );  return c; } break;
  case 146: { Crayon c; c.Int( 175, 175, 215, 255 );  return c; } break;
  case 147: { Crayon c; c.Int( 175, 175, 255, 255 );  return c; } break;
  case 148: { Crayon c; c.Int( 175, 215, 0, 255 );    return c; } break;
  case 149: { Crayon c; c.Int( 175, 215, 95, 255 );   return c; } break;
  case 150: { Crayon c; c.Int( 175, 215, 135, 255 );  return c; } break;
  case 151: { Crayon c; c.Int( 175, 215, 175, 255 );  return c; } break;
  case 152: { Crayon c; c.Int( 175, 215, 215, 255 );  return c; } break;
  case 153: { Crayon c; c.Int( 175, 215, 255, 255 );  return c; } break;
  case 154: { Crayon c; c.Int( 175, 255, 0, 255 );    return c; } break;
  case 155: { Crayon c; c.Int( 175, 255, 95, 255 );   return c; } break;
  case 156: { Crayon c; c.Int( 175, 255, 135, 255 );  return c; } break;
  case 157: { Crayon c; c.Int( 175, 255, 175, 255 );  return c; } break;
  case 158: { Crayon c; c.Int( 175, 255, 215, 255 );  return c; } break;
  case 159: { Crayon c; c.Int( 175, 255, 255, 255 );  return c; } break;
  case 160: { Crayon c; c.Int( 215, 0, 0, 255 );      return c; } break;
  case 161: { Crayon c; c.Int( 215, 0, 95, 255 );     return c; } break;
  case 162: { Crayon c; c.Int( 215, 0, 135, 255 );    return c; } break;
  case 163: { Crayon c; c.Int( 215, 0, 175, 255 );    return c; } break;
  case 164: { Crayon c; c.Int( 215, 0, 215, 255 );    return c; } break;
  case 165: { Crayon c; c.Int( 215, 0, 255, 255 );    return c; } break;
  case 166: { Crayon c; c.Int( 215, 95, 0, 255 );     return c; } break;
  case 167: { Crayon c; c.Int( 215, 95, 95, 255 );    return c; } break;
  case 168: { Crayon c; c.Int( 215, 95, 135, 255 );   return c; } break;
  case 169: { Crayon c; c.Int( 215, 95, 175, 255 );   return c; } break;
  case 170: { Crayon c; c.Int( 215, 95, 215, 255 );   return c; } break;
  case 171: { Crayon c; c.Int( 215, 95, 255, 255 );   return c; } break;
  case 172: { Crayon c; c.Int( 215, 135, 0, 255 );    return c; } break;
  case 173: { Crayon c; c.Int( 215, 135, 95, 255 );   return c; } break;
  case 174: { Crayon c; c.Int( 215, 135, 135, 255 );  return c; } break;
  case 175: { Crayon c; c.Int( 215, 135, 175, 255 );  return c; } break;
  case 176: { Crayon c; c.Int( 215, 135, 215, 255 );  return c; } break;
  case 177: { Crayon c; c.Int( 215, 135, 255, 255 );  return c; } break;
  case 178: { Crayon c; c.Int( 215, 175, 0, 255 );    return c; } break;
  case 179: { Crayon c; c.Int( 215, 175, 95, 255 );   return c; } break;
  case 180: { Crayon c; c.Int( 215, 175, 135, 255 );  return c; } break;
  case 181: { Crayon c; c.Int( 215, 175, 175, 255 );  return c; } break;
  case 182: { Crayon c; c.Int( 215, 175, 215, 255 );  return c; } break;
  case 183: { Crayon c; c.Int( 215, 175, 255, 255 );  return c; } break;
  case 184: { Crayon c; c.Int( 215, 215, 0, 255 );    return c; } break;
  case 185: { Crayon c; c.Int( 215, 215, 95, 255 );   return c; } break;
  case 186: { Crayon c; c.Int( 215, 215, 135, 255 );  return c; } break;
  case 187: { Crayon c; c.Int( 215, 215, 175, 255 );  return c; } break;
  case 188: { Crayon c; c.Int( 215, 215, 215, 255 );  return c; } break;
  case 189: { Crayon c; c.Int( 215, 215, 255, 255 );  return c; } break;
  case 190: { Crayon c; c.Int( 215, 255, 0, 255 );    return c; } break;
  case 191: { Crayon c; c.Int( 215, 255, 95, 255 );   return c; } break;
  case 192: { Crayon c; c.Int( 215, 255, 135, 255 );  return c; } break;
  case 193: { Crayon c; c.Int( 215, 255, 175, 255 );  return c; } break;
  case 194: { Crayon c; c.Int( 215, 255, 215, 255 );  return c; } break;
  case 195: { Crayon c; c.Int( 215, 255, 255, 255 );  return c; } break;
  case 196: { Crayon c; c.Int( 255, 0, 0, 255 );      return c; } break;
  case 197: { Crayon c; c.Int( 255, 0, 95, 255 );     return c; } break;
  case 198: { Crayon c; c.Int( 255, 0, 135, 255 );    return c; } break;
  case 199: { Crayon c; c.Int( 255, 0, 175, 255 );    return c; } break;
  case 200: { Crayon c; c.Int( 255, 0, 215, 255 );    return c; } break;
  case 201: { Crayon c; c.Int( 255, 0, 255, 255 );    return c; } break;
  case 202: { Crayon c; c.Int( 255, 95, 0, 255 );     return c; } break;
  case 203: { Crayon c; c.Int( 255, 95, 95, 255 );    return c; } break;
  case 204: { Crayon c; c.Int( 255, 95, 135, 255 );   return c; } break;
  case 205: { Crayon c; c.Int( 255, 95, 175, 255 );   return c; } break;
  case 206: { Crayon c; c.Int( 255, 95, 215, 255 );   return c; } break;
  case 207: { Crayon c; c.Int( 255, 95, 255, 255 );   return c; } break;
  case 208: { Crayon c; c.Int( 255, 135, 0, 255 );    return c; } break;
  case 209: { Crayon c; c.Int( 255, 135, 95, 255 );   return c; } break;
  case 210: { Crayon c; c.Int( 255, 135, 135, 255 );  return c; } break;
  case 211: { Crayon c; c.Int( 255, 135, 175, 255 );  return c; } break;
  case 212: { Crayon c; c.Int( 255, 135, 215, 255 );  return c; } break;
  case 213: { Crayon c; c.Int( 255, 135, 255, 255 );  return c; } break;
  case 214: { Crayon c; c.Int( 255, 175, 0, 255 );    return c; } break;
  case 215: { Crayon c; c.Int( 255, 175, 95, 255 );   return c; } break;
  case 216: { Crayon c; c.Int( 255, 175, 135, 255 );  return c; } break;
  case 217: { Crayon c; c.Int( 255, 175, 175, 255 );  return c; } break;
  case 218: { Crayon c; c.Int( 255, 175, 215, 255 );  return c; } break;
  case 219: { Crayon c; c.Int( 255, 175, 255, 255 );  return c; } break;
  case 220: { Crayon c; c.Int( 255, 215, 0, 255 );    return c; } break;
  case 221: { Crayon c; c.Int( 255, 215, 95, 255 );   return c; } break;
  case 222: { Crayon c; c.Int( 255, 215, 135, 255 );  return c; } break;
  case 223: { Crayon c; c.Int( 255, 215, 175, 255 );  return c; } break;
  case 224: { Crayon c; c.Int( 255, 215, 215, 255 );  return c; } break;
  case 225: { Crayon c; c.Int( 255, 215, 255, 255 );  return c; } break;
  case 226: { Crayon c; c.Int( 255, 255, 0, 255 );    return c; } break;
  case 227: { Crayon c; c.Int( 255, 255, 95, 255 );   return c; } break;
  case 228: { Crayon c; c.Int( 255, 255, 135, 255 );  return c; } break;
  case 229: { Crayon c; c.Int( 255, 255, 175, 255 );  return c; } break;
  case 230: { Crayon c; c.Int( 255, 255, 215, 255 );  return c; } break;
  case 231: { Crayon c; c.Int( 255, 255, 255, 255 );  return c; } break;
  case 232: { Crayon c; c.Int( 0, 0, 0, 255 );        return c; } break;
  case 233: { Crayon c; c.Int( 18, 18, 18, 255 );     return c; } break;
  case 234: { Crayon c; c.Int( 28, 28, 28, 255 );     return c; } break;
  case 235: { Crayon c; c.Int( 38, 38, 38, 255 );     return c; } break;
  case 236: { Crayon c; c.Int( 48, 48, 48, 255 );     return c; } break;
  case 237: { Crayon c; c.Int( 58, 58, 58, 255 );     return c; } break;
  case 238: { Crayon c; c.Int( 68, 68, 68, 255 );     return c; } break;
  case 239: { Crayon c; c.Int( 78, 78, 78, 255 );     return c; } break;
  case 240: { Crayon c; c.Int( 88, 88, 88, 255 );     return c; } break;
  case 241: { Crayon c; c.Int( 98, 98, 98, 255 );     return c; } break;
  case 242: { Crayon c; c.Int( 108, 108, 108, 255 );  return c; } break;
  case 243: { Crayon c; c.Int( 118, 118, 118, 255 );  return c; } break;
  case 244: { Crayon c; c.Int( 128, 128, 128, 255 );  return c; } break;
  case 245: { Crayon c; c.Int( 138, 138, 138, 255 );  return c; } break;
  case 246: { Crayon c; c.Int( 148, 148, 148, 255 );  return c; } break;
  case 247: { Crayon c; c.Int( 158, 158, 158, 255 );  return c; } break;
  case 248: { Crayon c; c.Int( 168, 168, 168, 255 );  return c; } break;
  case 249: { Crayon c; c.Int( 178, 178, 178, 255 );  return c; } break;
  case 250: { Crayon c; c.Int( 188, 188, 188, 255 );  return c; } break;
  case 251: { Crayon c; c.Int( 198, 198, 198, 255 );  return c; } break;
  case 252: { Crayon c; c.Int( 208, 208, 208, 255 );  return c; } break;
  case 253: { Crayon c; c.Int( 218, 218, 218, 255 );  return c; } break;
  case 254: { Crayon c; c.Int( 228, 228, 228, 255 );  return c; } break;
  case 255: { Crayon c; c.Int( 238, 238, 238, 255 );  return c; } break;
 }
}

int CrayonBox::c16x16( Crayon c ) {
 for ( int i=0; i<256; i++ ) { if ( c.Near(c16x16(i),2) ) return i; }
 return -1;
}
Crayon CrayonBox::c16x16( int i ) {
 Crayon result;
 int x=(i%16);
 int y=(i/16);
 switch ( y ) {
 case  0: result.Int(x*16,    0,    0,255); break; // red ramp
 case  1: result.Int(0,    x*16,    0,255); break; // green ramp
 case  2: result.Int(0,       0, x*16,255); break; // blue ramp
 case  3: result.Int(x*16, x*16,    0,255); break; // r+g ramp
 case  4: result.Int(0,    x*16, x*16,255); break; // g+b ramp
 case  5: result.Int(x*16,    0, x*16,255); break; // r+b ramp
 case  6: result.Int(x*16,  x*8,  x*4,255); break; // r16 g8 b4 ramp
 case  7: result.Int(x*16,  x*4,  x*8,255); break; // r16 g4 b8 ramp
 case  8: result.Int(x*16,    0,  x*4,255); break; // r16 g0 b4 ramp
 case  9: result.Int(x*4,   x*8, x*16,255); break; // r4 g8 b16 ramp
 case 10: result.Int(x*4,  x*16,  x*8,255); break; // r4 g16 b8 ramp
 case 11: result.Int(x*8,   x*4, x*16,255); break; // r8 g4 b16 ramp
 case 12: result.Int(x*8,  x*16,  x*4,255); break; // r8 g16 b4 ramp
 case 13: result.Int(x*4,     0, x*16,255); break; // r4 g0 b16 ramp
 case 14: result.Int(x*16,  x*8,    0,255); break; // r16 g8 b0 ramp
 case 15: result.Int(x*16, x*16, x*16,255); break; // r+g+b ramp
 }
 return result;
}

void Crayon::BinaryWrite( BinaryFile *open ) {
 open->write(&rf);
 open->write(&gf);
 open->write(&bf);
 open->write(&af);
}

void Crayon::BinaryRead( BinaryFile *open ) {
 open->read(&rf);
 open->read(&gf);
 open->read(&bf);
 open->read(&af);
 Float(rf,gf,bf,af);
}

Crayon::Crayon(void)
{
 rd=gd=bd=ad=0.0;
 rf=gf=bf=af=0.0f;
 r=g=b=a=0;
 f255=false;
 floats[0]=floats[1]=floats[2]=floats[3]=0.0f;
}

Crayon::Crayon(int R, int G, int B) {
 rd=gd=bd=ad=0.0;
 rf=gf=bf=af=0.0f;
 r=g=b=a=0;
 f255=false;
 floats[0]=floats[1]=floats[2]=floats[3]=0.0f;
 Int(R,G,B,255);
}

Crayon::Crayon(float R, float G, float B) {
 rd=gd=bd=ad=0.0;
 rf=gf=bf=af=0.0f;
 r=g=b=a=0;
 f255=false;
 floats[0]=floats[1]=floats[2]=floats[3]=0.0f;
 Float(R,G,B,1.0f);
}

Crayon::Crayon(float R, float G, float B, float A) {
 rd=gd=bd=ad=0.0;
 rf=gf=bf=af=0.0f;
 r=g=b=a=0;
 f255=false;
 floats[0]=floats[1]=floats[2]=floats[3]=0.0f;
 Float(R,G,B,A);
}

Crayon::~Crayon(void)
{
}

void Crayon::gl(void) {
 glColor4fv(floats);
}

void Crayon::gl(Blends blend) {
 Blending(blend);
 glColor4fv(floats);
}

void Crayon::Wrapi(void)
{
 while ( this->r<0 ) this->r=255-this->r;  while ( this->r>255 ) this->r-=255;
 while ( this->g<0 ) this->g=255-this->g;  while ( this->g>255 ) this->g-=255;
 while ( this->b<0 ) this->b=255-this->b;  while ( this->b>255 ) this->b-=255;
 while ( this->a<0 ) this->a=255-this->a;  while ( this->a>255 ) this->a-=255;
}

void Crayon::SubWrapf( float sub )
{
 rf=rf-sub; if ( rf<0.0f ) rf+=1.0f;
 bf=bf-sub; if ( gf<0.0f ) gf+=1.0f;
 gf=gf-sub; if ( bf<0.0f ) bf+=1.0f;
 af=af-sub; if ( af<0.0f ) af+=1.0f;
 floats[0]=rf;
 floats[1]=bf;
 floats[2]=gf;
 floats[3]=af;
}

void Crayon::Absi(void)
{
 this->r=abs(this->r);
 this->g=abs(this->g);
 this->b=abs(this->b);
 this->a=abs(this->a);
}

void Crayon::ColorInterp( Crayon A, Crayon B, float f )
{
 this->r=iround( (float) ilerp( A.r, B.r, f ) );
 this->g=iround( (float) ilerp( A.g, B.g, f ) );
 this->b=iround( (float) ilerp( A.b, B.b, f ) );
 this->a=iround( (float) ilerp( A.a, B.a, f ) );
 rd=(double)(floats[0]=(this->rf=(float) r / 255.0f));
 gd=(double)(floats[1]=(this->gf=(float) g / 255.0f));
 bd=(double)(floats[2]=(this->bf=(float) b / 255.0f));
 ad=(double)(floats[3]=(this->af=(float) a / 255.0f));
}

void Crayon::ColorCombine4( Crayon A, Crayon B, Crayon C, Crayon D, float i1, float i2, float i3, float alpha ) {
 Crayon AB; AB.ColorInterp(A,B,abs(i1));
 Crayon BC; BC.ColorInterp(B,C,abs(i2));
 Crayon CD; CD.ColorInterp(C,D,abs(i3));
 Crayon DA; DA.ColorInterp(D,A,abs(1.0f-i2));
 Float(
  (AB.rf+BC.rf+CD.rf+DA.rf)/4.0f,
  (AB.gf+BC.gf+CD.gf+DA.gf)/4.0f,
  (AB.bf+BC.bf+CD.bf+DA.bf)/4.0f,
  1.0f
 );
}

// If f is an alpha value, B is added to A with B's alpha as the interpolation factor of B
void Crayon::ColorAddTransparency( Crayon A, Crayon B )
{
 this->r=iround( (float) ilerp( A.r, B.r, B.af ) );
 this->g=iround( (float) ilerp( A.g, B.g, B.af ) );
 this->b=iround( (float) ilerp( A.b, B.b, B.af ) );
 this->a=255;
 floats[0]=this->rf=(float) r / 255.0f;
 floats[1]=this->gf=(float) g / 255.0f;
 floats[2]=this->bf=(float) b / 255.0f;
 floats[3]=this->af=1.0f;
}

void Crayon::SubColorFactori( Crayon A, Crayon B, float f )
{
 Int(
  iround( (A.r-B.r)*f ),
  iround( (A.g-B.g)*f ),
  iround( (A.b-B.b)*f ),
  iround( (A.a-B.a)*f )
 );
}

void Crayon::SubColori( Crayon A, Crayon B )
{
 Int(
  A.r-B.r,
  A.g-B.g,
  A.b-B.b,
  A.a-B.a
 );
}

void Crayon::AddColori( Crayon A, Crayon B )
{
 Int(
  A.r+B.r,
  A.g+B.g,
  A.b+B.b,
  A.a+B.a
 );
}

Crayon Crayon::ScaleByAlpha( float alpha )
{
 Float(
  this->rf*(alpha/255.0f),
  this->gf*(alpha/255.0f),
  this->bf*(alpha/255.0f),
  1.0f
 );
 return *this;
}

Crayon Crayon::ScaleByAlpha( double alpha )
{
 Float(
  this->rf*(float)(alpha/255.0),
  this->gf*(float)(alpha/255.0),
  this->bf*(float)(alpha/255.0),
  1.0f
 );
 return *this;
}

void Crayon::SubColorf( Crayon A, Crayon B )
{
 Float(
  A.rf-B.rf,
  A.gf-B.gf,
  A.bf-B.bf,
  A.af-B.af
 );
}

void Crayon::AddColorf( Crayon A, Crayon B )
{
 Float(
  A.rf+B.rf,
  A.gf+B.gf,
  A.bf+B.bf,
  A.af+B.af
 );
}

void Crayon::ScaleByAlphaf( float alpha )
{
 Float(
  this->rf*alpha,
  this->gf*alpha,
  this->bf*alpha,
  1.0f
 );
}

Crayon Crayon::Alpha( float alpha ) {
 af=floats[3]=alpha;
 ad=(double)af;
 a=(int) (alpha*255.0f);
 return *this;
}

Crayon Crayon::Alpha( double alpha ) {
 ad=alpha;
 af=floats[3]=(float)alpha;
 a=(int) (alpha*255.0f);
 return *this;
}

void Crayon::Int( int R, int G, int B, int A )
{
 r=R;
 g=G;
 b=B;
 a=A;
 floats[0]=rf=((float) R / 255.0f);
 floats[1]=gf=((float) G / 255.0f);
 floats[2]=bf=((float) B / 255.0f);
 floats[3]=af=((float) A / 255.0f);
 rd=(double)rf;
 gd=(double)gf;
 bd=(double)bf;
 ad=(double)af;
}

void Crayon::Float( float R, float G, float B, float A )
{
 r=(int) (R*255.0f);
 g=(int) (G*255.0f);
 b=(int) (B*255.0f);
 a=(int) (A*255.0f);
 rf=floats[0]=R;
 gf=floats[1]=G;
 bf=floats[2]=B;
 af=floats[3]=A;
 rd=(double)rf;
 gd=(double)gf;
 bd=(double)bf;
 ad=(double)af;
}

void Crayon::Float255( float R, float G, float B, float A )
{
 r=(int) (R);
 g=(int) (G);
 b=(int) (B);
 a=(int) (A);
 rf=R;
 gf=G;
 bf=B;
 af=A;
 f255=true;
 floats[0]=rf;
 floats[1]=gf;
 floats[2]=bf;
 floats[3]=af;
 rd=(double)rf;
 gd=(double)gf;
 bd=(double)bf;
 ad=(double)af;
}

// Color by name
void Crayon::Name( const char *s )
{
}

void Crayon::fromCrayon( Crayon C )
{
 floats[0]=this->rf=C.rf;
 floats[1]=this->gf=C.gf;
 floats[2]=this->bf=C.bf;
 floats[3]=this->af=C.af;
 this->r=C.r;
 this->g=C.g;
 this->b=C.b;
 this->a=C.a;
 this->f255=C.f255;
 this->rd=C.rd;
 this->gd=C.gd;
 this->bd=C.bd;
 this->ad=C.ad;
}

void Crayon::fromFloat( float *f )
{
 floats[0]=this->rf=f[0];
 floats[1]=this->gf=f[1];
 floats[2]=this->bf=f[2];
 floats[3]=this->af=f[3];
 this->r=(int) (255.0f*f[0]);
 this->g=(int) (255.0f*f[1]);
 this->b=(int) (255.0f*f[2]);
 this->a=(int) (255.0f*f[3]);
 rd=(double)rf;
 gd=(double)gf;
 bd=(double)bf;
 ad=(double)af;
}

void Crayon::fromFloats( float *f )
{
 floats[0]=this->rf=f[0];
 floats[1]=this->gf=f[1];
 floats[2]=this->bf=f[2];
 floats[3]=this->af=f[3];
 this->r=(int) (255.0f*f[0]);
 this->g=(int) (255.0f*f[1]);
 this->b=(int) (255.0f*f[2]);
 this->a=(int) (255.0f*f[3]);
 rd=(double)rf;
 gd=(double)gf;
 bd=(double)bf;
 ad=(double)af;
}

float *Crayon::asFloatPrenormalizedf()
{
 float *f=new float[4];
 f[0]=(float) this->r;
 f[1]=(float) this->g;
 f[2]=(float) this->b;
 f[3]=(float) this->a;
 return f;
}

float *Crayon::asFloati()
{
 float *f=new float[4];
 f[0]=(float) this->r/255.0f;
 f[1]=(float) this->g/255.0f;
 f[2]=(float) this->b/255.0f;
 f[3]=(float) this->a/255.0f;
 return f; 
}

float *Crayon::asFloatf()
{
 float *f=new float[4]; 
 f[0]=this->rf/255.0f;
 f[1]=this->gf/255.0f;
 f[2]=this->bf/255.0f;
 f[3]=this->af/255.0f;
 return f;
}

float *Crayon::asFloatAlphai( float alpha )
{
 float *f=new float[4];
 f[0]=(float) this->r/255.0f;
 f[1]=(float) this->g/255.0f;
 f[2]=(float) this->b/255.0f;
 f[3]=alpha;
 return f;
}

float *Crayon::asFloatAlphaf( float alpha )
{
 float *f=new float[4];
 f[0]=this->rf;
 f[1]=this->gf;
 f[2]=this->bf;
 f[3]=alpha;
 return f;
}

void Crayon::Opacity( int o ) {
 af= f255 ? (float) o : ((float) o/255.0f);
 a=o;
}

void Crayon::Opacity( float o ) {
 af=o;
 a=(int) (255.0f*o);
 floats[3]=this->af;
}

void Crayon::Pick( Colors c ) {
 fromCrayon(crayons.Pick(c));
}

void Crayon::Any() {
 *this=crayons.Any();
}

void Crayon::Any( int seed ) {
 *this=crayons.Any(seed);
}

void Crayon::random() {
 Float(
  float_range(0.0f,1.0f),
  float_range(0.0f,1.0f),
  float_range(0.0f,1.0f),
  1.0f
 );
}

void Crayon::random( int seed ) {
 Random *s=randoms.GetSet(seed);
 Float( s->range(seed,seed+1,seed+2,0.0f,1.0f),
        s->range(seed+3,seed+4,seed+5,0.0f,1.0f),
        s->range(seed+6,seed+7,seed+8,0.0f,1.0f),
        1.0f
      );
}

void Crayons::ReadAppend( BinaryFile *open ) {
 CrayonHandle *reading=new CrayonHandle;
 Append(reading);
 reading->color.BinaryRead(open);
}

void Crayons::Hold(Colors c) {
 CrayonHandle *crayon=new CrayonHandle;
 crayon->color.fromCrayon(crayons.Pick(c));
 Append(crayon);
}

void Crayon::Solid( int seed ) {
 fromCrayon(crayons.SolidSeeded(seed));
}

void Crayon::Brights( int seed ) {
 fromCrayon(crayons.Brights(seed));
}

void Crayon::Vivids( int seed ) {
 fromCrayon(crayons.Vivids(seed));
}


bool operator== ( const Crayon& a, const Crayon& b ) {
 return a.rf == b.rf
     && a.gf == b.gf
     && a.bf == b.bf
     && a.af == b.af;
}

bool operator!= ( const Crayon& a, const Crayon& b ) {
 return !(a == b);
}

Crayon operator+ ( const Crayon& a, const Crayon& b ) {
 Crayon result;
 result=a;
 result+=b;
 return result;
}

Crayon operator* ( const Crayon& a, const Crayon& b ) {
 Crayon result;
 result=a;
 result*=b;
 return result;
}

CrayonBox::CrayonBox() {
  pick.Size(TOTAL_COLORS_AND_GLASS);
  pick[  0].Int(  80, 150, 161, 255 );  // cyan
  pick[  1].Int( 130,  60,   0, 255 );  // chocolate
  pick[  2].Int( 255, 255, 255, 255 );  // alabaster (white)
  pick[  3].Int(   0,   0,   0, 255 );  // black
  pick[  4].Int(   0,   0,   0,   0 );  // clear
  pick[  5].Int( 255,   0,   0, 255 );  // red255
  pick[  6].Int(   0, 255,   0, 255 );  // green255
  pick[  7].Int(   0,   0, 255, 255 );  // blue255
  pick[  8].Int( 236,   0, 132, 255 );  // hotPink
  pick[  9].Int(  75, 179, 230, 255 );  // aliceBlue
  pick[ 10].Int(   0,   0, 255, 255 );  // blue
  pick[ 11].Int(   0, 255,   0, 255 );  // green
  pick[ 12].Int(  96,  96, 255, 255 );  // cadetBlue
  pick[ 13].Int( 238,  42,  36, 255 );  // orangeRed
  pick[ 14].Int( 204, 102,  51, 255 );  // brown
  pick[ 15].Int(   0,   0,  30, 255 );  // indigo
  pick[ 16].Int( 220,  60, 175, 255 );  // thistle
  pick[ 17].Int( 186,  32,  37, 255 );  // darkRed
  pick[ 18].Int( 171,  63,  36, 255 );  // darkSalmon
  pick[ 19].Int( 223,  90,  45, 255 );  // salmon
  pick[ 20].Int( 150,  90, 165, 255 );  // purple
  pick[ 21].Int(   0, 126, 255, 255 );  // deepSkyBlue
  pick[ 22].Int(  85, 104, 176, 255 );  // steelBlue
  pick[ 23].Int(  90, 135, 167, 255 );  // slateBlue
  pick[ 24].Int( 255, 204,  51, 255 );  // yellow
  pick[ 25].Int( 247, 145,  30, 255 );  // orange
  pick[ 26].Int( 153,   0,   0, 255 );  // bloodRed
  pick[ 27].Int(   0, 126, 255, 255 );  // skyBlue
  pick[ 28].Int( 239, 115,  61, 255 );  // roseBrown
  pick[ 29].Int( 140,  80, 239, 255 );  // violet
  pick[ 30].Int(  80,  40,   0, 255 );  // darkChocolate
  pick[ 31].Int( 148,  12,  41, 255 );  // crimson
  pick[ 32].Int( 203, 217,  43, 255 );  // chartreuse
  pick[ 33].Int(  78, 144, 217, 255 );  // lightSkyBlue
  pick[ 34].Int( 164, 106,  40, 255 );  // beige
  pick[ 35].Int( 210, 108,  72, 255 );  // bisque
  pick[ 36].Int( 209,  37,  39, 255 );  // fireBrick
  pick[ 37].Int( 255, 204,   0, 255 );  // gold
  pick[ 38].Int( 255, 245, 230, 255 );  // OldLace
  pick[ 39].Int( 250, 240, 230, 255 );  // Linen
  pick[ 40].Int(   0, 206, 209, 255 );  // turquoise
  pick[ 41].Int( 127, 127, 127, 255 );  // gray
  pick[ 42].Int( 100, 149, 237, 255 );  // cornflowerBlue
  pick[ 43].Int(  32,  32,  32, 255 );  // charcoal
  pick[ 44].Int( 255, 248, 220, 255 );  // cornsilk
  pick[ 45].Int( 245, 245, 220, 255 );  // beige
  pick[ 46].Int( 255, 253, 208, 255 );  // cream
  pick[ 47].Int( 255, 250, 205, 255 );  // lemonChiffon
  pick[ 48].Int( 238, 232, 170, 255 );  // paleGoldenrod
  pick[ 49].Int( 245, 222, 179, 255 );  // wheat
  pick[ 50].Int( 254, 216,  93, 255 );  // dandelion
  pick[ 51].Int( 255, 219,  88, 255 );  // mustard
  pick[ 52].Int( 252, 214, 103, 255 );  // mediumGoldenrod
  pick[ 53].Int( 251, 236,  93, 255 );  // corn
  pick[ 54].Int( 253, 233,  16, 255 );  // lemonYellow
  pick[ 55].Int( 255, 223,   0, 255 );  // goldenYellow
  pick[ 56].Int( 255, 216,   0, 255 );  // schoolBusYellow
  pick[ 57].Int( 255, 215,   0, 255 );  // gOLDEN
  pick[ 58].Int( 255, 204,  51, 255 );  // sunglow
  pick[ 59].Int( 255, 204,   0, 255 );  // tangerineYellow
  pick[ 60].Int( 244, 196,  48, 255 );  // saffron
  pick[ 61].Int( 252, 194,   0, 255 );  // goldenPoppy
  pick[ 62].Int( 255, 191,   0, 255 );  // amber
  pick[ 63].Int( 255, 186,   0, 255 );  // selectiveYellow
  pick[ 64].Int( 255, 185, 123, 255 );  // macaroniAndCheese
  pick[ 65].Int( 244, 164,  96, 255 );  // sandyBrown
  pick[ 66].Int( 237, 201, 175, 255 );  // desertSand
  pick[ 67].Int( 230, 190, 138, 255 );  // paleGold
  pick[ 68].Int( 195, 163, 104, 255 );  // brass
  pick[ 69].Int( 169, 143, 100, 255 );  // californiaGold
  pick[ 70].Int( 197, 179,  88, 255 );  // vegasGold
  pick[ 71].Int( 207, 181,  59, 255 );  // oldGold
  pick[ 72].Int( 212, 175,  55, 255 );  // metallicGold
  pick[ 73].Int( 218, 165,  32, 255 );  // goldenrod
  pick[ 74].Int( 203, 161,  53, 255 );  // satinSheenGold
  pick[ 75].Int( 193, 160,   4, 255 );  // buddhaGold
  pick[ 76].Int( 205, 133,  63, 255 );  // peru
  pick[ 77].Int( 183, 110, 121, 255 );  // roseGold
  pick[ 78].Int( 204, 119,  34, 255 );  // ochre
  pick[ 79].Int( 184, 134,  11, 255 );  // darkGoldenrod
  pick[ 80].Int( 150, 113,  23, 255 );  // sandyTaupe
  pick[ 81].Int( 153, 101,  21, 255 );  // goldenBrown
  pick[ 82].Int( 115,  74,  18, 255 );  // rawUmber
  pick[ 83].Int( 250, 218, 221, 255 );  // palePink
  pick[ 84].Int( 253, 215, 228, 255 );  // pigPink
  pick[ 85].Int( 255, 192, 203, 255 );  // pink
  pick[ 86].Int( 255, 183, 213, 255 );  // cottonCandy
  pick[ 87].Int( 244, 194, 194, 255 );  // babyPink
  pick[ 88].Int( 255, 182, 193, 255 );  // mediumPink
  pick[ 89].Int( 255, 183, 197, 255 );  // cherryBlossomPink
  pick[ 90].Int( 246, 173, 198, 255 );  // nadashikoPink
  pick[ 91].Int( 246, 166, 201, 255 );  // carnationPink
  pick[ 92].Int( 252, 128, 165, 255 );  // tickleMePink
  pick[ 93].Int( 226,  80, 155, 255 );  // raspberryPink
  pick[ 94].Int( 234,  84, 128, 255 );  // darkPink
  pick[ 95].Int( 251,  96, 127, 255 );  // brinkPink
  pick[ 96].Int( 204, 136, 153, 255 );  // puce
  pick[ 97].Int( 222, 165, 164, 255 );  // pastelPink
  pick[ 98].Int( 255, 153, 153, 255 );  // lightSalmonPink
  pick[ 99].Int( 255, 145, 164, 255 );  // salmonPink
  pick[100].Int( 255, 140, 105, 255 );  // mediumSalmon
  pick[101].Int( 250, 128, 114, 255 );  // salmon
  pick[102].Int( 233, 150, 122, 255 );  // darkSalmon
  pick[103].Int( 248, 131, 121, 255 );  // coralPink
  pick[104].Int( 240, 128, 128, 255 );  // lightCoral
  pick[105].Int( 230, 103,  97, 255 );  // lightCarminePink
  pick[106].Int( 235,  76,  66, 255 );  // carminePink
  pick[107].Int( 239,  48,  36, 255 );  // deepCarminePink
  pick[108].Int( 227,  66,  52, 255 );  // vermilion
  pick[109].Int( 210, 105,  30, 255 );  // lightChocolate
  pick[110].Int( 226, 114,  91, 255 );  // terraCotta
  pick[111].Int( 255,  99,  71, 255 );  // tomato
  pick[112].Int( 255,  64,  64, 255 );  // coralRed
  pick[113].Int( 255,   0,  56, 255 );  // carmineRed
  pick[114].Int( 255,  63,  52, 255 );  // redOrange
  pick[115].Int( 255,  63,   0, 255 );  // electricVermilion
  pick[116].Int( 255,  69,   0, 255 );  // orangeRed
  pick[117].Int( 255,  50,   7, 255 );  // coquelicot
  pick[118].Int( 255,  32,   0, 255 );  // scarlet
  pick[119].Int( 255,   0,   0, 255 );  // rED
  pick[120].Int( 237,  28,  36, 255 );  // pigmentRed
  pick[121].Int( 230,  32,  32, 255 );  // lust
  pick[122].Int( 220,  20,  60, 255 );  // crimson
  pick[123].Int( 224,  17,  95, 255 );  // ruby
  pick[124].Int( 227,  11,  92, 255 );  // raspberry
  pick[125].Int( 227,  37, 107, 255 );  // razzmatazz
  pick[126].Int( 222,  49,  99, 255 );  // cerise
  pick[127].Int( 227,  38,  54, 255 );  // alizarinCrimson
  pick[128].Int( 255,   0,  63, 255 );  // electricCrimson
  pick[129].Int( 255,   3,  62, 255 );  // americanRose
  pick[130].Int( 255,  53,  94, 255 );  // radicalRed
  pick[131].Int( 229,  43,  80, 255 );  // amaranth
  pick[132].Int( 215,   0,  64, 255 );  // richCarmine
  pick[133].Int( 206,  22,  32, 255 );  // fireEngineRed
  pick[134].Int( 201,   0,  22, 255 );  // harvardCrimson
  pick[135].Int( 196,  30,  58, 255 );  // cardinal
  pick[136].Int( 195,  33,  72, 255 );  // brightMaroon
  pick[137].Int( 176,  48,  96, 255 );  // richMaroon
  pick[138].Int( 179,  68, 108, 255 );  // raspberryRose
  pick[139].Int( 145,  95, 109, 255 );  // raspberryGlace
  pick[140].Int( 169,  32,  62, 255 );  // deepCarmine
  pick[141].Int( 190,   0,  50, 255 );  // crimsonGlory
  pick[142].Int( 184,   0,  73, 255 );  // brightTyrianPurple
  pick[143].Int( 175,  64,  53, 255 );  // mediumCarmine
  pick[144].Int( 192,  64,   0, 255 );  // mahogany
  pick[145].Int( 200,   8,  21, 255 );  // venetianRed
  pick[146].Int( 204,  51,  51, 255 );  // persianRed
  pick[147].Int( 203,  65,  84, 255 );  // brickRed
  pick[148].Int( 204,  78,  92, 255 );  // darkTerraCotta
  pick[149].Int( 205,  92,  92, 255 );  // chestnut
  pick[150].Int( 196,  86,  55, 255 );  // fuzzyWuzzyBrown
  pick[151].Int( 185,  78,  72, 255 );  // deepChestnut
  pick[152].Int( 171,  78,  82, 255 );  // redwood
  pick[153].Int( 144,  93,  93, 255 );  // mediumChocolate
  pick[154].Int( 152, 105,  96, 255 );  // darkChestnut
  pick[155].Int( 166, 123,  91, 255 );  // cafeAuLait
  pick[156].Int( 128,  70,  27, 255 );  // russet
  pick[157].Int( 183,  65,  14, 255 );  // rust
  pick[158].Int( 138,  51,  36, 255 );  // burntUmber
  pick[159].Int( 159,  29,  53, 255 );  // vividBurgundy
  pick[160].Int( 165,  42,  42, 255 );  // redBrown
  pick[161].Int( 178,  34,  34, 255 );  // fireBrick
  pick[162].Int( 179,  27,  27, 255 );  // carnelian
  pick[163].Int( 174,  22,  42, 255 );  // upsdellRed
  pick[164].Int( 168,  28,   7, 255 );  // rufous
  pick[165].Int( 159,  56,  29, 255 );  // cognac
  pick[166].Int( 156,  13,   7, 255 );  // teaRed
  pick[167].Int( 146,   0,  10, 255 );  // sangria
  pick[168].Int( 147,  39,  36, 255 );  // vividAuburn
  pick[169].Int( 144,   0,  32, 255 );  // burgundy
  pick[170].Int(  97,  64,  81, 255 );  // mediumTyrianPurple
  pick[171].Int( 150,   0,  24, 255 );  // carmine
  pick[172].Int( 139,   0,   0, 255 );  // darkRed
  pick[173].Int( 128,  24,  24, 255 );  // faluRed
  pick[174].Int( 128,   0,   0, 255 );  // maroon
  pick[175].Int( 123,  54,  54, 255 );  // tuscanRed
  pick[176].Int( 121,  68,  59, 255 );  // bole
  pick[177].Int( 123,  63,   0, 255 );  // chocolate
  pick[178].Int( 103,  76,  71, 255 );  // darkChocolate
  pick[179].Int( 113,  47,  38, 255 );  // auburn
  pick[180].Int( 112,  28,  28, 255 );  // persianPlum
  pick[181].Int( 135,  38,  87, 255 );  // darkRaspberry
  pick[182].Int( 102,   2,  60, 255 );  // tyrianPurple
  pick[183].Int(  89,  39,  32, 255 );  // caputMortuum
  pick[184].Int(  75,  54,  33, 255 );  // coffee
  pick[185].Int( 154, 185, 115, 255 );  // olivineGreen
  pick[186].Int( 107, 142,  35, 255 );  // oliveDrab
  pick[187].Int( 128, 128,   0, 255 );  // oLIVE
  pick[188].Int(  85, 107,  47, 255 );  // darkOliveGreen
  pick[189].Int( 230, 230, 250, 255 );  // lavenderMist
  pick[190].Int( 220, 208, 255, 255 );  // lightMauve
  pick[191].Int( 224, 176, 255, 255 );  // mauve
  pick[192].Int( 216, 191, 216, 255 );  // thistle
  pick[193].Int( 200, 162, 200, 255 );  // lilac
  pick[194].Int( 226, 156, 210, 255 );  // lightOrchid
  pick[195].Int( 221, 160, 221, 255 );  // palePlum
  pick[196].Int( 238, 130, 238, 255 );  // lavenderMagenta
  pick[197].Int(  49,   2,  15, 255 );  // orchid
  pick[198].Int( 223, 115, 255, 255 );  // heliotrope
  pick[199].Int( 221,   0, 255, 255 );  // psychedelicPurple
  pick[200].Int( 191,   0, 255, 255 );  // electricPurple
  pick[201].Int( 175,   0, 255, 255 );  // pontiff
  pick[202].Int( 160,  92, 240, 255 );  // x11Purple
  pick[203].Int( 153,  50, 204, 255 );  // darkOrchid
  pick[204].Int( 139,   0, 204, 255 );  // deepPurple
  pick[205].Int( 125,  38, 205, 255 );  // internetPurple
  pick[206].Int( 138,  43, 226, 255 );  // deepIndigo
  pick[207].Int( 101,  45,  93, 255 );  // purpleHeart
  pick[208].Int( 107,  63, 160, 255 );  // royalPurple
  pick[209].Int( 102,   0, 153, 255 );  // genericPurple
  pick[210].Int(  75,   0, 130, 255 );  // pigmentIndigo
  pick[211].Int(  49,   0,  98, 255 );  // darkIndigo
  pick[212].Int(  62,  47, 132, 255 );  // vulgarPurple
  pick[213].Int(  50,  18, 122, 255 );  // persianIndigo
  pick[214].Int(  18,  10, 143, 255 );  // ultramarine
  pick[215].Int(  28,  57, 187, 255 );  // persianBlue
  pick[216].Int(  63,   0, 255, 255 );  // electricUltramarine
  pick[217].Int(  82,  24, 250, 255 );  // hanPurple
  pick[218].Int(  95,   0, 255, 255 );  // westminster
  pick[219].Int( 102,   0, 255, 255 );  // indigo
  pick[220].Int( 139,   0, 255, 255 );  // violet
  pick[221].Int( 148,   0, 211, 255 );  // pigmentViolet
  pick[222].Int( 131,  89, 163, 255 );  // violetPurple
  pick[223].Int( 181, 126, 220, 255 );  // lavender
  pick[224].Int( 150, 120, 182, 255 );  // purpleMountainMajesty
  pick[225].Int( 136, 120, 195, 255 );  // purpleYam
  pick[226].Int( 147, 112, 219, 255 );  // mediumPurple
  pick[227].Int( 153, 102, 204, 255 );  // amethyst
  pick[228].Int( 153,  85, 187, 255 );  // deepLilac
  pick[229].Int( 183,  85, 211, 255 );  // mediumOrchid
  pick[230].Int( 193,  84, 193, 255 );  // deepFuchsia
  pick[231].Int( 202, 130, 175, 255 );  // operaMauve
  pick[232].Int( 175, 134, 142, 255 );  // mauveTaupe
  pick[233].Int( 197,  75, 140, 255 );  // mulberry
  pick[234].Int( 219, 112, 147, 255 );  // paleRedViolet
  pick[235].Int( 217,  73, 114, 255 );  // cabaret
  pick[236].Int( 202,  44, 146, 255 );  // royalFuchsia
  pick[237].Int( 189,  51, 164, 255 );  // byzantine
  pick[238].Int( 181,  51, 137, 255 );  // fandango
  pick[239].Int( 187,  51, 133, 255 );  // mediumRedViolet
  pick[240].Int( 199,  21, 133, 255 );  // redViolet
  pick[241].Int( 182,  49, 108, 255 );  // hibiscus
  pick[242].Int( 184,   0,  73, 255 );  // brightTyrianPurple
  pick[243].Int(  97,  64,  81, 255 );  // mediumTyrianPurple
  pick[244].Int( 171,  39,  79, 255 );  // amaranthPurple
  pick[245].Int( 170,  55,  90, 255 );  // nightShadz
  pick[246].Int( 171,  52, 114, 255 );  // royalHeath
  pick[247].Int( 165,  11,  94, 255 );  // jazzberryJam
  pick[248].Int( 159,  43, 104, 255 );  // amaranthDeepPurple
  pick[249].Int( 153,   0, 102, 255 );  // redVioletEggplant
  pick[250].Int( 135,  21,  80, 255 );  // disco
  pick[251].Int( 132,  49, 121, 255 );  // plum
  pick[252].Int( 153,  17, 153, 255 );  // violetEggplant
  pick[253].Int( 128,   0, 128, 255 );  // pURPLE
  pick[254].Int( 112,  41,  99, 255 );  // byzantium
  pick[255].Int( 104,  40,  96, 255 );  // palatinatePurple
  pick[256].Int(  90,   0,  74, 255 );  // tokyoPurple
  pick[257].Int( 102,  56,  84, 255 );  // halayaUbe
  pick[258].Int( 120,  24,  74, 255 );  // pansyPurple
  pick[259].Int( 102,   0,  69, 255 );  // nightclub
  pick[260].Int( 102,   2,  60, 255 );  // tyrianPurple
  pick[261].Int( 118,  57,  93, 255 );  // cosmic
  pick[262].Int( 112,  28,  28, 255 );  // persianPlum
  pick[263].Int(  97,  64,  81, 255 );  // eggplant
  pick[264].Int(  93,  57,  84, 255 );  // darkByzantium
  pick[265].Int(  80,  64,  77, 255 );  // purpleTaupe
  pick[266].Int(  77,   1,  53, 255 );  // blackberry
  pick[267].Int( 245, 245, 220, 255 );  // beige
  pick[268].Int( 255, 235, 205, 255 );  // blanchedAlmond
  pick[269].Int( 251, 231, 128, 255 );  // bananaMania
  pick[270].Int( 255, 228, 182, 255 );  // moccasin
  pick[271].Int( 250, 223, 173, 255 );  // peachYellow
  pick[272].Int( 245, 222, 179, 255 );  // wheat
  pick[273].Int( 255, 228, 196, 255 );  // bisque
  pick[274].Int( 255, 222, 173, 255 );  // navajoWhite
  pick[275].Int( 240, 230, 140, 255 );  // lightKhaki
  pick[276].Int( 238, 232, 170, 255 );  // paleGoldenrod
  pick[277].Int( 240, 220, 130, 255 );  // buff
  pick[278].Int( 238, 220, 130, 255 );  // flax
  pick[279].Int( 235, 194, 175, 255 );  // zinnwaldite
  pick[280].Int( 237, 201, 175, 255 );  // desertSand
  pick[281].Int( 205, 184, 145, 255 );  // ecru
  pick[282].Int( 210, 180, 140, 255 );  // tan
  pick[283].Int( 195, 176, 145, 255 );  // khaki
  pick[284].Int( 189, 183, 107, 255 );  // darkKhaki
  pick[285].Int( 188, 143, 143, 255 );  // rosyBrown
  pick[286].Int( 226, 114,  91, 255 );  // terraCotta
  pick[287].Int( 254, 111,  94, 255 );  // bittersweet
  pick[288].Int( 244, 164,  96, 255 );  // sandyBrown
  pick[289].Int( 218, 165,  32, 255 );  // goldenrod
  pick[290].Int( 195, 163, 104, 255 );  // brass
  pick[291].Int( 188, 152,  26, 255 );  // paleTaupe
  pick[292].Int( 200, 138, 101, 255 );  // antiqueBrass
  pick[293].Int( 205, 127,  50, 255 );  // bronze
  pick[294].Int( 204, 119,  34, 255 );  // ochre
  pick[295].Int( 184, 115,  51, 255 );  // copper
  pick[296].Int( 184, 134,  11, 255 );  // darkGoldenrod
  pick[297].Int( 150, 113,  23, 255 );  // sandyTaupe
  pick[298].Int( 145, 129,  81, 255 );  // darkTan
  pick[299].Int( 131, 112,  80, 255 );  // shadow
  pick[300].Int( 146, 111,  91, 255 );  // beaver
  pick[301].Int( 166, 123,  91, 255 );  // frenchBeige
  pick[302].Int( 152, 118,  54, 255 );  // paleBrown
  pick[303].Int( 205, 133,  63, 255 );  // peru
  pick[304].Int( 210, 105,  30, 255 );  // cocoaBean
  pick[305].Int( 205,  87,   0, 255 );  // tenne
  pick[306].Int( 204,  85,   0, 255 );  // burntOrange
  pick[307].Int( 192,  64,   0, 255 );  // mahogany
  pick[308].Int( 204,  78,  92, 255 );  // darkTerraCotta
  pick[309].Int( 205,  92,  92, 255 );  // chestnut
  pick[310].Int( 196,  86,  55, 255 );  // fuzzyWuzzyBrown
  pick[311].Int( 185,  78,  72, 255 );  // deepChestnut
  pick[312].Int( 152, 105,  96, 255 );  // darkChestnut
  pick[313].Int( 175,  89,  67, 255 );  // mediumBrown
  pick[314].Int( 160,  82,  45, 255 );  // sienna
  pick[315].Int( 150,  75,   0, 255 );  // bROWN
  pick[316].Int( 139,  69,  13, 255 );  // saddleBrown
  pick[317].Int( 183,  65,  14, 255 );  // rust
  pick[318].Int( 128,  70,  27, 255 );  // russet
  pick[319].Int( 138,  51,  36, 255 );  // burntUmber
  pick[320].Int( 165,  42,  42, 255 );  // redBrown
  pick[321].Int( 144,   0,  32, 255 );  // burgundy
  pick[322].Int( 128,   0,   0, 255 );  // maroon
  pick[323].Int( 113,  47,  38, 255 );  // auburn
  pick[324].Int( 115,  74,  18, 255 );  // rawUmber
  pick[325].Int( 100,  65,  23, 255 );  // pullmanBrown
  pick[326].Int( 101,  67,  33, 255 );  // darkBrown
  pick[327].Int( 112,  66,  20, 255 );  // sepia
  pick[328].Int( 121,  68,  59, 255 );  // bole
  pick[329].Int( 103,  76,  21, 255 );  // mediumTaupe
  pick[330].Int(  72,  60,  50, 255 );  // taupe
  pick[331].Int(  75,  54,  33, 255 );  // coffee
  pick[332].Int(  83,  75,  79, 255 );  // liver
  pick[333].Int(  61,  43,  31, 255 );  // bistre
  pick[334].Int(  50,  20,  20, 255 );  // sealBrown
  pick[335].Int( 255, 245, 238, 255 );  // seashell
  pick[336].Int( 253, 245, 230, 255 );  // oldLace
  pick[337].Int( 250, 235, 215, 255 );  // antiqueWhite
  pick[338].Int( 255, 235, 205, 255 );  // blanchedAlmond
  pick[339].Int( 247, 231, 206, 255 );  // champagne
  pick[340].Int( 251, 231, 128, 255 );  // bananaMania
  pick[341].Int( 255, 228, 182, 255 );  // moccasin
  pick[342].Int( 255, 228, 196, 255 );  // bisque
  pick[343].Int( 255, 222, 173, 255 );  // navajoWhite
  pick[344].Int( 255, 218, 185, 255 );  // peachPuff
  pick[345].Int( 255, 229, 180, 255 );  // peach
  pick[346].Int( 255, 203, 164, 255 );  // deepPeach
  pick[347].Int( 255, 204, 153, 255 );  // peachOrange
  pick[348].Int( 253, 213, 177, 255 );  // brightApricot
  pick[349].Int( 251, 206, 177, 255 );  // apricot
  pick[350].Int( 254, 186, 173, 255 );  // melon
  pick[351].Int( 255, 189, 136, 255 );  // macaroniAndCheese
  pick[352].Int( 244, 164,  96, 255 );  // sandyBrown
  pick[353].Int( 255, 160, 122, 255 );  // lightSalmon
  pick[354].Int( 233, 150, 122, 255 );  // darkSalmon
  pick[355].Int( 255, 140, 105, 255 );  // mediumSalmon
  pick[356].Int( 255, 127,  80, 255 );  // coral
  pick[357].Int( 250, 128, 114, 255 );  // salmon
  pick[358].Int( 248, 131, 121, 255 );  // coralPink
  pick[359].Int( 240, 128, 128, 255 );  // lightCoral
  pick[360].Int( 254, 111,  94, 255 );  // bittersweet
  pick[361].Int( 226, 114,  91, 255 );  // terraCotta
  pick[362].Int( 248, 131, 121, 255 );  // teaRose
  pick[363].Int( 255, 153, 102, 255 );  // vividTangerine
  pick[364].Int( 255, 153, 102, 255 );  // atomicTangerine
  pick[365].Int( 255, 160,   0, 255 );  // orangePeel
  pick[366].Int( 255, 128,   0, 255 );  // orange
  pick[367].Int( 255, 153,  51, 255 );  // deepSaffron
  pick[368].Int( 205, 140,   0, 255 );  // darkOrange
  pick[369].Int( 243, 132,   0, 255 );  // tangerine
  pick[370].Int( 255, 127,   0, 255 );  // oRANGE
  pick[371].Int( 255, 126,   0, 255 );  // automotiveAmber
  pick[372].Int( 152, 118,  54, 255 );  // pumpkin
  pick[373].Int( 255, 104,  31, 255 );  // richOrange
  pick[374].Int( 255, 102,   0, 255 );  // safetyOrange
  pick[375].Int( 255,  97,   3, 255 );  // cadmiumOrange
  pick[376].Int( 236,  88,   0, 255 );  // persimmon
  pick[377].Int( 249,  77,   0, 255 );  // tangelo
  pick[378].Int( 227,  66,  52, 255 );  // vermilion
  pick[379].Int( 255,  63,  52, 255 );  // redOrange
  pick[380].Int( 255,  63,   0, 255 );  // electricVermilion
  pick[381].Int( 255,  69,   0, 255 );  // orangeRed
  pick[382].Int( 255,  79,   0, 255 );  // internationalOrange
  pick[383].Int( 255,  90,  54, 255 );  // portlandOrange
  pick[384].Int( 255,  96,  55, 255 );  // ultraOrange
  pick[385].Int( 255,  99,  71, 255 );  // tomato
  pick[386].Int( 255, 150,   0, 255 );  // deepCarrotOrange
  pick[387].Int( 237, 145,  33, 255 );  // carrotOrange
  pick[388].Int( 255, 153,  51, 255 );  // neonCarrot
  pick[389].Int( 228, 155,  18, 255 );  // gamboge
  pick[390].Int( 204, 119,  34, 255 );  // ochre
  pick[391].Int( 205, 127,  50, 255 );  // bronze
  pick[392].Int( 184, 115,  51, 255 );  // copper
  pick[393].Int( 152, 118,  54, 255 );  // paleBrown
  pick[394].Int( 200, 138, 101, 255 );  // antiqueBrass
  pick[395].Int( 205, 133,  63, 255 );  // peru
  pick[396].Int( 210, 105,  30, 255 );  // cocoaBean
  pick[397].Int( 205,  87,   0, 255 );  // tenne
  pick[398].Int( 204,  85,   0, 255 );  // burntOrange
  pick[399].Int( 192,  64,   0, 255 );  // mahogany
  pick[400].Int( 204,  78,  92, 255 );  // darkTerraCotta
  pick[401].Int( 205,  91,  69, 255 );  // darkCoral
  pick[402].Int( 175,  89,  67, 255 );  // mediumBrown
  pick[403].Int( 160,  82,  45, 255 );  // sienna
  pick[404].Int( 150,  75,   0, 255 );  // brown
  pick[405].Int( 139,  69,  13, 255 );  // saddleBrown
  pick[406].Int( 183,  65,  14, 255 );  // rust
  pick[407].Int( 128,  70,  27, 255 );  // russet
  pick[408].Int( 165,  42,  42, 255 );  // redBrown
  pick[409].Int( 159,  56,  29, 255 );  // cognacBrown
  pick[410].Int( 138,  51,  36, 255 );  // burntUmber
  pick[411].Int( 100,  65,  23, 255 );  // pullmanBrown
  pick[412].Int( 101,  67,  33, 255 );  // darkBrown
  pick[413].Int( 174, 134,  11, 255 );  // mediumTaupe
  pick[414].Int(  72,  60,  50, 255 );  // taupe
  pick[415].Int(  63,   0, 255, 255 );  // electricUltramarine
  pick[416].Int(   0, 102, 204, 255 );  // mediumNavyBlue
  pick[417].Int(   0,   0, 205, 255 );  // mediumBlue
  pick[418].Int(   0,  56, 186, 255 );  // royalAzure
  pick[419].Int(  51,  51, 153, 255 );  // pigmentBlue
  pick[420].Int(  18,  10, 143, 255 );  // ultramarine
  pick[421].Int(   0,   0, 139, 255 );  // darkBlue
  pick[422].Int(   0,   0, 128, 255 );  // nAVYBLUE
  pick[423].Int(  50,  18, 122, 255 );  // persianIndigo
  pick[424].Int(  35,  41, 122, 255 );  // st.Patrick'sBlue
  pick[425].Int(  25,  25, 112, 255 );  // midnightBlue
  pick[426].Int(   8,  37, 103, 255 );  // sapphire
  pick[427].Int( 201, 192, 187, 255 );  // paleSilver
  pick[428].Int( 192, 192, 192, 255 );  // sILVER
  pick[429].Int( 240, 248, 255, 255 );  // aliceBlue
  pick[430].Int( 231, 254, 255, 255 );  // bubbles
  pick[431].Int( 224, 255, 255, 255 );  // lightCyan
  pick[432].Int( 175, 238, 238, 255 );  // paleTurquoise
  pick[433].Int( 150, 222, 205, 255 );  // paleRobinEggBlue
  pick[434].Int( 127, 255, 212, 255 );  // aquamarine
  pick[435].Int( 113, 217, 226, 255 );  // aquamarineBlue
  pick[436].Int( 137, 207, 240, 255 );  // babyBlue
  pick[437].Int( 125, 249, 255, 255 );  // electricBlue
  pick[438].Int(   0, 255, 255, 255 );  // cyan
  pick[439].Int(   0, 255, 239, 255 );  // turquoiseBlue
  pick[440].Int(   8, 232, 222, 255 );  // brightTurquoise
  pick[441].Int(  48, 213, 200, 255 );  // turquoise
  pick[442].Int( 115, 194, 215, 255 );  // mayaBlue
  pick[443].Int(   0, 191, 255, 255 );  // capri
  pick[444].Int(   0, 183, 235, 255 );  // processCyan
  pick[445].Int(   2, 164, 211, 255 );  // brightCerulean
  pick[446].Int( 150, 150, 255, 255 );  // deepTurquoise
  pick[447].Int(   0, 204, 204, 255 );  // robinEggBlue
  pick[448].Int(  72, 209, 204, 255 );  // mediumTurquoise
  pick[449].Int( 102, 205, 170, 255 );  // mediumAquamarine
  pick[450].Int(  32, 178, 170, 255 );  // lightSeaGreen
  pick[451].Int(  95, 158, 109, 255 );  // cadetBlue
  pick[452].Int(  64, 130, 109, 255 );  // viridian
  pick[453].Int(   1, 121, 111, 255 );  // pineGreen
  pick[454].Int(   0, 139, 139, 255 );  // darkCyan
  pick[455].Int(   0, 128, 128, 255 );  // teal
  pick[456].Int(  54, 117, 136, 255 );  // tealBlue
  pick[457].Int(   0, 149, 182, 255 );  // bondiBlue
  pick[458].Int(   8, 146, 208, 255 );  // richElectricBlue
  pick[459].Int(   0, 123, 167, 255 );  // cerulean
  pick[460].Int(  70, 130, 180, 255 );  // steelBlue
  pick[461].Int(   3,  80, 150, 255 );  // mediumElectricBlue
  pick[462].Int(  83, 104, 120, 255 );  // darkElectricBlue
  pick[463].Int(   0,  73,  83, 255 );  // midnightGreen
  pick[464].Int( 250, 218, 221, 255 );  // palePink
  pick[465].Int( 253, 215, 228, 255 );  // pigPink
  pick[466].Int( 255, 192, 203, 255 );  // pINK
  pick[467].Int( 255, 183, 213, 255 );  // cottonCandy
  pick[468].Int( 244, 194, 194, 255 );  // babyPink
  pick[469].Int( 255, 182, 193, 255 );  // mediumPink
  pick[470].Int( 255, 183, 197, 255 );  // cherryBlossomPink
  pick[471].Int( 246, 173, 198, 255 );  // nadashikoPink
  pick[472].Int( 246, 166, 201, 255 );  // carnationPink
  pick[473].Int( 251, 174, 210, 255 );  // lavenderPink
  pick[474].Int( 251, 160, 227, 255 );  // lavenderRose
  pick[475].Int( 249, 132, 229, 255 );  // lightFuchsiaPink
  pick[476].Int( 247, 127, 190, 255 );  // persianPink
  pick[477].Int( 252, 128, 165, 255 );  // tickleMePink
  pick[478].Int( 240, 145, 169, 255 );  // mauvelous
  pick[479].Int( 241, 156, 187, 255 );  // amaranthPink
  pick[480].Int( 221, 190, 195, 255 );  // paleAmaranthPink
  pick[481].Int( 222, 165, 164, 255 );  // pastelPink
  pick[482].Int( 204, 136, 153, 255 );  // puce
  pick[483].Int( 219, 112, 147, 255 );  // paleVioletRed
  pick[484].Int( 230, 103,  97, 255 );  // lightCarminePink
  pick[485].Int( 240, 128, 128, 255 );  // lightCoral
  pick[486].Int( 248, 131, 121, 255 );  // coralPink
  pick[487].Int( 250, 128, 114, 255 );  // salmon
  pick[488].Int( 255, 153, 153, 255 );  // lightSalmonPink
  pick[489].Int( 255, 145, 164, 255 );  // salmonPink
  pick[490].Int( 255, 153, 102, 255 );  // pinkOrange
  pick[491].Int( 255, 127,  80, 255 );  // coral
  pick[492].Int( 249,  90,  97, 255 );  // carnation
  pick[493].Int( 235,  76,  66, 255 );  // carminePink
  pick[494].Int( 239,  48,  36, 255 );  // deepCarminePink
  pick[495].Int( 253,  91, 153, 255 );  // wildWatermelon
  pick[496].Int( 251,  96, 127, 255 );  // brinkPink
  pick[497].Int( 234,  84, 128, 255 );  // darkPink
  pick[498].Int( 219,  80, 127, 255 );  // cranberry
  pick[499].Int( 246,  74, 138, 255 );  // frenchRose
  pick[500].Int( 247,  70, 138, 255 );  // violetRed
  pick[501].Int( 255, 105, 180, 255 );  // hotPink
  pick[502].Int( 246,  83, 166, 255 );  // brilliantRose
  pick[503].Int( 226,  80, 155, 255 );  // raspberryPink
  pick[504].Int( 255,  51, 153, 255 );  // wildStrawberry
  pick[505].Int( 255,   0, 127, 255 );  // brightPink
  pick[506].Int( 255,  20, 147, 255 );  // deepPink
  pick[507].Int( 236,  59, 131, 255 );  // cerisePink
  pick[508].Int( 218,  49,  99, 255 );  // cerise
  pick[509].Int( 184,   0,  73, 255 );  // tyrianPink
  pick[510].Int( 218,  29, 129, 255 );  // vividCerise
  pick[511].Int( 205,  38, 130, 255 );  // amaranthCerise
  pick[512].Int( 218,  50, 135, 255 );  // deepCerise
  pick[513].Int( 244,   0, 161, 255 );  // hollywoodCerise
  pick[514].Int( 254,  40, 162, 255 );  // persianRose
  pick[515].Int( 252,  15, 192, 255 );  // shockingPink
  pick[516].Int( 255,  51, 204, 255 );  // razzleDazzleRose
  pick[517].Int( 255, 119, 255, 255 );  // fuchsiaPink
  pick[518].Int( 255, 111, 255, 255 );  // ultraPink
  pick[519].Int( 255, 102, 255, 255 );  // pinkFlamingo
  pick[520].Int( 255,   0, 255, 255 );  // magenta
  pick[521].Int( 255,   0, 204, 255 );  // hotMagenta
  pick[522].Int( 237,  60, 202, 255 );  // amaranthMagenta
  pick[523].Int( 207, 113, 175, 255 );  // skyMagenta
  pick[524].Int( 181,  84, 137, 255 );  // fandango2
  pick[525].Int(  53,  53,  53, 255 );  // ansigrey
  pick[526].Int( 205,   0,   0, 255 );  // ansired
  pick[527].Int(   0, 205,   0, 255 );  // ansigreen
  pick[528].Int( 205, 205,   0, 255 );  // ansiamber
  pick[529].Int(   0,   0, 238, 255 );  // ansiblue
  pick[530].Int( 205,   0, 205, 255 );  // ansimagenta
  pick[531].Int( 000, 205, 205, 255 );  // ansicyan
  pick[532].Int( 127, 127, 127, 255 );  // ansiboldgrey
  pick[533].Int( 254,   0,   0, 255 );  // ansiboldred
  pick[534].Int(   0, 254,   0, 255 );  // ansiboldgreen
  pick[535].Int(  92,  92, 255, 255 );  // ansiboldblue
  pick[536].Int( 205, 000, 205, 255 );  // ansiboldmagenta
  pick[537].Int( 254, 254, 254, 255 );  // ansiwhite
  pick[538].Int(   0, 205, 205, 255 );  // ansibackcyan
  pick[539].Int( 205,   0,   0, 255 );  // ansibackred
  pick[540].Int(  53,  53,  53, 255 );  // ansibackgrey
  pick[541].Int(  85, 107,  47, 255 );  // ansimutedgreen
  pick[542].Int( 217, 164,  32, 255 );  // ansimutedyellow
  pick[543].Int( 174,  71,  71, 255 );  // ansimutedred
  pick[544].Int( 111, 153, 180, 255 );  // ansimutedblue
  pick[545].Int( 167, 161,  94, 255 );  // ansimutedgreen2
  pick[546].Int( 206,  92,   0, 255 );  // ansialtred
  pick[547].Int( 173, 127, 168, 255 );  // ansialtmagenta
  pick[548].Int( 255, 193,  53, 255 );  // ansialtyellow
  pick[549].Int(  84, 133, 164, 255 );  // ansialtblue
  pick[550].Int( 182, 184, 185, 255 );  // ansialtwhite
  pick[551].Int( 184, 139,  16, 255 );  // ansialtbrown
  pick[552].Int( 102, 136,   0, 255 );  // ansialtgreen
  pick[553].Int(  85,  85,  68, 255 );  // ansialtgrey
  pick[554].Int(  38,  39,  41, 255 );  // ansialtblack
  pick[555].Int( 128, 120,  85, 255 );  // ansialtcyan
  pick[556].Int( 130, 200, 130, 255 );  // ansimonogreen
  pick[557].Int( 253, 255,  84, 255 );  // ansimonoamber
  pick[558].Int( 188, 143,  40, 255 );  // ansimsxamber
  pick[559].Int( 124, 113, 218, 255 );  // c64blue (emulated)
  pick[560].Int(   5, 255, 254, 255 );  // c64cyan (emulated)
  pick[561].Int(  59,  51,  28, 255 );  // pullmanGreen
  pick[562].Int( 194,  94,   3, 255 );  // omahaOrange
  pick[563].Int( 150,  99,   5, 255 );  // imitationGold
  pick[564].Int( 184, 171, 158, 255 );  // dupontGray
  pick[565].Int( 156,  31,  46, 255 );  // vermillion
  pick[566].Int(  89,  38,  38, 255 );  // mineralRed
  pick[567].Int(   0, 158, 130, 255 );  // glacierGreen
  pick[568].Int( 237, 181,  18, 255 );  // wfeYellow
  pick[569].Int(   0, 120, 171, 255 );  // bigSkyBlue
  pick[570].Int(  54,  54,  48, 255 );  // gnGray
  pick[571].Int(  49, 198, 222, 255 );  // c64cyan??
  pick[572].Int(   0, 255,   0, 127 );  // greenGlass
  pick[573].Int(   0,   0, 255, 127 );  // greenGlass??
  pick[574].Int(   0, 255,   0, 127 );  // blueGlass
  pick[575].Int( 0,0,0,255 );  // rgb000
  pick[576].Int( 0,0,15,255 );  // rgb001
  pick[577].Int( 0,0,31,255 );  // rgb002
  pick[578].Int( 0,0,47,255 );  // rgb003
  pick[579].Int( 0,0,63,255 );  // rgb004
  pick[580].Int( 0,0,79,255 );  // rgb005
  pick[581].Int( 0,0,95,255 );  // rgb006
  pick[582].Int( 0,0,111,255 );  // rgb007
  pick[583].Int( 0,0,127,255 );  // rgb008
  pick[584].Int( 0,0,143,255 );  // rgb009
  pick[585].Int( 0,0,159,255 );  // rgb00A
  pick[586].Int( 0,0,175,255 );  // rgb00B
  pick[587].Int( 0,0,191,255 );  // rgb00C
  pick[588].Int( 0,0,207,255 );  // rgb00D
  pick[589].Int( 0,0,223,255 );  // rgb00E
  pick[590].Int( 0,0,239,255 );  // rgb00F
  pick[591].Int( 0,15,0,255 );  // rgb010
  pick[592].Int( 0,15,15,255 );  // rgb011
  pick[593].Int( 0,15,31,255 );  // rgb012
  pick[594].Int( 0,15,47,255 );  // rgb013
  pick[595].Int( 0,15,63,255 );  // rgb014
  pick[596].Int( 0,15,79,255 );  // rgb015
  pick[597].Int( 0,15,95,255 );  // rgb016
  pick[598].Int( 0,15,111,255 );  // rgb017
  pick[599].Int( 0,15,127,255 );  // rgb018
  pick[600].Int( 0,15,143,255 );  // rgb019
  pick[601].Int( 0,15,159,255 );  // rgb01A
  pick[602].Int( 0,15,175,255 );  // rgb01B
  pick[603].Int( 0,15,191,255 );  // rgb01C
  pick[604].Int( 0,15,207,255 );  // rgb01D
  pick[605].Int( 0,15,223,255 );  // rgb01E
  pick[606].Int( 0,15,239,255 );  // rgb01F
  pick[607].Int( 0,31,0,255 );  // rgb020
  pick[608].Int( 0,31,15,255 );  // rgb021
  pick[609].Int( 0,31,31,255 );  // rgb022
  pick[610].Int( 0,31,47,255 );  // rgb023
  pick[611].Int( 0,31,63,255 );  // rgb024
  pick[612].Int( 0,31,79,255 );  // rgb025
  pick[613].Int( 0,31,95,255 );  // rgb026
  pick[614].Int( 0,31,111,255 );  // rgb027
  pick[615].Int( 0,31,127,255 );  // rgb028
  pick[616].Int( 0,31,143,255 );  // rgb029
  pick[617].Int( 0,31,159,255 );  // rgb02A
  pick[618].Int( 0,31,175,255 );  // rgb02B
  pick[619].Int( 0,31,191,255 );  // rgb02C
  pick[620].Int( 0,31,207,255 );  // rgb02D
  pick[621].Int( 0,31,223,255 );  // rgb02E
  pick[622].Int( 0,31,239,255 );  // rgb02F
  pick[623].Int( 0,47,0,255 );  // rgb030
  pick[624].Int( 0,47,15,255 );  // rgb031
  pick[625].Int( 0,47,31,255 );  // rgb032
  pick[626].Int( 0,47,47,255 );  // rgb033
  pick[627].Int( 0,47,63,255 );  // rgb034
  pick[628].Int( 0,47,79,255 );  // rgb035
  pick[629].Int( 0,47,95,255 );  // rgb036
  pick[630].Int( 0,47,111,255 );  // rgb037
  pick[631].Int( 0,47,127,255 );  // rgb038
  pick[632].Int( 0,47,143,255 );  // rgb039
  pick[633].Int( 0,47,159,255 );  // rgb03A
  pick[634].Int( 0,47,175,255 );  // rgb03B
  pick[635].Int( 0,47,191,255 );  // rgb03C
  pick[636].Int( 0,47,207,255 );  // rgb03D
  pick[637].Int( 0,47,223,255 );  // rgb03E
  pick[638].Int( 0,47,239,255 );  // rgb03F
  pick[639].Int( 0,63,0,255 );  // rgb040
  pick[640].Int( 0,63,15,255 );  // rgb041
  pick[641].Int( 0,63,31,255 );  // rgb042
  pick[642].Int( 0,63,47,255 );  // rgb043
  pick[643].Int( 0,63,63,255 );  // rgb044
  pick[644].Int( 0,63,79,255 );  // rgb045
  pick[645].Int( 0,63,95,255 );  // rgb046
  pick[646].Int( 0,63,111,255 );  // rgb047
  pick[647].Int( 0,63,127,255 );  // rgb048
  pick[648].Int( 0,63,143,255 );  // rgb049
  pick[649].Int( 0,63,159,255 );  // rgb04A
  pick[650].Int( 0,63,175,255 );  // rgb04B
  pick[651].Int( 0,63,191,255 );  // rgb04C
  pick[652].Int( 0,63,207,255 );  // rgb04D
  pick[653].Int( 0,63,223,255 );  // rgb04E
  pick[654].Int( 0,63,239,255 );  // rgb04F
  pick[655].Int( 0,79,0,255 );  // rgb050
  pick[656].Int( 0,79,15,255 );  // rgb051
  pick[657].Int( 0,79,31,255 );  // rgb052
  pick[658].Int( 0,79,47,255 );  // rgb053
  pick[659].Int( 0,79,63,255 );  // rgb054
  pick[660].Int( 0,79,79,255 );  // rgb055
  pick[661].Int( 0,79,95,255 );  // rgb056
  pick[662].Int( 0,79,111,255 );  // rgb057
  pick[663].Int( 0,79,127,255 );  // rgb058
  pick[664].Int( 0,79,143,255 );  // rgb059
  pick[665].Int( 0,79,159,255 );  // rgb05A
  pick[666].Int( 0,79,175,255 );  // rgb05B
  pick[667].Int( 0,79,191,255 );  // rgb05C
  pick[668].Int( 0,79,207,255 );  // rgb05D
  pick[669].Int( 0,79,223,255 );  // rgb05E
  pick[670].Int( 0,79,239,255 );  // rgb05F
  pick[671].Int( 0,95,0,255 );  // rgb060
  pick[672].Int( 0,95,15,255 );  // rgb061
  pick[673].Int( 0,95,31,255 );  // rgb062
  pick[674].Int( 0,95,47,255 );  // rgb063
  pick[675].Int( 0,95,63,255 );  // rgb064
  pick[676].Int( 0,95,79,255 );  // rgb065
  pick[677].Int( 0,95,95,255 );  // rgb066
  pick[678].Int( 0,95,111,255 );  // rgb067
  pick[679].Int( 0,95,127,255 );  // rgb068
  pick[680].Int( 0,95,143,255 );  // rgb069
  pick[681].Int( 0,95,159,255 );  // rgb06A
  pick[682].Int( 0,95,175,255 );  // rgb06B
  pick[683].Int( 0,95,191,255 );  // rgb06C
  pick[684].Int( 0,95,207,255 );  // rgb06D
  pick[685].Int( 0,95,223,255 );  // rgb06E
  pick[686].Int( 0,95,239,255 );  // rgb06F
  pick[687].Int( 0,111,0,255 );  // rgb070
  pick[688].Int( 0,111,15,255 );  // rgb071
  pick[689].Int( 0,111,31,255 );  // rgb072
  pick[690].Int( 0,111,47,255 );  // rgb073
  pick[691].Int( 0,111,63,255 );  // rgb074
  pick[692].Int( 0,111,79,255 );  // rgb075
  pick[693].Int( 0,111,95,255 );  // rgb076
  pick[694].Int( 0,111,111,255 );  // rgb077
  pick[695].Int( 0,111,127,255 );  // rgb078
  pick[696].Int( 0,111,143,255 );  // rgb079
  pick[697].Int( 0,111,159,255 );  // rgb07A
  pick[698].Int( 0,111,175,255 );  // rgb07B
  pick[699].Int( 0,111,191,255 );  // rgb07C
  pick[700].Int( 0,111,207,255 );  // rgb07D
  pick[701].Int( 0,111,223,255 );  // rgb07E
  pick[702].Int( 0,111,239,255 );  // rgb07F
  pick[703].Int( 0,127,0,255 );  // rgb080
  pick[704].Int( 0,127,15,255 );  // rgb081
  pick[705].Int( 0,127,31,255 );  // rgb082
  pick[706].Int( 0,127,47,255 );  // rgb083
  pick[707].Int( 0,127,63,255 );  // rgb084
  pick[708].Int( 0,127,79,255 );  // rgb085
  pick[709].Int( 0,127,95,255 );  // rgb086
  pick[710].Int( 0,127,111,255 );  // rgb087
  pick[711].Int( 0,127,127,255 );  // rgb088
  pick[712].Int( 0,127,143,255 );  // rgb089
  pick[713].Int( 0,127,159,255 );  // rgb08A
  pick[714].Int( 0,127,175,255 );  // rgb08B
  pick[715].Int( 0,127,191,255 );  // rgb08C
  pick[716].Int( 0,127,207,255 );  // rgb08D
  pick[717].Int( 0,127,223,255 );  // rgb08E
  pick[718].Int( 0,127,239,255 );  // rgb08F
  pick[719].Int( 0,143,0,255 );  // rgb090
  pick[720].Int( 0,143,15,255 );  // rgb091
  pick[721].Int( 0,143,31,255 );  // rgb092
  pick[722].Int( 0,143,47,255 );  // rgb093
  pick[723].Int( 0,143,63,255 );  // rgb094
  pick[724].Int( 0,143,79,255 );  // rgb095
  pick[725].Int( 0,143,95,255 );  // rgb096
  pick[726].Int( 0,143,111,255 );  // rgb097
  pick[727].Int( 0,143,127,255 );  // rgb098
  pick[728].Int( 0,143,143,255 );  // rgb099
  pick[729].Int( 0,143,159,255 );  // rgb09A
  pick[730].Int( 0,143,175,255 );  // rgb09B
  pick[731].Int( 0,143,191,255 );  // rgb09C
  pick[732].Int( 0,143,207,255 );  // rgb09D
  pick[733].Int( 0,143,223,255 );  // rgb09E
  pick[734].Int( 0,143,239,255 );  // rgb09F
  pick[735].Int( 0,159,0,255 );  // rgb0A0
  pick[736].Int( 0,159,15,255 );  // rgb0A1
  pick[737].Int( 0,159,31,255 );  // rgb0A2
  pick[738].Int( 0,159,47,255 );  // rgb0A3
  pick[739].Int( 0,159,63,255 );  // rgb0A4
  pick[740].Int( 0,159,79,255 );  // rgb0A5
  pick[741].Int( 0,159,95,255 );  // rgb0A6
  pick[742].Int( 0,159,111,255 );  // rgb0A7
  pick[743].Int( 0,159,127,255 );  // rgb0A8
  pick[744].Int( 0,159,143,255 );  // rgb0A9
  pick[745].Int( 0,159,159,255 );  // rgb0AA
  pick[746].Int( 0,159,175,255 );  // rgb0AB
  pick[747].Int( 0,159,191,255 );  // rgb0AC
  pick[748].Int( 0,159,207,255 );  // rgb0AD
  pick[749].Int( 0,159,223,255 );  // rgb0AE
  pick[750].Int( 0,159,239,255 );  // rgb0AF
  pick[751].Int( 0,175,0,255 );  // rgb0B0
  pick[752].Int( 0,175,15,255 );  // rgb0B1
  pick[753].Int( 0,175,31,255 );  // rgb0B2
  pick[754].Int( 0,175,47,255 );  // rgb0B3
  pick[755].Int( 0,175,63,255 );  // rgb0B4
  pick[756].Int( 0,175,79,255 );  // rgb0B5
  pick[757].Int( 0,175,95,255 );  // rgb0B6
  pick[758].Int( 0,175,111,255 );  // rgb0B7
  pick[759].Int( 0,175,127,255 );  // rgb0B8
  pick[760].Int( 0,175,143,255 );  // rgb0B9
  pick[761].Int( 0,175,159,255 );  // rgb0BA
  pick[762].Int( 0,175,175,255 );  // rgb0BB
  pick[763].Int( 0,175,191,255 );  // rgb0BC
  pick[764].Int( 0,175,207,255 );  // rgb0BD
  pick[765].Int( 0,175,223,255 );  // rgb0BE
  pick[766].Int( 0,175,239,255 );  // rgb0BF
  pick[767].Int( 0,191,0,255 );  // rgb0C0
  pick[768].Int( 0,191,15,255 );  // rgb0C1
  pick[769].Int( 0,191,31,255 );  // rgb0C2
  pick[770].Int( 0,191,47,255 );  // rgb0C3
  pick[771].Int( 0,191,63,255 );  // rgb0C4
  pick[772].Int( 0,191,79,255 );  // rgb0C5
  pick[773].Int( 0,191,95,255 );  // rgb0C6
  pick[774].Int( 0,191,111,255 );  // rgb0C7
  pick[775].Int( 0,191,127,255 );  // rgb0C8
  pick[776].Int( 0,191,143,255 );  // rgb0C9
  pick[777].Int( 0,191,159,255 );  // rgb0CA
  pick[778].Int( 0,191,175,255 );  // rgb0CB
  pick[779].Int( 0,191,191,255 );  // rgb0CC
  pick[780].Int( 0,191,207,255 );  // rgb0CD
  pick[781].Int( 0,191,223,255 );  // rgb0CE
  pick[782].Int( 0,191,239,255 );  // rgb0CF
  pick[783].Int( 0,207,0,255 );  // rgb0D0
  pick[784].Int( 0,207,15,255 );  // rgb0D1
  pick[785].Int( 0,207,31,255 );  // rgb0D2
  pick[786].Int( 0,207,47,255 );  // rgb0D3
  pick[787].Int( 0,207,63,255 );  // rgb0D4
  pick[788].Int( 0,207,79,255 );  // rgb0D5
  pick[789].Int( 0,207,95,255 );  // rgb0D6
  pick[790].Int( 0,207,111,255 );  // rgb0D7
  pick[791].Int( 0,207,127,255 );  // rgb0D8
  pick[792].Int( 0,207,143,255 );  // rgb0D9
  pick[793].Int( 0,207,159,255 );  // rgb0DA
  pick[794].Int( 0,207,175,255 );  // rgb0DB
  pick[795].Int( 0,207,191,255 );  // rgb0DC
  pick[796].Int( 0,207,207,255 );  // rgb0DD
  pick[797].Int( 0,207,223,255 );  // rgb0DE
  pick[798].Int( 0,207,239,255 );  // rgb0DF
  pick[799].Int( 0,223,0,255 );  // rgb0E0
  pick[800].Int( 0,223,15,255 );  // rgb0E1
  pick[801].Int( 0,223,31,255 );  // rgb0E2
  pick[802].Int( 0,223,47,255 );  // rgb0E3
  pick[803].Int( 0,223,63,255 );  // rgb0E4
  pick[804].Int( 0,223,79,255 );  // rgb0E5
  pick[805].Int( 0,223,95,255 );  // rgb0E6
  pick[806].Int( 0,223,111,255 );  // rgb0E7
  pick[807].Int( 0,223,127,255 );  // rgb0E8
  pick[808].Int( 0,223,143,255 );  // rgb0E9
  pick[809].Int( 0,223,159,255 );  // rgb0EA
  pick[810].Int( 0,223,175,255 );  // rgb0EB
  pick[811].Int( 0,223,191,255 );  // rgb0EC
  pick[812].Int( 0,223,207,255 );  // rgb0ED
  pick[813].Int( 0,223,223,255 );  // rgb0EE
  pick[814].Int( 0,223,239,255 );  // rgb0EF
  pick[815].Int( 0,239,0,255 );  // rgb0F0
  pick[816].Int( 0,239,15,255 );  // rgb0F1
  pick[817].Int( 0,239,31,255 );  // rgb0F2
  pick[818].Int( 0,239,47,255 );  // rgb0F3
  pick[819].Int( 0,239,63,255 );  // rgb0F4
  pick[820].Int( 0,239,79,255 );  // rgb0F5
  pick[821].Int( 0,239,95,255 );  // rgb0F6
  pick[822].Int( 0,239,111,255 );  // rgb0F7
  pick[823].Int( 0,239,127,255 );  // rgb0F8
  pick[824].Int( 0,239,143,255 );  // rgb0F9
  pick[825].Int( 0,239,159,255 );  // rgb0FA
  pick[826].Int( 0,239,175,255 );  // rgb0FB
  pick[827].Int( 0,239,191,255 );  // rgb0FC
  pick[828].Int( 0,239,207,255 );  // rgb0FD
  pick[829].Int( 0,239,223,255 );  // rgb0FE
  pick[830].Int( 0,239,239,255 );  // rgb0FF
  pick[831].Int( 15,0,0,255 );  // rgb100
  pick[832].Int( 15,0,15,255 );  // rgb101
  pick[833].Int( 15,0,31,255 );  // rgb102
  pick[834].Int( 15,0,47,255 );  // rgb103
  pick[835].Int( 15,0,63,255 );  // rgb104
  pick[836].Int( 15,0,79,255 );  // rgb105
  pick[837].Int( 15,0,95,255 );  // rgb106
  pick[838].Int( 15,0,111,255 );  // rgb107
  pick[839].Int( 15,0,127,255 );  // rgb108
  pick[840].Int( 15,0,143,255 );  // rgb109
  pick[841].Int( 15,0,159,255 );  // rgb10A
  pick[842].Int( 15,0,175,255 );  // rgb10B
  pick[843].Int( 15,0,191,255 );  // rgb10C
  pick[844].Int( 15,0,207,255 );  // rgb10D
  pick[845].Int( 15,0,223,255 );  // rgb10E
  pick[846].Int( 15,0,239,255 );  // rgb10F
  pick[847].Int( 15,15,0,255 );  // rgb110
  pick[848].Int( 15,15,15,255 );  // rgb111
  pick[849].Int( 15,15,31,255 );  // rgb112
  pick[850].Int( 15,15,47,255 );  // rgb113
  pick[851].Int( 15,15,63,255 );  // rgb114
  pick[852].Int( 15,15,79,255 );  // rgb115
  pick[853].Int( 15,15,95,255 );  // rgb116
  pick[854].Int( 15,15,111,255 );  // rgb117
  pick[855].Int( 15,15,127,255 );  // rgb118
  pick[856].Int( 15,15,143,255 );  // rgb119
  pick[857].Int( 15,15,159,255 );  // rgb11A
  pick[858].Int( 15,15,175,255 );  // rgb11B
  pick[859].Int( 15,15,191,255 );  // rgb11C
  pick[860].Int( 15,15,207,255 );  // rgb11D
  pick[861].Int( 15,15,223,255 );  // rgb11E
  pick[862].Int( 15,15,239,255 );  // rgb11F
  pick[863].Int( 15,31,0,255 );  // rgb120
  pick[864].Int( 15,31,15,255 );  // rgb121
  pick[865].Int( 15,31,31,255 );  // rgb122
  pick[866].Int( 15,31,47,255 );  // rgb123
  pick[867].Int( 15,31,63,255 );  // rgb124
  pick[868].Int( 15,31,79,255 );  // rgb125
  pick[869].Int( 15,31,95,255 );  // rgb126
  pick[870].Int( 15,31,111,255 );  // rgb127
  pick[871].Int( 15,31,127,255 );  // rgb128
  pick[872].Int( 15,31,143,255 );  // rgb129
  pick[873].Int( 15,31,159,255 );  // rgb12A
  pick[874].Int( 15,31,175,255 );  // rgb12B
  pick[875].Int( 15,31,191,255 );  // rgb12C
  pick[876].Int( 15,31,207,255 );  // rgb12D
  pick[877].Int( 15,31,223,255 );  // rgb12E
  pick[878].Int( 15,31,239,255 );  // rgb12F
  pick[879].Int( 15,47,0,255 );  // rgb130
  pick[880].Int( 15,47,15,255 );  // rgb131
  pick[881].Int( 15,47,31,255 );  // rgb132
  pick[882].Int( 15,47,47,255 );  // rgb133
  pick[883].Int( 15,47,63,255 );  // rgb134
  pick[884].Int( 15,47,79,255 );  // rgb135
  pick[885].Int( 15,47,95,255 );  // rgb136
  pick[886].Int( 15,47,111,255 );  // rgb137
  pick[887].Int( 15,47,127,255 );  // rgb138
  pick[888].Int( 15,47,143,255 );  // rgb139
  pick[889].Int( 15,47,159,255 );  // rgb13A
  pick[890].Int( 15,47,175,255 );  // rgb13B
  pick[891].Int( 15,47,191,255 );  // rgb13C
  pick[892].Int( 15,47,207,255 );  // rgb13D
  pick[893].Int( 15,47,223,255 );  // rgb13E
  pick[894].Int( 15,47,239,255 );  // rgb13F
  pick[895].Int( 15,63,0,255 );  // rgb140
  pick[896].Int( 15,63,15,255 );  // rgb141
  pick[897].Int( 15,63,31,255 );  // rgb142
  pick[898].Int( 15,63,47,255 );  // rgb143
  pick[899].Int( 15,63,63,255 );  // rgb144
  pick[900].Int( 15,63,79,255 );  // rgb145
  pick[901].Int( 15,63,95,255 );  // rgb146
  pick[902].Int( 15,63,111,255 );  // rgb147
  pick[903].Int( 15,63,127,255 );  // rgb148
  pick[904].Int( 15,63,143,255 );  // rgb149
  pick[905].Int( 15,63,159,255 );  // rgb14A
  pick[906].Int( 15,63,175,255 );  // rgb14B
  pick[907].Int( 15,63,191,255 );  // rgb14C
  pick[908].Int( 15,63,207,255 );  // rgb14D
  pick[909].Int( 15,63,223,255 );  // rgb14E
  pick[910].Int( 15,63,239,255 );  // rgb14F
  pick[911].Int( 15,79,0,255 );  // rgb150
  pick[912].Int( 15,79,15,255 );  // rgb151
  pick[913].Int( 15,79,31,255 );  // rgb152
  pick[914].Int( 15,79,47,255 );  // rgb153
  pick[915].Int( 15,79,63,255 );  // rgb154
  pick[916].Int( 15,79,79,255 );  // rgb155
  pick[917].Int( 15,79,95,255 );  // rgb156
  pick[918].Int( 15,79,111,255 );  // rgb157
  pick[919].Int( 15,79,127,255 );  // rgb158
  pick[920].Int( 15,79,143,255 );  // rgb159
  pick[921].Int( 15,79,159,255 );  // rgb15A
  pick[922].Int( 15,79,175,255 );  // rgb15B
  pick[923].Int( 15,79,191,255 );  // rgb15C
  pick[924].Int( 15,79,207,255 );  // rgb15D
  pick[925].Int( 15,79,223,255 );  // rgb15E
  pick[926].Int( 15,79,239,255 );  // rgb15F
  pick[927].Int( 15,95,0,255 );  // rgb160
  pick[928].Int( 15,95,15,255 );  // rgb161
  pick[929].Int( 15,95,31,255 );  // rgb162
  pick[930].Int( 15,95,47,255 );  // rgb163
  pick[931].Int( 15,95,63,255 );  // rgb164
  pick[932].Int( 15,95,79,255 );  // rgb165
  pick[933].Int( 15,95,95,255 );  // rgb166
  pick[934].Int( 15,95,111,255 );  // rgb167
  pick[935].Int( 15,95,127,255 );  // rgb168
  pick[936].Int( 15,95,143,255 );  // rgb169
  pick[937].Int( 15,95,159,255 );  // rgb16A
  pick[938].Int( 15,95,175,255 );  // rgb16B
  pick[939].Int( 15,95,191,255 );  // rgb16C
  pick[940].Int( 15,95,207,255 );  // rgb16D
  pick[941].Int( 15,95,223,255 );  // rgb16E
  pick[942].Int( 15,95,239,255 );  // rgb16F
  pick[943].Int( 15,111,0,255 );  // rgb170
  pick[944].Int( 15,111,15,255 );  // rgb171
  pick[945].Int( 15,111,31,255 );  // rgb172
  pick[946].Int( 15,111,47,255 );  // rgb173
  pick[947].Int( 15,111,63,255 );  // rgb174
  pick[948].Int( 15,111,79,255 );  // rgb175
  pick[949].Int( 15,111,95,255 );  // rgb176
  pick[950].Int( 15,111,111,255 );  // rgb177
  pick[951].Int( 15,111,127,255 );  // rgb178
  pick[952].Int( 15,111,143,255 );  // rgb179
  pick[953].Int( 15,111,159,255 );  // rgb17A
  pick[954].Int( 15,111,175,255 );  // rgb17B
  pick[955].Int( 15,111,191,255 );  // rgb17C
  pick[956].Int( 15,111,207,255 );  // rgb17D
  pick[957].Int( 15,111,223,255 );  // rgb17E
  pick[958].Int( 15,111,239,255 );  // rgb17F
  pick[959].Int( 15,127,0,255 );  // rgb180
  pick[960].Int( 15,127,15,255 );  // rgb181
  pick[961].Int( 15,127,31,255 );  // rgb182
  pick[962].Int( 15,127,47,255 );  // rgb183
  pick[963].Int( 15,127,63,255 );  // rgb184
  pick[964].Int( 15,127,79,255 );  // rgb185
  pick[965].Int( 15,127,95,255 );  // rgb186
  pick[966].Int( 15,127,111,255 );  // rgb187
  pick[967].Int( 15,127,127,255 );  // rgb188
  pick[968].Int( 15,127,143,255 );  // rgb189
  pick[969].Int( 15,127,159,255 );  // rgb18A
  pick[970].Int( 15,127,175,255 );  // rgb18B
  pick[971].Int( 15,127,191,255 );  // rgb18C
  pick[972].Int( 15,127,207,255 );  // rgb18D
  pick[973].Int( 15,127,223,255 );  // rgb18E
  pick[974].Int( 15,127,239,255 );  // rgb18F
  pick[975].Int( 15,143,0,255 );  // rgb190
  pick[976].Int( 15,143,15,255 );  // rgb191
  pick[977].Int( 15,143,31,255 );  // rgb192
  pick[978].Int( 15,143,47,255 );  // rgb193
  pick[979].Int( 15,143,63,255 );  // rgb194
  pick[980].Int( 15,143,79,255 );  // rgb195
  pick[981].Int( 15,143,95,255 );  // rgb196
  pick[982].Int( 15,143,111,255 );  // rgb197
  pick[983].Int( 15,143,127,255 );  // rgb198
  pick[984].Int( 15,143,143,255 );  // rgb199
  pick[985].Int( 15,143,159,255 );  // rgb19A
  pick[986].Int( 15,143,175,255 );  // rgb19B
  pick[987].Int( 15,143,191,255 );  // rgb19C
  pick[988].Int( 15,143,207,255 );  // rgb19D
  pick[989].Int( 15,143,223,255 );  // rgb19E
  pick[990].Int( 15,143,239,255 );  // rgb19F
  pick[991].Int( 15,159,0,255 );  // rgb1A0
  pick[992].Int( 15,159,15,255 );  // rgb1A1
  pick[993].Int( 15,159,31,255 );  // rgb1A2
  pick[994].Int( 15,159,47,255 );  // rgb1A3
  pick[995].Int( 15,159,63,255 );  // rgb1A4
  pick[996].Int( 15,159,79,255 );  // rgb1A5
  pick[997].Int( 15,159,95,255 );  // rgb1A6
  pick[998].Int( 15,159,111,255 );  // rgb1A7
  pick[999].Int( 15,159,127,255 );  // rgb1A8
  pick[1000].Int( 15,159,143,255 );  // rgb1A9
  pick[1001].Int( 15,159,159,255 );  // rgb1AA
  pick[1002].Int( 15,159,175,255 );  // rgb1AB
  pick[1003].Int( 15,159,191,255 );  // rgb1AC
  pick[1004].Int( 15,159,207,255 );  // rgb1AD
  pick[1005].Int( 15,159,223,255 );  // rgb1AE
  pick[1006].Int( 15,159,239,255 );  // rgb1AF
  pick[1007].Int( 15,175,0,255 );  // rgb1B0
  pick[1008].Int( 15,175,15,255 );  // rgb1B1
  pick[1009].Int( 15,175,31,255 );  // rgb1B2
  pick[1010].Int( 15,175,47,255 );  // rgb1B3
  pick[1011].Int( 15,175,63,255 );  // rgb1B4
  pick[1012].Int( 15,175,79,255 );  // rgb1B5
  pick[1013].Int( 15,175,95,255 );  // rgb1B6
  pick[1014].Int( 15,175,111,255 );  // rgb1B7
  pick[1015].Int( 15,175,127,255 );  // rgb1B8
  pick[1016].Int( 15,175,143,255 );  // rgb1B9
  pick[1017].Int( 15,175,159,255 );  // rgb1BA
  pick[1018].Int( 15,175,175,255 );  // rgb1BB
  pick[1019].Int( 15,175,191,255 );  // rgb1BC
  pick[1020].Int( 15,175,207,255 );  // rgb1BD
  pick[1021].Int( 15,175,223,255 );  // rgb1BE
  pick[1022].Int( 15,175,239,255 );  // rgb1BF
  pick[1023].Int( 15,191,0,255 );  // rgb1C0
  pick[1024].Int( 15,191,15,255 );  // rgb1C1
  pick[1025].Int( 15,191,31,255 );  // rgb1C2
  pick[1026].Int( 15,191,47,255 );  // rgb1C3
  pick[1027].Int( 15,191,63,255 );  // rgb1C4
  pick[1028].Int( 15,191,79,255 );  // rgb1C5
  pick[1029].Int( 15,191,95,255 );  // rgb1C6
  pick[1030].Int( 15,191,111,255 );  // rgb1C7
  pick[1031].Int( 15,191,127,255 );  // rgb1C8
  pick[1032].Int( 15,191,143,255 );  // rgb1C9
  pick[1033].Int( 15,191,159,255 );  // rgb1CA
  pick[1034].Int( 15,191,175,255 );  // rgb1CB
  pick[1035].Int( 15,191,191,255 );  // rgb1CC
  pick[1036].Int( 15,191,207,255 );  // rgb1CD
  pick[1037].Int( 15,191,223,255 );  // rgb1CE
  pick[1038].Int( 15,191,239,255 );  // rgb1CF
  pick[1039].Int( 15,207,0,255 );  // rgb1D0
  pick[1040].Int( 15,207,15,255 );  // rgb1D1
  pick[1041].Int( 15,207,31,255 );  // rgb1D2
  pick[1042].Int( 15,207,47,255 );  // rgb1D3
  pick[1043].Int( 15,207,63,255 );  // rgb1D4
  pick[1044].Int( 15,207,79,255 );  // rgb1D5
  pick[1045].Int( 15,207,95,255 );  // rgb1D6
  pick[1046].Int( 15,207,111,255 );  // rgb1D7
  pick[1047].Int( 15,207,127,255 );  // rgb1D8
  pick[1048].Int( 15,207,143,255 );  // rgb1D9
  pick[1049].Int( 15,207,159,255 );  // rgb1DA
  pick[1050].Int( 15,207,175,255 );  // rgb1DB
  pick[1051].Int( 15,207,191,255 );  // rgb1DC
  pick[1052].Int( 15,207,207,255 );  // rgb1DD
  pick[1053].Int( 15,207,223,255 );  // rgb1DE
  pick[1054].Int( 15,207,239,255 );  // rgb1DF
  pick[1055].Int( 15,223,0,255 );  // rgb1E0
  pick[1056].Int( 15,223,15,255 );  // rgb1E1
  pick[1057].Int( 15,223,31,255 );  // rgb1E2
  pick[1058].Int( 15,223,47,255 );  // rgb1E3
  pick[1059].Int( 15,223,63,255 );  // rgb1E4
  pick[1060].Int( 15,223,79,255 );  // rgb1E5
  pick[1061].Int( 15,223,95,255 );  // rgb1E6
  pick[1062].Int( 15,223,111,255 );  // rgb1E7
  pick[1063].Int( 15,223,127,255 );  // rgb1E8
  pick[1064].Int( 15,223,143,255 );  // rgb1E9
  pick[1065].Int( 15,223,159,255 );  // rgb1EA
  pick[1066].Int( 15,223,175,255 );  // rgb1EB
  pick[1067].Int( 15,223,191,255 );  // rgb1EC
  pick[1068].Int( 15,223,207,255 );  // rgb1ED
  pick[1069].Int( 15,223,223,255 );  // rgb1EE
  pick[1070].Int( 15,223,239,255 );  // rgb1EF
  pick[1071].Int( 15,239,0,255 );  // rgb1F0
  pick[1072].Int( 15,239,15,255 );  // rgb1F1
  pick[1073].Int( 15,239,31,255 );  // rgb1F2
  pick[1074].Int( 15,239,47,255 );  // rgb1F3
  pick[1075].Int( 15,239,63,255 );  // rgb1F4
  pick[1076].Int( 15,239,79,255 );  // rgb1F5
  pick[1077].Int( 15,239,95,255 );  // rgb1F6
  pick[1078].Int( 15,239,111,255 );  // rgb1F7
  pick[1079].Int( 15,239,127,255 );  // rgb1F8
  pick[1080].Int( 15,239,143,255 );  // rgb1F9
  pick[1081].Int( 15,239,159,255 );  // rgb1FA
  pick[1082].Int( 15,239,175,255 );  // rgb1FB
  pick[1083].Int( 15,239,191,255 );  // rgb1FC
  pick[1084].Int( 15,239,207,255 );  // rgb1FD
  pick[1085].Int( 15,239,223,255 );  // rgb1FE
  pick[1086].Int( 15,239,239,255 );  // rgb1FF
  pick[1087].Int( 31,0,0,255 );  // rgb200
  pick[1088].Int( 31,0,15,255 );  // rgb201
  pick[1089].Int( 31,0,31,255 );  // rgb202
  pick[1090].Int( 31,0,47,255 );  // rgb203
  pick[1091].Int( 31,0,63,255 );  // rgb204
  pick[1092].Int( 31,0,79,255 );  // rgb205
  pick[1093].Int( 31,0,95,255 );  // rgb206
  pick[1094].Int( 31,0,111,255 );  // rgb207
  pick[1095].Int( 31,0,127,255 );  // rgb208
  pick[1096].Int( 31,0,143,255 );  // rgb209
  pick[1097].Int( 31,0,159,255 );  // rgb20A
  pick[1098].Int( 31,0,175,255 );  // rgb20B
  pick[1099].Int( 31,0,191,255 );  // rgb20C
  pick[1100].Int( 31,0,207,255 );  // rgb20D
  pick[1101].Int( 31,0,223,255 );  // rgb20E
  pick[1102].Int( 31,0,239,255 );  // rgb20F
  pick[1103].Int( 31,15,0,255 );  // rgb210
  pick[1104].Int( 31,15,15,255 );  // rgb211
  pick[1105].Int( 31,15,31,255 );  // rgb212
  pick[1106].Int( 31,15,47,255 );  // rgb213
  pick[1107].Int( 31,15,63,255 );  // rgb214
  pick[1108].Int( 31,15,79,255 );  // rgb215
  pick[1109].Int( 31,15,95,255 );  // rgb216
  pick[1110].Int( 31,15,111,255 );  // rgb217
  pick[1111].Int( 31,15,127,255 );  // rgb218
  pick[1112].Int( 31,15,143,255 );  // rgb219
  pick[1113].Int( 31,15,159,255 );  // rgb21A
  pick[1114].Int( 31,15,175,255 );  // rgb21B
  pick[1115].Int( 31,15,191,255 );  // rgb21C
  pick[1116].Int( 31,15,207,255 );  // rgb21D
  pick[1117].Int( 31,15,223,255 );  // rgb21E
  pick[1118].Int( 31,15,239,255 );  // rgb21F
  pick[1119].Int( 31,31,0,255 );  // rgb220
  pick[1120].Int( 31,31,15,255 );  // rgb221
  pick[1121].Int( 31,31,31,255 );  // rgb222
  pick[1122].Int( 31,31,47,255 );  // rgb223
  pick[1123].Int( 31,31,63,255 );  // rgb224
  pick[1124].Int( 31,31,79,255 );  // rgb225
  pick[1125].Int( 31,31,95,255 );  // rgb226
  pick[1126].Int( 31,31,111,255 );  // rgb227
  pick[1127].Int( 31,31,127,255 );  // rgb228
  pick[1128].Int( 31,31,143,255 );  // rgb229
  pick[1129].Int( 31,31,159,255 );  // rgb22A
  pick[1130].Int( 31,31,175,255 );  // rgb22B
  pick[1131].Int( 31,31,191,255 );  // rgb22C
  pick[1132].Int( 31,31,207,255 );  // rgb22D
  pick[1133].Int( 31,31,223,255 );  // rgb22E
  pick[1134].Int( 31,31,239,255 );  // rgb22F
  pick[1135].Int( 31,47,0,255 );  // rgb230
  pick[1136].Int( 31,47,15,255 );  // rgb231
  pick[1137].Int( 31,47,31,255 );  // rgb232
  pick[1138].Int( 31,47,47,255 );  // rgb233
  pick[1139].Int( 31,47,63,255 );  // rgb234
  pick[1140].Int( 31,47,79,255 );  // rgb235
  pick[1141].Int( 31,47,95,255 );  // rgb236
  pick[1142].Int( 31,47,111,255 );  // rgb237
  pick[1143].Int( 31,47,127,255 );  // rgb238
  pick[1144].Int( 31,47,143,255 );  // rgb239
  pick[1145].Int( 31,47,159,255 );  // rgb23A
  pick[1146].Int( 31,47,175,255 );  // rgb23B
  pick[1147].Int( 31,47,191,255 );  // rgb23C
  pick[1148].Int( 31,47,207,255 );  // rgb23D
  pick[1149].Int( 31,47,223,255 );  // rgb23E
  pick[1150].Int( 31,47,239,255 );  // rgb23F
  pick[1151].Int( 31,63,0,255 );  // rgb240
  pick[1152].Int( 31,63,15,255 );  // rgb241
  pick[1153].Int( 31,63,31,255 );  // rgb242
  pick[1154].Int( 31,63,47,255 );  // rgb243
  pick[1155].Int( 31,63,63,255 );  // rgb244
  pick[1156].Int( 31,63,79,255 );  // rgb245
  pick[1157].Int( 31,63,95,255 );  // rgb246
  pick[1158].Int( 31,63,111,255 );  // rgb247
  pick[1159].Int( 31,63,127,255 );  // rgb248
  pick[1160].Int( 31,63,143,255 );  // rgb249
  pick[1161].Int( 31,63,159,255 );  // rgb24A
  pick[1162].Int( 31,63,175,255 );  // rgb24B
  pick[1163].Int( 31,63,191,255 );  // rgb24C
  pick[1164].Int( 31,63,207,255 );  // rgb24D
  pick[1165].Int( 31,63,223,255 );  // rgb24E
  pick[1166].Int( 31,63,239,255 );  // rgb24F
  pick[1167].Int( 31,79,0,255 );  // rgb250
  pick[1168].Int( 31,79,15,255 );  // rgb251
  pick[1169].Int( 31,79,31,255 );  // rgb252
  pick[1170].Int( 31,79,47,255 );  // rgb253
  pick[1171].Int( 31,79,63,255 );  // rgb254
  pick[1172].Int( 31,79,79,255 );  // rgb255
  pick[1173].Int( 31,79,95,255 );  // rgb256
  pick[1174].Int( 31,79,111,255 );  // rgb257
  pick[1175].Int( 31,79,127,255 );  // rgb258
  pick[1176].Int( 31,79,143,255 );  // rgb259
  pick[1177].Int( 31,79,159,255 );  // rgb25A
  pick[1178].Int( 31,79,175,255 );  // rgb25B
  pick[1179].Int( 31,79,191,255 );  // rgb25C
  pick[1180].Int( 31,79,207,255 );  // rgb25D
  pick[1181].Int( 31,79,223,255 );  // rgb25E
  pick[1182].Int( 31,79,239,255 );  // rgb25F
  pick[1183].Int( 31,95,0,255 );  // rgb260
  pick[1184].Int( 31,95,15,255 );  // rgb261
  pick[1185].Int( 31,95,31,255 );  // rgb262
  pick[1186].Int( 31,95,47,255 );  // rgb263
  pick[1187].Int( 31,95,63,255 );  // rgb264
  pick[1188].Int( 31,95,79,255 );  // rgb265
  pick[1189].Int( 31,95,95,255 );  // rgb266
  pick[1190].Int( 31,95,111,255 );  // rgb267
  pick[1191].Int( 31,95,127,255 );  // rgb268
  pick[1192].Int( 31,95,143,255 );  // rgb269
  pick[1193].Int( 31,95,159,255 );  // rgb26A
  pick[1194].Int( 31,95,175,255 );  // rgb26B
  pick[1195].Int( 31,95,191,255 );  // rgb26C
  pick[1196].Int( 31,95,207,255 );  // rgb26D
  pick[1197].Int( 31,95,223,255 );  // rgb26E
  pick[1198].Int( 31,95,239,255 );  // rgb26F
  pick[1199].Int( 31,111,0,255 );  // rgb270
  pick[1200].Int( 31,111,15,255 );  // rgb271
  pick[1201].Int( 31,111,31,255 );  // rgb272
  pick[1202].Int( 31,111,47,255 );  // rgb273
  pick[1203].Int( 31,111,63,255 );  // rgb274
  pick[1204].Int( 31,111,79,255 );  // rgb275
  pick[1205].Int( 31,111,95,255 );  // rgb276
  pick[1206].Int( 31,111,111,255 );  // rgb277
  pick[1207].Int( 31,111,127,255 );  // rgb278
  pick[1208].Int( 31,111,143,255 );  // rgb279
  pick[1209].Int( 31,111,159,255 );  // rgb27A
  pick[1210].Int( 31,111,175,255 );  // rgb27B
  pick[1211].Int( 31,111,191,255 );  // rgb27C
  pick[1212].Int( 31,111,207,255 );  // rgb27D
  pick[1213].Int( 31,111,223,255 );  // rgb27E
  pick[1214].Int( 31,111,239,255 );  // rgb27F
  pick[1215].Int( 31,127,0,255 );  // rgb280
  pick[1216].Int( 31,127,15,255 );  // rgb281
  pick[1217].Int( 31,127,31,255 );  // rgb282
  pick[1218].Int( 31,127,47,255 );  // rgb283
  pick[1219].Int( 31,127,63,255 );  // rgb284
  pick[1220].Int( 31,127,79,255 );  // rgb285
  pick[1221].Int( 31,127,95,255 );  // rgb286
  pick[1222].Int( 31,127,111,255 );  // rgb287
  pick[1223].Int( 31,127,127,255 );  // rgb288
  pick[1224].Int( 31,127,143,255 );  // rgb289
  pick[1225].Int( 31,127,159,255 );  // rgb28A
  pick[1226].Int( 31,127,175,255 );  // rgb28B
  pick[1227].Int( 31,127,191,255 );  // rgb28C
  pick[1228].Int( 31,127,207,255 );  // rgb28D
  pick[1229].Int( 31,127,223,255 );  // rgb28E
  pick[1230].Int( 31,127,239,255 );  // rgb28F
  pick[1231].Int( 31,143,0,255 );  // rgb290
  pick[1232].Int( 31,143,15,255 );  // rgb291
  pick[1233].Int( 31,143,31,255 );  // rgb292
  pick[1234].Int( 31,143,47,255 );  // rgb293
  pick[1235].Int( 31,143,63,255 );  // rgb294
  pick[1236].Int( 31,143,79,255 );  // rgb295
  pick[1237].Int( 31,143,95,255 );  // rgb296
  pick[1238].Int( 31,143,111,255 );  // rgb297
  pick[1239].Int( 31,143,127,255 );  // rgb298
  pick[1240].Int( 31,143,143,255 );  // rgb299
  pick[1241].Int( 31,143,159,255 );  // rgb29A
  pick[1242].Int( 31,143,175,255 );  // rgb29B
  pick[1243].Int( 31,143,191,255 );  // rgb29C
  pick[1244].Int( 31,143,207,255 );  // rgb29D
  pick[1245].Int( 31,143,223,255 );  // rgb29E
  pick[1246].Int( 31,143,239,255 );  // rgb29F
  pick[1247].Int( 31,159,0,255 );  // rgb2A0
  pick[1248].Int( 31,159,15,255 );  // rgb2A1
  pick[1249].Int( 31,159,31,255 );  // rgb2A2
  pick[1250].Int( 31,159,47,255 );  // rgb2A3
  pick[1251].Int( 31,159,63,255 );  // rgb2A4
  pick[1252].Int( 31,159,79,255 );  // rgb2A5
  pick[1253].Int( 31,159,95,255 );  // rgb2A6
  pick[1254].Int( 31,159,111,255 );  // rgb2A7
  pick[1255].Int( 31,159,127,255 );  // rgb2A8
  pick[1256].Int( 31,159,143,255 );  // rgb2A9
  pick[1257].Int( 31,159,159,255 );  // rgb2AA
  pick[1258].Int( 31,159,175,255 );  // rgb2AB
  pick[1259].Int( 31,159,191,255 );  // rgb2AC
  pick[1260].Int( 31,159,207,255 );  // rgb2AD
  pick[1261].Int( 31,159,223,255 );  // rgb2AE
  pick[1262].Int( 31,159,239,255 );  // rgb2AF
  pick[1263].Int( 31,175,0,255 );  // rgb2B0
  pick[1264].Int( 31,175,15,255 );  // rgb2B1
  pick[1265].Int( 31,175,31,255 );  // rgb2B2
  pick[1266].Int( 31,175,47,255 );  // rgb2B3
  pick[1267].Int( 31,175,63,255 );  // rgb2B4
  pick[1268].Int( 31,175,79,255 );  // rgb2B5
  pick[1269].Int( 31,175,95,255 );  // rgb2B6
  pick[1270].Int( 31,175,111,255 );  // rgb2B7
  pick[1271].Int( 31,175,127,255 );  // rgb2B8
  pick[1272].Int( 31,175,143,255 );  // rgb2B9
  pick[1273].Int( 31,175,159,255 );  // rgb2BA
  pick[1274].Int( 31,175,175,255 );  // rgb2BB
  pick[1275].Int( 31,175,191,255 );  // rgb2BC
  pick[1276].Int( 31,175,207,255 );  // rgb2BD
  pick[1277].Int( 31,175,223,255 );  // rgb2BE
  pick[1278].Int( 31,175,239,255 );  // rgb2BF
  pick[1279].Int( 31,191,0,255 );  // rgb2C0
  pick[1280].Int( 31,191,15,255 );  // rgb2C1
  pick[1281].Int( 31,191,31,255 );  // rgb2C2
  pick[1282].Int( 31,191,47,255 );  // rgb2C3
  pick[1283].Int( 31,191,63,255 );  // rgb2C4
  pick[1284].Int( 31,191,79,255 );  // rgb2C5
  pick[1285].Int( 31,191,95,255 );  // rgb2C6
  pick[1286].Int( 31,191,111,255 );  // rgb2C7
  pick[1287].Int( 31,191,127,255 );  // rgb2C8
  pick[1288].Int( 31,191,143,255 );  // rgb2C9
  pick[1289].Int( 31,191,159,255 );  // rgb2CA
  pick[1290].Int( 31,191,175,255 );  // rgb2CB
  pick[1291].Int( 31,191,191,255 );  // rgb2CC
  pick[1292].Int( 31,191,207,255 );  // rgb2CD
  pick[1293].Int( 31,191,223,255 );  // rgb2CE
  pick[1294].Int( 31,191,239,255 );  // rgb2CF
  pick[1295].Int( 31,207,0,255 );  // rgb2D0
  pick[1296].Int( 31,207,15,255 );  // rgb2D1
  pick[1297].Int( 31,207,31,255 );  // rgb2D2
  pick[1298].Int( 31,207,47,255 );  // rgb2D3
  pick[1299].Int( 31,207,63,255 );  // rgb2D4
  pick[1300].Int( 31,207,79,255 );  // rgb2D5
  pick[1301].Int( 31,207,95,255 );  // rgb2D6
  pick[1302].Int( 31,207,111,255 );  // rgb2D7
  pick[1303].Int( 31,207,127,255 );  // rgb2D8
  pick[1304].Int( 31,207,143,255 );  // rgb2D9
  pick[1305].Int( 31,207,159,255 );  // rgb2DA
  pick[1306].Int( 31,207,175,255 );  // rgb2DB
  pick[1307].Int( 31,207,191,255 );  // rgb2DC
  pick[1308].Int( 31,207,207,255 );  // rgb2DD
  pick[1309].Int( 31,207,223,255 );  // rgb2DE
  pick[1310].Int( 31,207,239,255 );  // rgb2DF
  pick[1311].Int( 31,223,0,255 );  // rgb2E0
  pick[1312].Int( 31,223,15,255 );  // rgb2E1
  pick[1313].Int( 31,223,31,255 );  // rgb2E2
  pick[1314].Int( 31,223,47,255 );  // rgb2E3
  pick[1315].Int( 31,223,63,255 );  // rgb2E4
  pick[1316].Int( 31,223,79,255 );  // rgb2E5
  pick[1317].Int( 31,223,95,255 );  // rgb2E6
  pick[1318].Int( 31,223,111,255 );  // rgb2E7
  pick[1319].Int( 31,223,127,255 );  // rgb2E8
  pick[1320].Int( 31,223,143,255 );  // rgb2E9
  pick[1321].Int( 31,223,159,255 );  // rgb2EA
  pick[1322].Int( 31,223,175,255 );  // rgb2EB
  pick[1323].Int( 31,223,191,255 );  // rgb2EC
  pick[1324].Int( 31,223,207,255 );  // rgb2ED
  pick[1325].Int( 31,223,223,255 );  // rgb2EE
  pick[1326].Int( 31,223,239,255 );  // rgb2EF
  pick[1327].Int( 31,239,0,255 );  // rgb2F0
  pick[1328].Int( 31,239,15,255 );  // rgb2F1
  pick[1329].Int( 31,239,31,255 );  // rgb2F2
  pick[1330].Int( 31,239,47,255 );  // rgb2F3
  pick[1331].Int( 31,239,63,255 );  // rgb2F4
  pick[1332].Int( 31,239,79,255 );  // rgb2F5
  pick[1333].Int( 31,239,95,255 );  // rgb2F6
  pick[1334].Int( 31,239,111,255 );  // rgb2F7
  pick[1335].Int( 31,239,127,255 );  // rgb2F8
  pick[1336].Int( 31,239,143,255 );  // rgb2F9
  pick[1337].Int( 31,239,159,255 );  // rgb2FA
  pick[1338].Int( 31,239,175,255 );  // rgb2FB
  pick[1339].Int( 31,239,191,255 );  // rgb2FC
  pick[1340].Int( 31,239,207,255 );  // rgb2FD
  pick[1341].Int( 31,239,223,255 );  // rgb2FE
  pick[1342].Int( 31,239,239,255 );  // rgb2FF
  pick[1343].Int( 47,0,0,255 );  // rgb300
  pick[1344].Int( 47,0,15,255 );  // rgb301
  pick[1345].Int( 47,0,31,255 );  // rgb302
  pick[1346].Int( 47,0,47,255 );  // rgb303
  pick[1347].Int( 47,0,63,255 );  // rgb304
  pick[1348].Int( 47,0,79,255 );  // rgb305
  pick[1349].Int( 47,0,95,255 );  // rgb306
  pick[1350].Int( 47,0,111,255 );  // rgb307
  pick[1351].Int( 47,0,127,255 );  // rgb308
  pick[1352].Int( 47,0,143,255 );  // rgb309
  pick[1353].Int( 47,0,159,255 );  // rgb30A
  pick[1354].Int( 47,0,175,255 );  // rgb30B
  pick[1355].Int( 47,0,191,255 );  // rgb30C
  pick[1356].Int( 47,0,207,255 );  // rgb30D
  pick[1357].Int( 47,0,223,255 );  // rgb30E
  pick[1358].Int( 47,0,239,255 );  // rgb30F
  pick[1359].Int( 47,15,0,255 );  // rgb310
  pick[1360].Int( 47,15,15,255 );  // rgb311
  pick[1361].Int( 47,15,31,255 );  // rgb312
  pick[1362].Int( 47,15,47,255 );  // rgb313
  pick[1363].Int( 47,15,63,255 );  // rgb314
  pick[1364].Int( 47,15,79,255 );  // rgb315
  pick[1365].Int( 47,15,95,255 );  // rgb316
  pick[1366].Int( 47,15,111,255 );  // rgb317
  pick[1367].Int( 47,15,127,255 );  // rgb318
  pick[1368].Int( 47,15,143,255 );  // rgb319
  pick[1369].Int( 47,15,159,255 );  // rgb31A
  pick[1370].Int( 47,15,175,255 );  // rgb31B
  pick[1371].Int( 47,15,191,255 );  // rgb31C
  pick[1372].Int( 47,15,207,255 );  // rgb31D
  pick[1373].Int( 47,15,223,255 );  // rgb31E
  pick[1374].Int( 47,15,239,255 );  // rgb31F
  pick[1375].Int( 47,31,0,255 );  // rgb320
  pick[1376].Int( 47,31,15,255 );  // rgb321
  pick[1377].Int( 47,31,31,255 );  // rgb322
  pick[1378].Int( 47,31,47,255 );  // rgb323
  pick[1379].Int( 47,31,63,255 );  // rgb324
  pick[1380].Int( 47,31,79,255 );  // rgb325
  pick[1381].Int( 47,31,95,255 );  // rgb326
  pick[1382].Int( 47,31,111,255 );  // rgb327
  pick[1383].Int( 47,31,127,255 );  // rgb328
  pick[1384].Int( 47,31,143,255 );  // rgb329
  pick[1385].Int( 47,31,159,255 );  // rgb32A
  pick[1386].Int( 47,31,175,255 );  // rgb32B
  pick[1387].Int( 47,31,191,255 );  // rgb32C
  pick[1388].Int( 47,31,207,255 );  // rgb32D
  pick[1389].Int( 47,31,223,255 );  // rgb32E
  pick[1390].Int( 47,31,239,255 );  // rgb32F
  pick[1391].Int( 47,47,0,255 );  // rgb330
  pick[1392].Int( 47,47,15,255 );  // rgb331
  pick[1393].Int( 47,47,31,255 );  // rgb332
  pick[1394].Int( 47,47,47,255 );  // rgb333
  pick[1395].Int( 47,47,63,255 );  // rgb334
  pick[1396].Int( 47,47,79,255 );  // rgb335
  pick[1397].Int( 47,47,95,255 );  // rgb336
  pick[1398].Int( 47,47,111,255 );  // rgb337
  pick[1399].Int( 47,47,127,255 );  // rgb338
  pick[1400].Int( 47,47,143,255 );  // rgb339
  pick[1401].Int( 47,47,159,255 );  // rgb33A
  pick[1402].Int( 47,47,175,255 );  // rgb33B
  pick[1403].Int( 47,47,191,255 );  // rgb33C
  pick[1404].Int( 47,47,207,255 );  // rgb33D
  pick[1405].Int( 47,47,223,255 );  // rgb33E
  pick[1406].Int( 47,47,239,255 );  // rgb33F
  pick[1407].Int( 47,63,0,255 );  // rgb340
  pick[1408].Int( 47,63,15,255 );  // rgb341
  pick[1409].Int( 47,63,31,255 );  // rgb342
  pick[1410].Int( 47,63,47,255 );  // rgb343
  pick[1411].Int( 47,63,63,255 );  // rgb344
  pick[1412].Int( 47,63,79,255 );  // rgb345
  pick[1413].Int( 47,63,95,255 );  // rgb346
  pick[1414].Int( 47,63,111,255 );  // rgb347
  pick[1415].Int( 47,63,127,255 );  // rgb348
  pick[1416].Int( 47,63,143,255 );  // rgb349
  pick[1417].Int( 47,63,159,255 );  // rgb34A
  pick[1418].Int( 47,63,175,255 );  // rgb34B
  pick[1419].Int( 47,63,191,255 );  // rgb34C
  pick[1420].Int( 47,63,207,255 );  // rgb34D
  pick[1421].Int( 47,63,223,255 );  // rgb34E
  pick[1422].Int( 47,63,239,255 );  // rgb34F
  pick[1423].Int( 47,79,0,255 );  // rgb350
  pick[1424].Int( 47,79,15,255 );  // rgb351
  pick[1425].Int( 47,79,31,255 );  // rgb352
  pick[1426].Int( 47,79,47,255 );  // rgb353
  pick[1427].Int( 47,79,63,255 );  // rgb354
  pick[1428].Int( 47,79,79,255 );  // rgb355
  pick[1429].Int( 47,79,95,255 );  // rgb356
  pick[1430].Int( 47,79,111,255 );  // rgb357
  pick[1431].Int( 47,79,127,255 );  // rgb358
  pick[1432].Int( 47,79,143,255 );  // rgb359
  pick[1433].Int( 47,79,159,255 );  // rgb35A
  pick[1434].Int( 47,79,175,255 );  // rgb35B
  pick[1435].Int( 47,79,191,255 );  // rgb35C
  pick[1436].Int( 47,79,207,255 );  // rgb35D
  pick[1437].Int( 47,79,223,255 );  // rgb35E
  pick[1438].Int( 47,79,239,255 );  // rgb35F
  pick[1439].Int( 47,95,0,255 );  // rgb360
  pick[1440].Int( 47,95,15,255 );  // rgb361
  pick[1441].Int( 47,95,31,255 );  // rgb362
  pick[1442].Int( 47,95,47,255 );  // rgb363
  pick[1443].Int( 47,95,63,255 );  // rgb364
  pick[1444].Int( 47,95,79,255 );  // rgb365
  pick[1445].Int( 47,95,95,255 );  // rgb366
  pick[1446].Int( 47,95,111,255 );  // rgb367
  pick[1447].Int( 47,95,127,255 );  // rgb368
  pick[1448].Int( 47,95,143,255 );  // rgb369
  pick[1449].Int( 47,95,159,255 );  // rgb36A
  pick[1450].Int( 47,95,175,255 );  // rgb36B
  pick[1451].Int( 47,95,191,255 );  // rgb36C
  pick[1452].Int( 47,95,207,255 );  // rgb36D
  pick[1453].Int( 47,95,223,255 );  // rgb36E
  pick[1454].Int( 47,95,239,255 );  // rgb36F
  pick[1455].Int( 47,111,0,255 );  // rgb370
  pick[1456].Int( 47,111,15,255 );  // rgb371
  pick[1457].Int( 47,111,31,255 );  // rgb372
  pick[1458].Int( 47,111,47,255 );  // rgb373
  pick[1459].Int( 47,111,63,255 );  // rgb374
  pick[1460].Int( 47,111,79,255 );  // rgb375
  pick[1461].Int( 47,111,95,255 );  // rgb376
  pick[1462].Int( 47,111,111,255 );  // rgb377
  pick[1463].Int( 47,111,127,255 );  // rgb378
  pick[1464].Int( 47,111,143,255 );  // rgb379
  pick[1465].Int( 47,111,159,255 );  // rgb37A
  pick[1466].Int( 47,111,175,255 );  // rgb37B
  pick[1467].Int( 47,111,191,255 );  // rgb37C
  pick[1468].Int( 47,111,207,255 );  // rgb37D
  pick[1469].Int( 47,111,223,255 );  // rgb37E
  pick[1470].Int( 47,111,239,255 );  // rgb37F
  pick[1471].Int( 47,127,0,255 );  // rgb380
  pick[1472].Int( 47,127,15,255 );  // rgb381
  pick[1473].Int( 47,127,31,255 );  // rgb382
  pick[1474].Int( 47,127,47,255 );  // rgb383
  pick[1475].Int( 47,127,63,255 );  // rgb384
  pick[1476].Int( 47,127,79,255 );  // rgb385
  pick[1477].Int( 47,127,95,255 );  // rgb386
  pick[1478].Int( 47,127,111,255 );  // rgb387
  pick[1479].Int( 47,127,127,255 );  // rgb388
  pick[1480].Int( 47,127,143,255 );  // rgb389
  pick[1481].Int( 47,127,159,255 );  // rgb38A
  pick[1482].Int( 47,127,175,255 );  // rgb38B
  pick[1483].Int( 47,127,191,255 );  // rgb38C
  pick[1484].Int( 47,127,207,255 );  // rgb38D
  pick[1485].Int( 47,127,223,255 );  // rgb38E
  pick[1486].Int( 47,127,239,255 );  // rgb38F
  pick[1487].Int( 47,143,0,255 );  // rgb390
  pick[1488].Int( 47,143,15,255 );  // rgb391
  pick[1489].Int( 47,143,31,255 );  // rgb392
  pick[1490].Int( 47,143,47,255 );  // rgb393
  pick[1491].Int( 47,143,63,255 );  // rgb394
  pick[1492].Int( 47,143,79,255 );  // rgb395
  pick[1493].Int( 47,143,95,255 );  // rgb396
  pick[1494].Int( 47,143,111,255 );  // rgb397
  pick[1495].Int( 47,143,127,255 );  // rgb398
  pick[1496].Int( 47,143,143,255 );  // rgb399
  pick[1497].Int( 47,143,159,255 );  // rgb39A
  pick[1498].Int( 47,143,175,255 );  // rgb39B
  pick[1499].Int( 47,143,191,255 );  // rgb39C
  pick[1500].Int( 47,143,207,255 );  // rgb39D
  pick[1501].Int( 47,143,223,255 );  // rgb39E
  pick[1502].Int( 47,143,239,255 );  // rgb39F
  pick[1503].Int( 47,159,0,255 );  // rgb3A0
  pick[1504].Int( 47,159,15,255 );  // rgb3A1
  pick[1505].Int( 47,159,31,255 );  // rgb3A2
  pick[1506].Int( 47,159,47,255 );  // rgb3A3
  pick[1507].Int( 47,159,63,255 );  // rgb3A4
  pick[1508].Int( 47,159,79,255 );  // rgb3A5
  pick[1509].Int( 47,159,95,255 );  // rgb3A6
  pick[1510].Int( 47,159,111,255 );  // rgb3A7
  pick[1511].Int( 47,159,127,255 );  // rgb3A8
  pick[1512].Int( 47,159,143,255 );  // rgb3A9
  pick[1513].Int( 47,159,159,255 );  // rgb3AA
  pick[1514].Int( 47,159,175,255 );  // rgb3AB
  pick[1515].Int( 47,159,191,255 );  // rgb3AC
  pick[1516].Int( 47,159,207,255 );  // rgb3AD
  pick[1517].Int( 47,159,223,255 );  // rgb3AE
  pick[1518].Int( 47,159,239,255 );  // rgb3AF
  pick[1519].Int( 47,175,0,255 );  // rgb3B0
  pick[1520].Int( 47,175,15,255 );  // rgb3B1
  pick[1521].Int( 47,175,31,255 );  // rgb3B2
  pick[1522].Int( 47,175,47,255 );  // rgb3B3
  pick[1523].Int( 47,175,63,255 );  // rgb3B4
  pick[1524].Int( 47,175,79,255 );  // rgb3B5
  pick[1525].Int( 47,175,95,255 );  // rgb3B6
  pick[1526].Int( 47,175,111,255 );  // rgb3B7
  pick[1527].Int( 47,175,127,255 );  // rgb3B8
  pick[1528].Int( 47,175,143,255 );  // rgb3B9
  pick[1529].Int( 47,175,159,255 );  // rgb3BA
  pick[1530].Int( 47,175,175,255 );  // rgb3BB
  pick[1531].Int( 47,175,191,255 );  // rgb3BC
  pick[1532].Int( 47,175,207,255 );  // rgb3BD
  pick[1533].Int( 47,175,223,255 );  // rgb3BE
  pick[1534].Int( 47,175,239,255 );  // rgb3BF
  pick[1535].Int( 47,191,0,255 );  // rgb3C0
  pick[1536].Int( 47,191,15,255 );  // rgb3C1
  pick[1537].Int( 47,191,31,255 );  // rgb3C2
  pick[1538].Int( 47,191,47,255 );  // rgb3C3
  pick[1539].Int( 47,191,63,255 );  // rgb3C4
  pick[1540].Int( 47,191,79,255 );  // rgb3C5
  pick[1541].Int( 47,191,95,255 );  // rgb3C6
  pick[1542].Int( 47,191,111,255 );  // rgb3C7
  pick[1543].Int( 47,191,127,255 );  // rgb3C8
  pick[1544].Int( 47,191,143,255 );  // rgb3C9
  pick[1545].Int( 47,191,159,255 );  // rgb3CA
  pick[1546].Int( 47,191,175,255 );  // rgb3CB
  pick[1547].Int( 47,191,191,255 );  // rgb3CC
  pick[1548].Int( 47,191,207,255 );  // rgb3CD
  pick[1549].Int( 47,191,223,255 );  // rgb3CE
  pick[1550].Int( 47,191,239,255 );  // rgb3CF
  pick[1551].Int( 47,207,0,255 );  // rgb3D0
  pick[1552].Int( 47,207,15,255 );  // rgb3D1
  pick[1553].Int( 47,207,31,255 );  // rgb3D2
  pick[1554].Int( 47,207,47,255 );  // rgb3D3
  pick[1555].Int( 47,207,63,255 );  // rgb3D4
  pick[1556].Int( 47,207,79,255 );  // rgb3D5
  pick[1557].Int( 47,207,95,255 );  // rgb3D6
  pick[1558].Int( 47,207,111,255 );  // rgb3D7
  pick[1559].Int( 47,207,127,255 );  // rgb3D8
  pick[1560].Int( 47,207,143,255 );  // rgb3D9
  pick[1561].Int( 47,207,159,255 );  // rgb3DA
  pick[1562].Int( 47,207,175,255 );  // rgb3DB
  pick[1563].Int( 47,207,191,255 );  // rgb3DC
  pick[1564].Int( 47,207,207,255 );  // rgb3DD
  pick[1565].Int( 47,207,223,255 );  // rgb3DE
  pick[1566].Int( 47,207,239,255 );  // rgb3DF
  pick[1567].Int( 47,223,0,255 );  // rgb3E0
  pick[1568].Int( 47,223,15,255 );  // rgb3E1
  pick[1569].Int( 47,223,31,255 );  // rgb3E2
  pick[1570].Int( 47,223,47,255 );  // rgb3E3
  pick[1571].Int( 47,223,63,255 );  // rgb3E4
  pick[1572].Int( 47,223,79,255 );  // rgb3E5
  pick[1573].Int( 47,223,95,255 );  // rgb3E6
  pick[1574].Int( 47,223,111,255 );  // rgb3E7
  pick[1575].Int( 47,223,127,255 );  // rgb3E8
  pick[1576].Int( 47,223,143,255 );  // rgb3E9
  pick[1577].Int( 47,223,159,255 );  // rgb3EA
  pick[1578].Int( 47,223,175,255 );  // rgb3EB
  pick[1579].Int( 47,223,191,255 );  // rgb3EC
  pick[1580].Int( 47,223,207,255 );  // rgb3ED
  pick[1581].Int( 47,223,223,255 );  // rgb3EE
  pick[1582].Int( 47,223,239,255 );  // rgb3EF
  pick[1583].Int( 47,239,0,255 );  // rgb3F0
  pick[1584].Int( 47,239,15,255 );  // rgb3F1
  pick[1585].Int( 47,239,31,255 );  // rgb3F2
  pick[1586].Int( 47,239,47,255 );  // rgb3F3
  pick[1587].Int( 47,239,63,255 );  // rgb3F4
  pick[1588].Int( 47,239,79,255 );  // rgb3F5
  pick[1589].Int( 47,239,95,255 );  // rgb3F6
  pick[1590].Int( 47,239,111,255 );  // rgb3F7
  pick[1591].Int( 47,239,127,255 );  // rgb3F8
  pick[1592].Int( 47,239,143,255 );  // rgb3F9
  pick[1593].Int( 47,239,159,255 );  // rgb3FA
  pick[1594].Int( 47,239,175,255 );  // rgb3FB
  pick[1595].Int( 47,239,191,255 );  // rgb3FC
  pick[1596].Int( 47,239,207,255 );  // rgb3FD
  pick[1597].Int( 47,239,223,255 );  // rgb3FE
  pick[1598].Int( 47,239,239,255 );  // rgb3FF
  pick[1599].Int( 63,0,0,255 );  // rgb400
  pick[1600].Int( 63,0,15,255 );  // rgb401
  pick[1601].Int( 63,0,31,255 );  // rgb402
  pick[1602].Int( 63,0,47,255 );  // rgb403
  pick[1603].Int( 63,0,63,255 );  // rgb404
  pick[1604].Int( 63,0,79,255 );  // rgb405
  pick[1605].Int( 63,0,95,255 );  // rgb406
  pick[1606].Int( 63,0,111,255 );  // rgb407
  pick[1607].Int( 63,0,127,255 );  // rgb408
  pick[1608].Int( 63,0,143,255 );  // rgb409
  pick[1609].Int( 63,0,159,255 );  // rgb40A
  pick[1610].Int( 63,0,175,255 );  // rgb40B
  pick[1611].Int( 63,0,191,255 );  // rgb40C
  pick[1612].Int( 63,0,207,255 );  // rgb40D
  pick[1613].Int( 63,0,223,255 );  // rgb40E
  pick[1614].Int( 63,0,239,255 );  // rgb40F
  pick[1615].Int( 63,15,0,255 );  // rgb410
  pick[1616].Int( 63,15,15,255 );  // rgb411
  pick[1617].Int( 63,15,31,255 );  // rgb412
  pick[1618].Int( 63,15,47,255 );  // rgb413
  pick[1619].Int( 63,15,63,255 );  // rgb414
  pick[1620].Int( 63,15,79,255 );  // rgb415
  pick[1621].Int( 63,15,95,255 );  // rgb416
  pick[1622].Int( 63,15,111,255 );  // rgb417
  pick[1623].Int( 63,15,127,255 );  // rgb418
  pick[1624].Int( 63,15,143,255 );  // rgb419
  pick[1625].Int( 63,15,159,255 );  // rgb41A
  pick[1626].Int( 63,15,175,255 );  // rgb41B
  pick[1627].Int( 63,15,191,255 );  // rgb41C
  pick[1628].Int( 63,15,207,255 );  // rgb41D
  pick[1629].Int( 63,15,223,255 );  // rgb41E
  pick[1630].Int( 63,15,239,255 );  // rgb41F
  pick[1631].Int( 63,31,0,255 );  // rgb420
  pick[1632].Int( 63,31,15,255 );  // rgb421
  pick[1633].Int( 63,31,31,255 );  // rgb422
  pick[1634].Int( 63,31,47,255 );  // rgb423
  pick[1635].Int( 63,31,63,255 );  // rgb424
  pick[1636].Int( 63,31,79,255 );  // rgb425
  pick[1637].Int( 63,31,95,255 );  // rgb426
  pick[1638].Int( 63,31,111,255 );  // rgb427
  pick[1639].Int( 63,31,127,255 );  // rgb428
  pick[1640].Int( 63,31,143,255 );  // rgb429
  pick[1641].Int( 63,31,159,255 );  // rgb42A
  pick[1642].Int( 63,31,175,255 );  // rgb42B
  pick[1643].Int( 63,31,191,255 );  // rgb42C
  pick[1644].Int( 63,31,207,255 );  // rgb42D
  pick[1645].Int( 63,31,223,255 );  // rgb42E
  pick[1646].Int( 63,31,239,255 );  // rgb42F
  pick[1647].Int( 63,47,0,255 );  // rgb430
  pick[1648].Int( 63,47,15,255 );  // rgb431
  pick[1649].Int( 63,47,31,255 );  // rgb432
  pick[1650].Int( 63,47,47,255 );  // rgb433
  pick[1651].Int( 63,47,63,255 );  // rgb434
  pick[1652].Int( 63,47,79,255 );  // rgb435
  pick[1653].Int( 63,47,95,255 );  // rgb436
  pick[1654].Int( 63,47,111,255 );  // rgb437
  pick[1655].Int( 63,47,127,255 );  // rgb438
  pick[1656].Int( 63,47,143,255 );  // rgb439
  pick[1657].Int( 63,47,159,255 );  // rgb43A
  pick[1658].Int( 63,47,175,255 );  // rgb43B
  pick[1659].Int( 63,47,191,255 );  // rgb43C
  pick[1660].Int( 63,47,207,255 );  // rgb43D
  pick[1661].Int( 63,47,223,255 );  // rgb43E
  pick[1662].Int( 63,47,239,255 );  // rgb43F
  pick[1663].Int( 63,63,0,255 );  // rgb440
  pick[1664].Int( 63,63,15,255 );  // rgb441
  pick[1665].Int( 63,63,31,255 );  // rgb442
  pick[1666].Int( 63,63,47,255 );  // rgb443
  pick[1667].Int( 63,63,63,255 );  // rgb444
  pick[1668].Int( 63,63,79,255 );  // rgb445
  pick[1669].Int( 63,63,95,255 );  // rgb446
  pick[1670].Int( 63,63,111,255 );  // rgb447
  pick[1671].Int( 63,63,127,255 );  // rgb448
  pick[1672].Int( 63,63,143,255 );  // rgb449
  pick[1673].Int( 63,63,159,255 );  // rgb44A
  pick[1674].Int( 63,63,175,255 );  // rgb44B
  pick[1675].Int( 63,63,191,255 );  // rgb44C
  pick[1676].Int( 63,63,207,255 );  // rgb44D
  pick[1677].Int( 63,63,223,255 );  // rgb44E
  pick[1678].Int( 63,63,239,255 );  // rgb44F
  pick[1679].Int( 63,79,0,255 );  // rgb450
  pick[1680].Int( 63,79,15,255 );  // rgb451
  pick[1681].Int( 63,79,31,255 );  // rgb452
  pick[1682].Int( 63,79,47,255 );  // rgb453
  pick[1683].Int( 63,79,63,255 );  // rgb454
  pick[1684].Int( 63,79,79,255 );  // rgb455
  pick[1685].Int( 63,79,95,255 );  // rgb456
  pick[1686].Int( 63,79,111,255 );  // rgb457
  pick[1687].Int( 63,79,127,255 );  // rgb458
  pick[1688].Int( 63,79,143,255 );  // rgb459
  pick[1689].Int( 63,79,159,255 );  // rgb45A
  pick[1690].Int( 63,79,175,255 );  // rgb45B
  pick[1691].Int( 63,79,191,255 );  // rgb45C
  pick[1692].Int( 63,79,207,255 );  // rgb45D
  pick[1693].Int( 63,79,223,255 );  // rgb45E
  pick[1694].Int( 63,79,239,255 );  // rgb45F
  pick[1695].Int( 63,95,0,255 );  // rgb460
  pick[1696].Int( 63,95,15,255 );  // rgb461
  pick[1697].Int( 63,95,31,255 );  // rgb462
  pick[1698].Int( 63,95,47,255 );  // rgb463
  pick[1699].Int( 63,95,63,255 );  // rgb464
  pick[1700].Int( 63,95,79,255 );  // rgb465
  pick[1701].Int( 63,95,95,255 );  // rgb466
  pick[1702].Int( 63,95,111,255 );  // rgb467
  pick[1703].Int( 63,95,127,255 );  // rgb468
  pick[1704].Int( 63,95,143,255 );  // rgb469
  pick[1705].Int( 63,95,159,255 );  // rgb46A
  pick[1706].Int( 63,95,175,255 );  // rgb46B
  pick[1707].Int( 63,95,191,255 );  // rgb46C
  pick[1708].Int( 63,95,207,255 );  // rgb46D
  pick[1709].Int( 63,95,223,255 );  // rgb46E
  pick[1710].Int( 63,95,239,255 );  // rgb46F
  pick[1711].Int( 63,111,0,255 );  // rgb470
  pick[1712].Int( 63,111,15,255 );  // rgb471
  pick[1713].Int( 63,111,31,255 );  // rgb472
  pick[1714].Int( 63,111,47,255 );  // rgb473
  pick[1715].Int( 63,111,63,255 );  // rgb474
  pick[1716].Int( 63,111,79,255 );  // rgb475
  pick[1717].Int( 63,111,95,255 );  // rgb476
  pick[1718].Int( 63,111,111,255 );  // rgb477
  pick[1719].Int( 63,111,127,255 );  // rgb478
  pick[1720].Int( 63,111,143,255 );  // rgb479
  pick[1721].Int( 63,111,159,255 );  // rgb47A
  pick[1722].Int( 63,111,175,255 );  // rgb47B
  pick[1723].Int( 63,111,191,255 );  // rgb47C
  pick[1724].Int( 63,111,207,255 );  // rgb47D
  pick[1725].Int( 63,111,223,255 );  // rgb47E
  pick[1726].Int( 63,111,239,255 );  // rgb47F
  pick[1727].Int( 63,127,0,255 );  // rgb480
  pick[1728].Int( 63,127,15,255 );  // rgb481
  pick[1729].Int( 63,127,31,255 );  // rgb482
  pick[1730].Int( 63,127,47,255 );  // rgb483
  pick[1731].Int( 63,127,63,255 );  // rgb484
  pick[1732].Int( 63,127,79,255 );  // rgb485
  pick[1733].Int( 63,127,95,255 );  // rgb486
  pick[1734].Int( 63,127,111,255 );  // rgb487
  pick[1735].Int( 63,127,127,255 );  // rgb488
  pick[1736].Int( 63,127,143,255 );  // rgb489
  pick[1737].Int( 63,127,159,255 );  // rgb48A
  pick[1738].Int( 63,127,175,255 );  // rgb48B
  pick[1739].Int( 63,127,191,255 );  // rgb48C
  pick[1740].Int( 63,127,207,255 );  // rgb48D
  pick[1741].Int( 63,127,223,255 );  // rgb48E
  pick[1742].Int( 63,127,239,255 );  // rgb48F
  pick[1743].Int( 63,143,0,255 );  // rgb490
  pick[1744].Int( 63,143,15,255 );  // rgb491
  pick[1745].Int( 63,143,31,255 );  // rgb492
  pick[1746].Int( 63,143,47,255 );  // rgb493
  pick[1747].Int( 63,143,63,255 );  // rgb494
  pick[1748].Int( 63,143,79,255 );  // rgb495
  pick[1749].Int( 63,143,95,255 );  // rgb496
  pick[1750].Int( 63,143,111,255 );  // rgb497
  pick[1751].Int( 63,143,127,255 );  // rgb498
  pick[1752].Int( 63,143,143,255 );  // rgb499
  pick[1753].Int( 63,143,159,255 );  // rgb49A
  pick[1754].Int( 63,143,175,255 );  // rgb49B
  pick[1755].Int( 63,143,191,255 );  // rgb49C
  pick[1756].Int( 63,143,207,255 );  // rgb49D
  pick[1757].Int( 63,143,223,255 );  // rgb49E
  pick[1758].Int( 63,143,239,255 );  // rgb49F
  pick[1759].Int( 63,159,0,255 );  // rgb4A0
  pick[1760].Int( 63,159,15,255 );  // rgb4A1
  pick[1761].Int( 63,159,31,255 );  // rgb4A2
  pick[1762].Int( 63,159,47,255 );  // rgb4A3
  pick[1763].Int( 63,159,63,255 );  // rgb4A4
  pick[1764].Int( 63,159,79,255 );  // rgb4A5
  pick[1765].Int( 63,159,95,255 );  // rgb4A6
  pick[1766].Int( 63,159,111,255 );  // rgb4A7
  pick[1767].Int( 63,159,127,255 );  // rgb4A8
  pick[1768].Int( 63,159,143,255 );  // rgb4A9
  pick[1769].Int( 63,159,159,255 );  // rgb4AA
  pick[1770].Int( 63,159,175,255 );  // rgb4AB
  pick[1771].Int( 63,159,191,255 );  // rgb4AC
  pick[1772].Int( 63,159,207,255 );  // rgb4AD
  pick[1773].Int( 63,159,223,255 );  // rgb4AE
  pick[1774].Int( 63,159,239,255 );  // rgb4AF
  pick[1775].Int( 63,175,0,255 );  // rgb4B0
  pick[1776].Int( 63,175,15,255 );  // rgb4B1
  pick[1777].Int( 63,175,31,255 );  // rgb4B2
  pick[1778].Int( 63,175,47,255 );  // rgb4B3
  pick[1779].Int( 63,175,63,255 );  // rgb4B4
  pick[1780].Int( 63,175,79,255 );  // rgb4B5
  pick[1781].Int( 63,175,95,255 );  // rgb4B6
  pick[1782].Int( 63,175,111,255 );  // rgb4B7
  pick[1783].Int( 63,175,127,255 );  // rgb4B8
  pick[1784].Int( 63,175,143,255 );  // rgb4B9
  pick[1785].Int( 63,175,159,255 );  // rgb4BA
  pick[1786].Int( 63,175,175,255 );  // rgb4BB
  pick[1787].Int( 63,175,191,255 );  // rgb4BC
  pick[1788].Int( 63,175,207,255 );  // rgb4BD
  pick[1789].Int( 63,175,223,255 );  // rgb4BE
  pick[1790].Int( 63,175,239,255 );  // rgb4BF
  pick[1791].Int( 63,191,0,255 );  // rgb4C0
  pick[1792].Int( 63,191,15,255 );  // rgb4C1
  pick[1793].Int( 63,191,31,255 );  // rgb4C2
  pick[1794].Int( 63,191,47,255 );  // rgb4C3
  pick[1795].Int( 63,191,63,255 );  // rgb4C4
  pick[1796].Int( 63,191,79,255 );  // rgb4C5
  pick[1797].Int( 63,191,95,255 );  // rgb4C6
  pick[1798].Int( 63,191,111,255 );  // rgb4C7
  pick[1799].Int( 63,191,127,255 );  // rgb4C8
  pick[1800].Int( 63,191,143,255 );  // rgb4C9
  pick[1801].Int( 63,191,159,255 );  // rgb4CA
  pick[1802].Int( 63,191,175,255 );  // rgb4CB
  pick[1803].Int( 63,191,191,255 );  // rgb4CC
  pick[1804].Int( 63,191,207,255 );  // rgb4CD
  pick[1805].Int( 63,191,223,255 );  // rgb4CE
  pick[1806].Int( 63,191,239,255 );  // rgb4CF
  pick[1807].Int( 63,207,0,255 );  // rgb4D0
  pick[1808].Int( 63,207,15,255 );  // rgb4D1
  pick[1809].Int( 63,207,31,255 );  // rgb4D2
  pick[1810].Int( 63,207,47,255 );  // rgb4D3
  pick[1811].Int( 63,207,63,255 );  // rgb4D4
  pick[1812].Int( 63,207,79,255 );  // rgb4D5
  pick[1813].Int( 63,207,95,255 );  // rgb4D6
  pick[1814].Int( 63,207,111,255 );  // rgb4D7
  pick[1815].Int( 63,207,127,255 );  // rgb4D8
  pick[1816].Int( 63,207,143,255 );  // rgb4D9
  pick[1817].Int( 63,207,159,255 );  // rgb4DA
  pick[1818].Int( 63,207,175,255 );  // rgb4DB
  pick[1819].Int( 63,207,191,255 );  // rgb4DC
  pick[1820].Int( 63,207,207,255 );  // rgb4DD
  pick[1821].Int( 63,207,223,255 );  // rgb4DE
  pick[1822].Int( 63,207,239,255 );  // rgb4DF
  pick[1823].Int( 63,223,0,255 );  // rgb4E0
  pick[1824].Int( 63,223,15,255 );  // rgb4E1
  pick[1825].Int( 63,223,31,255 );  // rgb4E2
  pick[1826].Int( 63,223,47,255 );  // rgb4E3
  pick[1827].Int( 63,223,63,255 );  // rgb4E4
  pick[1828].Int( 63,223,79,255 );  // rgb4E5
  pick[1829].Int( 63,223,95,255 );  // rgb4E6
  pick[1830].Int( 63,223,111,255 );  // rgb4E7
  pick[1831].Int( 63,223,127,255 );  // rgb4E8
  pick[1832].Int( 63,223,143,255 );  // rgb4E9
  pick[1833].Int( 63,223,159,255 );  // rgb4EA
  pick[1834].Int( 63,223,175,255 );  // rgb4EB
  pick[1835].Int( 63,223,191,255 );  // rgb4EC
  pick[1836].Int( 63,223,207,255 );  // rgb4ED
  pick[1837].Int( 63,223,223,255 );  // rgb4EE
  pick[1838].Int( 63,223,239,255 );  // rgb4EF
  pick[1839].Int( 63,239,0,255 );  // rgb4F0
  pick[1840].Int( 63,239,15,255 );  // rgb4F1
  pick[1841].Int( 63,239,31,255 );  // rgb4F2
  pick[1842].Int( 63,239,47,255 );  // rgb4F3
  pick[1843].Int( 63,239,63,255 );  // rgb4F4
  pick[1844].Int( 63,239,79,255 );  // rgb4F5
  pick[1845].Int( 63,239,95,255 );  // rgb4F6
  pick[1846].Int( 63,239,111,255 );  // rgb4F7
  pick[1847].Int( 63,239,127,255 );  // rgb4F8
  pick[1848].Int( 63,239,143,255 );  // rgb4F9
  pick[1849].Int( 63,239,159,255 );  // rgb4FA
  pick[1850].Int( 63,239,175,255 );  // rgb4FB
  pick[1851].Int( 63,239,191,255 );  // rgb4FC
  pick[1852].Int( 63,239,207,255 );  // rgb4FD
  pick[1853].Int( 63,239,223,255 );  // rgb4FE
  pick[1854].Int( 63,239,239,255 );  // rgb4FF
  pick[1855].Int( 79,0,0,255 );  // rgb500
  pick[1856].Int( 79,0,15,255 );  // rgb501
  pick[1857].Int( 79,0,31,255 );  // rgb502
  pick[1858].Int( 79,0,47,255 );  // rgb503
  pick[1859].Int( 79,0,63,255 );  // rgb504
  pick[1860].Int( 79,0,79,255 );  // rgb505
  pick[1861].Int( 79,0,95,255 );  // rgb506
  pick[1862].Int( 79,0,111,255 );  // rgb507
  pick[1863].Int( 79,0,127,255 );  // rgb508
  pick[1864].Int( 79,0,143,255 );  // rgb509
  pick[1865].Int( 79,0,159,255 );  // rgb50A
  pick[1866].Int( 79,0,175,255 );  // rgb50B
  pick[1867].Int( 79,0,191,255 );  // rgb50C
  pick[1868].Int( 79,0,207,255 );  // rgb50D
  pick[1869].Int( 79,0,223,255 );  // rgb50E
  pick[1870].Int( 79,0,239,255 );  // rgb50F
  pick[1871].Int( 79,15,0,255 );  // rgb510
  pick[1872].Int( 79,15,15,255 );  // rgb511
  pick[1873].Int( 79,15,31,255 );  // rgb512
  pick[1874].Int( 79,15,47,255 );  // rgb513
  pick[1875].Int( 79,15,63,255 );  // rgb514
  pick[1876].Int( 79,15,79,255 );  // rgb515
  pick[1877].Int( 79,15,95,255 );  // rgb516
  pick[1878].Int( 79,15,111,255 );  // rgb517
  pick[1879].Int( 79,15,127,255 );  // rgb518
  pick[1880].Int( 79,15,143,255 );  // rgb519
  pick[1881].Int( 79,15,159,255 );  // rgb51A
  pick[1882].Int( 79,15,175,255 );  // rgb51B
  pick[1883].Int( 79,15,191,255 );  // rgb51C
  pick[1884].Int( 79,15,207,255 );  // rgb51D
  pick[1885].Int( 79,15,223,255 );  // rgb51E
  pick[1886].Int( 79,15,239,255 );  // rgb51F
  pick[1887].Int( 79,31,0,255 );  // rgb520
  pick[1888].Int( 79,31,15,255 );  // rgb521
  pick[1889].Int( 79,31,31,255 );  // rgb522
  pick[1890].Int( 79,31,47,255 );  // rgb523
  pick[1891].Int( 79,31,63,255 );  // rgb524
  pick[1892].Int( 79,31,79,255 );  // rgb525
  pick[1893].Int( 79,31,95,255 );  // rgb526
  pick[1894].Int( 79,31,111,255 );  // rgb527
  pick[1895].Int( 79,31,127,255 );  // rgb528
  pick[1896].Int( 79,31,143,255 );  // rgb529
  pick[1897].Int( 79,31,159,255 );  // rgb52A
  pick[1898].Int( 79,31,175,255 );  // rgb52B
  pick[1899].Int( 79,31,191,255 );  // rgb52C
  pick[1900].Int( 79,31,207,255 );  // rgb52D
  pick[1901].Int( 79,31,223,255 );  // rgb52E
  pick[1902].Int( 79,31,239,255 );  // rgb52F
  pick[1903].Int( 79,47,0,255 );  // rgb530
  pick[1904].Int( 79,47,15,255 );  // rgb531
  pick[1905].Int( 79,47,31,255 );  // rgb532
  pick[1906].Int( 79,47,47,255 );  // rgb533
  pick[1907].Int( 79,47,63,255 );  // rgb534
  pick[1908].Int( 79,47,79,255 );  // rgb535
  pick[1909].Int( 79,47,95,255 );  // rgb536
  pick[1910].Int( 79,47,111,255 );  // rgb537
  pick[1911].Int( 79,47,127,255 );  // rgb538
  pick[1912].Int( 79,47,143,255 );  // rgb539
  pick[1913].Int( 79,47,159,255 );  // rgb53A
  pick[1914].Int( 79,47,175,255 );  // rgb53B
  pick[1915].Int( 79,47,191,255 );  // rgb53C
  pick[1916].Int( 79,47,207,255 );  // rgb53D
  pick[1917].Int( 79,47,223,255 );  // rgb53E
  pick[1918].Int( 79,47,239,255 );  // rgb53F
  pick[1919].Int( 79,63,0,255 );  // rgb540
  pick[1920].Int( 79,63,15,255 );  // rgb541
  pick[1921].Int( 79,63,31,255 );  // rgb542
  pick[1922].Int( 79,63,47,255 );  // rgb543
  pick[1923].Int( 79,63,63,255 );  // rgb544
  pick[1924].Int( 79,63,79,255 );  // rgb545
  pick[1925].Int( 79,63,95,255 );  // rgb546
  pick[1926].Int( 79,63,111,255 );  // rgb547
  pick[1927].Int( 79,63,127,255 );  // rgb548
  pick[1928].Int( 79,63,143,255 );  // rgb549
  pick[1929].Int( 79,63,159,255 );  // rgb54A
  pick[1930].Int( 79,63,175,255 );  // rgb54B
  pick[1931].Int( 79,63,191,255 );  // rgb54C
  pick[1932].Int( 79,63,207,255 );  // rgb54D
  pick[1933].Int( 79,63,223,255 );  // rgb54E
  pick[1934].Int( 79,63,239,255 );  // rgb54F
  pick[1935].Int( 79,79,0,255 );  // rgb550
  pick[1936].Int( 79,79,15,255 );  // rgb551
  pick[1937].Int( 79,79,31,255 );  // rgb552
  pick[1938].Int( 79,79,47,255 );  // rgb553
  pick[1939].Int( 79,79,63,255 );  // rgb554
  pick[1940].Int( 79,79,79,255 );  // rgb555
  pick[1941].Int( 79,79,95,255 );  // rgb556
  pick[1942].Int( 79,79,111,255 );  // rgb557
  pick[1943].Int( 79,79,127,255 );  // rgb558
  pick[1944].Int( 79,79,143,255 );  // rgb559
  pick[1945].Int( 79,79,159,255 );  // rgb55A
  pick[1946].Int( 79,79,175,255 );  // rgb55B
  pick[1947].Int( 79,79,191,255 );  // rgb55C
  pick[1948].Int( 79,79,207,255 );  // rgb55D
  pick[1949].Int( 79,79,223,255 );  // rgb55E
  pick[1950].Int( 79,79,239,255 );  // rgb55F
  pick[1951].Int( 79,95,0,255 );  // rgb560
  pick[1952].Int( 79,95,15,255 );  // rgb561
  pick[1953].Int( 79,95,31,255 );  // rgb562
  pick[1954].Int( 79,95,47,255 );  // rgb563
  pick[1955].Int( 79,95,63,255 );  // rgb564
  pick[1956].Int( 79,95,79,255 );  // rgb565
  pick[1957].Int( 79,95,95,255 );  // rgb566
  pick[1958].Int( 79,95,111,255 );  // rgb567
  pick[1959].Int( 79,95,127,255 );  // rgb568
  pick[1960].Int( 79,95,143,255 );  // rgb569
  pick[1961].Int( 79,95,159,255 );  // rgb56A
  pick[1962].Int( 79,95,175,255 );  // rgb56B
  pick[1963].Int( 79,95,191,255 );  // rgb56C
  pick[1964].Int( 79,95,207,255 );  // rgb56D
  pick[1965].Int( 79,95,223,255 );  // rgb56E
  pick[1966].Int( 79,95,239,255 );  // rgb56F
  pick[1967].Int( 79,111,0,255 );  // rgb570
  pick[1968].Int( 79,111,15,255 );  // rgb571
  pick[1969].Int( 79,111,31,255 );  // rgb572
  pick[1970].Int( 79,111,47,255 );  // rgb573
  pick[1971].Int( 79,111,63,255 );  // rgb574
  pick[1972].Int( 79,111,79,255 );  // rgb575
  pick[1973].Int( 79,111,95,255 );  // rgb576
  pick[1974].Int( 79,111,111,255 );  // rgb577
  pick[1975].Int( 79,111,127,255 );  // rgb578
  pick[1976].Int( 79,111,143,255 );  // rgb579
  pick[1977].Int( 79,111,159,255 );  // rgb57A
  pick[1978].Int( 79,111,175,255 );  // rgb57B
  pick[1979].Int( 79,111,191,255 );  // rgb57C
  pick[1980].Int( 79,111,207,255 );  // rgb57D
  pick[1981].Int( 79,111,223,255 );  // rgb57E
  pick[1982].Int( 79,111,239,255 );  // rgb57F
  pick[1983].Int( 79,127,0,255 );  // rgb580
  pick[1984].Int( 79,127,15,255 );  // rgb581
  pick[1985].Int( 79,127,31,255 );  // rgb582
  pick[1986].Int( 79,127,47,255 );  // rgb583
  pick[1987].Int( 79,127,63,255 );  // rgb584
  pick[1988].Int( 79,127,79,255 );  // rgb585
  pick[1989].Int( 79,127,95,255 );  // rgb586
  pick[1990].Int( 79,127,111,255 );  // rgb587
  pick[1991].Int( 79,127,127,255 );  // rgb588
  pick[1992].Int( 79,127,143,255 );  // rgb589
  pick[1993].Int( 79,127,159,255 );  // rgb58A
  pick[1994].Int( 79,127,175,255 );  // rgb58B
  pick[1995].Int( 79,127,191,255 );  // rgb58C
  pick[1996].Int( 79,127,207,255 );  // rgb58D
  pick[1997].Int( 79,127,223,255 );  // rgb58E
  pick[1998].Int( 79,127,239,255 );  // rgb58F
  pick[1999].Int( 79,143,0,255 );  // rgb590
  pick[2000].Int( 79,143,15,255 );  // rgb591
  pick[2001].Int( 79,143,31,255 );  // rgb592
  pick[2002].Int( 79,143,47,255 );  // rgb593
  pick[2003].Int( 79,143,63,255 );  // rgb594
  pick[2004].Int( 79,143,79,255 );  // rgb595
  pick[2005].Int( 79,143,95,255 );  // rgb596
  pick[2006].Int( 79,143,111,255 );  // rgb597
  pick[2007].Int( 79,143,127,255 );  // rgb598
  pick[2008].Int( 79,143,143,255 );  // rgb599
  pick[2009].Int( 79,143,159,255 );  // rgb59A
  pick[2010].Int( 79,143,175,255 );  // rgb59B
  pick[2011].Int( 79,143,191,255 );  // rgb59C
  pick[2012].Int( 79,143,207,255 );  // rgb59D
  pick[2013].Int( 79,143,223,255 );  // rgb59E
  pick[2014].Int( 79,143,239,255 );  // rgb59F
  pick[2015].Int( 79,159,0,255 );  // rgb5A0
  pick[2016].Int( 79,159,15,255 );  // rgb5A1
  pick[2017].Int( 79,159,31,255 );  // rgb5A2
  pick[2018].Int( 79,159,47,255 );  // rgb5A3
  pick[2019].Int( 79,159,63,255 );  // rgb5A4
  pick[2020].Int( 79,159,79,255 );  // rgb5A5
  pick[2021].Int( 79,159,95,255 );  // rgb5A6
  pick[2022].Int( 79,159,111,255 );  // rgb5A7
  pick[2023].Int( 79,159,127,255 );  // rgb5A8
  pick[2024].Int( 79,159,143,255 );  // rgb5A9
  pick[2025].Int( 79,159,159,255 );  // rgb5AA
  pick[2026].Int( 79,159,175,255 );  // rgb5AB
  pick[2027].Int( 79,159,191,255 );  // rgb5AC
  pick[2028].Int( 79,159,207,255 );  // rgb5AD
  pick[2029].Int( 79,159,223,255 );  // rgb5AE
  pick[2030].Int( 79,159,239,255 );  // rgb5AF
  pick[2031].Int( 79,175,0,255 );  // rgb5B0
  pick[2032].Int( 79,175,15,255 );  // rgb5B1
  pick[2033].Int( 79,175,31,255 );  // rgb5B2
  pick[2034].Int( 79,175,47,255 );  // rgb5B3
  pick[2035].Int( 79,175,63,255 );  // rgb5B4
  pick[2036].Int( 79,175,79,255 );  // rgb5B5
  pick[2037].Int( 79,175,95,255 );  // rgb5B6
  pick[2038].Int( 79,175,111,255 );  // rgb5B7
  pick[2039].Int( 79,175,127,255 );  // rgb5B8
  pick[2040].Int( 79,175,143,255 );  // rgb5B9
  pick[2041].Int( 79,175,159,255 );  // rgb5BA
  pick[2042].Int( 79,175,175,255 );  // rgb5BB
  pick[2043].Int( 79,175,191,255 );  // rgb5BC
  pick[2044].Int( 79,175,207,255 );  // rgb5BD
  pick[2045].Int( 79,175,223,255 );  // rgb5BE
  pick[2046].Int( 79,175,239,255 );  // rgb5BF
  pick[2047].Int( 79,191,0,255 );  // rgb5C0
  pick[2048].Int( 79,191,15,255 );  // rgb5C1
  pick[2049].Int( 79,191,31,255 );  // rgb5C2
  pick[2050].Int( 79,191,47,255 );  // rgb5C3
  pick[2051].Int( 79,191,63,255 );  // rgb5C4
  pick[2052].Int( 79,191,79,255 );  // rgb5C5
  pick[2053].Int( 79,191,95,255 );  // rgb5C6
  pick[2054].Int( 79,191,111,255 );  // rgb5C7
  pick[2055].Int( 79,191,127,255 );  // rgb5C8
  pick[2056].Int( 79,191,143,255 );  // rgb5C9
  pick[2057].Int( 79,191,159,255 );  // rgb5CA
  pick[2058].Int( 79,191,175,255 );  // rgb5CB
  pick[2059].Int( 79,191,191,255 );  // rgb5CC
  pick[2060].Int( 79,191,207,255 );  // rgb5CD
  pick[2061].Int( 79,191,223,255 );  // rgb5CE
  pick[2062].Int( 79,191,239,255 );  // rgb5CF
  pick[2063].Int( 79,207,0,255 );  // rgb5D0
  pick[2064].Int( 79,207,15,255 );  // rgb5D1
  pick[2065].Int( 79,207,31,255 );  // rgb5D2
  pick[2066].Int( 79,207,47,255 );  // rgb5D3
  pick[2067].Int( 79,207,63,255 );  // rgb5D4
  pick[2068].Int( 79,207,79,255 );  // rgb5D5
  pick[2069].Int( 79,207,95,255 );  // rgb5D6
  pick[2070].Int( 79,207,111,255 );  // rgb5D7
  pick[2071].Int( 79,207,127,255 );  // rgb5D8
  pick[2072].Int( 79,207,143,255 );  // rgb5D9
  pick[2073].Int( 79,207,159,255 );  // rgb5DA
  pick[2074].Int( 79,207,175,255 );  // rgb5DB
  pick[2075].Int( 79,207,191,255 );  // rgb5DC
  pick[2076].Int( 79,207,207,255 );  // rgb5DD
  pick[2077].Int( 79,207,223,255 );  // rgb5DE
  pick[2078].Int( 79,207,239,255 );  // rgb5DF
  pick[2079].Int( 79,223,0,255 );  // rgb5E0
  pick[2080].Int( 79,223,15,255 );  // rgb5E1
  pick[2081].Int( 79,223,31,255 );  // rgb5E2
  pick[2082].Int( 79,223,47,255 );  // rgb5E3
  pick[2083].Int( 79,223,63,255 );  // rgb5E4
  pick[2084].Int( 79,223,79,255 );  // rgb5E5
  pick[2085].Int( 79,223,95,255 );  // rgb5E6
  pick[2086].Int( 79,223,111,255 );  // rgb5E7
  pick[2087].Int( 79,223,127,255 );  // rgb5E8
  pick[2088].Int( 79,223,143,255 );  // rgb5E9
  pick[2089].Int( 79,223,159,255 );  // rgb5EA
  pick[2090].Int( 79,223,175,255 );  // rgb5EB
  pick[2091].Int( 79,223,191,255 );  // rgb5EC
  pick[2092].Int( 79,223,207,255 );  // rgb5ED
  pick[2093].Int( 79,223,223,255 );  // rgb5EE
  pick[2094].Int( 79,223,239,255 );  // rgb5EF
  pick[2095].Int( 79,239,0,255 );  // rgb5F0
  pick[2096].Int( 79,239,15,255 );  // rgb5F1
  pick[2097].Int( 79,239,31,255 );  // rgb5F2
  pick[2098].Int( 79,239,47,255 );  // rgb5F3
  pick[2099].Int( 79,239,63,255 );  // rgb5F4
  pick[2100].Int( 79,239,79,255 );  // rgb5F5
  pick[2101].Int( 79,239,95,255 );  // rgb5F6
  pick[2102].Int( 79,239,111,255 );  // rgb5F7
  pick[2103].Int( 79,239,127,255 );  // rgb5F8
  pick[2104].Int( 79,239,143,255 );  // rgb5F9
  pick[2105].Int( 79,239,159,255 );  // rgb5FA
  pick[2106].Int( 79,239,175,255 );  // rgb5FB
  pick[2107].Int( 79,239,191,255 );  // rgb5FC
  pick[2108].Int( 79,239,207,255 );  // rgb5FD
  pick[2109].Int( 79,239,223,255 );  // rgb5FE
  pick[2110].Int( 79,239,239,255 );  // rgb5FF
  pick[2111].Int( 95,0,0,255 );  // rgb600
  pick[2112].Int( 95,0,15,255 );  // rgb601
  pick[2113].Int( 95,0,31,255 );  // rgb602
  pick[2114].Int( 95,0,47,255 );  // rgb603
  pick[2115].Int( 95,0,63,255 );  // rgb604
  pick[2116].Int( 95,0,79,255 );  // rgb605
  pick[2117].Int( 95,0,95,255 );  // rgb606
  pick[2118].Int( 95,0,111,255 );  // rgb607
  pick[2119].Int( 95,0,127,255 );  // rgb608
  pick[2120].Int( 95,0,143,255 );  // rgb609
  pick[2121].Int( 95,0,159,255 );  // rgb60A
  pick[2122].Int( 95,0,175,255 );  // rgb60B
  pick[2123].Int( 95,0,191,255 );  // rgb60C
  pick[2124].Int( 95,0,207,255 );  // rgb60D
  pick[2125].Int( 95,0,223,255 );  // rgb60E
  pick[2126].Int( 95,0,239,255 );  // rgb60F
  pick[2127].Int( 95,15,0,255 );  // rgb610
  pick[2128].Int( 95,15,15,255 );  // rgb611
  pick[2129].Int( 95,15,31,255 );  // rgb612
  pick[2130].Int( 95,15,47,255 );  // rgb613
  pick[2131].Int( 95,15,63,255 );  // rgb614
  pick[2132].Int( 95,15,79,255 );  // rgb615
  pick[2133].Int( 95,15,95,255 );  // rgb616
  pick[2134].Int( 95,15,111,255 );  // rgb617
  pick[2135].Int( 95,15,127,255 );  // rgb618
  pick[2136].Int( 95,15,143,255 );  // rgb619
  pick[2137].Int( 95,15,159,255 );  // rgb61A
  pick[2138].Int( 95,15,175,255 );  // rgb61B
  pick[2139].Int( 95,15,191,255 );  // rgb61C
  pick[2140].Int( 95,15,207,255 );  // rgb61D
  pick[2141].Int( 95,15,223,255 );  // rgb61E
  pick[2142].Int( 95,15,239,255 );  // rgb61F
  pick[2143].Int( 95,31,0,255 );  // rgb620
  pick[2144].Int( 95,31,15,255 );  // rgb621
  pick[2145].Int( 95,31,31,255 );  // rgb622
  pick[2146].Int( 95,31,47,255 );  // rgb623
  pick[2147].Int( 95,31,63,255 );  // rgb624
  pick[2148].Int( 95,31,79,255 );  // rgb625
  pick[2149].Int( 95,31,95,255 );  // rgb626
  pick[2150].Int( 95,31,111,255 );  // rgb627
  pick[2151].Int( 95,31,127,255 );  // rgb628
  pick[2152].Int( 95,31,143,255 );  // rgb629
  pick[2153].Int( 95,31,159,255 );  // rgb62A
  pick[2154].Int( 95,31,175,255 );  // rgb62B
  pick[2155].Int( 95,31,191,255 );  // rgb62C
  pick[2156].Int( 95,31,207,255 );  // rgb62D
  pick[2157].Int( 95,31,223,255 );  // rgb62E
  pick[2158].Int( 95,31,239,255 );  // rgb62F
  pick[2159].Int( 95,47,0,255 );  // rgb630
  pick[2160].Int( 95,47,15,255 );  // rgb631
  pick[2161].Int( 95,47,31,255 );  // rgb632
  pick[2162].Int( 95,47,47,255 );  // rgb633
  pick[2163].Int( 95,47,63,255 );  // rgb634
  pick[2164].Int( 95,47,79,255 );  // rgb635
  pick[2165].Int( 95,47,95,255 );  // rgb636
  pick[2166].Int( 95,47,111,255 );  // rgb637
  pick[2167].Int( 95,47,127,255 );  // rgb638
  pick[2168].Int( 95,47,143,255 );  // rgb639
  pick[2169].Int( 95,47,159,255 );  // rgb63A
  pick[2170].Int( 95,47,175,255 );  // rgb63B
  pick[2171].Int( 95,47,191,255 );  // rgb63C
  pick[2172].Int( 95,47,207,255 );  // rgb63D
  pick[2173].Int( 95,47,223,255 );  // rgb63E
  pick[2174].Int( 95,47,239,255 );  // rgb63F
  pick[2175].Int( 95,63,0,255 );  // rgb640
  pick[2176].Int( 95,63,15,255 );  // rgb641
  pick[2177].Int( 95,63,31,255 );  // rgb642
  pick[2178].Int( 95,63,47,255 );  // rgb643
  pick[2179].Int( 95,63,63,255 );  // rgb644
  pick[2180].Int( 95,63,79,255 );  // rgb645
  pick[2181].Int( 95,63,95,255 );  // rgb646
  pick[2182].Int( 95,63,111,255 );  // rgb647
  pick[2183].Int( 95,63,127,255 );  // rgb648
  pick[2184].Int( 95,63,143,255 );  // rgb649
  pick[2185].Int( 95,63,159,255 );  // rgb64A
  pick[2186].Int( 95,63,175,255 );  // rgb64B
  pick[2187].Int( 95,63,191,255 );  // rgb64C
  pick[2188].Int( 95,63,207,255 );  // rgb64D
  pick[2189].Int( 95,63,223,255 );  // rgb64E
  pick[2190].Int( 95,63,239,255 );  // rgb64F
  pick[2191].Int( 95,79,0,255 );  // rgb650
  pick[2192].Int( 95,79,15,255 );  // rgb651
  pick[2193].Int( 95,79,31,255 );  // rgb652
  pick[2194].Int( 95,79,47,255 );  // rgb653
  pick[2195].Int( 95,79,63,255 );  // rgb654
  pick[2196].Int( 95,79,79,255 );  // rgb655
  pick[2197].Int( 95,79,95,255 );  // rgb656
  pick[2198].Int( 95,79,111,255 );  // rgb657
  pick[2199].Int( 95,79,127,255 );  // rgb658
  pick[2200].Int( 95,79,143,255 );  // rgb659
  pick[2201].Int( 95,79,159,255 );  // rgb65A
  pick[2202].Int( 95,79,175,255 );  // rgb65B
  pick[2203].Int( 95,79,191,255 );  // rgb65C
  pick[2204].Int( 95,79,207,255 );  // rgb65D
  pick[2205].Int( 95,79,223,255 );  // rgb65E
  pick[2206].Int( 95,79,239,255 );  // rgb65F
  pick[2207].Int( 95,95,0,255 );  // rgb660
  pick[2208].Int( 95,95,15,255 );  // rgb661
  pick[2209].Int( 95,95,31,255 );  // rgb662
  pick[2210].Int( 95,95,47,255 );  // rgb663
  pick[2211].Int( 95,95,63,255 );  // rgb664
  pick[2212].Int( 95,95,79,255 );  // rgb665
  pick[2213].Int( 95,95,95,255 );  // rgb666
  pick[2214].Int( 95,95,111,255 );  // rgb667
  pick[2215].Int( 95,95,127,255 );  // rgb668
  pick[2216].Int( 95,95,143,255 );  // rgb669
  pick[2217].Int( 95,95,159,255 );  // rgb66A
  pick[2218].Int( 95,95,175,255 );  // rgb66B
  pick[2219].Int( 95,95,191,255 );  // rgb66C
  pick[2220].Int( 95,95,207,255 );  // rgb66D
  pick[2221].Int( 95,95,223,255 );  // rgb66E
  pick[2222].Int( 95,95,239,255 );  // rgb66F
  pick[2223].Int( 95,111,0,255 );  // rgb670
  pick[2224].Int( 95,111,15,255 );  // rgb671
  pick[2225].Int( 95,111,31,255 );  // rgb672
  pick[2226].Int( 95,111,47,255 );  // rgb673
  pick[2227].Int( 95,111,63,255 );  // rgb674
  pick[2228].Int( 95,111,79,255 );  // rgb675
  pick[2229].Int( 95,111,95,255 );  // rgb676
  pick[2230].Int( 95,111,111,255 );  // rgb677
  pick[2231].Int( 95,111,127,255 );  // rgb678
  pick[2232].Int( 95,111,143,255 );  // rgb679
  pick[2233].Int( 95,111,159,255 );  // rgb67A
  pick[2234].Int( 95,111,175,255 );  // rgb67B
  pick[2235].Int( 95,111,191,255 );  // rgb67C
  pick[2236].Int( 95,111,207,255 );  // rgb67D
  pick[2237].Int( 95,111,223,255 );  // rgb67E
  pick[2238].Int( 95,111,239,255 );  // rgb67F
  pick[2239].Int( 95,127,0,255 );  // rgb680
  pick[2240].Int( 95,127,15,255 );  // rgb681
  pick[2241].Int( 95,127,31,255 );  // rgb682
  pick[2242].Int( 95,127,47,255 );  // rgb683
  pick[2243].Int( 95,127,63,255 );  // rgb684
  pick[2244].Int( 95,127,79,255 );  // rgb685
  pick[2245].Int( 95,127,95,255 );  // rgb686
  pick[2246].Int( 95,127,111,255 );  // rgb687
  pick[2247].Int( 95,127,127,255 );  // rgb688
  pick[2248].Int( 95,127,143,255 );  // rgb689
  pick[2249].Int( 95,127,159,255 );  // rgb68A
  pick[2250].Int( 95,127,175,255 );  // rgb68B
  pick[2251].Int( 95,127,191,255 );  // rgb68C
  pick[2252].Int( 95,127,207,255 );  // rgb68D
  pick[2253].Int( 95,127,223,255 );  // rgb68E
  pick[2254].Int( 95,127,239,255 );  // rgb68F
  pick[2255].Int( 95,143,0,255 );  // rgb690
  pick[2256].Int( 95,143,15,255 );  // rgb691
  pick[2257].Int( 95,143,31,255 );  // rgb692
  pick[2258].Int( 95,143,47,255 );  // rgb693
  pick[2259].Int( 95,143,63,255 );  // rgb694
  pick[2260].Int( 95,143,79,255 );  // rgb695
  pick[2261].Int( 95,143,95,255 );  // rgb696
  pick[2262].Int( 95,143,111,255 );  // rgb697
  pick[2263].Int( 95,143,127,255 );  // rgb698
  pick[2264].Int( 95,143,143,255 );  // rgb699
  pick[2265].Int( 95,143,159,255 );  // rgb69A
  pick[2266].Int( 95,143,175,255 );  // rgb69B
  pick[2267].Int( 95,143,191,255 );  // rgb69C
  pick[2268].Int( 95,143,207,255 );  // rgb69D
  pick[2269].Int( 95,143,223,255 );  // rgb69E
  pick[2270].Int( 95,143,239,255 );  // rgb69F
  pick[2271].Int( 95,159,0,255 );  // rgb6A0
  pick[2272].Int( 95,159,15,255 );  // rgb6A1
  pick[2273].Int( 95,159,31,255 );  // rgb6A2
  pick[2274].Int( 95,159,47,255 );  // rgb6A3
  pick[2275].Int( 95,159,63,255 );  // rgb6A4
  pick[2276].Int( 95,159,79,255 );  // rgb6A5
  pick[2277].Int( 95,159,95,255 );  // rgb6A6
  pick[2278].Int( 95,159,111,255 );  // rgb6A7
  pick[2279].Int( 95,159,127,255 );  // rgb6A8
  pick[2280].Int( 95,159,143,255 );  // rgb6A9
  pick[2281].Int( 95,159,159,255 );  // rgb6AA
  pick[2282].Int( 95,159,175,255 );  // rgb6AB
  pick[2283].Int( 95,159,191,255 );  // rgb6AC
  pick[2284].Int( 95,159,207,255 );  // rgb6AD
  pick[2285].Int( 95,159,223,255 );  // rgb6AE
  pick[2286].Int( 95,159,239,255 );  // rgb6AF
  pick[2287].Int( 95,175,0,255 );  // rgb6B0
  pick[2288].Int( 95,175,15,255 );  // rgb6B1
  pick[2289].Int( 95,175,31,255 );  // rgb6B2
  pick[2290].Int( 95,175,47,255 );  // rgb6B3
  pick[2291].Int( 95,175,63,255 );  // rgb6B4
  pick[2292].Int( 95,175,79,255 );  // rgb6B5
  pick[2293].Int( 95,175,95,255 );  // rgb6B6
  pick[2294].Int( 95,175,111,255 );  // rgb6B7
  pick[2295].Int( 95,175,127,255 );  // rgb6B8
  pick[2296].Int( 95,175,143,255 );  // rgb6B9
  pick[2297].Int( 95,175,159,255 );  // rgb6BA
  pick[2298].Int( 95,175,175,255 );  // rgb6BB
  pick[2299].Int( 95,175,191,255 );  // rgb6BC
  pick[2300].Int( 95,175,207,255 );  // rgb6BD
  pick[2301].Int( 95,175,223,255 );  // rgb6BE
  pick[2302].Int( 95,175,239,255 );  // rgb6BF
  pick[2303].Int( 95,191,0,255 );  // rgb6C0
  pick[2304].Int( 95,191,15,255 );  // rgb6C1
  pick[2305].Int( 95,191,31,255 );  // rgb6C2
  pick[2306].Int( 95,191,47,255 );  // rgb6C3
  pick[2307].Int( 95,191,63,255 );  // rgb6C4
  pick[2308].Int( 95,191,79,255 );  // rgb6C5
  pick[2309].Int( 95,191,95,255 );  // rgb6C6
  pick[2310].Int( 95,191,111,255 );  // rgb6C7
  pick[2311].Int( 95,191,127,255 );  // rgb6C8
  pick[2312].Int( 95,191,143,255 );  // rgb6C9
  pick[2313].Int( 95,191,159,255 );  // rgb6CA
  pick[2314].Int( 95,191,175,255 );  // rgb6CB
  pick[2315].Int( 95,191,191,255 );  // rgb6CC
  pick[2316].Int( 95,191,207,255 );  // rgb6CD
  pick[2317].Int( 95,191,223,255 );  // rgb6CE
  pick[2318].Int( 95,191,239,255 );  // rgb6CF
  pick[2319].Int( 95,207,0,255 );  // rgb6D0
  pick[2320].Int( 95,207,15,255 );  // rgb6D1
  pick[2321].Int( 95,207,31,255 );  // rgb6D2
  pick[2322].Int( 95,207,47,255 );  // rgb6D3
  pick[2323].Int( 95,207,63,255 );  // rgb6D4
  pick[2324].Int( 95,207,79,255 );  // rgb6D5
  pick[2325].Int( 95,207,95,255 );  // rgb6D6
  pick[2326].Int( 95,207,111,255 );  // rgb6D7
  pick[2327].Int( 95,207,127,255 );  // rgb6D8
  pick[2328].Int( 95,207,143,255 );  // rgb6D9
  pick[2329].Int( 95,207,159,255 );  // rgb6DA
  pick[2330].Int( 95,207,175,255 );  // rgb6DB
  pick[2331].Int( 95,207,191,255 );  // rgb6DC
  pick[2332].Int( 95,207,207,255 );  // rgb6DD
  pick[2333].Int( 95,207,223,255 );  // rgb6DE
  pick[2334].Int( 95,207,239,255 );  // rgb6DF
  pick[2335].Int( 95,223,0,255 );  // rgb6E0
  pick[2336].Int( 95,223,15,255 );  // rgb6E1
  pick[2337].Int( 95,223,31,255 );  // rgb6E2
  pick[2338].Int( 95,223,47,255 );  // rgb6E3
  pick[2339].Int( 95,223,63,255 );  // rgb6E4
  pick[2340].Int( 95,223,79,255 );  // rgb6E5
  pick[2341].Int( 95,223,95,255 );  // rgb6E6
  pick[2342].Int( 95,223,111,255 );  // rgb6E7
  pick[2343].Int( 95,223,127,255 );  // rgb6E8
  pick[2344].Int( 95,223,143,255 );  // rgb6E9
  pick[2345].Int( 95,223,159,255 );  // rgb6EA
  pick[2346].Int( 95,223,175,255 );  // rgb6EB
  pick[2347].Int( 95,223,191,255 );  // rgb6EC
  pick[2348].Int( 95,223,207,255 );  // rgb6ED
  pick[2349].Int( 95,223,223,255 );  // rgb6EE
  pick[2350].Int( 95,223,239,255 );  // rgb6EF
  pick[2351].Int( 95,239,0,255 );  // rgb6F0
  pick[2352].Int( 95,239,15,255 );  // rgb6F1
  pick[2353].Int( 95,239,31,255 );  // rgb6F2
  pick[2354].Int( 95,239,47,255 );  // rgb6F3
  pick[2355].Int( 95,239,63,255 );  // rgb6F4
  pick[2356].Int( 95,239,79,255 );  // rgb6F5
  pick[2357].Int( 95,239,95,255 );  // rgb6F6
  pick[2358].Int( 95,239,111,255 );  // rgb6F7
  pick[2359].Int( 95,239,127,255 );  // rgb6F8
  pick[2360].Int( 95,239,143,255 );  // rgb6F9
  pick[2361].Int( 95,239,159,255 );  // rgb6FA
  pick[2362].Int( 95,239,175,255 );  // rgb6FB
  pick[2363].Int( 95,239,191,255 );  // rgb6FC
  pick[2364].Int( 95,239,207,255 );  // rgb6FD
  pick[2365].Int( 95,239,223,255 );  // rgb6FE
  pick[2366].Int( 95,239,239,255 );  // rgb6FF
  pick[2367].Int( 111,0,0,255 );  // rgb700
  pick[2368].Int( 111,0,15,255 );  // rgb701
  pick[2369].Int( 111,0,31,255 );  // rgb702
  pick[2370].Int( 111,0,47,255 );  // rgb703
  pick[2371].Int( 111,0,63,255 );  // rgb704
  pick[2372].Int( 111,0,79,255 );  // rgb705
  pick[2373].Int( 111,0,95,255 );  // rgb706
  pick[2374].Int( 111,0,111,255 );  // rgb707
  pick[2375].Int( 111,0,127,255 );  // rgb708
  pick[2376].Int( 111,0,143,255 );  // rgb709
  pick[2377].Int( 111,0,159,255 );  // rgb70A
  pick[2378].Int( 111,0,175,255 );  // rgb70B
  pick[2379].Int( 111,0,191,255 );  // rgb70C
  pick[2380].Int( 111,0,207,255 );  // rgb70D
  pick[2381].Int( 111,0,223,255 );  // rgb70E
  pick[2382].Int( 111,0,239,255 );  // rgb70F
  pick[2383].Int( 111,15,0,255 );  // rgb710
  pick[2384].Int( 111,15,15,255 );  // rgb711
  pick[2385].Int( 111,15,31,255 );  // rgb712
  pick[2386].Int( 111,15,47,255 );  // rgb713
  pick[2387].Int( 111,15,63,255 );  // rgb714
  pick[2388].Int( 111,15,79,255 );  // rgb715
  pick[2389].Int( 111,15,95,255 );  // rgb716
  pick[2390].Int( 111,15,111,255 );  // rgb717
  pick[2391].Int( 111,15,127,255 );  // rgb718
  pick[2392].Int( 111,15,143,255 );  // rgb719
  pick[2393].Int( 111,15,159,255 );  // rgb71A
  pick[2394].Int( 111,15,175,255 );  // rgb71B
  pick[2395].Int( 111,15,191,255 );  // rgb71C
  pick[2396].Int( 111,15,207,255 );  // rgb71D
  pick[2397].Int( 111,15,223,255 );  // rgb71E
  pick[2398].Int( 111,15,239,255 );  // rgb71F
  pick[2399].Int( 111,31,0,255 );  // rgb720
  pick[2400].Int( 111,31,15,255 );  // rgb721
  pick[2401].Int( 111,31,31,255 );  // rgb722
  pick[2402].Int( 111,31,47,255 );  // rgb723
  pick[2403].Int( 111,31,63,255 );  // rgb724
  pick[2404].Int( 111,31,79,255 );  // rgb725
  pick[2405].Int( 111,31,95,255 );  // rgb726
  pick[2406].Int( 111,31,111,255 );  // rgb727
  pick[2407].Int( 111,31,127,255 );  // rgb728
  pick[2408].Int( 111,31,143,255 );  // rgb729
  pick[2409].Int( 111,31,159,255 );  // rgb72A
  pick[2410].Int( 111,31,175,255 );  // rgb72B
  pick[2411].Int( 111,31,191,255 );  // rgb72C
  pick[2412].Int( 111,31,207,255 );  // rgb72D
  pick[2413].Int( 111,31,223,255 );  // rgb72E
  pick[2414].Int( 111,31,239,255 );  // rgb72F
  pick[2415].Int( 111,47,0,255 );  // rgb730
  pick[2416].Int( 111,47,15,255 );  // rgb731
  pick[2417].Int( 111,47,31,255 );  // rgb732
  pick[2418].Int( 111,47,47,255 );  // rgb733
  pick[2419].Int( 111,47,63,255 );  // rgb734
  pick[2420].Int( 111,47,79,255 );  // rgb735
  pick[2421].Int( 111,47,95,255 );  // rgb736
  pick[2422].Int( 111,47,111,255 );  // rgb737
  pick[2423].Int( 111,47,127,255 );  // rgb738
  pick[2424].Int( 111,47,143,255 );  // rgb739
  pick[2425].Int( 111,47,159,255 );  // rgb73A
  pick[2426].Int( 111,47,175,255 );  // rgb73B
  pick[2427].Int( 111,47,191,255 );  // rgb73C
  pick[2428].Int( 111,47,207,255 );  // rgb73D
  pick[2429].Int( 111,47,223,255 );  // rgb73E
  pick[2430].Int( 111,47,239,255 );  // rgb73F
  pick[2431].Int( 111,63,0,255 );  // rgb740
  pick[2432].Int( 111,63,15,255 );  // rgb741
  pick[2433].Int( 111,63,31,255 );  // rgb742
  pick[2434].Int( 111,63,47,255 );  // rgb743
  pick[2435].Int( 111,63,63,255 );  // rgb744
  pick[2436].Int( 111,63,79,255 );  // rgb745
  pick[2437].Int( 111,63,95,255 );  // rgb746
  pick[2438].Int( 111,63,111,255 );  // rgb747
  pick[2439].Int( 111,63,127,255 );  // rgb748
  pick[2440].Int( 111,63,143,255 );  // rgb749
  pick[2441].Int( 111,63,159,255 );  // rgb74A
  pick[2442].Int( 111,63,175,255 );  // rgb74B
  pick[2443].Int( 111,63,191,255 );  // rgb74C
  pick[2444].Int( 111,63,207,255 );  // rgb74D
  pick[2445].Int( 111,63,223,255 );  // rgb74E
  pick[2446].Int( 111,63,239,255 );  // rgb74F
  pick[2447].Int( 111,79,0,255 );  // rgb750
  pick[2448].Int( 111,79,15,255 );  // rgb751
  pick[2449].Int( 111,79,31,255 );  // rgb752
  pick[2450].Int( 111,79,47,255 );  // rgb753
  pick[2451].Int( 111,79,63,255 );  // rgb754
  pick[2452].Int( 111,79,79,255 );  // rgb755
  pick[2453].Int( 111,79,95,255 );  // rgb756
  pick[2454].Int( 111,79,111,255 );  // rgb757
  pick[2455].Int( 111,79,127,255 );  // rgb758
  pick[2456].Int( 111,79,143,255 );  // rgb759
  pick[2457].Int( 111,79,159,255 );  // rgb75A
  pick[2458].Int( 111,79,175,255 );  // rgb75B
  pick[2459].Int( 111,79,191,255 );  // rgb75C
  pick[2460].Int( 111,79,207,255 );  // rgb75D
  pick[2461].Int( 111,79,223,255 );  // rgb75E
  pick[2462].Int( 111,79,239,255 );  // rgb75F
  pick[2463].Int( 111,95,0,255 );  // rgb760
  pick[2464].Int( 111,95,15,255 );  // rgb761
  pick[2465].Int( 111,95,31,255 );  // rgb762
  pick[2466].Int( 111,95,47,255 );  // rgb763
  pick[2467].Int( 111,95,63,255 );  // rgb764
  pick[2468].Int( 111,95,79,255 );  // rgb765
  pick[2469].Int( 111,95,95,255 );  // rgb766
  pick[2470].Int( 111,95,111,255 );  // rgb767
  pick[2471].Int( 111,95,127,255 );  // rgb768
  pick[2472].Int( 111,95,143,255 );  // rgb769
  pick[2473].Int( 111,95,159,255 );  // rgb76A
  pick[2474].Int( 111,95,175,255 );  // rgb76B
  pick[2475].Int( 111,95,191,255 );  // rgb76C
  pick[2476].Int( 111,95,207,255 );  // rgb76D
  pick[2477].Int( 111,95,223,255 );  // rgb76E
  pick[2478].Int( 111,95,239,255 );  // rgb76F
  pick[2479].Int( 111,111,0,255 );  // rgb770
  pick[2480].Int( 111,111,15,255 );  // rgb771
  pick[2481].Int( 111,111,31,255 );  // rgb772
  pick[2482].Int( 111,111,47,255 );  // rgb773
  pick[2483].Int( 111,111,63,255 );  // rgb774
  pick[2484].Int( 111,111,79,255 );  // rgb775
  pick[2485].Int( 111,111,95,255 );  // rgb776
  pick[2486].Int( 111,111,111,255 );  // rgb777
  pick[2487].Int( 111,111,127,255 );  // rgb778
  pick[2488].Int( 111,111,143,255 );  // rgb779
  pick[2489].Int( 111,111,159,255 );  // rgb77A
  pick[2490].Int( 111,111,175,255 );  // rgb77B
  pick[2491].Int( 111,111,191,255 );  // rgb77C
  pick[2492].Int( 111,111,207,255 );  // rgb77D
  pick[2493].Int( 111,111,223,255 );  // rgb77E
  pick[2494].Int( 111,111,239,255 );  // rgb77F
  pick[2495].Int( 111,127,0,255 );  // rgb780
  pick[2496].Int( 111,127,15,255 );  // rgb781
  pick[2497].Int( 111,127,31,255 );  // rgb782
  pick[2498].Int( 111,127,47,255 );  // rgb783
  pick[2499].Int( 111,127,63,255 );  // rgb784
  pick[2500].Int( 111,127,79,255 );  // rgb785
  pick[2501].Int( 111,127,95,255 );  // rgb786
  pick[2502].Int( 111,127,111,255 );  // rgb787
  pick[2503].Int( 111,127,127,255 );  // rgb788
  pick[2504].Int( 111,127,143,255 );  // rgb789
  pick[2505].Int( 111,127,159,255 );  // rgb78A
  pick[2506].Int( 111,127,175,255 );  // rgb78B
  pick[2507].Int( 111,127,191,255 );  // rgb78C
  pick[2508].Int( 111,127,207,255 );  // rgb78D
  pick[2509].Int( 111,127,223,255 );  // rgb78E
  pick[2510].Int( 111,127,239,255 );  // rgb78F
  pick[2511].Int( 111,143,0,255 );  // rgb790
  pick[2512].Int( 111,143,15,255 );  // rgb791
  pick[2513].Int( 111,143,31,255 );  // rgb792
  pick[2514].Int( 111,143,47,255 );  // rgb793
  pick[2515].Int( 111,143,63,255 );  // rgb794
  pick[2516].Int( 111,143,79,255 );  // rgb795
  pick[2517].Int( 111,143,95,255 );  // rgb796
  pick[2518].Int( 111,143,111,255 );  // rgb797
  pick[2519].Int( 111,143,127,255 );  // rgb798
  pick[2520].Int( 111,143,143,255 );  // rgb799
  pick[2521].Int( 111,143,159,255 );  // rgb79A
  pick[2522].Int( 111,143,175,255 );  // rgb79B
  pick[2523].Int( 111,143,191,255 );  // rgb79C
  pick[2524].Int( 111,143,207,255 );  // rgb79D
  pick[2525].Int( 111,143,223,255 );  // rgb79E
  pick[2526].Int( 111,143,239,255 );  // rgb79F
  pick[2527].Int( 111,159,0,255 );  // rgb7A0
  pick[2528].Int( 111,159,15,255 );  // rgb7A1
  pick[2529].Int( 111,159,31,255 );  // rgb7A2
  pick[2530].Int( 111,159,47,255 );  // rgb7A3
  pick[2531].Int( 111,159,63,255 );  // rgb7A4
  pick[2532].Int( 111,159,79,255 );  // rgb7A5
  pick[2533].Int( 111,159,95,255 );  // rgb7A6
  pick[2534].Int( 111,159,111,255 );  // rgb7A7
  pick[2535].Int( 111,159,127,255 );  // rgb7A8
  pick[2536].Int( 111,159,143,255 );  // rgb7A9
  pick[2537].Int( 111,159,159,255 );  // rgb7AA
  pick[2538].Int( 111,159,175,255 );  // rgb7AB
  pick[2539].Int( 111,159,191,255 );  // rgb7AC
  pick[2540].Int( 111,159,207,255 );  // rgb7AD
  pick[2541].Int( 111,159,223,255 );  // rgb7AE
  pick[2542].Int( 111,159,239,255 );  // rgb7AF
  pick[2543].Int( 111,175,0,255 );  // rgb7B0
  pick[2544].Int( 111,175,15,255 );  // rgb7B1
  pick[2545].Int( 111,175,31,255 );  // rgb7B2
  pick[2546].Int( 111,175,47,255 );  // rgb7B3
  pick[2547].Int( 111,175,63,255 );  // rgb7B4
  pick[2548].Int( 111,175,79,255 );  // rgb7B5
  pick[2549].Int( 111,175,95,255 );  // rgb7B6
  pick[2550].Int( 111,175,111,255 );  // rgb7B7
  pick[2551].Int( 111,175,127,255 );  // rgb7B8
  pick[2552].Int( 111,175,143,255 );  // rgb7B9
  pick[2553].Int( 111,175,159,255 );  // rgb7BA
  pick[2554].Int( 111,175,175,255 );  // rgb7BB
  pick[2555].Int( 111,175,191,255 );  // rgb7BC
  pick[2556].Int( 111,175,207,255 );  // rgb7BD
  pick[2557].Int( 111,175,223,255 );  // rgb7BE
  pick[2558].Int( 111,175,239,255 );  // rgb7BF
  pick[2559].Int( 111,191,0,255 );  // rgb7C0
  pick[2560].Int( 111,191,15,255 );  // rgb7C1
  pick[2561].Int( 111,191,31,255 );  // rgb7C2
  pick[2562].Int( 111,191,47,255 );  // rgb7C3
  pick[2563].Int( 111,191,63,255 );  // rgb7C4
  pick[2564].Int( 111,191,79,255 );  // rgb7C5
  pick[2565].Int( 111,191,95,255 );  // rgb7C6
  pick[2566].Int( 111,191,111,255 );  // rgb7C7
  pick[2567].Int( 111,191,127,255 );  // rgb7C8
  pick[2568].Int( 111,191,143,255 );  // rgb7C9
  pick[2569].Int( 111,191,159,255 );  // rgb7CA
  pick[2570].Int( 111,191,175,255 );  // rgb7CB
  pick[2571].Int( 111,191,191,255 );  // rgb7CC
  pick[2572].Int( 111,191,207,255 );  // rgb7CD
  pick[2573].Int( 111,191,223,255 );  // rgb7CE
  pick[2574].Int( 111,191,239,255 );  // rgb7CF
  pick[2575].Int( 111,207,0,255 );  // rgb7D0
  pick[2576].Int( 111,207,15,255 );  // rgb7D1
  pick[2577].Int( 111,207,31,255 );  // rgb7D2
  pick[2578].Int( 111,207,47,255 );  // rgb7D3
  pick[2579].Int( 111,207,63,255 );  // rgb7D4
  pick[2580].Int( 111,207,79,255 );  // rgb7D5
  pick[2581].Int( 111,207,95,255 );  // rgb7D6
  pick[2582].Int( 111,207,111,255 );  // rgb7D7
  pick[2583].Int( 111,207,127,255 );  // rgb7D8
  pick[2584].Int( 111,207,143,255 );  // rgb7D9
  pick[2585].Int( 111,207,159,255 );  // rgb7DA
  pick[2586].Int( 111,207,175,255 );  // rgb7DB
  pick[2587].Int( 111,207,191,255 );  // rgb7DC
  pick[2588].Int( 111,207,207,255 );  // rgb7DD
  pick[2589].Int( 111,207,223,255 );  // rgb7DE
  pick[2590].Int( 111,207,239,255 );  // rgb7DF
  pick[2591].Int( 111,223,0,255 );  // rgb7E0
  pick[2592].Int( 111,223,15,255 );  // rgb7E1
  pick[2593].Int( 111,223,31,255 );  // rgb7E2
  pick[2594].Int( 111,223,47,255 );  // rgb7E3
  pick[2595].Int( 111,223,63,255 );  // rgb7E4
  pick[2596].Int( 111,223,79,255 );  // rgb7E5
  pick[2597].Int( 111,223,95,255 );  // rgb7E6
  pick[2598].Int( 111,223,111,255 );  // rgb7E7
  pick[2599].Int( 111,223,127,255 );  // rgb7E8
  pick[2600].Int( 111,223,143,255 );  // rgb7E9
  pick[2601].Int( 111,223,159,255 );  // rgb7EA
  pick[2602].Int( 111,223,175,255 );  // rgb7EB
  pick[2603].Int( 111,223,191,255 );  // rgb7EC
  pick[2604].Int( 111,223,207,255 );  // rgb7ED
  pick[2605].Int( 111,223,223,255 );  // rgb7EE
  pick[2606].Int( 111,223,239,255 );  // rgb7EF
  pick[2607].Int( 111,239,0,255 );  // rgb7F0
  pick[2608].Int( 111,239,15,255 );  // rgb7F1
  pick[2609].Int( 111,239,31,255 );  // rgb7F2
  pick[2610].Int( 111,239,47,255 );  // rgb7F3
  pick[2611].Int( 111,239,63,255 );  // rgb7F4
  pick[2612].Int( 111,239,79,255 );  // rgb7F5
  pick[2613].Int( 111,239,95,255 );  // rgb7F6
  pick[2614].Int( 111,239,111,255 );  // rgb7F7
  pick[2615].Int( 111,239,127,255 );  // rgb7F8
  pick[2616].Int( 111,239,143,255 );  // rgb7F9
  pick[2617].Int( 111,239,159,255 );  // rgb7FA
  pick[2618].Int( 111,239,175,255 );  // rgb7FB
  pick[2619].Int( 111,239,191,255 );  // rgb7FC
  pick[2620].Int( 111,239,207,255 );  // rgb7FD
  pick[2621].Int( 111,239,223,255 );  // rgb7FE
  pick[2622].Int( 111,239,239,255 );  // rgb7FF
  pick[2623].Int( 127,0,0,255 );  // rgb800
  pick[2624].Int( 127,0,15,255 );  // rgb801
  pick[2625].Int( 127,0,31,255 );  // rgb802
  pick[2626].Int( 127,0,47,255 );  // rgb803
  pick[2627].Int( 127,0,63,255 );  // rgb804
  pick[2628].Int( 127,0,79,255 );  // rgb805
  pick[2629].Int( 127,0,95,255 );  // rgb806
  pick[2630].Int( 127,0,111,255 );  // rgb807
  pick[2631].Int( 127,0,127,255 );  // rgb808
  pick[2632].Int( 127,0,143,255 );  // rgb809
  pick[2633].Int( 127,0,159,255 );  // rgb80A
  pick[2634].Int( 127,0,175,255 );  // rgb80B
  pick[2635].Int( 127,0,191,255 );  // rgb80C
  pick[2636].Int( 127,0,207,255 );  // rgb80D
  pick[2637].Int( 127,0,223,255 );  // rgb80E
  pick[2638].Int( 127,0,239,255 );  // rgb80F
  pick[2639].Int( 127,15,0,255 );  // rgb810
  pick[2640].Int( 127,15,15,255 );  // rgb811
  pick[2641].Int( 127,15,31,255 );  // rgb812
  pick[2642].Int( 127,15,47,255 );  // rgb813
  pick[2643].Int( 127,15,63,255 );  // rgb814
  pick[2644].Int( 127,15,79,255 );  // rgb815
  pick[2645].Int( 127,15,95,255 );  // rgb816
  pick[2646].Int( 127,15,111,255 );  // rgb817
  pick[2647].Int( 127,15,127,255 );  // rgb818
  pick[2648].Int( 127,15,143,255 );  // rgb819
  pick[2649].Int( 127,15,159,255 );  // rgb81A
  pick[2650].Int( 127,15,175,255 );  // rgb81B
  pick[2651].Int( 127,15,191,255 );  // rgb81C
  pick[2652].Int( 127,15,207,255 );  // rgb81D
  pick[2653].Int( 127,15,223,255 );  // rgb81E
  pick[2654].Int( 127,15,239,255 );  // rgb81F
  pick[2655].Int( 127,31,0,255 );  // rgb820
  pick[2656].Int( 127,31,15,255 );  // rgb821
  pick[2657].Int( 127,31,31,255 );  // rgb822
  pick[2658].Int( 127,31,47,255 );  // rgb823
  pick[2659].Int( 127,31,63,255 );  // rgb824
  pick[2660].Int( 127,31,79,255 );  // rgb825
  pick[2661].Int( 127,31,95,255 );  // rgb826
  pick[2662].Int( 127,31,111,255 );  // rgb827
  pick[2663].Int( 127,31,127,255 );  // rgb828
  pick[2664].Int( 127,31,143,255 );  // rgb829
  pick[2665].Int( 127,31,159,255 );  // rgb82A
  pick[2666].Int( 127,31,175,255 );  // rgb82B
  pick[2667].Int( 127,31,191,255 );  // rgb82C
  pick[2668].Int( 127,31,207,255 );  // rgb82D
  pick[2669].Int( 127,31,223,255 );  // rgb82E
  pick[2670].Int( 127,31,239,255 );  // rgb82F
  pick[2671].Int( 127,47,0,255 );  // rgb830
  pick[2672].Int( 127,47,15,255 );  // rgb831
  pick[2673].Int( 127,47,31,255 );  // rgb832
  pick[2674].Int( 127,47,47,255 );  // rgb833
  pick[2675].Int( 127,47,63,255 );  // rgb834
  pick[2676].Int( 127,47,79,255 );  // rgb835
  pick[2677].Int( 127,47,95,255 );  // rgb836
  pick[2678].Int( 127,47,111,255 );  // rgb837
  pick[2679].Int( 127,47,127,255 );  // rgb838
  pick[2680].Int( 127,47,143,255 );  // rgb839
  pick[2681].Int( 127,47,159,255 );  // rgb83A
  pick[2682].Int( 127,47,175,255 );  // rgb83B
  pick[2683].Int( 127,47,191,255 );  // rgb83C
  pick[2684].Int( 127,47,207,255 );  // rgb83D
  pick[2685].Int( 127,47,223,255 );  // rgb83E
  pick[2686].Int( 127,47,239,255 );  // rgb83F
  pick[2687].Int( 127,63,0,255 );  // rgb840
  pick[2688].Int( 127,63,15,255 );  // rgb841
  pick[2689].Int( 127,63,31,255 );  // rgb842
  pick[2690].Int( 127,63,47,255 );  // rgb843
  pick[2691].Int( 127,63,63,255 );  // rgb844
  pick[2692].Int( 127,63,79,255 );  // rgb845
  pick[2693].Int( 127,63,95,255 );  // rgb846
  pick[2694].Int( 127,63,111,255 );  // rgb847
  pick[2695].Int( 127,63,127,255 );  // rgb848
  pick[2696].Int( 127,63,143,255 );  // rgb849
  pick[2697].Int( 127,63,159,255 );  // rgb84A
  pick[2698].Int( 127,63,175,255 );  // rgb84B
  pick[2699].Int( 127,63,191,255 );  // rgb84C
  pick[2700].Int( 127,63,207,255 );  // rgb84D
  pick[2701].Int( 127,63,223,255 );  // rgb84E
  pick[2702].Int( 127,63,239,255 );  // rgb84F
  pick[2703].Int( 127,79,0,255 );  // rgb850
  pick[2704].Int( 127,79,15,255 );  // rgb851
  pick[2705].Int( 127,79,31,255 );  // rgb852
  pick[2706].Int( 127,79,47,255 );  // rgb853
  pick[2707].Int( 127,79,63,255 );  // rgb854
  pick[2708].Int( 127,79,79,255 );  // rgb855
  pick[2709].Int( 127,79,95,255 );  // rgb856
  pick[2710].Int( 127,79,111,255 );  // rgb857
  pick[2711].Int( 127,79,127,255 );  // rgb858
  pick[2712].Int( 127,79,143,255 );  // rgb859
  pick[2713].Int( 127,79,159,255 );  // rgb85A
  pick[2714].Int( 127,79,175,255 );  // rgb85B
  pick[2715].Int( 127,79,191,255 );  // rgb85C
  pick[2716].Int( 127,79,207,255 );  // rgb85D
  pick[2717].Int( 127,79,223,255 );  // rgb85E
  pick[2718].Int( 127,79,239,255 );  // rgb85F
  pick[2719].Int( 127,95,0,255 );  // rgb860
  pick[2720].Int( 127,95,15,255 );  // rgb861
  pick[2721].Int( 127,95,31,255 );  // rgb862
  pick[2722].Int( 127,95,47,255 );  // rgb863
  pick[2723].Int( 127,95,63,255 );  // rgb864
  pick[2724].Int( 127,95,79,255 );  // rgb865
  pick[2725].Int( 127,95,95,255 );  // rgb866
  pick[2726].Int( 127,95,111,255 );  // rgb867
  pick[2727].Int( 127,95,127,255 );  // rgb868
  pick[2728].Int( 127,95,143,255 );  // rgb869
  pick[2729].Int( 127,95,159,255 );  // rgb86A
  pick[2730].Int( 127,95,175,255 );  // rgb86B
  pick[2731].Int( 127,95,191,255 );  // rgb86C
  pick[2732].Int( 127,95,207,255 );  // rgb86D
  pick[2733].Int( 127,95,223,255 );  // rgb86E
  pick[2734].Int( 127,95,239,255 );  // rgb86F
  pick[2735].Int( 127,111,0,255 );  // rgb870
  pick[2736].Int( 127,111,15,255 );  // rgb871
  pick[2737].Int( 127,111,31,255 );  // rgb872
  pick[2738].Int( 127,111,47,255 );  // rgb873
  pick[2739].Int( 127,111,63,255 );  // rgb874
  pick[2740].Int( 127,111,79,255 );  // rgb875
  pick[2741].Int( 127,111,95,255 );  // rgb876
  pick[2742].Int( 127,111,111,255 );  // rgb877
  pick[2743].Int( 127,111,127,255 );  // rgb878
  pick[2744].Int( 127,111,143,255 );  // rgb879
  pick[2745].Int( 127,111,159,255 );  // rgb87A
  pick[2746].Int( 127,111,175,255 );  // rgb87B
  pick[2747].Int( 127,111,191,255 );  // rgb87C
  pick[2748].Int( 127,111,207,255 );  // rgb87D
  pick[2749].Int( 127,111,223,255 );  // rgb87E
  pick[2750].Int( 127,111,239,255 );  // rgb87F
  pick[2751].Int( 127,127,0,255 );  // rgb880
  pick[2752].Int( 127,127,15,255 );  // rgb881
  pick[2753].Int( 127,127,31,255 );  // rgb882
  pick[2754].Int( 127,127,47,255 );  // rgb883
  pick[2755].Int( 127,127,63,255 );  // rgb884
  pick[2756].Int( 127,127,79,255 );  // rgb885
  pick[2757].Int( 127,127,95,255 );  // rgb886
  pick[2758].Int( 127,127,111,255 );  // rgb887
  pick[2759].Int( 127,127,127,255 );  // rgb888
  pick[2760].Int( 127,127,143,255 );  // rgb889
  pick[2761].Int( 127,127,159,255 );  // rgb88A
  pick[2762].Int( 127,127,175,255 );  // rgb88B
  pick[2763].Int( 127,127,191,255 );  // rgb88C
  pick[2764].Int( 127,127,207,255 );  // rgb88D
  pick[2765].Int( 127,127,223,255 );  // rgb88E
  pick[2766].Int( 127,127,239,255 );  // rgb88F
  pick[2767].Int( 127,143,0,255 );  // rgb890
  pick[2768].Int( 127,143,15,255 );  // rgb891
  pick[2769].Int( 127,143,31,255 );  // rgb892
  pick[2770].Int( 127,143,47,255 );  // rgb893
  pick[2771].Int( 127,143,63,255 );  // rgb894
  pick[2772].Int( 127,143,79,255 );  // rgb895
  pick[2773].Int( 127,143,95,255 );  // rgb896
  pick[2774].Int( 127,143,111,255 );  // rgb897
  pick[2775].Int( 127,143,127,255 );  // rgb898
  pick[2776].Int( 127,143,143,255 );  // rgb899
  pick[2777].Int( 127,143,159,255 );  // rgb89A
  pick[2778].Int( 127,143,175,255 );  // rgb89B
  pick[2779].Int( 127,143,191,255 );  // rgb89C
  pick[2780].Int( 127,143,207,255 );  // rgb89D
  pick[2781].Int( 127,143,223,255 );  // rgb89E
  pick[2782].Int( 127,143,239,255 );  // rgb89F
  pick[2783].Int( 127,159,0,255 );  // rgb8A0
  pick[2784].Int( 127,159,15,255 );  // rgb8A1
  pick[2785].Int( 127,159,31,255 );  // rgb8A2
  pick[2786].Int( 127,159,47,255 );  // rgb8A3
  pick[2787].Int( 127,159,63,255 );  // rgb8A4
  pick[2788].Int( 127,159,79,255 );  // rgb8A5
  pick[2789].Int( 127,159,95,255 );  // rgb8A6
  pick[2790].Int( 127,159,111,255 );  // rgb8A7
  pick[2791].Int( 127,159,127,255 );  // rgb8A8
  pick[2792].Int( 127,159,143,255 );  // rgb8A9
  pick[2793].Int( 127,159,159,255 );  // rgb8AA
  pick[2794].Int( 127,159,175,255 );  // rgb8AB
  pick[2795].Int( 127,159,191,255 );  // rgb8AC
  pick[2796].Int( 127,159,207,255 );  // rgb8AD
  pick[2797].Int( 127,159,223,255 );  // rgb8AE
  pick[2798].Int( 127,159,239,255 );  // rgb8AF
  pick[2799].Int( 127,175,0,255 );  // rgb8B0
  pick[2800].Int( 127,175,15,255 );  // rgb8B1
  pick[2801].Int( 127,175,31,255 );  // rgb8B2
  pick[2802].Int( 127,175,47,255 );  // rgb8B3
  pick[2803].Int( 127,175,63,255 );  // rgb8B4
  pick[2804].Int( 127,175,79,255 );  // rgb8B5
  pick[2805].Int( 127,175,95,255 );  // rgb8B6
  pick[2806].Int( 127,175,111,255 );  // rgb8B7
  pick[2807].Int( 127,175,127,255 );  // rgb8B8
  pick[2808].Int( 127,175,143,255 );  // rgb8B9
  pick[2809].Int( 127,175,159,255 );  // rgb8BA
  pick[2810].Int( 127,175,175,255 );  // rgb8BB
  pick[2811].Int( 127,175,191,255 );  // rgb8BC
  pick[2812].Int( 127,175,207,255 );  // rgb8BD
  pick[2813].Int( 127,175,223,255 );  // rgb8BE
  pick[2814].Int( 127,175,239,255 );  // rgb8BF
  pick[2815].Int( 127,191,0,255 );  // rgb8C0
  pick[2816].Int( 127,191,15,255 );  // rgb8C1
  pick[2817].Int( 127,191,31,255 );  // rgb8C2
  pick[2818].Int( 127,191,47,255 );  // rgb8C3
  pick[2819].Int( 127,191,63,255 );  // rgb8C4
  pick[2820].Int( 127,191,79,255 );  // rgb8C5
  pick[2821].Int( 127,191,95,255 );  // rgb8C6
  pick[2822].Int( 127,191,111,255 );  // rgb8C7
  pick[2823].Int( 127,191,127,255 );  // rgb8C8
  pick[2824].Int( 127,191,143,255 );  // rgb8C9
  pick[2825].Int( 127,191,159,255 );  // rgb8CA
  pick[2826].Int( 127,191,175,255 );  // rgb8CB
  pick[2827].Int( 127,191,191,255 );  // rgb8CC
  pick[2828].Int( 127,191,207,255 );  // rgb8CD
  pick[2829].Int( 127,191,223,255 );  // rgb8CE
  pick[2830].Int( 127,191,239,255 );  // rgb8CF
  pick[2831].Int( 127,207,0,255 );  // rgb8D0
  pick[2832].Int( 127,207,15,255 );  // rgb8D1
  pick[2833].Int( 127,207,31,255 );  // rgb8D2
  pick[2834].Int( 127,207,47,255 );  // rgb8D3
  pick[2835].Int( 127,207,63,255 );  // rgb8D4
  pick[2836].Int( 127,207,79,255 );  // rgb8D5
  pick[2837].Int( 127,207,95,255 );  // rgb8D6
  pick[2838].Int( 127,207,111,255 );  // rgb8D7
  pick[2839].Int( 127,207,127,255 );  // rgb8D8
  pick[2840].Int( 127,207,143,255 );  // rgb8D9
  pick[2841].Int( 127,207,159,255 );  // rgb8DA
  pick[2842].Int( 127,207,175,255 );  // rgb8DB
  pick[2843].Int( 127,207,191,255 );  // rgb8DC
  pick[2844].Int( 127,207,207,255 );  // rgb8DD
  pick[2845].Int( 127,207,223,255 );  // rgb8DE
  pick[2846].Int( 127,207,239,255 );  // rgb8DF
  pick[2847].Int( 127,223,0,255 );  // rgb8E0
  pick[2848].Int( 127,223,15,255 );  // rgb8E1
  pick[2849].Int( 127,223,31,255 );  // rgb8E2
  pick[2850].Int( 127,223,47,255 );  // rgb8E3
  pick[2851].Int( 127,223,63,255 );  // rgb8E4
  pick[2852].Int( 127,223,79,255 );  // rgb8E5
  pick[2853].Int( 127,223,95,255 );  // rgb8E6
  pick[2854].Int( 127,223,111,255 );  // rgb8E7
  pick[2855].Int( 127,223,127,255 );  // rgb8E8
  pick[2856].Int( 127,223,143,255 );  // rgb8E9
  pick[2857].Int( 127,223,159,255 );  // rgb8EA
  pick[2858].Int( 127,223,175,255 );  // rgb8EB
  pick[2859].Int( 127,223,191,255 );  // rgb8EC
  pick[2860].Int( 127,223,207,255 );  // rgb8ED
  pick[2861].Int( 127,223,223,255 );  // rgb8EE
  pick[2862].Int( 127,223,239,255 );  // rgb8EF
  pick[2863].Int( 127,239,0,255 );  // rgb8F0
  pick[2864].Int( 127,239,15,255 );  // rgb8F1
  pick[2865].Int( 127,239,31,255 );  // rgb8F2
  pick[2866].Int( 127,239,47,255 );  // rgb8F3
  pick[2867].Int( 127,239,63,255 );  // rgb8F4
  pick[2868].Int( 127,239,79,255 );  // rgb8F5
  pick[2869].Int( 127,239,95,255 );  // rgb8F6
  pick[2870].Int( 127,239,111,255 );  // rgb8F7
  pick[2871].Int( 127,239,127,255 );  // rgb8F8
  pick[2872].Int( 127,239,143,255 );  // rgb8F9
  pick[2873].Int( 127,239,159,255 );  // rgb8FA
  pick[2874].Int( 127,239,175,255 );  // rgb8FB
  pick[2875].Int( 127,239,191,255 );  // rgb8FC
  pick[2876].Int( 127,239,207,255 );  // rgb8FD
  pick[2877].Int( 127,239,223,255 );  // rgb8FE
  pick[2878].Int( 127,239,239,255 );  // rgb8FF
  pick[2879].Int( 143,0,0,255 );  // rgb900
  pick[2880].Int( 143,0,15,255 );  // rgb901
  pick[2881].Int( 143,0,31,255 );  // rgb902
  pick[2882].Int( 143,0,47,255 );  // rgb903
  pick[2883].Int( 143,0,63,255 );  // rgb904
  pick[2884].Int( 143,0,79,255 );  // rgb905
  pick[2885].Int( 143,0,95,255 );  // rgb906
  pick[2886].Int( 143,0,111,255 );  // rgb907
  pick[2887].Int( 143,0,127,255 );  // rgb908
  pick[2888].Int( 143,0,143,255 );  // rgb909
  pick[2889].Int( 143,0,159,255 );  // rgb90A
  pick[2890].Int( 143,0,175,255 );  // rgb90B
  pick[2891].Int( 143,0,191,255 );  // rgb90C
  pick[2892].Int( 143,0,207,255 );  // rgb90D
  pick[2893].Int( 143,0,223,255 );  // rgb90E
  pick[2894].Int( 143,0,239,255 );  // rgb90F
  pick[2895].Int( 143,15,0,255 );  // rgb910
  pick[2896].Int( 143,15,15,255 );  // rgb911
  pick[2897].Int( 143,15,31,255 );  // rgb912
  pick[2898].Int( 143,15,47,255 );  // rgb913
  pick[2899].Int( 143,15,63,255 );  // rgb914
  pick[2900].Int( 143,15,79,255 );  // rgb915
  pick[2901].Int( 143,15,95,255 );  // rgb916
  pick[2902].Int( 143,15,111,255 );  // rgb917
  pick[2903].Int( 143,15,127,255 );  // rgb918
  pick[2904].Int( 143,15,143,255 );  // rgb919
  pick[2905].Int( 143,15,159,255 );  // rgb91A
  pick[2906].Int( 143,15,175,255 );  // rgb91B
  pick[2907].Int( 143,15,191,255 );  // rgb91C
  pick[2908].Int( 143,15,207,255 );  // rgb91D
  pick[2909].Int( 143,15,223,255 );  // rgb91E
  pick[2910].Int( 143,15,239,255 );  // rgb91F
  pick[2911].Int( 143,31,0,255 );  // rgb920
  pick[2912].Int( 143,31,15,255 );  // rgb921
  pick[2913].Int( 143,31,31,255 );  // rgb922
  pick[2914].Int( 143,31,47,255 );  // rgb923
  pick[2915].Int( 143,31,63,255 );  // rgb924
  pick[2916].Int( 143,31,79,255 );  // rgb925
  pick[2917].Int( 143,31,95,255 );  // rgb926
  pick[2918].Int( 143,31,111,255 );  // rgb927
  pick[2919].Int( 143,31,127,255 );  // rgb928
  pick[2920].Int( 143,31,143,255 );  // rgb929
  pick[2921].Int( 143,31,159,255 );  // rgb92A
  pick[2922].Int( 143,31,175,255 );  // rgb92B
  pick[2923].Int( 143,31,191,255 );  // rgb92C
  pick[2924].Int( 143,31,207,255 );  // rgb92D
  pick[2925].Int( 143,31,223,255 );  // rgb92E
  pick[2926].Int( 143,31,239,255 );  // rgb92F
  pick[2927].Int( 143,47,0,255 );  // rgb930
  pick[2928].Int( 143,47,15,255 );  // rgb931
  pick[2929].Int( 143,47,31,255 );  // rgb932
  pick[2930].Int( 143,47,47,255 );  // rgb933
  pick[2931].Int( 143,47,63,255 );  // rgb934
  pick[2932].Int( 143,47,79,255 );  // rgb935
  pick[2933].Int( 143,47,95,255 );  // rgb936
  pick[2934].Int( 143,47,111,255 );  // rgb937
  pick[2935].Int( 143,47,127,255 );  // rgb938
  pick[2936].Int( 143,47,143,255 );  // rgb939
  pick[2937].Int( 143,47,159,255 );  // rgb93A
  pick[2938].Int( 143,47,175,255 );  // rgb93B
  pick[2939].Int( 143,47,191,255 );  // rgb93C
  pick[2940].Int( 143,47,207,255 );  // rgb93D
  pick[2941].Int( 143,47,223,255 );  // rgb93E
  pick[2942].Int( 143,47,239,255 );  // rgb93F
  pick[2943].Int( 143,63,0,255 );  // rgb940
  pick[2944].Int( 143,63,15,255 );  // rgb941
  pick[2945].Int( 143,63,31,255 );  // rgb942
  pick[2946].Int( 143,63,47,255 );  // rgb943
  pick[2947].Int( 143,63,63,255 );  // rgb944
  pick[2948].Int( 143,63,79,255 );  // rgb945
  pick[2949].Int( 143,63,95,255 );  // rgb946
  pick[2950].Int( 143,63,111,255 );  // rgb947
  pick[2951].Int( 143,63,127,255 );  // rgb948
  pick[2952].Int( 143,63,143,255 );  // rgb949
  pick[2953].Int( 143,63,159,255 );  // rgb94A
  pick[2954].Int( 143,63,175,255 );  // rgb94B
  pick[2955].Int( 143,63,191,255 );  // rgb94C
  pick[2956].Int( 143,63,207,255 );  // rgb94D
  pick[2957].Int( 143,63,223,255 );  // rgb94E
  pick[2958].Int( 143,63,239,255 );  // rgb94F
  pick[2959].Int( 143,79,0,255 );  // rgb950
  pick[2960].Int( 143,79,15,255 );  // rgb951
  pick[2961].Int( 143,79,31,255 );  // rgb952
  pick[2962].Int( 143,79,47,255 );  // rgb953
  pick[2963].Int( 143,79,63,255 );  // rgb954
  pick[2964].Int( 143,79,79,255 );  // rgb955
  pick[2965].Int( 143,79,95,255 );  // rgb956
  pick[2966].Int( 143,79,111,255 );  // rgb957
  pick[2967].Int( 143,79,127,255 );  // rgb958
  pick[2968].Int( 143,79,143,255 );  // rgb959
  pick[2969].Int( 143,79,159,255 );  // rgb95A
  pick[2970].Int( 143,79,175,255 );  // rgb95B
  pick[2971].Int( 143,79,191,255 );  // rgb95C
  pick[2972].Int( 143,79,207,255 );  // rgb95D
  pick[2973].Int( 143,79,223,255 );  // rgb95E
  pick[2974].Int( 143,79,239,255 );  // rgb95F
  pick[2975].Int( 143,95,0,255 );  // rgb960
  pick[2976].Int( 143,95,15,255 );  // rgb961
  pick[2977].Int( 143,95,31,255 );  // rgb962
  pick[2978].Int( 143,95,47,255 );  // rgb963
  pick[2979].Int( 143,95,63,255 );  // rgb964
  pick[2980].Int( 143,95,79,255 );  // rgb965
  pick[2981].Int( 143,95,95,255 );  // rgb966
  pick[2982].Int( 143,95,111,255 );  // rgb967
  pick[2983].Int( 143,95,127,255 );  // rgb968
  pick[2984].Int( 143,95,143,255 );  // rgb969
  pick[2985].Int( 143,95,159,255 );  // rgb96A
  pick[2986].Int( 143,95,175,255 );  // rgb96B
  pick[2987].Int( 143,95,191,255 );  // rgb96C
  pick[2988].Int( 143,95,207,255 );  // rgb96D
  pick[2989].Int( 143,95,223,255 );  // rgb96E
  pick[2990].Int( 143,95,239,255 );  // rgb96F
  pick[2991].Int( 143,111,0,255 );  // rgb970
  pick[2992].Int( 143,111,15,255 );  // rgb971
  pick[2993].Int( 143,111,31,255 );  // rgb972
  pick[2994].Int( 143,111,47,255 );  // rgb973
  pick[2995].Int( 143,111,63,255 );  // rgb974
  pick[2996].Int( 143,111,79,255 );  // rgb975
  pick[2997].Int( 143,111,95,255 );  // rgb976
  pick[2998].Int( 143,111,111,255 );  // rgb977
  pick[2999].Int( 143,111,127,255 );  // rgb978
  pick[3000].Int( 143,111,143,255 );  // rgb979
  pick[3001].Int( 143,111,159,255 );  // rgb97A
  pick[3002].Int( 143,111,175,255 );  // rgb97B
  pick[3003].Int( 143,111,191,255 );  // rgb97C
  pick[3004].Int( 143,111,207,255 );  // rgb97D
  pick[3005].Int( 143,111,223,255 );  // rgb97E
  pick[3006].Int( 143,111,239,255 );  // rgb97F
  pick[3007].Int( 143,127,0,255 );  // rgb980
  pick[3008].Int( 143,127,15,255 );  // rgb981
  pick[3009].Int( 143,127,31,255 );  // rgb982
  pick[3010].Int( 143,127,47,255 );  // rgb983
  pick[3011].Int( 143,127,63,255 );  // rgb984
  pick[3012].Int( 143,127,79,255 );  // rgb985
  pick[3013].Int( 143,127,95,255 );  // rgb986
  pick[3014].Int( 143,127,111,255 );  // rgb987
  pick[3015].Int( 143,127,127,255 );  // rgb988
  pick[3016].Int( 143,127,143,255 );  // rgb989
  pick[3017].Int( 143,127,159,255 );  // rgb98A
  pick[3018].Int( 143,127,175,255 );  // rgb98B
  pick[3019].Int( 143,127,191,255 );  // rgb98C
  pick[3020].Int( 143,127,207,255 );  // rgb98D
  pick[3021].Int( 143,127,223,255 );  // rgb98E
  pick[3022].Int( 143,127,239,255 );  // rgb98F
  pick[3023].Int( 143,143,0,255 );  // rgb990
  pick[3024].Int( 143,143,15,255 );  // rgb991
  pick[3025].Int( 143,143,31,255 );  // rgb992
  pick[3026].Int( 143,143,47,255 );  // rgb993
  pick[3027].Int( 143,143,63,255 );  // rgb994
  pick[3028].Int( 143,143,79,255 );  // rgb995
  pick[3029].Int( 143,143,95,255 );  // rgb996
  pick[3030].Int( 143,143,111,255 );  // rgb997
  pick[3031].Int( 143,143,127,255 );  // rgb998
  pick[3032].Int( 143,143,143,255 );  // rgb999
  pick[3033].Int( 143,143,159,255 );  // rgb99A
  pick[3034].Int( 143,143,175,255 );  // rgb99B
  pick[3035].Int( 143,143,191,255 );  // rgb99C
  pick[3036].Int( 143,143,207,255 );  // rgb99D
  pick[3037].Int( 143,143,223,255 );  // rgb99E
  pick[3038].Int( 143,143,239,255 );  // rgb99F
  pick[3039].Int( 143,159,0,255 );  // rgb9A0
  pick[3040].Int( 143,159,15,255 );  // rgb9A1
  pick[3041].Int( 143,159,31,255 );  // rgb9A2
  pick[3042].Int( 143,159,47,255 );  // rgb9A3
  pick[3043].Int( 143,159,63,255 );  // rgb9A4
  pick[3044].Int( 143,159,79,255 );  // rgb9A5
  pick[3045].Int( 143,159,95,255 );  // rgb9A6
  pick[3046].Int( 143,159,111,255 );  // rgb9A7
  pick[3047].Int( 143,159,127,255 );  // rgb9A8
  pick[3048].Int( 143,159,143,255 );  // rgb9A9
  pick[3049].Int( 143,159,159,255 );  // rgb9AA
  pick[3050].Int( 143,159,175,255 );  // rgb9AB
  pick[3051].Int( 143,159,191,255 );  // rgb9AC
  pick[3052].Int( 143,159,207,255 );  // rgb9AD
  pick[3053].Int( 143,159,223,255 );  // rgb9AE
  pick[3054].Int( 143,159,239,255 );  // rgb9AF
  pick[3055].Int( 143,175,0,255 );  // rgb9B0
  pick[3056].Int( 143,175,15,255 );  // rgb9B1
  pick[3057].Int( 143,175,31,255 );  // rgb9B2
  pick[3058].Int( 143,175,47,255 );  // rgb9B3
  pick[3059].Int( 143,175,63,255 );  // rgb9B4
  pick[3060].Int( 143,175,79,255 );  // rgb9B5
  pick[3061].Int( 143,175,95,255 );  // rgb9B6
  pick[3062].Int( 143,175,111,255 );  // rgb9B7
  pick[3063].Int( 143,175,127,255 );  // rgb9B8
  pick[3064].Int( 143,175,143,255 );  // rgb9B9
  pick[3065].Int( 143,175,159,255 );  // rgb9BA
  pick[3066].Int( 143,175,175,255 );  // rgb9BB
  pick[3067].Int( 143,175,191,255 );  // rgb9BC
  pick[3068].Int( 143,175,207,255 );  // rgb9BD
  pick[3069].Int( 143,175,223,255 );  // rgb9BE
  pick[3070].Int( 143,175,239,255 );  // rgb9BF
  pick[3071].Int( 143,191,0,255 );  // rgb9C0
  pick[3072].Int( 143,191,15,255 );  // rgb9C1
  pick[3073].Int( 143,191,31,255 );  // rgb9C2
  pick[3074].Int( 143,191,47,255 );  // rgb9C3
  pick[3075].Int( 143,191,63,255 );  // rgb9C4
  pick[3076].Int( 143,191,79,255 );  // rgb9C5
  pick[3077].Int( 143,191,95,255 );  // rgb9C6
  pick[3078].Int( 143,191,111,255 );  // rgb9C7
  pick[3079].Int( 143,191,127,255 );  // rgb9C8
  pick[3080].Int( 143,191,143,255 );  // rgb9C9
  pick[3081].Int( 143,191,159,255 );  // rgb9CA
  pick[3082].Int( 143,191,175,255 );  // rgb9CB
  pick[3083].Int( 143,191,191,255 );  // rgb9CC
  pick[3084].Int( 143,191,207,255 );  // rgb9CD
  pick[3085].Int( 143,191,223,255 );  // rgb9CE
  pick[3086].Int( 143,191,239,255 );  // rgb9CF
  pick[3087].Int( 143,207,0,255 );  // rgb9D0
  pick[3088].Int( 143,207,15,255 );  // rgb9D1
  pick[3089].Int( 143,207,31,255 );  // rgb9D2
  pick[3090].Int( 143,207,47,255 );  // rgb9D3
  pick[3091].Int( 143,207,63,255 );  // rgb9D4
  pick[3092].Int( 143,207,79,255 );  // rgb9D5
  pick[3093].Int( 143,207,95,255 );  // rgb9D6
  pick[3094].Int( 143,207,111,255 );  // rgb9D7
  pick[3095].Int( 143,207,127,255 );  // rgb9D8
  pick[3096].Int( 143,207,143,255 );  // rgb9D9
  pick[3097].Int( 143,207,159,255 );  // rgb9DA
  pick[3098].Int( 143,207,175,255 );  // rgb9DB
  pick[3099].Int( 143,207,191,255 );  // rgb9DC
  pick[3100].Int( 143,207,207,255 );  // rgb9DD
  pick[3101].Int( 143,207,223,255 );  // rgb9DE
  pick[3102].Int( 143,207,239,255 );  // rgb9DF
  pick[3103].Int( 143,223,0,255 );  // rgb9E0
  pick[3104].Int( 143,223,15,255 );  // rgb9E1
  pick[3105].Int( 143,223,31,255 );  // rgb9E2
  pick[3106].Int( 143,223,47,255 );  // rgb9E3
  pick[3107].Int( 143,223,63,255 );  // rgb9E4
  pick[3108].Int( 143,223,79,255 );  // rgb9E5
  pick[3109].Int( 143,223,95,255 );  // rgb9E6
  pick[3110].Int( 143,223,111,255 );  // rgb9E7
  pick[3111].Int( 143,223,127,255 );  // rgb9E8
  pick[3112].Int( 143,223,143,255 );  // rgb9E9
  pick[3113].Int( 143,223,159,255 );  // rgb9EA
  pick[3114].Int( 143,223,175,255 );  // rgb9EB
  pick[3115].Int( 143,223,191,255 );  // rgb9EC
  pick[3116].Int( 143,223,207,255 );  // rgb9ED
  pick[3117].Int( 143,223,223,255 );  // rgb9EE
  pick[3118].Int( 143,223,239,255 );  // rgb9EF
  pick[3119].Int( 143,239,0,255 );  // rgb9F0
  pick[3120].Int( 143,239,15,255 );  // rgb9F1
  pick[3121].Int( 143,239,31,255 );  // rgb9F2
  pick[3122].Int( 143,239,47,255 );  // rgb9F3
  pick[3123].Int( 143,239,63,255 );  // rgb9F4
  pick[3124].Int( 143,239,79,255 );  // rgb9F5
  pick[3125].Int( 143,239,95,255 );  // rgb9F6
  pick[3126].Int( 143,239,111,255 );  // rgb9F7
  pick[3127].Int( 143,239,127,255 );  // rgb9F8
  pick[3128].Int( 143,239,143,255 );  // rgb9F9
  pick[3129].Int( 143,239,159,255 );  // rgb9FA
  pick[3130].Int( 143,239,175,255 );  // rgb9FB
  pick[3131].Int( 143,239,191,255 );  // rgb9FC
  pick[3132].Int( 143,239,207,255 );  // rgb9FD
  pick[3133].Int( 143,239,223,255 );  // rgb9FE
  pick[3134].Int( 143,239,239,255 );  // rgb9FF
  pick[3135].Int( 159,0,0,255 );  // rgbA00
  pick[3136].Int( 159,0,15,255 );  // rgbA01
  pick[3137].Int( 159,0,31,255 );  // rgbA02
  pick[3138].Int( 159,0,47,255 );  // rgbA03
  pick[3139].Int( 159,0,63,255 );  // rgbA04
  pick[3140].Int( 159,0,79,255 );  // rgbA05
  pick[3141].Int( 159,0,95,255 );  // rgbA06
  pick[3142].Int( 159,0,111,255 );  // rgbA07
  pick[3143].Int( 159,0,127,255 );  // rgbA08
  pick[3144].Int( 159,0,143,255 );  // rgbA09
  pick[3145].Int( 159,0,159,255 );  // rgbA0A
  pick[3146].Int( 159,0,175,255 );  // rgbA0B
  pick[3147].Int( 159,0,191,255 );  // rgbA0C
  pick[3148].Int( 159,0,207,255 );  // rgbA0D
  pick[3149].Int( 159,0,223,255 );  // rgbA0E
  pick[3150].Int( 159,0,239,255 );  // rgbA0F
  pick[3151].Int( 159,15,0,255 );  // rgbA10
  pick[3152].Int( 159,15,15,255 );  // rgbA11
  pick[3153].Int( 159,15,31,255 );  // rgbA12
  pick[3154].Int( 159,15,47,255 );  // rgbA13
  pick[3155].Int( 159,15,63,255 );  // rgbA14
  pick[3156].Int( 159,15,79,255 );  // rgbA15
  pick[3157].Int( 159,15,95,255 );  // rgbA16
  pick[3158].Int( 159,15,111,255 );  // rgbA17
  pick[3159].Int( 159,15,127,255 );  // rgbA18
  pick[3160].Int( 159,15,143,255 );  // rgbA19
  pick[3161].Int( 159,15,159,255 );  // rgbA1A
  pick[3162].Int( 159,15,175,255 );  // rgbA1B
  pick[3163].Int( 159,15,191,255 );  // rgbA1C
  pick[3164].Int( 159,15,207,255 );  // rgbA1D
  pick[3165].Int( 159,15,223,255 );  // rgbA1E
  pick[3166].Int( 159,15,239,255 );  // rgbA1F
  pick[3167].Int( 159,31,0,255 );  // rgbA20
  pick[3168].Int( 159,31,15,255 );  // rgbA21
  pick[3169].Int( 159,31,31,255 );  // rgbA22
  pick[3170].Int( 159,31,47,255 );  // rgbA23
  pick[3171].Int( 159,31,63,255 );  // rgbA24
  pick[3172].Int( 159,31,79,255 );  // rgbA25
  pick[3173].Int( 159,31,95,255 );  // rgbA26
  pick[3174].Int( 159,31,111,255 );  // rgbA27
  pick[3175].Int( 159,31,127,255 );  // rgbA28
  pick[3176].Int( 159,31,143,255 );  // rgbA29
  pick[3177].Int( 159,31,159,255 );  // rgbA2A
  pick[3178].Int( 159,31,175,255 );  // rgbA2B
  pick[3179].Int( 159,31,191,255 );  // rgbA2C
  pick[3180].Int( 159,31,207,255 );  // rgbA2D
  pick[3181].Int( 159,31,223,255 );  // rgbA2E
  pick[3182].Int( 159,31,239,255 );  // rgbA2F
  pick[3183].Int( 159,47,0,255 );  // rgbA30
  pick[3184].Int( 159,47,15,255 );  // rgbA31
  pick[3185].Int( 159,47,31,255 );  // rgbA32
  pick[3186].Int( 159,47,47,255 );  // rgbA33
  pick[3187].Int( 159,47,63,255 );  // rgbA34
  pick[3188].Int( 159,47,79,255 );  // rgbA35
  pick[3189].Int( 159,47,95,255 );  // rgbA36
  pick[3190].Int( 159,47,111,255 );  // rgbA37
  pick[3191].Int( 159,47,127,255 );  // rgbA38
  pick[3192].Int( 159,47,143,255 );  // rgbA39
  pick[3193].Int( 159,47,159,255 );  // rgbA3A
  pick[3194].Int( 159,47,175,255 );  // rgbA3B
  pick[3195].Int( 159,47,191,255 );  // rgbA3C
  pick[3196].Int( 159,47,207,255 );  // rgbA3D
  pick[3197].Int( 159,47,223,255 );  // rgbA3E
  pick[3198].Int( 159,47,239,255 );  // rgbA3F
  pick[3199].Int( 159,63,0,255 );  // rgbA40
  pick[3200].Int( 159,63,15,255 );  // rgbA41
  pick[3201].Int( 159,63,31,255 );  // rgbA42
  pick[3202].Int( 159,63,47,255 );  // rgbA43
  pick[3203].Int( 159,63,63,255 );  // rgbA44
  pick[3204].Int( 159,63,79,255 );  // rgbA45
  pick[3205].Int( 159,63,95,255 );  // rgbA46
  pick[3206].Int( 159,63,111,255 );  // rgbA47
  pick[3207].Int( 159,63,127,255 );  // rgbA48
  pick[3208].Int( 159,63,143,255 );  // rgbA49
  pick[3209].Int( 159,63,159,255 );  // rgbA4A
  pick[3210].Int( 159,63,175,255 );  // rgbA4B
  pick[3211].Int( 159,63,191,255 );  // rgbA4C
  pick[3212].Int( 159,63,207,255 );  // rgbA4D
  pick[3213].Int( 159,63,223,255 );  // rgbA4E
  pick[3214].Int( 159,63,239,255 );  // rgbA4F
  pick[3215].Int( 159,79,0,255 );  // rgbA50
  pick[3216].Int( 159,79,15,255 );  // rgbA51
  pick[3217].Int( 159,79,31,255 );  // rgbA52
  pick[3218].Int( 159,79,47,255 );  // rgbA53
  pick[3219].Int( 159,79,63,255 );  // rgbA54
  pick[3220].Int( 159,79,79,255 );  // rgbA55
  pick[3221].Int( 159,79,95,255 );  // rgbA56
  pick[3222].Int( 159,79,111,255 );  // rgbA57
  pick[3223].Int( 159,79,127,255 );  // rgbA58
  pick[3224].Int( 159,79,143,255 );  // rgbA59
  pick[3225].Int( 159,79,159,255 );  // rgbA5A
  pick[3226].Int( 159,79,175,255 );  // rgbA5B
  pick[3227].Int( 159,79,191,255 );  // rgbA5C
  pick[3228].Int( 159,79,207,255 );  // rgbA5D
  pick[3229].Int( 159,79,223,255 );  // rgbA5E
  pick[3230].Int( 159,79,239,255 );  // rgbA5F
  pick[3231].Int( 159,95,0,255 );  // rgbA60
  pick[3232].Int( 159,95,15,255 );  // rgbA61
  pick[3233].Int( 159,95,31,255 );  // rgbA62
  pick[3234].Int( 159,95,47,255 );  // rgbA63
  pick[3235].Int( 159,95,63,255 );  // rgbA64
  pick[3236].Int( 159,95,79,255 );  // rgbA65
  pick[3237].Int( 159,95,95,255 );  // rgbA66
  pick[3238].Int( 159,95,111,255 );  // rgbA67
  pick[3239].Int( 159,95,127,255 );  // rgbA68
  pick[3240].Int( 159,95,143,255 );  // rgbA69
  pick[3241].Int( 159,95,159,255 );  // rgbA6A
  pick[3242].Int( 159,95,175,255 );  // rgbA6B
  pick[3243].Int( 159,95,191,255 );  // rgbA6C
  pick[3244].Int( 159,95,207,255 );  // rgbA6D
  pick[3245].Int( 159,95,223,255 );  // rgbA6E
  pick[3246].Int( 159,95,239,255 );  // rgbA6F
  pick[3247].Int( 159,111,0,255 );  // rgbA70
  pick[3248].Int( 159,111,15,255 );  // rgbA71
  pick[3249].Int( 159,111,31,255 );  // rgbA72
  pick[3250].Int( 159,111,47,255 );  // rgbA73
  pick[3251].Int( 159,111,63,255 );  // rgbA74
  pick[3252].Int( 159,111,79,255 );  // rgbA75
  pick[3253].Int( 159,111,95,255 );  // rgbA76
  pick[3254].Int( 159,111,111,255 );  // rgbA77
  pick[3255].Int( 159,111,127,255 );  // rgbA78
  pick[3256].Int( 159,111,143,255 );  // rgbA79
  pick[3257].Int( 159,111,159,255 );  // rgbA7A
  pick[3258].Int( 159,111,175,255 );  // rgbA7B
  pick[3259].Int( 159,111,191,255 );  // rgbA7C
  pick[3260].Int( 159,111,207,255 );  // rgbA7D
  pick[3261].Int( 159,111,223,255 );  // rgbA7E
  pick[3262].Int( 159,111,239,255 );  // rgbA7F
  pick[3263].Int( 159,127,0,255 );  // rgbA80
  pick[3264].Int( 159,127,15,255 );  // rgbA81
  pick[3265].Int( 159,127,31,255 );  // rgbA82
  pick[3266].Int( 159,127,47,255 );  // rgbA83
  pick[3267].Int( 159,127,63,255 );  // rgbA84
  pick[3268].Int( 159,127,79,255 );  // rgbA85
  pick[3269].Int( 159,127,95,255 );  // rgbA86
  pick[3270].Int( 159,127,111,255 );  // rgbA87
  pick[3271].Int( 159,127,127,255 );  // rgbA88
  pick[3272].Int( 159,127,143,255 );  // rgbA89
  pick[3273].Int( 159,127,159,255 );  // rgbA8A
  pick[3274].Int( 159,127,175,255 );  // rgbA8B
  pick[3275].Int( 159,127,191,255 );  // rgbA8C
  pick[3276].Int( 159,127,207,255 );  // rgbA8D
  pick[3277].Int( 159,127,223,255 );  // rgbA8E
  pick[3278].Int( 159,127,239,255 );  // rgbA8F
  pick[3279].Int( 159,143,0,255 );  // rgbA90
  pick[3280].Int( 159,143,15,255 );  // rgbA91
  pick[3281].Int( 159,143,31,255 );  // rgbA92
  pick[3282].Int( 159,143,47,255 );  // rgbA93
  pick[3283].Int( 159,143,63,255 );  // rgbA94
  pick[3284].Int( 159,143,79,255 );  // rgbA95
  pick[3285].Int( 159,143,95,255 );  // rgbA96
  pick[3286].Int( 159,143,111,255 );  // rgbA97
  pick[3287].Int( 159,143,127,255 );  // rgbA98
  pick[3288].Int( 159,143,143,255 );  // rgbA99
  pick[3289].Int( 159,143,159,255 );  // rgbA9A
  pick[3290].Int( 159,143,175,255 );  // rgbA9B
  pick[3291].Int( 159,143,191,255 );  // rgbA9C
  pick[3292].Int( 159,143,207,255 );  // rgbA9D
  pick[3293].Int( 159,143,223,255 );  // rgbA9E
  pick[3294].Int( 159,143,239,255 );  // rgbA9F
  pick[3295].Int( 159,159,0,255 );  // rgbAA0
  pick[3296].Int( 159,159,15,255 );  // rgbAA1
  pick[3297].Int( 159,159,31,255 );  // rgbAA2
  pick[3298].Int( 159,159,47,255 );  // rgbAA3
  pick[3299].Int( 159,159,63,255 );  // rgbAA4
  pick[3300].Int( 159,159,79,255 );  // rgbAA5
  pick[3301].Int( 159,159,95,255 );  // rgbAA6
  pick[3302].Int( 159,159,111,255 );  // rgbAA7
  pick[3303].Int( 159,159,127,255 );  // rgbAA8
  pick[3304].Int( 159,159,143,255 );  // rgbAA9
  pick[3305].Int( 159,159,159,255 );  // rgbAAA
  pick[3306].Int( 159,159,175,255 );  // rgbAAB
  pick[3307].Int( 159,159,191,255 );  // rgbAAC
  pick[3308].Int( 159,159,207,255 );  // rgbAAD
  pick[3309].Int( 159,159,223,255 );  // rgbAAE
  pick[3310].Int( 159,159,239,255 );  // rgbAAF
  pick[3311].Int( 159,175,0,255 );  // rgbAB0
  pick[3312].Int( 159,175,15,255 );  // rgbAB1
  pick[3313].Int( 159,175,31,255 );  // rgbAB2
  pick[3314].Int( 159,175,47,255 );  // rgbAB3
  pick[3315].Int( 159,175,63,255 );  // rgbAB4
  pick[3316].Int( 159,175,79,255 );  // rgbAB5
  pick[3317].Int( 159,175,95,255 );  // rgbAB6
  pick[3318].Int( 159,175,111,255 );  // rgbAB7
  pick[3319].Int( 159,175,127,255 );  // rgbAB8
  pick[3320].Int( 159,175,143,255 );  // rgbAB9
  pick[3321].Int( 159,175,159,255 );  // rgbABA
  pick[3322].Int( 159,175,175,255 );  // rgbABB
  pick[3323].Int( 159,175,191,255 );  // rgbABC
  pick[3324].Int( 159,175,207,255 );  // rgbABD
  pick[3325].Int( 159,175,223,255 );  // rgbABE
  pick[3326].Int( 159,175,239,255 );  // rgbABF
  pick[3327].Int( 159,191,0,255 );  // rgbAC0
  pick[3328].Int( 159,191,15,255 );  // rgbAC1
  pick[3329].Int( 159,191,31,255 );  // rgbAC2
  pick[3330].Int( 159,191,47,255 );  // rgbAC3
  pick[3331].Int( 159,191,63,255 );  // rgbAC4
  pick[3332].Int( 159,191,79,255 );  // rgbAC5
  pick[3333].Int( 159,191,95,255 );  // rgbAC6
  pick[3334].Int( 159,191,111,255 );  // rgbAC7
  pick[3335].Int( 159,191,127,255 );  // rgbAC8
  pick[3336].Int( 159,191,143,255 );  // rgbAC9
  pick[3337].Int( 159,191,159,255 );  // rgbACA
  pick[3338].Int( 159,191,175,255 );  // rgbACB
  pick[3339].Int( 159,191,191,255 );  // rgbACC
  pick[3340].Int( 159,191,207,255 );  // rgbACD
  pick[3341].Int( 159,191,223,255 );  // rgbACE
  pick[3342].Int( 159,191,239,255 );  // rgbACF
  pick[3343].Int( 159,207,0,255 );  // rgbAD0
  pick[3344].Int( 159,207,15,255 );  // rgbAD1
  pick[3345].Int( 159,207,31,255 );  // rgbAD2
  pick[3346].Int( 159,207,47,255 );  // rgbAD3
  pick[3347].Int( 159,207,63,255 );  // rgbAD4
  pick[3348].Int( 159,207,79,255 );  // rgbAD5
  pick[3349].Int( 159,207,95,255 );  // rgbAD6
  pick[3350].Int( 159,207,111,255 );  // rgbAD7
  pick[3351].Int( 159,207,127,255 );  // rgbAD8
  pick[3352].Int( 159,207,143,255 );  // rgbAD9
  pick[3353].Int( 159,207,159,255 );  // rgbADA
  pick[3354].Int( 159,207,175,255 );  // rgbADB
  pick[3355].Int( 159,207,191,255 );  // rgbADC
  pick[3356].Int( 159,207,207,255 );  // rgbADD
  pick[3357].Int( 159,207,223,255 );  // rgbADE
  pick[3358].Int( 159,207,239,255 );  // rgbADF
  pick[3359].Int( 159,223,0,255 );  // rgbAE0
  pick[3360].Int( 159,223,15,255 );  // rgbAE1
  pick[3361].Int( 159,223,31,255 );  // rgbAE2
  pick[3362].Int( 159,223,47,255 );  // rgbAE3
  pick[3363].Int( 159,223,63,255 );  // rgbAE4
  pick[3364].Int( 159,223,79,255 );  // rgbAE5
  pick[3365].Int( 159,223,95,255 );  // rgbAE6
  pick[3366].Int( 159,223,111,255 );  // rgbAE7
  pick[3367].Int( 159,223,127,255 );  // rgbAE8
  pick[3368].Int( 159,223,143,255 );  // rgbAE9
  pick[3369].Int( 159,223,159,255 );  // rgbAEA
  pick[3370].Int( 159,223,175,255 );  // rgbAEB
  pick[3371].Int( 159,223,191,255 );  // rgbAEC
  pick[3372].Int( 159,223,207,255 );  // rgbAED
  pick[3373].Int( 159,223,223,255 );  // rgbAEE
  pick[3374].Int( 159,223,239,255 );  // rgbAEF
  pick[3375].Int( 159,239,0,255 );  // rgbAF0
  pick[3376].Int( 159,239,15,255 );  // rgbAF1
  pick[3377].Int( 159,239,31,255 );  // rgbAF2
  pick[3378].Int( 159,239,47,255 );  // rgbAF3
  pick[3379].Int( 159,239,63,255 );  // rgbAF4
  pick[3380].Int( 159,239,79,255 );  // rgbAF5
  pick[3381].Int( 159,239,95,255 );  // rgbAF6
  pick[3382].Int( 159,239,111,255 );  // rgbAF7
  pick[3383].Int( 159,239,127,255 );  // rgbAF8
  pick[3384].Int( 159,239,143,255 );  // rgbAF9
  pick[3385].Int( 159,239,159,255 );  // rgbAFA
  pick[3386].Int( 159,239,175,255 );  // rgbAFB
  pick[3387].Int( 159,239,191,255 );  // rgbAFC
  pick[3388].Int( 159,239,207,255 );  // rgbAFD
  pick[3389].Int( 159,239,223,255 );  // rgbAFE
  pick[3390].Int( 159,239,239,255 );  // rgbAFF
  pick[3391].Int( 175,0,0,255 );  // rgbB00
  pick[3392].Int( 175,0,15,255 );  // rgbB01
  pick[3393].Int( 175,0,31,255 );  // rgbB02
  pick[3394].Int( 175,0,47,255 );  // rgbB03
  pick[3395].Int( 175,0,63,255 );  // rgbB04
  pick[3396].Int( 175,0,79,255 );  // rgbB05
  pick[3397].Int( 175,0,95,255 );  // rgbB06
  pick[3398].Int( 175,0,111,255 );  // rgbB07
  pick[3399].Int( 175,0,127,255 );  // rgbB08
  pick[3400].Int( 175,0,143,255 );  // rgbB09
  pick[3401].Int( 175,0,159,255 );  // rgbB0A
  pick[3402].Int( 175,0,175,255 );  // rgbB0B
  pick[3403].Int( 175,0,191,255 );  // rgbB0C
  pick[3404].Int( 175,0,207,255 );  // rgbB0D
  pick[3405].Int( 175,0,223,255 );  // rgbB0E
  pick[3406].Int( 175,0,239,255 );  // rgbB0F
  pick[3407].Int( 175,15,0,255 );  // rgbB10
  pick[3408].Int( 175,15,15,255 );  // rgbB11
  pick[3409].Int( 175,15,31,255 );  // rgbB12
  pick[3410].Int( 175,15,47,255 );  // rgbB13
  pick[3411].Int( 175,15,63,255 );  // rgbB14
  pick[3412].Int( 175,15,79,255 );  // rgbB15
  pick[3413].Int( 175,15,95,255 );  // rgbB16
  pick[3414].Int( 175,15,111,255 );  // rgbB17
  pick[3415].Int( 175,15,127,255 );  // rgbB18
  pick[3416].Int( 175,15,143,255 );  // rgbB19
  pick[3417].Int( 175,15,159,255 );  // rgbB1A
  pick[3418].Int( 175,15,175,255 );  // rgbB1B
  pick[3419].Int( 175,15,191,255 );  // rgbB1C
  pick[3420].Int( 175,15,207,255 );  // rgbB1D
  pick[3421].Int( 175,15,223,255 );  // rgbB1E
  pick[3422].Int( 175,15,239,255 );  // rgbB1F
  pick[3423].Int( 175,31,0,255 );  // rgbB20
  pick[3424].Int( 175,31,15,255 );  // rgbB21
  pick[3425].Int( 175,31,31,255 );  // rgbB22
  pick[3426].Int( 175,31,47,255 );  // rgbB23
  pick[3427].Int( 175,31,63,255 );  // rgbB24
  pick[3428].Int( 175,31,79,255 );  // rgbB25
  pick[3429].Int( 175,31,95,255 );  // rgbB26
  pick[3430].Int( 175,31,111,255 );  // rgbB27
  pick[3431].Int( 175,31,127,255 );  // rgbB28
  pick[3432].Int( 175,31,143,255 );  // rgbB29
  pick[3433].Int( 175,31,159,255 );  // rgbB2A
  pick[3434].Int( 175,31,175,255 );  // rgbB2B
  pick[3435].Int( 175,31,191,255 );  // rgbB2C
  pick[3436].Int( 175,31,207,255 );  // rgbB2D
  pick[3437].Int( 175,31,223,255 );  // rgbB2E
  pick[3438].Int( 175,31,239,255 );  // rgbB2F
  pick[3439].Int( 175,47,0,255 );  // rgbB30
  pick[3440].Int( 175,47,15,255 );  // rgbB31
  pick[3441].Int( 175,47,31,255 );  // rgbB32
  pick[3442].Int( 175,47,47,255 );  // rgbB33
  pick[3443].Int( 175,47,63,255 );  // rgbB34
  pick[3444].Int( 175,47,79,255 );  // rgbB35
  pick[3445].Int( 175,47,95,255 );  // rgbB36
  pick[3446].Int( 175,47,111,255 );  // rgbB37
  pick[3447].Int( 175,47,127,255 );  // rgbB38
  pick[3448].Int( 175,47,143,255 );  // rgbB39
  pick[3449].Int( 175,47,159,255 );  // rgbB3A
  pick[3450].Int( 175,47,175,255 );  // rgbB3B
  pick[3451].Int( 175,47,191,255 );  // rgbB3C
  pick[3452].Int( 175,47,207,255 );  // rgbB3D
  pick[3453].Int( 175,47,223,255 );  // rgbB3E
  pick[3454].Int( 175,47,239,255 );  // rgbB3F
  pick[3455].Int( 175,63,0,255 );  // rgbB40
  pick[3456].Int( 175,63,15,255 );  // rgbB41
  pick[3457].Int( 175,63,31,255 );  // rgbB42
  pick[3458].Int( 175,63,47,255 );  // rgbB43
  pick[3459].Int( 175,63,63,255 );  // rgbB44
  pick[3460].Int( 175,63,79,255 );  // rgbB45
  pick[3461].Int( 175,63,95,255 );  // rgbB46
  pick[3462].Int( 175,63,111,255 );  // rgbB47
  pick[3463].Int( 175,63,127,255 );  // rgbB48
  pick[3464].Int( 175,63,143,255 );  // rgbB49
  pick[3465].Int( 175,63,159,255 );  // rgbB4A
  pick[3466].Int( 175,63,175,255 );  // rgbB4B
  pick[3467].Int( 175,63,191,255 );  // rgbB4C
  pick[3468].Int( 175,63,207,255 );  // rgbB4D
  pick[3469].Int( 175,63,223,255 );  // rgbB4E
  pick[3470].Int( 175,63,239,255 );  // rgbB4F
  pick[3471].Int( 175,79,0,255 );  // rgbB50
  pick[3472].Int( 175,79,15,255 );  // rgbB51
  pick[3473].Int( 175,79,31,255 );  // rgbB52
  pick[3474].Int( 175,79,47,255 );  // rgbB53
  pick[3475].Int( 175,79,63,255 );  // rgbB54
  pick[3476].Int( 175,79,79,255 );  // rgbB55
  pick[3477].Int( 175,79,95,255 );  // rgbB56
  pick[3478].Int( 175,79,111,255 );  // rgbB57
  pick[3479].Int( 175,79,127,255 );  // rgbB58
  pick[3480].Int( 175,79,143,255 );  // rgbB59
  pick[3481].Int( 175,79,159,255 );  // rgbB5A
  pick[3482].Int( 175,79,175,255 );  // rgbB5B
  pick[3483].Int( 175,79,191,255 );  // rgbB5C
  pick[3484].Int( 175,79,207,255 );  // rgbB5D
  pick[3485].Int( 175,79,223,255 );  // rgbB5E
  pick[3486].Int( 175,79,239,255 );  // rgbB5F
  pick[3487].Int( 175,95,0,255 );  // rgbB60
  pick[3488].Int( 175,95,15,255 );  // rgbB61
  pick[3489].Int( 175,95,31,255 );  // rgbB62
  pick[3490].Int( 175,95,47,255 );  // rgbB63
  pick[3491].Int( 175,95,63,255 );  // rgbB64
  pick[3492].Int( 175,95,79,255 );  // rgbB65
  pick[3493].Int( 175,95,95,255 );  // rgbB66
  pick[3494].Int( 175,95,111,255 );  // rgbB67
  pick[3495].Int( 175,95,127,255 );  // rgbB68
  pick[3496].Int( 175,95,143,255 );  // rgbB69
  pick[3497].Int( 175,95,159,255 );  // rgbB6A
  pick[3498].Int( 175,95,175,255 );  // rgbB6B
  pick[3499].Int( 175,95,191,255 );  // rgbB6C
  pick[3500].Int( 175,95,207,255 );  // rgbB6D
  pick[3501].Int( 175,95,223,255 );  // rgbB6E
  pick[3502].Int( 175,95,239,255 );  // rgbB6F
  pick[3503].Int( 175,111,0,255 );  // rgbB70
  pick[3504].Int( 175,111,15,255 );  // rgbB71
  pick[3505].Int( 175,111,31,255 );  // rgbB72
  pick[3506].Int( 175,111,47,255 );  // rgbB73
  pick[3507].Int( 175,111,63,255 );  // rgbB74
  pick[3508].Int( 175,111,79,255 );  // rgbB75
  pick[3509].Int( 175,111,95,255 );  // rgbB76
  pick[3510].Int( 175,111,111,255 );  // rgbB77
  pick[3511].Int( 175,111,127,255 );  // rgbB78
  pick[3512].Int( 175,111,143,255 );  // rgbB79
  pick[3513].Int( 175,111,159,255 );  // rgbB7A
  pick[3514].Int( 175,111,175,255 );  // rgbB7B
  pick[3515].Int( 175,111,191,255 );  // rgbB7C
  pick[3516].Int( 175,111,207,255 );  // rgbB7D
  pick[3517].Int( 175,111,223,255 );  // rgbB7E
  pick[3518].Int( 175,111,239,255 );  // rgbB7F
  pick[3519].Int( 175,127,0,255 );  // rgbB80
  pick[3520].Int( 175,127,15,255 );  // rgbB81
  pick[3521].Int( 175,127,31,255 );  // rgbB82
  pick[3522].Int( 175,127,47,255 );  // rgbB83
  pick[3523].Int( 175,127,63,255 );  // rgbB84
  pick[3524].Int( 175,127,79,255 );  // rgbB85
  pick[3525].Int( 175,127,95,255 );  // rgbB86
  pick[3526].Int( 175,127,111,255 );  // rgbB87
  pick[3527].Int( 175,127,127,255 );  // rgbB88
  pick[3528].Int( 175,127,143,255 );  // rgbB89
  pick[3529].Int( 175,127,159,255 );  // rgbB8A
  pick[3530].Int( 175,127,175,255 );  // rgbB8B
  pick[3531].Int( 175,127,191,255 );  // rgbB8C
  pick[3532].Int( 175,127,207,255 );  // rgbB8D
  pick[3533].Int( 175,127,223,255 );  // rgbB8E
  pick[3534].Int( 175,127,239,255 );  // rgbB8F
  pick[3535].Int( 175,143,0,255 );  // rgbB90
  pick[3536].Int( 175,143,15,255 );  // rgbB91
  pick[3537].Int( 175,143,31,255 );  // rgbB92
  pick[3538].Int( 175,143,47,255 );  // rgbB93
  pick[3539].Int( 175,143,63,255 );  // rgbB94
  pick[3540].Int( 175,143,79,255 );  // rgbB95
  pick[3541].Int( 175,143,95,255 );  // rgbB96
  pick[3542].Int( 175,143,111,255 );  // rgbB97
  pick[3543].Int( 175,143,127,255 );  // rgbB98
  pick[3544].Int( 175,143,143,255 );  // rgbB99
  pick[3545].Int( 175,143,159,255 );  // rgbB9A
  pick[3546].Int( 175,143,175,255 );  // rgbB9B
  pick[3547].Int( 175,143,191,255 );  // rgbB9C
  pick[3548].Int( 175,143,207,255 );  // rgbB9D
  pick[3549].Int( 175,143,223,255 );  // rgbB9E
  pick[3550].Int( 175,143,239,255 );  // rgbB9F
  pick[3551].Int( 175,159,0,255 );  // rgbBA0
  pick[3552].Int( 175,159,15,255 );  // rgbBA1
  pick[3553].Int( 175,159,31,255 );  // rgbBA2
  pick[3554].Int( 175,159,47,255 );  // rgbBA3
  pick[3555].Int( 175,159,63,255 );  // rgbBA4
  pick[3556].Int( 175,159,79,255 );  // rgbBA5
  pick[3557].Int( 175,159,95,255 );  // rgbBA6
  pick[3558].Int( 175,159,111,255 );  // rgbBA7
  pick[3559].Int( 175,159,127,255 );  // rgbBA8
  pick[3560].Int( 175,159,143,255 );  // rgbBA9
  pick[3561].Int( 175,159,159,255 );  // rgbBAA
  pick[3562].Int( 175,159,175,255 );  // rgbBAB
  pick[3563].Int( 175,159,191,255 );  // rgbBAC
  pick[3564].Int( 175,159,207,255 );  // rgbBAD
  pick[3565].Int( 175,159,223,255 );  // rgbBAE
  pick[3566].Int( 175,159,239,255 );  // rgbBAF
  pick[3567].Int( 175,175,0,255 );  // rgbBB0
  pick[3568].Int( 175,175,15,255 );  // rgbBB1
  pick[3569].Int( 175,175,31,255 );  // rgbBB2
  pick[3570].Int( 175,175,47,255 );  // rgbBB3
  pick[3571].Int( 175,175,63,255 );  // rgbBB4
  pick[3572].Int( 175,175,79,255 );  // rgbBB5
  pick[3573].Int( 175,175,95,255 );  // rgbBB6
  pick[3574].Int( 175,175,111,255 );  // rgbBB7
  pick[3575].Int( 175,175,127,255 );  // rgbBB8
  pick[3576].Int( 175,175,143,255 );  // rgbBB9
  pick[3577].Int( 175,175,159,255 );  // rgbBBA
  pick[3578].Int( 175,175,175,255 );  // rgbBBB
  pick[3579].Int( 175,175,191,255 );  // rgbBBC
  pick[3580].Int( 175,175,207,255 );  // rgbBBD
  pick[3581].Int( 175,175,223,255 );  // rgbBBE
  pick[3582].Int( 175,175,239,255 );  // rgbBBF
  pick[3583].Int( 175,191,0,255 );  // rgbBC0
  pick[3584].Int( 175,191,15,255 );  // rgbBC1
  pick[3585].Int( 175,191,31,255 );  // rgbBC2
  pick[3586].Int( 175,191,47,255 );  // rgbBC3
  pick[3587].Int( 175,191,63,255 );  // rgbBC4
  pick[3588].Int( 175,191,79,255 );  // rgbBC5
  pick[3589].Int( 175,191,95,255 );  // rgbBC6
  pick[3590].Int( 175,191,111,255 );  // rgbBC7
  pick[3591].Int( 175,191,127,255 );  // rgbBC8
  pick[3592].Int( 175,191,143,255 );  // rgbBC9
  pick[3593].Int( 175,191,159,255 );  // rgbBCA
  pick[3594].Int( 175,191,175,255 );  // rgbBCB
  pick[3595].Int( 175,191,191,255 );  // rgbBCC
  pick[3596].Int( 175,191,207,255 );  // rgbBCD
  pick[3597].Int( 175,191,223,255 );  // rgbBCE
  pick[3598].Int( 175,191,239,255 );  // rgbBCF
  pick[3599].Int( 175,207,0,255 );  // rgbBD0
  pick[3600].Int( 175,207,15,255 );  // rgbBD1
  pick[3601].Int( 175,207,31,255 );  // rgbBD2
  pick[3602].Int( 175,207,47,255 );  // rgbBD3
  pick[3603].Int( 175,207,63,255 );  // rgbBD4
  pick[3604].Int( 175,207,79,255 );  // rgbBD5
  pick[3605].Int( 175,207,95,255 );  // rgbBD6
  pick[3606].Int( 175,207,111,255 );  // rgbBD7
  pick[3607].Int( 175,207,127,255 );  // rgbBD8
  pick[3608].Int( 175,207,143,255 );  // rgbBD9
  pick[3609].Int( 175,207,159,255 );  // rgbBDA
  pick[3610].Int( 175,207,175,255 );  // rgbBDB
  pick[3611].Int( 175,207,191,255 );  // rgbBDC
  pick[3612].Int( 175,207,207,255 );  // rgbBDD
  pick[3613].Int( 175,207,223,255 );  // rgbBDE
  pick[3614].Int( 175,207,239,255 );  // rgbBDF
  pick[3615].Int( 175,223,0,255 );  // rgbBE0
  pick[3616].Int( 175,223,15,255 );  // rgbBE1
  pick[3617].Int( 175,223,31,255 );  // rgbBE2
  pick[3618].Int( 175,223,47,255 );  // rgbBE3
  pick[3619].Int( 175,223,63,255 );  // rgbBE4
  pick[3620].Int( 175,223,79,255 );  // rgbBE5
  pick[3621].Int( 175,223,95,255 );  // rgbBE6
  pick[3622].Int( 175,223,111,255 );  // rgbBE7
  pick[3623].Int( 175,223,127,255 );  // rgbBE8
  pick[3624].Int( 175,223,143,255 );  // rgbBE9
  pick[3625].Int( 175,223,159,255 );  // rgbBEA
  pick[3626].Int( 175,223,175,255 );  // rgbBEB
  pick[3627].Int( 175,223,191,255 );  // rgbBEC
  pick[3628].Int( 175,223,207,255 );  // rgbBED
  pick[3629].Int( 175,223,223,255 );  // rgbBEE
  pick[3630].Int( 175,223,239,255 );  // rgbBEF
  pick[3631].Int( 175,239,0,255 );  // rgbBF0
  pick[3632].Int( 175,239,15,255 );  // rgbBF1
  pick[3633].Int( 175,239,31,255 );  // rgbBF2
  pick[3634].Int( 175,239,47,255 );  // rgbBF3
  pick[3635].Int( 175,239,63,255 );  // rgbBF4
  pick[3636].Int( 175,239,79,255 );  // rgbBF5
  pick[3637].Int( 175,239,95,255 );  // rgbBF6
  pick[3638].Int( 175,239,111,255 );  // rgbBF7
  pick[3639].Int( 175,239,127,255 );  // rgbBF8
  pick[3640].Int( 175,239,143,255 );  // rgbBF9
  pick[3641].Int( 175,239,159,255 );  // rgbBFA
  pick[3642].Int( 175,239,175,255 );  // rgbBFB
  pick[3643].Int( 175,239,191,255 );  // rgbBFC
  pick[3644].Int( 175,239,207,255 );  // rgbBFD
  pick[3645].Int( 175,239,223,255 );  // rgbBFE
  pick[3646].Int( 175,239,239,255 );  // rgbBFF
  pick[3647].Int( 191,0,0,255 );  // rgbC00
  pick[3648].Int( 191,0,15,255 );  // rgbC01
  pick[3649].Int( 191,0,31,255 );  // rgbC02
  pick[3650].Int( 191,0,47,255 );  // rgbC03
  pick[3651].Int( 191,0,63,255 );  // rgbC04
  pick[3652].Int( 191,0,79,255 );  // rgbC05
  pick[3653].Int( 191,0,95,255 );  // rgbC06
  pick[3654].Int( 191,0,111,255 );  // rgbC07
  pick[3655].Int( 191,0,127,255 );  // rgbC08
  pick[3656].Int( 191,0,143,255 );  // rgbC09
  pick[3657].Int( 191,0,159,255 );  // rgbC0A
  pick[3658].Int( 191,0,175,255 );  // rgbC0B
  pick[3659].Int( 191,0,191,255 );  // rgbC0C
  pick[3660].Int( 191,0,207,255 );  // rgbC0D
  pick[3661].Int( 191,0,223,255 );  // rgbC0E
  pick[3662].Int( 191,0,239,255 );  // rgbC0F
  pick[3663].Int( 191,15,0,255 );  // rgbC10
  pick[3664].Int( 191,15,15,255 );  // rgbC11
  pick[3665].Int( 191,15,31,255 );  // rgbC12
  pick[3666].Int( 191,15,47,255 );  // rgbC13
  pick[3667].Int( 191,15,63,255 );  // rgbC14
  pick[3668].Int( 191,15,79,255 );  // rgbC15
  pick[3669].Int( 191,15,95,255 );  // rgbC16
  pick[3670].Int( 191,15,111,255 );  // rgbC17
  pick[3671].Int( 191,15,127,255 );  // rgbC18
  pick[3672].Int( 191,15,143,255 );  // rgbC19
  pick[3673].Int( 191,15,159,255 );  // rgbC1A
  pick[3674].Int( 191,15,175,255 );  // rgbC1B
  pick[3675].Int( 191,15,191,255 );  // rgbC1C
  pick[3676].Int( 191,15,207,255 );  // rgbC1D
  pick[3677].Int( 191,15,223,255 );  // rgbC1E
  pick[3678].Int( 191,15,239,255 );  // rgbC1F
  pick[3679].Int( 191,31,0,255 );  // rgbC20
  pick[3680].Int( 191,31,15,255 );  // rgbC21
  pick[3681].Int( 191,31,31,255 );  // rgbC22
  pick[3682].Int( 191,31,47,255 );  // rgbC23
  pick[3683].Int( 191,31,63,255 );  // rgbC24
  pick[3684].Int( 191,31,79,255 );  // rgbC25
  pick[3685].Int( 191,31,95,255 );  // rgbC26
  pick[3686].Int( 191,31,111,255 );  // rgbC27
  pick[3687].Int( 191,31,127,255 );  // rgbC28
  pick[3688].Int( 191,31,143,255 );  // rgbC29
  pick[3689].Int( 191,31,159,255 );  // rgbC2A
  pick[3690].Int( 191,31,175,255 );  // rgbC2B
  pick[3691].Int( 191,31,191,255 );  // rgbC2C
  pick[3692].Int( 191,31,207,255 );  // rgbC2D
  pick[3693].Int( 191,31,223,255 );  // rgbC2E
  pick[3694].Int( 191,31,239,255 );  // rgbC2F
  pick[3695].Int( 191,47,0,255 );  // rgbC30
  pick[3696].Int( 191,47,15,255 );  // rgbC31
  pick[3697].Int( 191,47,31,255 );  // rgbC32
  pick[3698].Int( 191,47,47,255 );  // rgbC33
  pick[3699].Int( 191,47,63,255 );  // rgbC34
  pick[3700].Int( 191,47,79,255 );  // rgbC35
  pick[3701].Int( 191,47,95,255 );  // rgbC36
  pick[3702].Int( 191,47,111,255 );  // rgbC37
  pick[3703].Int( 191,47,127,255 );  // rgbC38
  pick[3704].Int( 191,47,143,255 );  // rgbC39
  pick[3705].Int( 191,47,159,255 );  // rgbC3A
  pick[3706].Int( 191,47,175,255 );  // rgbC3B
  pick[3707].Int( 191,47,191,255 );  // rgbC3C
  pick[3708].Int( 191,47,207,255 );  // rgbC3D
  pick[3709].Int( 191,47,223,255 );  // rgbC3E
  pick[3710].Int( 191,47,239,255 );  // rgbC3F
  pick[3711].Int( 191,63,0,255 );  // rgbC40
  pick[3712].Int( 191,63,15,255 );  // rgbC41
  pick[3713].Int( 191,63,31,255 );  // rgbC42
  pick[3714].Int( 191,63,47,255 );  // rgbC43
  pick[3715].Int( 191,63,63,255 );  // rgbC44
  pick[3716].Int( 191,63,79,255 );  // rgbC45
  pick[3717].Int( 191,63,95,255 );  // rgbC46
  pick[3718].Int( 191,63,111,255 );  // rgbC47
  pick[3719].Int( 191,63,127,255 );  // rgbC48
  pick[3720].Int( 191,63,143,255 );  // rgbC49
  pick[3721].Int( 191,63,159,255 );  // rgbC4A
  pick[3722].Int( 191,63,175,255 );  // rgbC4B
  pick[3723].Int( 191,63,191,255 );  // rgbC4C
  pick[3724].Int( 191,63,207,255 );  // rgbC4D
  pick[3725].Int( 191,63,223,255 );  // rgbC4E
  pick[3726].Int( 191,63,239,255 );  // rgbC4F
  pick[3727].Int( 191,79,0,255 );  // rgbC50
  pick[3728].Int( 191,79,15,255 );  // rgbC51
  pick[3729].Int( 191,79,31,255 );  // rgbC52
  pick[3730].Int( 191,79,47,255 );  // rgbC53
  pick[3731].Int( 191,79,63,255 );  // rgbC54
  pick[3732].Int( 191,79,79,255 );  // rgbC55
  pick[3733].Int( 191,79,95,255 );  // rgbC56
  pick[3734].Int( 191,79,111,255 );  // rgbC57
  pick[3735].Int( 191,79,127,255 );  // rgbC58
  pick[3736].Int( 191,79,143,255 );  // rgbC59
  pick[3737].Int( 191,79,159,255 );  // rgbC5A
  pick[3738].Int( 191,79,175,255 );  // rgbC5B
  pick[3739].Int( 191,79,191,255 );  // rgbC5C
  pick[3740].Int( 191,79,207,255 );  // rgbC5D
  pick[3741].Int( 191,79,223,255 );  // rgbC5E
  pick[3742].Int( 191,79,239,255 );  // rgbC5F
  pick[3743].Int( 191,95,0,255 );  // rgbC60
  pick[3744].Int( 191,95,15,255 );  // rgbC61
  pick[3745].Int( 191,95,31,255 );  // rgbC62
  pick[3746].Int( 191,95,47,255 );  // rgbC63
  pick[3747].Int( 191,95,63,255 );  // rgbC64
  pick[3748].Int( 191,95,79,255 );  // rgbC65
  pick[3749].Int( 191,95,95,255 );  // rgbC66
  pick[3750].Int( 191,95,111,255 );  // rgbC67
  pick[3751].Int( 191,95,127,255 );  // rgbC68
  pick[3752].Int( 191,95,143,255 );  // rgbC69
  pick[3753].Int( 191,95,159,255 );  // rgbC6A
  pick[3754].Int( 191,95,175,255 );  // rgbC6B
  pick[3755].Int( 191,95,191,255 );  // rgbC6C
  pick[3756].Int( 191,95,207,255 );  // rgbC6D
  pick[3757].Int( 191,95,223,255 );  // rgbC6E
  pick[3758].Int( 191,95,239,255 );  // rgbC6F
  pick[3759].Int( 191,111,0,255 );  // rgbC70
  pick[3760].Int( 191,111,15,255 );  // rgbC71
  pick[3761].Int( 191,111,31,255 );  // rgbC72
  pick[3762].Int( 191,111,47,255 );  // rgbC73
  pick[3763].Int( 191,111,63,255 );  // rgbC74
  pick[3764].Int( 191,111,79,255 );  // rgbC75
  pick[3765].Int( 191,111,95,255 );  // rgbC76
  pick[3766].Int( 191,111,111,255 );  // rgbC77
  pick[3767].Int( 191,111,127,255 );  // rgbC78
  pick[3768].Int( 191,111,143,255 );  // rgbC79
  pick[3769].Int( 191,111,159,255 );  // rgbC7A
  pick[3770].Int( 191,111,175,255 );  // rgbC7B
  pick[3771].Int( 191,111,191,255 );  // rgbC7C
  pick[3772].Int( 191,111,207,255 );  // rgbC7D
  pick[3773].Int( 191,111,223,255 );  // rgbC7E
  pick[3774].Int( 191,111,239,255 );  // rgbC7F
  pick[3775].Int( 191,127,0,255 );  // rgbC80
  pick[3776].Int( 191,127,15,255 );  // rgbC81
  pick[3777].Int( 191,127,31,255 );  // rgbC82
  pick[3778].Int( 191,127,47,255 );  // rgbC83
  pick[3779].Int( 191,127,63,255 );  // rgbC84
  pick[3780].Int( 191,127,79,255 );  // rgbC85
  pick[3781].Int( 191,127,95,255 );  // rgbC86
  pick[3782].Int( 191,127,111,255 );  // rgbC87
  pick[3783].Int( 191,127,127,255 );  // rgbC88
  pick[3784].Int( 191,127,143,255 );  // rgbC89
  pick[3785].Int( 191,127,159,255 );  // rgbC8A
  pick[3786].Int( 191,127,175,255 );  // rgbC8B
  pick[3787].Int( 191,127,191,255 );  // rgbC8C
  pick[3788].Int( 191,127,207,255 );  // rgbC8D
  pick[3789].Int( 191,127,223,255 );  // rgbC8E
  pick[3790].Int( 191,127,239,255 );  // rgbC8F
  pick[3791].Int( 191,143,0,255 );  // rgbC90
  pick[3792].Int( 191,143,15,255 );  // rgbC91
  pick[3793].Int( 191,143,31,255 );  // rgbC92
  pick[3794].Int( 191,143,47,255 );  // rgbC93
  pick[3795].Int( 191,143,63,255 );  // rgbC94
  pick[3796].Int( 191,143,79,255 );  // rgbC95
  pick[3797].Int( 191,143,95,255 );  // rgbC96
  pick[3798].Int( 191,143,111,255 );  // rgbC97
  pick[3799].Int( 191,143,127,255 );  // rgbC98
  pick[3800].Int( 191,143,143,255 );  // rgbC99
  pick[3801].Int( 191,143,159,255 );  // rgbC9A
  pick[3802].Int( 191,143,175,255 );  // rgbC9B
  pick[3803].Int( 191,143,191,255 );  // rgbC9C
  pick[3804].Int( 191,143,207,255 );  // rgbC9D
  pick[3805].Int( 191,143,223,255 );  // rgbC9E
  pick[3806].Int( 191,143,239,255 );  // rgbC9F
  pick[3807].Int( 191,159,0,255 );  // rgbCA0
  pick[3808].Int( 191,159,15,255 );  // rgbCA1
  pick[3809].Int( 191,159,31,255 );  // rgbCA2
  pick[3810].Int( 191,159,47,255 );  // rgbCA3
  pick[3811].Int( 191,159,63,255 );  // rgbCA4
  pick[3812].Int( 191,159,79,255 );  // rgbCA5
  pick[3813].Int( 191,159,95,255 );  // rgbCA6
  pick[3814].Int( 191,159,111,255 );  // rgbCA7
  pick[3815].Int( 191,159,127,255 );  // rgbCA8
  pick[3816].Int( 191,159,143,255 );  // rgbCA9
  pick[3817].Int( 191,159,159,255 );  // rgbCAA
  pick[3818].Int( 191,159,175,255 );  // rgbCAB
  pick[3819].Int( 191,159,191,255 );  // rgbCAC
  pick[3820].Int( 191,159,207,255 );  // rgbCAD
  pick[3821].Int( 191,159,223,255 );  // rgbCAE
  pick[3822].Int( 191,159,239,255 );  // rgbCAF
  pick[3823].Int( 191,175,0,255 );  // rgbCB0
  pick[3824].Int( 191,175,15,255 );  // rgbCB1
  pick[3825].Int( 191,175,31,255 );  // rgbCB2
  pick[3826].Int( 191,175,47,255 );  // rgbCB3
  pick[3827].Int( 191,175,63,255 );  // rgbCB4
  pick[3828].Int( 191,175,79,255 );  // rgbCB5
  pick[3829].Int( 191,175,95,255 );  // rgbCB6
  pick[3830].Int( 191,175,111,255 );  // rgbCB7
  pick[3831].Int( 191,175,127,255 );  // rgbCB8
  pick[3832].Int( 191,175,143,255 );  // rgbCB9
  pick[3833].Int( 191,175,159,255 );  // rgbCBA
  pick[3834].Int( 191,175,175,255 );  // rgbCBB
  pick[3835].Int( 191,175,191,255 );  // rgbCBC
  pick[3836].Int( 191,175,207,255 );  // rgbCBD
  pick[3837].Int( 191,175,223,255 );  // rgbCBE
  pick[3838].Int( 191,175,239,255 );  // rgbCBF
  pick[3839].Int( 191,191,0,255 );  // rgbCC0
  pick[3840].Int( 191,191,15,255 );  // rgbCC1
  pick[3841].Int( 191,191,31,255 );  // rgbCC2
  pick[3842].Int( 191,191,47,255 );  // rgbCC3
  pick[3843].Int( 191,191,63,255 );  // rgbCC4
  pick[3844].Int( 191,191,79,255 );  // rgbCC5
  pick[3845].Int( 191,191,95,255 );  // rgbCC6
  pick[3846].Int( 191,191,111,255 );  // rgbCC7
  pick[3847].Int( 191,191,127,255 );  // rgbCC8
  pick[3848].Int( 191,191,143,255 );  // rgbCC9
  pick[3849].Int( 191,191,159,255 );  // rgbCCA
  pick[3850].Int( 191,191,175,255 );  // rgbCCB
  pick[3851].Int( 191,191,191,255 );  // rgbCCC
  pick[3852].Int( 191,191,207,255 );  // rgbCCD
  pick[3853].Int( 191,191,223,255 );  // rgbCCE
  pick[3854].Int( 191,191,239,255 );  // rgbCCF
  pick[3855].Int( 191,207,0,255 );  // rgbCD0
  pick[3856].Int( 191,207,15,255 );  // rgbCD1
  pick[3857].Int( 191,207,31,255 );  // rgbCD2
  pick[3858].Int( 191,207,47,255 );  // rgbCD3
  pick[3859].Int( 191,207,63,255 );  // rgbCD4
  pick[3860].Int( 191,207,79,255 );  // rgbCD5
  pick[3861].Int( 191,207,95,255 );  // rgbCD6
  pick[3862].Int( 191,207,111,255 );  // rgbCD7
  pick[3863].Int( 191,207,127,255 );  // rgbCD8
  pick[3864].Int( 191,207,143,255 );  // rgbCD9
  pick[3865].Int( 191,207,159,255 );  // rgbCDA
  pick[3866].Int( 191,207,175,255 );  // rgbCDB
  pick[3867].Int( 191,207,191,255 );  // rgbCDC
  pick[3868].Int( 191,207,207,255 );  // rgbCDD
  pick[3869].Int( 191,207,223,255 );  // rgbCDE
  pick[3870].Int( 191,207,239,255 );  // rgbCDF
  pick[3871].Int( 191,223,0,255 );  // rgbCE0
  pick[3872].Int( 191,223,15,255 );  // rgbCE1
  pick[3873].Int( 191,223,31,255 );  // rgbCE2
  pick[3874].Int( 191,223,47,255 );  // rgbCE3
  pick[3875].Int( 191,223,63,255 );  // rgbCE4
  pick[3876].Int( 191,223,79,255 );  // rgbCE5
  pick[3877].Int( 191,223,95,255 );  // rgbCE6
  pick[3878].Int( 191,223,111,255 );  // rgbCE7
  pick[3879].Int( 191,223,127,255 );  // rgbCE8
  pick[3880].Int( 191,223,143,255 );  // rgbCE9
  pick[3881].Int( 191,223,159,255 );  // rgbCEA
  pick[3882].Int( 191,223,175,255 );  // rgbCEB
  pick[3883].Int( 191,223,191,255 );  // rgbCEC
  pick[3884].Int( 191,223,207,255 );  // rgbCED
  pick[3885].Int( 191,223,223,255 );  // rgbCEE
  pick[3886].Int( 191,223,239,255 );  // rgbCEF
  pick[3887].Int( 191,239,0,255 );  // rgbCF0
  pick[3888].Int( 191,239,15,255 );  // rgbCF1
  pick[3889].Int( 191,239,31,255 );  // rgbCF2
  pick[3890].Int( 191,239,47,255 );  // rgbCF3
  pick[3891].Int( 191,239,63,255 );  // rgbCF4
  pick[3892].Int( 191,239,79,255 );  // rgbCF5
  pick[3893].Int( 191,239,95,255 );  // rgbCF6
  pick[3894].Int( 191,239,111,255 );  // rgbCF7
  pick[3895].Int( 191,239,127,255 );  // rgbCF8
  pick[3896].Int( 191,239,143,255 );  // rgbCF9
  pick[3897].Int( 191,239,159,255 );  // rgbCFA
  pick[3898].Int( 191,239,175,255 );  // rgbCFB
  pick[3899].Int( 191,239,191,255 );  // rgbCFC
  pick[3900].Int( 191,239,207,255 );  // rgbCFD
  pick[3901].Int( 191,239,223,255 );  // rgbCFE
  pick[3902].Int( 191,239,239,255 );  // rgbCFF
  pick[3903].Int( 207,0,0,255 );  // rgbD00
  pick[3904].Int( 207,0,15,255 );  // rgbD01
  pick[3905].Int( 207,0,31,255 );  // rgbD02
  pick[3906].Int( 207,0,47,255 );  // rgbD03
  pick[3907].Int( 207,0,63,255 );  // rgbD04
  pick[3908].Int( 207,0,79,255 );  // rgbD05
  pick[3909].Int( 207,0,95,255 );  // rgbD06
  pick[3910].Int( 207,0,111,255 );  // rgbD07
  pick[3911].Int( 207,0,127,255 );  // rgbD08
  pick[3912].Int( 207,0,143,255 );  // rgbD09
  pick[3913].Int( 207,0,159,255 );  // rgbD0A
  pick[3914].Int( 207,0,175,255 );  // rgbD0B
  pick[3915].Int( 207,0,191,255 );  // rgbD0C
  pick[3916].Int( 207,0,207,255 );  // rgbD0D
  pick[3917].Int( 207,0,223,255 );  // rgbD0E
  pick[3918].Int( 207,0,239,255 );  // rgbD0F
  pick[3919].Int( 207,15,0,255 );  // rgbD10
  pick[3920].Int( 207,15,15,255 );  // rgbD11
  pick[3921].Int( 207,15,31,255 );  // rgbD12
  pick[3922].Int( 207,15,47,255 );  // rgbD13
  pick[3923].Int( 207,15,63,255 );  // rgbD14
  pick[3924].Int( 207,15,79,255 );  // rgbD15
  pick[3925].Int( 207,15,95,255 );  // rgbD16
  pick[3926].Int( 207,15,111,255 );  // rgbD17
  pick[3927].Int( 207,15,127,255 );  // rgbD18
  pick[3928].Int( 207,15,143,255 );  // rgbD19
  pick[3929].Int( 207,15,159,255 );  // rgbD1A
  pick[3930].Int( 207,15,175,255 );  // rgbD1B
  pick[3931].Int( 207,15,191,255 );  // rgbD1C
  pick[3932].Int( 207,15,207,255 );  // rgbD1D
  pick[3933].Int( 207,15,223,255 );  // rgbD1E
  pick[3934].Int( 207,15,239,255 );  // rgbD1F
  pick[3935].Int( 207,31,0,255 );  // rgbD20
  pick[3936].Int( 207,31,15,255 );  // rgbD21
  pick[3937].Int( 207,31,31,255 );  // rgbD22
  pick[3938].Int( 207,31,47,255 );  // rgbD23
  pick[3939].Int( 207,31,63,255 );  // rgbD24
  pick[3940].Int( 207,31,79,255 );  // rgbD25
  pick[3941].Int( 207,31,95,255 );  // rgbD26
  pick[3942].Int( 207,31,111,255 );  // rgbD27
  pick[3943].Int( 207,31,127,255 );  // rgbD28
  pick[3944].Int( 207,31,143,255 );  // rgbD29
  pick[3945].Int( 207,31,159,255 );  // rgbD2A
  pick[3946].Int( 207,31,175,255 );  // rgbD2B
  pick[3947].Int( 207,31,191,255 );  // rgbD2C
  pick[3948].Int( 207,31,207,255 );  // rgbD2D
  pick[3949].Int( 207,31,223,255 );  // rgbD2E
  pick[3950].Int( 207,31,239,255 );  // rgbD2F
  pick[3951].Int( 207,47,0,255 );  // rgbD30
  pick[3952].Int( 207,47,15,255 );  // rgbD31
  pick[3953].Int( 207,47,31,255 );  // rgbD32
  pick[3954].Int( 207,47,47,255 );  // rgbD33
  pick[3955].Int( 207,47,63,255 );  // rgbD34
  pick[3956].Int( 207,47,79,255 );  // rgbD35
  pick[3957].Int( 207,47,95,255 );  // rgbD36
  pick[3958].Int( 207,47,111,255 );  // rgbD37
  pick[3959].Int( 207,47,127,255 );  // rgbD38
  pick[3960].Int( 207,47,143,255 );  // rgbD39
  pick[3961].Int( 207,47,159,255 );  // rgbD3A
  pick[3962].Int( 207,47,175,255 );  // rgbD3B
  pick[3963].Int( 207,47,191,255 );  // rgbD3C
  pick[3964].Int( 207,47,207,255 );  // rgbD3D
  pick[3965].Int( 207,47,223,255 );  // rgbD3E
  pick[3966].Int( 207,47,239,255 );  // rgbD3F
  pick[3967].Int( 207,63,0,255 );  // rgbD40
  pick[3968].Int( 207,63,15,255 );  // rgbD41
  pick[3969].Int( 207,63,31,255 );  // rgbD42
  pick[3970].Int( 207,63,47,255 );  // rgbD43
  pick[3971].Int( 207,63,63,255 );  // rgbD44
  pick[3972].Int( 207,63,79,255 );  // rgbD45
  pick[3973].Int( 207,63,95,255 );  // rgbD46
  pick[3974].Int( 207,63,111,255 );  // rgbD47
  pick[3975].Int( 207,63,127,255 );  // rgbD48
  pick[3976].Int( 207,63,143,255 );  // rgbD49
  pick[3977].Int( 207,63,159,255 );  // rgbD4A
  pick[3978].Int( 207,63,175,255 );  // rgbD4B
  pick[3979].Int( 207,63,191,255 );  // rgbD4C
  pick[3980].Int( 207,63,207,255 );  // rgbD4D
  pick[3981].Int( 207,63,223,255 );  // rgbD4E
  pick[3982].Int( 207,63,239,255 );  // rgbD4F
  pick[3983].Int( 207,79,0,255 );  // rgbD50
  pick[3984].Int( 207,79,15,255 );  // rgbD51
  pick[3985].Int( 207,79,31,255 );  // rgbD52
  pick[3986].Int( 207,79,47,255 );  // rgbD53
  pick[3987].Int( 207,79,63,255 );  // rgbD54
  pick[3988].Int( 207,79,79,255 );  // rgbD55
  pick[3989].Int( 207,79,95,255 );  // rgbD56
  pick[3990].Int( 207,79,111,255 );  // rgbD57
  pick[3991].Int( 207,79,127,255 );  // rgbD58
  pick[3992].Int( 207,79,143,255 );  // rgbD59
  pick[3993].Int( 207,79,159,255 );  // rgbD5A
  pick[3994].Int( 207,79,175,255 );  // rgbD5B
  pick[3995].Int( 207,79,191,255 );  // rgbD5C
  pick[3996].Int( 207,79,207,255 );  // rgbD5D
  pick[3997].Int( 207,79,223,255 );  // rgbD5E
  pick[3998].Int( 207,79,239,255 );  // rgbD5F
  pick[3999].Int( 207,95,0,255 );  // rgbD60
  pick[4000].Int( 207,95,15,255 );  // rgbD61
  pick[4001].Int( 207,95,31,255 );  // rgbD62
  pick[4002].Int( 207,95,47,255 );  // rgbD63
  pick[4003].Int( 207,95,63,255 );  // rgbD64
  pick[4004].Int( 207,95,79,255 );  // rgbD65
  pick[4005].Int( 207,95,95,255 );  // rgbD66
  pick[4006].Int( 207,95,111,255 );  // rgbD67
  pick[4007].Int( 207,95,127,255 );  // rgbD68
  pick[4008].Int( 207,95,143,255 );  // rgbD69
  pick[4009].Int( 207,95,159,255 );  // rgbD6A
  pick[4010].Int( 207,95,175,255 );  // rgbD6B
  pick[4011].Int( 207,95,191,255 );  // rgbD6C
  pick[4012].Int( 207,95,207,255 );  // rgbD6D
  pick[4013].Int( 207,95,223,255 );  // rgbD6E
  pick[4014].Int( 207,95,239,255 );  // rgbD6F
  pick[4015].Int( 207,111,0,255 );  // rgbD70
  pick[4016].Int( 207,111,15,255 );  // rgbD71
  pick[4017].Int( 207,111,31,255 );  // rgbD72
  pick[4018].Int( 207,111,47,255 );  // rgbD73
  pick[4019].Int( 207,111,63,255 );  // rgbD74
  pick[4020].Int( 207,111,79,255 );  // rgbD75
  pick[4021].Int( 207,111,95,255 );  // rgbD76
  pick[4022].Int( 207,111,111,255 );  // rgbD77
  pick[4023].Int( 207,111,127,255 );  // rgbD78
  pick[4024].Int( 207,111,143,255 );  // rgbD79
  pick[4025].Int( 207,111,159,255 );  // rgbD7A
  pick[4026].Int( 207,111,175,255 );  // rgbD7B
  pick[4027].Int( 207,111,191,255 );  // rgbD7C
  pick[4028].Int( 207,111,207,255 );  // rgbD7D
  pick[4029].Int( 207,111,223,255 );  // rgbD7E
  pick[4030].Int( 207,111,239,255 );  // rgbD7F
  pick[4031].Int( 207,127,0,255 );  // rgbD80
  pick[4032].Int( 207,127,15,255 );  // rgbD81
  pick[4033].Int( 207,127,31,255 );  // rgbD82
  pick[4034].Int( 207,127,47,255 );  // rgbD83
  pick[4035].Int( 207,127,63,255 );  // rgbD84
  pick[4036].Int( 207,127,79,255 );  // rgbD85
  pick[4037].Int( 207,127,95,255 );  // rgbD86
  pick[4038].Int( 207,127,111,255 );  // rgbD87
  pick[4039].Int( 207,127,127,255 );  // rgbD88
  pick[4040].Int( 207,127,143,255 );  // rgbD89
  pick[4041].Int( 207,127,159,255 );  // rgbD8A
  pick[4042].Int( 207,127,175,255 );  // rgbD8B
  pick[4043].Int( 207,127,191,255 );  // rgbD8C
  pick[4044].Int( 207,127,207,255 );  // rgbD8D
  pick[4045].Int( 207,127,223,255 );  // rgbD8E
  pick[4046].Int( 207,127,239,255 );  // rgbD8F
  pick[4047].Int( 207,143,0,255 );  // rgbD90
  pick[4048].Int( 207,143,15,255 );  // rgbD91
  pick[4049].Int( 207,143,31,255 );  // rgbD92
  pick[4050].Int( 207,143,47,255 );  // rgbD93
  pick[4051].Int( 207,143,63,255 );  // rgbD94
  pick[4052].Int( 207,143,79,255 );  // rgbD95
  pick[4053].Int( 207,143,95,255 );  // rgbD96
  pick[4054].Int( 207,143,111,255 );  // rgbD97
  pick[4055].Int( 207,143,127,255 );  // rgbD98
  pick[4056].Int( 207,143,143,255 );  // rgbD99
  pick[4057].Int( 207,143,159,255 );  // rgbD9A
  pick[4058].Int( 207,143,175,255 );  // rgbD9B
  pick[4059].Int( 207,143,191,255 );  // rgbD9C
  pick[4060].Int( 207,143,207,255 );  // rgbD9D
  pick[4061].Int( 207,143,223,255 );  // rgbD9E
  pick[4062].Int( 207,143,239,255 );  // rgbD9F
  pick[4063].Int( 207,159,0,255 );  // rgbDA0
  pick[4064].Int( 207,159,15,255 );  // rgbDA1
  pick[4065].Int( 207,159,31,255 );  // rgbDA2
  pick[4066].Int( 207,159,47,255 );  // rgbDA3
  pick[4067].Int( 207,159,63,255 );  // rgbDA4
  pick[4068].Int( 207,159,79,255 );  // rgbDA5
  pick[4069].Int( 207,159,95,255 );  // rgbDA6
  pick[4070].Int( 207,159,111,255 );  // rgbDA7
  pick[4071].Int( 207,159,127,255 );  // rgbDA8
  pick[4072].Int( 207,159,143,255 );  // rgbDA9
  pick[4073].Int( 207,159,159,255 );  // rgbDAA
  pick[4074].Int( 207,159,175,255 );  // rgbDAB
  pick[4075].Int( 207,159,191,255 );  // rgbDAC
  pick[4076].Int( 207,159,207,255 );  // rgbDAD
  pick[4077].Int( 207,159,223,255 );  // rgbDAE
  pick[4078].Int( 207,159,239,255 );  // rgbDAF
  pick[4079].Int( 207,175,0,255 );  // rgbDB0
  pick[4080].Int( 207,175,15,255 );  // rgbDB1
  pick[4081].Int( 207,175,31,255 );  // rgbDB2
  pick[4082].Int( 207,175,47,255 );  // rgbDB3
  pick[4083].Int( 207,175,63,255 );  // rgbDB4
  pick[4084].Int( 207,175,79,255 );  // rgbDB5
  pick[4085].Int( 207,175,95,255 );  // rgbDB6
  pick[4086].Int( 207,175,111,255 );  // rgbDB7
  pick[4087].Int( 207,175,127,255 );  // rgbDB8
  pick[4088].Int( 207,175,143,255 );  // rgbDB9
  pick[4089].Int( 207,175,159,255 );  // rgbDBA
  pick[4090].Int( 207,175,175,255 );  // rgbDBB
  pick[4091].Int( 207,175,191,255 );  // rgbDBC
  pick[4092].Int( 207,175,207,255 );  // rgbDBD
  pick[4093].Int( 207,175,223,255 );  // rgbDBE
  pick[4094].Int( 207,175,239,255 );  // rgbDBF
  pick[4095].Int( 207,191,0,255 );  // rgbDC0
  pick[4096].Int( 207,191,15,255 );  // rgbDC1
  pick[4097].Int( 207,191,31,255 );  // rgbDC2
  pick[4098].Int( 207,191,47,255 );  // rgbDC3
  pick[4099].Int( 207,191,63,255 );  // rgbDC4
  pick[4100].Int( 207,191,79,255 );  // rgbDC5
  pick[4101].Int( 207,191,95,255 );  // rgbDC6
  pick[4102].Int( 207,191,111,255 );  // rgbDC7
  pick[4103].Int( 207,191,127,255 );  // rgbDC8
  pick[4104].Int( 207,191,143,255 );  // rgbDC9
  pick[4105].Int( 207,191,159,255 );  // rgbDCA
  pick[4106].Int( 207,191,175,255 );  // rgbDCB
  pick[4107].Int( 207,191,191,255 );  // rgbDCC
  pick[4108].Int( 207,191,207,255 );  // rgbDCD
  pick[4109].Int( 207,191,223,255 );  // rgbDCE
  pick[4110].Int( 207,191,239,255 );  // rgbDCF
  pick[4111].Int( 207,207,0,255 );  // rgbDD0
  pick[4112].Int( 207,207,15,255 );  // rgbDD1
  pick[4113].Int( 207,207,31,255 );  // rgbDD2
  pick[4114].Int( 207,207,47,255 );  // rgbDD3
  pick[4115].Int( 207,207,63,255 );  // rgbDD4
  pick[4116].Int( 207,207,79,255 );  // rgbDD5
  pick[4117].Int( 207,207,95,255 );  // rgbDD6
  pick[4118].Int( 207,207,111,255 );  // rgbDD7
  pick[4119].Int( 207,207,127,255 );  // rgbDD8
  pick[4120].Int( 207,207,143,255 );  // rgbDD9
  pick[4121].Int( 207,207,159,255 );  // rgbDDA
  pick[4122].Int( 207,207,175,255 );  // rgbDDB
  pick[4123].Int( 207,207,191,255 );  // rgbDDC
  pick[4124].Int( 207,207,207,255 );  // rgbDDD
  pick[4125].Int( 207,207,223,255 );  // rgbDDE
  pick[4126].Int( 207,207,239,255 );  // rgbDDF
  pick[4127].Int( 207,223,0,255 );  // rgbDE0
  pick[4128].Int( 207,223,15,255 );  // rgbDE1
  pick[4129].Int( 207,223,31,255 );  // rgbDE2
  pick[4130].Int( 207,223,47,255 );  // rgbDE3
  pick[4131].Int( 207,223,63,255 );  // rgbDE4
  pick[4132].Int( 207,223,79,255 );  // rgbDE5
  pick[4133].Int( 207,223,95,255 );  // rgbDE6
  pick[4134].Int( 207,223,111,255 );  // rgbDE7
  pick[4135].Int( 207,223,127,255 );  // rgbDE8
  pick[4136].Int( 207,223,143,255 );  // rgbDE9
  pick[4137].Int( 207,223,159,255 );  // rgbDEA
  pick[4138].Int( 207,223,175,255 );  // rgbDEB
  pick[4139].Int( 207,223,191,255 );  // rgbDEC
  pick[4140].Int( 207,223,207,255 );  // rgbDED
  pick[4141].Int( 207,223,223,255 );  // rgbDEE
  pick[4142].Int( 207,223,239,255 );  // rgbDEF
  pick[4143].Int( 207,239,0,255 );  // rgbDF0
  pick[4144].Int( 207,239,15,255 );  // rgbDF1
  pick[4145].Int( 207,239,31,255 );  // rgbDF2
  pick[4146].Int( 207,239,47,255 );  // rgbDF3
  pick[4147].Int( 207,239,63,255 );  // rgbDF4
  pick[4148].Int( 207,239,79,255 );  // rgbDF5
  pick[4149].Int( 207,239,95,255 );  // rgbDF6
  pick[4150].Int( 207,239,111,255 );  // rgbDF7
  pick[4151].Int( 207,239,127,255 );  // rgbDF8
  pick[4152].Int( 207,239,143,255 );  // rgbDF9
  pick[4153].Int( 207,239,159,255 );  // rgbDFA
  pick[4154].Int( 207,239,175,255 );  // rgbDFB
  pick[4155].Int( 207,239,191,255 );  // rgbDFC
  pick[4156].Int( 207,239,207,255 );  // rgbDFD
  pick[4157].Int( 207,239,223,255 );  // rgbDFE
  pick[4158].Int( 207,239,239,255 );  // rgbDFF
  pick[4159].Int( 223,0,0,255 );  // rgbE00
  pick[4160].Int( 223,0,15,255 );  // rgbE01
  pick[4161].Int( 223,0,31,255 );  // rgbE02
  pick[4162].Int( 223,0,47,255 );  // rgbE03
  pick[4163].Int( 223,0,63,255 );  // rgbE04
  pick[4164].Int( 223,0,79,255 );  // rgbE05
  pick[4165].Int( 223,0,95,255 );  // rgbE06
  pick[4166].Int( 223,0,111,255 );  // rgbE07
  pick[4167].Int( 223,0,127,255 );  // rgbE08
  pick[4168].Int( 223,0,143,255 );  // rgbE09
  pick[4169].Int( 223,0,159,255 );  // rgbE0A
  pick[4170].Int( 223,0,175,255 );  // rgbE0B
  pick[4171].Int( 223,0,191,255 );  // rgbE0C
  pick[4172].Int( 223,0,207,255 );  // rgbE0D
  pick[4173].Int( 223,0,223,255 );  // rgbE0E
  pick[4174].Int( 223,0,239,255 );  // rgbE0F
  pick[4175].Int( 223,15,0,255 );  // rgbE10
  pick[4176].Int( 223,15,15,255 );  // rgbE11
  pick[4177].Int( 223,15,31,255 );  // rgbE12
  pick[4178].Int( 223,15,47,255 );  // rgbE13
  pick[4179].Int( 223,15,63,255 );  // rgbE14
  pick[4180].Int( 223,15,79,255 );  // rgbE15
  pick[4181].Int( 223,15,95,255 );  // rgbE16
  pick[4182].Int( 223,15,111,255 );  // rgbE17
  pick[4183].Int( 223,15,127,255 );  // rgbE18
  pick[4184].Int( 223,15,143,255 );  // rgbE19
  pick[4185].Int( 223,15,159,255 );  // rgbE1A
  pick[4186].Int( 223,15,175,255 );  // rgbE1B
  pick[4187].Int( 223,15,191,255 );  // rgbE1C
  pick[4188].Int( 223,15,207,255 );  // rgbE1D
  pick[4189].Int( 223,15,223,255 );  // rgbE1E
  pick[4190].Int( 223,15,239,255 );  // rgbE1F
  pick[4191].Int( 223,31,0,255 );  // rgbE20
  pick[4192].Int( 223,31,15,255 );  // rgbE21
  pick[4193].Int( 223,31,31,255 );  // rgbE22
  pick[4194].Int( 223,31,47,255 );  // rgbE23
  pick[4195].Int( 223,31,63,255 );  // rgbE24
  pick[4196].Int( 223,31,79,255 );  // rgbE25
  pick[4197].Int( 223,31,95,255 );  // rgbE26
  pick[4198].Int( 223,31,111,255 );  // rgbE27
  pick[4199].Int( 223,31,127,255 );  // rgbE28
  pick[4200].Int( 223,31,143,255 );  // rgbE29
  pick[4201].Int( 223,31,159,255 );  // rgbE2A
  pick[4202].Int( 223,31,175,255 );  // rgbE2B
  pick[4203].Int( 223,31,191,255 );  // rgbE2C
  pick[4204].Int( 223,31,207,255 );  // rgbE2D
  pick[4205].Int( 223,31,223,255 );  // rgbE2E
  pick[4206].Int( 223,31,239,255 );  // rgbE2F
  pick[4207].Int( 223,47,0,255 );  // rgbE30
  pick[4208].Int( 223,47,15,255 );  // rgbE31
  pick[4209].Int( 223,47,31,255 );  // rgbE32
  pick[4210].Int( 223,47,47,255 );  // rgbE33
  pick[4211].Int( 223,47,63,255 );  // rgbE34
  pick[4212].Int( 223,47,79,255 );  // rgbE35
  pick[4213].Int( 223,47,95,255 );  // rgbE36
  pick[4214].Int( 223,47,111,255 );  // rgbE37
  pick[4215].Int( 223,47,127,255 );  // rgbE38
  pick[4216].Int( 223,47,143,255 );  // rgbE39
  pick[4217].Int( 223,47,159,255 );  // rgbE3A
  pick[4218].Int( 223,47,175,255 );  // rgbE3B
  pick[4219].Int( 223,47,191,255 );  // rgbE3C
  pick[4220].Int( 223,47,207,255 );  // rgbE3D
  pick[4221].Int( 223,47,223,255 );  // rgbE3E
  pick[4222].Int( 223,47,239,255 );  // rgbE3F
  pick[4223].Int( 223,63,0,255 );  // rgbE40
  pick[4224].Int( 223,63,15,255 );  // rgbE41
  pick[4225].Int( 223,63,31,255 );  // rgbE42
  pick[4226].Int( 223,63,47,255 );  // rgbE43
  pick[4227].Int( 223,63,63,255 );  // rgbE44
  pick[4228].Int( 223,63,79,255 );  // rgbE45
  pick[4229].Int( 223,63,95,255 );  // rgbE46
  pick[4230].Int( 223,63,111,255 );  // rgbE47
  pick[4231].Int( 223,63,127,255 );  // rgbE48
  pick[4232].Int( 223,63,143,255 );  // rgbE49
  pick[4233].Int( 223,63,159,255 );  // rgbE4A
  pick[4234].Int( 223,63,175,255 );  // rgbE4B
  pick[4235].Int( 223,63,191,255 );  // rgbE4C
  pick[4236].Int( 223,63,207,255 );  // rgbE4D
  pick[4237].Int( 223,63,223,255 );  // rgbE4E
  pick[4238].Int( 223,63,239,255 );  // rgbE4F
  pick[4239].Int( 223,79,0,255 );  // rgbE50
  pick[4240].Int( 223,79,15,255 );  // rgbE51
  pick[4241].Int( 223,79,31,255 );  // rgbE52
  pick[4242].Int( 223,79,47,255 );  // rgbE53
  pick[4243].Int( 223,79,63,255 );  // rgbE54
  pick[4244].Int( 223,79,79,255 );  // rgbE55
  pick[4245].Int( 223,79,95,255 );  // rgbE56
  pick[4246].Int( 223,79,111,255 );  // rgbE57
  pick[4247].Int( 223,79,127,255 );  // rgbE58
  pick[4248].Int( 223,79,143,255 );  // rgbE59
  pick[4249].Int( 223,79,159,255 );  // rgbE5A
  pick[4250].Int( 223,79,175,255 );  // rgbE5B
  pick[4251].Int( 223,79,191,255 );  // rgbE5C
  pick[4252].Int( 223,79,207,255 );  // rgbE5D
  pick[4253].Int( 223,79,223,255 );  // rgbE5E
  pick[4254].Int( 223,79,239,255 );  // rgbE5F
  pick[4255].Int( 223,95,0,255 );  // rgbE60
  pick[4256].Int( 223,95,15,255 );  // rgbE61
  pick[4257].Int( 223,95,31,255 );  // rgbE62
  pick[4258].Int( 223,95,47,255 );  // rgbE63
  pick[4259].Int( 223,95,63,255 );  // rgbE64
  pick[4260].Int( 223,95,79,255 );  // rgbE65
  pick[4261].Int( 223,95,95,255 );  // rgbE66
  pick[4262].Int( 223,95,111,255 );  // rgbE67
  pick[4263].Int( 223,95,127,255 );  // rgbE68
  pick[4264].Int( 223,95,143,255 );  // rgbE69
  pick[4265].Int( 223,95,159,255 );  // rgbE6A
  pick[4266].Int( 223,95,175,255 );  // rgbE6B
  pick[4267].Int( 223,95,191,255 );  // rgbE6C
  pick[4268].Int( 223,95,207,255 );  // rgbE6D
  pick[4269].Int( 223,95,223,255 );  // rgbE6E
  pick[4270].Int( 223,95,239,255 );  // rgbE6F
  pick[4271].Int( 223,111,0,255 );  // rgbE70
  pick[4272].Int( 223,111,15,255 );  // rgbE71
  pick[4273].Int( 223,111,31,255 );  // rgbE72
  pick[4274].Int( 223,111,47,255 );  // rgbE73
  pick[4275].Int( 223,111,63,255 );  // rgbE74
  pick[4276].Int( 223,111,79,255 );  // rgbE75
  pick[4277].Int( 223,111,95,255 );  // rgbE76
  pick[4278].Int( 223,111,111,255 );  // rgbE77
  pick[4279].Int( 223,111,127,255 );  // rgbE78
  pick[4280].Int( 223,111,143,255 );  // rgbE79
  pick[4281].Int( 223,111,159,255 );  // rgbE7A
  pick[4282].Int( 223,111,175,255 );  // rgbE7B
  pick[4283].Int( 223,111,191,255 );  // rgbE7C
  pick[4284].Int( 223,111,207,255 );  // rgbE7D
  pick[4285].Int( 223,111,223,255 );  // rgbE7E
  pick[4286].Int( 223,111,239,255 );  // rgbE7F
  pick[4287].Int( 223,127,0,255 );  // rgbE80
  pick[4288].Int( 223,127,15,255 );  // rgbE81
  pick[4289].Int( 223,127,31,255 );  // rgbE82
  pick[4290].Int( 223,127,47,255 );  // rgbE83
  pick[4291].Int( 223,127,63,255 );  // rgbE84
  pick[4292].Int( 223,127,79,255 );  // rgbE85
  pick[4293].Int( 223,127,95,255 );  // rgbE86
  pick[4294].Int( 223,127,111,255 );  // rgbE87
  pick[4295].Int( 223,127,127,255 );  // rgbE88
  pick[4296].Int( 223,127,143,255 );  // rgbE89
  pick[4297].Int( 223,127,159,255 );  // rgbE8A
  pick[4298].Int( 223,127,175,255 );  // rgbE8B
  pick[4299].Int( 223,127,191,255 );  // rgbE8C
  pick[4300].Int( 223,127,207,255 );  // rgbE8D
  pick[4301].Int( 223,127,223,255 );  // rgbE8E
  pick[4302].Int( 223,127,239,255 );  // rgbE8F
  pick[4303].Int( 223,143,0,255 );  // rgbE90
  pick[4304].Int( 223,143,15,255 );  // rgbE91
  pick[4305].Int( 223,143,31,255 );  // rgbE92
  pick[4306].Int( 223,143,47,255 );  // rgbE93
  pick[4307].Int( 223,143,63,255 );  // rgbE94
  pick[4308].Int( 223,143,79,255 );  // rgbE95
  pick[4309].Int( 223,143,95,255 );  // rgbE96
  pick[4310].Int( 223,143,111,255 );  // rgbE97
  pick[4311].Int( 223,143,127,255 );  // rgbE98
  pick[4312].Int( 223,143,143,255 );  // rgbE99
  pick[4313].Int( 223,143,159,255 );  // rgbE9A
  pick[4314].Int( 223,143,175,255 );  // rgbE9B
  pick[4315].Int( 223,143,191,255 );  // rgbE9C
  pick[4316].Int( 223,143,207,255 );  // rgbE9D
  pick[4317].Int( 223,143,223,255 );  // rgbE9E
  pick[4318].Int( 223,143,239,255 );  // rgbE9F
  pick[4319].Int( 223,159,0,255 );  // rgbEA0
  pick[4320].Int( 223,159,15,255 );  // rgbEA1
  pick[4321].Int( 223,159,31,255 );  // rgbEA2
  pick[4322].Int( 223,159,47,255 );  // rgbEA3
  pick[4323].Int( 223,159,63,255 );  // rgbEA4
  pick[4324].Int( 223,159,79,255 );  // rgbEA5
  pick[4325].Int( 223,159,95,255 );  // rgbEA6
  pick[4326].Int( 223,159,111,255 );  // rgbEA7
  pick[4327].Int( 223,159,127,255 );  // rgbEA8
  pick[4328].Int( 223,159,143,255 );  // rgbEA9
  pick[4329].Int( 223,159,159,255 );  // rgbEAA
  pick[4330].Int( 223,159,175,255 );  // rgbEAB
  pick[4331].Int( 223,159,191,255 );  // rgbEAC
  pick[4332].Int( 223,159,207,255 );  // rgbEAD
  pick[4333].Int( 223,159,223,255 );  // rgbEAE
  pick[4334].Int( 223,159,239,255 );  // rgbEAF
  pick[4335].Int( 223,175,0,255 );  // rgbEB0
  pick[4336].Int( 223,175,15,255 );  // rgbEB1
  pick[4337].Int( 223,175,31,255 );  // rgbEB2
  pick[4338].Int( 223,175,47,255 );  // rgbEB3
  pick[4339].Int( 223,175,63,255 );  // rgbEB4
  pick[4340].Int( 223,175,79,255 );  // rgbEB5
  pick[4341].Int( 223,175,95,255 );  // rgbEB6
  pick[4342].Int( 223,175,111,255 );  // rgbEB7
  pick[4343].Int( 223,175,127,255 );  // rgbEB8
  pick[4344].Int( 223,175,143,255 );  // rgbEB9
  pick[4345].Int( 223,175,159,255 );  // rgbEBA
  pick[4346].Int( 223,175,175,255 );  // rgbEBB
  pick[4347].Int( 223,175,191,255 );  // rgbEBC
  pick[4348].Int( 223,175,207,255 );  // rgbEBD
  pick[4349].Int( 223,175,223,255 );  // rgbEBE
  pick[4350].Int( 223,175,239,255 );  // rgbEBF
  pick[4351].Int( 223,191,0,255 );  // rgbEC0
  pick[4352].Int( 223,191,15,255 );  // rgbEC1
  pick[4353].Int( 223,191,31,255 );  // rgbEC2
  pick[4354].Int( 223,191,47,255 );  // rgbEC3
  pick[4355].Int( 223,191,63,255 );  // rgbEC4
  pick[4356].Int( 223,191,79,255 );  // rgbEC5
  pick[4357].Int( 223,191,95,255 );  // rgbEC6
  pick[4358].Int( 223,191,111,255 );  // rgbEC7
  pick[4359].Int( 223,191,127,255 );  // rgbEC8
  pick[4360].Int( 223,191,143,255 );  // rgbEC9
  pick[4361].Int( 223,191,159,255 );  // rgbECA
  pick[4362].Int( 223,191,175,255 );  // rgbECB
  pick[4363].Int( 223,191,191,255 );  // rgbECC
  pick[4364].Int( 223,191,207,255 );  // rgbECD
  pick[4365].Int( 223,191,223,255 );  // rgbECE
  pick[4366].Int( 223,191,239,255 );  // rgbECF
  pick[4367].Int( 223,207,0,255 );  // rgbED0
  pick[4368].Int( 223,207,15,255 );  // rgbED1
  pick[4369].Int( 223,207,31,255 );  // rgbED2
  pick[4370].Int( 223,207,47,255 );  // rgbED3
  pick[4371].Int( 223,207,63,255 );  // rgbED4
  pick[4372].Int( 223,207,79,255 );  // rgbED5
  pick[4373].Int( 223,207,95,255 );  // rgbED6
  pick[4374].Int( 223,207,111,255 );  // rgbED7
  pick[4375].Int( 223,207,127,255 );  // rgbED8
  pick[4376].Int( 223,207,143,255 );  // rgbED9
  pick[4377].Int( 223,207,159,255 );  // rgbEDA
  pick[4378].Int( 223,207,175,255 );  // rgbEDB
  pick[4379].Int( 223,207,191,255 );  // rgbEDC
  pick[4380].Int( 223,207,207,255 );  // rgbEDD
  pick[4381].Int( 223,207,223,255 );  // rgbEDE
  pick[4382].Int( 223,207,239,255 );  // rgbEDF
  pick[4383].Int( 223,223,0,255 );  // rgbEE0
  pick[4384].Int( 223,223,15,255 );  // rgbEE1
  pick[4385].Int( 223,223,31,255 );  // rgbEE2
  pick[4386].Int( 223,223,47,255 );  // rgbEE3
  pick[4387].Int( 223,223,63,255 );  // rgbEE4
  pick[4388].Int( 223,223,79,255 );  // rgbEE5
  pick[4389].Int( 223,223,95,255 );  // rgbEE6
  pick[4390].Int( 223,223,111,255 );  // rgbEE7
  pick[4391].Int( 223,223,127,255 );  // rgbEE8
  pick[4392].Int( 223,223,143,255 );  // rgbEE9
  pick[4393].Int( 223,223,159,255 );  // rgbEEA
  pick[4394].Int( 223,223,175,255 );  // rgbEEB
  pick[4395].Int( 223,223,191,255 );  // rgbEEC
  pick[4396].Int( 223,223,207,255 );  // rgbEED
  pick[4397].Int( 223,223,223,255 );  // rgbEEE
  pick[4398].Int( 223,223,239,255 );  // rgbEEF
  pick[4399].Int( 223,239,0,255 );  // rgbEF0
  pick[4400].Int( 223,239,15,255 );  // rgbEF1
  pick[4401].Int( 223,239,31,255 );  // rgbEF2
  pick[4402].Int( 223,239,47,255 );  // rgbEF3
  pick[4403].Int( 223,239,63,255 );  // rgbEF4
  pick[4404].Int( 223,239,79,255 );  // rgbEF5
  pick[4405].Int( 223,239,95,255 );  // rgbEF6
  pick[4406].Int( 223,239,111,255 );  // rgbEF7
  pick[4407].Int( 223,239,127,255 );  // rgbEF8
  pick[4408].Int( 223,239,143,255 );  // rgbEF9
  pick[4409].Int( 223,239,159,255 );  // rgbEFA
  pick[4410].Int( 223,239,175,255 );  // rgbEFB
  pick[4411].Int( 223,239,191,255 );  // rgbEFC
  pick[4412].Int( 223,239,207,255 );  // rgbEFD
  pick[4413].Int( 223,239,223,255 );  // rgbEFE
  pick[4414].Int( 223,239,239,255 );  // rgbEFF
  pick[4415].Int( 239,0,0,255 );  // rgbF00
  pick[4416].Int( 239,0,15,255 );  // rgbF01
  pick[4417].Int( 239,0,31,255 );  // rgbF02
  pick[4418].Int( 239,0,47,255 );  // rgbF03
  pick[4419].Int( 239,0,63,255 );  // rgbF04
  pick[4420].Int( 239,0,79,255 );  // rgbF05
  pick[4421].Int( 239,0,95,255 );  // rgbF06
  pick[4422].Int( 239,0,111,255 );  // rgbF07
  pick[4423].Int( 239,0,127,255 );  // rgbF08
  pick[4424].Int( 239,0,143,255 );  // rgbF09
  pick[4425].Int( 239,0,159,255 );  // rgbF0A
  pick[4426].Int( 239,0,175,255 );  // rgbF0B
  pick[4427].Int( 239,0,191,255 );  // rgbF0C
  pick[4428].Int( 239,0,207,255 );  // rgbF0D
  pick[4429].Int( 239,0,223,255 );  // rgbF0E
  pick[4430].Int( 239,0,239,255 );  // rgbF0F
  pick[4431].Int( 239,15,0,255 );  // rgbF10
  pick[4432].Int( 239,15,15,255 );  // rgbF11
  pick[4433].Int( 239,15,31,255 );  // rgbF12
  pick[4434].Int( 239,15,47,255 );  // rgbF13
  pick[4435].Int( 239,15,63,255 );  // rgbF14
  pick[4436].Int( 239,15,79,255 );  // rgbF15
  pick[4437].Int( 239,15,95,255 );  // rgbF16
  pick[4438].Int( 239,15,111,255 );  // rgbF17
  pick[4439].Int( 239,15,127,255 );  // rgbF18
  pick[4440].Int( 239,15,143,255 );  // rgbF19
  pick[4441].Int( 239,15,159,255 );  // rgbF1A
  pick[4442].Int( 239,15,175,255 );  // rgbF1B
  pick[4443].Int( 239,15,191,255 );  // rgbF1C
  pick[4444].Int( 239,15,207,255 );  // rgbF1D
  pick[4445].Int( 239,15,223,255 );  // rgbF1E
  pick[4446].Int( 239,15,239,255 );  // rgbF1F
  pick[4447].Int( 239,31,0,255 );  // rgbF20
  pick[4448].Int( 239,31,15,255 );  // rgbF21
  pick[4449].Int( 239,31,31,255 );  // rgbF22
  pick[4450].Int( 239,31,47,255 );  // rgbF23
  pick[4451].Int( 239,31,63,255 );  // rgbF24
  pick[4452].Int( 239,31,79,255 );  // rgbF25
  pick[4453].Int( 239,31,95,255 );  // rgbF26
  pick[4454].Int( 239,31,111,255 );  // rgbF27
  pick[4455].Int( 239,31,127,255 );  // rgbF28
  pick[4456].Int( 239,31,143,255 );  // rgbF29
  pick[4457].Int( 239,31,159,255 );  // rgbF2A
  pick[4458].Int( 239,31,175,255 );  // rgbF2B
  pick[4459].Int( 239,31,191,255 );  // rgbF2C
  pick[4460].Int( 239,31,207,255 );  // rgbF2D
  pick[4461].Int( 239,31,223,255 );  // rgbF2E
  pick[4462].Int( 239,31,239,255 );  // rgbF2F
  pick[4463].Int( 239,47,0,255 );  // rgbF30
  pick[4464].Int( 239,47,15,255 );  // rgbF31
  pick[4465].Int( 239,47,31,255 );  // rgbF32
  pick[4466].Int( 239,47,47,255 );  // rgbF33
  pick[4467].Int( 239,47,63,255 );  // rgbF34
  pick[4468].Int( 239,47,79,255 );  // rgbF35
  pick[4469].Int( 239,47,95,255 );  // rgbF36
  pick[4470].Int( 239,47,111,255 );  // rgbF37
  pick[4471].Int( 239,47,127,255 );  // rgbF38
  pick[4472].Int( 239,47,143,255 );  // rgbF39
  pick[4473].Int( 239,47,159,255 );  // rgbF3A
  pick[4474].Int( 239,47,175,255 );  // rgbF3B
  pick[4475].Int( 239,47,191,255 );  // rgbF3C
  pick[4476].Int( 239,47,207,255 );  // rgbF3D
  pick[4477].Int( 239,47,223,255 );  // rgbF3E
  pick[4478].Int( 239,47,239,255 );  // rgbF3F
  pick[4479].Int( 239,63,0,255 );  // rgbF40
  pick[4480].Int( 239,63,15,255 );  // rgbF41
  pick[4481].Int( 239,63,31,255 );  // rgbF42
  pick[4482].Int( 239,63,47,255 );  // rgbF43
  pick[4483].Int( 239,63,63,255 );  // rgbF44
  pick[4484].Int( 239,63,79,255 );  // rgbF45
  pick[4485].Int( 239,63,95,255 );  // rgbF46
  pick[4486].Int( 239,63,111,255 );  // rgbF47
  pick[4487].Int( 239,63,127,255 );  // rgbF48
  pick[4488].Int( 239,63,143,255 );  // rgbF49
  pick[4489].Int( 239,63,159,255 );  // rgbF4A
  pick[4490].Int( 239,63,175,255 );  // rgbF4B
  pick[4491].Int( 239,63,191,255 );  // rgbF4C
  pick[4492].Int( 239,63,207,255 );  // rgbF4D
  pick[4493].Int( 239,63,223,255 );  // rgbF4E
  pick[4494].Int( 239,63,239,255 );  // rgbF4F
  pick[4495].Int( 239,79,0,255 );  // rgbF50
  pick[4496].Int( 239,79,15,255 );  // rgbF51
  pick[4497].Int( 239,79,31,255 );  // rgbF52
  pick[4498].Int( 239,79,47,255 );  // rgbF53
  pick[4499].Int( 239,79,63,255 );  // rgbF54
  pick[4500].Int( 239,79,79,255 );  // rgbF55
  pick[4501].Int( 239,79,95,255 );  // rgbF56
  pick[4502].Int( 239,79,111,255 );  // rgbF57
  pick[4503].Int( 239,79,127,255 );  // rgbF58
  pick[4504].Int( 239,79,143,255 );  // rgbF59
  pick[4505].Int( 239,79,159,255 );  // rgbF5A
  pick[4506].Int( 239,79,175,255 );  // rgbF5B
  pick[4507].Int( 239,79,191,255 );  // rgbF5C
  pick[4508].Int( 239,79,207,255 );  // rgbF5D
  pick[4509].Int( 239,79,223,255 );  // rgbF5E
  pick[4510].Int( 239,79,239,255 );  // rgbF5F
  pick[4511].Int( 239,95,0,255 );  // rgbF60
  pick[4512].Int( 239,95,15,255 );  // rgbF61
  pick[4513].Int( 239,95,31,255 );  // rgbF62
  pick[4514].Int( 239,95,47,255 );  // rgbF63
  pick[4515].Int( 239,95,63,255 );  // rgbF64
  pick[4516].Int( 239,95,79,255 );  // rgbF65
  pick[4517].Int( 239,95,95,255 );  // rgbF66
  pick[4518].Int( 239,95,111,255 );  // rgbF67
  pick[4519].Int( 239,95,127,255 );  // rgbF68
  pick[4520].Int( 239,95,143,255 );  // rgbF69
  pick[4521].Int( 239,95,159,255 );  // rgbF6A
  pick[4522].Int( 239,95,175,255 );  // rgbF6B
  pick[4523].Int( 239,95,191,255 );  // rgbF6C
  pick[4524].Int( 239,95,207,255 );  // rgbF6D
  pick[4525].Int( 239,95,223,255 );  // rgbF6E
  pick[4526].Int( 239,95,239,255 );  // rgbF6F
  pick[4527].Int( 239,111,0,255 );  // rgbF70
  pick[4528].Int( 239,111,15,255 );  // rgbF71
  pick[4529].Int( 239,111,31,255 );  // rgbF72
  pick[4530].Int( 239,111,47,255 );  // rgbF73
  pick[4531].Int( 239,111,63,255 );  // rgbF74
  pick[4532].Int( 239,111,79,255 );  // rgbF75
  pick[4533].Int( 239,111,95,255 );  // rgbF76
  pick[4534].Int( 239,111,111,255 );  // rgbF77
  pick[4535].Int( 239,111,127,255 );  // rgbF78
  pick[4536].Int( 239,111,143,255 );  // rgbF79
  pick[4537].Int( 239,111,159,255 );  // rgbF7A
  pick[4538].Int( 239,111,175,255 );  // rgbF7B
  pick[4539].Int( 239,111,191,255 );  // rgbF7C
  pick[4540].Int( 239,111,207,255 );  // rgbF7D
  pick[4541].Int( 239,111,223,255 );  // rgbF7E
  pick[4542].Int( 239,111,239,255 );  // rgbF7F
  pick[4543].Int( 239,127,0,255 );  // rgbF80
  pick[4544].Int( 239,127,15,255 );  // rgbF81
  pick[4545].Int( 239,127,31,255 );  // rgbF82
  pick[4546].Int( 239,127,47,255 );  // rgbF83
  pick[4547].Int( 239,127,63,255 );  // rgbF84
  pick[4548].Int( 239,127,79,255 );  // rgbF85
  pick[4549].Int( 239,127,95,255 );  // rgbF86
  pick[4550].Int( 239,127,111,255 );  // rgbF87
  pick[4551].Int( 239,127,127,255 );  // rgbF88
  pick[4552].Int( 239,127,143,255 );  // rgbF89
  pick[4553].Int( 239,127,159,255 );  // rgbF8A
  pick[4554].Int( 239,127,175,255 );  // rgbF8B
  pick[4555].Int( 239,127,191,255 );  // rgbF8C
  pick[4556].Int( 239,127,207,255 );  // rgbF8D
  pick[4557].Int( 239,127,223,255 );  // rgbF8E
  pick[4558].Int( 239,127,239,255 );  // rgbF8F
  pick[4559].Int( 239,143,0,255 );  // rgbF90
  pick[4560].Int( 239,143,15,255 );  // rgbF91
  pick[4561].Int( 239,143,31,255 );  // rgbF92
  pick[4562].Int( 239,143,47,255 );  // rgbF93
  pick[4563].Int( 239,143,63,255 );  // rgbF94
  pick[4564].Int( 239,143,79,255 );  // rgbF95
  pick[4565].Int( 239,143,95,255 );  // rgbF96
  pick[4566].Int( 239,143,111,255 );  // rgbF97
  pick[4567].Int( 239,143,127,255 );  // rgbF98
  pick[4568].Int( 239,143,143,255 );  // rgbF99
  pick[4569].Int( 239,143,159,255 );  // rgbF9A
  pick[4570].Int( 239,143,175,255 );  // rgbF9B
  pick[4571].Int( 239,143,191,255 );  // rgbF9C
  pick[4572].Int( 239,143,207,255 );  // rgbF9D
  pick[4573].Int( 239,143,223,255 );  // rgbF9E
  pick[4574].Int( 239,143,239,255 );  // rgbF9F
  pick[4575].Int( 239,159,0,255 );  // rgbFA0
  pick[4576].Int( 239,159,15,255 );  // rgbFA1
  pick[4577].Int( 239,159,31,255 );  // rgbFA2
  pick[4578].Int( 239,159,47,255 );  // rgbFA3
  pick[4579].Int( 239,159,63,255 );  // rgbFA4
  pick[4580].Int( 239,159,79,255 );  // rgbFA5
  pick[4581].Int( 239,159,95,255 );  // rgbFA6
  pick[4582].Int( 239,159,111,255 );  // rgbFA7
  pick[4583].Int( 239,159,127,255 );  // rgbFA8
  pick[4584].Int( 239,159,143,255 );  // rgbFA9
  pick[4585].Int( 239,159,159,255 );  // rgbFAA
  pick[4586].Int( 239,159,175,255 );  // rgbFAB
  pick[4587].Int( 239,159,191,255 );  // rgbFAC
  pick[4588].Int( 239,159,207,255 );  // rgbFAD
  pick[4589].Int( 239,159,223,255 );  // rgbFAE
  pick[4590].Int( 239,159,239,255 );  // rgbFAF
  pick[4591].Int( 239,175,0,255 );  // rgbFB0
  pick[4592].Int( 239,175,15,255 );  // rgbFB1
  pick[4593].Int( 239,175,31,255 );  // rgbFB2
  pick[4594].Int( 239,175,47,255 );  // rgbFB3
  pick[4595].Int( 239,175,63,255 );  // rgbFB4
  pick[4596].Int( 239,175,79,255 );  // rgbFB5
  pick[4597].Int( 239,175,95,255 );  // rgbFB6
  pick[4598].Int( 239,175,111,255 );  // rgbFB7
  pick[4599].Int( 239,175,127,255 );  // rgbFB8
  pick[4600].Int( 239,175,143,255 );  // rgbFB9
  pick[4601].Int( 239,175,159,255 );  // rgbFBA
  pick[4602].Int( 239,175,175,255 );  // rgbFBB
  pick[4603].Int( 239,175,191,255 );  // rgbFBC
  pick[4604].Int( 239,175,207,255 );  // rgbFBD
  pick[4605].Int( 239,175,223,255 );  // rgbFBE
  pick[4606].Int( 239,175,239,255 );  // rgbFBF
  pick[4607].Int( 239,191,0,255 );  // rgbFC0
  pick[4608].Int( 239,191,15,255 );  // rgbFC1
  pick[4609].Int( 239,191,31,255 );  // rgbFC2
  pick[4610].Int( 239,191,47,255 );  // rgbFC3
  pick[4611].Int( 239,191,63,255 );  // rgbFC4
  pick[4612].Int( 239,191,79,255 );  // rgbFC5
  pick[4613].Int( 239,191,95,255 );  // rgbFC6
  pick[4614].Int( 239,191,111,255 );  // rgbFC7
  pick[4615].Int( 239,191,127,255 );  // rgbFC8
  pick[4616].Int( 239,191,143,255 );  // rgbFC9
  pick[4617].Int( 239,191,159,255 );  // rgbFCA
  pick[4618].Int( 239,191,175,255 );  // rgbFCB
  pick[4619].Int( 239,191,191,255 );  // rgbFCC
  pick[4620].Int( 239,191,207,255 );  // rgbFCD
  pick[4621].Int( 239,191,223,255 );  // rgbFCE
  pick[4622].Int( 239,191,239,255 );  // rgbFCF
  pick[4623].Int( 239,207,0,255 );  // rgbFD0
  pick[4624].Int( 239,207,15,255 );  // rgbFD1
  pick[4625].Int( 239,207,31,255 );  // rgbFD2
  pick[4626].Int( 239,207,47,255 );  // rgbFD3
  pick[4627].Int( 239,207,63,255 );  // rgbFD4
  pick[4628].Int( 239,207,79,255 );  // rgbFD5
  pick[4629].Int( 239,207,95,255 );  // rgbFD6
  pick[4630].Int( 239,207,111,255 );  // rgbFD7
  pick[4631].Int( 239,207,127,255 );  // rgbFD8
  pick[4632].Int( 239,207,143,255 );  // rgbFD9
  pick[4633].Int( 239,207,159,255 );  // rgbFDA
  pick[4634].Int( 239,207,175,255 );  // rgbFDB
  pick[4635].Int( 239,207,191,255 );  // rgbFDC
  pick[4636].Int( 239,207,207,255 );  // rgbFDD
  pick[4637].Int( 239,207,223,255 );  // rgbFDE
  pick[4638].Int( 239,207,239,255 );  // rgbFDF
  pick[4639].Int( 239,223,0,255 );  // rgbFE0
  pick[4640].Int( 239,223,15,255 );  // rgbFE1
  pick[4641].Int( 239,223,31,255 );  // rgbFE2
  pick[4642].Int( 239,223,47,255 );  // rgbFE3
  pick[4643].Int( 239,223,63,255 );  // rgbFE4
  pick[4644].Int( 239,223,79,255 );  // rgbFE5
  pick[4645].Int( 239,223,95,255 );  // rgbFE6
  pick[4646].Int( 239,223,111,255 );  // rgbFE7
  pick[4647].Int( 239,223,127,255 );  // rgbFE8
  pick[4648].Int( 239,223,143,255 );  // rgbFE9
  pick[4649].Int( 239,223,159,255 );  // rgbFEA
  pick[4650].Int( 239,223,175,255 );  // rgbFEB
  pick[4651].Int( 239,223,191,255 );  // rgbFEC
  pick[4652].Int( 239,223,207,255 );  // rgbFED
  pick[4653].Int( 239,223,223,255 );  // rgbFEE
  pick[4654].Int( 239,223,239,255 );  // rgbFEF
  pick[4655].Int( 239,239,0,255 );  // rgbFF0
  pick[4656].Int( 239,239,15,255 );  // rgbFF1
  pick[4657].Int( 239,239,31,255 );  // rgbFF2
  pick[4658].Int( 239,239,47,255 );  // rgbFF3
  pick[4659].Int( 239,239,63,255 );  // rgbFF4
  pick[4660].Int( 239,239,79,255 );  // rgbFF5
  pick[4661].Int( 239,239,95,255 );  // rgbFF6
  pick[4662].Int( 239,239,111,255 );  // rgbFF7
  pick[4663].Int( 239,239,127,255 );  // rgbFF8
  pick[4664].Int( 239,239,143,255 );  // rgbFF9
  pick[4665].Int( 239,239,159,255 );  // rgbFFA
  pick[4666].Int( 239,239,175,255 );  // rgbFFB
  pick[4667].Int( 239,239,191,255 );  // rgbFFC
  pick[4668].Int( 239,239,207,255 );  // rgbFFD
  pick[4669].Int( 239,239,223,255 );  // rgbFFE
  pick[4670].Int( 239,239,239,255 );  // rgbFFF -----------------------------------
  pick[4671].Int( 0,0,0,25 );  // rgb0001
  pick[4672].Int( 0,0,31,25 );  // rgb0021
  pick[4673].Int( 0,0,63,25 );  // rgb0041
  pick[4674].Int( 0,0,95,25 );  // rgb0061
  pick[4675].Int( 0,0,127,25 );  // rgb0081
  pick[4676].Int( 0,0,159,25 );  // rgb00A1
  pick[4677].Int( 0,0,191,25 );  // rgb00C1
  pick[4678].Int( 0,0,223,25 );  // rgb00E1
  pick[4679].Int( 0,31,0,25 );  // rgb0201
  pick[4680].Int( 0,31,31,25 );  // rgb0221
  pick[4681].Int( 0,31,63,25 );  // rgb0241
  pick[4682].Int( 0,31,95,25 );  // rgb0261
  pick[4683].Int( 0,31,127,25 );  // rgb0281
  pick[4684].Int( 0,31,159,25 );  // rgb02A1
  pick[4685].Int( 0,31,191,25 );  // rgb02C1
  pick[4686].Int( 0,31,223,25 );  // rgb02E1
  pick[4687].Int( 0,63,0,25 );  // rgb0401
  pick[4688].Int( 0,63,31,25 );  // rgb0421
  pick[4689].Int( 0,63,63,25 );  // rgb0441
  pick[4690].Int( 0,63,95,25 );  // rgb0461
  pick[4691].Int( 0,63,127,25 );  // rgb0481
  pick[4692].Int( 0,63,159,25 );  // rgb04A1
  pick[4693].Int( 0,63,191,25 );  // rgb04C1
  pick[4694].Int( 0,63,223,25 );  // rgb04E1
  pick[4695].Int( 0,95,0,25 );  // rgb0601
  pick[4696].Int( 0,95,31,25 );  // rgb0621
  pick[4697].Int( 0,95,63,25 );  // rgb0641
  pick[4698].Int( 0,95,95,25 );  // rgb0661
  pick[4699].Int( 0,95,127,25 );  // rgb0681
  pick[4700].Int( 0,95,159,25 );  // rgb06A1
  pick[4701].Int( 0,95,191,25 );  // rgb06C1
  pick[4702].Int( 0,95,223,25 );  // rgb06E1
  pick[4703].Int( 0,127,0,25 );  // rgb0801
  pick[4704].Int( 0,127,31,25 );  // rgb0821
  pick[4705].Int( 0,127,63,25 );  // rgb0841
  pick[4706].Int( 0,127,95,25 );  // rgb0861
  pick[4707].Int( 0,127,127,25 );  // rgb0881
  pick[4708].Int( 0,127,159,25 );  // rgb08A1
  pick[4709].Int( 0,127,191,25 );  // rgb08C1
  pick[4710].Int( 0,127,223,25 );  // rgb08E1
  pick[4711].Int( 0,159,0,25 );  // rgb0A01
  pick[4712].Int( 0,159,31,25 );  // rgb0A21
  pick[4713].Int( 0,159,63,25 );  // rgb0A41
  pick[4714].Int( 0,159,95,25 );  // rgb0A61
  pick[4715].Int( 0,159,127,25 );  // rgb0A81
  pick[4716].Int( 0,159,159,25 );  // rgb0AA1
  pick[4717].Int( 0,159,191,25 );  // rgb0AC1
  pick[4718].Int( 0,159,223,25 );  // rgb0AE1
  pick[4719].Int( 0,191,0,25 );  // rgb0C01
  pick[4720].Int( 0,191,31,25 );  // rgb0C21
  pick[4721].Int( 0,191,63,25 );  // rgb0C41
  pick[4722].Int( 0,191,95,25 );  // rgb0C61
  pick[4723].Int( 0,191,127,25 );  // rgb0C81
  pick[4724].Int( 0,191,159,25 );  // rgb0CA1
  pick[4725].Int( 0,191,191,25 );  // rgb0CC1
  pick[4726].Int( 0,191,223,25 );  // rgb0CE1
  pick[4727].Int( 0,223,0,25 );  // rgb0E01
  pick[4728].Int( 0,223,31,25 );  // rgb0E21
  pick[4729].Int( 0,223,63,25 );  // rgb0E41
  pick[4730].Int( 0,223,95,25 );  // rgb0E61
  pick[4731].Int( 0,223,127,25 );  // rgb0E81
  pick[4732].Int( 0,223,159,25 );  // rgb0EA1
  pick[4733].Int( 0,223,191,25 );  // rgb0EC1
  pick[4734].Int( 0,223,223,25 );  // rgb0EE1
  pick[4735].Int( 31,0,0,25 );  // rgb2001
  pick[4736].Int( 31,0,31,25 );  // rgb2021
  pick[4737].Int( 31,0,63,25 );  // rgb2041
  pick[4738].Int( 31,0,95,25 );  // rgb2061
  pick[4739].Int( 31,0,127,25 );  // rgb2081
  pick[4740].Int( 31,0,159,25 );  // rgb20A1
  pick[4741].Int( 31,0,191,25 );  // rgb20C1
  pick[4742].Int( 31,0,223,25 );  // rgb20E1
  pick[4743].Int( 31,31,0,25 );  // rgb2201
  pick[4744].Int( 31,31,31,25 );  // rgb2221
  pick[4745].Int( 31,31,63,25 );  // rgb2241
  pick[4746].Int( 31,31,95,25 );  // rgb2261
  pick[4747].Int( 31,31,127,25 );  // rgb2281
  pick[4748].Int( 31,31,159,25 );  // rgb22A1
  pick[4749].Int( 31,31,191,25 );  // rgb22C1
  pick[4750].Int( 31,31,223,25 );  // rgb22E1
  pick[4751].Int( 31,63,0,25 );  // rgb2401
  pick[4752].Int( 31,63,31,25 );  // rgb2421
  pick[4753].Int( 31,63,63,25 );  // rgb2441
  pick[4754].Int( 31,63,95,25 );  // rgb2461
  pick[4755].Int( 31,63,127,25 );  // rgb2481
  pick[4756].Int( 31,63,159,25 );  // rgb24A1
  pick[4757].Int( 31,63,191,25 );  // rgb24C1
  pick[4758].Int( 31,63,223,25 );  // rgb24E1
  pick[4759].Int( 31,95,0,25 );  // rgb2601
  pick[4760].Int( 31,95,31,25 );  // rgb2621
  pick[4761].Int( 31,95,63,25 );  // rgb2641
  pick[4762].Int( 31,95,95,25 );  // rgb2661
  pick[4763].Int( 31,95,127,25 );  // rgb2681
  pick[4764].Int( 31,95,159,25 );  // rgb26A1
  pick[4765].Int( 31,95,191,25 );  // rgb26C1
  pick[4766].Int( 31,95,223,25 );  // rgb26E1
  pick[4767].Int( 31,127,0,25 );  // rgb2801
  pick[4768].Int( 31,127,31,25 );  // rgb2821
  pick[4769].Int( 31,127,63,25 );  // rgb2841
  pick[4770].Int( 31,127,95,25 );  // rgb2861
  pick[4771].Int( 31,127,127,25 );  // rgb2881
  pick[4772].Int( 31,127,159,25 );  // rgb28A1
  pick[4773].Int( 31,127,191,25 );  // rgb28C1
  pick[4774].Int( 31,127,223,25 );  // rgb28E1
  pick[4775].Int( 31,159,0,25 );  // rgb2A01
  pick[4776].Int( 31,159,31,25 );  // rgb2A21
  pick[4777].Int( 31,159,63,25 );  // rgb2A41
  pick[4778].Int( 31,159,95,25 );  // rgb2A61
  pick[4779].Int( 31,159,127,25 );  // rgb2A81
  pick[4780].Int( 31,159,159,25 );  // rgb2AA1
  pick[4781].Int( 31,159,191,25 );  // rgb2AC1
  pick[4782].Int( 31,159,223,25 );  // rgb2AE1
  pick[4783].Int( 31,191,0,25 );  // rgb2C01
  pick[4784].Int( 31,191,31,25 );  // rgb2C21
  pick[4785].Int( 31,191,63,25 );  // rgb2C41
  pick[4786].Int( 31,191,95,25 );  // rgb2C61
  pick[4787].Int( 31,191,127,25 );  // rgb2C81
  pick[4788].Int( 31,191,159,25 );  // rgb2CA1
  pick[4789].Int( 31,191,191,25 );  // rgb2CC1
  pick[4790].Int( 31,191,223,25 );  // rgb2CE1
  pick[4791].Int( 31,223,0,25 );  // rgb2E01
  pick[4792].Int( 31,223,31,25 );  // rgb2E21
  pick[4793].Int( 31,223,63,25 );  // rgb2E41
  pick[4794].Int( 31,223,95,25 );  // rgb2E61
  pick[4795].Int( 31,223,127,25 );  // rgb2E81
  pick[4796].Int( 31,223,159,25 );  // rgb2EA1
  pick[4797].Int( 31,223,191,25 );  // rgb2EC1
  pick[4798].Int( 31,223,223,25 );  // rgb2EE1
  pick[4799].Int( 63,0,0,25 );  // rgb4001
  pick[4800].Int( 63,0,31,25 );  // rgb4021
  pick[4801].Int( 63,0,63,25 );  // rgb4041
  pick[4802].Int( 63,0,95,25 );  // rgb4061
  pick[4803].Int( 63,0,127,25 );  // rgb4081
  pick[4804].Int( 63,0,159,25 );  // rgb40A1
  pick[4805].Int( 63,0,191,25 );  // rgb40C1
  pick[4806].Int( 63,0,223,25 );  // rgb40E1
  pick[4807].Int( 63,31,0,25 );  // rgb4201
  pick[4808].Int( 63,31,31,25 );  // rgb4221
  pick[4809].Int( 63,31,63,25 );  // rgb4241
  pick[4810].Int( 63,31,95,25 );  // rgb4261
  pick[4811].Int( 63,31,127,25 );  // rgb4281
  pick[4812].Int( 63,31,159,25 );  // rgb42A1
  pick[4813].Int( 63,31,191,25 );  // rgb42C1
  pick[4814].Int( 63,31,223,25 );  // rgb42E1
  pick[4815].Int( 63,63,0,25 );  // rgb4401
  pick[4816].Int( 63,63,31,25 );  // rgb4421
  pick[4817].Int( 63,63,63,25 );  // rgb4441
  pick[4818].Int( 63,63,95,25 );  // rgb4461
  pick[4819].Int( 63,63,127,25 );  // rgb4481
  pick[4820].Int( 63,63,159,25 );  // rgb44A1
  pick[4821].Int( 63,63,191,25 );  // rgb44C1
  pick[4822].Int( 63,63,223,25 );  // rgb44E1
  pick[4823].Int( 63,95,0,25 );  // rgb4601
  pick[4824].Int( 63,95,31,25 );  // rgb4621
  pick[4825].Int( 63,95,63,25 );  // rgb4641
  pick[4826].Int( 63,95,95,25 );  // rgb4661
  pick[4827].Int( 63,95,127,25 );  // rgb4681
  pick[4828].Int( 63,95,159,25 );  // rgb46A1
  pick[4829].Int( 63,95,191,25 );  // rgb46C1
  pick[4830].Int( 63,95,223,25 );  // rgb46E1
  pick[4831].Int( 63,127,0,25 );  // rgb4801
  pick[4832].Int( 63,127,31,25 );  // rgb4821
  pick[4833].Int( 63,127,63,25 );  // rgb4841
  pick[4834].Int( 63,127,95,25 );  // rgb4861
  pick[4835].Int( 63,127,127,25 );  // rgb4881
  pick[4836].Int( 63,127,159,25 );  // rgb48A1
  pick[4837].Int( 63,127,191,25 );  // rgb48C1
  pick[4838].Int( 63,127,223,25 );  // rgb48E1
  pick[4839].Int( 63,159,0,25 );  // rgb4A01
  pick[4840].Int( 63,159,31,25 );  // rgb4A21
  pick[4841].Int( 63,159,63,25 );  // rgb4A41
  pick[4842].Int( 63,159,95,25 );  // rgb4A61
  pick[4843].Int( 63,159,127,25 );  // rgb4A81
  pick[4844].Int( 63,159,159,25 );  // rgb4AA1
  pick[4845].Int( 63,159,191,25 );  // rgb4AC1
  pick[4846].Int( 63,159,223,25 );  // rgb4AE1
  pick[4847].Int( 63,191,0,25 );  // rgb4C01
  pick[4848].Int( 63,191,31,25 );  // rgb4C21
  pick[4849].Int( 63,191,63,25 );  // rgb4C41
  pick[4850].Int( 63,191,95,25 );  // rgb4C61
  pick[4851].Int( 63,191,127,25 );  // rgb4C81
  pick[4852].Int( 63,191,159,25 );  // rgb4CA1
  pick[4853].Int( 63,191,191,25 );  // rgb4CC1
  pick[4854].Int( 63,191,223,25 );  // rgb4CE1
  pick[4855].Int( 63,223,0,25 );  // rgb4E01
  pick[4856].Int( 63,223,31,25 );  // rgb4E21
  pick[4857].Int( 63,223,63,25 );  // rgb4E41
  pick[4858].Int( 63,223,95,25 );  // rgb4E61
  pick[4859].Int( 63,223,127,25 );  // rgb4E81
  pick[4860].Int( 63,223,159,25 );  // rgb4EA1
  pick[4861].Int( 63,223,191,25 );  // rgb4EC1
  pick[4862].Int( 63,223,223,25 );  // rgb4EE1
  pick[4863].Int( 95,0,0,25 );  // rgb6001
  pick[4864].Int( 95,0,31,25 );  // rgb6021
  pick[4865].Int( 95,0,63,25 );  // rgb6041
  pick[4866].Int( 95,0,95,25 );  // rgb6061
  pick[4867].Int( 95,0,127,25 );  // rgb6081
  pick[4868].Int( 95,0,159,25 );  // rgb60A1
  pick[4869].Int( 95,0,191,25 );  // rgb60C1
  pick[4870].Int( 95,0,223,25 );  // rgb60E1
  pick[4871].Int( 95,31,0,25 );  // rgb6201
  pick[4872].Int( 95,31,31,25 );  // rgb6221
  pick[4873].Int( 95,31,63,25 );  // rgb6241
  pick[4874].Int( 95,31,95,25 );  // rgb6261
  pick[4875].Int( 95,31,127,25 );  // rgb6281
  pick[4876].Int( 95,31,159,25 );  // rgb62A1
  pick[4877].Int( 95,31,191,25 );  // rgb62C1
  pick[4878].Int( 95,31,223,25 );  // rgb62E1
  pick[4879].Int( 95,63,0,25 );  // rgb6401
  pick[4880].Int( 95,63,31,25 );  // rgb6421
  pick[4881].Int( 95,63,63,25 );  // rgb6441
  pick[4882].Int( 95,63,95,25 );  // rgb6461
  pick[4883].Int( 95,63,127,25 );  // rgb6481
  pick[4884].Int( 95,63,159,25 );  // rgb64A1
  pick[4885].Int( 95,63,191,25 );  // rgb64C1
  pick[4886].Int( 95,63,223,25 );  // rgb64E1
  pick[4887].Int( 95,95,0,25 );  // rgb6601
  pick[4888].Int( 95,95,31,25 );  // rgb6621
  pick[4889].Int( 95,95,63,25 );  // rgb6641
  pick[4890].Int( 95,95,95,25 );  // rgb6661
  pick[4891].Int( 95,95,127,25 );  // rgb6681
  pick[4892].Int( 95,95,159,25 );  // rgb66A1
  pick[4893].Int( 95,95,191,25 );  // rgb66C1
  pick[4894].Int( 95,95,223,25 );  // rgb66E1
  pick[4895].Int( 95,127,0,25 );  // rgb6801
  pick[4896].Int( 95,127,31,25 );  // rgb6821
  pick[4897].Int( 95,127,63,25 );  // rgb6841
  pick[4898].Int( 95,127,95,25 );  // rgb6861
  pick[4899].Int( 95,127,127,25 );  // rgb6881
  pick[4900].Int( 95,127,159,25 );  // rgb68A1
  pick[4901].Int( 95,127,191,25 );  // rgb68C1
  pick[4902].Int( 95,127,223,25 );  // rgb68E1
  pick[4903].Int( 95,159,0,25 );  // rgb6A01
  pick[4904].Int( 95,159,31,25 );  // rgb6A21
  pick[4905].Int( 95,159,63,25 );  // rgb6A41
  pick[4906].Int( 95,159,95,25 );  // rgb6A61
  pick[4907].Int( 95,159,127,25 );  // rgb6A81
  pick[4908].Int( 95,159,159,25 );  // rgb6AA1
  pick[4909].Int( 95,159,191,25 );  // rgb6AC1
  pick[4910].Int( 95,159,223,25 );  // rgb6AE1
  pick[4911].Int( 95,191,0,25 );  // rgb6C01
  pick[4912].Int( 95,191,31,25 );  // rgb6C21
  pick[4913].Int( 95,191,63,25 );  // rgb6C41
  pick[4914].Int( 95,191,95,25 );  // rgb6C61
  pick[4915].Int( 95,191,127,25 );  // rgb6C81
  pick[4916].Int( 95,191,159,25 );  // rgb6CA1
  pick[4917].Int( 95,191,191,25 );  // rgb6CC1
  pick[4918].Int( 95,191,223,25 );  // rgb6CE1
  pick[4919].Int( 95,223,0,25 );  // rgb6E01
  pick[4920].Int( 95,223,31,25 );  // rgb6E21
  pick[4921].Int( 95,223,63,25 );  // rgb6E41
  pick[4922].Int( 95,223,95,25 );  // rgb6E61
  pick[4923].Int( 95,223,127,25 );  // rgb6E81
  pick[4924].Int( 95,223,159,25 );  // rgb6EA1
  pick[4925].Int( 95,223,191,25 );  // rgb6EC1
  pick[4926].Int( 95,223,223,25 );  // rgb6EE1
  pick[4927].Int( 127,0,0,25 );  // rgb8001
  pick[4928].Int( 127,0,31,25 );  // rgb8021
  pick[4929].Int( 127,0,63,25 );  // rgb8041
  pick[4930].Int( 127,0,95,25 );  // rgb8061
  pick[4931].Int( 127,0,127,25 );  // rgb8081
  pick[4932].Int( 127,0,159,25 );  // rgb80A1
  pick[4933].Int( 127,0,191,25 );  // rgb80C1
  pick[4934].Int( 127,0,223,25 );  // rgb80E1
  pick[4935].Int( 127,31,0,25 );  // rgb8201
  pick[4936].Int( 127,31,31,25 );  // rgb8221
  pick[4937].Int( 127,31,63,25 );  // rgb8241
  pick[4938].Int( 127,31,95,25 );  // rgb8261
  pick[4939].Int( 127,31,127,25 );  // rgb8281
  pick[4940].Int( 127,31,159,25 );  // rgb82A1
  pick[4941].Int( 127,31,191,25 );  // rgb82C1
  pick[4942].Int( 127,31,223,25 );  // rgb82E1
  pick[4943].Int( 127,63,0,25 );  // rgb8401
  pick[4944].Int( 127,63,31,25 );  // rgb8421
  pick[4945].Int( 127,63,63,25 );  // rgb8441
  pick[4946].Int( 127,63,95,25 );  // rgb8461
  pick[4947].Int( 127,63,127,25 );  // rgb8481
  pick[4948].Int( 127,63,159,25 );  // rgb84A1
  pick[4949].Int( 127,63,191,25 );  // rgb84C1
  pick[4950].Int( 127,63,223,25 );  // rgb84E1
  pick[4951].Int( 127,95,0,25 );  // rgb8601
  pick[4952].Int( 127,95,31,25 );  // rgb8621
  pick[4953].Int( 127,95,63,25 );  // rgb8641
  pick[4954].Int( 127,95,95,25 );  // rgb8661
  pick[4955].Int( 127,95,127,25 );  // rgb8681
  pick[4956].Int( 127,95,159,25 );  // rgb86A1
  pick[4957].Int( 127,95,191,25 );  // rgb86C1
  pick[4958].Int( 127,95,223,25 );  // rgb86E1
  pick[4959].Int( 127,127,0,25 );  // rgb8801
  pick[4960].Int( 127,127,31,25 );  // rgb8821
  pick[4961].Int( 127,127,63,25 );  // rgb8841
  pick[4962].Int( 127,127,95,25 );  // rgb8861
  pick[4963].Int( 127,127,127,25 );  // rgb8881
  pick[4964].Int( 127,127,159,25 );  // rgb88A1
  pick[4965].Int( 127,127,191,25 );  // rgb88C1
  pick[4966].Int( 127,127,223,25 );  // rgb88E1
  pick[4967].Int( 127,159,0,25 );  // rgb8A01
  pick[4968].Int( 127,159,31,25 );  // rgb8A21
  pick[4969].Int( 127,159,63,25 );  // rgb8A41
  pick[4970].Int( 127,159,95,25 );  // rgb8A61
  pick[4971].Int( 127,159,127,25 );  // rgb8A81
  pick[4972].Int( 127,159,159,25 );  // rgb8AA1
  pick[4973].Int( 127,159,191,25 );  // rgb8AC1
  pick[4974].Int( 127,159,223,25 );  // rgb8AE1
  pick[4975].Int( 127,191,0,25 );  // rgb8C01
  pick[4976].Int( 127,191,31,25 );  // rgb8C21
  pick[4977].Int( 127,191,63,25 );  // rgb8C41
  pick[4978].Int( 127,191,95,25 );  // rgb8C61
  pick[4979].Int( 127,191,127,25 );  // rgb8C81
  pick[4980].Int( 127,191,159,25 );  // rgb8CA1
  pick[4981].Int( 127,191,191,25 );  // rgb8CC1
  pick[4982].Int( 127,191,223,25 );  // rgb8CE1
  pick[4983].Int( 127,223,0,25 );  // rgb8E01
  pick[4984].Int( 127,223,31,25 );  // rgb8E21
  pick[4985].Int( 127,223,63,25 );  // rgb8E41
  pick[4986].Int( 127,223,95,25 );  // rgb8E61
  pick[4987].Int( 127,223,127,25 );  // rgb8E81
  pick[4988].Int( 127,223,159,25 );  // rgb8EA1
  pick[4989].Int( 127,223,191,25 );  // rgb8EC1
  pick[4990].Int( 127,223,223,25 );  // rgb8EE1
  pick[4991].Int( 159,0,0,25 );  // rgbA001
  pick[4992].Int( 159,0,31,25 );  // rgbA021
  pick[4993].Int( 159,0,63,25 );  // rgbA041
  pick[4994].Int( 159,0,95,25 );  // rgbA061
  pick[4995].Int( 159,0,127,25 );  // rgbA081
  pick[4996].Int( 159,0,159,25 );  // rgbA0A1
  pick[4997].Int( 159,0,191,25 );  // rgbA0C1
  pick[4998].Int( 159,0,223,25 );  // rgbA0E1
  pick[4999].Int( 159,31,0,25 );  // rgbA201
  pick[5000].Int( 159,31,31,25 );  // rgbA221
  pick[5001].Int( 159,31,63,25 );  // rgbA241
  pick[5002].Int( 159,31,95,25 );  // rgbA261
  pick[5003].Int( 159,31,127,25 );  // rgbA281
  pick[5004].Int( 159,31,159,25 );  // rgbA2A1
  pick[5005].Int( 159,31,191,25 );  // rgbA2C1
  pick[5006].Int( 159,31,223,25 );  // rgbA2E1
  pick[5007].Int( 159,63,0,25 );  // rgbA401
  pick[5008].Int( 159,63,31,25 );  // rgbA421
  pick[5009].Int( 159,63,63,25 );  // rgbA441
  pick[5010].Int( 159,63,95,25 );  // rgbA461
  pick[5011].Int( 159,63,127,25 );  // rgbA481
  pick[5012].Int( 159,63,159,25 );  // rgbA4A1
  pick[5013].Int( 159,63,191,25 );  // rgbA4C1
  pick[5014].Int( 159,63,223,25 );  // rgbA4E1
  pick[5015].Int( 159,95,0,25 );  // rgbA601
  pick[5016].Int( 159,95,31,25 );  // rgbA621
  pick[5017].Int( 159,95,63,25 );  // rgbA641
  pick[5018].Int( 159,95,95,25 );  // rgbA661
  pick[5019].Int( 159,95,127,25 );  // rgbA681
  pick[5020].Int( 159,95,159,25 );  // rgbA6A1
  pick[5021].Int( 159,95,191,25 );  // rgbA6C1
  pick[5022].Int( 159,95,223,25 );  // rgbA6E1
  pick[5023].Int( 159,127,0,25 );  // rgbA801
  pick[5024].Int( 159,127,31,25 );  // rgbA821
  pick[5025].Int( 159,127,63,25 );  // rgbA841
  pick[5026].Int( 159,127,95,25 );  // rgbA861
  pick[5027].Int( 159,127,127,25 );  // rgbA881
  pick[5028].Int( 159,127,159,25 );  // rgbA8A1
  pick[5029].Int( 159,127,191,25 );  // rgbA8C1
  pick[5030].Int( 159,127,223,25 );  // rgbA8E1
  pick[5031].Int( 159,159,0,25 );  // rgbAA01
  pick[5032].Int( 159,159,31,25 );  // rgbAA21
  pick[5033].Int( 159,159,63,25 );  // rgbAA41
  pick[5034].Int( 159,159,95,25 );  // rgbAA61
  pick[5035].Int( 159,159,127,25 );  // rgbAA81
  pick[5036].Int( 159,159,159,25 );  // rgbAAA1
  pick[5037].Int( 159,159,191,25 );  // rgbAAC1
  pick[5038].Int( 159,159,223,25 );  // rgbAAE1
  pick[5039].Int( 159,191,0,25 );  // rgbAC01
  pick[5040].Int( 159,191,31,25 );  // rgbAC21
  pick[5041].Int( 159,191,63,25 );  // rgbAC41
  pick[5042].Int( 159,191,95,25 );  // rgbAC61
  pick[5043].Int( 159,191,127,25 );  // rgbAC81
  pick[5044].Int( 159,191,159,25 );  // rgbACA1
  pick[5045].Int( 159,191,191,25 );  // rgbACC1
  pick[5046].Int( 159,191,223,25 );  // rgbACE1
  pick[5047].Int( 159,223,0,25 );  // rgbAE01
  pick[5048].Int( 159,223,31,25 );  // rgbAE21
  pick[5049].Int( 159,223,63,25 );  // rgbAE41
  pick[5050].Int( 159,223,95,25 );  // rgbAE61
  pick[5051].Int( 159,223,127,25 );  // rgbAE81
  pick[5052].Int( 159,223,159,25 );  // rgbAEA1
  pick[5053].Int( 159,223,191,25 );  // rgbAEC1
  pick[5054].Int( 159,223,223,25 );  // rgbAEE1
  pick[5055].Int( 191,0,0,25 );  // rgbC001
  pick[5056].Int( 191,0,31,25 );  // rgbC021
  pick[5057].Int( 191,0,63,25 );  // rgbC041
  pick[5058].Int( 191,0,95,25 );  // rgbC061
  pick[5059].Int( 191,0,127,25 );  // rgbC081
  pick[5060].Int( 191,0,159,25 );  // rgbC0A1
  pick[5061].Int( 191,0,191,25 );  // rgbC0C1
  pick[5062].Int( 191,0,223,25 );  // rgbC0E1
  pick[5063].Int( 191,31,0,25 );  // rgbC201
  pick[5064].Int( 191,31,31,25 );  // rgbC221
  pick[5065].Int( 191,31,63,25 );  // rgbC241
  pick[5066].Int( 191,31,95,25 );  // rgbC261
  pick[5067].Int( 191,31,127,25 );  // rgbC281
  pick[5068].Int( 191,31,159,25 );  // rgbC2A1
  pick[5069].Int( 191,31,191,25 );  // rgbC2C1
  pick[5070].Int( 191,31,223,25 );  // rgbC2E1
  pick[5071].Int( 191,63,0,25 );  // rgbC401
  pick[5072].Int( 191,63,31,25 );  // rgbC421
  pick[5073].Int( 191,63,63,25 );  // rgbC441
  pick[5074].Int( 191,63,95,25 );  // rgbC461
  pick[5075].Int( 191,63,127,25 );  // rgbC481
  pick[5076].Int( 191,63,159,25 );  // rgbC4A1
  pick[5077].Int( 191,63,191,25 );  // rgbC4C1
  pick[5078].Int( 191,63,223,25 );  // rgbC4E1
  pick[5079].Int( 191,95,0,25 );  // rgbC601
  pick[5080].Int( 191,95,31,25 );  // rgbC621
  pick[5081].Int( 191,95,63,25 );  // rgbC641
  pick[5082].Int( 191,95,95,25 );  // rgbC661
  pick[5083].Int( 191,95,127,25 );  // rgbC681
  pick[5084].Int( 191,95,159,25 );  // rgbC6A1
  pick[5085].Int( 191,95,191,25 );  // rgbC6C1
  pick[5086].Int( 191,95,223,25 );  // rgbC6E1
  pick[5087].Int( 191,127,0,25 );  // rgbC801
  pick[5088].Int( 191,127,31,25 );  // rgbC821
  pick[5089].Int( 191,127,63,25 );  // rgbC841
  pick[5090].Int( 191,127,95,25 );  // rgbC861
  pick[5091].Int( 191,127,127,25 );  // rgbC881
  pick[5092].Int( 191,127,159,25 );  // rgbC8A1
  pick[5093].Int( 191,127,191,25 );  // rgbC8C1
  pick[5094].Int( 191,127,223,25 );  // rgbC8E1
  pick[5095].Int( 191,159,0,25 );  // rgbCA01
  pick[5096].Int( 191,159,31,25 );  // rgbCA21
  pick[5097].Int( 191,159,63,25 );  // rgbCA41
  pick[5098].Int( 191,159,95,25 );  // rgbCA61
  pick[5099].Int( 191,159,127,25 );  // rgbCA81
  pick[5100].Int( 191,159,159,25 );  // rgbCAA1
  pick[5101].Int( 191,159,191,25 );  // rgbCAC1
  pick[5102].Int( 191,159,223,25 );  // rgbCAE1
  pick[5103].Int( 191,191,0,25 );  // rgbCC01
  pick[5104].Int( 191,191,31,25 );  // rgbCC21
  pick[5105].Int( 191,191,63,25 );  // rgbCC41
  pick[5106].Int( 191,191,95,25 );  // rgbCC61
  pick[5107].Int( 191,191,127,25 );  // rgbCC81
  pick[5108].Int( 191,191,159,25 );  // rgbCCA1
  pick[5109].Int( 191,191,191,25 );  // rgbCCC1
  pick[5110].Int( 191,191,223,25 );  // rgbCCE1
  pick[5111].Int( 191,223,0,25 );  // rgbCE01
  pick[5112].Int( 191,223,31,25 );  // rgbCE21
  pick[5113].Int( 191,223,63,25 );  // rgbCE41
  pick[5114].Int( 191,223,95,25 );  // rgbCE61
  pick[5115].Int( 191,223,127,25 );  // rgbCE81
  pick[5116].Int( 191,223,159,25 );  // rgbCEA1
  pick[5117].Int( 191,223,191,25 );  // rgbCEC1
  pick[5118].Int( 191,223,223,25 );  // rgbCEE1
  pick[5119].Int( 223,0,0,25 );  // rgbE001
  pick[5120].Int( 223,0,31,25 );  // rgbE021
  pick[5121].Int( 223,0,63,25 );  // rgbE041
  pick[5122].Int( 223,0,95,25 );  // rgbE061
  pick[5123].Int( 223,0,127,25 );  // rgbE081
  pick[5124].Int( 223,0,159,25 );  // rgbE0A1
  pick[5125].Int( 223,0,191,25 );  // rgbE0C1
  pick[5126].Int( 223,0,223,25 );  // rgbE0E1
  pick[5127].Int( 223,31,0,25 );  // rgbE201
  pick[5128].Int( 223,31,31,25 );  // rgbE221
  pick[5129].Int( 223,31,63,25 );  // rgbE241
  pick[5130].Int( 223,31,95,25 );  // rgbE261
  pick[5131].Int( 223,31,127,25 );  // rgbE281
  pick[5132].Int( 223,31,159,25 );  // rgbE2A1
  pick[5133].Int( 223,31,191,25 );  // rgbE2C1
  pick[5134].Int( 223,31,223,25 );  // rgbE2E1
  pick[5135].Int( 223,63,0,25 );  // rgbE401
  pick[5136].Int( 223,63,31,25 );  // rgbE421
  pick[5137].Int( 223,63,63,25 );  // rgbE441
  pick[5138].Int( 223,63,95,25 );  // rgbE461
  pick[5139].Int( 223,63,127,25 );  // rgbE481
  pick[5140].Int( 223,63,159,25 );  // rgbE4A1
  pick[5141].Int( 223,63,191,25 );  // rgbE4C1
  pick[5142].Int( 223,63,223,25 );  // rgbE4E1
  pick[5143].Int( 223,95,0,25 );  // rgbE601
  pick[5144].Int( 223,95,31,25 );  // rgbE621
  pick[5145].Int( 223,95,63,25 );  // rgbE641
  pick[5146].Int( 223,95,95,25 );  // rgbE661
  pick[5147].Int( 223,95,127,25 );  // rgbE681
  pick[5148].Int( 223,95,159,25 );  // rgbE6A1
  pick[5149].Int( 223,95,191,25 );  // rgbE6C1
  pick[5150].Int( 223,95,223,25 );  // rgbE6E1
  pick[5151].Int( 223,127,0,25 );  // rgbE801
  pick[5152].Int( 223,127,31,25 );  // rgbE821
  pick[5153].Int( 223,127,63,25 );  // rgbE841
  pick[5154].Int( 223,127,95,25 );  // rgbE861
  pick[5155].Int( 223,127,127,25 );  // rgbE881
  pick[5156].Int( 223,127,159,25 );  // rgbE8A1
  pick[5157].Int( 223,127,191,25 );  // rgbE8C1
  pick[5158].Int( 223,127,223,25 );  // rgbE8E1
  pick[5159].Int( 223,159,0,25 );  // rgbEA01
  pick[5160].Int( 223,159,31,25 );  // rgbEA21
  pick[5161].Int( 223,159,63,25 );  // rgbEA41
  pick[5162].Int( 223,159,95,25 );  // rgbEA61
  pick[5163].Int( 223,159,127,25 );  // rgbEA81
  pick[5164].Int( 223,159,159,25 );  // rgbEAA1
  pick[5165].Int( 223,159,191,25 );  // rgbEAC1
  pick[5166].Int( 223,159,223,25 );  // rgbEAE1
  pick[5167].Int( 223,191,0,25 );  // rgbEC01
  pick[5168].Int( 223,191,31,25 );  // rgbEC21
  pick[5169].Int( 223,191,63,25 );  // rgbEC41
  pick[5170].Int( 223,191,95,25 );  // rgbEC61
  pick[5171].Int( 223,191,127,25 );  // rgbEC81
  pick[5172].Int( 223,191,159,25 );  // rgbECA1
  pick[5173].Int( 223,191,191,25 );  // rgbECC1
  pick[5174].Int( 223,191,223,25 );  // rgbECE1
  pick[5175].Int( 223,223,0,25 );  // rgbEE01
  pick[5176].Int( 223,223,31,25 );  // rgbEE21
  pick[5177].Int( 223,223,63,25 );  // rgbEE41
  pick[5178].Int( 223,223,95,25 );  // rgbEE61
  pick[5179].Int( 223,223,127,25 );  // rgbEE81
  pick[5180].Int( 223,223,159,25 );  // rgbEEA1
  pick[5181].Int( 223,223,191,25 );  // rgbEEC1
  pick[5182].Int( 223,223,223,25 );  // rgbEEE1
  pick[5183].Int( 0,0,0,51 );  // rgb0002
  pick[5184].Int( 0,0,31,51 );  // rgb0022
  pick[5185].Int( 0,0,63,51 );  // rgb0042
  pick[5186].Int( 0,0,95,51 );  // rgb0062
  pick[5187].Int( 0,0,127,51 );  // rgb0082
  pick[5188].Int( 0,0,159,51 );  // rgb00A2
  pick[5189].Int( 0,0,191,51 );  // rgb00C2
  pick[5190].Int( 0,0,223,51 );  // rgb00E2
  pick[5191].Int( 0,31,0,51 );  // rgb0202
  pick[5192].Int( 0,31,31,51 );  // rgb0222
  pick[5193].Int( 0,31,63,51 );  // rgb0242
  pick[5194].Int( 0,31,95,51 );  // rgb0262
  pick[5195].Int( 0,31,127,51 );  // rgb0282
  pick[5196].Int( 0,31,159,51 );  // rgb02A2
  pick[5197].Int( 0,31,191,51 );  // rgb02C2
  pick[5198].Int( 0,31,223,51 );  // rgb02E2
  pick[5199].Int( 0,63,0,51 );  // rgb0402
  pick[5200].Int( 0,63,31,51 );  // rgb0422
  pick[5201].Int( 0,63,63,51 );  // rgb0442
  pick[5202].Int( 0,63,95,51 );  // rgb0462
  pick[5203].Int( 0,63,127,51 );  // rgb0482
  pick[5204].Int( 0,63,159,51 );  // rgb04A2
  pick[5205].Int( 0,63,191,51 );  // rgb04C2
  pick[5206].Int( 0,63,223,51 );  // rgb04E2
  pick[5207].Int( 0,95,0,51 );  // rgb0602
  pick[5208].Int( 0,95,31,51 );  // rgb0622
  pick[5209].Int( 0,95,63,51 );  // rgb0642
  pick[5210].Int( 0,95,95,51 );  // rgb0662
  pick[5211].Int( 0,95,127,51 );  // rgb0682
  pick[5212].Int( 0,95,159,51 );  // rgb06A2
  pick[5213].Int( 0,95,191,51 );  // rgb06C2
  pick[5214].Int( 0,95,223,51 );  // rgb06E2
  pick[5215].Int( 0,127,0,51 );  // rgb0802
  pick[5216].Int( 0,127,31,51 );  // rgb0822
  pick[5217].Int( 0,127,63,51 );  // rgb0842
  pick[5218].Int( 0,127,95,51 );  // rgb0862
  pick[5219].Int( 0,127,127,51 );  // rgb0882
  pick[5220].Int( 0,127,159,51 );  // rgb08A2
  pick[5221].Int( 0,127,191,51 );  // rgb08C2
  pick[5222].Int( 0,127,223,51 );  // rgb08E2
  pick[5223].Int( 0,159,0,51 );  // rgb0A02
  pick[5224].Int( 0,159,31,51 );  // rgb0A22
  pick[5225].Int( 0,159,63,51 );  // rgb0A42
  pick[5226].Int( 0,159,95,51 );  // rgb0A62
  pick[5227].Int( 0,159,127,51 );  // rgb0A82
  pick[5228].Int( 0,159,159,51 );  // rgb0AA2
  pick[5229].Int( 0,159,191,51 );  // rgb0AC2
  pick[5230].Int( 0,159,223,51 );  // rgb0AE2
  pick[5231].Int( 0,191,0,51 );  // rgb0C02
  pick[5232].Int( 0,191,31,51 );  // rgb0C22
  pick[5233].Int( 0,191,63,51 );  // rgb0C42
  pick[5234].Int( 0,191,95,51 );  // rgb0C62
  pick[5235].Int( 0,191,127,51 );  // rgb0C82
  pick[5236].Int( 0,191,159,51 );  // rgb0CA2
  pick[5237].Int( 0,191,191,51 );  // rgb0CC2
  pick[5238].Int( 0,191,223,51 );  // rgb0CE2
  pick[5239].Int( 0,223,0,51 );  // rgb0E02
  pick[5240].Int( 0,223,31,51 );  // rgb0E22
  pick[5241].Int( 0,223,63,51 );  // rgb0E42
  pick[5242].Int( 0,223,95,51 );  // rgb0E62
  pick[5243].Int( 0,223,127,51 );  // rgb0E82
  pick[5244].Int( 0,223,159,51 );  // rgb0EA2
  pick[5245].Int( 0,223,191,51 );  // rgb0EC2
  pick[5246].Int( 0,223,223,51 );  // rgb0EE2
  pick[5247].Int( 31,0,0,51 );  // rgb2002
  pick[5248].Int( 31,0,31,51 );  // rgb2022
  pick[5249].Int( 31,0,63,51 );  // rgb2042
  pick[5250].Int( 31,0,95,51 );  // rgb2062
  pick[5251].Int( 31,0,127,51 );  // rgb2082
  pick[5252].Int( 31,0,159,51 );  // rgb20A2
  pick[5253].Int( 31,0,191,51 );  // rgb20C2
  pick[5254].Int( 31,0,223,51 );  // rgb20E2
  pick[5255].Int( 31,31,0,51 );  // rgb2202
  pick[5256].Int( 31,31,31,51 );  // rgb2222
  pick[5257].Int( 31,31,63,51 );  // rgb2242
  pick[5258].Int( 31,31,95,51 );  // rgb2262
  pick[5259].Int( 31,31,127,51 );  // rgb2282
  pick[5260].Int( 31,31,159,51 );  // rgb22A2
  pick[5261].Int( 31,31,191,51 );  // rgb22C2
  pick[5262].Int( 31,31,223,51 );  // rgb22E2
  pick[5263].Int( 31,63,0,51 );  // rgb2402
  pick[5264].Int( 31,63,31,51 );  // rgb2422
  pick[5265].Int( 31,63,63,51 );  // rgb2442
  pick[5266].Int( 31,63,95,51 );  // rgb2462
  pick[5267].Int( 31,63,127,51 );  // rgb2482
  pick[5268].Int( 31,63,159,51 );  // rgb24A2
  pick[5269].Int( 31,63,191,51 );  // rgb24C2
  pick[5270].Int( 31,63,223,51 );  // rgb24E2
  pick[5271].Int( 31,95,0,51 );  // rgb2602
  pick[5272].Int( 31,95,31,51 );  // rgb2622
  pick[5273].Int( 31,95,63,51 );  // rgb2642
  pick[5274].Int( 31,95,95,51 );  // rgb2662
  pick[5275].Int( 31,95,127,51 );  // rgb2682
  pick[5276].Int( 31,95,159,51 );  // rgb26A2
  pick[5277].Int( 31,95,191,51 );  // rgb26C2
  pick[5278].Int( 31,95,223,51 );  // rgb26E2
  pick[5279].Int( 31,127,0,51 );  // rgb2802
  pick[5280].Int( 31,127,31,51 );  // rgb2822
  pick[5281].Int( 31,127,63,51 );  // rgb2842
  pick[5282].Int( 31,127,95,51 );  // rgb2862
  pick[5283].Int( 31,127,127,51 );  // rgb2882
  pick[5284].Int( 31,127,159,51 );  // rgb28A2
  pick[5285].Int( 31,127,191,51 );  // rgb28C2
  pick[5286].Int( 31,127,223,51 );  // rgb28E2
  pick[5287].Int( 31,159,0,51 );  // rgb2A02
  pick[5288].Int( 31,159,31,51 );  // rgb2A22
  pick[5289].Int( 31,159,63,51 );  // rgb2A42
  pick[5290].Int( 31,159,95,51 );  // rgb2A62
  pick[5291].Int( 31,159,127,51 );  // rgb2A82
  pick[5292].Int( 31,159,159,51 );  // rgb2AA2
  pick[5293].Int( 31,159,191,51 );  // rgb2AC2
  pick[5294].Int( 31,159,223,51 );  // rgb2AE2
  pick[5295].Int( 31,191,0,51 );  // rgb2C02
  pick[5296].Int( 31,191,31,51 );  // rgb2C22
  pick[5297].Int( 31,191,63,51 );  // rgb2C42
  pick[5298].Int( 31,191,95,51 );  // rgb2C62
  pick[5299].Int( 31,191,127,51 );  // rgb2C82
  pick[5300].Int( 31,191,159,51 );  // rgb2CA2
  pick[5301].Int( 31,191,191,51 );  // rgb2CC2
  pick[5302].Int( 31,191,223,51 );  // rgb2CE2
  pick[5303].Int( 31,223,0,51 );  // rgb2E02
  pick[5304].Int( 31,223,31,51 );  // rgb2E22
  pick[5305].Int( 31,223,63,51 );  // rgb2E42
  pick[5306].Int( 31,223,95,51 );  // rgb2E62
  pick[5307].Int( 31,223,127,51 );  // rgb2E82
  pick[5308].Int( 31,223,159,51 );  // rgb2EA2
  pick[5309].Int( 31,223,191,51 );  // rgb2EC2
  pick[5310].Int( 31,223,223,51 );  // rgb2EE2
  pick[5311].Int( 63,0,0,51 );  // rgb4002
  pick[5312].Int( 63,0,31,51 );  // rgb4022
  pick[5313].Int( 63,0,63,51 );  // rgb4042
  pick[5314].Int( 63,0,95,51 );  // rgb4062
  pick[5315].Int( 63,0,127,51 );  // rgb4082
  pick[5316].Int( 63,0,159,51 );  // rgb40A2
  pick[5317].Int( 63,0,191,51 );  // rgb40C2
  pick[5318].Int( 63,0,223,51 );  // rgb40E2
  pick[5319].Int( 63,31,0,51 );  // rgb4202
  pick[5320].Int( 63,31,31,51 );  // rgb4222
  pick[5321].Int( 63,31,63,51 );  // rgb4242
  pick[5322].Int( 63,31,95,51 );  // rgb4262
  pick[5323].Int( 63,31,127,51 );  // rgb4282
  pick[5324].Int( 63,31,159,51 );  // rgb42A2
  pick[5325].Int( 63,31,191,51 );  // rgb42C2
  pick[5326].Int( 63,31,223,51 );  // rgb42E2
  pick[5327].Int( 63,63,0,51 );  // rgb4402
  pick[5328].Int( 63,63,31,51 );  // rgb4422
  pick[5329].Int( 63,63,63,51 );  // rgb4442
  pick[5330].Int( 63,63,95,51 );  // rgb4462
  pick[5331].Int( 63,63,127,51 );  // rgb4482
  pick[5332].Int( 63,63,159,51 );  // rgb44A2
  pick[5333].Int( 63,63,191,51 );  // rgb44C2
  pick[5334].Int( 63,63,223,51 );  // rgb44E2
  pick[5335].Int( 63,95,0,51 );  // rgb4602
  pick[5336].Int( 63,95,31,51 );  // rgb4622
  pick[5337].Int( 63,95,63,51 );  // rgb4642
  pick[5338].Int( 63,95,95,51 );  // rgb4662
  pick[5339].Int( 63,95,127,51 );  // rgb4682
  pick[5340].Int( 63,95,159,51 );  // rgb46A2
  pick[5341].Int( 63,95,191,51 );  // rgb46C2
  pick[5342].Int( 63,95,223,51 );  // rgb46E2
  pick[5343].Int( 63,127,0,51 );  // rgb4802
  pick[5344].Int( 63,127,31,51 );  // rgb4822
  pick[5345].Int( 63,127,63,51 );  // rgb4842
  pick[5346].Int( 63,127,95,51 );  // rgb4862
  pick[5347].Int( 63,127,127,51 );  // rgb4882
  pick[5348].Int( 63,127,159,51 );  // rgb48A2
  pick[5349].Int( 63,127,191,51 );  // rgb48C2
  pick[5350].Int( 63,127,223,51 );  // rgb48E2
  pick[5351].Int( 63,159,0,51 );  // rgb4A02
  pick[5352].Int( 63,159,31,51 );  // rgb4A22
  pick[5353].Int( 63,159,63,51 );  // rgb4A42
  pick[5354].Int( 63,159,95,51 );  // rgb4A62
  pick[5355].Int( 63,159,127,51 );  // rgb4A82
  pick[5356].Int( 63,159,159,51 );  // rgb4AA2
  pick[5357].Int( 63,159,191,51 );  // rgb4AC2
  pick[5358].Int( 63,159,223,51 );  // rgb4AE2
  pick[5359].Int( 63,191,0,51 );  // rgb4C02
  pick[5360].Int( 63,191,31,51 );  // rgb4C22
  pick[5361].Int( 63,191,63,51 );  // rgb4C42
  pick[5362].Int( 63,191,95,51 );  // rgb4C62
  pick[5363].Int( 63,191,127,51 );  // rgb4C82
  pick[5364].Int( 63,191,159,51 );  // rgb4CA2
  pick[5365].Int( 63,191,191,51 );  // rgb4CC2
  pick[5366].Int( 63,191,223,51 );  // rgb4CE2
  pick[5367].Int( 63,223,0,51 );  // rgb4E02
  pick[5368].Int( 63,223,31,51 );  // rgb4E22
  pick[5369].Int( 63,223,63,51 );  // rgb4E42
  pick[5370].Int( 63,223,95,51 );  // rgb4E62
  pick[5371].Int( 63,223,127,51 );  // rgb4E82
  pick[5372].Int( 63,223,159,51 );  // rgb4EA2
  pick[5373].Int( 63,223,191,51 );  // rgb4EC2
  pick[5374].Int( 63,223,223,51 );  // rgb4EE2
  pick[5375].Int( 95,0,0,51 );  // rgb6002
  pick[5376].Int( 95,0,31,51 );  // rgb6022
  pick[5377].Int( 95,0,63,51 );  // rgb6042
  pick[5378].Int( 95,0,95,51 );  // rgb6062
  pick[5379].Int( 95,0,127,51 );  // rgb6082
  pick[5380].Int( 95,0,159,51 );  // rgb60A2
  pick[5381].Int( 95,0,191,51 );  // rgb60C2
  pick[5382].Int( 95,0,223,51 );  // rgb60E2
  pick[5383].Int( 95,31,0,51 );  // rgb6202
  pick[5384].Int( 95,31,31,51 );  // rgb6222
  pick[5385].Int( 95,31,63,51 );  // rgb6242
  pick[5386].Int( 95,31,95,51 );  // rgb6262
  pick[5387].Int( 95,31,127,51 );  // rgb6282
  pick[5388].Int( 95,31,159,51 );  // rgb62A2
  pick[5389].Int( 95,31,191,51 );  // rgb62C2
  pick[5390].Int( 95,31,223,51 );  // rgb62E2
  pick[5391].Int( 95,63,0,51 );  // rgb6402
  pick[5392].Int( 95,63,31,51 );  // rgb6422
  pick[5393].Int( 95,63,63,51 );  // rgb6442
  pick[5394].Int( 95,63,95,51 );  // rgb6462
  pick[5395].Int( 95,63,127,51 );  // rgb6482
  pick[5396].Int( 95,63,159,51 );  // rgb64A2
  pick[5397].Int( 95,63,191,51 );  // rgb64C2
  pick[5398].Int( 95,63,223,51 );  // rgb64E2
  pick[5399].Int( 95,95,0,51 );  // rgb6602
  pick[5400].Int( 95,95,31,51 );  // rgb6622
  pick[5401].Int( 95,95,63,51 );  // rgb6642
  pick[5402].Int( 95,95,95,51 );  // rgb6662
  pick[5403].Int( 95,95,127,51 );  // rgb6682
  pick[5404].Int( 95,95,159,51 );  // rgb66A2
  pick[5405].Int( 95,95,191,51 );  // rgb66C2
  pick[5406].Int( 95,95,223,51 );  // rgb66E2
  pick[5407].Int( 95,127,0,51 );  // rgb6802
  pick[5408].Int( 95,127,31,51 );  // rgb6822
  pick[5409].Int( 95,127,63,51 );  // rgb6842
  pick[5410].Int( 95,127,95,51 );  // rgb6862
  pick[5411].Int( 95,127,127,51 );  // rgb6882
  pick[5412].Int( 95,127,159,51 );  // rgb68A2
  pick[5413].Int( 95,127,191,51 );  // rgb68C2
  pick[5414].Int( 95,127,223,51 );  // rgb68E2
  pick[5415].Int( 95,159,0,51 );  // rgb6A02
  pick[5416].Int( 95,159,31,51 );  // rgb6A22
  pick[5417].Int( 95,159,63,51 );  // rgb6A42
  pick[5418].Int( 95,159,95,51 );  // rgb6A62
  pick[5419].Int( 95,159,127,51 );  // rgb6A82
  pick[5420].Int( 95,159,159,51 );  // rgb6AA2
  pick[5421].Int( 95,159,191,51 );  // rgb6AC2
  pick[5422].Int( 95,159,223,51 );  // rgb6AE2
  pick[5423].Int( 95,191,0,51 );  // rgb6C02
  pick[5424].Int( 95,191,31,51 );  // rgb6C22
  pick[5425].Int( 95,191,63,51 );  // rgb6C42
  pick[5426].Int( 95,191,95,51 );  // rgb6C62
  pick[5427].Int( 95,191,127,51 );  // rgb6C82
  pick[5428].Int( 95,191,159,51 );  // rgb6CA2
  pick[5429].Int( 95,191,191,51 );  // rgb6CC2
  pick[5430].Int( 95,191,223,51 );  // rgb6CE2
  pick[5431].Int( 95,223,0,51 );  // rgb6E02
  pick[5432].Int( 95,223,31,51 );  // rgb6E22
  pick[5433].Int( 95,223,63,51 );  // rgb6E42
  pick[5434].Int( 95,223,95,51 );  // rgb6E62
  pick[5435].Int( 95,223,127,51 );  // rgb6E82
  pick[5436].Int( 95,223,159,51 );  // rgb6EA2
  pick[5437].Int( 95,223,191,51 );  // rgb6EC2
  pick[5438].Int( 95,223,223,51 );  // rgb6EE2
  pick[5439].Int( 127,0,0,51 );  // rgb8002
  pick[5440].Int( 127,0,31,51 );  // rgb8022
  pick[5441].Int( 127,0,63,51 );  // rgb8042
  pick[5442].Int( 127,0,95,51 );  // rgb8062
  pick[5443].Int( 127,0,127,51 );  // rgb8082
  pick[5444].Int( 127,0,159,51 );  // rgb80A2
  pick[5445].Int( 127,0,191,51 );  // rgb80C2
  pick[5446].Int( 127,0,223,51 );  // rgb80E2
  pick[5447].Int( 127,31,0,51 );  // rgb8202
  pick[5448].Int( 127,31,31,51 );  // rgb8222
  pick[5449].Int( 127,31,63,51 );  // rgb8242
  pick[5450].Int( 127,31,95,51 );  // rgb8262
  pick[5451].Int( 127,31,127,51 );  // rgb8282
  pick[5452].Int( 127,31,159,51 );  // rgb82A2
  pick[5453].Int( 127,31,191,51 );  // rgb82C2
  pick[5454].Int( 127,31,223,51 );  // rgb82E2
  pick[5455].Int( 127,63,0,51 );  // rgb8402
  pick[5456].Int( 127,63,31,51 );  // rgb8422
  pick[5457].Int( 127,63,63,51 );  // rgb8442
  pick[5458].Int( 127,63,95,51 );  // rgb8462
  pick[5459].Int( 127,63,127,51 );  // rgb8482
  pick[5460].Int( 127,63,159,51 );  // rgb84A2
  pick[5461].Int( 127,63,191,51 );  // rgb84C2
  pick[5462].Int( 127,63,223,51 );  // rgb84E2
  pick[5463].Int( 127,95,0,51 );  // rgb8602
  pick[5464].Int( 127,95,31,51 );  // rgb8622
  pick[5465].Int( 127,95,63,51 );  // rgb8642
  pick[5466].Int( 127,95,95,51 );  // rgb8662
  pick[5467].Int( 127,95,127,51 );  // rgb8682
  pick[5468].Int( 127,95,159,51 );  // rgb86A2
  pick[5469].Int( 127,95,191,51 );  // rgb86C2
  pick[5470].Int( 127,95,223,51 );  // rgb86E2
  pick[5471].Int( 127,127,0,51 );  // rgb8802
  pick[5472].Int( 127,127,31,51 );  // rgb8822
  pick[5473].Int( 127,127,63,51 );  // rgb8842
  pick[5474].Int( 127,127,95,51 );  // rgb8862
  pick[5475].Int( 127,127,127,51 );  // rgb8882
  pick[5476].Int( 127,127,159,51 );  // rgb88A2
  pick[5477].Int( 127,127,191,51 );  // rgb88C2
  pick[5478].Int( 127,127,223,51 );  // rgb88E2
  pick[5479].Int( 127,159,0,51 );  // rgb8A02
  pick[5480].Int( 127,159,31,51 );  // rgb8A22
  pick[5481].Int( 127,159,63,51 );  // rgb8A42
  pick[5482].Int( 127,159,95,51 );  // rgb8A62
  pick[5483].Int( 127,159,127,51 );  // rgb8A82
  pick[5484].Int( 127,159,159,51 );  // rgb8AA2
  pick[5485].Int( 127,159,191,51 );  // rgb8AC2
  pick[5486].Int( 127,159,223,51 );  // rgb8AE2
  pick[5487].Int( 127,191,0,51 );  // rgb8C02
  pick[5488].Int( 127,191,31,51 );  // rgb8C22
  pick[5489].Int( 127,191,63,51 );  // rgb8C42
  pick[5490].Int( 127,191,95,51 );  // rgb8C62
  pick[5491].Int( 127,191,127,51 );  // rgb8C82
  pick[5492].Int( 127,191,159,51 );  // rgb8CA2
  pick[5493].Int( 127,191,191,51 );  // rgb8CC2
  pick[5494].Int( 127,191,223,51 );  // rgb8CE2
  pick[5495].Int( 127,223,0,51 );  // rgb8E02
  pick[5496].Int( 127,223,31,51 );  // rgb8E22
  pick[5497].Int( 127,223,63,51 );  // rgb8E42
  pick[5498].Int( 127,223,95,51 );  // rgb8E62
  pick[5499].Int( 127,223,127,51 );  // rgb8E82
  pick[5500].Int( 127,223,159,51 );  // rgb8EA2
  pick[5501].Int( 127,223,191,51 );  // rgb8EC2
  pick[5502].Int( 127,223,223,51 );  // rgb8EE2
  pick[5503].Int( 159,0,0,51 );  // rgbA002
  pick[5504].Int( 159,0,31,51 );  // rgbA022
  pick[5505].Int( 159,0,63,51 );  // rgbA042
  pick[5506].Int( 159,0,95,51 );  // rgbA062
  pick[5507].Int( 159,0,127,51 );  // rgbA082
  pick[5508].Int( 159,0,159,51 );  // rgbA0A2
  pick[5509].Int( 159,0,191,51 );  // rgbA0C2
  pick[5510].Int( 159,0,223,51 );  // rgbA0E2
  pick[5511].Int( 159,31,0,51 );  // rgbA202
  pick[5512].Int( 159,31,31,51 );  // rgbA222
  pick[5513].Int( 159,31,63,51 );  // rgbA242
  pick[5514].Int( 159,31,95,51 );  // rgbA262
  pick[5515].Int( 159,31,127,51 );  // rgbA282
  pick[5516].Int( 159,31,159,51 );  // rgbA2A2
  pick[5517].Int( 159,31,191,51 );  // rgbA2C2
  pick[5518].Int( 159,31,223,51 );  // rgbA2E2
  pick[5519].Int( 159,63,0,51 );  // rgbA402
  pick[5520].Int( 159,63,31,51 );  // rgbA422
  pick[5521].Int( 159,63,63,51 );  // rgbA442
  pick[5522].Int( 159,63,95,51 );  // rgbA462
  pick[5523].Int( 159,63,127,51 );  // rgbA482
  pick[5524].Int( 159,63,159,51 );  // rgbA4A2
  pick[5525].Int( 159,63,191,51 );  // rgbA4C2
  pick[5526].Int( 159,63,223,51 );  // rgbA4E2
  pick[5527].Int( 159,95,0,51 );  // rgbA602
  pick[5528].Int( 159,95,31,51 );  // rgbA622
  pick[5529].Int( 159,95,63,51 );  // rgbA642
  pick[5530].Int( 159,95,95,51 );  // rgbA662
  pick[5531].Int( 159,95,127,51 );  // rgbA682
  pick[5532].Int( 159,95,159,51 );  // rgbA6A2
  pick[5533].Int( 159,95,191,51 );  // rgbA6C2
  pick[5534].Int( 159,95,223,51 );  // rgbA6E2
  pick[5535].Int( 159,127,0,51 );  // rgbA802
  pick[5536].Int( 159,127,31,51 );  // rgbA822
  pick[5537].Int( 159,127,63,51 );  // rgbA842
  pick[5538].Int( 159,127,95,51 );  // rgbA862
  pick[5539].Int( 159,127,127,51 );  // rgbA882
  pick[5540].Int( 159,127,159,51 );  // rgbA8A2
  pick[5541].Int( 159,127,191,51 );  // rgbA8C2
  pick[5542].Int( 159,127,223,51 );  // rgbA8E2
  pick[5543].Int( 159,159,0,51 );  // rgbAA02
  pick[5544].Int( 159,159,31,51 );  // rgbAA22
  pick[5545].Int( 159,159,63,51 );  // rgbAA42
  pick[5546].Int( 159,159,95,51 );  // rgbAA62
  pick[5547].Int( 159,159,127,51 );  // rgbAA82
  pick[5548].Int( 159,159,159,51 );  // rgbAAA2
  pick[5549].Int( 159,159,191,51 );  // rgbAAC2
  pick[5550].Int( 159,159,223,51 );  // rgbAAE2
  pick[5551].Int( 159,191,0,51 );  // rgbAC02
  pick[5552].Int( 159,191,31,51 );  // rgbAC22
  pick[5553].Int( 159,191,63,51 );  // rgbAC42
  pick[5554].Int( 159,191,95,51 );  // rgbAC62
  pick[5555].Int( 159,191,127,51 );  // rgbAC82
  pick[5556].Int( 159,191,159,51 );  // rgbACA2
  pick[5557].Int( 159,191,191,51 );  // rgbACC2
  pick[5558].Int( 159,191,223,51 );  // rgbACE2
  pick[5559].Int( 159,223,0,51 );  // rgbAE02
  pick[5560].Int( 159,223,31,51 );  // rgbAE22
  pick[5561].Int( 159,223,63,51 );  // rgbAE42
  pick[5562].Int( 159,223,95,51 );  // rgbAE62
  pick[5563].Int( 159,223,127,51 );  // rgbAE82
  pick[5564].Int( 159,223,159,51 );  // rgbAEA2
  pick[5565].Int( 159,223,191,51 );  // rgbAEC2
  pick[5566].Int( 159,223,223,51 );  // rgbAEE2
  pick[5567].Int( 191,0,0,51 );  // rgbC002
  pick[5568].Int( 191,0,31,51 );  // rgbC022
  pick[5569].Int( 191,0,63,51 );  // rgbC042
  pick[5570].Int( 191,0,95,51 );  // rgbC062
  pick[5571].Int( 191,0,127,51 );  // rgbC082
  pick[5572].Int( 191,0,159,51 );  // rgbC0A2
  pick[5573].Int( 191,0,191,51 );  // rgbC0C2
  pick[5574].Int( 191,0,223,51 );  // rgbC0E2
  pick[5575].Int( 191,31,0,51 );  // rgbC202
  pick[5576].Int( 191,31,31,51 );  // rgbC222
  pick[5577].Int( 191,31,63,51 );  // rgbC242
  pick[5578].Int( 191,31,95,51 );  // rgbC262
  pick[5579].Int( 191,31,127,51 );  // rgbC282
  pick[5580].Int( 191,31,159,51 );  // rgbC2A2
  pick[5581].Int( 191,31,191,51 );  // rgbC2C2
  pick[5582].Int( 191,31,223,51 );  // rgbC2E2
  pick[5583].Int( 191,63,0,51 );  // rgbC402
  pick[5584].Int( 191,63,31,51 );  // rgbC422
  pick[5585].Int( 191,63,63,51 );  // rgbC442
  pick[5586].Int( 191,63,95,51 );  // rgbC462
  pick[5587].Int( 191,63,127,51 );  // rgbC482
  pick[5588].Int( 191,63,159,51 );  // rgbC4A2
  pick[5589].Int( 191,63,191,51 );  // rgbC4C2
  pick[5590].Int( 191,63,223,51 );  // rgbC4E2
  pick[5591].Int( 191,95,0,51 );  // rgbC602
  pick[5592].Int( 191,95,31,51 );  // rgbC622
  pick[5593].Int( 191,95,63,51 );  // rgbC642
  pick[5594].Int( 191,95,95,51 );  // rgbC662
  pick[5595].Int( 191,95,127,51 );  // rgbC682
  pick[5596].Int( 191,95,159,51 );  // rgbC6A2
  pick[5597].Int( 191,95,191,51 );  // rgbC6C2
  pick[5598].Int( 191,95,223,51 );  // rgbC6E2
  pick[5599].Int( 191,127,0,51 );  // rgbC802
  pick[5600].Int( 191,127,31,51 );  // rgbC822
  pick[5601].Int( 191,127,63,51 );  // rgbC842
  pick[5602].Int( 191,127,95,51 );  // rgbC862
  pick[5603].Int( 191,127,127,51 );  // rgbC882
  pick[5604].Int( 191,127,159,51 );  // rgbC8A2
  pick[5605].Int( 191,127,191,51 );  // rgbC8C2
  pick[5606].Int( 191,127,223,51 );  // rgbC8E2
  pick[5607].Int( 191,159,0,51 );  // rgbCA02
  pick[5608].Int( 191,159,31,51 );  // rgbCA22
  pick[5609].Int( 191,159,63,51 );  // rgbCA42
  pick[5610].Int( 191,159,95,51 );  // rgbCA62
  pick[5611].Int( 191,159,127,51 );  // rgbCA82
  pick[5612].Int( 191,159,159,51 );  // rgbCAA2
  pick[5613].Int( 191,159,191,51 );  // rgbCAC2
  pick[5614].Int( 191,159,223,51 );  // rgbCAE2
  pick[5615].Int( 191,191,0,51 );  // rgbCC02
  pick[5616].Int( 191,191,31,51 );  // rgbCC22
  pick[5617].Int( 191,191,63,51 );  // rgbCC42
  pick[5618].Int( 191,191,95,51 );  // rgbCC62
  pick[5619].Int( 191,191,127,51 );  // rgbCC82
  pick[5620].Int( 191,191,159,51 );  // rgbCCA2
  pick[5621].Int( 191,191,191,51 );  // rgbCCC2
  pick[5622].Int( 191,191,223,51 );  // rgbCCE2
  pick[5623].Int( 191,223,0,51 );  // rgbCE02
  pick[5624].Int( 191,223,31,51 );  // rgbCE22
  pick[5625].Int( 191,223,63,51 );  // rgbCE42
  pick[5626].Int( 191,223,95,51 );  // rgbCE62
  pick[5627].Int( 191,223,127,51 );  // rgbCE82
  pick[5628].Int( 191,223,159,51 );  // rgbCEA2
  pick[5629].Int( 191,223,191,51 );  // rgbCEC2
  pick[5630].Int( 191,223,223,51 );  // rgbCEE2
  pick[5631].Int( 223,0,0,51 );  // rgbE002
  pick[5632].Int( 223,0,31,51 );  // rgbE022
  pick[5633].Int( 223,0,63,51 );  // rgbE042
  pick[5634].Int( 223,0,95,51 );  // rgbE062
  pick[5635].Int( 223,0,127,51 );  // rgbE082
  pick[5636].Int( 223,0,159,51 );  // rgbE0A2
  pick[5637].Int( 223,0,191,51 );  // rgbE0C2
  pick[5638].Int( 223,0,223,51 );  // rgbE0E2
  pick[5639].Int( 223,31,0,51 );  // rgbE202
  pick[5640].Int( 223,31,31,51 );  // rgbE222
  pick[5641].Int( 223,31,63,51 );  // rgbE242
  pick[5642].Int( 223,31,95,51 );  // rgbE262
  pick[5643].Int( 223,31,127,51 );  // rgbE282
  pick[5644].Int( 223,31,159,51 );  // rgbE2A2
  pick[5645].Int( 223,31,191,51 );  // rgbE2C2
  pick[5646].Int( 223,31,223,51 );  // rgbE2E2
  pick[5647].Int( 223,63,0,51 );  // rgbE402
  pick[5648].Int( 223,63,31,51 );  // rgbE422
  pick[5649].Int( 223,63,63,51 );  // rgbE442
  pick[5650].Int( 223,63,95,51 );  // rgbE462
  pick[5651].Int( 223,63,127,51 );  // rgbE482
  pick[5652].Int( 223,63,159,51 );  // rgbE4A2
  pick[5653].Int( 223,63,191,51 );  // rgbE4C2
  pick[5654].Int( 223,63,223,51 );  // rgbE4E2
  pick[5655].Int( 223,95,0,51 );  // rgbE602
  pick[5656].Int( 223,95,31,51 );  // rgbE622
  pick[5657].Int( 223,95,63,51 );  // rgbE642
  pick[5658].Int( 223,95,95,51 );  // rgbE662
  pick[5659].Int( 223,95,127,51 );  // rgbE682
  pick[5660].Int( 223,95,159,51 );  // rgbE6A2
  pick[5661].Int( 223,95,191,51 );  // rgbE6C2
  pick[5662].Int( 223,95,223,51 );  // rgbE6E2
  pick[5663].Int( 223,127,0,51 );  // rgbE802
  pick[5664].Int( 223,127,31,51 );  // rgbE822
  pick[5665].Int( 223,127,63,51 );  // rgbE842
  pick[5666].Int( 223,127,95,51 );  // rgbE862
  pick[5667].Int( 223,127,127,51 );  // rgbE882
  pick[5668].Int( 223,127,159,51 );  // rgbE8A2
  pick[5669].Int( 223,127,191,51 );  // rgbE8C2
  pick[5670].Int( 223,127,223,51 );  // rgbE8E2
  pick[5671].Int( 223,159,0,51 );  // rgbEA02
  pick[5672].Int( 223,159,31,51 );  // rgbEA22
  pick[5673].Int( 223,159,63,51 );  // rgbEA42
  pick[5674].Int( 223,159,95,51 );  // rgbEA62
  pick[5675].Int( 223,159,127,51 );  // rgbEA82
  pick[5676].Int( 223,159,159,51 );  // rgbEAA2
  pick[5677].Int( 223,159,191,51 );  // rgbEAC2
  pick[5678].Int( 223,159,223,51 );  // rgbEAE2
  pick[5679].Int( 223,191,0,51 );  // rgbEC02
  pick[5680].Int( 223,191,31,51 );  // rgbEC22
  pick[5681].Int( 223,191,63,51 );  // rgbEC42
  pick[5682].Int( 223,191,95,51 );  // rgbEC62
  pick[5683].Int( 223,191,127,51 );  // rgbEC82
  pick[5684].Int( 223,191,159,51 );  // rgbECA2
  pick[5685].Int( 223,191,191,51 );  // rgbECC2
  pick[5686].Int( 223,191,223,51 );  // rgbECE2
  pick[5687].Int( 223,223,0,51 );  // rgbEE02
  pick[5688].Int( 223,223,31,51 );  // rgbEE22
  pick[5689].Int( 223,223,63,51 );  // rgbEE42
  pick[5690].Int( 223,223,95,51 );  // rgbEE62
  pick[5691].Int( 223,223,127,51 );  // rgbEE82
  pick[5692].Int( 223,223,159,51 );  // rgbEEA2
  pick[5693].Int( 223,223,191,51 );  // rgbEEC2
  pick[5694].Int( 223,223,223,51 );  // rgbEEE2
  pick[5695].Int( 0,0,0,76 );  // rgb0003
  pick[5696].Int( 0,0,31,76 );  // rgb0023
  pick[5697].Int( 0,0,63,76 );  // rgb0043
  pick[5698].Int( 0,0,95,76 );  // rgb0063
  pick[5699].Int( 0,0,127,76 );  // rgb0083
  pick[5700].Int( 0,0,159,76 );  // rgb00A3
  pick[5701].Int( 0,0,191,76 );  // rgb00C3
  pick[5702].Int( 0,0,223,76 );  // rgb00E3
  pick[5703].Int( 0,31,0,76 );  // rgb0203
  pick[5704].Int( 0,31,31,76 );  // rgb0223
  pick[5705].Int( 0,31,63,76 );  // rgb0243
  pick[5706].Int( 0,31,95,76 );  // rgb0263
  pick[5707].Int( 0,31,127,76 );  // rgb0283
  pick[5708].Int( 0,31,159,76 );  // rgb02A3
  pick[5709].Int( 0,31,191,76 );  // rgb02C3
  pick[5710].Int( 0,31,223,76 );  // rgb02E3
  pick[5711].Int( 0,63,0,76 );  // rgb0403
  pick[5712].Int( 0,63,31,76 );  // rgb0423
  pick[5713].Int( 0,63,63,76 );  // rgb0443
  pick[5714].Int( 0,63,95,76 );  // rgb0463
  pick[5715].Int( 0,63,127,76 );  // rgb0483
  pick[5716].Int( 0,63,159,76 );  // rgb04A3
  pick[5717].Int( 0,63,191,76 );  // rgb04C3
  pick[5718].Int( 0,63,223,76 );  // rgb04E3
  pick[5719].Int( 0,95,0,76 );  // rgb0603
  pick[5720].Int( 0,95,31,76 );  // rgb0623
  pick[5721].Int( 0,95,63,76 );  // rgb0643
  pick[5722].Int( 0,95,95,76 );  // rgb0663
  pick[5723].Int( 0,95,127,76 );  // rgb0683
  pick[5724].Int( 0,95,159,76 );  // rgb06A3
  pick[5725].Int( 0,95,191,76 );  // rgb06C3
  pick[5726].Int( 0,95,223,76 );  // rgb06E3
  pick[5727].Int( 0,127,0,76 );  // rgb0803
  pick[5728].Int( 0,127,31,76 );  // rgb0823
  pick[5729].Int( 0,127,63,76 );  // rgb0843
  pick[5730].Int( 0,127,95,76 );  // rgb0863
  pick[5731].Int( 0,127,127,76 );  // rgb0883
  pick[5732].Int( 0,127,159,76 );  // rgb08A3
  pick[5733].Int( 0,127,191,76 );  // rgb08C3
  pick[5734].Int( 0,127,223,76 );  // rgb08E3
  pick[5735].Int( 0,159,0,76 );  // rgb0A03
  pick[5736].Int( 0,159,31,76 );  // rgb0A23
  pick[5737].Int( 0,159,63,76 );  // rgb0A43
  pick[5738].Int( 0,159,95,76 );  // rgb0A63
  pick[5739].Int( 0,159,127,76 );  // rgb0A83
  pick[5740].Int( 0,159,159,76 );  // rgb0AA3
  pick[5741].Int( 0,159,191,76 );  // rgb0AC3
  pick[5742].Int( 0,159,223,76 );  // rgb0AE3
  pick[5743].Int( 0,191,0,76 );  // rgb0C03
  pick[5744].Int( 0,191,31,76 );  // rgb0C23
  pick[5745].Int( 0,191,63,76 );  // rgb0C43
  pick[5746].Int( 0,191,95,76 );  // rgb0C63
  pick[5747].Int( 0,191,127,76 );  // rgb0C83
  pick[5748].Int( 0,191,159,76 );  // rgb0CA3
  pick[5749].Int( 0,191,191,76 );  // rgb0CC3
  pick[5750].Int( 0,191,223,76 );  // rgb0CE3
  pick[5751].Int( 0,223,0,76 );  // rgb0E03
  pick[5752].Int( 0,223,31,76 );  // rgb0E23
  pick[5753].Int( 0,223,63,76 );  // rgb0E43
  pick[5754].Int( 0,223,95,76 );  // rgb0E63
  pick[5755].Int( 0,223,127,76 );  // rgb0E83
  pick[5756].Int( 0,223,159,76 );  // rgb0EA3
  pick[5757].Int( 0,223,191,76 );  // rgb0EC3
  pick[5758].Int( 0,223,223,76 );  // rgb0EE3
  pick[5759].Int( 31,0,0,76 );  // rgb2003
  pick[5760].Int( 31,0,31,76 );  // rgb2023
  pick[5761].Int( 31,0,63,76 );  // rgb2043
  pick[5762].Int( 31,0,95,76 );  // rgb2063
  pick[5763].Int( 31,0,127,76 );  // rgb2083
  pick[5764].Int( 31,0,159,76 );  // rgb20A3
  pick[5765].Int( 31,0,191,76 );  // rgb20C3
  pick[5766].Int( 31,0,223,76 );  // rgb20E3
  pick[5767].Int( 31,31,0,76 );  // rgb2203
  pick[5768].Int( 31,31,31,76 );  // rgb2223
  pick[5769].Int( 31,31,63,76 );  // rgb2243
  pick[5770].Int( 31,31,95,76 );  // rgb2263
  pick[5771].Int( 31,31,127,76 );  // rgb2283
  pick[5772].Int( 31,31,159,76 );  // rgb22A3
  pick[5773].Int( 31,31,191,76 );  // rgb22C3
  pick[5774].Int( 31,31,223,76 );  // rgb22E3
  pick[5775].Int( 31,63,0,76 );  // rgb2403
  pick[5776].Int( 31,63,31,76 );  // rgb2423
  pick[5777].Int( 31,63,63,76 );  // rgb2443
  pick[5778].Int( 31,63,95,76 );  // rgb2463
  pick[5779].Int( 31,63,127,76 );  // rgb2483
  pick[5780].Int( 31,63,159,76 );  // rgb24A3
  pick[5781].Int( 31,63,191,76 );  // rgb24C3
  pick[5782].Int( 31,63,223,76 );  // rgb24E3
  pick[5783].Int( 31,95,0,76 );  // rgb2603
  pick[5784].Int( 31,95,31,76 );  // rgb2623
  pick[5785].Int( 31,95,63,76 );  // rgb2643
  pick[5786].Int( 31,95,95,76 );  // rgb2663
  pick[5787].Int( 31,95,127,76 );  // rgb2683
  pick[5788].Int( 31,95,159,76 );  // rgb26A3
  pick[5789].Int( 31,95,191,76 );  // rgb26C3
  pick[5790].Int( 31,95,223,76 );  // rgb26E3
  pick[5791].Int( 31,127,0,76 );  // rgb2803
  pick[5792].Int( 31,127,31,76 );  // rgb2823
  pick[5793].Int( 31,127,63,76 );  // rgb2843
  pick[5794].Int( 31,127,95,76 );  // rgb2863
  pick[5795].Int( 31,127,127,76 );  // rgb2883
  pick[5796].Int( 31,127,159,76 );  // rgb28A3
  pick[5797].Int( 31,127,191,76 );  // rgb28C3
  pick[5798].Int( 31,127,223,76 );  // rgb28E3
  pick[5799].Int( 31,159,0,76 );  // rgb2A03
  pick[5800].Int( 31,159,31,76 );  // rgb2A23
  pick[5801].Int( 31,159,63,76 );  // rgb2A43
  pick[5802].Int( 31,159,95,76 );  // rgb2A63
  pick[5803].Int( 31,159,127,76 );  // rgb2A83
  pick[5804].Int( 31,159,159,76 );  // rgb2AA3
  pick[5805].Int( 31,159,191,76 );  // rgb2AC3
  pick[5806].Int( 31,159,223,76 );  // rgb2AE3
  pick[5807].Int( 31,191,0,76 );  // rgb2C03
  pick[5808].Int( 31,191,31,76 );  // rgb2C23
  pick[5809].Int( 31,191,63,76 );  // rgb2C43
  pick[5810].Int( 31,191,95,76 );  // rgb2C63
  pick[5811].Int( 31,191,127,76 );  // rgb2C83
  pick[5812].Int( 31,191,159,76 );  // rgb2CA3
  pick[5813].Int( 31,191,191,76 );  // rgb2CC3
  pick[5814].Int( 31,191,223,76 );  // rgb2CE3
  pick[5815].Int( 31,223,0,76 );  // rgb2E03
  pick[5816].Int( 31,223,31,76 );  // rgb2E23
  pick[5817].Int( 31,223,63,76 );  // rgb2E43
  pick[5818].Int( 31,223,95,76 );  // rgb2E63
  pick[5819].Int( 31,223,127,76 );  // rgb2E83
  pick[5820].Int( 31,223,159,76 );  // rgb2EA3
  pick[5821].Int( 31,223,191,76 );  // rgb2EC3
  pick[5822].Int( 31,223,223,76 );  // rgb2EE3
  pick[5823].Int( 63,0,0,76 );  // rgb4003
  pick[5824].Int( 63,0,31,76 );  // rgb4023
  pick[5825].Int( 63,0,63,76 );  // rgb4043
  pick[5826].Int( 63,0,95,76 );  // rgb4063
  pick[5827].Int( 63,0,127,76 );  // rgb4083
  pick[5828].Int( 63,0,159,76 );  // rgb40A3
  pick[5829].Int( 63,0,191,76 );  // rgb40C3
  pick[5830].Int( 63,0,223,76 );  // rgb40E3
  pick[5831].Int( 63,31,0,76 );  // rgb4203
  pick[5832].Int( 63,31,31,76 );  // rgb4223
  pick[5833].Int( 63,31,63,76 );  // rgb4243
  pick[5834].Int( 63,31,95,76 );  // rgb4263
  pick[5835].Int( 63,31,127,76 );  // rgb4283
  pick[5836].Int( 63,31,159,76 );  // rgb42A3
  pick[5837].Int( 63,31,191,76 );  // rgb42C3
  pick[5838].Int( 63,31,223,76 );  // rgb42E3
  pick[5839].Int( 63,63,0,76 );  // rgb4403
  pick[5840].Int( 63,63,31,76 );  // rgb4423
  pick[5841].Int( 63,63,63,76 );  // rgb4443
  pick[5842].Int( 63,63,95,76 );  // rgb4463
  pick[5843].Int( 63,63,127,76 );  // rgb4483
  pick[5844].Int( 63,63,159,76 );  // rgb44A3
  pick[5845].Int( 63,63,191,76 );  // rgb44C3
  pick[5846].Int( 63,63,223,76 );  // rgb44E3
  pick[5847].Int( 63,95,0,76 );  // rgb4603
  pick[5848].Int( 63,95,31,76 );  // rgb4623
  pick[5849].Int( 63,95,63,76 );  // rgb4643
  pick[5850].Int( 63,95,95,76 );  // rgb4663
  pick[5851].Int( 63,95,127,76 );  // rgb4683
  pick[5852].Int( 63,95,159,76 );  // rgb46A3
  pick[5853].Int( 63,95,191,76 );  // rgb46C3
  pick[5854].Int( 63,95,223,76 );  // rgb46E3
  pick[5855].Int( 63,127,0,76 );  // rgb4803
  pick[5856].Int( 63,127,31,76 );  // rgb4823
  pick[5857].Int( 63,127,63,76 );  // rgb4843
  pick[5858].Int( 63,127,95,76 );  // rgb4863
  pick[5859].Int( 63,127,127,76 );  // rgb4883
  pick[5860].Int( 63,127,159,76 );  // rgb48A3
  pick[5861].Int( 63,127,191,76 );  // rgb48C3
  pick[5862].Int( 63,127,223,76 );  // rgb48E3
  pick[5863].Int( 63,159,0,76 );  // rgb4A03
  pick[5864].Int( 63,159,31,76 );  // rgb4A23
  pick[5865].Int( 63,159,63,76 );  // rgb4A43
  pick[5866].Int( 63,159,95,76 );  // rgb4A63
  pick[5867].Int( 63,159,127,76 );  // rgb4A83
  pick[5868].Int( 63,159,159,76 );  // rgb4AA3
  pick[5869].Int( 63,159,191,76 );  // rgb4AC3
  pick[5870].Int( 63,159,223,76 );  // rgb4AE3
  pick[5871].Int( 63,191,0,76 );  // rgb4C03
  pick[5872].Int( 63,191,31,76 );  // rgb4C23
  pick[5873].Int( 63,191,63,76 );  // rgb4C43
  pick[5874].Int( 63,191,95,76 );  // rgb4C63
  pick[5875].Int( 63,191,127,76 );  // rgb4C83
  pick[5876].Int( 63,191,159,76 );  // rgb4CA3
  pick[5877].Int( 63,191,191,76 );  // rgb4CC3
  pick[5878].Int( 63,191,223,76 );  // rgb4CE3
  pick[5879].Int( 63,223,0,76 );  // rgb4E03
  pick[5880].Int( 63,223,31,76 );  // rgb4E23
  pick[5881].Int( 63,223,63,76 );  // rgb4E43
  pick[5882].Int( 63,223,95,76 );  // rgb4E63
  pick[5883].Int( 63,223,127,76 );  // rgb4E83
  pick[5884].Int( 63,223,159,76 );  // rgb4EA3
  pick[5885].Int( 63,223,191,76 );  // rgb4EC3
  pick[5886].Int( 63,223,223,76 );  // rgb4EE3
  pick[5887].Int( 95,0,0,76 );  // rgb6003
  pick[5888].Int( 95,0,31,76 );  // rgb6023
  pick[5889].Int( 95,0,63,76 );  // rgb6043
  pick[5890].Int( 95,0,95,76 );  // rgb6063
  pick[5891].Int( 95,0,127,76 );  // rgb6083
  pick[5892].Int( 95,0,159,76 );  // rgb60A3
  pick[5893].Int( 95,0,191,76 );  // rgb60C3
  pick[5894].Int( 95,0,223,76 );  // rgb60E3
  pick[5895].Int( 95,31,0,76 );  // rgb6203
  pick[5896].Int( 95,31,31,76 );  // rgb6223
  pick[5897].Int( 95,31,63,76 );  // rgb6243
  pick[5898].Int( 95,31,95,76 );  // rgb6263
  pick[5899].Int( 95,31,127,76 );  // rgb6283
  pick[5900].Int( 95,31,159,76 );  // rgb62A3
  pick[5901].Int( 95,31,191,76 );  // rgb62C3
  pick[5902].Int( 95,31,223,76 );  // rgb62E3
  pick[5903].Int( 95,63,0,76 );  // rgb6403
  pick[5904].Int( 95,63,31,76 );  // rgb6423
  pick[5905].Int( 95,63,63,76 );  // rgb6443
  pick[5906].Int( 95,63,95,76 );  // rgb6463
  pick[5907].Int( 95,63,127,76 );  // rgb6483
  pick[5908].Int( 95,63,159,76 );  // rgb64A3
  pick[5909].Int( 95,63,191,76 );  // rgb64C3
  pick[5910].Int( 95,63,223,76 );  // rgb64E3
  pick[5911].Int( 95,95,0,76 );  // rgb6603
  pick[5912].Int( 95,95,31,76 );  // rgb6623
  pick[5913].Int( 95,95,63,76 );  // rgb6643
  pick[5914].Int( 95,95,95,76 );  // rgb6663
  pick[5915].Int( 95,95,127,76 );  // rgb6683
  pick[5916].Int( 95,95,159,76 );  // rgb66A3
  pick[5917].Int( 95,95,191,76 );  // rgb66C3
  pick[5918].Int( 95,95,223,76 );  // rgb66E3
  pick[5919].Int( 95,127,0,76 );  // rgb6803
  pick[5920].Int( 95,127,31,76 );  // rgb6823
  pick[5921].Int( 95,127,63,76 );  // rgb6843
  pick[5922].Int( 95,127,95,76 );  // rgb6863
  pick[5923].Int( 95,127,127,76 );  // rgb6883
  pick[5924].Int( 95,127,159,76 );  // rgb68A3
  pick[5925].Int( 95,127,191,76 );  // rgb68C3
  pick[5926].Int( 95,127,223,76 );  // rgb68E3
  pick[5927].Int( 95,159,0,76 );  // rgb6A03
  pick[5928].Int( 95,159,31,76 );  // rgb6A23
  pick[5929].Int( 95,159,63,76 );  // rgb6A43
  pick[5930].Int( 95,159,95,76 );  // rgb6A63
  pick[5931].Int( 95,159,127,76 );  // rgb6A83
  pick[5932].Int( 95,159,159,76 );  // rgb6AA3
  pick[5933].Int( 95,159,191,76 );  // rgb6AC3
  pick[5934].Int( 95,159,223,76 );  // rgb6AE3
  pick[5935].Int( 95,191,0,76 );  // rgb6C03
  pick[5936].Int( 95,191,31,76 );  // rgb6C23
  pick[5937].Int( 95,191,63,76 );  // rgb6C43
  pick[5938].Int( 95,191,95,76 );  // rgb6C63
  pick[5939].Int( 95,191,127,76 );  // rgb6C83
  pick[5940].Int( 95,191,159,76 );  // rgb6CA3
  pick[5941].Int( 95,191,191,76 );  // rgb6CC3
  pick[5942].Int( 95,191,223,76 );  // rgb6CE3
  pick[5943].Int( 95,223,0,76 );  // rgb6E03
  pick[5944].Int( 95,223,31,76 );  // rgb6E23
  pick[5945].Int( 95,223,63,76 );  // rgb6E43
  pick[5946].Int( 95,223,95,76 );  // rgb6E63
  pick[5947].Int( 95,223,127,76 );  // rgb6E83
  pick[5948].Int( 95,223,159,76 );  // rgb6EA3
  pick[5949].Int( 95,223,191,76 );  // rgb6EC3
  pick[5950].Int( 95,223,223,76 );  // rgb6EE3
  pick[5951].Int( 127,0,0,76 );  // rgb8003
  pick[5952].Int( 127,0,31,76 );  // rgb8023
  pick[5953].Int( 127,0,63,76 );  // rgb8043
  pick[5954].Int( 127,0,95,76 );  // rgb8063
  pick[5955].Int( 127,0,127,76 );  // rgb8083
  pick[5956].Int( 127,0,159,76 );  // rgb80A3
  pick[5957].Int( 127,0,191,76 );  // rgb80C3
  pick[5958].Int( 127,0,223,76 );  // rgb80E3
  pick[5959].Int( 127,31,0,76 );  // rgb8203
  pick[5960].Int( 127,31,31,76 );  // rgb8223
  pick[5961].Int( 127,31,63,76 );  // rgb8243
  pick[5962].Int( 127,31,95,76 );  // rgb8263
  pick[5963].Int( 127,31,127,76 );  // rgb8283
  pick[5964].Int( 127,31,159,76 );  // rgb82A3
  pick[5965].Int( 127,31,191,76 );  // rgb82C3
  pick[5966].Int( 127,31,223,76 );  // rgb82E3
  pick[5967].Int( 127,63,0,76 );  // rgb8403
  pick[5968].Int( 127,63,31,76 );  // rgb8423
  pick[5969].Int( 127,63,63,76 );  // rgb8443
  pick[5970].Int( 127,63,95,76 );  // rgb8463
  pick[5971].Int( 127,63,127,76 );  // rgb8483
  pick[5972].Int( 127,63,159,76 );  // rgb84A3
  pick[5973].Int( 127,63,191,76 );  // rgb84C3
  pick[5974].Int( 127,63,223,76 );  // rgb84E3
  pick[5975].Int( 127,95,0,76 );  // rgb8603
  pick[5976].Int( 127,95,31,76 );  // rgb8623
  pick[5977].Int( 127,95,63,76 );  // rgb8643
  pick[5978].Int( 127,95,95,76 );  // rgb8663
  pick[5979].Int( 127,95,127,76 );  // rgb8683
  pick[5980].Int( 127,95,159,76 );  // rgb86A3
  pick[5981].Int( 127,95,191,76 );  // rgb86C3
  pick[5982].Int( 127,95,223,76 );  // rgb86E3
  pick[5983].Int( 127,127,0,76 );  // rgb8803
  pick[5984].Int( 127,127,31,76 );  // rgb8823
  pick[5985].Int( 127,127,63,76 );  // rgb8843
  pick[5986].Int( 127,127,95,76 );  // rgb8863
  pick[5987].Int( 127,127,127,76 );  // rgb8883
  pick[5988].Int( 127,127,159,76 );  // rgb88A3
  pick[5989].Int( 127,127,191,76 );  // rgb88C3
  pick[5990].Int( 127,127,223,76 );  // rgb88E3
  pick[5991].Int( 127,159,0,76 );  // rgb8A03
  pick[5992].Int( 127,159,31,76 );  // rgb8A23
  pick[5993].Int( 127,159,63,76 );  // rgb8A43
  pick[5994].Int( 127,159,95,76 );  // rgb8A63
  pick[5995].Int( 127,159,127,76 );  // rgb8A83
  pick[5996].Int( 127,159,159,76 );  // rgb8AA3
  pick[5997].Int( 127,159,191,76 );  // rgb8AC3
  pick[5998].Int( 127,159,223,76 );  // rgb8AE3
  pick[5999].Int( 127,191,0,76 );  // rgb8C03
  pick[6000].Int( 127,191,31,76 );  // rgb8C23
  pick[6001].Int( 127,191,63,76 );  // rgb8C43
  pick[6002].Int( 127,191,95,76 );  // rgb8C63
  pick[6003].Int( 127,191,127,76 );  // rgb8C83
  pick[6004].Int( 127,191,159,76 );  // rgb8CA3
  pick[6005].Int( 127,191,191,76 );  // rgb8CC3
  pick[6006].Int( 127,191,223,76 );  // rgb8CE3
  pick[6007].Int( 127,223,0,76 );  // rgb8E03
  pick[6008].Int( 127,223,31,76 );  // rgb8E23
  pick[6009].Int( 127,223,63,76 );  // rgb8E43
  pick[6010].Int( 127,223,95,76 );  // rgb8E63
  pick[6011].Int( 127,223,127,76 );  // rgb8E83
  pick[6012].Int( 127,223,159,76 );  // rgb8EA3
  pick[6013].Int( 127,223,191,76 );  // rgb8EC3
  pick[6014].Int( 127,223,223,76 );  // rgb8EE3
  pick[6015].Int( 159,0,0,76 );  // rgbA003
  pick[6016].Int( 159,0,31,76 );  // rgbA023
  pick[6017].Int( 159,0,63,76 );  // rgbA043
  pick[6018].Int( 159,0,95,76 );  // rgbA063
  pick[6019].Int( 159,0,127,76 );  // rgbA083
  pick[6020].Int( 159,0,159,76 );  // rgbA0A3
  pick[6021].Int( 159,0,191,76 );  // rgbA0C3
  pick[6022].Int( 159,0,223,76 );  // rgbA0E3
  pick[6023].Int( 159,31,0,76 );  // rgbA203
  pick[6024].Int( 159,31,31,76 );  // rgbA223
  pick[6025].Int( 159,31,63,76 );  // rgbA243
  pick[6026].Int( 159,31,95,76 );  // rgbA263
  pick[6027].Int( 159,31,127,76 );  // rgbA283
  pick[6028].Int( 159,31,159,76 );  // rgbA2A3
  pick[6029].Int( 159,31,191,76 );  // rgbA2C3
  pick[6030].Int( 159,31,223,76 );  // rgbA2E3
  pick[6031].Int( 159,63,0,76 );  // rgbA403
  pick[6032].Int( 159,63,31,76 );  // rgbA423
  pick[6033].Int( 159,63,63,76 );  // rgbA443
  pick[6034].Int( 159,63,95,76 );  // rgbA463
  pick[6035].Int( 159,63,127,76 );  // rgbA483
  pick[6036].Int( 159,63,159,76 );  // rgbA4A3
  pick[6037].Int( 159,63,191,76 );  // rgbA4C3
  pick[6038].Int( 159,63,223,76 );  // rgbA4E3
  pick[6039].Int( 159,95,0,76 );  // rgbA603
  pick[6040].Int( 159,95,31,76 );  // rgbA623
  pick[6041].Int( 159,95,63,76 );  // rgbA643
  pick[6042].Int( 159,95,95,76 );  // rgbA663
  pick[6043].Int( 159,95,127,76 );  // rgbA683
  pick[6044].Int( 159,95,159,76 );  // rgbA6A3
  pick[6045].Int( 159,95,191,76 );  // rgbA6C3
  pick[6046].Int( 159,95,223,76 );  // rgbA6E3
  pick[6047].Int( 159,127,0,76 );  // rgbA803
  pick[6048].Int( 159,127,31,76 );  // rgbA823
  pick[6049].Int( 159,127,63,76 );  // rgbA843
  pick[6050].Int( 159,127,95,76 );  // rgbA863
  pick[6051].Int( 159,127,127,76 );  // rgbA883
  pick[6052].Int( 159,127,159,76 );  // rgbA8A3
  pick[6053].Int( 159,127,191,76 );  // rgbA8C3
  pick[6054].Int( 159,127,223,76 );  // rgbA8E3
  pick[6055].Int( 159,159,0,76 );  // rgbAA03
  pick[6056].Int( 159,159,31,76 );  // rgbAA23
  pick[6057].Int( 159,159,63,76 );  // rgbAA43
  pick[6058].Int( 159,159,95,76 );  // rgbAA63
  pick[6059].Int( 159,159,127,76 );  // rgbAA83
  pick[6060].Int( 159,159,159,76 );  // rgbAAA3
  pick[6061].Int( 159,159,191,76 );  // rgbAAC3
  pick[6062].Int( 159,159,223,76 );  // rgbAAE3
  pick[6063].Int( 159,191,0,76 );  // rgbAC03
  pick[6064].Int( 159,191,31,76 );  // rgbAC23
  pick[6065].Int( 159,191,63,76 );  // rgbAC43
  pick[6066].Int( 159,191,95,76 );  // rgbAC63
  pick[6067].Int( 159,191,127,76 );  // rgbAC83
  pick[6068].Int( 159,191,159,76 );  // rgbACA3
  pick[6069].Int( 159,191,191,76 );  // rgbACC3
  pick[6070].Int( 159,191,223,76 );  // rgbACE3
  pick[6071].Int( 159,223,0,76 );  // rgbAE03
  pick[6072].Int( 159,223,31,76 );  // rgbAE23
  pick[6073].Int( 159,223,63,76 );  // rgbAE43
  pick[6074].Int( 159,223,95,76 );  // rgbAE63
  pick[6075].Int( 159,223,127,76 );  // rgbAE83
  pick[6076].Int( 159,223,159,76 );  // rgbAEA3
  pick[6077].Int( 159,223,191,76 );  // rgbAEC3
  pick[6078].Int( 159,223,223,76 );  // rgbAEE3
  pick[6079].Int( 191,0,0,76 );  // rgbC003
  pick[6080].Int( 191,0,31,76 );  // rgbC023
  pick[6081].Int( 191,0,63,76 );  // rgbC043
  pick[6082].Int( 191,0,95,76 );  // rgbC063
  pick[6083].Int( 191,0,127,76 );  // rgbC083
  pick[6084].Int( 191,0,159,76 );  // rgbC0A3
  pick[6085].Int( 191,0,191,76 );  // rgbC0C3
  pick[6086].Int( 191,0,223,76 );  // rgbC0E3
  pick[6087].Int( 191,31,0,76 );  // rgbC203
  pick[6088].Int( 191,31,31,76 );  // rgbC223
  pick[6089].Int( 191,31,63,76 );  // rgbC243
  pick[6090].Int( 191,31,95,76 );  // rgbC263
  pick[6091].Int( 191,31,127,76 );  // rgbC283
  pick[6092].Int( 191,31,159,76 );  // rgbC2A3
  pick[6093].Int( 191,31,191,76 );  // rgbC2C3
  pick[6094].Int( 191,31,223,76 );  // rgbC2E3
  pick[6095].Int( 191,63,0,76 );  // rgbC403
  pick[6096].Int( 191,63,31,76 );  // rgbC423
  pick[6097].Int( 191,63,63,76 );  // rgbC443
  pick[6098].Int( 191,63,95,76 );  // rgbC463
  pick[6099].Int( 191,63,127,76 );  // rgbC483
  pick[6100].Int( 191,63,159,76 );  // rgbC4A3
  pick[6101].Int( 191,63,191,76 );  // rgbC4C3
  pick[6102].Int( 191,63,223,76 );  // rgbC4E3
  pick[6103].Int( 191,95,0,76 );  // rgbC603
  pick[6104].Int( 191,95,31,76 );  // rgbC623
  pick[6105].Int( 191,95,63,76 );  // rgbC643
  pick[6106].Int( 191,95,95,76 );  // rgbC663
  pick[6107].Int( 191,95,127,76 );  // rgbC683
  pick[6108].Int( 191,95,159,76 );  // rgbC6A3
  pick[6109].Int( 191,95,191,76 );  // rgbC6C3
  pick[6110].Int( 191,95,223,76 );  // rgbC6E3
  pick[6111].Int( 191,127,0,76 );  // rgbC803
  pick[6112].Int( 191,127,31,76 );  // rgbC823
  pick[6113].Int( 191,127,63,76 );  // rgbC843
  pick[6114].Int( 191,127,95,76 );  // rgbC863
  pick[6115].Int( 191,127,127,76 );  // rgbC883
  pick[6116].Int( 191,127,159,76 );  // rgbC8A3
  pick[6117].Int( 191,127,191,76 );  // rgbC8C3
  pick[6118].Int( 191,127,223,76 );  // rgbC8E3
  pick[6119].Int( 191,159,0,76 );  // rgbCA03
  pick[6120].Int( 191,159,31,76 );  // rgbCA23
  pick[6121].Int( 191,159,63,76 );  // rgbCA43
  pick[6122].Int( 191,159,95,76 );  // rgbCA63
  pick[6123].Int( 191,159,127,76 );  // rgbCA83
  pick[6124].Int( 191,159,159,76 );  // rgbCAA3
  pick[6125].Int( 191,159,191,76 );  // rgbCAC3
  pick[6126].Int( 191,159,223,76 );  // rgbCAE3
  pick[6127].Int( 191,191,0,76 );  // rgbCC03
  pick[6128].Int( 191,191,31,76 );  // rgbCC23
  pick[6129].Int( 191,191,63,76 );  // rgbCC43
  pick[6130].Int( 191,191,95,76 );  // rgbCC63
  pick[6131].Int( 191,191,127,76 );  // rgbCC83
  pick[6132].Int( 191,191,159,76 );  // rgbCCA3
  pick[6133].Int( 191,191,191,76 );  // rgbCCC3
  pick[6134].Int( 191,191,223,76 );  // rgbCCE3
  pick[6135].Int( 191,223,0,76 );  // rgbCE03
  pick[6136].Int( 191,223,31,76 );  // rgbCE23
  pick[6137].Int( 191,223,63,76 );  // rgbCE43
  pick[6138].Int( 191,223,95,76 );  // rgbCE63
  pick[6139].Int( 191,223,127,76 );  // rgbCE83
  pick[6140].Int( 191,223,159,76 );  // rgbCEA3
  pick[6141].Int( 191,223,191,76 );  // rgbCEC3
  pick[6142].Int( 191,223,223,76 );  // rgbCEE3
  pick[6143].Int( 223,0,0,76 );  // rgbE003
  pick[6144].Int( 223,0,31,76 );  // rgbE023
  pick[6145].Int( 223,0,63,76 );  // rgbE043
  pick[6146].Int( 223,0,95,76 );  // rgbE063
  pick[6147].Int( 223,0,127,76 );  // rgbE083
  pick[6148].Int( 223,0,159,76 );  // rgbE0A3
  pick[6149].Int( 223,0,191,76 );  // rgbE0C3
  pick[6150].Int( 223,0,223,76 );  // rgbE0E3
  pick[6151].Int( 223,31,0,76 );  // rgbE203
  pick[6152].Int( 223,31,31,76 );  // rgbE223
  pick[6153].Int( 223,31,63,76 );  // rgbE243
  pick[6154].Int( 223,31,95,76 );  // rgbE263
  pick[6155].Int( 223,31,127,76 );  // rgbE283
  pick[6156].Int( 223,31,159,76 );  // rgbE2A3
  pick[6157].Int( 223,31,191,76 );  // rgbE2C3
  pick[6158].Int( 223,31,223,76 );  // rgbE2E3
  pick[6159].Int( 223,63,0,76 );  // rgbE403
  pick[6160].Int( 223,63,31,76 );  // rgbE423
  pick[6161].Int( 223,63,63,76 );  // rgbE443
  pick[6162].Int( 223,63,95,76 );  // rgbE463
  pick[6163].Int( 223,63,127,76 );  // rgbE483
  pick[6164].Int( 223,63,159,76 );  // rgbE4A3
  pick[6165].Int( 223,63,191,76 );  // rgbE4C3
  pick[6166].Int( 223,63,223,76 );  // rgbE4E3
  pick[6167].Int( 223,95,0,76 );  // rgbE603
  pick[6168].Int( 223,95,31,76 );  // rgbE623
  pick[6169].Int( 223,95,63,76 );  // rgbE643
  pick[6170].Int( 223,95,95,76 );  // rgbE663
  pick[6171].Int( 223,95,127,76 );  // rgbE683
  pick[6172].Int( 223,95,159,76 );  // rgbE6A3
  pick[6173].Int( 223,95,191,76 );  // rgbE6C3
  pick[6174].Int( 223,95,223,76 );  // rgbE6E3
  pick[6175].Int( 223,127,0,76 );  // rgbE803
  pick[6176].Int( 223,127,31,76 );  // rgbE823
  pick[6177].Int( 223,127,63,76 );  // rgbE843
  pick[6178].Int( 223,127,95,76 );  // rgbE863
  pick[6179].Int( 223,127,127,76 );  // rgbE883
  pick[6180].Int( 223,127,159,76 );  // rgbE8A3
  pick[6181].Int( 223,127,191,76 );  // rgbE8C3
  pick[6182].Int( 223,127,223,76 );  // rgbE8E3
  pick[6183].Int( 223,159,0,76 );  // rgbEA03
  pick[6184].Int( 223,159,31,76 );  // rgbEA23
  pick[6185].Int( 223,159,63,76 );  // rgbEA43
  pick[6186].Int( 223,159,95,76 );  // rgbEA63
  pick[6187].Int( 223,159,127,76 );  // rgbEA83
  pick[6188].Int( 223,159,159,76 );  // rgbEAA3
  pick[6189].Int( 223,159,191,76 );  // rgbEAC3
  pick[6190].Int( 223,159,223,76 );  // rgbEAE3
  pick[6191].Int( 223,191,0,76 );  // rgbEC03
  pick[6192].Int( 223,191,31,76 );  // rgbEC23
  pick[6193].Int( 223,191,63,76 );  // rgbEC43
  pick[6194].Int( 223,191,95,76 );  // rgbEC63
  pick[6195].Int( 223,191,127,76 );  // rgbEC83
  pick[6196].Int( 223,191,159,76 );  // rgbECA3
  pick[6197].Int( 223,191,191,76 );  // rgbECC3
  pick[6198].Int( 223,191,223,76 );  // rgbECE3
  pick[6199].Int( 223,223,0,76 );  // rgbEE03
  pick[6200].Int( 223,223,31,76 );  // rgbEE23
  pick[6201].Int( 223,223,63,76 );  // rgbEE43
  pick[6202].Int( 223,223,95,76 );  // rgbEE63
  pick[6203].Int( 223,223,127,76 );  // rgbEE83
  pick[6204].Int( 223,223,159,76 );  // rgbEEA3
  pick[6205].Int( 223,223,191,76 );  // rgbEEC3
  pick[6206].Int( 223,223,223,76 );  // rgbEEE3
  pick[6207].Int( 0,0,0,102 );  // rgb0004
  pick[6208].Int( 0,0,31,102 );  // rgb0024
  pick[6209].Int( 0,0,63,102 );  // rgb0044
  pick[6210].Int( 0,0,95,102 );  // rgb0064
  pick[6211].Int( 0,0,127,102 );  // rgb0084
  pick[6212].Int( 0,0,159,102 );  // rgb00A4
  pick[6213].Int( 0,0,191,102 );  // rgb00C4
  pick[6214].Int( 0,0,223,102 );  // rgb00E4
  pick[6215].Int( 0,31,0,102 );  // rgb0204
  pick[6216].Int( 0,31,31,102 );  // rgb0224
  pick[6217].Int( 0,31,63,102 );  // rgb0244
  pick[6218].Int( 0,31,95,102 );  // rgb0264
  pick[6219].Int( 0,31,127,102 );  // rgb0284
  pick[6220].Int( 0,31,159,102 );  // rgb02A4
  pick[6221].Int( 0,31,191,102 );  // rgb02C4
  pick[6222].Int( 0,31,223,102 );  // rgb02E4
  pick[6223].Int( 0,63,0,102 );  // rgb0404
  pick[6224].Int( 0,63,31,102 );  // rgb0424
  pick[6225].Int( 0,63,63,102 );  // rgb0444
  pick[6226].Int( 0,63,95,102 );  // rgb0464
  pick[6227].Int( 0,63,127,102 );  // rgb0484
  pick[6228].Int( 0,63,159,102 );  // rgb04A4
  pick[6229].Int( 0,63,191,102 );  // rgb04C4
  pick[6230].Int( 0,63,223,102 );  // rgb04E4
  pick[6231].Int( 0,95,0,102 );  // rgb0604
  pick[6232].Int( 0,95,31,102 );  // rgb0624
  pick[6233].Int( 0,95,63,102 );  // rgb0644
  pick[6234].Int( 0,95,95,102 );  // rgb0664
  pick[6235].Int( 0,95,127,102 );  // rgb0684
  pick[6236].Int( 0,95,159,102 );  // rgb06A4
  pick[6237].Int( 0,95,191,102 );  // rgb06C4
  pick[6238].Int( 0,95,223,102 );  // rgb06E4
  pick[6239].Int( 0,127,0,102 );  // rgb0804
  pick[6240].Int( 0,127,31,102 );  // rgb0824
  pick[6241].Int( 0,127,63,102 );  // rgb0844
  pick[6242].Int( 0,127,95,102 );  // rgb0864
  pick[6243].Int( 0,127,127,102 );  // rgb0884
  pick[6244].Int( 0,127,159,102 );  // rgb08A4
  pick[6245].Int( 0,127,191,102 );  // rgb08C4
  pick[6246].Int( 0,127,223,102 );  // rgb08E4
  pick[6247].Int( 0,159,0,102 );  // rgb0A04
  pick[6248].Int( 0,159,31,102 );  // rgb0A24
  pick[6249].Int( 0,159,63,102 );  // rgb0A44
  pick[6250].Int( 0,159,95,102 );  // rgb0A64
  pick[6251].Int( 0,159,127,102 );  // rgb0A84
  pick[6252].Int( 0,159,159,102 );  // rgb0AA4
  pick[6253].Int( 0,159,191,102 );  // rgb0AC4
  pick[6254].Int( 0,159,223,102 );  // rgb0AE4
  pick[6255].Int( 0,191,0,102 );  // rgb0C04
  pick[6256].Int( 0,191,31,102 );  // rgb0C24
  pick[6257].Int( 0,191,63,102 );  // rgb0C44
  pick[6258].Int( 0,191,95,102 );  // rgb0C64
  pick[6259].Int( 0,191,127,102 );  // rgb0C84
  pick[6260].Int( 0,191,159,102 );  // rgb0CA4
  pick[6261].Int( 0,191,191,102 );  // rgb0CC4
  pick[6262].Int( 0,191,223,102 );  // rgb0CE4
  pick[6263].Int( 0,223,0,102 );  // rgb0E04
  pick[6264].Int( 0,223,31,102 );  // rgb0E24
  pick[6265].Int( 0,223,63,102 );  // rgb0E44
  pick[6266].Int( 0,223,95,102 );  // rgb0E64
  pick[6267].Int( 0,223,127,102 );  // rgb0E84
  pick[6268].Int( 0,223,159,102 );  // rgb0EA4
  pick[6269].Int( 0,223,191,102 );  // rgb0EC4
  pick[6270].Int( 0,223,223,102 );  // rgb0EE4
  pick[6271].Int( 31,0,0,102 );  // rgb2004
  pick[6272].Int( 31,0,31,102 );  // rgb2024
  pick[6273].Int( 31,0,63,102 );  // rgb2044
  pick[6274].Int( 31,0,95,102 );  // rgb2064
  pick[6275].Int( 31,0,127,102 );  // rgb2084
  pick[6276].Int( 31,0,159,102 );  // rgb20A4
  pick[6277].Int( 31,0,191,102 );  // rgb20C4
  pick[6278].Int( 31,0,223,102 );  // rgb20E4
  pick[6279].Int( 31,31,0,102 );  // rgb2204
  pick[6280].Int( 31,31,31,102 );  // rgb2224
  pick[6281].Int( 31,31,63,102 );  // rgb2244
  pick[6282].Int( 31,31,95,102 );  // rgb2264
  pick[6283].Int( 31,31,127,102 );  // rgb2284
  pick[6284].Int( 31,31,159,102 );  // rgb22A4
  pick[6285].Int( 31,31,191,102 );  // rgb22C4
  pick[6286].Int( 31,31,223,102 );  // rgb22E4
  pick[6287].Int( 31,63,0,102 );  // rgb2404
  pick[6288].Int( 31,63,31,102 );  // rgb2424
  pick[6289].Int( 31,63,63,102 );  // rgb2444
  pick[6290].Int( 31,63,95,102 );  // rgb2464
  pick[6291].Int( 31,63,127,102 );  // rgb2484
  pick[6292].Int( 31,63,159,102 );  // rgb24A4
  pick[6293].Int( 31,63,191,102 );  // rgb24C4
  pick[6294].Int( 31,63,223,102 );  // rgb24E4
  pick[6295].Int( 31,95,0,102 );  // rgb2604
  pick[6296].Int( 31,95,31,102 );  // rgb2624
  pick[6297].Int( 31,95,63,102 );  // rgb2644
  pick[6298].Int( 31,95,95,102 );  // rgb2664
  pick[6299].Int( 31,95,127,102 );  // rgb2684
  pick[6300].Int( 31,95,159,102 );  // rgb26A4
  pick[6301].Int( 31,95,191,102 );  // rgb26C4
  pick[6302].Int( 31,95,223,102 );  // rgb26E4
  pick[6303].Int( 31,127,0,102 );  // rgb2804
  pick[6304].Int( 31,127,31,102 );  // rgb2824
  pick[6305].Int( 31,127,63,102 );  // rgb2844
  pick[6306].Int( 31,127,95,102 );  // rgb2864
  pick[6307].Int( 31,127,127,102 );  // rgb2884
  pick[6308].Int( 31,127,159,102 );  // rgb28A4
  pick[6309].Int( 31,127,191,102 );  // rgb28C4
  pick[6310].Int( 31,127,223,102 );  // rgb28E4
  pick[6311].Int( 31,159,0,102 );  // rgb2A04
  pick[6312].Int( 31,159,31,102 );  // rgb2A24
  pick[6313].Int( 31,159,63,102 );  // rgb2A44
  pick[6314].Int( 31,159,95,102 );  // rgb2A64
  pick[6315].Int( 31,159,127,102 );  // rgb2A84
  pick[6316].Int( 31,159,159,102 );  // rgb2AA4
  pick[6317].Int( 31,159,191,102 );  // rgb2AC4
  pick[6318].Int( 31,159,223,102 );  // rgb2AE4
  pick[6319].Int( 31,191,0,102 );  // rgb2C04
  pick[6320].Int( 31,191,31,102 );  // rgb2C24
  pick[6321].Int( 31,191,63,102 );  // rgb2C44
  pick[6322].Int( 31,191,95,102 );  // rgb2C64
  pick[6323].Int( 31,191,127,102 );  // rgb2C84
  pick[6324].Int( 31,191,159,102 );  // rgb2CA4
  pick[6325].Int( 31,191,191,102 );  // rgb2CC4
  pick[6326].Int( 31,191,223,102 );  // rgb2CE4
  pick[6327].Int( 31,223,0,102 );  // rgb2E04
  pick[6328].Int( 31,223,31,102 );  // rgb2E24
  pick[6329].Int( 31,223,63,102 );  // rgb2E44
  pick[6330].Int( 31,223,95,102 );  // rgb2E64
  pick[6331].Int( 31,223,127,102 );  // rgb2E84
  pick[6332].Int( 31,223,159,102 );  // rgb2EA4
  pick[6333].Int( 31,223,191,102 );  // rgb2EC4
  pick[6334].Int( 31,223,223,102 );  // rgb2EE4
  pick[6335].Int( 63,0,0,102 );  // rgb4004
  pick[6336].Int( 63,0,31,102 );  // rgb4024
  pick[6337].Int( 63,0,63,102 );  // rgb4044
  pick[6338].Int( 63,0,95,102 );  // rgb4064
  pick[6339].Int( 63,0,127,102 );  // rgb4084
  pick[6340].Int( 63,0,159,102 );  // rgb40A4
  pick[6341].Int( 63,0,191,102 );  // rgb40C4
  pick[6342].Int( 63,0,223,102 );  // rgb40E4
  pick[6343].Int( 63,31,0,102 );  // rgb4204
  pick[6344].Int( 63,31,31,102 );  // rgb4224
  pick[6345].Int( 63,31,63,102 );  // rgb4244
  pick[6346].Int( 63,31,95,102 );  // rgb4264
  pick[6347].Int( 63,31,127,102 );  // rgb4284
  pick[6348].Int( 63,31,159,102 );  // rgb42A4
  pick[6349].Int( 63,31,191,102 );  // rgb42C4
  pick[6350].Int( 63,31,223,102 );  // rgb42E4
  pick[6351].Int( 63,63,0,102 );  // rgb4404
  pick[6352].Int( 63,63,31,102 );  // rgb4424
  pick[6353].Int( 63,63,63,102 );  // rgb4444
  pick[6354].Int( 63,63,95,102 );  // rgb4464
  pick[6355].Int( 63,63,127,102 );  // rgb4484
  pick[6356].Int( 63,63,159,102 );  // rgb44A4
  pick[6357].Int( 63,63,191,102 );  // rgb44C4
  pick[6358].Int( 63,63,223,102 );  // rgb44E4
  pick[6359].Int( 63,95,0,102 );  // rgb4604
  pick[6360].Int( 63,95,31,102 );  // rgb4624
  pick[6361].Int( 63,95,63,102 );  // rgb4644
  pick[6362].Int( 63,95,95,102 );  // rgb4664
  pick[6363].Int( 63,95,127,102 );  // rgb4684
  pick[6364].Int( 63,95,159,102 );  // rgb46A4
  pick[6365].Int( 63,95,191,102 );  // rgb46C4
  pick[6366].Int( 63,95,223,102 );  // rgb46E4
  pick[6367].Int( 63,127,0,102 );  // rgb4804
  pick[6368].Int( 63,127,31,102 );  // rgb4824
  pick[6369].Int( 63,127,63,102 );  // rgb4844
  pick[6370].Int( 63,127,95,102 );  // rgb4864
  pick[6371].Int( 63,127,127,102 );  // rgb4884
  pick[6372].Int( 63,127,159,102 );  // rgb48A4
  pick[6373].Int( 63,127,191,102 );  // rgb48C4
  pick[6374].Int( 63,127,223,102 );  // rgb48E4
  pick[6375].Int( 63,159,0,102 );  // rgb4A04
  pick[6376].Int( 63,159,31,102 );  // rgb4A24
  pick[6377].Int( 63,159,63,102 );  // rgb4A44
  pick[6378].Int( 63,159,95,102 );  // rgb4A64
  pick[6379].Int( 63,159,127,102 );  // rgb4A84
  pick[6380].Int( 63,159,159,102 );  // rgb4AA4
  pick[6381].Int( 63,159,191,102 );  // rgb4AC4
  pick[6382].Int( 63,159,223,102 );  // rgb4AE4
  pick[6383].Int( 63,191,0,102 );  // rgb4C04
  pick[6384].Int( 63,191,31,102 );  // rgb4C24
  pick[6385].Int( 63,191,63,102 );  // rgb4C44
  pick[6386].Int( 63,191,95,102 );  // rgb4C64
  pick[6387].Int( 63,191,127,102 );  // rgb4C84
  pick[6388].Int( 63,191,159,102 );  // rgb4CA4
  pick[6389].Int( 63,191,191,102 );  // rgb4CC4
  pick[6390].Int( 63,191,223,102 );  // rgb4CE4
  pick[6391].Int( 63,223,0,102 );  // rgb4E04
  pick[6392].Int( 63,223,31,102 );  // rgb4E24
  pick[6393].Int( 63,223,63,102 );  // rgb4E44
  pick[6394].Int( 63,223,95,102 );  // rgb4E64
  pick[6395].Int( 63,223,127,102 );  // rgb4E84
  pick[6396].Int( 63,223,159,102 );  // rgb4EA4
  pick[6397].Int( 63,223,191,102 );  // rgb4EC4
  pick[6398].Int( 63,223,223,102 );  // rgb4EE4
  pick[6399].Int( 95,0,0,102 );  // rgb6004
  pick[6400].Int( 95,0,31,102 );  // rgb6024
  pick[6401].Int( 95,0,63,102 );  // rgb6044
  pick[6402].Int( 95,0,95,102 );  // rgb6064
  pick[6403].Int( 95,0,127,102 );  // rgb6084
  pick[6404].Int( 95,0,159,102 );  // rgb60A4
  pick[6405].Int( 95,0,191,102 );  // rgb60C4
  pick[6406].Int( 95,0,223,102 );  // rgb60E4
  pick[6407].Int( 95,31,0,102 );  // rgb6204
  pick[6408].Int( 95,31,31,102 );  // rgb6224
  pick[6409].Int( 95,31,63,102 );  // rgb6244
  pick[6410].Int( 95,31,95,102 );  // rgb6264
  pick[6411].Int( 95,31,127,102 );  // rgb6284
  pick[6412].Int( 95,31,159,102 );  // rgb62A4
  pick[6413].Int( 95,31,191,102 );  // rgb62C4
  pick[6414].Int( 95,31,223,102 );  // rgb62E4
  pick[6415].Int( 95,63,0,102 );  // rgb6404
  pick[6416].Int( 95,63,31,102 );  // rgb6424
  pick[6417].Int( 95,63,63,102 );  // rgb6444
  pick[6418].Int( 95,63,95,102 );  // rgb6464
  pick[6419].Int( 95,63,127,102 );  // rgb6484
  pick[6420].Int( 95,63,159,102 );  // rgb64A4
  pick[6421].Int( 95,63,191,102 );  // rgb64C4
  pick[6422].Int( 95,63,223,102 );  // rgb64E4
  pick[6423].Int( 95,95,0,102 );  // rgb6604
  pick[6424].Int( 95,95,31,102 );  // rgb6624
  pick[6425].Int( 95,95,63,102 );  // rgb6644
  pick[6426].Int( 95,95,95,102 );  // rgb6664
  pick[6427].Int( 95,95,127,102 );  // rgb6684
  pick[6428].Int( 95,95,159,102 );  // rgb66A4
  pick[6429].Int( 95,95,191,102 );  // rgb66C4
  pick[6430].Int( 95,95,223,102 );  // rgb66E4
  pick[6431].Int( 95,127,0,102 );  // rgb6804
  pick[6432].Int( 95,127,31,102 );  // rgb6824
  pick[6433].Int( 95,127,63,102 );  // rgb6844
  pick[6434].Int( 95,127,95,102 );  // rgb6864
  pick[6435].Int( 95,127,127,102 );  // rgb6884
  pick[6436].Int( 95,127,159,102 );  // rgb68A4
  pick[6437].Int( 95,127,191,102 );  // rgb68C4
  pick[6438].Int( 95,127,223,102 );  // rgb68E4
  pick[6439].Int( 95,159,0,102 );  // rgb6A04
  pick[6440].Int( 95,159,31,102 );  // rgb6A24
  pick[6441].Int( 95,159,63,102 );  // rgb6A44
  pick[6442].Int( 95,159,95,102 );  // rgb6A64
  pick[6443].Int( 95,159,127,102 );  // rgb6A84
  pick[6444].Int( 95,159,159,102 );  // rgb6AA4
  pick[6445].Int( 95,159,191,102 );  // rgb6AC4
  pick[6446].Int( 95,159,223,102 );  // rgb6AE4
  pick[6447].Int( 95,191,0,102 );  // rgb6C04
  pick[6448].Int( 95,191,31,102 );  // rgb6C24
  pick[6449].Int( 95,191,63,102 );  // rgb6C44
  pick[6450].Int( 95,191,95,102 );  // rgb6C64
  pick[6451].Int( 95,191,127,102 );  // rgb6C84
  pick[6452].Int( 95,191,159,102 );  // rgb6CA4
  pick[6453].Int( 95,191,191,102 );  // rgb6CC4
  pick[6454].Int( 95,191,223,102 );  // rgb6CE4
  pick[6455].Int( 95,223,0,102 );  // rgb6E04
  pick[6456].Int( 95,223,31,102 );  // rgb6E24
  pick[6457].Int( 95,223,63,102 );  // rgb6E44
  pick[6458].Int( 95,223,95,102 );  // rgb6E64
  pick[6459].Int( 95,223,127,102 );  // rgb6E84
  pick[6460].Int( 95,223,159,102 );  // rgb6EA4
  pick[6461].Int( 95,223,191,102 );  // rgb6EC4
  pick[6462].Int( 95,223,223,102 );  // rgb6EE4
  pick[6463].Int( 127,0,0,102 );  // rgb8004
  pick[6464].Int( 127,0,31,102 );  // rgb8024
  pick[6465].Int( 127,0,63,102 );  // rgb8044
  pick[6466].Int( 127,0,95,102 );  // rgb8064
  pick[6467].Int( 127,0,127,102 );  // rgb8084
  pick[6468].Int( 127,0,159,102 );  // rgb80A4
  pick[6469].Int( 127,0,191,102 );  // rgb80C4
  pick[6470].Int( 127,0,223,102 );  // rgb80E4
  pick[6471].Int( 127,31,0,102 );  // rgb8204
  pick[6472].Int( 127,31,31,102 );  // rgb8224
  pick[6473].Int( 127,31,63,102 );  // rgb8244
  pick[6474].Int( 127,31,95,102 );  // rgb8264
  pick[6475].Int( 127,31,127,102 );  // rgb8284
  pick[6476].Int( 127,31,159,102 );  // rgb82A4
  pick[6477].Int( 127,31,191,102 );  // rgb82C4
  pick[6478].Int( 127,31,223,102 );  // rgb82E4
  pick[6479].Int( 127,63,0,102 );  // rgb8404
  pick[6480].Int( 127,63,31,102 );  // rgb8424
  pick[6481].Int( 127,63,63,102 );  // rgb8444
  pick[6482].Int( 127,63,95,102 );  // rgb8464
  pick[6483].Int( 127,63,127,102 );  // rgb8484
  pick[6484].Int( 127,63,159,102 );  // rgb84A4
  pick[6485].Int( 127,63,191,102 );  // rgb84C4
  pick[6486].Int( 127,63,223,102 );  // rgb84E4
  pick[6487].Int( 127,95,0,102 );  // rgb8604
  pick[6488].Int( 127,95,31,102 );  // rgb8624
  pick[6489].Int( 127,95,63,102 );  // rgb8644
  pick[6490].Int( 127,95,95,102 );  // rgb8664
  pick[6491].Int( 127,95,127,102 );  // rgb8684
  pick[6492].Int( 127,95,159,102 );  // rgb86A4
  pick[6493].Int( 127,95,191,102 );  // rgb86C4
  pick[6494].Int( 127,95,223,102 );  // rgb86E4
  pick[6495].Int( 127,127,0,102 );  // rgb8804
  pick[6496].Int( 127,127,31,102 );  // rgb8824
  pick[6497].Int( 127,127,63,102 );  // rgb8844
  pick[6498].Int( 127,127,95,102 );  // rgb8864
  pick[6499].Int( 127,127,127,102 );  // rgb8884
  pick[6500].Int( 127,127,159,102 );  // rgb88A4
  pick[6501].Int( 127,127,191,102 );  // rgb88C4
  pick[6502].Int( 127,127,223,102 );  // rgb88E4
  pick[6503].Int( 127,159,0,102 );  // rgb8A04
  pick[6504].Int( 127,159,31,102 );  // rgb8A24
  pick[6505].Int( 127,159,63,102 );  // rgb8A44
  pick[6506].Int( 127,159,95,102 );  // rgb8A64
  pick[6507].Int( 127,159,127,102 );  // rgb8A84
  pick[6508].Int( 127,159,159,102 );  // rgb8AA4
  pick[6509].Int( 127,159,191,102 );  // rgb8AC4
  pick[6510].Int( 127,159,223,102 );  // rgb8AE4
  pick[6511].Int( 127,191,0,102 );  // rgb8C04
  pick[6512].Int( 127,191,31,102 );  // rgb8C24
  pick[6513].Int( 127,191,63,102 );  // rgb8C44
  pick[6514].Int( 127,191,95,102 );  // rgb8C64
  pick[6515].Int( 127,191,127,102 );  // rgb8C84
  pick[6516].Int( 127,191,159,102 );  // rgb8CA4
  pick[6517].Int( 127,191,191,102 );  // rgb8CC4
  pick[6518].Int( 127,191,223,102 );  // rgb8CE4
  pick[6519].Int( 127,223,0,102 );  // rgb8E04
  pick[6520].Int( 127,223,31,102 );  // rgb8E24
  pick[6521].Int( 127,223,63,102 );  // rgb8E44
  pick[6522].Int( 127,223,95,102 );  // rgb8E64
  pick[6523].Int( 127,223,127,102 );  // rgb8E84
  pick[6524].Int( 127,223,159,102 );  // rgb8EA4
  pick[6525].Int( 127,223,191,102 );  // rgb8EC4
  pick[6526].Int( 127,223,223,102 );  // rgb8EE4
  pick[6527].Int( 159,0,0,102 );  // rgbA004
  pick[6528].Int( 159,0,31,102 );  // rgbA024
  pick[6529].Int( 159,0,63,102 );  // rgbA044
  pick[6530].Int( 159,0,95,102 );  // rgbA064
  pick[6531].Int( 159,0,127,102 );  // rgbA084
  pick[6532].Int( 159,0,159,102 );  // rgbA0A4
  pick[6533].Int( 159,0,191,102 );  // rgbA0C4
  pick[6534].Int( 159,0,223,102 );  // rgbA0E4
  pick[6535].Int( 159,31,0,102 );  // rgbA204
  pick[6536].Int( 159,31,31,102 );  // rgbA224
  pick[6537].Int( 159,31,63,102 );  // rgbA244
  pick[6538].Int( 159,31,95,102 );  // rgbA264
  pick[6539].Int( 159,31,127,102 );  // rgbA284
  pick[6540].Int( 159,31,159,102 );  // rgbA2A4
  pick[6541].Int( 159,31,191,102 );  // rgbA2C4
  pick[6542].Int( 159,31,223,102 );  // rgbA2E4
  pick[6543].Int( 159,63,0,102 );  // rgbA404
  pick[6544].Int( 159,63,31,102 );  // rgbA424
  pick[6545].Int( 159,63,63,102 );  // rgbA444
  pick[6546].Int( 159,63,95,102 );  // rgbA464
  pick[6547].Int( 159,63,127,102 );  // rgbA484
  pick[6548].Int( 159,63,159,102 );  // rgbA4A4
  pick[6549].Int( 159,63,191,102 );  // rgbA4C4
  pick[6550].Int( 159,63,223,102 );  // rgbA4E4
  pick[6551].Int( 159,95,0,102 );  // rgbA604
  pick[6552].Int( 159,95,31,102 );  // rgbA624
  pick[6553].Int( 159,95,63,102 );  // rgbA644
  pick[6554].Int( 159,95,95,102 );  // rgbA664
  pick[6555].Int( 159,95,127,102 );  // rgbA684
  pick[6556].Int( 159,95,159,102 );  // rgbA6A4
  pick[6557].Int( 159,95,191,102 );  // rgbA6C4
  pick[6558].Int( 159,95,223,102 );  // rgbA6E4
  pick[6559].Int( 159,127,0,102 );  // rgbA804
  pick[6560].Int( 159,127,31,102 );  // rgbA824
  pick[6561].Int( 159,127,63,102 );  // rgbA844
  pick[6562].Int( 159,127,95,102 );  // rgbA864
  pick[6563].Int( 159,127,127,102 );  // rgbA884
  pick[6564].Int( 159,127,159,102 );  // rgbA8A4
  pick[6565].Int( 159,127,191,102 );  // rgbA8C4
  pick[6566].Int( 159,127,223,102 );  // rgbA8E4
  pick[6567].Int( 159,159,0,102 );  // rgbAA04
  pick[6568].Int( 159,159,31,102 );  // rgbAA24
  pick[6569].Int( 159,159,63,102 );  // rgbAA44
  pick[6570].Int( 159,159,95,102 );  // rgbAA64
  pick[6571].Int( 159,159,127,102 );  // rgbAA84
  pick[6572].Int( 159,159,159,102 );  // rgbAAA4
  pick[6573].Int( 159,159,191,102 );  // rgbAAC4
  pick[6574].Int( 159,159,223,102 );  // rgbAAE4
  pick[6575].Int( 159,191,0,102 );  // rgbAC04
  pick[6576].Int( 159,191,31,102 );  // rgbAC24
  pick[6577].Int( 159,191,63,102 );  // rgbAC44
  pick[6578].Int( 159,191,95,102 );  // rgbAC64
  pick[6579].Int( 159,191,127,102 );  // rgbAC84
  pick[6580].Int( 159,191,159,102 );  // rgbACA4
  pick[6581].Int( 159,191,191,102 );  // rgbACC4
  pick[6582].Int( 159,191,223,102 );  // rgbACE4
  pick[6583].Int( 159,223,0,102 );  // rgbAE04
  pick[6584].Int( 159,223,31,102 );  // rgbAE24
  pick[6585].Int( 159,223,63,102 );  // rgbAE44
  pick[6586].Int( 159,223,95,102 );  // rgbAE64
  pick[6587].Int( 159,223,127,102 );  // rgbAE84
  pick[6588].Int( 159,223,159,102 );  // rgbAEA4
  pick[6589].Int( 159,223,191,102 );  // rgbAEC4
  pick[6590].Int( 159,223,223,102 );  // rgbAEE4
  pick[6591].Int( 191,0,0,102 );  // rgbC004
  pick[6592].Int( 191,0,31,102 );  // rgbC024
  pick[6593].Int( 191,0,63,102 );  // rgbC044
  pick[6594].Int( 191,0,95,102 );  // rgbC064
  pick[6595].Int( 191,0,127,102 );  // rgbC084
  pick[6596].Int( 191,0,159,102 );  // rgbC0A4
  pick[6597].Int( 191,0,191,102 );  // rgbC0C4
  pick[6598].Int( 191,0,223,102 );  // rgbC0E4
  pick[6599].Int( 191,31,0,102 );  // rgbC204
  pick[6600].Int( 191,31,31,102 );  // rgbC224
  pick[6601].Int( 191,31,63,102 );  // rgbC244
  pick[6602].Int( 191,31,95,102 );  // rgbC264
  pick[6603].Int( 191,31,127,102 );  // rgbC284
  pick[6604].Int( 191,31,159,102 );  // rgbC2A4
  pick[6605].Int( 191,31,191,102 );  // rgbC2C4
  pick[6606].Int( 191,31,223,102 );  // rgbC2E4
  pick[6607].Int( 191,63,0,102 );  // rgbC404
  pick[6608].Int( 191,63,31,102 );  // rgbC424
  pick[6609].Int( 191,63,63,102 );  // rgbC444
  pick[6610].Int( 191,63,95,102 );  // rgbC464
  pick[6611].Int( 191,63,127,102 );  // rgbC484
  pick[6612].Int( 191,63,159,102 );  // rgbC4A4
  pick[6613].Int( 191,63,191,102 );  // rgbC4C4
  pick[6614].Int( 191,63,223,102 );  // rgbC4E4
  pick[6615].Int( 191,95,0,102 );  // rgbC604
  pick[6616].Int( 191,95,31,102 );  // rgbC624
  pick[6617].Int( 191,95,63,102 );  // rgbC644
  pick[6618].Int( 191,95,95,102 );  // rgbC664
  pick[6619].Int( 191,95,127,102 );  // rgbC684
  pick[6620].Int( 191,95,159,102 );  // rgbC6A4
  pick[6621].Int( 191,95,191,102 );  // rgbC6C4
  pick[6622].Int( 191,95,223,102 );  // rgbC6E4
  pick[6623].Int( 191,127,0,102 );  // rgbC804
  pick[6624].Int( 191,127,31,102 );  // rgbC824
  pick[6625].Int( 191,127,63,102 );  // rgbC844
  pick[6626].Int( 191,127,95,102 );  // rgbC864
  pick[6627].Int( 191,127,127,102 );  // rgbC884
  pick[6628].Int( 191,127,159,102 );  // rgbC8A4
  pick[6629].Int( 191,127,191,102 );  // rgbC8C4
  pick[6630].Int( 191,127,223,102 );  // rgbC8E4
  pick[6631].Int( 191,159,0,102 );  // rgbCA04
  pick[6632].Int( 191,159,31,102 );  // rgbCA24
  pick[6633].Int( 191,159,63,102 );  // rgbCA44
  pick[6634].Int( 191,159,95,102 );  // rgbCA64
  pick[6635].Int( 191,159,127,102 );  // rgbCA84
  pick[6636].Int( 191,159,159,102 );  // rgbCAA4
  pick[6637].Int( 191,159,191,102 );  // rgbCAC4
  pick[6638].Int( 191,159,223,102 );  // rgbCAE4
  pick[6639].Int( 191,191,0,102 );  // rgbCC04
  pick[6640].Int( 191,191,31,102 );  // rgbCC24
  pick[6641].Int( 191,191,63,102 );  // rgbCC44
  pick[6642].Int( 191,191,95,102 );  // rgbCC64
  pick[6643].Int( 191,191,127,102 );  // rgbCC84
  pick[6644].Int( 191,191,159,102 );  // rgbCCA4
  pick[6645].Int( 191,191,191,102 );  // rgbCCC4
  pick[6646].Int( 191,191,223,102 );  // rgbCCE4
  pick[6647].Int( 191,223,0,102 );  // rgbCE04
  pick[6648].Int( 191,223,31,102 );  // rgbCE24
  pick[6649].Int( 191,223,63,102 );  // rgbCE44
  pick[6650].Int( 191,223,95,102 );  // rgbCE64
  pick[6651].Int( 191,223,127,102 );  // rgbCE84
  pick[6652].Int( 191,223,159,102 );  // rgbCEA4
  pick[6653].Int( 191,223,191,102 );  // rgbCEC4
  pick[6654].Int( 191,223,223,102 );  // rgbCEE4
  pick[6655].Int( 223,0,0,102 );  // rgbE004
  pick[6656].Int( 223,0,31,102 );  // rgbE024
  pick[6657].Int( 223,0,63,102 );  // rgbE044
  pick[6658].Int( 223,0,95,102 );  // rgbE064
  pick[6659].Int( 223,0,127,102 );  // rgbE084
  pick[6660].Int( 223,0,159,102 );  // rgbE0A4
  pick[6661].Int( 223,0,191,102 );  // rgbE0C4
  pick[6662].Int( 223,0,223,102 );  // rgbE0E4
  pick[6663].Int( 223,31,0,102 );  // rgbE204
  pick[6664].Int( 223,31,31,102 );  // rgbE224
  pick[6665].Int( 223,31,63,102 );  // rgbE244
  pick[6666].Int( 223,31,95,102 );  // rgbE264
  pick[6667].Int( 223,31,127,102 );  // rgbE284
  pick[6668].Int( 223,31,159,102 );  // rgbE2A4
  pick[6669].Int( 223,31,191,102 );  // rgbE2C4
  pick[6670].Int( 223,31,223,102 );  // rgbE2E4
  pick[6671].Int( 223,63,0,102 );  // rgbE404
  pick[6672].Int( 223,63,31,102 );  // rgbE424
  pick[6673].Int( 223,63,63,102 );  // rgbE444
  pick[6674].Int( 223,63,95,102 );  // rgbE464
  pick[6675].Int( 223,63,127,102 );  // rgbE484
  pick[6676].Int( 223,63,159,102 );  // rgbE4A4
  pick[6677].Int( 223,63,191,102 );  // rgbE4C4
  pick[6678].Int( 223,63,223,102 );  // rgbE4E4
  pick[6679].Int( 223,95,0,102 );  // rgbE604
  pick[6680].Int( 223,95,31,102 );  // rgbE624
  pick[6681].Int( 223,95,63,102 );  // rgbE644
  pick[6682].Int( 223,95,95,102 );  // rgbE664
  pick[6683].Int( 223,95,127,102 );  // rgbE684
  pick[6684].Int( 223,95,159,102 );  // rgbE6A4
  pick[6685].Int( 223,95,191,102 );  // rgbE6C4
  pick[6686].Int( 223,95,223,102 );  // rgbE6E4
  pick[6687].Int( 223,127,0,102 );  // rgbE804
  pick[6688].Int( 223,127,31,102 );  // rgbE824
  pick[6689].Int( 223,127,63,102 );  // rgbE844
  pick[6690].Int( 223,127,95,102 );  // rgbE864
  pick[6691].Int( 223,127,127,102 );  // rgbE884
  pick[6692].Int( 223,127,159,102 );  // rgbE8A4
  pick[6693].Int( 223,127,191,102 );  // rgbE8C4
  pick[6694].Int( 223,127,223,102 );  // rgbE8E4
  pick[6695].Int( 223,159,0,102 );  // rgbEA04
  pick[6696].Int( 223,159,31,102 );  // rgbEA24
  pick[6697].Int( 223,159,63,102 );  // rgbEA44
  pick[6698].Int( 223,159,95,102 );  // rgbEA64
  pick[6699].Int( 223,159,127,102 );  // rgbEA84
  pick[6700].Int( 223,159,159,102 );  // rgbEAA4
  pick[6701].Int( 223,159,191,102 );  // rgbEAC4
  pick[6702].Int( 223,159,223,102 );  // rgbEAE4
  pick[6703].Int( 223,191,0,102 );  // rgbEC04
  pick[6704].Int( 223,191,31,102 );  // rgbEC24
  pick[6705].Int( 223,191,63,102 );  // rgbEC44
  pick[6706].Int( 223,191,95,102 );  // rgbEC64
  pick[6707].Int( 223,191,127,102 );  // rgbEC84
  pick[6708].Int( 223,191,159,102 );  // rgbECA4
  pick[6709].Int( 223,191,191,102 );  // rgbECC4
  pick[6710].Int( 223,191,223,102 );  // rgbECE4
  pick[6711].Int( 223,223,0,102 );  // rgbEE04
  pick[6712].Int( 223,223,31,102 );  // rgbEE24
  pick[6713].Int( 223,223,63,102 );  // rgbEE44
  pick[6714].Int( 223,223,95,102 );  // rgbEE64
  pick[6715].Int( 223,223,127,102 );  // rgbEE84
  pick[6716].Int( 223,223,159,102 );  // rgbEEA4
  pick[6717].Int( 223,223,191,102 );  // rgbEEC4
  pick[6718].Int( 223,223,223,102 );  // rgbEEE4
  pick[6719].Int( 0,0,0,127 );  // rgb0005
  pick[6720].Int( 0,0,31,127 );  // rgb0025
  pick[6721].Int( 0,0,63,127 );  // rgb0045
  pick[6722].Int( 0,0,95,127 );  // rgb0065
  pick[6723].Int( 0,0,127,127 );  // rgb0085
  pick[6724].Int( 0,0,159,127 );  // rgb00A5
  pick[6725].Int( 0,0,191,127 );  // rgb00C5
  pick[6726].Int( 0,0,223,127 );  // rgb00E5
  pick[6727].Int( 0,31,0,127 );  // rgb0205
  pick[6728].Int( 0,31,31,127 );  // rgb0225
  pick[6729].Int( 0,31,63,127 );  // rgb0245
  pick[6730].Int( 0,31,95,127 );  // rgb0265
  pick[6731].Int( 0,31,127,127 );  // rgb0285
  pick[6732].Int( 0,31,159,127 );  // rgb02A5
  pick[6733].Int( 0,31,191,127 );  // rgb02C5
  pick[6734].Int( 0,31,223,127 );  // rgb02E5
  pick[6735].Int( 0,63,0,127 );  // rgb0405
  pick[6736].Int( 0,63,31,127 );  // rgb0425
  pick[6737].Int( 0,63,63,127 );  // rgb0445
  pick[6738].Int( 0,63,95,127 );  // rgb0465
  pick[6739].Int( 0,63,127,127 );  // rgb0485
  pick[6740].Int( 0,63,159,127 );  // rgb04A5
  pick[6741].Int( 0,63,191,127 );  // rgb04C5
  pick[6742].Int( 0,63,223,127 );  // rgb04E5
  pick[6743].Int( 0,95,0,127 );  // rgb0605
  pick[6744].Int( 0,95,31,127 );  // rgb0625
  pick[6745].Int( 0,95,63,127 );  // rgb0645
  pick[6746].Int( 0,95,95,127 );  // rgb0665
  pick[6747].Int( 0,95,127,127 );  // rgb0685
  pick[6748].Int( 0,95,159,127 );  // rgb06A5
  pick[6749].Int( 0,95,191,127 );  // rgb06C5
  pick[6750].Int( 0,95,223,127 );  // rgb06E5
  pick[6751].Int( 0,127,0,127 );  // rgb0805
  pick[6752].Int( 0,127,31,127 );  // rgb0825
  pick[6753].Int( 0,127,63,127 );  // rgb0845
  pick[6754].Int( 0,127,95,127 );  // rgb0865
  pick[6755].Int( 0,127,127,127 );  // rgb0885
  pick[6756].Int( 0,127,159,127 );  // rgb08A5
  pick[6757].Int( 0,127,191,127 );  // rgb08C5
  pick[6758].Int( 0,127,223,127 );  // rgb08E5
  pick[6759].Int( 0,159,0,127 );  // rgb0A05
  pick[6760].Int( 0,159,31,127 );  // rgb0A25
  pick[6761].Int( 0,159,63,127 );  // rgb0A45
  pick[6762].Int( 0,159,95,127 );  // rgb0A65
  pick[6763].Int( 0,159,127,127 );  // rgb0A85
  pick[6764].Int( 0,159,159,127 );  // rgb0AA5
  pick[6765].Int( 0,159,191,127 );  // rgb0AC5
  pick[6766].Int( 0,159,223,127 );  // rgb0AE5
  pick[6767].Int( 0,191,0,127 );  // rgb0C05
  pick[6768].Int( 0,191,31,127 );  // rgb0C25
  pick[6769].Int( 0,191,63,127 );  // rgb0C45
  pick[6770].Int( 0,191,95,127 );  // rgb0C65
  pick[6771].Int( 0,191,127,127 );  // rgb0C85
  pick[6772].Int( 0,191,159,127 );  // rgb0CA5
  pick[6773].Int( 0,191,191,127 );  // rgb0CC5
  pick[6774].Int( 0,191,223,127 );  // rgb0CE5
  pick[6775].Int( 0,223,0,127 );  // rgb0E05
  pick[6776].Int( 0,223,31,127 );  // rgb0E25
  pick[6777].Int( 0,223,63,127 );  // rgb0E45
  pick[6778].Int( 0,223,95,127 );  // rgb0E65
  pick[6779].Int( 0,223,127,127 );  // rgb0E85
  pick[6780].Int( 0,223,159,127 );  // rgb0EA5
  pick[6781].Int( 0,223,191,127 );  // rgb0EC5
  pick[6782].Int( 0,223,223,127 );  // rgb0EE5
  pick[6783].Int( 31,0,0,127 );  // rgb2005
  pick[6784].Int( 31,0,31,127 );  // rgb2025
  pick[6785].Int( 31,0,63,127 );  // rgb2045
  pick[6786].Int( 31,0,95,127 );  // rgb2065
  pick[6787].Int( 31,0,127,127 );  // rgb2085
  pick[6788].Int( 31,0,159,127 );  // rgb20A5
  pick[6789].Int( 31,0,191,127 );  // rgb20C5
  pick[6790].Int( 31,0,223,127 );  // rgb20E5
  pick[6791].Int( 31,31,0,127 );  // rgb2205
  pick[6792].Int( 31,31,31,127 );  // rgb2225
  pick[6793].Int( 31,31,63,127 );  // rgb2245
  pick[6794].Int( 31,31,95,127 );  // rgb2265
  pick[6795].Int( 31,31,127,127 );  // rgb2285
  pick[6796].Int( 31,31,159,127 );  // rgb22A5
  pick[6797].Int( 31,31,191,127 );  // rgb22C5
  pick[6798].Int( 31,31,223,127 );  // rgb22E5
  pick[6799].Int( 31,63,0,127 );  // rgb2405
  pick[6800].Int( 31,63,31,127 );  // rgb2425
  pick[6801].Int( 31,63,63,127 );  // rgb2445
  pick[6802].Int( 31,63,95,127 );  // rgb2465
  pick[6803].Int( 31,63,127,127 );  // rgb2485
  pick[6804].Int( 31,63,159,127 );  // rgb24A5
  pick[6805].Int( 31,63,191,127 );  // rgb24C5
  pick[6806].Int( 31,63,223,127 );  // rgb24E5
  pick[6807].Int( 31,95,0,127 );  // rgb2605
  pick[6808].Int( 31,95,31,127 );  // rgb2625
  pick[6809].Int( 31,95,63,127 );  // rgb2645
  pick[6810].Int( 31,95,95,127 );  // rgb2665
  pick[6811].Int( 31,95,127,127 );  // rgb2685
  pick[6812].Int( 31,95,159,127 );  // rgb26A5
  pick[6813].Int( 31,95,191,127 );  // rgb26C5
  pick[6814].Int( 31,95,223,127 );  // rgb26E5
  pick[6815].Int( 31,127,0,127 );  // rgb2805
  pick[6816].Int( 31,127,31,127 );  // rgb2825
  pick[6817].Int( 31,127,63,127 );  // rgb2845
  pick[6818].Int( 31,127,95,127 );  // rgb2865
  pick[6819].Int( 31,127,127,127 );  // rgb2885
  pick[6820].Int( 31,127,159,127 );  // rgb28A5
  pick[6821].Int( 31,127,191,127 );  // rgb28C5
  pick[6822].Int( 31,127,223,127 );  // rgb28E5
  pick[6823].Int( 31,159,0,127 );  // rgb2A05
  pick[6824].Int( 31,159,31,127 );  // rgb2A25
  pick[6825].Int( 31,159,63,127 );  // rgb2A45
  pick[6826].Int( 31,159,95,127 );  // rgb2A65
  pick[6827].Int( 31,159,127,127 );  // rgb2A85
  pick[6828].Int( 31,159,159,127 );  // rgb2AA5
  pick[6829].Int( 31,159,191,127 );  // rgb2AC5
  pick[6830].Int( 31,159,223,127 );  // rgb2AE5
  pick[6831].Int( 31,191,0,127 );  // rgb2C05
  pick[6832].Int( 31,191,31,127 );  // rgb2C25
  pick[6833].Int( 31,191,63,127 );  // rgb2C45
  pick[6834].Int( 31,191,95,127 );  // rgb2C65
  pick[6835].Int( 31,191,127,127 );  // rgb2C85
  pick[6836].Int( 31,191,159,127 );  // rgb2CA5
  pick[6837].Int( 31,191,191,127 );  // rgb2CC5
  pick[6838].Int( 31,191,223,127 );  // rgb2CE5
  pick[6839].Int( 31,223,0,127 );  // rgb2E05
  pick[6840].Int( 31,223,31,127 );  // rgb2E25
  pick[6841].Int( 31,223,63,127 );  // rgb2E45
  pick[6842].Int( 31,223,95,127 );  // rgb2E65
  pick[6843].Int( 31,223,127,127 );  // rgb2E85
  pick[6844].Int( 31,223,159,127 );  // rgb2EA5
  pick[6845].Int( 31,223,191,127 );  // rgb2EC5
  pick[6846].Int( 31,223,223,127 );  // rgb2EE5
  pick[6847].Int( 63,0,0,127 );  // rgb4005
  pick[6848].Int( 63,0,31,127 );  // rgb4025
  pick[6849].Int( 63,0,63,127 );  // rgb4045
  pick[6850].Int( 63,0,95,127 );  // rgb4065
  pick[6851].Int( 63,0,127,127 );  // rgb4085
  pick[6852].Int( 63,0,159,127 );  // rgb40A5
  pick[6853].Int( 63,0,191,127 );  // rgb40C5
  pick[6854].Int( 63,0,223,127 );  // rgb40E5
  pick[6855].Int( 63,31,0,127 );  // rgb4205
  pick[6856].Int( 63,31,31,127 );  // rgb4225
  pick[6857].Int( 63,31,63,127 );  // rgb4245
  pick[6858].Int( 63,31,95,127 );  // rgb4265
  pick[6859].Int( 63,31,127,127 );  // rgb4285
  pick[6860].Int( 63,31,159,127 );  // rgb42A5
  pick[6861].Int( 63,31,191,127 );  // rgb42C5
  pick[6862].Int( 63,31,223,127 );  // rgb42E5
  pick[6863].Int( 63,63,0,127 );  // rgb4405
  pick[6864].Int( 63,63,31,127 );  // rgb4425
  pick[6865].Int( 63,63,63,127 );  // rgb4445
  pick[6866].Int( 63,63,95,127 );  // rgb4465
  pick[6867].Int( 63,63,127,127 );  // rgb4485
  pick[6868].Int( 63,63,159,127 );  // rgb44A5
  pick[6869].Int( 63,63,191,127 );  // rgb44C5
  pick[6870].Int( 63,63,223,127 );  // rgb44E5
  pick[6871].Int( 63,95,0,127 );  // rgb4605
  pick[6872].Int( 63,95,31,127 );  // rgb4625
  pick[6873].Int( 63,95,63,127 );  // rgb4645
  pick[6874].Int( 63,95,95,127 );  // rgb4665
  pick[6875].Int( 63,95,127,127 );  // rgb4685
  pick[6876].Int( 63,95,159,127 );  // rgb46A5
  pick[6877].Int( 63,95,191,127 );  // rgb46C5
  pick[6878].Int( 63,95,223,127 );  // rgb46E5
  pick[6879].Int( 63,127,0,127 );  // rgb4805
  pick[6880].Int( 63,127,31,127 );  // rgb4825
  pick[6881].Int( 63,127,63,127 );  // rgb4845
  pick[6882].Int( 63,127,95,127 );  // rgb4865
  pick[6883].Int( 63,127,127,127 );  // rgb4885
  pick[6884].Int( 63,127,159,127 );  // rgb48A5
  pick[6885].Int( 63,127,191,127 );  // rgb48C5
  pick[6886].Int( 63,127,223,127 );  // rgb48E5
  pick[6887].Int( 63,159,0,127 );  // rgb4A05
  pick[6888].Int( 63,159,31,127 );  // rgb4A25
  pick[6889].Int( 63,159,63,127 );  // rgb4A45
  pick[6890].Int( 63,159,95,127 );  // rgb4A65
  pick[6891].Int( 63,159,127,127 );  // rgb4A85
  pick[6892].Int( 63,159,159,127 );  // rgb4AA5
  pick[6893].Int( 63,159,191,127 );  // rgb4AC5
  pick[6894].Int( 63,159,223,127 );  // rgb4AE5
  pick[6895].Int( 63,191,0,127 );  // rgb4C05
  pick[6896].Int( 63,191,31,127 );  // rgb4C25
  pick[6897].Int( 63,191,63,127 );  // rgb4C45
  pick[6898].Int( 63,191,95,127 );  // rgb4C65
  pick[6899].Int( 63,191,127,127 );  // rgb4C85
  pick[6900].Int( 63,191,159,127 );  // rgb4CA5
  pick[6901].Int( 63,191,191,127 );  // rgb4CC5
  pick[6902].Int( 63,191,223,127 );  // rgb4CE5
  pick[6903].Int( 63,223,0,127 );  // rgb4E05
  pick[6904].Int( 63,223,31,127 );  // rgb4E25
  pick[6905].Int( 63,223,63,127 );  // rgb4E45
  pick[6906].Int( 63,223,95,127 );  // rgb4E65
  pick[6907].Int( 63,223,127,127 );  // rgb4E85
  pick[6908].Int( 63,223,159,127 );  // rgb4EA5
  pick[6909].Int( 63,223,191,127 );  // rgb4EC5
  pick[6910].Int( 63,223,223,127 );  // rgb4EE5
  pick[6911].Int( 95,0,0,127 );  // rgb6005
  pick[6912].Int( 95,0,31,127 );  // rgb6025
  pick[6913].Int( 95,0,63,127 );  // rgb6045
  pick[6914].Int( 95,0,95,127 );  // rgb6065
  pick[6915].Int( 95,0,127,127 );  // rgb6085
  pick[6916].Int( 95,0,159,127 );  // rgb60A5
  pick[6917].Int( 95,0,191,127 );  // rgb60C5
  pick[6918].Int( 95,0,223,127 );  // rgb60E5
  pick[6919].Int( 95,31,0,127 );  // rgb6205
  pick[6920].Int( 95,31,31,127 );  // rgb6225
  pick[6921].Int( 95,31,63,127 );  // rgb6245
  pick[6922].Int( 95,31,95,127 );  // rgb6265
  pick[6923].Int( 95,31,127,127 );  // rgb6285
  pick[6924].Int( 95,31,159,127 );  // rgb62A5
  pick[6925].Int( 95,31,191,127 );  // rgb62C5
  pick[6926].Int( 95,31,223,127 );  // rgb62E5
  pick[6927].Int( 95,63,0,127 );  // rgb6405
  pick[6928].Int( 95,63,31,127 );  // rgb6425
  pick[6929].Int( 95,63,63,127 );  // rgb6445
  pick[6930].Int( 95,63,95,127 );  // rgb6465
  pick[6931].Int( 95,63,127,127 );  // rgb6485
  pick[6932].Int( 95,63,159,127 );  // rgb64A5
  pick[6933].Int( 95,63,191,127 );  // rgb64C5
  pick[6934].Int( 95,63,223,127 );  // rgb64E5
  pick[6935].Int( 95,95,0,127 );  // rgb6605
  pick[6936].Int( 95,95,31,127 );  // rgb6625
  pick[6937].Int( 95,95,63,127 );  // rgb6645
  pick[6938].Int( 95,95,95,127 );  // rgb6665
  pick[6939].Int( 95,95,127,127 );  // rgb6685
  pick[6940].Int( 95,95,159,127 );  // rgb66A5
  pick[6941].Int( 95,95,191,127 );  // rgb66C5
  pick[6942].Int( 95,95,223,127 );  // rgb66E5
  pick[6943].Int( 95,127,0,127 );  // rgb6805
  pick[6944].Int( 95,127,31,127 );  // rgb6825
  pick[6945].Int( 95,127,63,127 );  // rgb6845
  pick[6946].Int( 95,127,95,127 );  // rgb6865
  pick[6947].Int( 95,127,127,127 );  // rgb6885
  pick[6948].Int( 95,127,159,127 );  // rgb68A5
  pick[6949].Int( 95,127,191,127 );  // rgb68C5
  pick[6950].Int( 95,127,223,127 );  // rgb68E5
  pick[6951].Int( 95,159,0,127 );  // rgb6A05
  pick[6952].Int( 95,159,31,127 );  // rgb6A25
  pick[6953].Int( 95,159,63,127 );  // rgb6A45
  pick[6954].Int( 95,159,95,127 );  // rgb6A65
  pick[6955].Int( 95,159,127,127 );  // rgb6A85
  pick[6956].Int( 95,159,159,127 );  // rgb6AA5
  pick[6957].Int( 95,159,191,127 );  // rgb6AC5
  pick[6958].Int( 95,159,223,127 );  // rgb6AE5
  pick[6959].Int( 95,191,0,127 );  // rgb6C05
  pick[6960].Int( 95,191,31,127 );  // rgb6C25
  pick[6961].Int( 95,191,63,127 );  // rgb6C45
  pick[6962].Int( 95,191,95,127 );  // rgb6C65
  pick[6963].Int( 95,191,127,127 );  // rgb6C85
  pick[6964].Int( 95,191,159,127 );  // rgb6CA5
  pick[6965].Int( 95,191,191,127 );  // rgb6CC5
  pick[6966].Int( 95,191,223,127 );  // rgb6CE5
  pick[6967].Int( 95,223,0,127 );  // rgb6E05
  pick[6968].Int( 95,223,31,127 );  // rgb6E25
  pick[6969].Int( 95,223,63,127 );  // rgb6E45
  pick[6970].Int( 95,223,95,127 );  // rgb6E65
  pick[6971].Int( 95,223,127,127 );  // rgb6E85
  pick[6972].Int( 95,223,159,127 );  // rgb6EA5
  pick[6973].Int( 95,223,191,127 );  // rgb6EC5
  pick[6974].Int( 95,223,223,127 );  // rgb6EE5
  pick[6975].Int( 127,0,0,127 );  // rgb8005
  pick[6976].Int( 127,0,31,127 );  // rgb8025
  pick[6977].Int( 127,0,63,127 );  // rgb8045
  pick[6978].Int( 127,0,95,127 );  // rgb8065
  pick[6979].Int( 127,0,127,127 );  // rgb8085
  pick[6980].Int( 127,0,159,127 );  // rgb80A5
  pick[6981].Int( 127,0,191,127 );  // rgb80C5
  pick[6982].Int( 127,0,223,127 );  // rgb80E5
  pick[6983].Int( 127,31,0,127 );  // rgb8205
  pick[6984].Int( 127,31,31,127 );  // rgb8225
  pick[6985].Int( 127,31,63,127 );  // rgb8245
  pick[6986].Int( 127,31,95,127 );  // rgb8265
  pick[6987].Int( 127,31,127,127 );  // rgb8285
  pick[6988].Int( 127,31,159,127 );  // rgb82A5
  pick[6989].Int( 127,31,191,127 );  // rgb82C5
  pick[6990].Int( 127,31,223,127 );  // rgb82E5
  pick[6991].Int( 127,63,0,127 );  // rgb8405
  pick[6992].Int( 127,63,31,127 );  // rgb8425
  pick[6993].Int( 127,63,63,127 );  // rgb8445
  pick[6994].Int( 127,63,95,127 );  // rgb8465
  pick[6995].Int( 127,63,127,127 );  // rgb8485
  pick[6996].Int( 127,63,159,127 );  // rgb84A5
  pick[6997].Int( 127,63,191,127 );  // rgb84C5
  pick[6998].Int( 127,63,223,127 );  // rgb84E5
  pick[6999].Int( 127,95,0,127 );  // rgb8605
  pick[7000].Int( 127,95,31,127 );  // rgb8625
  pick[7001].Int( 127,95,63,127 );  // rgb8645
  pick[7002].Int( 127,95,95,127 );  // rgb8665
  pick[7003].Int( 127,95,127,127 );  // rgb8685
  pick[7004].Int( 127,95,159,127 );  // rgb86A5
  pick[7005].Int( 127,95,191,127 );  // rgb86C5
  pick[7006].Int( 127,95,223,127 );  // rgb86E5
  pick[7007].Int( 127,127,0,127 );  // rgb8805
  pick[7008].Int( 127,127,31,127 );  // rgb8825
  pick[7009].Int( 127,127,63,127 );  // rgb8845
  pick[7010].Int( 127,127,95,127 );  // rgb8865
  pick[7011].Int( 127,127,127,127 );  // rgb8885
  pick[7012].Int( 127,127,159,127 );  // rgb88A5
  pick[7013].Int( 127,127,191,127 );  // rgb88C5
  pick[7014].Int( 127,127,223,127 );  // rgb88E5
  pick[7015].Int( 127,159,0,127 );  // rgb8A05
  pick[7016].Int( 127,159,31,127 );  // rgb8A25
  pick[7017].Int( 127,159,63,127 );  // rgb8A45
  pick[7018].Int( 127,159,95,127 );  // rgb8A65
  pick[7019].Int( 127,159,127,127 );  // rgb8A85
  pick[7020].Int( 127,159,159,127 );  // rgb8AA5
  pick[7021].Int( 127,159,191,127 );  // rgb8AC5
  pick[7022].Int( 127,159,223,127 );  // rgb8AE5
  pick[7023].Int( 127,191,0,127 );  // rgb8C05
  pick[7024].Int( 127,191,31,127 );  // rgb8C25
  pick[7025].Int( 127,191,63,127 );  // rgb8C45
  pick[7026].Int( 127,191,95,127 );  // rgb8C65
  pick[7027].Int( 127,191,127,127 );  // rgb8C85
  pick[7028].Int( 127,191,159,127 );  // rgb8CA5
  pick[7029].Int( 127,191,191,127 );  // rgb8CC5
  pick[7030].Int( 127,191,223,127 );  // rgb8CE5
  pick[7031].Int( 127,223,0,127 );  // rgb8E05
  pick[7032].Int( 127,223,31,127 );  // rgb8E25
  pick[7033].Int( 127,223,63,127 );  // rgb8E45
  pick[7034].Int( 127,223,95,127 );  // rgb8E65
  pick[7035].Int( 127,223,127,127 );  // rgb8E85
  pick[7036].Int( 127,223,159,127 );  // rgb8EA5
  pick[7037].Int( 127,223,191,127 );  // rgb8EC5
  pick[7038].Int( 127,223,223,127 );  // rgb8EE5
  pick[7039].Int( 159,0,0,127 );  // rgbA005
  pick[7040].Int( 159,0,31,127 );  // rgbA025
  pick[7041].Int( 159,0,63,127 );  // rgbA045
  pick[7042].Int( 159,0,95,127 );  // rgbA065
  pick[7043].Int( 159,0,127,127 );  // rgbA085
  pick[7044].Int( 159,0,159,127 );  // rgbA0A5
  pick[7045].Int( 159,0,191,127 );  // rgbA0C5
  pick[7046].Int( 159,0,223,127 );  // rgbA0E5
  pick[7047].Int( 159,31,0,127 );  // rgbA205
  pick[7048].Int( 159,31,31,127 );  // rgbA225
  pick[7049].Int( 159,31,63,127 );  // rgbA245
  pick[7050].Int( 159,31,95,127 );  // rgbA265
  pick[7051].Int( 159,31,127,127 );  // rgbA285
  pick[7052].Int( 159,31,159,127 );  // rgbA2A5
  pick[7053].Int( 159,31,191,127 );  // rgbA2C5
  pick[7054].Int( 159,31,223,127 );  // rgbA2E5
  pick[7055].Int( 159,63,0,127 );  // rgbA405
  pick[7056].Int( 159,63,31,127 );  // rgbA425
  pick[7057].Int( 159,63,63,127 );  // rgbA445
  pick[7058].Int( 159,63,95,127 );  // rgbA465
  pick[7059].Int( 159,63,127,127 );  // rgbA485
  pick[7060].Int( 159,63,159,127 );  // rgbA4A5
  pick[7061].Int( 159,63,191,127 );  // rgbA4C5
  pick[7062].Int( 159,63,223,127 );  // rgbA4E5
  pick[7063].Int( 159,95,0,127 );  // rgbA605
  pick[7064].Int( 159,95,31,127 );  // rgbA625
  pick[7065].Int( 159,95,63,127 );  // rgbA645
  pick[7066].Int( 159,95,95,127 );  // rgbA665
  pick[7067].Int( 159,95,127,127 );  // rgbA685
  pick[7068].Int( 159,95,159,127 );  // rgbA6A5
  pick[7069].Int( 159,95,191,127 );  // rgbA6C5
  pick[7070].Int( 159,95,223,127 );  // rgbA6E5
  pick[7071].Int( 159,127,0,127 );  // rgbA805
  pick[7072].Int( 159,127,31,127 );  // rgbA825
  pick[7073].Int( 159,127,63,127 );  // rgbA845
  pick[7074].Int( 159,127,95,127 );  // rgbA865
  pick[7075].Int( 159,127,127,127 );  // rgbA885
  pick[7076].Int( 159,127,159,127 );  // rgbA8A5
  pick[7077].Int( 159,127,191,127 );  // rgbA8C5
  pick[7078].Int( 159,127,223,127 );  // rgbA8E5
  pick[7079].Int( 159,159,0,127 );  // rgbAA05
  pick[7080].Int( 159,159,31,127 );  // rgbAA25
  pick[7081].Int( 159,159,63,127 );  // rgbAA45
  pick[7082].Int( 159,159,95,127 );  // rgbAA65
  pick[7083].Int( 159,159,127,127 );  // rgbAA85
  pick[7084].Int( 159,159,159,127 );  // rgbAAA5
  pick[7085].Int( 159,159,191,127 );  // rgbAAC5
  pick[7086].Int( 159,159,223,127 );  // rgbAAE5
  pick[7087].Int( 159,191,0,127 );  // rgbAC05
  pick[7088].Int( 159,191,31,127 );  // rgbAC25
  pick[7089].Int( 159,191,63,127 );  // rgbAC45
  pick[7090].Int( 159,191,95,127 );  // rgbAC65
  pick[7091].Int( 159,191,127,127 );  // rgbAC85
  pick[7092].Int( 159,191,159,127 );  // rgbACA5
  pick[7093].Int( 159,191,191,127 );  // rgbACC5
  pick[7094].Int( 159,191,223,127 );  // rgbACE5
  pick[7095].Int( 159,223,0,127 );  // rgbAE05
  pick[7096].Int( 159,223,31,127 );  // rgbAE25
  pick[7097].Int( 159,223,63,127 );  // rgbAE45
  pick[7098].Int( 159,223,95,127 );  // rgbAE65
  pick[7099].Int( 159,223,127,127 );  // rgbAE85
  pick[7100].Int( 159,223,159,127 );  // rgbAEA5
  pick[7101].Int( 159,223,191,127 );  // rgbAEC5
  pick[7102].Int( 159,223,223,127 );  // rgbAEE5
  pick[7103].Int( 191,0,0,127 );  // rgbC005
  pick[7104].Int( 191,0,31,127 );  // rgbC025
  pick[7105].Int( 191,0,63,127 );  // rgbC045
  pick[7106].Int( 191,0,95,127 );  // rgbC065
  pick[7107].Int( 191,0,127,127 );  // rgbC085
  pick[7108].Int( 191,0,159,127 );  // rgbC0A5
  pick[7109].Int( 191,0,191,127 );  // rgbC0C5
  pick[7110].Int( 191,0,223,127 );  // rgbC0E5
  pick[7111].Int( 191,31,0,127 );  // rgbC205
  pick[7112].Int( 191,31,31,127 );  // rgbC225
  pick[7113].Int( 191,31,63,127 );  // rgbC245
  pick[7114].Int( 191,31,95,127 );  // rgbC265
  pick[7115].Int( 191,31,127,127 );  // rgbC285
  pick[7116].Int( 191,31,159,127 );  // rgbC2A5
  pick[7117].Int( 191,31,191,127 );  // rgbC2C5
  pick[7118].Int( 191,31,223,127 );  // rgbC2E5
  pick[7119].Int( 191,63,0,127 );  // rgbC405
  pick[7120].Int( 191,63,31,127 );  // rgbC425
  pick[7121].Int( 191,63,63,127 );  // rgbC445
  pick[7122].Int( 191,63,95,127 );  // rgbC465
  pick[7123].Int( 191,63,127,127 );  // rgbC485
  pick[7124].Int( 191,63,159,127 );  // rgbC4A5
  pick[7125].Int( 191,63,191,127 );  // rgbC4C5
  pick[7126].Int( 191,63,223,127 );  // rgbC4E5
  pick[7127].Int( 191,95,0,127 );  // rgbC605
  pick[7128].Int( 191,95,31,127 );  // rgbC625
  pick[7129].Int( 191,95,63,127 );  // rgbC645
  pick[7130].Int( 191,95,95,127 );  // rgbC665
  pick[7131].Int( 191,95,127,127 );  // rgbC685
  pick[7132].Int( 191,95,159,127 );  // rgbC6A5
  pick[7133].Int( 191,95,191,127 );  // rgbC6C5
  pick[7134].Int( 191,95,223,127 );  // rgbC6E5
  pick[7135].Int( 191,127,0,127 );  // rgbC805
  pick[7136].Int( 191,127,31,127 );  // rgbC825
  pick[7137].Int( 191,127,63,127 );  // rgbC845
  pick[7138].Int( 191,127,95,127 );  // rgbC865
  pick[7139].Int( 191,127,127,127 );  // rgbC885
  pick[7140].Int( 191,127,159,127 );  // rgbC8A5
  pick[7141].Int( 191,127,191,127 );  // rgbC8C5
  pick[7142].Int( 191,127,223,127 );  // rgbC8E5
  pick[7143].Int( 191,159,0,127 );  // rgbCA05
  pick[7144].Int( 191,159,31,127 );  // rgbCA25
  pick[7145].Int( 191,159,63,127 );  // rgbCA45
  pick[7146].Int( 191,159,95,127 );  // rgbCA65
  pick[7147].Int( 191,159,127,127 );  // rgbCA85
  pick[7148].Int( 191,159,159,127 );  // rgbCAA5
  pick[7149].Int( 191,159,191,127 );  // rgbCAC5
  pick[7150].Int( 191,159,223,127 );  // rgbCAE5
  pick[7151].Int( 191,191,0,127 );  // rgbCC05
  pick[7152].Int( 191,191,31,127 );  // rgbCC25
  pick[7153].Int( 191,191,63,127 );  // rgbCC45
  pick[7154].Int( 191,191,95,127 );  // rgbCC65
  pick[7155].Int( 191,191,127,127 );  // rgbCC85
  pick[7156].Int( 191,191,159,127 );  // rgbCCA5
  pick[7157].Int( 191,191,191,127 );  // rgbCCC5
  pick[7158].Int( 191,191,223,127 );  // rgbCCE5
  pick[7159].Int( 191,223,0,127 );  // rgbCE05
  pick[7160].Int( 191,223,31,127 );  // rgbCE25
  pick[7161].Int( 191,223,63,127 );  // rgbCE45
  pick[7162].Int( 191,223,95,127 );  // rgbCE65
  pick[7163].Int( 191,223,127,127 );  // rgbCE85
  pick[7164].Int( 191,223,159,127 );  // rgbCEA5
  pick[7165].Int( 191,223,191,127 );  // rgbCEC5
  pick[7166].Int( 191,223,223,127 );  // rgbCEE5
  pick[7167].Int( 223,0,0,127 );  // rgbE005
  pick[7168].Int( 223,0,31,127 );  // rgbE025
  pick[7169].Int( 223,0,63,127 );  // rgbE045
  pick[7170].Int( 223,0,95,127 );  // rgbE065
  pick[7171].Int( 223,0,127,127 );  // rgbE085
  pick[7172].Int( 223,0,159,127 );  // rgbE0A5
  pick[7173].Int( 223,0,191,127 );  // rgbE0C5
  pick[7174].Int( 223,0,223,127 );  // rgbE0E5
  pick[7175].Int( 223,31,0,127 );  // rgbE205
  pick[7176].Int( 223,31,31,127 );  // rgbE225
  pick[7177].Int( 223,31,63,127 );  // rgbE245
  pick[7178].Int( 223,31,95,127 );  // rgbE265
  pick[7179].Int( 223,31,127,127 );  // rgbE285
  pick[7180].Int( 223,31,159,127 );  // rgbE2A5
  pick[7181].Int( 223,31,191,127 );  // rgbE2C5
  pick[7182].Int( 223,31,223,127 );  // rgbE2E5
  pick[7183].Int( 223,63,0,127 );  // rgbE405
  pick[7184].Int( 223,63,31,127 );  // rgbE425
  pick[7185].Int( 223,63,63,127 );  // rgbE445
  pick[7186].Int( 223,63,95,127 );  // rgbE465
  pick[7187].Int( 223,63,127,127 );  // rgbE485
  pick[7188].Int( 223,63,159,127 );  // rgbE4A5
  pick[7189].Int( 223,63,191,127 );  // rgbE4C5
  pick[7190].Int( 223,63,223,127 );  // rgbE4E5
  pick[7191].Int( 223,95,0,127 );  // rgbE605
  pick[7192].Int( 223,95,31,127 );  // rgbE625
  pick[7193].Int( 223,95,63,127 );  // rgbE645
  pick[7194].Int( 223,95,95,127 );  // rgbE665
  pick[7195].Int( 223,95,127,127 );  // rgbE685
  pick[7196].Int( 223,95,159,127 );  // rgbE6A5
  pick[7197].Int( 223,95,191,127 );  // rgbE6C5
  pick[7198].Int( 223,95,223,127 );  // rgbE6E5
  pick[7199].Int( 223,127,0,127 );  // rgbE805
  pick[7200].Int( 223,127,31,127 );  // rgbE825
  pick[7201].Int( 223,127,63,127 );  // rgbE845
  pick[7202].Int( 223,127,95,127 );  // rgbE865
  pick[7203].Int( 223,127,127,127 );  // rgbE885
  pick[7204].Int( 223,127,159,127 );  // rgbE8A5
  pick[7205].Int( 223,127,191,127 );  // rgbE8C5
  pick[7206].Int( 223,127,223,127 );  // rgbE8E5
  pick[7207].Int( 223,159,0,127 );  // rgbEA05
  pick[7208].Int( 223,159,31,127 );  // rgbEA25
  pick[7209].Int( 223,159,63,127 );  // rgbEA45
  pick[7210].Int( 223,159,95,127 );  // rgbEA65
  pick[7211].Int( 223,159,127,127 );  // rgbEA85
  pick[7212].Int( 223,159,159,127 );  // rgbEAA5
  pick[7213].Int( 223,159,191,127 );  // rgbEAC5
  pick[7214].Int( 223,159,223,127 );  // rgbEAE5
  pick[7215].Int( 223,191,0,127 );  // rgbEC05
  pick[7216].Int( 223,191,31,127 );  // rgbEC25
  pick[7217].Int( 223,191,63,127 );  // rgbEC45
  pick[7218].Int( 223,191,95,127 );  // rgbEC65
  pick[7219].Int( 223,191,127,127 );  // rgbEC85
  pick[7220].Int( 223,191,159,127 );  // rgbECA5
  pick[7221].Int( 223,191,191,127 );  // rgbECC5
  pick[7222].Int( 223,191,223,127 );  // rgbECE5
  pick[7223].Int( 223,223,0,127 );  // rgbEE05
  pick[7224].Int( 223,223,31,127 );  // rgbEE25
  pick[7225].Int( 223,223,63,127 );  // rgbEE45
  pick[7226].Int( 223,223,95,127 );  // rgbEE65
  pick[7227].Int( 223,223,127,127 );  // rgbEE85
  pick[7228].Int( 223,223,159,127 );  // rgbEEA5
  pick[7229].Int( 223,223,191,127 );  // rgbEEC5
  pick[7230].Int( 223,223,223,127 );  // rgbEEE5
  pick[7231].Int( 0,0,0,153 );  // rgb0006
  pick[7232].Int( 0,0,31,153 );  // rgb0026
  pick[7233].Int( 0,0,63,153 );  // rgb0046
  pick[7234].Int( 0,0,95,153 );  // rgb0066
  pick[7235].Int( 0,0,127,153 );  // rgb0086
  pick[7236].Int( 0,0,159,153 );  // rgb00A6
  pick[7237].Int( 0,0,191,153 );  // rgb00C6
  pick[7238].Int( 0,0,223,153 );  // rgb00E6
  pick[7239].Int( 0,31,0,153 );  // rgb0206
  pick[7240].Int( 0,31,31,153 );  // rgb0226
  pick[7241].Int( 0,31,63,153 );  // rgb0246
  pick[7242].Int( 0,31,95,153 );  // rgb0266
  pick[7243].Int( 0,31,127,153 );  // rgb0286
  pick[7244].Int( 0,31,159,153 );  // rgb02A6
  pick[7245].Int( 0,31,191,153 );  // rgb02C6
  pick[7246].Int( 0,31,223,153 );  // rgb02E6
  pick[7247].Int( 0,63,0,153 );  // rgb0406
  pick[7248].Int( 0,63,31,153 );  // rgb0426
  pick[7249].Int( 0,63,63,153 );  // rgb0446
  pick[7250].Int( 0,63,95,153 );  // rgb0466
  pick[7251].Int( 0,63,127,153 );  // rgb0486
  pick[7252].Int( 0,63,159,153 );  // rgb04A6
  pick[7253].Int( 0,63,191,153 );  // rgb04C6
  pick[7254].Int( 0,63,223,153 );  // rgb04E6
  pick[7255].Int( 0,95,0,153 );  // rgb0606
  pick[7256].Int( 0,95,31,153 );  // rgb0626
  pick[7257].Int( 0,95,63,153 );  // rgb0646
  pick[7258].Int( 0,95,95,153 );  // rgb0666
  pick[7259].Int( 0,95,127,153 );  // rgb0686
  pick[7260].Int( 0,95,159,153 );  // rgb06A6
  pick[7261].Int( 0,95,191,153 );  // rgb06C6
  pick[7262].Int( 0,95,223,153 );  // rgb06E6
  pick[7263].Int( 0,127,0,153 );  // rgb0806
  pick[7264].Int( 0,127,31,153 );  // rgb0826
  pick[7265].Int( 0,127,63,153 );  // rgb0846
  pick[7266].Int( 0,127,95,153 );  // rgb0866
  pick[7267].Int( 0,127,127,153 );  // rgb0886
  pick[7268].Int( 0,127,159,153 );  // rgb08A6
  pick[7269].Int( 0,127,191,153 );  // rgb08C6
  pick[7270].Int( 0,127,223,153 );  // rgb08E6
  pick[7271].Int( 0,159,0,153 );  // rgb0A06
  pick[7272].Int( 0,159,31,153 );  // rgb0A26
  pick[7273].Int( 0,159,63,153 );  // rgb0A46
  pick[7274].Int( 0,159,95,153 );  // rgb0A66
  pick[7275].Int( 0,159,127,153 );  // rgb0A86
  pick[7276].Int( 0,159,159,153 );  // rgb0AA6
  pick[7277].Int( 0,159,191,153 );  // rgb0AC6
  pick[7278].Int( 0,159,223,153 );  // rgb0AE6
  pick[7279].Int( 0,191,0,153 );  // rgb0C06
  pick[7280].Int( 0,191,31,153 );  // rgb0C26
  pick[7281].Int( 0,191,63,153 );  // rgb0C46
  pick[7282].Int( 0,191,95,153 );  // rgb0C66
  pick[7283].Int( 0,191,127,153 );  // rgb0C86
  pick[7284].Int( 0,191,159,153 );  // rgb0CA6
  pick[7285].Int( 0,191,191,153 );  // rgb0CC6
  pick[7286].Int( 0,191,223,153 );  // rgb0CE6
  pick[7287].Int( 0,223,0,153 );  // rgb0E06
  pick[7288].Int( 0,223,31,153 );  // rgb0E26
  pick[7289].Int( 0,223,63,153 );  // rgb0E46
  pick[7290].Int( 0,223,95,153 );  // rgb0E66
  pick[7291].Int( 0,223,127,153 );  // rgb0E86
  pick[7292].Int( 0,223,159,153 );  // rgb0EA6
  pick[7293].Int( 0,223,191,153 );  // rgb0EC6
  pick[7294].Int( 0,223,223,153 );  // rgb0EE6
  pick[7295].Int( 31,0,0,153 );  // rgb2006
  pick[7296].Int( 31,0,31,153 );  // rgb2026
  pick[7297].Int( 31,0,63,153 );  // rgb2046
  pick[7298].Int( 31,0,95,153 );  // rgb2066
  pick[7299].Int( 31,0,127,153 );  // rgb2086
  pick[7300].Int( 31,0,159,153 );  // rgb20A6
  pick[7301].Int( 31,0,191,153 );  // rgb20C6
  pick[7302].Int( 31,0,223,153 );  // rgb20E6
  pick[7303].Int( 31,31,0,153 );  // rgb2206
  pick[7304].Int( 31,31,31,153 );  // rgb2226
  pick[7305].Int( 31,31,63,153 );  // rgb2246
  pick[7306].Int( 31,31,95,153 );  // rgb2266
  pick[7307].Int( 31,31,127,153 );  // rgb2286
  pick[7308].Int( 31,31,159,153 );  // rgb22A6
  pick[7309].Int( 31,31,191,153 );  // rgb22C6
  pick[7310].Int( 31,31,223,153 );  // rgb22E6
  pick[7311].Int( 31,63,0,153 );  // rgb2406
  pick[7312].Int( 31,63,31,153 );  // rgb2426
  pick[7313].Int( 31,63,63,153 );  // rgb2446
  pick[7314].Int( 31,63,95,153 );  // rgb2466
  pick[7315].Int( 31,63,127,153 );  // rgb2486
  pick[7316].Int( 31,63,159,153 );  // rgb24A6
  pick[7317].Int( 31,63,191,153 );  // rgb24C6
  pick[7318].Int( 31,63,223,153 );  // rgb24E6
  pick[7319].Int( 31,95,0,153 );  // rgb2606
  pick[7320].Int( 31,95,31,153 );  // rgb2626
  pick[7321].Int( 31,95,63,153 );  // rgb2646
  pick[7322].Int( 31,95,95,153 );  // rgb2666
  pick[7323].Int( 31,95,127,153 );  // rgb2686
  pick[7324].Int( 31,95,159,153 );  // rgb26A6
  pick[7325].Int( 31,95,191,153 );  // rgb26C6
  pick[7326].Int( 31,95,223,153 );  // rgb26E6
  pick[7327].Int( 31,127,0,153 );  // rgb2806
  pick[7328].Int( 31,127,31,153 );  // rgb2826
  pick[7329].Int( 31,127,63,153 );  // rgb2846
  pick[7330].Int( 31,127,95,153 );  // rgb2866
  pick[7331].Int( 31,127,127,153 );  // rgb2886
  pick[7332].Int( 31,127,159,153 );  // rgb28A6
  pick[7333].Int( 31,127,191,153 );  // rgb28C6
  pick[7334].Int( 31,127,223,153 );  // rgb28E6
  pick[7335].Int( 31,159,0,153 );  // rgb2A06
  pick[7336].Int( 31,159,31,153 );  // rgb2A26
  pick[7337].Int( 31,159,63,153 );  // rgb2A46
  pick[7338].Int( 31,159,95,153 );  // rgb2A66
  pick[7339].Int( 31,159,127,153 );  // rgb2A86
  pick[7340].Int( 31,159,159,153 );  // rgb2AA6
  pick[7341].Int( 31,159,191,153 );  // rgb2AC6
  pick[7342].Int( 31,159,223,153 );  // rgb2AE6
  pick[7343].Int( 31,191,0,153 );  // rgb2C06
  pick[7344].Int( 31,191,31,153 );  // rgb2C26
  pick[7345].Int( 31,191,63,153 );  // rgb2C46
  pick[7346].Int( 31,191,95,153 );  // rgb2C66
  pick[7347].Int( 31,191,127,153 );  // rgb2C86
  pick[7348].Int( 31,191,159,153 );  // rgb2CA6
  pick[7349].Int( 31,191,191,153 );  // rgb2CC6
  pick[7350].Int( 31,191,223,153 );  // rgb2CE6
  pick[7351].Int( 31,223,0,153 );  // rgb2E06
  pick[7352].Int( 31,223,31,153 );  // rgb2E26
  pick[7353].Int( 31,223,63,153 );  // rgb2E46
  pick[7354].Int( 31,223,95,153 );  // rgb2E66
  pick[7355].Int( 31,223,127,153 );  // rgb2E86
  pick[7356].Int( 31,223,159,153 );  // rgb2EA6
  pick[7357].Int( 31,223,191,153 );  // rgb2EC6
  pick[7358].Int( 31,223,223,153 );  // rgb2EE6
  pick[7359].Int( 63,0,0,153 );  // rgb4006
  pick[7360].Int( 63,0,31,153 );  // rgb4026
  pick[7361].Int( 63,0,63,153 );  // rgb4046
  pick[7362].Int( 63,0,95,153 );  // rgb4066
  pick[7363].Int( 63,0,127,153 );  // rgb4086
  pick[7364].Int( 63,0,159,153 );  // rgb40A6
  pick[7365].Int( 63,0,191,153 );  // rgb40C6
  pick[7366].Int( 63,0,223,153 );  // rgb40E6
  pick[7367].Int( 63,31,0,153 );  // rgb4206
  pick[7368].Int( 63,31,31,153 );  // rgb4226
  pick[7369].Int( 63,31,63,153 );  // rgb4246
  pick[7370].Int( 63,31,95,153 );  // rgb4266
  pick[7371].Int( 63,31,127,153 );  // rgb4286
  pick[7372].Int( 63,31,159,153 );  // rgb42A6
  pick[7373].Int( 63,31,191,153 );  // rgb42C6
  pick[7374].Int( 63,31,223,153 );  // rgb42E6
  pick[7375].Int( 63,63,0,153 );  // rgb4406
  pick[7376].Int( 63,63,31,153 );  // rgb4426
  pick[7377].Int( 63,63,63,153 );  // rgb4446
  pick[7378].Int( 63,63,95,153 );  // rgb4466
  pick[7379].Int( 63,63,127,153 );  // rgb4486
  pick[7380].Int( 63,63,159,153 );  // rgb44A6
  pick[7381].Int( 63,63,191,153 );  // rgb44C6
  pick[7382].Int( 63,63,223,153 );  // rgb44E6
  pick[7383].Int( 63,95,0,153 );  // rgb4606
  pick[7384].Int( 63,95,31,153 );  // rgb4626
  pick[7385].Int( 63,95,63,153 );  // rgb4646
  pick[7386].Int( 63,95,95,153 );  // rgb4666
  pick[7387].Int( 63,95,127,153 );  // rgb4686
  pick[7388].Int( 63,95,159,153 );  // rgb46A6
  pick[7389].Int( 63,95,191,153 );  // rgb46C6
  pick[7390].Int( 63,95,223,153 );  // rgb46E6
  pick[7391].Int( 63,127,0,153 );  // rgb4806
  pick[7392].Int( 63,127,31,153 );  // rgb4826
  pick[7393].Int( 63,127,63,153 );  // rgb4846
  pick[7394].Int( 63,127,95,153 );  // rgb4866
  pick[7395].Int( 63,127,127,153 );  // rgb4886
  pick[7396].Int( 63,127,159,153 );  // rgb48A6
  pick[7397].Int( 63,127,191,153 );  // rgb48C6
  pick[7398].Int( 63,127,223,153 );  // rgb48E6
  pick[7399].Int( 63,159,0,153 );  // rgb4A06
  pick[7400].Int( 63,159,31,153 );  // rgb4A26
  pick[7401].Int( 63,159,63,153 );  // rgb4A46
  pick[7402].Int( 63,159,95,153 );  // rgb4A66
  pick[7403].Int( 63,159,127,153 );  // rgb4A86
  pick[7404].Int( 63,159,159,153 );  // rgb4AA6
  pick[7405].Int( 63,159,191,153 );  // rgb4AC6
  pick[7406].Int( 63,159,223,153 );  // rgb4AE6
  pick[7407].Int( 63,191,0,153 );  // rgb4C06
  pick[7408].Int( 63,191,31,153 );  // rgb4C26
  pick[7409].Int( 63,191,63,153 );  // rgb4C46
  pick[7410].Int( 63,191,95,153 );  // rgb4C66
  pick[7411].Int( 63,191,127,153 );  // rgb4C86
  pick[7412].Int( 63,191,159,153 );  // rgb4CA6
  pick[7413].Int( 63,191,191,153 );  // rgb4CC6
  pick[7414].Int( 63,191,223,153 );  // rgb4CE6
  pick[7415].Int( 63,223,0,153 );  // rgb4E06
  pick[7416].Int( 63,223,31,153 );  // rgb4E26
  pick[7417].Int( 63,223,63,153 );  // rgb4E46
  pick[7418].Int( 63,223,95,153 );  // rgb4E66
  pick[7419].Int( 63,223,127,153 );  // rgb4E86
  pick[7420].Int( 63,223,159,153 );  // rgb4EA6
  pick[7421].Int( 63,223,191,153 );  // rgb4EC6
  pick[7422].Int( 63,223,223,153 );  // rgb4EE6
  pick[7423].Int( 95,0,0,153 );  // rgb6006
  pick[7424].Int( 95,0,31,153 );  // rgb6026
  pick[7425].Int( 95,0,63,153 );  // rgb6046
  pick[7426].Int( 95,0,95,153 );  // rgb6066
  pick[7427].Int( 95,0,127,153 );  // rgb6086
  pick[7428].Int( 95,0,159,153 );  // rgb60A6
  pick[7429].Int( 95,0,191,153 );  // rgb60C6
  pick[7430].Int( 95,0,223,153 );  // rgb60E6
  pick[7431].Int( 95,31,0,153 );  // rgb6206
  pick[7432].Int( 95,31,31,153 );  // rgb6226
  pick[7433].Int( 95,31,63,153 );  // rgb6246
  pick[7434].Int( 95,31,95,153 );  // rgb6266
  pick[7435].Int( 95,31,127,153 );  // rgb6286
  pick[7436].Int( 95,31,159,153 );  // rgb62A6
  pick[7437].Int( 95,31,191,153 );  // rgb62C6
  pick[7438].Int( 95,31,223,153 );  // rgb62E6
  pick[7439].Int( 95,63,0,153 );  // rgb6406
  pick[7440].Int( 95,63,31,153 );  // rgb6426
  pick[7441].Int( 95,63,63,153 );  // rgb6446
  pick[7442].Int( 95,63,95,153 );  // rgb6466
  pick[7443].Int( 95,63,127,153 );  // rgb6486
  pick[7444].Int( 95,63,159,153 );  // rgb64A6
  pick[7445].Int( 95,63,191,153 );  // rgb64C6
  pick[7446].Int( 95,63,223,153 );  // rgb64E6
  pick[7447].Int( 95,95,0,153 );  // rgb6606
  pick[7448].Int( 95,95,31,153 );  // rgb6626
  pick[7449].Int( 95,95,63,153 );  // rgb6646
  pick[7450].Int( 95,95,95,153 );  // rgb6666
  pick[7451].Int( 95,95,127,153 );  // rgb6686
  pick[7452].Int( 95,95,159,153 );  // rgb66A6
  pick[7453].Int( 95,95,191,153 );  // rgb66C6
  pick[7454].Int( 95,95,223,153 );  // rgb66E6
  pick[7455].Int( 95,127,0,153 );  // rgb6806
  pick[7456].Int( 95,127,31,153 );  // rgb6826
  pick[7457].Int( 95,127,63,153 );  // rgb6846
  pick[7458].Int( 95,127,95,153 );  // rgb6866
  pick[7459].Int( 95,127,127,153 );  // rgb6886
  pick[7460].Int( 95,127,159,153 );  // rgb68A6
  pick[7461].Int( 95,127,191,153 );  // rgb68C6
  pick[7462].Int( 95,127,223,153 );  // rgb68E6
  pick[7463].Int( 95,159,0,153 );  // rgb6A06
  pick[7464].Int( 95,159,31,153 );  // rgb6A26
  pick[7465].Int( 95,159,63,153 );  // rgb6A46
  pick[7466].Int( 95,159,95,153 );  // rgb6A66
  pick[7467].Int( 95,159,127,153 );  // rgb6A86
  pick[7468].Int( 95,159,159,153 );  // rgb6AA6
  pick[7469].Int( 95,159,191,153 );  // rgb6AC6
  pick[7470].Int( 95,159,223,153 );  // rgb6AE6
  pick[7471].Int( 95,191,0,153 );  // rgb6C06
  pick[7472].Int( 95,191,31,153 );  // rgb6C26
  pick[7473].Int( 95,191,63,153 );  // rgb6C46
  pick[7474].Int( 95,191,95,153 );  // rgb6C66
  pick[7475].Int( 95,191,127,153 );  // rgb6C86
  pick[7476].Int( 95,191,159,153 );  // rgb6CA6
  pick[7477].Int( 95,191,191,153 );  // rgb6CC6
  pick[7478].Int( 95,191,223,153 );  // rgb6CE6
  pick[7479].Int( 95,223,0,153 );  // rgb6E06
  pick[7480].Int( 95,223,31,153 );  // rgb6E26
  pick[7481].Int( 95,223,63,153 );  // rgb6E46
  pick[7482].Int( 95,223,95,153 );  // rgb6E66
  pick[7483].Int( 95,223,127,153 );  // rgb6E86
  pick[7484].Int( 95,223,159,153 );  // rgb6EA6
  pick[7485].Int( 95,223,191,153 );  // rgb6EC6
  pick[7486].Int( 95,223,223,153 );  // rgb6EE6
  pick[7487].Int( 127,0,0,153 );  // rgb8006
  pick[7488].Int( 127,0,31,153 );  // rgb8026
  pick[7489].Int( 127,0,63,153 );  // rgb8046
  pick[7490].Int( 127,0,95,153 );  // rgb8066
  pick[7491].Int( 127,0,127,153 );  // rgb8086
  pick[7492].Int( 127,0,159,153 );  // rgb80A6
  pick[7493].Int( 127,0,191,153 );  // rgb80C6
  pick[7494].Int( 127,0,223,153 );  // rgb80E6
  pick[7495].Int( 127,31,0,153 );  // rgb8206
  pick[7496].Int( 127,31,31,153 );  // rgb8226
  pick[7497].Int( 127,31,63,153 );  // rgb8246
  pick[7498].Int( 127,31,95,153 );  // rgb8266
  pick[7499].Int( 127,31,127,153 );  // rgb8286
  pick[7500].Int( 127,31,159,153 );  // rgb82A6
  pick[7501].Int( 127,31,191,153 );  // rgb82C6
  pick[7502].Int( 127,31,223,153 );  // rgb82E6
  pick[7503].Int( 127,63,0,153 );  // rgb8406
  pick[7504].Int( 127,63,31,153 );  // rgb8426
  pick[7505].Int( 127,63,63,153 );  // rgb8446
  pick[7506].Int( 127,63,95,153 );  // rgb8466
  pick[7507].Int( 127,63,127,153 );  // rgb8486
  pick[7508].Int( 127,63,159,153 );  // rgb84A6
  pick[7509].Int( 127,63,191,153 );  // rgb84C6
  pick[7510].Int( 127,63,223,153 );  // rgb84E6
  pick[7511].Int( 127,95,0,153 );  // rgb8606
  pick[7512].Int( 127,95,31,153 );  // rgb8626
  pick[7513].Int( 127,95,63,153 );  // rgb8646
  pick[7514].Int( 127,95,95,153 );  // rgb8666
  pick[7515].Int( 127,95,127,153 );  // rgb8686
  pick[7516].Int( 127,95,159,153 );  // rgb86A6
  pick[7517].Int( 127,95,191,153 );  // rgb86C6
  pick[7518].Int( 127,95,223,153 );  // rgb86E6
  pick[7519].Int( 127,127,0,153 );  // rgb8806
  pick[7520].Int( 127,127,31,153 );  // rgb8826
  pick[7521].Int( 127,127,63,153 );  // rgb8846
  pick[7522].Int( 127,127,95,153 );  // rgb8866
  pick[7523].Int( 127,127,127,153 );  // rgb8886
  pick[7524].Int( 127,127,159,153 );  // rgb88A6
  pick[7525].Int( 127,127,191,153 );  // rgb88C6
  pick[7526].Int( 127,127,223,153 );  // rgb88E6
  pick[7527].Int( 127,159,0,153 );  // rgb8A06
  pick[7528].Int( 127,159,31,153 );  // rgb8A26
  pick[7529].Int( 127,159,63,153 );  // rgb8A46
  pick[7530].Int( 127,159,95,153 );  // rgb8A66
  pick[7531].Int( 127,159,127,153 );  // rgb8A86
  pick[7532].Int( 127,159,159,153 );  // rgb8AA6
  pick[7533].Int( 127,159,191,153 );  // rgb8AC6
  pick[7534].Int( 127,159,223,153 );  // rgb8AE6
  pick[7535].Int( 127,191,0,153 );  // rgb8C06
  pick[7536].Int( 127,191,31,153 );  // rgb8C26
  pick[7537].Int( 127,191,63,153 );  // rgb8C46
  pick[7538].Int( 127,191,95,153 );  // rgb8C66
  pick[7539].Int( 127,191,127,153 );  // rgb8C86
  pick[7540].Int( 127,191,159,153 );  // rgb8CA6
  pick[7541].Int( 127,191,191,153 );  // rgb8CC6
  pick[7542].Int( 127,191,223,153 );  // rgb8CE6
  pick[7543].Int( 127,223,0,153 );  // rgb8E06
  pick[7544].Int( 127,223,31,153 );  // rgb8E26
  pick[7545].Int( 127,223,63,153 );  // rgb8E46
  pick[7546].Int( 127,223,95,153 );  // rgb8E66
  pick[7547].Int( 127,223,127,153 );  // rgb8E86
  pick[7548].Int( 127,223,159,153 );  // rgb8EA6
  pick[7549].Int( 127,223,191,153 );  // rgb8EC6
  pick[7550].Int( 127,223,223,153 );  // rgb8EE6
  pick[7551].Int( 159,0,0,153 );  // rgbA006
  pick[7552].Int( 159,0,31,153 );  // rgbA026
  pick[7553].Int( 159,0,63,153 );  // rgbA046
  pick[7554].Int( 159,0,95,153 );  // rgbA066
  pick[7555].Int( 159,0,127,153 );  // rgbA086
  pick[7556].Int( 159,0,159,153 );  // rgbA0A6
  pick[7557].Int( 159,0,191,153 );  // rgbA0C6
  pick[7558].Int( 159,0,223,153 );  // rgbA0E6
  pick[7559].Int( 159,31,0,153 );  // rgbA206
  pick[7560].Int( 159,31,31,153 );  // rgbA226
  pick[7561].Int( 159,31,63,153 );  // rgbA246
  pick[7562].Int( 159,31,95,153 );  // rgbA266
  pick[7563].Int( 159,31,127,153 );  // rgbA286
  pick[7564].Int( 159,31,159,153 );  // rgbA2A6
  pick[7565].Int( 159,31,191,153 );  // rgbA2C6
  pick[7566].Int( 159,31,223,153 );  // rgbA2E6
  pick[7567].Int( 159,63,0,153 );  // rgbA406
  pick[7568].Int( 159,63,31,153 );  // rgbA426
  pick[7569].Int( 159,63,63,153 );  // rgbA446
  pick[7570].Int( 159,63,95,153 );  // rgbA466
  pick[7571].Int( 159,63,127,153 );  // rgbA486
  pick[7572].Int( 159,63,159,153 );  // rgbA4A6
  pick[7573].Int( 159,63,191,153 );  // rgbA4C6
  pick[7574].Int( 159,63,223,153 );  // rgbA4E6
  pick[7575].Int( 159,95,0,153 );  // rgbA606
  pick[7576].Int( 159,95,31,153 );  // rgbA626
  pick[7577].Int( 159,95,63,153 );  // rgbA646
  pick[7578].Int( 159,95,95,153 );  // rgbA666
  pick[7579].Int( 159,95,127,153 );  // rgbA686
  pick[7580].Int( 159,95,159,153 );  // rgbA6A6
  pick[7581].Int( 159,95,191,153 );  // rgbA6C6
  pick[7582].Int( 159,95,223,153 );  // rgbA6E6
  pick[7583].Int( 159,127,0,153 );  // rgbA806
  pick[7584].Int( 159,127,31,153 );  // rgbA826
  pick[7585].Int( 159,127,63,153 );  // rgbA846
  pick[7586].Int( 159,127,95,153 );  // rgbA866
  pick[7587].Int( 159,127,127,153 );  // rgbA886
  pick[7588].Int( 159,127,159,153 );  // rgbA8A6
  pick[7589].Int( 159,127,191,153 );  // rgbA8C6
  pick[7590].Int( 159,127,223,153 );  // rgbA8E6
  pick[7591].Int( 159,159,0,153 );  // rgbAA06
  pick[7592].Int( 159,159,31,153 );  // rgbAA26
  pick[7593].Int( 159,159,63,153 );  // rgbAA46
  pick[7594].Int( 159,159,95,153 );  // rgbAA66
  pick[7595].Int( 159,159,127,153 );  // rgbAA86
  pick[7596].Int( 159,159,159,153 );  // rgbAAA6
  pick[7597].Int( 159,159,191,153 );  // rgbAAC6
  pick[7598].Int( 159,159,223,153 );  // rgbAAE6
  pick[7599].Int( 159,191,0,153 );  // rgbAC06
  pick[7600].Int( 159,191,31,153 );  // rgbAC26
  pick[7601].Int( 159,191,63,153 );  // rgbAC46
  pick[7602].Int( 159,191,95,153 );  // rgbAC66
  pick[7603].Int( 159,191,127,153 );  // rgbAC86
  pick[7604].Int( 159,191,159,153 );  // rgbACA6
  pick[7605].Int( 159,191,191,153 );  // rgbACC6
  pick[7606].Int( 159,191,223,153 );  // rgbACE6
  pick[7607].Int( 159,223,0,153 );  // rgbAE06
  pick[7608].Int( 159,223,31,153 );  // rgbAE26
  pick[7609].Int( 159,223,63,153 );  // rgbAE46
  pick[7610].Int( 159,223,95,153 );  // rgbAE66
  pick[7611].Int( 159,223,127,153 );  // rgbAE86
  pick[7612].Int( 159,223,159,153 );  // rgbAEA6
  pick[7613].Int( 159,223,191,153 );  // rgbAEC6
  pick[7614].Int( 159,223,223,153 );  // rgbAEE6
  pick[7615].Int( 191,0,0,153 );  // rgbC006
  pick[7616].Int( 191,0,31,153 );  // rgbC026
  pick[7617].Int( 191,0,63,153 );  // rgbC046
  pick[7618].Int( 191,0,95,153 );  // rgbC066
  pick[7619].Int( 191,0,127,153 );  // rgbC086
  pick[7620].Int( 191,0,159,153 );  // rgbC0A6
  pick[7621].Int( 191,0,191,153 );  // rgbC0C6
  pick[7622].Int( 191,0,223,153 );  // rgbC0E6
  pick[7623].Int( 191,31,0,153 );  // rgbC206
  pick[7624].Int( 191,31,31,153 );  // rgbC226
  pick[7625].Int( 191,31,63,153 );  // rgbC246
  pick[7626].Int( 191,31,95,153 );  // rgbC266
  pick[7627].Int( 191,31,127,153 );  // rgbC286
  pick[7628].Int( 191,31,159,153 );  // rgbC2A6
  pick[7629].Int( 191,31,191,153 );  // rgbC2C6
  pick[7630].Int( 191,31,223,153 );  // rgbC2E6
  pick[7631].Int( 191,63,0,153 );  // rgbC406
  pick[7632].Int( 191,63,31,153 );  // rgbC426
  pick[7633].Int( 191,63,63,153 );  // rgbC446
  pick[7634].Int( 191,63,95,153 );  // rgbC466
  pick[7635].Int( 191,63,127,153 );  // rgbC486
  pick[7636].Int( 191,63,159,153 );  // rgbC4A6
  pick[7637].Int( 191,63,191,153 );  // rgbC4C6
  pick[7638].Int( 191,63,223,153 );  // rgbC4E6
  pick[7639].Int( 191,95,0,153 );  // rgbC606
  pick[7640].Int( 191,95,31,153 );  // rgbC626
  pick[7641].Int( 191,95,63,153 );  // rgbC646
  pick[7642].Int( 191,95,95,153 );  // rgbC666
  pick[7643].Int( 191,95,127,153 );  // rgbC686
  pick[7644].Int( 191,95,159,153 );  // rgbC6A6
  pick[7645].Int( 191,95,191,153 );  // rgbC6C6
  pick[7646].Int( 191,95,223,153 );  // rgbC6E6
  pick[7647].Int( 191,127,0,153 );  // rgbC806
  pick[7648].Int( 191,127,31,153 );  // rgbC826
  pick[7649].Int( 191,127,63,153 );  // rgbC846
  pick[7650].Int( 191,127,95,153 );  // rgbC866
  pick[7651].Int( 191,127,127,153 );  // rgbC886
  pick[7652].Int( 191,127,159,153 );  // rgbC8A6
  pick[7653].Int( 191,127,191,153 );  // rgbC8C6
  pick[7654].Int( 191,127,223,153 );  // rgbC8E6
  pick[7655].Int( 191,159,0,153 );  // rgbCA06
  pick[7656].Int( 191,159,31,153 );  // rgbCA26
  pick[7657].Int( 191,159,63,153 );  // rgbCA46
  pick[7658].Int( 191,159,95,153 );  // rgbCA66
  pick[7659].Int( 191,159,127,153 );  // rgbCA86
  pick[7660].Int( 191,159,159,153 );  // rgbCAA6
  pick[7661].Int( 191,159,191,153 );  // rgbCAC6
  pick[7662].Int( 191,159,223,153 );  // rgbCAE6
  pick[7663].Int( 191,191,0,153 );  // rgbCC06
  pick[7664].Int( 191,191,31,153 );  // rgbCC26
  pick[7665].Int( 191,191,63,153 );  // rgbCC46
  pick[7666].Int( 191,191,95,153 );  // rgbCC66
  pick[7667].Int( 191,191,127,153 );  // rgbCC86
  pick[7668].Int( 191,191,159,153 );  // rgbCCA6
  pick[7669].Int( 191,191,191,153 );  // rgbCCC6
  pick[7670].Int( 191,191,223,153 );  // rgbCCE6
  pick[7671].Int( 191,223,0,153 );  // rgbCE06
  pick[7672].Int( 191,223,31,153 );  // rgbCE26
  pick[7673].Int( 191,223,63,153 );  // rgbCE46
  pick[7674].Int( 191,223,95,153 );  // rgbCE66
  pick[7675].Int( 191,223,127,153 );  // rgbCE86
  pick[7676].Int( 191,223,159,153 );  // rgbCEA6
  pick[7677].Int( 191,223,191,153 );  // rgbCEC6
  pick[7678].Int( 191,223,223,153 );  // rgbCEE6
  pick[7679].Int( 223,0,0,153 );  // rgbE006
  pick[7680].Int( 223,0,31,153 );  // rgbE026
  pick[7681].Int( 223,0,63,153 );  // rgbE046
  pick[7682].Int( 223,0,95,153 );  // rgbE066
  pick[7683].Int( 223,0,127,153 );  // rgbE086
  pick[7684].Int( 223,0,159,153 );  // rgbE0A6
  pick[7685].Int( 223,0,191,153 );  // rgbE0C6
  pick[7686].Int( 223,0,223,153 );  // rgbE0E6
  pick[7687].Int( 223,31,0,153 );  // rgbE206
  pick[7688].Int( 223,31,31,153 );  // rgbE226
  pick[7689].Int( 223,31,63,153 );  // rgbE246
  pick[7690].Int( 223,31,95,153 );  // rgbE266
  pick[7691].Int( 223,31,127,153 );  // rgbE286
  pick[7692].Int( 223,31,159,153 );  // rgbE2A6
  pick[7693].Int( 223,31,191,153 );  // rgbE2C6
  pick[7694].Int( 223,31,223,153 );  // rgbE2E6
  pick[7695].Int( 223,63,0,153 );  // rgbE406
  pick[7696].Int( 223,63,31,153 );  // rgbE426
  pick[7697].Int( 223,63,63,153 );  // rgbE446
  pick[7698].Int( 223,63,95,153 );  // rgbE466
  pick[7699].Int( 223,63,127,153 );  // rgbE486
  pick[7700].Int( 223,63,159,153 );  // rgbE4A6
  pick[7701].Int( 223,63,191,153 );  // rgbE4C6
  pick[7702].Int( 223,63,223,153 );  // rgbE4E6
  pick[7703].Int( 223,95,0,153 );  // rgbE606
  pick[7704].Int( 223,95,31,153 );  // rgbE626
  pick[7705].Int( 223,95,63,153 );  // rgbE646
  pick[7706].Int( 223,95,95,153 );  // rgbE666
  pick[7707].Int( 223,95,127,153 );  // rgbE686
  pick[7708].Int( 223,95,159,153 );  // rgbE6A6
  pick[7709].Int( 223,95,191,153 );  // rgbE6C6
  pick[7710].Int( 223,95,223,153 );  // rgbE6E6
  pick[7711].Int( 223,127,0,153 );  // rgbE806
  pick[7712].Int( 223,127,31,153 );  // rgbE826
  pick[7713].Int( 223,127,63,153 );  // rgbE846
  pick[7714].Int( 223,127,95,153 );  // rgbE866
  pick[7715].Int( 223,127,127,153 );  // rgbE886
  pick[7716].Int( 223,127,159,153 );  // rgbE8A6
  pick[7717].Int( 223,127,191,153 );  // rgbE8C6
  pick[7718].Int( 223,127,223,153 );  // rgbE8E6
  pick[7719].Int( 223,159,0,153 );  // rgbEA06
  pick[7720].Int( 223,159,31,153 );  // rgbEA26
  pick[7721].Int( 223,159,63,153 );  // rgbEA46
  pick[7722].Int( 223,159,95,153 );  // rgbEA66
  pick[7723].Int( 223,159,127,153 );  // rgbEA86
  pick[7724].Int( 223,159,159,153 );  // rgbEAA6
  pick[7725].Int( 223,159,191,153 );  // rgbEAC6
  pick[7726].Int( 223,159,223,153 );  // rgbEAE6
  pick[7727].Int( 223,191,0,153 );  // rgbEC06
  pick[7728].Int( 223,191,31,153 );  // rgbEC26
  pick[7729].Int( 223,191,63,153 );  // rgbEC46
  pick[7730].Int( 223,191,95,153 );  // rgbEC66
  pick[7731].Int( 223,191,127,153 );  // rgbEC86
  pick[7732].Int( 223,191,159,153 );  // rgbECA6
  pick[7733].Int( 223,191,191,153 );  // rgbECC6
  pick[7734].Int( 223,191,223,153 );  // rgbECE6
  pick[7735].Int( 223,223,0,153 );  // rgbEE06
  pick[7736].Int( 223,223,31,153 );  // rgbEE26
  pick[7737].Int( 223,223,63,153 );  // rgbEE46
  pick[7738].Int( 223,223,95,153 );  // rgbEE66
  pick[7739].Int( 223,223,127,153 );  // rgbEE86
  pick[7740].Int( 223,223,159,153 );  // rgbEEA6
  pick[7741].Int( 223,223,191,153 );  // rgbEEC6
  pick[7742].Int( 223,223,223,153 );  // rgbEEE6
  pick[7743].Int( 0,0,0,178 );  // rgb0007
  pick[7744].Int( 0,0,31,178 );  // rgb0027
  pick[7745].Int( 0,0,63,178 );  // rgb0047
  pick[7746].Int( 0,0,95,178 );  // rgb0067
  pick[7747].Int( 0,0,127,178 );  // rgb0087
  pick[7748].Int( 0,0,159,178 );  // rgb00A7
  pick[7749].Int( 0,0,191,178 );  // rgb00C7
  pick[7750].Int( 0,0,223,178 );  // rgb00E7
  pick[7751].Int( 0,31,0,178 );  // rgb0207
  pick[7752].Int( 0,31,31,178 );  // rgb0227
  pick[7753].Int( 0,31,63,178 );  // rgb0247
  pick[7754].Int( 0,31,95,178 );  // rgb0267
  pick[7755].Int( 0,31,127,178 );  // rgb0287
  pick[7756].Int( 0,31,159,178 );  // rgb02A7
  pick[7757].Int( 0,31,191,178 );  // rgb02C7
  pick[7758].Int( 0,31,223,178 );  // rgb02E7
  pick[7759].Int( 0,63,0,178 );  // rgb0407
  pick[7760].Int( 0,63,31,178 );  // rgb0427
  pick[7761].Int( 0,63,63,178 );  // rgb0447
  pick[7762].Int( 0,63,95,178 );  // rgb0467
  pick[7763].Int( 0,63,127,178 );  // rgb0487
  pick[7764].Int( 0,63,159,178 );  // rgb04A7
  pick[7765].Int( 0,63,191,178 );  // rgb04C7
  pick[7766].Int( 0,63,223,178 );  // rgb04E7
  pick[7767].Int( 0,95,0,178 );  // rgb0607
  pick[7768].Int( 0,95,31,178 );  // rgb0627
  pick[7769].Int( 0,95,63,178 );  // rgb0647
  pick[7770].Int( 0,95,95,178 );  // rgb0667
  pick[7771].Int( 0,95,127,178 );  // rgb0687
  pick[7772].Int( 0,95,159,178 );  // rgb06A7
  pick[7773].Int( 0,95,191,178 );  // rgb06C7
  pick[7774].Int( 0,95,223,178 );  // rgb06E7
  pick[7775].Int( 0,127,0,178 );  // rgb0807
  pick[7776].Int( 0,127,31,178 );  // rgb0827
  pick[7777].Int( 0,127,63,178 );  // rgb0847
  pick[7778].Int( 0,127,95,178 );  // rgb0867
  pick[7779].Int( 0,127,127,178 );  // rgb0887
  pick[7780].Int( 0,127,159,178 );  // rgb08A7
  pick[7781].Int( 0,127,191,178 );  // rgb08C7
  pick[7782].Int( 0,127,223,178 );  // rgb08E7
  pick[7783].Int( 0,159,0,178 );  // rgb0A07
  pick[7784].Int( 0,159,31,178 );  // rgb0A27
  pick[7785].Int( 0,159,63,178 );  // rgb0A47
  pick[7786].Int( 0,159,95,178 );  // rgb0A67
  pick[7787].Int( 0,159,127,178 );  // rgb0A87
  pick[7788].Int( 0,159,159,178 );  // rgb0AA7
  pick[7789].Int( 0,159,191,178 );  // rgb0AC7
  pick[7790].Int( 0,159,223,178 );  // rgb0AE7
  pick[7791].Int( 0,191,0,178 );  // rgb0C07
  pick[7792].Int( 0,191,31,178 );  // rgb0C27
  pick[7793].Int( 0,191,63,178 );  // rgb0C47
  pick[7794].Int( 0,191,95,178 );  // rgb0C67
  pick[7795].Int( 0,191,127,178 );  // rgb0C87
  pick[7796].Int( 0,191,159,178 );  // rgb0CA7
  pick[7797].Int( 0,191,191,178 );  // rgb0CC7
  pick[7798].Int( 0,191,223,178 );  // rgb0CE7
  pick[7799].Int( 0,223,0,178 );  // rgb0E07
  pick[7800].Int( 0,223,31,178 );  // rgb0E27
  pick[7801].Int( 0,223,63,178 );  // rgb0E47
  pick[7802].Int( 0,223,95,178 );  // rgb0E67
  pick[7803].Int( 0,223,127,178 );  // rgb0E87
  pick[7804].Int( 0,223,159,178 );  // rgb0EA7
  pick[7805].Int( 0,223,191,178 );  // rgb0EC7
  pick[7806].Int( 0,223,223,178 );  // rgb0EE7
  pick[7807].Int( 31,0,0,178 );  // rgb2007
  pick[7808].Int( 31,0,31,178 );  // rgb2027
  pick[7809].Int( 31,0,63,178 );  // rgb2047
  pick[7810].Int( 31,0,95,178 );  // rgb2067
  pick[7811].Int( 31,0,127,178 );  // rgb2087
  pick[7812].Int( 31,0,159,178 );  // rgb20A7
  pick[7813].Int( 31,0,191,178 );  // rgb20C7
  pick[7814].Int( 31,0,223,178 );  // rgb20E7
  pick[7815].Int( 31,31,0,178 );  // rgb2207
  pick[7816].Int( 31,31,31,178 );  // rgb2227
  pick[7817].Int( 31,31,63,178 );  // rgb2247
  pick[7818].Int( 31,31,95,178 );  // rgb2267
  pick[7819].Int( 31,31,127,178 );  // rgb2287
  pick[7820].Int( 31,31,159,178 );  // rgb22A7
  pick[7821].Int( 31,31,191,178 );  // rgb22C7
  pick[7822].Int( 31,31,223,178 );  // rgb22E7
  pick[7823].Int( 31,63,0,178 );  // rgb2407
  pick[7824].Int( 31,63,31,178 );  // rgb2427
  pick[7825].Int( 31,63,63,178 );  // rgb2447
  pick[7826].Int( 31,63,95,178 );  // rgb2467
  pick[7827].Int( 31,63,127,178 );  // rgb2487
  pick[7828].Int( 31,63,159,178 );  // rgb24A7
  pick[7829].Int( 31,63,191,178 );  // rgb24C7
  pick[7830].Int( 31,63,223,178 );  // rgb24E7
  pick[7831].Int( 31,95,0,178 );  // rgb2607
  pick[7832].Int( 31,95,31,178 );  // rgb2627
  pick[7833].Int( 31,95,63,178 );  // rgb2647
  pick[7834].Int( 31,95,95,178 );  // rgb2667
  pick[7835].Int( 31,95,127,178 );  // rgb2687
  pick[7836].Int( 31,95,159,178 );  // rgb26A7
  pick[7837].Int( 31,95,191,178 );  // rgb26C7
  pick[7838].Int( 31,95,223,178 );  // rgb26E7
  pick[7839].Int( 31,127,0,178 );  // rgb2807
  pick[7840].Int( 31,127,31,178 );  // rgb2827
  pick[7841].Int( 31,127,63,178 );  // rgb2847
  pick[7842].Int( 31,127,95,178 );  // rgb2867
  pick[7843].Int( 31,127,127,178 );  // rgb2887
  pick[7844].Int( 31,127,159,178 );  // rgb28A7
  pick[7845].Int( 31,127,191,178 );  // rgb28C7
  pick[7846].Int( 31,127,223,178 );  // rgb28E7
  pick[7847].Int( 31,159,0,178 );  // rgb2A07
  pick[7848].Int( 31,159,31,178 );  // rgb2A27
  pick[7849].Int( 31,159,63,178 );  // rgb2A47
  pick[7850].Int( 31,159,95,178 );  // rgb2A67
  pick[7851].Int( 31,159,127,178 );  // rgb2A87
  pick[7852].Int( 31,159,159,178 );  // rgb2AA7
  pick[7853].Int( 31,159,191,178 );  // rgb2AC7
  pick[7854].Int( 31,159,223,178 );  // rgb2AE7
  pick[7855].Int( 31,191,0,178 );  // rgb2C07
  pick[7856].Int( 31,191,31,178 );  // rgb2C27
  pick[7857].Int( 31,191,63,178 );  // rgb2C47
  pick[7858].Int( 31,191,95,178 );  // rgb2C67
  pick[7859].Int( 31,191,127,178 );  // rgb2C87
  pick[7860].Int( 31,191,159,178 );  // rgb2CA7
  pick[7861].Int( 31,191,191,178 );  // rgb2CC7
  pick[7862].Int( 31,191,223,178 );  // rgb2CE7
  pick[7863].Int( 31,223,0,178 );  // rgb2E07
  pick[7864].Int( 31,223,31,178 );  // rgb2E27
  pick[7865].Int( 31,223,63,178 );  // rgb2E47
  pick[7866].Int( 31,223,95,178 );  // rgb2E67
  pick[7867].Int( 31,223,127,178 );  // rgb2E87
  pick[7868].Int( 31,223,159,178 );  // rgb2EA7
  pick[7869].Int( 31,223,191,178 );  // rgb2EC7
  pick[7870].Int( 31,223,223,178 );  // rgb2EE7
  pick[7871].Int( 63,0,0,178 );  // rgb4007
  pick[7872].Int( 63,0,31,178 );  // rgb4027
  pick[7873].Int( 63,0,63,178 );  // rgb4047
  pick[7874].Int( 63,0,95,178 );  // rgb4067
  pick[7875].Int( 63,0,127,178 );  // rgb4087
  pick[7876].Int( 63,0,159,178 );  // rgb40A7
  pick[7877].Int( 63,0,191,178 );  // rgb40C7
  pick[7878].Int( 63,0,223,178 );  // rgb40E7
  pick[7879].Int( 63,31,0,178 );  // rgb4207
  pick[7880].Int( 63,31,31,178 );  // rgb4227
  pick[7881].Int( 63,31,63,178 );  // rgb4247
  pick[7882].Int( 63,31,95,178 );  // rgb4267
  pick[7883].Int( 63,31,127,178 );  // rgb4287
  pick[7884].Int( 63,31,159,178 );  // rgb42A7
  pick[7885].Int( 63,31,191,178 );  // rgb42C7
  pick[7886].Int( 63,31,223,178 );  // rgb42E7
  pick[7887].Int( 63,63,0,178 );  // rgb4407
  pick[7888].Int( 63,63,31,178 );  // rgb4427
  pick[7889].Int( 63,63,63,178 );  // rgb4447
  pick[7890].Int( 63,63,95,178 );  // rgb4467
  pick[7891].Int( 63,63,127,178 );  // rgb4487
  pick[7892].Int( 63,63,159,178 );  // rgb44A7
  pick[7893].Int( 63,63,191,178 );  // rgb44C7
  pick[7894].Int( 63,63,223,178 );  // rgb44E7
  pick[7895].Int( 63,95,0,178 );  // rgb4607
  pick[7896].Int( 63,95,31,178 );  // rgb4627
  pick[7897].Int( 63,95,63,178 );  // rgb4647
  pick[7898].Int( 63,95,95,178 );  // rgb4667
  pick[7899].Int( 63,95,127,178 );  // rgb4687
  pick[7900].Int( 63,95,159,178 );  // rgb46A7
  pick[7901].Int( 63,95,191,178 );  // rgb46C7
  pick[7902].Int( 63,95,223,178 );  // rgb46E7
  pick[7903].Int( 63,127,0,178 );  // rgb4807
  pick[7904].Int( 63,127,31,178 );  // rgb4827
  pick[7905].Int( 63,127,63,178 );  // rgb4847
  pick[7906].Int( 63,127,95,178 );  // rgb4867
  pick[7907].Int( 63,127,127,178 );  // rgb4887
  pick[7908].Int( 63,127,159,178 );  // rgb48A7
  pick[7909].Int( 63,127,191,178 );  // rgb48C7
  pick[7910].Int( 63,127,223,178 );  // rgb48E7
  pick[7911].Int( 63,159,0,178 );  // rgb4A07
  pick[7912].Int( 63,159,31,178 );  // rgb4A27
  pick[7913].Int( 63,159,63,178 );  // rgb4A47
  pick[7914].Int( 63,159,95,178 );  // rgb4A67
  pick[7915].Int( 63,159,127,178 );  // rgb4A87
  pick[7916].Int( 63,159,159,178 );  // rgb4AA7
  pick[7917].Int( 63,159,191,178 );  // rgb4AC7
  pick[7918].Int( 63,159,223,178 );  // rgb4AE7
  pick[7919].Int( 63,191,0,178 );  // rgb4C07
  pick[7920].Int( 63,191,31,178 );  // rgb4C27
  pick[7921].Int( 63,191,63,178 );  // rgb4C47
  pick[7922].Int( 63,191,95,178 );  // rgb4C67
  pick[7923].Int( 63,191,127,178 );  // rgb4C87
  pick[7924].Int( 63,191,159,178 );  // rgb4CA7
  pick[7925].Int( 63,191,191,178 );  // rgb4CC7
  pick[7926].Int( 63,191,223,178 );  // rgb4CE7
  pick[7927].Int( 63,223,0,178 );  // rgb4E07
  pick[7928].Int( 63,223,31,178 );  // rgb4E27
  pick[7929].Int( 63,223,63,178 );  // rgb4E47
  pick[7930].Int( 63,223,95,178 );  // rgb4E67
  pick[7931].Int( 63,223,127,178 );  // rgb4E87
  pick[7932].Int( 63,223,159,178 );  // rgb4EA7
  pick[7933].Int( 63,223,191,178 );  // rgb4EC7
  pick[7934].Int( 63,223,223,178 );  // rgb4EE7
  pick[7935].Int( 95,0,0,178 );  // rgb6007
  pick[7936].Int( 95,0,31,178 );  // rgb6027
  pick[7937].Int( 95,0,63,178 );  // rgb6047
  pick[7938].Int( 95,0,95,178 );  // rgb6067
  pick[7939].Int( 95,0,127,178 );  // rgb6087
  pick[7940].Int( 95,0,159,178 );  // rgb60A7
  pick[7941].Int( 95,0,191,178 );  // rgb60C7
  pick[7942].Int( 95,0,223,178 );  // rgb60E7
  pick[7943].Int( 95,31,0,178 );  // rgb6207
  pick[7944].Int( 95,31,31,178 );  // rgb6227
  pick[7945].Int( 95,31,63,178 );  // rgb6247
  pick[7946].Int( 95,31,95,178 );  // rgb6267
  pick[7947].Int( 95,31,127,178 );  // rgb6287
  pick[7948].Int( 95,31,159,178 );  // rgb62A7
  pick[7949].Int( 95,31,191,178 );  // rgb62C7
  pick[7950].Int( 95,31,223,178 );  // rgb62E7
  pick[7951].Int( 95,63,0,178 );  // rgb6407
  pick[7952].Int( 95,63,31,178 );  // rgb6427
  pick[7953].Int( 95,63,63,178 );  // rgb6447
  pick[7954].Int( 95,63,95,178 );  // rgb6467
  pick[7955].Int( 95,63,127,178 );  // rgb6487
  pick[7956].Int( 95,63,159,178 );  // rgb64A7
  pick[7957].Int( 95,63,191,178 );  // rgb64C7
  pick[7958].Int( 95,63,223,178 );  // rgb64E7
  pick[7959].Int( 95,95,0,178 );  // rgb6607
  pick[7960].Int( 95,95,31,178 );  // rgb6627
  pick[7961].Int( 95,95,63,178 );  // rgb6647
  pick[7962].Int( 95,95,95,178 );  // rgb6667
  pick[7963].Int( 95,95,127,178 );  // rgb6687
  pick[7964].Int( 95,95,159,178 );  // rgb66A7
  pick[7965].Int( 95,95,191,178 );  // rgb66C7
  pick[7966].Int( 95,95,223,178 );  // rgb66E7
  pick[7967].Int( 95,127,0,178 );  // rgb6807
  pick[7968].Int( 95,127,31,178 );  // rgb6827
  pick[7969].Int( 95,127,63,178 );  // rgb6847
  pick[7970].Int( 95,127,95,178 );  // rgb6867
  pick[7971].Int( 95,127,127,178 );  // rgb6887
  pick[7972].Int( 95,127,159,178 );  // rgb68A7
  pick[7973].Int( 95,127,191,178 );  // rgb68C7
  pick[7974].Int( 95,127,223,178 );  // rgb68E7
  pick[7975].Int( 95,159,0,178 );  // rgb6A07
  pick[7976].Int( 95,159,31,178 );  // rgb6A27
  pick[7977].Int( 95,159,63,178 );  // rgb6A47
  pick[7978].Int( 95,159,95,178 );  // rgb6A67
  pick[7979].Int( 95,159,127,178 );  // rgb6A87
  pick[7980].Int( 95,159,159,178 );  // rgb6AA7
  pick[7981].Int( 95,159,191,178 );  // rgb6AC7
  pick[7982].Int( 95,159,223,178 );  // rgb6AE7
  pick[7983].Int( 95,191,0,178 );  // rgb6C07
  pick[7984].Int( 95,191,31,178 );  // rgb6C27
  pick[7985].Int( 95,191,63,178 );  // rgb6C47
  pick[7986].Int( 95,191,95,178 );  // rgb6C67
  pick[7987].Int( 95,191,127,178 );  // rgb6C87
  pick[7988].Int( 95,191,159,178 );  // rgb6CA7
  pick[7989].Int( 95,191,191,178 );  // rgb6CC7
  pick[7990].Int( 95,191,223,178 );  // rgb6CE7
  pick[7991].Int( 95,223,0,178 );  // rgb6E07
  pick[7992].Int( 95,223,31,178 );  // rgb6E27
  pick[7993].Int( 95,223,63,178 );  // rgb6E47
  pick[7994].Int( 95,223,95,178 );  // rgb6E67
  pick[7995].Int( 95,223,127,178 );  // rgb6E87
  pick[7996].Int( 95,223,159,178 );  // rgb6EA7
  pick[7997].Int( 95,223,191,178 );  // rgb6EC7
  pick[7998].Int( 95,223,223,178 );  // rgb6EE7
  pick[7999].Int( 127,0,0,178 );  // rgb8007
  pick[8000].Int( 127,0,31,178 );  // rgb8027
  pick[8001].Int( 127,0,63,178 );  // rgb8047
  pick[8002].Int( 127,0,95,178 );  // rgb8067
  pick[8003].Int( 127,0,127,178 );  // rgb8087
  pick[8004].Int( 127,0,159,178 );  // rgb80A7
  pick[8005].Int( 127,0,191,178 );  // rgb80C7
  pick[8006].Int( 127,0,223,178 );  // rgb80E7
  pick[8007].Int( 127,31,0,178 );  // rgb8207
  pick[8008].Int( 127,31,31,178 );  // rgb8227
  pick[8009].Int( 127,31,63,178 );  // rgb8247
  pick[8010].Int( 127,31,95,178 );  // rgb8267
  pick[8011].Int( 127,31,127,178 );  // rgb8287
  pick[8012].Int( 127,31,159,178 );  // rgb82A7
  pick[8013].Int( 127,31,191,178 );  // rgb82C7
  pick[8014].Int( 127,31,223,178 );  // rgb82E7
  pick[8015].Int( 127,63,0,178 );  // rgb8407
  pick[8016].Int( 127,63,31,178 );  // rgb8427
  pick[8017].Int( 127,63,63,178 );  // rgb8447
  pick[8018].Int( 127,63,95,178 );  // rgb8467
  pick[8019].Int( 127,63,127,178 );  // rgb8487
  pick[8020].Int( 127,63,159,178 );  // rgb84A7
  pick[8021].Int( 127,63,191,178 );  // rgb84C7
  pick[8022].Int( 127,63,223,178 );  // rgb84E7
  pick[8023].Int( 127,95,0,178 );  // rgb8607
  pick[8024].Int( 127,95,31,178 );  // rgb8627
  pick[8025].Int( 127,95,63,178 );  // rgb8647
  pick[8026].Int( 127,95,95,178 );  // rgb8667
  pick[8027].Int( 127,95,127,178 );  // rgb8687
  pick[8028].Int( 127,95,159,178 );  // rgb86A7
  pick[8029].Int( 127,95,191,178 );  // rgb86C7
  pick[8030].Int( 127,95,223,178 );  // rgb86E7
  pick[8031].Int( 127,127,0,178 );  // rgb8807
  pick[8032].Int( 127,127,31,178 );  // rgb8827
  pick[8033].Int( 127,127,63,178 );  // rgb8847
  pick[8034].Int( 127,127,95,178 );  // rgb8867
  pick[8035].Int( 127,127,127,178 );  // rgb8887
  pick[8036].Int( 127,127,159,178 );  // rgb88A7
  pick[8037].Int( 127,127,191,178 );  // rgb88C7
  pick[8038].Int( 127,127,223,178 );  // rgb88E7
  pick[8039].Int( 127,159,0,178 );  // rgb8A07
  pick[8040].Int( 127,159,31,178 );  // rgb8A27
  pick[8041].Int( 127,159,63,178 );  // rgb8A47
  pick[8042].Int( 127,159,95,178 );  // rgb8A67
  pick[8043].Int( 127,159,127,178 );  // rgb8A87
  pick[8044].Int( 127,159,159,178 );  // rgb8AA7
  pick[8045].Int( 127,159,191,178 );  // rgb8AC7
  pick[8046].Int( 127,159,223,178 );  // rgb8AE7
  pick[8047].Int( 127,191,0,178 );  // rgb8C07
  pick[8048].Int( 127,191,31,178 );  // rgb8C27
  pick[8049].Int( 127,191,63,178 );  // rgb8C47
  pick[8050].Int( 127,191,95,178 );  // rgb8C67
  pick[8051].Int( 127,191,127,178 );  // rgb8C87
  pick[8052].Int( 127,191,159,178 );  // rgb8CA7
  pick[8053].Int( 127,191,191,178 );  // rgb8CC7
  pick[8054].Int( 127,191,223,178 );  // rgb8CE7
  pick[8055].Int( 127,223,0,178 );  // rgb8E07
  pick[8056].Int( 127,223,31,178 );  // rgb8E27
  pick[8057].Int( 127,223,63,178 );  // rgb8E47
  pick[8058].Int( 127,223,95,178 );  // rgb8E67
  pick[8059].Int( 127,223,127,178 );  // rgb8E87
  pick[8060].Int( 127,223,159,178 );  // rgb8EA7
  pick[8061].Int( 127,223,191,178 );  // rgb8EC7
  pick[8062].Int( 127,223,223,178 );  // rgb8EE7
  pick[8063].Int( 159,0,0,178 );  // rgbA007
  pick[8064].Int( 159,0,31,178 );  // rgbA027
  pick[8065].Int( 159,0,63,178 );  // rgbA047
  pick[8066].Int( 159,0,95,178 );  // rgbA067
  pick[8067].Int( 159,0,127,178 );  // rgbA087
  pick[8068].Int( 159,0,159,178 );  // rgbA0A7
  pick[8069].Int( 159,0,191,178 );  // rgbA0C7
  pick[8070].Int( 159,0,223,178 );  // rgbA0E7
  pick[8071].Int( 159,31,0,178 );  // rgbA207
  pick[8072].Int( 159,31,31,178 );  // rgbA227
  pick[8073].Int( 159,31,63,178 );  // rgbA247
  pick[8074].Int( 159,31,95,178 );  // rgbA267
  pick[8075].Int( 159,31,127,178 );  // rgbA287
  pick[8076].Int( 159,31,159,178 );  // rgbA2A7
  pick[8077].Int( 159,31,191,178 );  // rgbA2C7
  pick[8078].Int( 159,31,223,178 );  // rgbA2E7
  pick[8079].Int( 159,63,0,178 );  // rgbA407
  pick[8080].Int( 159,63,31,178 );  // rgbA427
  pick[8081].Int( 159,63,63,178 );  // rgbA447
  pick[8082].Int( 159,63,95,178 );  // rgbA467
  pick[8083].Int( 159,63,127,178 );  // rgbA487
  pick[8084].Int( 159,63,159,178 );  // rgbA4A7
  pick[8085].Int( 159,63,191,178 );  // rgbA4C7
  pick[8086].Int( 159,63,223,178 );  // rgbA4E7
  pick[8087].Int( 159,95,0,178 );  // rgbA607
  pick[8088].Int( 159,95,31,178 );  // rgbA627
  pick[8089].Int( 159,95,63,178 );  // rgbA647
  pick[8090].Int( 159,95,95,178 );  // rgbA667
  pick[8091].Int( 159,95,127,178 );  // rgbA687
  pick[8092].Int( 159,95,159,178 );  // rgbA6A7
  pick[8093].Int( 159,95,191,178 );  // rgbA6C7
  pick[8094].Int( 159,95,223,178 );  // rgbA6E7
  pick[8095].Int( 159,127,0,178 );  // rgbA807
  pick[8096].Int( 159,127,31,178 );  // rgbA827
  pick[8097].Int( 159,127,63,178 );  // rgbA847
  pick[8098].Int( 159,127,95,178 );  // rgbA867
  pick[8099].Int( 159,127,127,178 );  // rgbA887
  pick[8100].Int( 159,127,159,178 );  // rgbA8A7
  pick[8101].Int( 159,127,191,178 );  // rgbA8C7
  pick[8102].Int( 159,127,223,178 );  // rgbA8E7
  pick[8103].Int( 159,159,0,178 );  // rgbAA07
  pick[8104].Int( 159,159,31,178 );  // rgbAA27
  pick[8105].Int( 159,159,63,178 );  // rgbAA47
  pick[8106].Int( 159,159,95,178 );  // rgbAA67
  pick[8107].Int( 159,159,127,178 );  // rgbAA87
  pick[8108].Int( 159,159,159,178 );  // rgbAAA7
  pick[8109].Int( 159,159,191,178 );  // rgbAAC7
  pick[8110].Int( 159,159,223,178 );  // rgbAAE7
  pick[8111].Int( 159,191,0,178 );  // rgbAC07
  pick[8112].Int( 159,191,31,178 );  // rgbAC27
  pick[8113].Int( 159,191,63,178 );  // rgbAC47
  pick[8114].Int( 159,191,95,178 );  // rgbAC67
  pick[8115].Int( 159,191,127,178 );  // rgbAC87
  pick[8116].Int( 159,191,159,178 );  // rgbACA7
  pick[8117].Int( 159,191,191,178 );  // rgbACC7
  pick[8118].Int( 159,191,223,178 );  // rgbACE7
  pick[8119].Int( 159,223,0,178 );  // rgbAE07
  pick[8120].Int( 159,223,31,178 );  // rgbAE27
  pick[8121].Int( 159,223,63,178 );  // rgbAE47
  pick[8122].Int( 159,223,95,178 );  // rgbAE67
  pick[8123].Int( 159,223,127,178 );  // rgbAE87
  pick[8124].Int( 159,223,159,178 );  // rgbAEA7
  pick[8125].Int( 159,223,191,178 );  // rgbAEC7
  pick[8126].Int( 159,223,223,178 );  // rgbAEE7
  pick[8127].Int( 191,0,0,178 );  // rgbC007
  pick[8128].Int( 191,0,31,178 );  // rgbC027
  pick[8129].Int( 191,0,63,178 );  // rgbC047
  pick[8130].Int( 191,0,95,178 );  // rgbC067
  pick[8131].Int( 191,0,127,178 );  // rgbC087
  pick[8132].Int( 191,0,159,178 );  // rgbC0A7
  pick[8133].Int( 191,0,191,178 );  // rgbC0C7
  pick[8134].Int( 191,0,223,178 );  // rgbC0E7
  pick[8135].Int( 191,31,0,178 );  // rgbC207
  pick[8136].Int( 191,31,31,178 );  // rgbC227
  pick[8137].Int( 191,31,63,178 );  // rgbC247
  pick[8138].Int( 191,31,95,178 );  // rgbC267
  pick[8139].Int( 191,31,127,178 );  // rgbC287
  pick[8140].Int( 191,31,159,178 );  // rgbC2A7
  pick[8141].Int( 191,31,191,178 );  // rgbC2C7
  pick[8142].Int( 191,31,223,178 );  // rgbC2E7
  pick[8143].Int( 191,63,0,178 );  // rgbC407
  pick[8144].Int( 191,63,31,178 );  // rgbC427
  pick[8145].Int( 191,63,63,178 );  // rgbC447
  pick[8146].Int( 191,63,95,178 );  // rgbC467
  pick[8147].Int( 191,63,127,178 );  // rgbC487
  pick[8148].Int( 191,63,159,178 );  // rgbC4A7
  pick[8149].Int( 191,63,191,178 );  // rgbC4C7
  pick[8150].Int( 191,63,223,178 );  // rgbC4E7
  pick[8151].Int( 191,95,0,178 );  // rgbC607
  pick[8152].Int( 191,95,31,178 );  // rgbC627
  pick[8153].Int( 191,95,63,178 );  // rgbC647
  pick[8154].Int( 191,95,95,178 );  // rgbC667
  pick[8155].Int( 191,95,127,178 );  // rgbC687
  pick[8156].Int( 191,95,159,178 );  // rgbC6A7
  pick[8157].Int( 191,95,191,178 );  // rgbC6C7
  pick[8158].Int( 191,95,223,178 );  // rgbC6E7
  pick[8159].Int( 191,127,0,178 );  // rgbC807
  pick[8160].Int( 191,127,31,178 );  // rgbC827
  pick[8161].Int( 191,127,63,178 );  // rgbC847
  pick[8162].Int( 191,127,95,178 );  // rgbC867
  pick[8163].Int( 191,127,127,178 );  // rgbC887
  pick[8164].Int( 191,127,159,178 );  // rgbC8A7
  pick[8165].Int( 191,127,191,178 );  // rgbC8C7
  pick[8166].Int( 191,127,223,178 );  // rgbC8E7
  pick[8167].Int( 191,159,0,178 );  // rgbCA07
  pick[8168].Int( 191,159,31,178 );  // rgbCA27
  pick[8169].Int( 191,159,63,178 );  // rgbCA47
  pick[8170].Int( 191,159,95,178 );  // rgbCA67
  pick[8171].Int( 191,159,127,178 );  // rgbCA87
  pick[8172].Int( 191,159,159,178 );  // rgbCAA7
  pick[8173].Int( 191,159,191,178 );  // rgbCAC7
  pick[8174].Int( 191,159,223,178 );  // rgbCAE7
  pick[8175].Int( 191,191,0,178 );  // rgbCC07
  pick[8176].Int( 191,191,31,178 );  // rgbCC27
  pick[8177].Int( 191,191,63,178 );  // rgbCC47
  pick[8178].Int( 191,191,95,178 );  // rgbCC67
  pick[8179].Int( 191,191,127,178 );  // rgbCC87
  pick[8180].Int( 191,191,159,178 );  // rgbCCA7
  pick[8181].Int( 191,191,191,178 );  // rgbCCC7
  pick[8182].Int( 191,191,223,178 );  // rgbCCE7
  pick[8183].Int( 191,223,0,178 );  // rgbCE07
  pick[8184].Int( 191,223,31,178 );  // rgbCE27
  pick[8185].Int( 191,223,63,178 );  // rgbCE47
  pick[8186].Int( 191,223,95,178 );  // rgbCE67
  pick[8187].Int( 191,223,127,178 );  // rgbCE87
  pick[8188].Int( 191,223,159,178 );  // rgbCEA7
  pick[8189].Int( 191,223,191,178 );  // rgbCEC7
  pick[8190].Int( 191,223,223,178 );  // rgbCEE7
  pick[8191].Int( 223,0,0,178 );  // rgbE007
  pick[8192].Int( 223,0,31,178 );  // rgbE027
  pick[8193].Int( 223,0,63,178 );  // rgbE047
  pick[8194].Int( 223,0,95,178 );  // rgbE067
  pick[8195].Int( 223,0,127,178 );  // rgbE087
  pick[8196].Int( 223,0,159,178 );  // rgbE0A7
  pick[8197].Int( 223,0,191,178 );  // rgbE0C7
  pick[8198].Int( 223,0,223,178 );  // rgbE0E7
  pick[8199].Int( 223,31,0,178 );  // rgbE207
  pick[8200].Int( 223,31,31,178 );  // rgbE227
  pick[8201].Int( 223,31,63,178 );  // rgbE247
  pick[8202].Int( 223,31,95,178 );  // rgbE267
  pick[8203].Int( 223,31,127,178 );  // rgbE287
  pick[8204].Int( 223,31,159,178 );  // rgbE2A7
  pick[8205].Int( 223,31,191,178 );  // rgbE2C7
  pick[8206].Int( 223,31,223,178 );  // rgbE2E7
  pick[8207].Int( 223,63,0,178 );  // rgbE407
  pick[8208].Int( 223,63,31,178 );  // rgbE427
  pick[8209].Int( 223,63,63,178 );  // rgbE447
  pick[8210].Int( 223,63,95,178 );  // rgbE467
  pick[8211].Int( 223,63,127,178 );  // rgbE487
  pick[8212].Int( 223,63,159,178 );  // rgbE4A7
  pick[8213].Int( 223,63,191,178 );  // rgbE4C7
  pick[8214].Int( 223,63,223,178 );  // rgbE4E7
  pick[8215].Int( 223,95,0,178 );  // rgbE607
  pick[8216].Int( 223,95,31,178 );  // rgbE627
  pick[8217].Int( 223,95,63,178 );  // rgbE647
  pick[8218].Int( 223,95,95,178 );  // rgbE667
  pick[8219].Int( 223,95,127,178 );  // rgbE687
  pick[8220].Int( 223,95,159,178 );  // rgbE6A7
  pick[8221].Int( 223,95,191,178 );  // rgbE6C7
  pick[8222].Int( 223,95,223,178 );  // rgbE6E7
  pick[8223].Int( 223,127,0,178 );  // rgbE807
  pick[8224].Int( 223,127,31,178 );  // rgbE827
  pick[8225].Int( 223,127,63,178 );  // rgbE847
  pick[8226].Int( 223,127,95,178 );  // rgbE867
  pick[8227].Int( 223,127,127,178 );  // rgbE887
  pick[8228].Int( 223,127,159,178 );  // rgbE8A7
  pick[8229].Int( 223,127,191,178 );  // rgbE8C7
  pick[8230].Int( 223,127,223,178 );  // rgbE8E7
  pick[8231].Int( 223,159,0,178 );  // rgbEA07
  pick[8232].Int( 223,159,31,178 );  // rgbEA27
  pick[8233].Int( 223,159,63,178 );  // rgbEA47
  pick[8234].Int( 223,159,95,178 );  // rgbEA67
  pick[8235].Int( 223,159,127,178 );  // rgbEA87
  pick[8236].Int( 223,159,159,178 );  // rgbEAA7
  pick[8237].Int( 223,159,191,178 );  // rgbEAC7
  pick[8238].Int( 223,159,223,178 );  // rgbEAE7
  pick[8239].Int( 223,191,0,178 );  // rgbEC07
  pick[8240].Int( 223,191,31,178 );  // rgbEC27
  pick[8241].Int( 223,191,63,178 );  // rgbEC47
  pick[8242].Int( 223,191,95,178 );  // rgbEC67
  pick[8243].Int( 223,191,127,178 );  // rgbEC87
  pick[8244].Int( 223,191,159,178 );  // rgbECA7
  pick[8245].Int( 223,191,191,178 );  // rgbECC7
  pick[8246].Int( 223,191,223,178 );  // rgbECE7
  pick[8247].Int( 223,223,0,178 );  // rgbEE07
  pick[8248].Int( 223,223,31,178 );  // rgbEE27
  pick[8249].Int( 223,223,63,178 );  // rgbEE47
  pick[8250].Int( 223,223,95,178 );  // rgbEE67
  pick[8251].Int( 223,223,127,178 );  // rgbEE87
  pick[8252].Int( 223,223,159,178 );  // rgbEEA7
  pick[8253].Int( 223,223,191,178 );  // rgbEEC7
  pick[8254].Int( 223,223,223,178 );  // rgbEEE7
  pick[8255].Int( 0,0,0,204 );  // rgb0008
  pick[8256].Int( 0,0,31,204 );  // rgb0028
  pick[8257].Int( 0,0,63,204 );  // rgb0048
  pick[8258].Int( 0,0,95,204 );  // rgb0068
  pick[8259].Int( 0,0,127,204 );  // rgb0088
  pick[8260].Int( 0,0,159,204 );  // rgb00A8
  pick[8261].Int( 0,0,191,204 );  // rgb00C8
  pick[8262].Int( 0,0,223,204 );  // rgb00E8
  pick[8263].Int( 0,31,0,204 );  // rgb0208
  pick[8264].Int( 0,31,31,204 );  // rgb0228
  pick[8265].Int( 0,31,63,204 );  // rgb0248
  pick[8266].Int( 0,31,95,204 );  // rgb0268
  pick[8267].Int( 0,31,127,204 );  // rgb0288
  pick[8268].Int( 0,31,159,204 );  // rgb02A8
  pick[8269].Int( 0,31,191,204 );  // rgb02C8
  pick[8270].Int( 0,31,223,204 );  // rgb02E8
  pick[8271].Int( 0,63,0,204 );  // rgb0408
  pick[8272].Int( 0,63,31,204 );  // rgb0428
  pick[8273].Int( 0,63,63,204 );  // rgb0448
  pick[8274].Int( 0,63,95,204 );  // rgb0468
  pick[8275].Int( 0,63,127,204 );  // rgb0488
  pick[8276].Int( 0,63,159,204 );  // rgb04A8
  pick[8277].Int( 0,63,191,204 );  // rgb04C8
  pick[8278].Int( 0,63,223,204 );  // rgb04E8
  pick[8279].Int( 0,95,0,204 );  // rgb0608
  pick[8280].Int( 0,95,31,204 );  // rgb0628
  pick[8281].Int( 0,95,63,204 );  // rgb0648
  pick[8282].Int( 0,95,95,204 );  // rgb0668
  pick[8283].Int( 0,95,127,204 );  // rgb0688
  pick[8284].Int( 0,95,159,204 );  // rgb06A8
  pick[8285].Int( 0,95,191,204 );  // rgb06C8
  pick[8286].Int( 0,95,223,204 );  // rgb06E8
  pick[8287].Int( 0,127,0,204 );  // rgb0808
  pick[8288].Int( 0,127,31,204 );  // rgb0828
  pick[8289].Int( 0,127,63,204 );  // rgb0848
  pick[8290].Int( 0,127,95,204 );  // rgb0868
  pick[8291].Int( 0,127,127,204 );  // rgb0888
  pick[8292].Int( 0,127,159,204 );  // rgb08A8
  pick[8293].Int( 0,127,191,204 );  // rgb08C8
  pick[8294].Int( 0,127,223,204 );  // rgb08E8
  pick[8295].Int( 0,159,0,204 );  // rgb0A08
  pick[8296].Int( 0,159,31,204 );  // rgb0A28
  pick[8297].Int( 0,159,63,204 );  // rgb0A48
  pick[8298].Int( 0,159,95,204 );  // rgb0A68
  pick[8299].Int( 0,159,127,204 );  // rgb0A88
  pick[8300].Int( 0,159,159,204 );  // rgb0AA8
  pick[8301].Int( 0,159,191,204 );  // rgb0AC8
  pick[8302].Int( 0,159,223,204 );  // rgb0AE8
  pick[8303].Int( 0,191,0,204 );  // rgb0C08
  pick[8304].Int( 0,191,31,204 );  // rgb0C28
  pick[8305].Int( 0,191,63,204 );  // rgb0C48
  pick[8306].Int( 0,191,95,204 );  // rgb0C68
  pick[8307].Int( 0,191,127,204 );  // rgb0C88
  pick[8308].Int( 0,191,159,204 );  // rgb0CA8
  pick[8309].Int( 0,191,191,204 );  // rgb0CC8
  pick[8310].Int( 0,191,223,204 );  // rgb0CE8
  pick[8311].Int( 0,223,0,204 );  // rgb0E08
  pick[8312].Int( 0,223,31,204 );  // rgb0E28
  pick[8313].Int( 0,223,63,204 );  // rgb0E48
  pick[8314].Int( 0,223,95,204 );  // rgb0E68
  pick[8315].Int( 0,223,127,204 );  // rgb0E88
  pick[8316].Int( 0,223,159,204 );  // rgb0EA8
  pick[8317].Int( 0,223,191,204 );  // rgb0EC8
  pick[8318].Int( 0,223,223,204 );  // rgb0EE8
  pick[8319].Int( 31,0,0,204 );  // rgb2008
  pick[8320].Int( 31,0,31,204 );  // rgb2028
  pick[8321].Int( 31,0,63,204 );  // rgb2048
  pick[8322].Int( 31,0,95,204 );  // rgb2068
  pick[8323].Int( 31,0,127,204 );  // rgb2088
  pick[8324].Int( 31,0,159,204 );  // rgb20A8
  pick[8325].Int( 31,0,191,204 );  // rgb20C8
  pick[8326].Int( 31,0,223,204 );  // rgb20E8
  pick[8327].Int( 31,31,0,204 );  // rgb2208
  pick[8328].Int( 31,31,31,204 );  // rgb2228
  pick[8329].Int( 31,31,63,204 );  // rgb2248
  pick[8330].Int( 31,31,95,204 );  // rgb2268
  pick[8331].Int( 31,31,127,204 );  // rgb2288
  pick[8332].Int( 31,31,159,204 );  // rgb22A8
  pick[8333].Int( 31,31,191,204 );  // rgb22C8
  pick[8334].Int( 31,31,223,204 );  // rgb22E8
  pick[8335].Int( 31,63,0,204 );  // rgb2408
  pick[8336].Int( 31,63,31,204 );  // rgb2428
  pick[8337].Int( 31,63,63,204 );  // rgb2448
  pick[8338].Int( 31,63,95,204 );  // rgb2468
  pick[8339].Int( 31,63,127,204 );  // rgb2488
  pick[8340].Int( 31,63,159,204 );  // rgb24A8
  pick[8341].Int( 31,63,191,204 );  // rgb24C8
  pick[8342].Int( 31,63,223,204 );  // rgb24E8
  pick[8343].Int( 31,95,0,204 );  // rgb2608
  pick[8344].Int( 31,95,31,204 );  // rgb2628
  pick[8345].Int( 31,95,63,204 );  // rgb2648
  pick[8346].Int( 31,95,95,204 );  // rgb2668
  pick[8347].Int( 31,95,127,204 );  // rgb2688
  pick[8348].Int( 31,95,159,204 );  // rgb26A8
  pick[8349].Int( 31,95,191,204 );  // rgb26C8
  pick[8350].Int( 31,95,223,204 );  // rgb26E8
  pick[8351].Int( 31,127,0,204 );  // rgb2808
  pick[8352].Int( 31,127,31,204 );  // rgb2828
  pick[8353].Int( 31,127,63,204 );  // rgb2848
  pick[8354].Int( 31,127,95,204 );  // rgb2868
  pick[8355].Int( 31,127,127,204 );  // rgb2888
  pick[8356].Int( 31,127,159,204 );  // rgb28A8
  pick[8357].Int( 31,127,191,204 );  // rgb28C8
  pick[8358].Int( 31,127,223,204 );  // rgb28E8
  pick[8359].Int( 31,159,0,204 );  // rgb2A08
  pick[8360].Int( 31,159,31,204 );  // rgb2A28
  pick[8361].Int( 31,159,63,204 );  // rgb2A48
  pick[8362].Int( 31,159,95,204 );  // rgb2A68
  pick[8363].Int( 31,159,127,204 );  // rgb2A88
  pick[8364].Int( 31,159,159,204 );  // rgb2AA8
  pick[8365].Int( 31,159,191,204 );  // rgb2AC8
  pick[8366].Int( 31,159,223,204 );  // rgb2AE8
  pick[8367].Int( 31,191,0,204 );  // rgb2C08
  pick[8368].Int( 31,191,31,204 );  // rgb2C28
  pick[8369].Int( 31,191,63,204 );  // rgb2C48
  pick[8370].Int( 31,191,95,204 );  // rgb2C68
  pick[8371].Int( 31,191,127,204 );  // rgb2C88
  pick[8372].Int( 31,191,159,204 );  // rgb2CA8
  pick[8373].Int( 31,191,191,204 );  // rgb2CC8
  pick[8374].Int( 31,191,223,204 );  // rgb2CE8
  pick[8375].Int( 31,223,0,204 );  // rgb2E08
  pick[8376].Int( 31,223,31,204 );  // rgb2E28
  pick[8377].Int( 31,223,63,204 );  // rgb2E48
  pick[8378].Int( 31,223,95,204 );  // rgb2E68
  pick[8379].Int( 31,223,127,204 );  // rgb2E88
  pick[8380].Int( 31,223,159,204 );  // rgb2EA8
  pick[8381].Int( 31,223,191,204 );  // rgb2EC8
  pick[8382].Int( 31,223,223,204 );  // rgb2EE8
  pick[8383].Int( 63,0,0,204 );  // rgb4008
  pick[8384].Int( 63,0,31,204 );  // rgb4028
  pick[8385].Int( 63,0,63,204 );  // rgb4048
  pick[8386].Int( 63,0,95,204 );  // rgb4068
  pick[8387].Int( 63,0,127,204 );  // rgb4088
  pick[8388].Int( 63,0,159,204 );  // rgb40A8
  pick[8389].Int( 63,0,191,204 );  // rgb40C8
  pick[8390].Int( 63,0,223,204 );  // rgb40E8
  pick[8391].Int( 63,31,0,204 );  // rgb4208
  pick[8392].Int( 63,31,31,204 );  // rgb4228
  pick[8393].Int( 63,31,63,204 );  // rgb4248
  pick[8394].Int( 63,31,95,204 );  // rgb4268
  pick[8395].Int( 63,31,127,204 );  // rgb4288
  pick[8396].Int( 63,31,159,204 );  // rgb42A8
  pick[8397].Int( 63,31,191,204 );  // rgb42C8
  pick[8398].Int( 63,31,223,204 );  // rgb42E8
  pick[8399].Int( 63,63,0,204 );  // rgb4408
  pick[8400].Int( 63,63,31,204 );  // rgb4428
  pick[8401].Int( 63,63,63,204 );  // rgb4448
  pick[8402].Int( 63,63,95,204 );  // rgb4468
  pick[8403].Int( 63,63,127,204 );  // rgb4488
  pick[8404].Int( 63,63,159,204 );  // rgb44A8
  pick[8405].Int( 63,63,191,204 );  // rgb44C8
  pick[8406].Int( 63,63,223,204 );  // rgb44E8
  pick[8407].Int( 63,95,0,204 );  // rgb4608
  pick[8408].Int( 63,95,31,204 );  // rgb4628
  pick[8409].Int( 63,95,63,204 );  // rgb4648
  pick[8410].Int( 63,95,95,204 );  // rgb4668
  pick[8411].Int( 63,95,127,204 );  // rgb4688
  pick[8412].Int( 63,95,159,204 );  // rgb46A8
  pick[8413].Int( 63,95,191,204 );  // rgb46C8
  pick[8414].Int( 63,95,223,204 );  // rgb46E8
  pick[8415].Int( 63,127,0,204 );  // rgb4808
  pick[8416].Int( 63,127,31,204 );  // rgb4828
  pick[8417].Int( 63,127,63,204 );  // rgb4848
  pick[8418].Int( 63,127,95,204 );  // rgb4868
  pick[8419].Int( 63,127,127,204 );  // rgb4888
  pick[8420].Int( 63,127,159,204 );  // rgb48A8
  pick[8421].Int( 63,127,191,204 );  // rgb48C8
  pick[8422].Int( 63,127,223,204 );  // rgb48E8
  pick[8423].Int( 63,159,0,204 );  // rgb4A08
  pick[8424].Int( 63,159,31,204 );  // rgb4A28
  pick[8425].Int( 63,159,63,204 );  // rgb4A48
  pick[8426].Int( 63,159,95,204 );  // rgb4A68
  pick[8427].Int( 63,159,127,204 );  // rgb4A88
  pick[8428].Int( 63,159,159,204 );  // rgb4AA8
  pick[8429].Int( 63,159,191,204 );  // rgb4AC8
  pick[8430].Int( 63,159,223,204 );  // rgb4AE8
  pick[8431].Int( 63,191,0,204 );  // rgb4C08
  pick[8432].Int( 63,191,31,204 );  // rgb4C28
  pick[8433].Int( 63,191,63,204 );  // rgb4C48
  pick[8434].Int( 63,191,95,204 );  // rgb4C68
  pick[8435].Int( 63,191,127,204 );  // rgb4C88
  pick[8436].Int( 63,191,159,204 );  // rgb4CA8
  pick[8437].Int( 63,191,191,204 );  // rgb4CC8
  pick[8438].Int( 63,191,223,204 );  // rgb4CE8
  pick[8439].Int( 63,223,0,204 );  // rgb4E08
  pick[8440].Int( 63,223,31,204 );  // rgb4E28
  pick[8441].Int( 63,223,63,204 );  // rgb4E48
  pick[8442].Int( 63,223,95,204 );  // rgb4E68
  pick[8443].Int( 63,223,127,204 );  // rgb4E88
  pick[8444].Int( 63,223,159,204 );  // rgb4EA8
  pick[8445].Int( 63,223,191,204 );  // rgb4EC8
  pick[8446].Int( 63,223,223,204 );  // rgb4EE8
  pick[8447].Int( 95,0,0,204 );  // rgb6008
  pick[8448].Int( 95,0,31,204 );  // rgb6028
  pick[8449].Int( 95,0,63,204 );  // rgb6048
  pick[8450].Int( 95,0,95,204 );  // rgb6068
  pick[8451].Int( 95,0,127,204 );  // rgb6088
  pick[8452].Int( 95,0,159,204 );  // rgb60A8
  pick[8453].Int( 95,0,191,204 );  // rgb60C8
  pick[8454].Int( 95,0,223,204 );  // rgb60E8
  pick[8455].Int( 95,31,0,204 );  // rgb6208
  pick[8456].Int( 95,31,31,204 );  // rgb6228
  pick[8457].Int( 95,31,63,204 );  // rgb6248
  pick[8458].Int( 95,31,95,204 );  // rgb6268
  pick[8459].Int( 95,31,127,204 );  // rgb6288
  pick[8460].Int( 95,31,159,204 );  // rgb62A8
  pick[8461].Int( 95,31,191,204 );  // rgb62C8
  pick[8462].Int( 95,31,223,204 );  // rgb62E8
  pick[8463].Int( 95,63,0,204 );  // rgb6408
  pick[8464].Int( 95,63,31,204 );  // rgb6428
  pick[8465].Int( 95,63,63,204 );  // rgb6448
  pick[8466].Int( 95,63,95,204 );  // rgb6468
  pick[8467].Int( 95,63,127,204 );  // rgb6488
  pick[8468].Int( 95,63,159,204 );  // rgb64A8
  pick[8469].Int( 95,63,191,204 );  // rgb64C8
  pick[8470].Int( 95,63,223,204 );  // rgb64E8
  pick[8471].Int( 95,95,0,204 );  // rgb6608
  pick[8472].Int( 95,95,31,204 );  // rgb6628
  pick[8473].Int( 95,95,63,204 );  // rgb6648
  pick[8474].Int( 95,95,95,204 );  // rgb6668
  pick[8475].Int( 95,95,127,204 );  // rgb6688
  pick[8476].Int( 95,95,159,204 );  // rgb66A8
  pick[8477].Int( 95,95,191,204 );  // rgb66C8
  pick[8478].Int( 95,95,223,204 );  // rgb66E8
  pick[8479].Int( 95,127,0,204 );  // rgb6808
  pick[8480].Int( 95,127,31,204 );  // rgb6828
  pick[8481].Int( 95,127,63,204 );  // rgb6848
  pick[8482].Int( 95,127,95,204 );  // rgb6868
  pick[8483].Int( 95,127,127,204 );  // rgb6888
  pick[8484].Int( 95,127,159,204 );  // rgb68A8
  pick[8485].Int( 95,127,191,204 );  // rgb68C8
  pick[8486].Int( 95,127,223,204 );  // rgb68E8
  pick[8487].Int( 95,159,0,204 );  // rgb6A08
  pick[8488].Int( 95,159,31,204 );  // rgb6A28
  pick[8489].Int( 95,159,63,204 );  // rgb6A48
  pick[8490].Int( 95,159,95,204 );  // rgb6A68
  pick[8491].Int( 95,159,127,204 );  // rgb6A88
  pick[8492].Int( 95,159,159,204 );  // rgb6AA8
  pick[8493].Int( 95,159,191,204 );  // rgb6AC8
  pick[8494].Int( 95,159,223,204 );  // rgb6AE8
  pick[8495].Int( 95,191,0,204 );  // rgb6C08
  pick[8496].Int( 95,191,31,204 );  // rgb6C28
  pick[8497].Int( 95,191,63,204 );  // rgb6C48
  pick[8498].Int( 95,191,95,204 );  // rgb6C68
  pick[8499].Int( 95,191,127,204 );  // rgb6C88
  pick[8500].Int( 95,191,159,204 );  // rgb6CA8
  pick[8501].Int( 95,191,191,204 );  // rgb6CC8
  pick[8502].Int( 95,191,223,204 );  // rgb6CE8
  pick[8503].Int( 95,223,0,204 );  // rgb6E08
  pick[8504].Int( 95,223,31,204 );  // rgb6E28
  pick[8505].Int( 95,223,63,204 );  // rgb6E48
  pick[8506].Int( 95,223,95,204 );  // rgb6E68
  pick[8507].Int( 95,223,127,204 );  // rgb6E88
  pick[8508].Int( 95,223,159,204 );  // rgb6EA8
  pick[8509].Int( 95,223,191,204 );  // rgb6EC8
  pick[8510].Int( 95,223,223,204 );  // rgb6EE8
  pick[8511].Int( 127,0,0,204 );  // rgb8008
  pick[8512].Int( 127,0,31,204 );  // rgb8028
  pick[8513].Int( 127,0,63,204 );  // rgb8048
  pick[8514].Int( 127,0,95,204 );  // rgb8068
  pick[8515].Int( 127,0,127,204 );  // rgb8088
  pick[8516].Int( 127,0,159,204 );  // rgb80A8
  pick[8517].Int( 127,0,191,204 );  // rgb80C8
  pick[8518].Int( 127,0,223,204 );  // rgb80E8
  pick[8519].Int( 127,31,0,204 );  // rgb8208
  pick[8520].Int( 127,31,31,204 );  // rgb8228
  pick[8521].Int( 127,31,63,204 );  // rgb8248
  pick[8522].Int( 127,31,95,204 );  // rgb8268
  pick[8523].Int( 127,31,127,204 );  // rgb8288
  pick[8524].Int( 127,31,159,204 );  // rgb82A8
  pick[8525].Int( 127,31,191,204 );  // rgb82C8
  pick[8526].Int( 127,31,223,204 );  // rgb82E8
  pick[8527].Int( 127,63,0,204 );  // rgb8408
  pick[8528].Int( 127,63,31,204 );  // rgb8428
  pick[8529].Int( 127,63,63,204 );  // rgb8448
  pick[8530].Int( 127,63,95,204 );  // rgb8468
  pick[8531].Int( 127,63,127,204 );  // rgb8488
  pick[8532].Int( 127,63,159,204 );  // rgb84A8
  pick[8533].Int( 127,63,191,204 );  // rgb84C8
  pick[8534].Int( 127,63,223,204 );  // rgb84E8
  pick[8535].Int( 127,95,0,204 );  // rgb8608
  pick[8536].Int( 127,95,31,204 );  // rgb8628
  pick[8537].Int( 127,95,63,204 );  // rgb8648
  pick[8538].Int( 127,95,95,204 );  // rgb8668
  pick[8539].Int( 127,95,127,204 );  // rgb8688
  pick[8540].Int( 127,95,159,204 );  // rgb86A8
  pick[8541].Int( 127,95,191,204 );  // rgb86C8
  pick[8542].Int( 127,95,223,204 );  // rgb86E8
  pick[8543].Int( 127,127,0,204 );  // rgb8808
  pick[8544].Int( 127,127,31,204 );  // rgb8828
  pick[8545].Int( 127,127,63,204 );  // rgb8848
  pick[8546].Int( 127,127,95,204 );  // rgb8868
  pick[8547].Int( 127,127,127,204 );  // rgb8888
  pick[8548].Int( 127,127,159,204 );  // rgb88A8
  pick[8549].Int( 127,127,191,204 );  // rgb88C8
  pick[8550].Int( 127,127,223,204 );  // rgb88E8
  pick[8551].Int( 127,159,0,204 );  // rgb8A08
  pick[8552].Int( 127,159,31,204 );  // rgb8A28
  pick[8553].Int( 127,159,63,204 );  // rgb8A48
  pick[8554].Int( 127,159,95,204 );  // rgb8A68
  pick[8555].Int( 127,159,127,204 );  // rgb8A88
  pick[8556].Int( 127,159,159,204 );  // rgb8AA8
  pick[8557].Int( 127,159,191,204 );  // rgb8AC8
  pick[8558].Int( 127,159,223,204 );  // rgb8AE8
  pick[8559].Int( 127,191,0,204 );  // rgb8C08
  pick[8560].Int( 127,191,31,204 );  // rgb8C28
  pick[8561].Int( 127,191,63,204 );  // rgb8C48
  pick[8562].Int( 127,191,95,204 );  // rgb8C68
  pick[8563].Int( 127,191,127,204 );  // rgb8C88
  pick[8564].Int( 127,191,159,204 );  // rgb8CA8
  pick[8565].Int( 127,191,191,204 );  // rgb8CC8
  pick[8566].Int( 127,191,223,204 );  // rgb8CE8
  pick[8567].Int( 127,223,0,204 );  // rgb8E08
  pick[8568].Int( 127,223,31,204 );  // rgb8E28
  pick[8569].Int( 127,223,63,204 );  // rgb8E48
  pick[8570].Int( 127,223,95,204 );  // rgb8E68
  pick[8571].Int( 127,223,127,204 );  // rgb8E88
  pick[8572].Int( 127,223,159,204 );  // rgb8EA8
  pick[8573].Int( 127,223,191,204 );  // rgb8EC8
  pick[8574].Int( 127,223,223,204 );  // rgb8EE8
  pick[8575].Int( 159,0,0,204 );  // rgbA008
  pick[8576].Int( 159,0,31,204 );  // rgbA028
  pick[8577].Int( 159,0,63,204 );  // rgbA048
  pick[8578].Int( 159,0,95,204 );  // rgbA068
  pick[8579].Int( 159,0,127,204 );  // rgbA088
  pick[8580].Int( 159,0,159,204 );  // rgbA0A8
  pick[8581].Int( 159,0,191,204 );  // rgbA0C8
  pick[8582].Int( 159,0,223,204 );  // rgbA0E8
  pick[8583].Int( 159,31,0,204 );  // rgbA208
  pick[8584].Int( 159,31,31,204 );  // rgbA228
  pick[8585].Int( 159,31,63,204 );  // rgbA248
  pick[8586].Int( 159,31,95,204 );  // rgbA268
  pick[8587].Int( 159,31,127,204 );  // rgbA288
  pick[8588].Int( 159,31,159,204 );  // rgbA2A8
  pick[8589].Int( 159,31,191,204 );  // rgbA2C8
  pick[8590].Int( 159,31,223,204 );  // rgbA2E8
  pick[8591].Int( 159,63,0,204 );  // rgbA408
  pick[8592].Int( 159,63,31,204 );  // rgbA428
  pick[8593].Int( 159,63,63,204 );  // rgbA448
  pick[8594].Int( 159,63,95,204 );  // rgbA468
  pick[8595].Int( 159,63,127,204 );  // rgbA488
  pick[8596].Int( 159,63,159,204 );  // rgbA4A8
  pick[8597].Int( 159,63,191,204 );  // rgbA4C8
  pick[8598].Int( 159,63,223,204 );  // rgbA4E8
  pick[8599].Int( 159,95,0,204 );  // rgbA608
  pick[8600].Int( 159,95,31,204 );  // rgbA628
  pick[8601].Int( 159,95,63,204 );  // rgbA648
  pick[8602].Int( 159,95,95,204 );  // rgbA668
  pick[8603].Int( 159,95,127,204 );  // rgbA688
  pick[8604].Int( 159,95,159,204 );  // rgbA6A8
  pick[8605].Int( 159,95,191,204 );  // rgbA6C8
  pick[8606].Int( 159,95,223,204 );  // rgbA6E8
  pick[8607].Int( 159,127,0,204 );  // rgbA808
  pick[8608].Int( 159,127,31,204 );  // rgbA828
  pick[8609].Int( 159,127,63,204 );  // rgbA848
  pick[8610].Int( 159,127,95,204 );  // rgbA868
  pick[8611].Int( 159,127,127,204 );  // rgbA888
  pick[8612].Int( 159,127,159,204 );  // rgbA8A8
  pick[8613].Int( 159,127,191,204 );  // rgbA8C8
  pick[8614].Int( 159,127,223,204 );  // rgbA8E8
  pick[8615].Int( 159,159,0,204 );  // rgbAA08
  pick[8616].Int( 159,159,31,204 );  // rgbAA28
  pick[8617].Int( 159,159,63,204 );  // rgbAA48
  pick[8618].Int( 159,159,95,204 );  // rgbAA68
  pick[8619].Int( 159,159,127,204 );  // rgbAA88
  pick[8620].Int( 159,159,159,204 );  // rgbAAA8
  pick[8621].Int( 159,159,191,204 );  // rgbAAC8
  pick[8622].Int( 159,159,223,204 );  // rgbAAE8
  pick[8623].Int( 159,191,0,204 );  // rgbAC08
  pick[8624].Int( 159,191,31,204 );  // rgbAC28
  pick[8625].Int( 159,191,63,204 );  // rgbAC48
  pick[8626].Int( 159,191,95,204 );  // rgbAC68
  pick[8627].Int( 159,191,127,204 );  // rgbAC88
  pick[8628].Int( 159,191,159,204 );  // rgbACA8
  pick[8629].Int( 159,191,191,204 );  // rgbACC8
  pick[8630].Int( 159,191,223,204 );  // rgbACE8
  pick[8631].Int( 159,223,0,204 );  // rgbAE08
  pick[8632].Int( 159,223,31,204 );  // rgbAE28
  pick[8633].Int( 159,223,63,204 );  // rgbAE48
  pick[8634].Int( 159,223,95,204 );  // rgbAE68
  pick[8635].Int( 159,223,127,204 );  // rgbAE88
  pick[8636].Int( 159,223,159,204 );  // rgbAEA8
  pick[8637].Int( 159,223,191,204 );  // rgbAEC8
  pick[8638].Int( 159,223,223,204 );  // rgbAEE8
  pick[8639].Int( 191,0,0,204 );  // rgbC008
  pick[8640].Int( 191,0,31,204 );  // rgbC028
  pick[8641].Int( 191,0,63,204 );  // rgbC048
  pick[8642].Int( 191,0,95,204 );  // rgbC068
  pick[8643].Int( 191,0,127,204 );  // rgbC088
  pick[8644].Int( 191,0,159,204 );  // rgbC0A8
  pick[8645].Int( 191,0,191,204 );  // rgbC0C8
  pick[8646].Int( 191,0,223,204 );  // rgbC0E8
  pick[8647].Int( 191,31,0,204 );  // rgbC208
  pick[8648].Int( 191,31,31,204 );  // rgbC228
  pick[8649].Int( 191,31,63,204 );  // rgbC248
  pick[8650].Int( 191,31,95,204 );  // rgbC268
  pick[8651].Int( 191,31,127,204 );  // rgbC288
  pick[8652].Int( 191,31,159,204 );  // rgbC2A8
  pick[8653].Int( 191,31,191,204 );  // rgbC2C8
  pick[8654].Int( 191,31,223,204 );  // rgbC2E8
  pick[8655].Int( 191,63,0,204 );  // rgbC408
  pick[8656].Int( 191,63,31,204 );  // rgbC428
  pick[8657].Int( 191,63,63,204 );  // rgbC448
  pick[8658].Int( 191,63,95,204 );  // rgbC468
  pick[8659].Int( 191,63,127,204 );  // rgbC488
  pick[8660].Int( 191,63,159,204 );  // rgbC4A8
  pick[8661].Int( 191,63,191,204 );  // rgbC4C8
  pick[8662].Int( 191,63,223,204 );  // rgbC4E8
  pick[8663].Int( 191,95,0,204 );  // rgbC608
  pick[8664].Int( 191,95,31,204 );  // rgbC628
  pick[8665].Int( 191,95,63,204 );  // rgbC648
  pick[8666].Int( 191,95,95,204 );  // rgbC668
  pick[8667].Int( 191,95,127,204 );  // rgbC688
  pick[8668].Int( 191,95,159,204 );  // rgbC6A8
  pick[8669].Int( 191,95,191,204 );  // rgbC6C8
  pick[8670].Int( 191,95,223,204 );  // rgbC6E8
  pick[8671].Int( 191,127,0,204 );  // rgbC808
  pick[8672].Int( 191,127,31,204 );  // rgbC828
  pick[8673].Int( 191,127,63,204 );  // rgbC848
  pick[8674].Int( 191,127,95,204 );  // rgbC868
  pick[8675].Int( 191,127,127,204 );  // rgbC888
  pick[8676].Int( 191,127,159,204 );  // rgbC8A8
  pick[8677].Int( 191,127,191,204 );  // rgbC8C8
  pick[8678].Int( 191,127,223,204 );  // rgbC8E8
  pick[8679].Int( 191,159,0,204 );  // rgbCA08
  pick[8680].Int( 191,159,31,204 );  // rgbCA28
  pick[8681].Int( 191,159,63,204 );  // rgbCA48
  pick[8682].Int( 191,159,95,204 );  // rgbCA68
  pick[8683].Int( 191,159,127,204 );  // rgbCA88
  pick[8684].Int( 191,159,159,204 );  // rgbCAA8
  pick[8685].Int( 191,159,191,204 );  // rgbCAC8
  pick[8686].Int( 191,159,223,204 );  // rgbCAE8
  pick[8687].Int( 191,191,0,204 );  // rgbCC08
  pick[8688].Int( 191,191,31,204 );  // rgbCC28
  pick[8689].Int( 191,191,63,204 );  // rgbCC48
  pick[8690].Int( 191,191,95,204 );  // rgbCC68
  pick[8691].Int( 191,191,127,204 );  // rgbCC88
  pick[8692].Int( 191,191,159,204 );  // rgbCCA8
  pick[8693].Int( 191,191,191,204 );  // rgbCCC8
  pick[8694].Int( 191,191,223,204 );  // rgbCCE8
  pick[8695].Int( 191,223,0,204 );  // rgbCE08
  pick[8696].Int( 191,223,31,204 );  // rgbCE28
  pick[8697].Int( 191,223,63,204 );  // rgbCE48
  pick[8698].Int( 191,223,95,204 );  // rgbCE68
  pick[8699].Int( 191,223,127,204 );  // rgbCE88
  pick[8700].Int( 191,223,159,204 );  // rgbCEA8
  pick[8701].Int( 191,223,191,204 );  // rgbCEC8
  pick[8702].Int( 191,223,223,204 );  // rgbCEE8
  pick[8703].Int( 223,0,0,204 );  // rgbE008
  pick[8704].Int( 223,0,31,204 );  // rgbE028
  pick[8705].Int( 223,0,63,204 );  // rgbE048
  pick[8706].Int( 223,0,95,204 );  // rgbE068
  pick[8707].Int( 223,0,127,204 );  // rgbE088
  pick[8708].Int( 223,0,159,204 );  // rgbE0A8
  pick[8709].Int( 223,0,191,204 );  // rgbE0C8
  pick[8710].Int( 223,0,223,204 );  // rgbE0E8
  pick[8711].Int( 223,31,0,204 );  // rgbE208
  pick[8712].Int( 223,31,31,204 );  // rgbE228
  pick[8713].Int( 223,31,63,204 );  // rgbE248
  pick[8714].Int( 223,31,95,204 );  // rgbE268
  pick[8715].Int( 223,31,127,204 );  // rgbE288
  pick[8716].Int( 223,31,159,204 );  // rgbE2A8
  pick[8717].Int( 223,31,191,204 );  // rgbE2C8
  pick[8718].Int( 223,31,223,204 );  // rgbE2E8
  pick[8719].Int( 223,63,0,204 );  // rgbE408
  pick[8720].Int( 223,63,31,204 );  // rgbE428
  pick[8721].Int( 223,63,63,204 );  // rgbE448
  pick[8722].Int( 223,63,95,204 );  // rgbE468
  pick[8723].Int( 223,63,127,204 );  // rgbE488
  pick[8724].Int( 223,63,159,204 );  // rgbE4A8
  pick[8725].Int( 223,63,191,204 );  // rgbE4C8
  pick[8726].Int( 223,63,223,204 );  // rgbE4E8
  pick[8727].Int( 223,95,0,204 );  // rgbE608
  pick[8728].Int( 223,95,31,204 );  // rgbE628
  pick[8729].Int( 223,95,63,204 );  // rgbE648
  pick[8730].Int( 223,95,95,204 );  // rgbE668
  pick[8731].Int( 223,95,127,204 );  // rgbE688
  pick[8732].Int( 223,95,159,204 );  // rgbE6A8
  pick[8733].Int( 223,95,191,204 );  // rgbE6C8
  pick[8734].Int( 223,95,223,204 );  // rgbE6E8
  pick[8735].Int( 223,127,0,204 );  // rgbE808
  pick[8736].Int( 223,127,31,204 );  // rgbE828
  pick[8737].Int( 223,127,63,204 );  // rgbE848
  pick[8738].Int( 223,127,95,204 );  // rgbE868
  pick[8739].Int( 223,127,127,204 );  // rgbE888
  pick[8740].Int( 223,127,159,204 );  // rgbE8A8
  pick[8741].Int( 223,127,191,204 );  // rgbE8C8
  pick[8742].Int( 223,127,223,204 );  // rgbE8E8
  pick[8743].Int( 223,159,0,204 );  // rgbEA08
  pick[8744].Int( 223,159,31,204 );  // rgbEA28
  pick[8745].Int( 223,159,63,204 );  // rgbEA48
  pick[8746].Int( 223,159,95,204 );  // rgbEA68
  pick[8747].Int( 223,159,127,204 );  // rgbEA88
  pick[8748].Int( 223,159,159,204 );  // rgbEAA8
  pick[8749].Int( 223,159,191,204 );  // rgbEAC8
  pick[8750].Int( 223,159,223,204 );  // rgbEAE8
  pick[8751].Int( 223,191,0,204 );  // rgbEC08
  pick[8752].Int( 223,191,31,204 );  // rgbEC28
  pick[8753].Int( 223,191,63,204 );  // rgbEC48
  pick[8754].Int( 223,191,95,204 );  // rgbEC68
  pick[8755].Int( 223,191,127,204 );  // rgbEC88
  pick[8756].Int( 223,191,159,204 );  // rgbECA8
  pick[8757].Int( 223,191,191,204 );  // rgbECC8
  pick[8758].Int( 223,191,223,204 );  // rgbECE8
  pick[8759].Int( 223,223,0,204 );  // rgbEE08
  pick[8760].Int( 223,223,31,204 );  // rgbEE28
  pick[8761].Int( 223,223,63,204 );  // rgbEE48
  pick[8762].Int( 223,223,95,204 );  // rgbEE68
  pick[8763].Int( 223,223,127,204 );  // rgbEE88
  pick[8764].Int( 223,223,159,204 );  // rgbEEA8
  pick[8765].Int( 223,223,191,204 );  // rgbEEC8
  pick[8766].Int( 223,223,223,204 );  // rgbEEE8
  pick[8767].Int( 0,0,0,229 );  // rgb0009
  pick[8768].Int( 0,0,31,229 );  // rgb0029
  pick[8769].Int( 0,0,63,229 );  // rgb0049
  pick[8770].Int( 0,0,95,229 );  // rgb0069
  pick[8771].Int( 0,0,127,229 );  // rgb0089
  pick[8772].Int( 0,0,159,229 );  // rgb00A9
  pick[8773].Int( 0,0,191,229 );  // rgb00C9
  pick[8774].Int( 0,0,223,229 );  // rgb00E9
  pick[8775].Int( 0,31,0,229 );  // rgb0209
  pick[8776].Int( 0,31,31,229 );  // rgb0229
  pick[8777].Int( 0,31,63,229 );  // rgb0249
  pick[8778].Int( 0,31,95,229 );  // rgb0269
  pick[8779].Int( 0,31,127,229 );  // rgb0289
  pick[8780].Int( 0,31,159,229 );  // rgb02A9
  pick[8781].Int( 0,31,191,229 );  // rgb02C9
  pick[8782].Int( 0,31,223,229 );  // rgb02E9
  pick[8783].Int( 0,63,0,229 );  // rgb0409
  pick[8784].Int( 0,63,31,229 );  // rgb0429
  pick[8785].Int( 0,63,63,229 );  // rgb0449
  pick[8786].Int( 0,63,95,229 );  // rgb0469
  pick[8787].Int( 0,63,127,229 );  // rgb0489
  pick[8788].Int( 0,63,159,229 );  // rgb04A9
  pick[8789].Int( 0,63,191,229 );  // rgb04C9
  pick[8790].Int( 0,63,223,229 );  // rgb04E9
  pick[8791].Int( 0,95,0,229 );  // rgb0609
  pick[8792].Int( 0,95,31,229 );  // rgb0629
  pick[8793].Int( 0,95,63,229 );  // rgb0649
  pick[8794].Int( 0,95,95,229 );  // rgb0669
  pick[8795].Int( 0,95,127,229 );  // rgb0689
  pick[8796].Int( 0,95,159,229 );  // rgb06A9
  pick[8797].Int( 0,95,191,229 );  // rgb06C9
  pick[8798].Int( 0,95,223,229 );  // rgb06E9
  pick[8799].Int( 0,127,0,229 );  // rgb0809
  pick[8800].Int( 0,127,31,229 );  // rgb0829
  pick[8801].Int( 0,127,63,229 );  // rgb0849
  pick[8802].Int( 0,127,95,229 );  // rgb0869
  pick[8803].Int( 0,127,127,229 );  // rgb0889
  pick[8804].Int( 0,127,159,229 );  // rgb08A9
  pick[8805].Int( 0,127,191,229 );  // rgb08C9
  pick[8806].Int( 0,127,223,229 );  // rgb08E9
  pick[8807].Int( 0,159,0,229 );  // rgb0A09
  pick[8808].Int( 0,159,31,229 );  // rgb0A29
  pick[8809].Int( 0,159,63,229 );  // rgb0A49
  pick[8810].Int( 0,159,95,229 );  // rgb0A69
  pick[8811].Int( 0,159,127,229 );  // rgb0A89
  pick[8812].Int( 0,159,159,229 );  // rgb0AA9
  pick[8813].Int( 0,159,191,229 );  // rgb0AC9
  pick[8814].Int( 0,159,223,229 );  // rgb0AE9
  pick[8815].Int( 0,191,0,229 );  // rgb0C09
  pick[8816].Int( 0,191,31,229 );  // rgb0C29
  pick[8817].Int( 0,191,63,229 );  // rgb0C49
  pick[8818].Int( 0,191,95,229 );  // rgb0C69
  pick[8819].Int( 0,191,127,229 );  // rgb0C89
  pick[8820].Int( 0,191,159,229 );  // rgb0CA9
  pick[8821].Int( 0,191,191,229 );  // rgb0CC9
  pick[8822].Int( 0,191,223,229 );  // rgb0CE9
  pick[8823].Int( 0,223,0,229 );  // rgb0E09
  pick[8824].Int( 0,223,31,229 );  // rgb0E29
  pick[8825].Int( 0,223,63,229 );  // rgb0E49
  pick[8826].Int( 0,223,95,229 );  // rgb0E69
  pick[8827].Int( 0,223,127,229 );  // rgb0E89
  pick[8828].Int( 0,223,159,229 );  // rgb0EA9
  pick[8829].Int( 0,223,191,229 );  // rgb0EC9
  pick[8830].Int( 0,223,223,229 );  // rgb0EE9
  pick[8831].Int( 31,0,0,229 );  // rgb2009
  pick[8832].Int( 31,0,31,229 );  // rgb2029
  pick[8833].Int( 31,0,63,229 );  // rgb2049
  pick[8834].Int( 31,0,95,229 );  // rgb2069
  pick[8835].Int( 31,0,127,229 );  // rgb2089
  pick[8836].Int( 31,0,159,229 );  // rgb20A9
  pick[8837].Int( 31,0,191,229 );  // rgb20C9
  pick[8838].Int( 31,0,223,229 );  // rgb20E9
  pick[8839].Int( 31,31,0,229 );  // rgb2209
  pick[8840].Int( 31,31,31,229 );  // rgb2229
  pick[8841].Int( 31,31,63,229 );  // rgb2249
  pick[8842].Int( 31,31,95,229 );  // rgb2269
  pick[8843].Int( 31,31,127,229 );  // rgb2289
  pick[8844].Int( 31,31,159,229 );  // rgb22A9
  pick[8845].Int( 31,31,191,229 );  // rgb22C9
  pick[8846].Int( 31,31,223,229 );  // rgb22E9
  pick[8847].Int( 31,63,0,229 );  // rgb2409
  pick[8848].Int( 31,63,31,229 );  // rgb2429
  pick[8849].Int( 31,63,63,229 );  // rgb2449
  pick[8850].Int( 31,63,95,229 );  // rgb2469
  pick[8851].Int( 31,63,127,229 );  // rgb2489
  pick[8852].Int( 31,63,159,229 );  // rgb24A9
  pick[8853].Int( 31,63,191,229 );  // rgb24C9
  pick[8854].Int( 31,63,223,229 );  // rgb24E9
  pick[8855].Int( 31,95,0,229 );  // rgb2609
  pick[8856].Int( 31,95,31,229 );  // rgb2629
  pick[8857].Int( 31,95,63,229 );  // rgb2649
  pick[8858].Int( 31,95,95,229 );  // rgb2669
  pick[8859].Int( 31,95,127,229 );  // rgb2689
  pick[8860].Int( 31,95,159,229 );  // rgb26A9
  pick[8861].Int( 31,95,191,229 );  // rgb26C9
  pick[8862].Int( 31,95,223,229 );  // rgb26E9
  pick[8863].Int( 31,127,0,229 );  // rgb2809
  pick[8864].Int( 31,127,31,229 );  // rgb2829
  pick[8865].Int( 31,127,63,229 );  // rgb2849
  pick[8866].Int( 31,127,95,229 );  // rgb2869
  pick[8867].Int( 31,127,127,229 );  // rgb2889
  pick[8868].Int( 31,127,159,229 );  // rgb28A9
  pick[8869].Int( 31,127,191,229 );  // rgb28C9
  pick[8870].Int( 31,127,223,229 );  // rgb28E9
  pick[8871].Int( 31,159,0,229 );  // rgb2A09
  pick[8872].Int( 31,159,31,229 );  // rgb2A29
  pick[8873].Int( 31,159,63,229 );  // rgb2A49
  pick[8874].Int( 31,159,95,229 );  // rgb2A69
  pick[8875].Int( 31,159,127,229 );  // rgb2A89
  pick[8876].Int( 31,159,159,229 );  // rgb2AA9
  pick[8877].Int( 31,159,191,229 );  // rgb2AC9
  pick[8878].Int( 31,159,223,229 );  // rgb2AE9
  pick[8879].Int( 31,191,0,229 );  // rgb2C09
  pick[8880].Int( 31,191,31,229 );  // rgb2C29
  pick[8881].Int( 31,191,63,229 );  // rgb2C49
  pick[8882].Int( 31,191,95,229 );  // rgb2C69
  pick[8883].Int( 31,191,127,229 );  // rgb2C89
  pick[8884].Int( 31,191,159,229 );  // rgb2CA9
  pick[8885].Int( 31,191,191,229 );  // rgb2CC9
  pick[8886].Int( 31,191,223,229 );  // rgb2CE9
  pick[8887].Int( 31,223,0,229 );  // rgb2E09
  pick[8888].Int( 31,223,31,229 );  // rgb2E29
  pick[8889].Int( 31,223,63,229 );  // rgb2E49
  pick[8890].Int( 31,223,95,229 );  // rgb2E69
  pick[8891].Int( 31,223,127,229 );  // rgb2E89
  pick[8892].Int( 31,223,159,229 );  // rgb2EA9
  pick[8893].Int( 31,223,191,229 );  // rgb2EC9
  pick[8894].Int( 31,223,223,229 );  // rgb2EE9
  pick[8895].Int( 63,0,0,229 );  // rgb4009
  pick[8896].Int( 63,0,31,229 );  // rgb4029
  pick[8897].Int( 63,0,63,229 );  // rgb4049
  pick[8898].Int( 63,0,95,229 );  // rgb4069
  pick[8899].Int( 63,0,127,229 );  // rgb4089
  pick[8900].Int( 63,0,159,229 );  // rgb40A9
  pick[8901].Int( 63,0,191,229 );  // rgb40C9
  pick[8902].Int( 63,0,223,229 );  // rgb40E9
  pick[8903].Int( 63,31,0,229 );  // rgb4209
  pick[8904].Int( 63,31,31,229 );  // rgb4229
  pick[8905].Int( 63,31,63,229 );  // rgb4249
  pick[8906].Int( 63,31,95,229 );  // rgb4269
  pick[8907].Int( 63,31,127,229 );  // rgb4289
  pick[8908].Int( 63,31,159,229 );  // rgb42A9
  pick[8909].Int( 63,31,191,229 );  // rgb42C9
  pick[8910].Int( 63,31,223,229 );  // rgb42E9
  pick[8911].Int( 63,63,0,229 );  // rgb4409
  pick[8912].Int( 63,63,31,229 );  // rgb4429
  pick[8913].Int( 63,63,63,229 );  // rgb4449
  pick[8914].Int( 63,63,95,229 );  // rgb4469
  pick[8915].Int( 63,63,127,229 );  // rgb4489
  pick[8916].Int( 63,63,159,229 );  // rgb44A9
  pick[8917].Int( 63,63,191,229 );  // rgb44C9
  pick[8918].Int( 63,63,223,229 );  // rgb44E9
  pick[8919].Int( 63,95,0,229 );  // rgb4609
  pick[8920].Int( 63,95,31,229 );  // rgb4629
  pick[8921].Int( 63,95,63,229 );  // rgb4649
  pick[8922].Int( 63,95,95,229 );  // rgb4669
  pick[8923].Int( 63,95,127,229 );  // rgb4689
  pick[8924].Int( 63,95,159,229 );  // rgb46A9
  pick[8925].Int( 63,95,191,229 );  // rgb46C9
  pick[8926].Int( 63,95,223,229 );  // rgb46E9
  pick[8927].Int( 63,127,0,229 );  // rgb4809
  pick[8928].Int( 63,127,31,229 );  // rgb4829
  pick[8929].Int( 63,127,63,229 );  // rgb4849
  pick[8930].Int( 63,127,95,229 );  // rgb4869
  pick[8931].Int( 63,127,127,229 );  // rgb4889
  pick[8932].Int( 63,127,159,229 );  // rgb48A9
  pick[8933].Int( 63,127,191,229 );  // rgb48C9
  pick[8934].Int( 63,127,223,229 );  // rgb48E9
  pick[8935].Int( 63,159,0,229 );  // rgb4A09
  pick[8936].Int( 63,159,31,229 );  // rgb4A29
  pick[8937].Int( 63,159,63,229 );  // rgb4A49
  pick[8938].Int( 63,159,95,229 );  // rgb4A69
  pick[8939].Int( 63,159,127,229 );  // rgb4A89
  pick[8940].Int( 63,159,159,229 );  // rgb4AA9
  pick[8941].Int( 63,159,191,229 );  // rgb4AC9
  pick[8942].Int( 63,159,223,229 );  // rgb4AE9
  pick[8943].Int( 63,191,0,229 );  // rgb4C09
  pick[8944].Int( 63,191,31,229 );  // rgb4C29
  pick[8945].Int( 63,191,63,229 );  // rgb4C49
  pick[8946].Int( 63,191,95,229 );  // rgb4C69
  pick[8947].Int( 63,191,127,229 );  // rgb4C89
  pick[8948].Int( 63,191,159,229 );  // rgb4CA9
  pick[8949].Int( 63,191,191,229 );  // rgb4CC9
  pick[8950].Int( 63,191,223,229 );  // rgb4CE9
  pick[8951].Int( 63,223,0,229 );  // rgb4E09
  pick[8952].Int( 63,223,31,229 );  // rgb4E29
  pick[8953].Int( 63,223,63,229 );  // rgb4E49
  pick[8954].Int( 63,223,95,229 );  // rgb4E69
  pick[8955].Int( 63,223,127,229 );  // rgb4E89
  pick[8956].Int( 63,223,159,229 );  // rgb4EA9
  pick[8957].Int( 63,223,191,229 );  // rgb4EC9
  pick[8958].Int( 63,223,223,229 );  // rgb4EE9
  pick[8959].Int( 95,0,0,229 );  // rgb6009
  pick[8960].Int( 95,0,31,229 );  // rgb6029
  pick[8961].Int( 95,0,63,229 );  // rgb6049
  pick[8962].Int( 95,0,95,229 );  // rgb6069
  pick[8963].Int( 95,0,127,229 );  // rgb6089
  pick[8964].Int( 95,0,159,229 );  // rgb60A9
  pick[8965].Int( 95,0,191,229 );  // rgb60C9
  pick[8966].Int( 95,0,223,229 );  // rgb60E9
  pick[8967].Int( 95,31,0,229 );  // rgb6209
  pick[8968].Int( 95,31,31,229 );  // rgb6229
  pick[8969].Int( 95,31,63,229 );  // rgb6249
  pick[8970].Int( 95,31,95,229 );  // rgb6269
  pick[8971].Int( 95,31,127,229 );  // rgb6289
  pick[8972].Int( 95,31,159,229 );  // rgb62A9
  pick[8973].Int( 95,31,191,229 );  // rgb62C9
  pick[8974].Int( 95,31,223,229 );  // rgb62E9
  pick[8975].Int( 95,63,0,229 );  // rgb6409
  pick[8976].Int( 95,63,31,229 );  // rgb6429
  pick[8977].Int( 95,63,63,229 );  // rgb6449
  pick[8978].Int( 95,63,95,229 );  // rgb6469
  pick[8979].Int( 95,63,127,229 );  // rgb6489
  pick[8980].Int( 95,63,159,229 );  // rgb64A9
  pick[8981].Int( 95,63,191,229 );  // rgb64C9
  pick[8982].Int( 95,63,223,229 );  // rgb64E9
  pick[8983].Int( 95,95,0,229 );  // rgb6609
  pick[8984].Int( 95,95,31,229 );  // rgb6629
  pick[8985].Int( 95,95,63,229 );  // rgb6649
  pick[8986].Int( 95,95,95,229 );  // rgb6669
  pick[8987].Int( 95,95,127,229 );  // rgb6689
  pick[8988].Int( 95,95,159,229 );  // rgb66A9
  pick[8989].Int( 95,95,191,229 );  // rgb66C9
  pick[8990].Int( 95,95,223,229 );  // rgb66E9
  pick[8991].Int( 95,127,0,229 );  // rgb6809
  pick[8992].Int( 95,127,31,229 );  // rgb6829
  pick[8993].Int( 95,127,63,229 );  // rgb6849
  pick[8994].Int( 95,127,95,229 );  // rgb6869
  pick[8995].Int( 95,127,127,229 );  // rgb6889
  pick[8996].Int( 95,127,159,229 );  // rgb68A9
  pick[8997].Int( 95,127,191,229 );  // rgb68C9
  pick[8998].Int( 95,127,223,229 );  // rgb68E9
  pick[8999].Int( 95,159,0,229 );  // rgb6A09
  pick[9000].Int( 95,159,31,229 );  // rgb6A29
  pick[9001].Int( 95,159,63,229 );  // rgb6A49
  pick[9002].Int( 95,159,95,229 );  // rgb6A69
  pick[9003].Int( 95,159,127,229 );  // rgb6A89
  pick[9004].Int( 95,159,159,229 );  // rgb6AA9
  pick[9005].Int( 95,159,191,229 );  // rgb6AC9
  pick[9006].Int( 95,159,223,229 );  // rgb6AE9
  pick[9007].Int( 95,191,0,229 );  // rgb6C09
  pick[9008].Int( 95,191,31,229 );  // rgb6C29
  pick[9009].Int( 95,191,63,229 );  // rgb6C49
  pick[9010].Int( 95,191,95,229 );  // rgb6C69
  pick[9011].Int( 95,191,127,229 );  // rgb6C89
  pick[9012].Int( 95,191,159,229 );  // rgb6CA9
  pick[9013].Int( 95,191,191,229 );  // rgb6CC9
  pick[9014].Int( 95,191,223,229 );  // rgb6CE9
  pick[9015].Int( 95,223,0,229 );  // rgb6E09
  pick[9016].Int( 95,223,31,229 );  // rgb6E29
  pick[9017].Int( 95,223,63,229 );  // rgb6E49
  pick[9018].Int( 95,223,95,229 );  // rgb6E69
  pick[9019].Int( 95,223,127,229 );  // rgb6E89
  pick[9020].Int( 95,223,159,229 );  // rgb6EA9
  pick[9021].Int( 95,223,191,229 );  // rgb6EC9
  pick[9022].Int( 95,223,223,229 );  // rgb6EE9
  pick[9023].Int( 127,0,0,229 );  // rgb8009
  pick[9024].Int( 127,0,31,229 );  // rgb8029
  pick[9025].Int( 127,0,63,229 );  // rgb8049
  pick[9026].Int( 127,0,95,229 );  // rgb8069
  pick[9027].Int( 127,0,127,229 );  // rgb8089
  pick[9028].Int( 127,0,159,229 );  // rgb80A9
  pick[9029].Int( 127,0,191,229 );  // rgb80C9
  pick[9030].Int( 127,0,223,229 );  // rgb80E9
  pick[9031].Int( 127,31,0,229 );  // rgb8209
  pick[9032].Int( 127,31,31,229 );  // rgb8229
  pick[9033].Int( 127,31,63,229 );  // rgb8249
  pick[9034].Int( 127,31,95,229 );  // rgb8269
  pick[9035].Int( 127,31,127,229 );  // rgb8289
  pick[9036].Int( 127,31,159,229 );  // rgb82A9
  pick[9037].Int( 127,31,191,229 );  // rgb82C9
  pick[9038].Int( 127,31,223,229 );  // rgb82E9
  pick[9039].Int( 127,63,0,229 );  // rgb8409
  pick[9040].Int( 127,63,31,229 );  // rgb8429
  pick[9041].Int( 127,63,63,229 );  // rgb8449
  pick[9042].Int( 127,63,95,229 );  // rgb8469
  pick[9043].Int( 127,63,127,229 );  // rgb8489
  pick[9044].Int( 127,63,159,229 );  // rgb84A9
  pick[9045].Int( 127,63,191,229 );  // rgb84C9
  pick[9046].Int( 127,63,223,229 );  // rgb84E9
  pick[9047].Int( 127,95,0,229 );  // rgb8609
  pick[9048].Int( 127,95,31,229 );  // rgb8629
  pick[9049].Int( 127,95,63,229 );  // rgb8649
  pick[9050].Int( 127,95,95,229 );  // rgb8669
  pick[9051].Int( 127,95,127,229 );  // rgb8689
  pick[9052].Int( 127,95,159,229 );  // rgb86A9
  pick[9053].Int( 127,95,191,229 );  // rgb86C9
  pick[9054].Int( 127,95,223,229 );  // rgb86E9
  pick[9055].Int( 127,127,0,229 );  // rgb8809
  pick[9056].Int( 127,127,31,229 );  // rgb8829
  pick[9057].Int( 127,127,63,229 );  // rgb8849
  pick[9058].Int( 127,127,95,229 );  // rgb8869
  pick[9059].Int( 127,127,127,229 );  // rgb8889
  pick[9060].Int( 127,127,159,229 );  // rgb88A9
  pick[9061].Int( 127,127,191,229 );  // rgb88C9
  pick[9062].Int( 127,127,223,229 );  // rgb88E9
  pick[9063].Int( 127,159,0,229 );  // rgb8A09
  pick[9064].Int( 127,159,31,229 );  // rgb8A29
  pick[9065].Int( 127,159,63,229 );  // rgb8A49
  pick[9066].Int( 127,159,95,229 );  // rgb8A69
  pick[9067].Int( 127,159,127,229 );  // rgb8A89
  pick[9068].Int( 127,159,159,229 );  // rgb8AA9
  pick[9069].Int( 127,159,191,229 );  // rgb8AC9
  pick[9070].Int( 127,159,223,229 );  // rgb8AE9
  pick[9071].Int( 127,191,0,229 );  // rgb8C09
  pick[9072].Int( 127,191,31,229 );  // rgb8C29
  pick[9073].Int( 127,191,63,229 );  // rgb8C49
  pick[9074].Int( 127,191,95,229 );  // rgb8C69
  pick[9075].Int( 127,191,127,229 );  // rgb8C89
  pick[9076].Int( 127,191,159,229 );  // rgb8CA9
  pick[9077].Int( 127,191,191,229 );  // rgb8CC9
  pick[9078].Int( 127,191,223,229 );  // rgb8CE9
  pick[9079].Int( 127,223,0,229 );  // rgb8E09
  pick[9080].Int( 127,223,31,229 );  // rgb8E29
  pick[9081].Int( 127,223,63,229 );  // rgb8E49
  pick[9082].Int( 127,223,95,229 );  // rgb8E69
  pick[9083].Int( 127,223,127,229 );  // rgb8E89
  pick[9084].Int( 127,223,159,229 );  // rgb8EA9
  pick[9085].Int( 127,223,191,229 );  // rgb8EC9
  pick[9086].Int( 127,223,223,229 );  // rgb8EE9
  pick[9087].Int( 159,0,0,229 );  // rgbA009
  pick[9088].Int( 159,0,31,229 );  // rgbA029
  pick[9089].Int( 159,0,63,229 );  // rgbA049
  pick[9090].Int( 159,0,95,229 );  // rgbA069
  pick[9091].Int( 159,0,127,229 );  // rgbA089
  pick[9092].Int( 159,0,159,229 );  // rgbA0A9
  pick[9093].Int( 159,0,191,229 );  // rgbA0C9
  pick[9094].Int( 159,0,223,229 );  // rgbA0E9
  pick[9095].Int( 159,31,0,229 );  // rgbA209
  pick[9096].Int( 159,31,31,229 );  // rgbA229
  pick[9097].Int( 159,31,63,229 );  // rgbA249
  pick[9098].Int( 159,31,95,229 );  // rgbA269
  pick[9099].Int( 159,31,127,229 );  // rgbA289
  pick[9100].Int( 159,31,159,229 );  // rgbA2A9
  pick[9101].Int( 159,31,191,229 );  // rgbA2C9
  pick[9102].Int( 159,31,223,229 );  // rgbA2E9
  pick[9103].Int( 159,63,0,229 );  // rgbA409
  pick[9104].Int( 159,63,31,229 );  // rgbA429
  pick[9105].Int( 159,63,63,229 );  // rgbA449
  pick[9106].Int( 159,63,95,229 );  // rgbA469
  pick[9107].Int( 159,63,127,229 );  // rgbA489
  pick[9108].Int( 159,63,159,229 );  // rgbA4A9
  pick[9109].Int( 159,63,191,229 );  // rgbA4C9
  pick[9110].Int( 159,63,223,229 );  // rgbA4E9
  pick[9111].Int( 159,95,0,229 );  // rgbA609
  pick[9112].Int( 159,95,31,229 );  // rgbA629
  pick[9113].Int( 159,95,63,229 );  // rgbA649
  pick[9114].Int( 159,95,95,229 );  // rgbA669
  pick[9115].Int( 159,95,127,229 );  // rgbA689
  pick[9116].Int( 159,95,159,229 );  // rgbA6A9
  pick[9117].Int( 159,95,191,229 );  // rgbA6C9
  pick[9118].Int( 159,95,223,229 );  // rgbA6E9
  pick[9119].Int( 159,127,0,229 );  // rgbA809
  pick[9120].Int( 159,127,31,229 );  // rgbA829
  pick[9121].Int( 159,127,63,229 );  // rgbA849
  pick[9122].Int( 159,127,95,229 );  // rgbA869
  pick[9123].Int( 159,127,127,229 );  // rgbA889
  pick[9124].Int( 159,127,159,229 );  // rgbA8A9
  pick[9125].Int( 159,127,191,229 );  // rgbA8C9
  pick[9126].Int( 159,127,223,229 );  // rgbA8E9
  pick[9127].Int( 159,159,0,229 );  // rgbAA09
  pick[9128].Int( 159,159,31,229 );  // rgbAA29
  pick[9129].Int( 159,159,63,229 );  // rgbAA49
  pick[9130].Int( 159,159,95,229 );  // rgbAA69
  pick[9131].Int( 159,159,127,229 );  // rgbAA89
  pick[9132].Int( 159,159,159,229 );  // rgbAAA9
  pick[9133].Int( 159,159,191,229 );  // rgbAAC9
  pick[9134].Int( 159,159,223,229 );  // rgbAAE9
  pick[9135].Int( 159,191,0,229 );  // rgbAC09
  pick[9136].Int( 159,191,31,229 );  // rgbAC29
  pick[9137].Int( 159,191,63,229 );  // rgbAC49
  pick[9138].Int( 159,191,95,229 );  // rgbAC69
  pick[9139].Int( 159,191,127,229 );  // rgbAC89
  pick[9140].Int( 159,191,159,229 );  // rgbACA9
  pick[9141].Int( 159,191,191,229 );  // rgbACC9
  pick[9142].Int( 159,191,223,229 );  // rgbACE9
  pick[9143].Int( 159,223,0,229 );  // rgbAE09
  pick[9144].Int( 159,223,31,229 );  // rgbAE29
  pick[9145].Int( 159,223,63,229 );  // rgbAE49
  pick[9146].Int( 159,223,95,229 );  // rgbAE69
  pick[9147].Int( 159,223,127,229 );  // rgbAE89
  pick[9148].Int( 159,223,159,229 );  // rgbAEA9
  pick[9149].Int( 159,223,191,229 );  // rgbAEC9
  pick[9150].Int( 159,223,223,229 );  // rgbAEE9
  pick[9151].Int( 191,0,0,229 );  // rgbC009
  pick[9152].Int( 191,0,31,229 );  // rgbC029
  pick[9153].Int( 191,0,63,229 );  // rgbC049
  pick[9154].Int( 191,0,95,229 );  // rgbC069
  pick[9155].Int( 191,0,127,229 );  // rgbC089
  pick[9156].Int( 191,0,159,229 );  // rgbC0A9
  pick[9157].Int( 191,0,191,229 );  // rgbC0C9
  pick[9158].Int( 191,0,223,229 );  // rgbC0E9
  pick[9159].Int( 191,31,0,229 );  // rgbC209
  pick[9160].Int( 191,31,31,229 );  // rgbC229
  pick[9161].Int( 191,31,63,229 );  // rgbC249
  pick[9162].Int( 191,31,95,229 );  // rgbC269
  pick[9163].Int( 191,31,127,229 );  // rgbC289
  pick[9164].Int( 191,31,159,229 );  // rgbC2A9
  pick[9165].Int( 191,31,191,229 );  // rgbC2C9
  pick[9166].Int( 191,31,223,229 );  // rgbC2E9
  pick[9167].Int( 191,63,0,229 );  // rgbC409
  pick[9168].Int( 191,63,31,229 );  // rgbC429
  pick[9169].Int( 191,63,63,229 );  // rgbC449
  pick[9170].Int( 191,63,95,229 );  // rgbC469
  pick[9171].Int( 191,63,127,229 );  // rgbC489
  pick[9172].Int( 191,63,159,229 );  // rgbC4A9
  pick[9173].Int( 191,63,191,229 );  // rgbC4C9
  pick[9174].Int( 191,63,223,229 );  // rgbC4E9
  pick[9175].Int( 191,95,0,229 );  // rgbC609
  pick[9176].Int( 191,95,31,229 );  // rgbC629
  pick[9177].Int( 191,95,63,229 );  // rgbC649
  pick[9178].Int( 191,95,95,229 );  // rgbC669
  pick[9179].Int( 191,95,127,229 );  // rgbC689
  pick[9180].Int( 191,95,159,229 );  // rgbC6A9
  pick[9181].Int( 191,95,191,229 );  // rgbC6C9
  pick[9182].Int( 191,95,223,229 );  // rgbC6E9
  pick[9183].Int( 191,127,0,229 );  // rgbC809
  pick[9184].Int( 191,127,31,229 );  // rgbC829
  pick[9185].Int( 191,127,63,229 );  // rgbC849
  pick[9186].Int( 191,127,95,229 );  // rgbC869
  pick[9187].Int( 191,127,127,229 );  // rgbC889
  pick[9188].Int( 191,127,159,229 );  // rgbC8A9
  pick[9189].Int( 191,127,191,229 );  // rgbC8C9
  pick[9190].Int( 191,127,223,229 );  // rgbC8E9
  pick[9191].Int( 191,159,0,229 );  // rgbCA09
  pick[9192].Int( 191,159,31,229 );  // rgbCA29
  pick[9193].Int( 191,159,63,229 );  // rgbCA49
  pick[9194].Int( 191,159,95,229 );  // rgbCA69
  pick[9195].Int( 191,159,127,229 );  // rgbCA89
  pick[9196].Int( 191,159,159,229 );  // rgbCAA9
  pick[9197].Int( 191,159,191,229 );  // rgbCAC9
  pick[9198].Int( 191,159,223,229 );  // rgbCAE9
  pick[9199].Int( 191,191,0,229 );  // rgbCC09
  pick[9200].Int( 191,191,31,229 );  // rgbCC29
  pick[9201].Int( 191,191,63,229 );  // rgbCC49
  pick[9202].Int( 191,191,95,229 );  // rgbCC69
  pick[9203].Int( 191,191,127,229 );  // rgbCC89
  pick[9204].Int( 191,191,159,229 );  // rgbCCA9
  pick[9205].Int( 191,191,191,229 );  // rgbCCC9
  pick[9206].Int( 191,191,223,229 );  // rgbCCE9
  pick[9207].Int( 191,223,0,229 );  // rgbCE09
  pick[9208].Int( 191,223,31,229 );  // rgbCE29
  pick[9209].Int( 191,223,63,229 );  // rgbCE49
  pick[9210].Int( 191,223,95,229 );  // rgbCE69
  pick[9211].Int( 191,223,127,229 );  // rgbCE89
  pick[9212].Int( 191,223,159,229 );  // rgbCEA9
  pick[9213].Int( 191,223,191,229 );  // rgbCEC9
  pick[9214].Int( 191,223,223,229 );  // rgbCEE9
  pick[9215].Int( 223,0,0,229 );  // rgbE009
  pick[9216].Int( 223,0,31,229 );  // rgbE029
  pick[9217].Int( 223,0,63,229 );  // rgbE049
  pick[9218].Int( 223,0,95,229 );  // rgbE069
  pick[9219].Int( 223,0,127,229 );  // rgbE089
  pick[9220].Int( 223,0,159,229 );  // rgbE0A9
  pick[9221].Int( 223,0,191,229 );  // rgbE0C9
  pick[9222].Int( 223,0,223,229 );  // rgbE0E9
  pick[9223].Int( 223,31,0,229 );  // rgbE209
  pick[9224].Int( 223,31,31,229 );  // rgbE229
  pick[9225].Int( 223,31,63,229 );  // rgbE249
  pick[9226].Int( 223,31,95,229 );  // rgbE269
  pick[9227].Int( 223,31,127,229 );  // rgbE289
  pick[9228].Int( 223,31,159,229 );  // rgbE2A9
  pick[9229].Int( 223,31,191,229 );  // rgbE2C9
  pick[9230].Int( 223,31,223,229 );  // rgbE2E9
  pick[9231].Int( 223,63,0,229 );  // rgbE409
  pick[9232].Int( 223,63,31,229 );  // rgbE429
  pick[9233].Int( 223,63,63,229 );  // rgbE449
  pick[9234].Int( 223,63,95,229 );  // rgbE469
  pick[9235].Int( 223,63,127,229 );  // rgbE489
  pick[9236].Int( 223,63,159,229 );  // rgbE4A9
  pick[9237].Int( 223,63,191,229 );  // rgbE4C9
  pick[9238].Int( 223,63,223,229 );  // rgbE4E9
  pick[9239].Int( 223,95,0,229 );  // rgbE609
  pick[9240].Int( 223,95,31,229 );  // rgbE629
  pick[9241].Int( 223,95,63,229 );  // rgbE649
  pick[9242].Int( 223,95,95,229 );  // rgbE669
  pick[9243].Int( 223,95,127,229 );  // rgbE689
  pick[9244].Int( 223,95,159,229 );  // rgbE6A9
  pick[9245].Int( 223,95,191,229 );  // rgbE6C9
  pick[9246].Int( 223,95,223,229 );  // rgbE6E9
  pick[9247].Int( 223,127,0,229 );  // rgbE809
  pick[9248].Int( 223,127,31,229 );  // rgbE829
  pick[9249].Int( 223,127,63,229 );  // rgbE849
  pick[9250].Int( 223,127,95,229 );  // rgbE869
  pick[9251].Int( 223,127,127,229 );  // rgbE889
  pick[9252].Int( 223,127,159,229 );  // rgbE8A9
  pick[9253].Int( 223,127,191,229 );  // rgbE8C9
  pick[9254].Int( 223,127,223,229 );  // rgbE8E9
  pick[9255].Int( 223,159,0,229 );  // rgbEA09
  pick[9256].Int( 223,159,31,229 );  // rgbEA29
  pick[9257].Int( 223,159,63,229 );  // rgbEA49
  pick[9258].Int( 223,159,95,229 );  // rgbEA69
  pick[9259].Int( 223,159,127,229 );  // rgbEA89
  pick[9260].Int( 223,159,159,229 );  // rgbEAA9
  pick[9261].Int( 223,159,191,229 );  // rgbEAC9
  pick[9262].Int( 223,159,223,229 );  // rgbEAE9
  pick[9263].Int( 223,191,0,229 );  // rgbEC09
  pick[9264].Int( 223,191,31,229 );  // rgbEC29
  pick[9265].Int( 223,191,63,229 );  // rgbEC49
  pick[9266].Int( 223,191,95,229 );  // rgbEC69
  pick[9267].Int( 223,191,127,229 );  // rgbEC89
  pick[9268].Int( 223,191,159,229 );  // rgbECA9
  pick[9269].Int( 223,191,191,229 );  // rgbECC9
  pick[9270].Int( 223,191,223,229 );  // rgbECE9
  pick[9271].Int( 223,223,0,229 );  // rgbEE09
  pick[9272].Int( 223,223,31,229 );  // rgbEE29
  pick[9273].Int( 223,223,63,229 );  // rgbEE49
  pick[9274].Int( 223,223,95,229 );  // rgbEE69
  pick[9275].Int( 223,223,127,229 );  // rgbEE89
  pick[9276].Int( 223,223,159,229 );  // rgbEEA9
  pick[9277].Int( 223,223,191,229 );  // rgbEEC9
  pick[9278].Int( 223,223,223,229 );  // rgbEEE9

  // Vivid colors (brighter than mid-tone)
  vivid.Size(TOTAL_BRIGHTS);
  vivid[0].Int(  255, 255, 255, 255 ); 
  vivid[1].Int(  164, 255, 32, 255 );
  vivid[2].Int(  164, 164, 255, 255 );
  vivid[3].Int(  255, 255,  32, 255 );
  vivid[4].Int(  164, 255, 255, 255 );
  vivid[5].Int(  255, 164, 255, 255 );
  vivid[6].Int(  127, 255, 196, 255 );
  vivid[7].Int(  127, 255, 164, 255 );
  vivid[8].Int(  164, 255, 127, 255 );
  vivid[9].Int(  196, 255, 127, 255 );
  vivid[10].Int( 127, 196, 255, 255 );
  vivid[11].Int( 164, 127, 255, 255 );
  vivid[12].Int( 255, 127, 164, 255 );
  vivid[13].Int( 127, 196, 255, 255 );
  vivid[14].Int( 255, 127, 127, 255 );
  vivid[15].Int( 127, 196, 200, 255 );
  vivid[16].Int( 200, 127, 170, 255 );
  vivid[17].Int( 196, 127, 196, 255 );
  vivid[18].Int( 185, 107, 147, 255 );
  vivid[19].Int( 217, 164, 132, 255 );
  vivid[20].Int( 174, 171, 171, 255 );
  vivid[21].Int( 111, 153, 180, 255 );
  vivid[22].Int( 167, 161, 194, 255 );
  vivid[23].Int( 206, 192, 100, 255 );
  vivid[24].Int( 173, 127, 168, 255 );
  vivid[25].Int( 255, 193, 153, 255 );
  vivid[26].Int( 184, 133, 164, 255 );
  vivid[27].Int( 182, 184, 185, 255 );
  vivid[28].Int( 184, 139, 116, 255 );
  vivid[29].Int( 102, 236, 100, 255 );
  vivid[30].Int( 255, 185, 168, 255 );
  vivid[31].Int( 127, 139, 255, 255 );

  // Bright colors (not black)
  bright.Size(TOTAL_BRIGHTS);
  bright[0].Int(  255,   0,   0, 255 ); 
  bright[1].Int(  0,   255,   0, 255 );
  bright[2].Int(  0,     0, 255, 255 );
  bright[3].Int(  255, 255,   0, 255 );
  bright[4].Int(  0,   255, 255, 255 );
  bright[5].Int(  255,   0, 255, 255 );
  bright[6].Int(  127, 255, 196, 255 );
  bright[7].Int(  127, 255,  64, 255 );
  bright[8].Int(  64,  255, 127, 255 );
  bright[9].Int(  196, 255, 127, 255 );
  bright[10].Int( 127, 196, 255, 255 );
  bright[11].Int(  64, 127, 255, 255 );
  bright[12].Int( 255, 127,  64, 255 );
  bright[13].Int( 127, 196, 255, 255 );
  bright[14].Int( 255, 127, 127, 255 );
  bright[15].Int( 127, 196, 200, 255 );
  bright[16].Int( 200, 127, 170, 255 );
  bright[17].Int( 196, 127, 196, 255 );
  bright[18].Int(  85, 107,  47, 255 );
  bright[19].Int( 217, 164,  32, 255 );
  bright[20].Int( 174,  71,  71, 255 );
  bright[21].Int( 111, 153, 180, 255 );
  bright[22].Int( 167, 161,  94, 255 );
  bright[23].Int( 206,  92,   0, 255 );
  bright[24].Int( 173, 127, 168, 255 );
  bright[25].Int( 255, 193,  53, 255 );
  bright[26].Int(  84, 133, 164, 255 );
  bright[27].Int( 182, 184, 185, 255 );
  bright[28].Int( 184, 139,  16, 255 );
  bright[29].Int( 102, 136,   0, 255 );
  bright[30].Int( 255,  85,  68, 255 );
  bright[31].Int( 127,  39, 255, 255 );
                  
  // Initialize
  betweenFingers.Size(10);
  jam(0, 0,0,0,0);
  jam(1, 0,0,0,0);
  jam(2, 0,0,0,0);
  jam(3, 0,0,0,0);
  jam(4, 0,0,0,0);
  jam(5, 0,0,0,0);
  jam(6, 0,0,0,0);
  jam(7, 0,0,0,0);
  jam(8, 0,0,0,0);
  jam(9, 0,0,0,0);
 }