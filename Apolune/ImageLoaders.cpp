#include "ImageLoaders.h"

#include "GLImage.h"

#if defined(WIN32)
#include <GdiPlus.h>
#include <GdiPlusBase.h>
#include <GdiPlusBitmap.h>
#include <GdiPlusImaging.h>
#include <GdiPlusimageAttributes.h>
#include <GdiPlusPixelFormats.h>

// "GDI Helper Functions"
// Source: Microsoft, Public Domain

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
 UINT  num = 0;          // number of image encoders
 UINT  size = 0;         // size of the image encoder array in bytes
 Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
 Gdiplus::GetImageEncodersSize(&num, &size);
 if(size == 0) return -1;  // Failure
 pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
 if(pImageCodecInfo == NULL) return -1;  // Failure
 GetImageEncoders(num, size, pImageCodecInfo);
 for(UINT j = 0; j < num; ++j) {
  if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 ) {  // MSVC++ 2015 Code Analyzer says: c:\users\moops\documents\visual studio 2015\projects\framework\framework\imageloaders.cpp(27): warning C6385: Reading invalid data from 'pImageCodecInfo':  the readable size is 'size' bytes, but '152' bytes may be read.
   *pClsid = pImageCodecInfo[j].Clsid;
   free(pImageCodecInfo);
   return j;  // Success
  }    
 }
 free(pImageCodecInfo);
 return -1;  // Failure
}
#endif

// PNG Format ///////////////////////////////////////////////////////////////////////////////////////

#if defined(NEVER)
#include <png.h>

 // added by Winfried szukw00@arcor.de
#ifndef png_jmpbuf
#define png_jmpbuf(png_ptr) ((png_ptr)->png_jmpbuf)
#endif

bool LoadPNG( string filename, Indexed<GLubyte> *pixels, int *w, int *h ) {
#if !defined(USE_CORONA)
 if ( !file_exists((char*)filename.c_str()) ) {
  OUTPUT("LoadPNG() could not find `%s`\n", filename.c_str());
  return false; 
 }
 png_byte header[8];
 FILE *fp;
 fopen_s(&fp,filename.c_str(), "rb");
 // 1) Read the header
 fread(header, 1, 8, fp);
 if (png_sig_cmp(header, 0, 8)) {
  OUTPUT("LoadPNG() error: %s is not a PNG.\n", filename.c_str());
  fclose(fp);
  return false;
 }
 png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
 if (!png_ptr) {
  OUTPUT("LoadPNG() error: png_create_read_struct returned 0.\n");
  fclose(fp);
  return false;
 }
 // 2) Create png info struct
 png_infop info_ptr = png_create_info_struct(png_ptr);
 if (!info_ptr) {
  OUTPUT("LoadPNG() error: png_create_info_struct returned 0.\n");
  png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
  fclose(fp);
  return false;
 }
 // 3) Create png info struct
 png_infop end_info = png_create_info_struct(png_ptr);
 if (!end_info) {
  OUTPUT("LoadPNG() error: png_create_info_struct returned 0.\n");
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
  fclose(fp);
  return false;
 }
 // 4) Code in this if statement gets called if libpng encounters an error
 if (setjmp(png_jmpbuf(png_ptr))) {
  OUTPUT("LoadPNG() error reported from libpng\n");
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  fclose(fp);
  return 0;
 }
 // 5) Init png reading
 png_init_io(png_ptr, fp);
 // 6) Let libpng know you already read the first 8 bytes
 png_set_sig_bytes(png_ptr, 8);
 // 7) Read all the info up to the image data
 png_read_info(png_ptr, info_ptr);
 // 8) Variables to pass to get info
 int bit_depth, color_type;
 png_uint_32 temp_width, temp_height;
 // 9) get info about png
 png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type, NULL, NULL, NULL);
 if (w){ *w = (int) temp_width;  }
 if (h){ *h = (int) temp_height; }
 // 10) Update the png info struct.
 png_read_update_info(png_ptr, info_ptr);
 // 11) Row size in bytes.
 int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 // glTexImage2d requires rows to be 4-byte aligned
 rowbytes += 3 - ((rowbytes-1) % 4);
 // 13) Allocate the image_data as a big block, to be given to opengl
 Indexed<png_byte> image_data;
 unsigned int size=(rowbytes * temp_height + 15); // why +15?
 image_data.Size(size);
 // 14) row_pointers is for pointing to image_data for reading the png with libpng
 Indexed<png_bytep> row_pointers;
 row_pointers.Size(temp_height);
 // 15) set the individual row_pointers to point at the correct offsets of image_data
 for (unsigned int i = 0; i < temp_height; i++) row_pointers[temp_height - 1 - i] = image_data.list + i * rowbytes;
 // 16) Read the png into image_data through row_pointers
 png_read_image(png_ptr, row_pointers.list);
 // 17) Ungodlike blit
 pixels->Size(size);
 for ( unsigned int i=0; i<size; i++ ) (*pixels)[i]=(GLubyte) image_data[i];
 // 18) Destroy, close, return
 fclose(fp);
#endif
 return true;
}
#endif

string GdiPixelFormatString( Gdiplus::PixelFormat p ) {
 switch ( p ) {
  case PixelFormatIndexed         : return string("Indexed");
  case PixelFormatGDI             : return string("GDI");
  case PixelFormatAlpha           : return string("Alpha");
  case PixelFormatPAlpha          : return string("PAlpha");
  case PixelFormatExtended        : return string("Extended");
  case PixelFormatCanonical       : return string("Canonical");
  case PixelFormatUndefined       : return string("Undefined");
//  case PixelFormatDontCare        : return string("DontCare");
  case PixelFormat1bppIndexed     : return string("1bppIndexed");
  case PixelFormat4bppIndexed     : return string("4bppIndexed");
  case PixelFormat8bppIndexed     : return string("8bppIndexed");
  case PixelFormat16bppGrayScale  : return string("16bppGray");
  case PixelFormat16bppRGB555     : return string("16bppRGB555");
  case PixelFormat16bppRGB565     : return string("16bppRGB565");
  case PixelFormat16bppARGB1555   : return string("16bppARGB1555");
  case PixelFormat24bppRGB        : return string("24bppRGB");
  case PixelFormat32bppRGB        : return string("32bppRGB");
  case PixelFormat32bppARGB       : return string("32bppARGB");
  case PixelFormat32bppPARGB      : return string("32bppPARGB");
  case PixelFormat48bppRGB        : return string("48bppRGB");
  case PixelFormat64bppARGB       : return string("64bppARGB");
  case PixelFormat64bppPARGB      : return string("64bppPARGB");
  case PixelFormat32bppCMYK       : return string("32bppCMYK");
  case PixelFormatMax             : return string("Max");
  default : return string("Unknown");
 }
}

int GdiPixelFormatSupportBPP( Gdiplus::PixelFormat p ) {
 switch ( p ) {
  case PixelFormat24bppRGB        : return 3;
  case PixelFormat32bppRGB        : return 3;
  case PixelFormat32bppARGB       : return 4;
  case PixelFormat32bppPARGB      : return 4;
  default : return -1;
 }
}

//#define DONT_USE_GETPIXEL 1

//extern HDC game_dc;

bool LoadPNG( string filename, Indexed<GLubyte> *pixels, int *w, int *h ) {
 Indexed<WCHAR> fn;
 fn.Size(filename.length()+1);
 for ( int i=0; i<(int)fn.length-1; i++ ) fn[i]=(WCHAR)filename.at(i);
 fn[(unsigned int)fn.length-1]=(WCHAR)'\0';
 Gdiplus::Bitmap* gdibm = Gdiplus::Bitmap::FromFile( fn.list );
	if ( gdibm != null && gdibm->GetLastStatus() == Gdiplus::Ok ) {
//  Gdiplus::Color pixel;
  *w=(int)gdibm->GetWidth();
  *h=(int)gdibm->GetHeight();
  int bytesPerPixel=4; // size of the pixels array
  size_t size=(*w)*(*h)*bytesPerPixel;
  Gdiplus::PixelFormat pixelFormat=gdibm->GetPixelFormat();
  if ( pixelFormat == PixelFormat24bppRGB ) {
   Gdiplus::BitmapData imageData;
   Gdiplus::Status state = gdibm->LockBits(new Gdiplus::Rect(0, 0, *w, *h), Gdiplus::ImageLockModeRead, pixelFormat, &imageData );
   if ( state == Gdiplus::OutOfMemory || state != Gdiplus::Ok ) {
    OUTPUT("LoadPNG: Error loading image: %s\n", filename.c_str() );
    return true;
   }
   pixels->Size(size); // Pixels are allocated.
   for ( int x=0; x<(*w); x++ ) for ( int y=0; y<(*h); y++ ) {
    byte* row = (byte*)imageData.Scan0 + (y * imageData.Stride); 
    int i=x * 3; // number of bits in the disc image
    GLubyte blue  = (GLubyte)(int)row[i];
    GLubyte green = (GLubyte)(int)row[i+1];
    GLubyte red   = (GLubyte)(int)row[i+2];
    pixels->list[(x+y*(*w))*bytesPerPixel+0]=(GLubyte) blue;
    pixels->list[(x+y*(*w))*bytesPerPixel+1]=(GLubyte) green; 
    pixels->list[(x+y*(*w))*bytesPerPixel+2]=(GLubyte) red;
    pixels->list[(x+y*(*w))*bytesPerPixel+3]=255;
   }
   gdibm->UnlockBits(&imageData);
  } else
  if ( pixelFormat == PixelFormat32bppARGB ) {
   Gdiplus::BitmapData imageData;
   Gdiplus::Status state = gdibm->LockBits(new Gdiplus::Rect(0, 0, *w, *h), Gdiplus::ImageLockModeRead, pixelFormat, &imageData );
   if ( state == Gdiplus::OutOfMemory || state != Gdiplus::Ok ) {
    OUTPUT("LoadPNG: Error loading image: %s\n", filename.c_str() );
    return true;
   }
   pixels->Size(size);
   for ( int x=0; x<(*w); x++ ) for ( int y=0; y<(*h); y++ ) {
    byte* row = (byte*)imageData.Scan0 + (y * imageData.Stride); 
    int i=x * 4; // number of bits in the disc image
    GLubyte blue  = (GLubyte)(int)row[i];
    GLubyte green = (GLubyte)(int)row[i+1];
    GLubyte red   = (GLubyte)(int)row[i+2];
    GLubyte alpha = (GLubyte)(int)row[i+3];
    pixels->list[(x+y*(*w))*bytesPerPixel+0]=(GLubyte) blue;
    pixels->list[(x+y*(*w))*bytesPerPixel+1]=(GLubyte) green; 
    pixels->list[(x+y*(*w))*bytesPerPixel+2]=(GLubyte) red;
    pixels->list[(x+y*(*w))*bytesPerPixel+3]=(GLubyte) alpha;
   }
   gdibm->UnlockBits(&imageData);
  } else { // Catch-all (slow sometimes blocking operation!), handles indexed, etc
   pixels->Size(size);
   for ( int x=0; x<(*w); x++ ) for ( int y=0; y<(*h); y++ ) {
    Gdiplus::Color pixel;
    gdibm->GetPixel(x,y,&pixel);
    pixels->list[(x+y*(*w))*bytesPerPixel+0]=(GLubyte) pixel.GetB();
    pixels->list[(x+y*(*w))*bytesPerPixel+1]=(GLubyte) pixel.GetG();
    pixels->list[(x+y*(*w))*bytesPerPixel+2]=(GLubyte) pixel.GetR();
    pixels->list[(x+y*(*w))*bytesPerPixel+3]=(GLubyte) pixel.GetA();
   }
  }
  delete gdibm;
  return false;
 }
 OUTPUT("LoadPNG() said there was an issue loading the file `%s`\n", filename.c_str() );
 return true;
}

bool SavePNG( string filename, int w, int h, Indexed<GLubyte> *pixels ) {
#if defined(WIN32)
 CLSID encoder;
 INT    result;
 result = GetEncoderClsid(L"image/png", &encoder);
 if(result < 0) {
  printf("The PNG encoder is not installed.  Using JPEG fallback.\n");
  result = GetEncoderClsid(L"image/jpeg", &encoder);
  if ( result < 0 ) {
   OUTPUT("Unable to save in format PNG or JPG for file %s, aborting\n", filename.c_str() );
   return false;
  }
  replaceAll(filename,string(".png"),string(".jpg"));
 }
 Gdiplus::Bitmap *myBitmap = new Gdiplus::Bitmap( w,h, PixelFormat32bppARGB );
 Gdiplus::Color color;
 for ( int x=0; x<w; x++ ) for ( int y=0; y<h; y++ ) {
  GLubyte *pixel=&(pixels->list[(x+y*w)*4]);
  GLubyte
   b=pixel[0],
   g=pixel[1],
   r=pixel[2],
   a=pixel[3];
  color.SetValue(color.MakeARGB((BYTE)a,(BYTE)r,(BYTE)g,(BYTE)b));
  myBitmap->SetPixel(x,y,color);
 }
 WCHAR *fn=ConstCharToWCHAR(filename.c_str());
 myBitmap->Save( fn, &encoder, NULL );
 delete myBitmap;
 OUTPUT("CPUImage:SavePNG(%s) Saved file\n", filename.c_str() );
#endif
 return true;
}

// JPEG Format ///////////////////////////////////////////////////////////////////////////////////////

bool LoadJPG( string filename, Indexed<GLubyte> *pixels, int *w, int *h ) {
 OUTPUT("Loading JPEG!\n");
 Indexed<WCHAR> fn;
 fn.Size(filename.length()+1);
 for ( unsigned int i=0; i<fn.length-1; i++ ) fn[i]=(WCHAR)filename.at(i);
 fn[(unsigned int)fn.length-1]=(WCHAR)'\0';
 Gdiplus::Bitmap* gdibm = Gdiplus::Bitmap::FromFile( fn.list );
	if ( gdibm != null && gdibm->GetLastStatus() == Gdiplus::Ok ) {
//  Gdiplus::Color pixel;
  *w=(int)gdibm->GetWidth();
  *h=(int)gdibm->GetHeight();
  int bytesPerPixel=4; // size of the pixels array
  size_t size=(*w)*(*h)*bytesPerPixel;
  Gdiplus::PixelFormat pixelFormat=gdibm->GetPixelFormat();
  if ( pixelFormat == PixelFormat24bppRGB ) {
   Gdiplus::BitmapData imageData;
   Gdiplus::Status state = gdibm->LockBits(new Gdiplus::Rect(0, 0, *w, *h), Gdiplus::ImageLockModeRead, pixelFormat, &imageData );
   if ( state == Gdiplus::OutOfMemory || state != Gdiplus::Ok ) {
    OUTPUT("LoadJPG: Error loading image: %s\n", filename.c_str() );
    return true;
   }
   pixels->Size(size);
   for ( int x=0; x<(*w); x++ ) for ( int y=0; y<(*h); y++ ) {
    byte* row = (byte*)imageData.Scan0 + (y * imageData.Stride); 
    int i=x * 3; // number of bits in the disc image
    GLubyte blue   = (GLubyte)(int)row[i];
    GLubyte green = (GLubyte)(int)row[i+1];
    GLubyte red  = (GLubyte)(int)row[i+2];
    pixels->list[(x+y*(*w))*bytesPerPixel+0]=(GLubyte) blue;
    pixels->list[(x+y*(*w))*bytesPerPixel+1]=(GLubyte) green; 
    pixels->list[(x+y*(*w))*bytesPerPixel+2]=(GLubyte) red;
    pixels->list[(x+y*(*w))*bytesPerPixel+3]=255;
   }
   gdibm->UnlockBits(&imageData);
  } else
  if ( pixelFormat == PixelFormat32bppARGB ) {
   Gdiplus::BitmapData imageData;
   Gdiplus::Status state = gdibm->LockBits(new Gdiplus::Rect(0, 0, *w, *h), Gdiplus::ImageLockModeRead, pixelFormat, &imageData );
   if ( state == Gdiplus::OutOfMemory || state != Gdiplus::Ok ) {
    OUTPUT("LoadJPG: Error loading image: %s\n", filename.c_str() );
    return true;
   }
   pixels->Size(size);
   for ( int x=0; x<(*w); x++ ) for ( int y=0; y<(*h); y++ ) {
    byte* row = (byte*)imageData.Scan0 + (y * imageData.Stride); 
    int i=x * 4; // number of bits in the disc image
    GLubyte blue  = (GLubyte)(int)row[i];
    GLubyte green = (GLubyte)(int)row[i+1];
    GLubyte red   = (GLubyte)(int)row[i+2];
    GLubyte alpha = (GLubyte)(int)row[i+3];
    pixels->list[(x+y*(*w))*bytesPerPixel+0]=(GLubyte) blue;
    pixels->list[(x+y*(*w))*bytesPerPixel+1]=(GLubyte) green; 
    pixels->list[(x+y*(*w))*bytesPerPixel+2]=(GLubyte) red;
    pixels->list[(x+y*(*w))*bytesPerPixel+3]=(GLubyte) alpha;
   }
   gdibm->UnlockBits(&imageData);
  } else { // Catch-all (slow sometimes blocking operation!), handles indexed, etc
   pixels->Size(size);
   for ( int x=0; x<(*w); x++ ) for ( int y=0; y<(*h); y++ ) {
    Gdiplus::Color pixel;
    gdibm->GetPixel(x,y,&pixel);
    pixels->list[(x+y*(*w))*bytesPerPixel+0]=(GLubyte) pixel.GetB();
    pixels->list[(x+y*(*w))*bytesPerPixel+1]=(GLubyte) pixel.GetG();
    pixels->list[(x+y*(*w))*bytesPerPixel+2]=(GLubyte) pixel.GetR();
    pixels->list[(x+y*(*w))*bytesPerPixel+3]=(GLubyte) pixel.GetA();
   }
  }
  delete gdibm;
  return false;
 }
 OUTPUT("LoadJPG() said there was an issue loading the file `%s`\n", filename.c_str() );
 return true;
}
