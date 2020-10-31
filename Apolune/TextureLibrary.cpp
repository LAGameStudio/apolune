#include "TextureLibrary.h"

Zp<GLImage> invis,invisiblePNG;
TextureLibrary library;

#define TL_TABLE_SIZE 100
void TextureLibrary::Init() {
 table.resize(TL_TABLE_SIZE);
 glSetup=&gl;
}
#undef TL_TABLE_SIZE

void GetLoadTextureOpts( TextureLibraryLoadTextureOptions opts, GLint *mag, GLint *min, GLint *wrapS, GLint *wrapT ) {
 switch ( opts ) {
  default:
  case tllto_Default:                    // Mips Clip
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_CLAMP_TO_EDGE;
    (*wrapT)=GL_CLAMP_TO_EDGE;
   break;
  case tllto_Nearest:                    // Clip
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_CLAMP_TO_EDGE;
    (*wrapT)=GL_CLAMP_TO_EDGE;
   break;
  case tllto_Tiling:                     // Mips Tile
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_REPEAT;
   break;
  case tllto_Mirrored:                   // Mips Mirrored
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
  case tllto_Tiling_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_REPEAT;
   break;
  case tllto_Mirrored_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
  case tllto_TilingX_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_CLAMP_TO_EDGE;
   break;
  case tllto_TilingY_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_CLAMP_TO_EDGE;
   break;
  case tllto_MirroredX_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_CLAMP_TO_EDGE;
   break;
  case tllto_MirroredY_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_CLAMP_TO_EDGE;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
  case tllto_TilingX:                    // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_CLAMP_TO_EDGE;
   break;
  case tllto_TilingY:                    // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_CLAMP_TO_EDGE;
    (*wrapT)=GL_REPEAT;
   break;
  case tllto_MirroredX:                  // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_CLAMP_TO_EDGE;
   break;
  case tllto_MirroredY:                  // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_CLAMP_TO_EDGE;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
  case tllto_TilingX_MirroredY:          // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
  case tllto_TilingY_MirroredX:          // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_REPEAT;
   break;
  case tllto_MirroredX_TilingY:          // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_REPEAT;
   break;
  case tllto_MirroredY_TilingX:          // Mips
    (*min)=GL_LINEAR_MIPMAP_LINEAR;
    (*mag)=GL_LINEAR;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
  case tllto_TilingX_MirroredY_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
  case tllto_TilingY_MirroredX_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_REPEAT;
   break;
  case tllto_MirroredX_TilingY_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_MIRRORED_REPEAT;
    (*wrapT)=GL_REPEAT;
   break;
  case tllto_MirroredY_TilingX_Nearest:
    (*min)=GL_LINEAR_MIPMAP_NEAREST;
    (*mag)=GL_NEAREST;
    (*wrapS)=GL_REPEAT;
    (*wrapT)=GL_MIRRORED_REPEAT;
   break;
 }
}

GLint magMipAndWrapOptions( string k ) {
 if ( !str_cmp((char*)k.c_str(),"nearest_mipmap_nearest") ) return GL_NEAREST_MIPMAP_NEAREST;
 if ( !str_cmp((char*)k.c_str(),"linear_mipmap_nearest") ) return GL_LINEAR_MIPMAP_NEAREST;
 if ( !str_cmp((char*)k.c_str(),"nearest_mipmap_linear") ) return GL_NEAREST_MIPMAP_LINEAR;
 if ( !str_cmp((char*)k.c_str(),"linear_mipmap_linear") ) return GL_LINEAR_MIPMAP_LINEAR;
 if ( !str_cmp((char*)k.c_str(),"clamp") ) return GL_CLAMP;
 if ( !str_cmp((char*)k.c_str(),"repeat") ) return GL_REPEAT;
 if ( !str_cmp((char*)k.c_str(),"mirrored") ) return GL_MIRRORED_REPEAT;
 OUTPUT("magMipAndWrapOptions: Invalid option `%s`, choose one of: clamp, repeat (for wrapping), nearest_mipmap_nearest, linear_mipmap_nearest, nearest_mipmap_linear, linear_mipmap_linear (for mipmapping)", (char*)k.c_str() );
 return -1;
}

string magMipAndWrapOptionsString( GLint op ) {
 switch ( op ) {
 case GL_NEAREST_MIPMAP_NEAREST: return string("nearest_mipmap_nearest");
   case GL_LINEAR_MIPMAP_LINEAR: return string("linear_mipmap_linear");
  case GL_NEAREST_MIPMAP_LINEAR: return string("linear_mipmap_linear");
  case GL_LINEAR_MIPMAP_NEAREST: return string("linear_mipmap_nearest");
                  case GL_CLAMP: return string("clamp");
                 case GL_REPEAT: return string("repeat");
        case GL_MIRRORED_REPEAT: return string ("mirrored");
 }
 return string("");
}