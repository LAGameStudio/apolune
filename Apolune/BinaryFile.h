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
#include "string_functions.h"
#include "macros.h"
#include "ZeroTypes.h"
#include "Indexed.h"

#include <float.h>

//#include <iostream>
//#include <fstream>

void print_fopen_err( errno_t err );

// File Handler
class BinaryFile {
public:
// ifstream file;
 FILE *fp;
 Zbool writing,end_of_file,opened;
 errno_t err;
 size_t size; // Check for result of last operation - not file size on disk, use function filesize()
 string filename;
 BinaryFile() {
  fp=NULL;
  err=0;
  size=0;
  filename=string("");
 }
 BinaryFile( const char *name, bool writing ) {
  fp=NULL; err=0; size=0; filename=string(""); 
  opened=Open(name,writing);
 }
 bool Open( const char *name, bool writing ) {
  this->writing=writing;
  size=0;
  filename=string(name);
  err = fopen_s(&fp, name, ( writing ? "wb" : "rb" ) );
  if ( !fp ) {
   OUTPUT("BinaryFile:Open(`%s`) Could not open file for %s\n", name, writing?"writing":"reading" );
   return (opened=false).value;
  }
  if ( err != 0 ) {
   OUTPUT("BinaryFile::fopen_s() reports error accessing file `%s`\n", name);
   OUTPUT("Error message was "); print_fopen_err((int)errno);
   if ( fp ) fclose(fp);
   return (opened=false);
  }
  if ( !writing ) { return (opened=isValid()); }
  return (opened=true).value;
 }
 virtual bool isValid() { return true; }
/*
 BinaryFile( char *name, bool write, bool use_ifstream ) {
  filename=string(name);
  if ( use_ifstream ) {
   if ( write ) {
    file.open(name,ios::out|ios::binary);
   } else {
    file.open(name,ios::in|ios::binary);
   }
  } else {
   err = fopen_s(&fp,name, ( write ? "wb" : "rb" ) );
   if ( err != 0 ) OUTPUT("BinaryFile::fopen_s() reports error accessing file `%s`\n", name);
  }
 }
 */
 ~BinaryFile() {
  if ( fp ) {
   clearerr(fp);
   err=fflush(fp);
   if ( err != 0 ) {
    OUTPUT("fflush() reported an error %d, ferror() reports %d, errno=%d\n", err, ferror(fp), errno );
    OUTPUT("Error message was "); print_fopen_err((int)errno);
   }
   if ( fp ) err=fclose(fp);
   if ( err != 0 ) {
    OUTPUT("fclose() reported an error %d, ferror() reports %d, errno=%d\n", err, ferror(fp), errno );
    OUTPUT("Error message was "); print_fopen_err((int)errno);
   }
  }
  /*if (file.is_open()) file.close();*/
 }
 bool is_eof() {
  /* We test for EOF this way otherwise feof returns false on the byte after final file character */
  if ( end_of_file || !opened ) return true;
  int err_no=getc(fp);
  if ( err_no == EOF ) return (end_of_file=true);
  if ( feof(fp) ) end_of_file=true;
  err_no=ungetc(err_no,fp);
  return end_of_file;
 }
 bool fileok() {
  return ( fp && (writing || !is_eof()) ); // when writing mode is exclusive (as it is in this class) then eof is irrelevant
 }
 void Error( bool reading ) {
  err=1;
  if ( reading ) {
   end_of_file=true;
   OUTPUT("BinaryFile::read() warned of unexpected EOF (or other error, ferror=%d errno=%d) encountered.\n", ferror(fp),errno);
   OUTPUT("Error message was "); print_fopen_err((int)errno);
  } else {
   OUTPUT("BinaryFile::write() warns of unexpected error.\n");
   OUTPUT("Error message was "); print_fopen_err((int)errno);
  }
  if (fp) {
   OUTPUT("Closing file.\n");
   err=fclose(fp);
   if ( err!=0 ) OUTPUT("Error message was "); print_fopen_err((int)errno);
  }
 }
 /************************************************************************************************************** Writing */
 inline bool write( size_t size, size_t length, const void *val ) {
  return Fwrite(length,val,size);
 }
 inline bool write( bool *val )           { return scrawl(*val); }
 inline bool write( const char *val )     { return scrawl(strlen(val),val); }
 inline bool write( byte *val )           { return scrawl(*val); }
 inline bool write( short *val )          { return scrawl(*val); }
 inline bool write( unsigned short *val ) { return scrawl(*val); }
 inline bool write( int *val )            { return scrawl(*val); }
 inline bool write( unsigned int *val )   { return scrawl(*val); }
 inline bool write( float *val )          { return scrawl(*val); }
 inline bool write( double *val )         { return scrawl(*val); }
 inline bool write( size_t length, bool *val )           { return Fwrite( length, val, sizeof(bool) );           }
 inline bool write( size_t length, const char *val )     { return Fwrite( length, val, sizeof(char) );           }
 inline bool write( size_t length, byte *val )           { return Fwrite( length, val, sizeof(byte) );           }
 inline bool write( size_t length, unsigned short *val ) { return Fwrite( length, val, sizeof(unsigned short) ); }
 inline bool write( size_t length, short *val )          { return Fwrite( length, val, sizeof(short) );          }
 inline bool write( size_t length, int *val )            { return Fwrite( length, val, sizeof(int) );            }
 inline bool write( size_t length, unsigned int *val )   { return Fwrite( length, val, sizeof(unsigned int) );   }
 inline bool write( size_t length, float *val )          { return Fwrite( length, val, sizeof(float) );          }
 inline bool write( size_t length, double *val )         { return Fwrite( length, val, sizeof(double) );         }
 inline bool write( Indexed<Zbyte> *values )   { unsigned int length=(unsigned int)(values->length); if ( !scrawl(length) ) return false; return Fwrite(length, values, sizeof(byte)           ); }
 inline bool write( Indexed<Zushort> *values ) { unsigned int length=(unsigned int)(values->length); if ( !scrawl(length) ) return false; return Fwrite(length, values, sizeof(unsigned short) ); }
 inline bool write( Indexed<Zuint> *values )   { unsigned int length=(unsigned int)(values->length); if ( !scrawl(length) ) return false; return Fwrite(length, values, sizeof(unsigned int)   ); }
 inline bool write( Indexed<Zint> *values )    { unsigned int length=(unsigned int)(values->length); if ( !scrawl(length) ) return false; return Fwrite(length, values, sizeof(int)            ); }
 inline bool write( Indexed<Zfloat> *values )  { unsigned int length=(unsigned int)(values->length); if ( !scrawl(length) ) return false; return Fwrite(length, values, sizeof(float)          ); }
 inline bool write( Indexed<Zdouble> *values ) { unsigned int length=(unsigned int)(values->length); if ( !scrawl(length) ) return false; return Fwrite(length, values, sizeof(double)         ); }
 inline bool write( Indexed<uint32> *values )  { unsigned int length=(unsigned int)(values->length); if ( !scrawl(length) ) return false; return Fwrite(length, values, sizeof(uint32)         ); }
 inline bool write( Zbyte *b )          { return scrawl(b->value); }
 inline bool write( Zuint *u )          { return scrawl(u->value); }
 inline bool write( Zint *i )           { return scrawl(i->value); }
 inline bool write( Zfloat *f )         { return scrawl(f->value); }
 inline bool write( Zdouble *d )        { return scrawl(d->value); }
 inline bool write( const Zbyte &b )    { return scrawl(b.value);  }
 inline bool write( const Zuint &u )    { return scrawl(u.value);  }
 inline bool write( const Zint &i )     { return scrawl(i.value);  }
 inline bool write( const Zfloat &f )   { return scrawl(f.value);  }
 inline bool write( const Zdouble &d )  { return scrawl(d.value);  }
 inline bool scrawl( const Zstring &s ) { return scrawl(s.value);  }
// inline void scrawl( size_t i )       { scrawl((uint32)i); } // declared on line 148
 inline bool scrawl( int i )          { return Fwrite( 1, &i, sizeof(int) );          }
#ifndef STEAMTYPES_H
// inline bool scrawl( uint32 i )       { return Fwrite( 1, &i, sizeof(uint32) );       }
#endif
 inline bool scrawl( unsigned int i ) { return Fwrite( 1, &i, sizeof(unsigned int) ); }
 inline bool scrawl( float i )        { return Fwrite( 1, &i, sizeof(float) );        }
 inline bool scrawl( char i )         { return Fwrite( 1, &i, sizeof(char) );         }
 inline bool scrawl( double i )       { return Fwrite( 1, &i, sizeof(double) );       }
 inline bool scrawl( size_t length, const char *s ) { return Fwrite( length, s, sizeof(char) ); }
 inline bool scrawl( const string& s ) {
  size_t length=(size_t)s.length();
  if ( !scrawl((int)length) ) return false;
  return scrawl(length,s.c_str());
 }
 inline bool scrawl( const char *s ) {
  size_t length=(size_t)strlen(s);
  if ( !scrawl((int)length) ) return false;
  return scrawl(length,s);
 }
 bool Fwrite( size_t length, const void *towrite, size_t size ) {
  if ( !fileok() ) return false;
  this->size=fwrite( towrite, size, length, fp );
  if ( this->size != length ) {
   OUTPUT("BinaryFile:Fwrite(`%s`) warns writing was truncated, tried to write %d wrote %d\n",
    filename.c_str(),(int)size,(int)this->size);
   Error(false);
   return false;
  }
  return true;
 }
 /************************************************************************************************************** Reading */
 inline bool read( string *target ) {
  if ( !fileok() ) return false;
  *target=string("");
  int to_read=0;
  if ( !read(&to_read) ) return false;
  for ( int i=0; i<to_read && (err!=1); i++ ) {
   char c;
   if ( !read(&c) ) return false;
   *target+=c;
  }
  return true;
 }
 inline bool read( Zstring *target ) {
  string temp=string("");
  if ( !read(&temp) ) return false;
  (*target)=temp;
  return true;
 }
 bool read( bool *target )                           { return Fread(1,target,sizeof(bool));                }
 bool read( char *target )                           { return Fread(1,target,sizeof(char));                }
 bool read( byte *target )                           { return Fread(1,target,sizeof(byte));                }
 bool read( short *target )                          { return Fread(1,target,sizeof(short));               }
 bool read( unsigned short *target )                 { return Fread(1,target,sizeof(unsigned short));      }
 bool read( int *target )                            { return Fread(1,target,sizeof(int));                 }
 bool read( unsigned int *target )                   { return Fread(1,target,sizeof(unsigned int));        }
/// bool read( uint32 *target )                         { return Fread(1,target,sizeof(uint32));              }
 bool read( float *target )                          { return Fread(1,target,sizeof(float));               }
 bool read( double *target )                         { return Fread(1,target,sizeof(double));              }
 bool read( char *target,   size_t length )          { return Fread(length,target,sizeof(char));           }
 bool read( byte *target,   size_t length )          { return Fread(length,target,sizeof(byte));           }
 bool read( unsigned short *target,  size_t length ) { return Fread(length,target,sizeof(unsigned short)); }
 bool read( short *target,  size_t length )          { return Fread(length,target,sizeof(short));          }
 bool read( unsigned int *target, size_t length )    { return Fread(length,target,sizeof(unsigned int));   }
/// bool read( uint32 *target, size_t length )          { return Fread(length,target,sizeof(uint32));         }
 bool read( int *target,    size_t length )          { return Fread(length,target,sizeof(int));            }
 bool read( float *target,  size_t length )          { return Fread(length,target,sizeof(float));          }
 bool read( double *target, size_t length )          { return Fread(length,target,sizeof(double));         }
 inline bool read( Indexed<Zbyte> *values )   { unsigned int length=0; if ( !read(&length) ) return false; values->Size((size_t)length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( Indexed<Zushort> *values ) { unsigned int length=0; if ( !read(&length) ) return false; values->Size((size_t)length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( Indexed<Zuint> *values )   { unsigned int length=0; if ( !read(&length) ) return false; values->Size((size_t)length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( Indexed<Zint> *values )    { unsigned int length=0; if ( !read(&length) ) return false; values->Size((size_t)length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( Indexed<Zfloat> *values )  { unsigned int length=0; if ( !read(&length) ) return false; values->Size((size_t)length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( Indexed<Zdouble> *values ) { unsigned int length=0; if ( !read(&length) ) return false; values->Size((size_t)length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( size_t length, Indexed<Zbyte> *values )   { values->Size(length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( size_t length, Indexed<Zushort> *values ) { values->Size(length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( size_t length, Indexed<Zuint> *values )   { values->Size(length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( size_t length, Indexed<Zint> *values )    { values->Size(length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( size_t length, Indexed<Zfloat> *values )  { values->Size(length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( size_t length, Indexed<Zdouble> *values ) { values->Size(length); for ( unsigned int i=0; i<length && err!=1; i++ ) { if ( !read(&((*values)[i].value)) ) return false; } return true; }
 inline bool read( Zbyte *b )   { return read(&(b->value)); }
 inline bool read( Zuint *u )   { return read(&(u->value)); }
 inline bool read( Zint *i )    { return read(&(i->value)); }
 inline bool read( Zfloat *f )  { return read(&(f->value)); }
 inline bool read( Zdouble *d ) { return read(&(d->value)); }

 bool Fread( size_t length, void *target, size_t size ) {
  if ( !fileok() ) return false;
  clearerr(fp);
  size_t total=length*size;
  size=fread_s(target, total, size, length, fp);
  if ( size != length ) {
   err=1;
   OUTPUT("BinaryFile::Fread(`%s`) warns, fread_s: %d read values did not meet the expected size of %d\n",
    (char *) filename.c_str(), (int) size, (int) length );
   if ( feof(fp) ) {
    end_of_file=true;
    OUTPUT("feof() reports that there was an EOF encountered, ferror() reports %d\n", ferror(fp) );
    OUTPUT("Error message was "); print_fopen_err((int)errno);
   }
   else {
    OUTPUT("ferror() reports %d, errno=%d\n", ferror(fp), (int) errno );
    OUTPUT("Error message was "); print_fopen_err((int)errno);
    OUTPUT("If no EOF detected, possible causes: opening a file for reading when writing or vice versa (calling read or write in the wrong place),\n actual file i/o error, a modicum of other possibilities\n");
   }
   return false;
  } else
  if ( ferror(fp) != 0 ) {
   OUTPUT("ferror() reports %d, errno=%d\n", ferror(fp), (int) errno );
   OUTPUT("Error message was "); print_fopen_err((int)errno);
   return false;
  }
  if ( end_of_file ) {
   OUTPUT("Closing file.\n");
   OUTPUT("Error message was "); print_fopen_err((int)errno);
   err=fclose(fp);
   return false;
  }
  return true;
 }
};

// the following was written for the #NEVER code below, the CLR version, crapola!!!!!
// you could use this to 'queue' data pointers to be later dumped in one blocking shot.

#include "LinkedList.h"

enum BinaryFileWriteQueueTypes {
 _nulls,
 _bytes,
 _ints,
 _uints,
 _floats,
 _doubles,
 _chars
};

class BinaryFileWriteQueued : public ListItem {
public:
 BinaryFileWriteQueueTypes type;
 byte *bytes;
 int *ints;
 unsigned int *uints;
 float *floats;
 double *doubles;
 char *chars;
 int length;
 BinaryFileWriteQueued() : ListItem() {
  bytes=null;
  ints=null;
  uints=null;
  floats=null;
  doubles=null;
  chars=null;
  length=0;
  type=_nulls;
 }
 void Bytes( byte *data, int length ) {
  this->bytes=data;
  this->length=length;
  type=_bytes;
 }
 void Ints( int *data, int length ) {
  this->ints=data;
  this->length=length;
  type=_ints;
 }
 void UnsignedInts( unsigned int *data, int length ) {
  this->uints=data;
  this->length=length;
  type=_uints;
 }
 void Floats( float *data, int length ) {
  this->floats=data;
  this->length=length;
  type=_floats;
 }
 void Doubles( double *data, int length ) {
  this->doubles=data;
  this->length=length;
  type=_doubles;
 }
 void Chars( const char *data, int length ) {
  this->chars=(char *) data;
  this->length=length;
  type=_chars;
 }
};

class BinaryFileWriteQueue : public LinkedList {
public:
 CLEARLISTRESET(BinaryFileWriteQueued);
 ~BinaryFileWriteQueue() { Clear(); }
 void Bytes( byte *data, int length ) {
  BinaryFileWriteQueued *item=new BinaryFileWriteQueued;
  item->Bytes(data,length);
  Append(item);
 }
 void Ints( int *data, int length ) {
  BinaryFileWriteQueued *item=new BinaryFileWriteQueued;
  item->Ints(data,length);
  Append(item);
 }
 void UnsignedInts( unsigned int *data, int length ) {
  BinaryFileWriteQueued *item=new BinaryFileWriteQueued;
  item->UnsignedInts(data,length);
  Append(item);
 }
 void Floats( float *data, int length ) {
  BinaryFileWriteQueued *item=new BinaryFileWriteQueued;
  item->Floats(data,length);
  Append(item);
 }
 void Doubles( double *data, int length ) {
  BinaryFileWriteQueued *item=new BinaryFileWriteQueued;
  item->Doubles(data,length);
  Append(item);
 }
 void Chars( const char *data, int length ) {
  BinaryFileWriteQueued *item=new BinaryFileWriteQueued;
  item->Chars(data,length);
  Append(item);
 }
};


#if defined(NEVER)

// We don't use this because of the FORMAT() function, because varargs are not supported by CLR,
// and guess what, we used it extensively.

// Source: http://msdn.microsoft.com/en-us/library/67czzkst.aspx
// binary_write.cpp
// compile with: /clr
#using<system.dll>
using namespace System;
using namespace System::IO;

class BinaryFile {
 BinaryFileWriteQueue queue;
 string filename;
 bool open,error;
 BinaryFile() { open=false; error=true; filename=string("tempfile"); }
 BinaryFile( char *filename ) {
  this->filename=string(filename);
  open=true;
  error=false;
 }
 void Write() { 
  FileStream^ fs;
  String^ FileName=gcnew String(filename.c_str());
  if (File::Exists(FileName)) {
   fs = gcnew FileStream(FileName, FileMode::Truncate);
  } else fs = gcnew FileStream(FileName, FileMode::Create);
  BinaryWriter^ w = gcnew BinaryWriter(fs);
  open=true;
  error=false;
  EACH(queue.first,BinaryFileWriteQueued,b) {
   for ( int i=0; !error&&i<b->length; i++ ) {
    switch ( b->type ) {
     case _bytes:
      try { w->Write(b->bytes[i]); } catch (Exception^) {
       OUTPUT("BinaryFile:Write() reports failed to write bytes to `%s`, closing.\n", (char *) filename.c_str() );
       fs->Close();
       error=true;
       open=false;
       return;
      }
      break;
     case _ints:
      try { w->Write(b->ints[i]); } catch (Exception^) {
       OUTPUT("BinaryFile:Write() reports failed to write ints to `%s`, closing.\n", (char *) filename.c_str() );
       fs->Close();
       error=true;
       open=false;
       return;
      }
      break;
     case _uints:
      try { w->Write(b->uints[i]); } catch (Exception^) {
       OUTPUT("BinaryFile:Write() reports failed to write uints to `%s`, closing.\n", (char *) filename.c_str() );
       fs->Close();
       error=true;
       open=false;
       return;
      }
      break;
     case _floats:
      try { w->Write(b->floats[i]); } catch (Exception^) {
       OUTPUT("BinaryFile:Write() reports failed to write floats to `%s`, closing.\n", (char *) filename.c_str() );
       fs->Close();
       error=true;
       open=false;
      }
      break;
     case _doubles:
      try { w->Write(b->doubles[i]); } catch (Exception^) {
       OUTPUT("BinaryFile:Write() reports failed to write doubles to `%s`, closing.\n", (char *) filename.c_str() );
       fs->Close();
       error=true;
       open=false;
       return;
      }
      break;
     case _chars:
      try { w->Write(b->chars[i]); } catch (Exception^) {
       OUTPUT("BinaryFile:Write() reports failed to write chars to `%s`, closing.\n", (char *) filename.c_str() );
       fs->Close();
       error=true;
       open=false;
       return;
      }
      break;
    }
   }
  }
  fs->Close();
  open=false;
  error=false;
  OUTPUT("BinaryFile:Write() wrote the file `%s`\n", (char *) filename.c_str() );
 }
 void Bytes( byte *data, int length ) {
  queue.Bytes(data,length);
 }
 void Ints( int *data, int length ) {
  queue.Ints(data,length);
 }
 void UnsignedInts( unsigned int *data, int length ) {
  queue.UnsignedInts(data,length);
 }
 void Floats( float *data, int length ) {
  queue.Floats(data,length);
 }
 void Doubles( double *data, int length ) {
  queue.Doubles(data,length);
 }
 void Chars( char *data, int length ) {
  queue.Chars(data,length);
 }
 ~BinaryFile() {
 }
};

#endif

// a bunch of  leftover notes that demonstrates several different ways to handle files:


 /* Uses CLR: do not use
 BinaryFile *file;
 file=new BinaryFile(filename);
 file->Bytes( set->randoms, 1000000);
 file->UnsignedInts( set->randomsui, 250000 );
 file->Ints( set->randomsi, 250000 );
 file->Floats( set->randomsf, 250000 );
 file->Floats( set->randomsui, 100000 );
 for ( int i=0; i<100; i++ ) file->Ints( set->rands[i].rands, 100 );
 file->Write();
 delete file;
 */
 /*
 HANDLE hFile = CreateFile(filename, GENERIC_READ|GENERIC_WRITE,
    0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
 SetFilePointer(hFile, GetFileSize(hFile, NULL), NULL, FILE_BEGIN);
 DWORD dwBytes=0;
 BOOL bResult = WriteFile(hFile, set, sizeof(*set), &dwBytes, NULL);
 CloseHandle(hFile);
 */
/*    
 // Source: http://msdn.microsoft.com/en-us/library/windows/desktop/bb540534(v=vs.85).aspx
 HANDLE hFile
  = CreateFile(filename,               // name of the write
               GENERIC_WRITE,          // open for writing
               0,                      // do not share
               NULL,                   // default security
               CREATE_ALWAYS,          // create new file only
               FILE_ATTRIBUTE_NORMAL,  // normal file
               NULL);                  // no attr. template
 
 if (hFile == INVALID_HANDLE_VALUE) 
 { 
     OUTPUT("Randoms::SaveRandom reports CreateFile: Terminal failure: Unable to open file `%s` for write.\n", filename);
     return;
 }

 DWORD dwBytesToWrite;
 DWORD dwBytesWritten = 0;
 
 BOOL bErrorFlag
  = WriteFile(hFile,           // open file handle
              (LPVOID) set,      // start of data to write
              dwBytesToWrite=(DWORD) sizeof(*set),  // number of bytes to write
              &dwBytesWritten, // number of bytes that were written
              NULL);            // no overlapped structure
 
 if (FALSE == bErrorFlag) {
  OUTPUT("Randoms::SaveRandom reports WriteFile: Terminal failure: Unable to write to file.\n");
 } else {
  if (dwBytesWritten != dwBytesToWrite) {
      // This is an error because a synchronous write that results in
      // success (WriteFile returns TRUE) should write all data as
      // requested. This would not necessarily be the case for
      // asynchronous writes.
   // note: this probably means disk full
      OUTPUT("Randoms::SaveRandom reports Error: dwBytesWritten != dwBytesToWrite\n");
  } else {
   OUTPUT("Wrote %d bytes.\n", (int)  dwBytesWritten);
  }
 }
 
 CloseHandle(hFile);
 */
/* FILE *pFile;
 errno_t error=fopen_s(&pFile,filename,"wb");
 if ( error != 0 || !pFile ) {
  OUTPUT("Randoms:SaveRandom() Error! Cannot write '%s'\n",filename);
  return;
 } 
 fwrite(set->randoms,sizeof(byte),1000000,pFile);
 fwrite(set->randomsui,sizeof(unsigned int),250000,pFile);
 fwrite(set->randomsi,sizeof(int),250000,pFile);
 fwrite(set->randomsf,sizeof(float),250000,pFile);
 fwrite(set->resequenced,sizeof(float),100000,pFile);
 for ( int i=0; i<100; i++ )
 fwrite(set->rands[i].rands,sizeof(int),PSEUDORAND_MAX,pFile);
 fclose(pFile); there was even one other way*/