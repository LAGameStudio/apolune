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
#define STRICT
#include <Windows.h>
#include <unrar.h>
#include "macros.h"
#include "Strings.h"

extern string RAR_File_Buffer;
extern string RAR_Password;
int CALLBACK RARCallbackProc(UINT msg,LPARAM UserData,LPARAM P1,LPARAM P2);

enum ERR_TYPE {ERR_OPEN, ERR_READ, ERR_PROCESS};

class Derar {
public:
 string filename,password;
 bool opened,error;
 string comment;
 Strings files;
 HANDLE hArcData;
 int RHCode,PFCode;
 struct RARHeaderDataEx HeaderData;
 struct RAROpenArchiveDataEx OpenArchiveData;

 Derar() {
  error=false;
  opened=false;
  filename=string("");
  password=string("");
  comment=string("");
 }
 Derar( const char *fn ) {
  error=false;
  opened=false;
  filename=string("");
  password=string("");
  comment=string("");
  Open(fn);
 }
 ~Derar() { Close(); }
 void SetPassword( const char *pw ) {
  password=string(pw)+string("\r\n");
 }
 void OutError(int Error,int ErrType) {
//  OUTPUT("Derar::Error in RAR: ");
  switch(Error)  {
         case ERAR_NO_MEMORY: OUTPUT("Derar:\nNot enough memory"); break;
          case ERAR_BAD_DATA: OUTPUT("Derar:\n%s: archive header or data are damaged",filename.c_str()); break;
       case ERAR_BAD_ARCHIVE: OUTPUT("Derar:\n%s is not RAR archive",filename.c_str()); break;
    case ERAR_UNKNOWN_FORMAT: OUTPUT("Derar: Unknown archive format"); break;
             case ERAR_EOPEN: if (ErrType==ERR_PROCESS) // Returned by RARProcessFile.
                              OUTPUT("Derar: Volume open error"); else
                              OUTPUT("Derar:\nCannot open %s",filename.c_str()); break;
           case ERAR_ECREATE: OUTPUT("Derar: File create error"); break;
            case ERAR_ECLOSE: OUTPUT("Derar: File close error"); break;
             case ERAR_EREAD: OUTPUT("Derar: Read error"); break;
            case ERAR_EWRITE: OUTPUT("Derar: Write error"); break;
         case ERAR_SMALL_BUF: OUTPUT("Derar: Buffer for archive comment is too small, comment truncated"); break;
           case ERAR_UNKNOWN: OUTPUT("Derar: Unknown error"); break;
  case ERAR_MISSING_PASSWORD: OUTPUT("Derar: Password for encrypted file or header is not specified"); break;
  }
 }
 // Gets the archive list/comments after testing archive integrity.
 bool Open( const char *fn ) {  
  if ( opened ) Close();
  filename=string(fn);
  char CmtBuf[16384];

  RAR_Password=password;

  OUTPUT("Derar::Open() attempting to open file `%s`:\n",fn);
  ZeroMemory(&OpenArchiveData,sizeof(OpenArchiveData));
  OpenArchiveData.ArcName=(char *) filename.c_str();
  OpenArchiveData.CmtBuf=CmtBuf;
  OpenArchiveData.CmtBufSize=sizeof(CmtBuf);
  OpenArchiveData.OpenMode=RAR_OM_LIST;
  OpenArchiveData.Callback=RARCallbackProc;
  OpenArchiveData.UserData=0;
  hArcData=RAROpenArchiveEx(&OpenArchiveData);

  if (OpenArchiveData.OpenResult!=0) {
    OutError(OpenArchiveData.OpenResult,ERR_OPEN);
    error=true;
    return false;
  }

  PrintInfo(OpenArchiveData.Flags);
  if (OpenArchiveData.CmtState==1) comment=string(CmtBuf);  
  
  HeaderData.CmtBuf=NULL;
  ZeroMemory(&OpenArchiveData.Reserved,sizeof(OpenArchiveData.Reserved));

  while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0)
  {
    __int64 UnpSize=HeaderData.UnpSize+(((__int64)HeaderData.UnpSizeHigh)<<32);
    String *s=new String;
    s->s=string(HeaderData.FileName);
    s->integer=(int) UnpSize;
    if ((PFCode=RARProcessFile(hArcData,RAR_SKIP,NULL,NULL))!=0) {
      OutError(PFCode,ERR_PROCESS);
      break;
    }
  }
  OutError(RHCode,ERR_READ);
  RARCloseArchive(hArcData);
 }
 // Called after extract methods
 bool Rewind() {
  if ( !opened || error ) return false;
  static char CmtBuf[16384];
  RAR_Password=password;
  ZeroMemory(&OpenArchiveData,sizeof(OpenArchiveData));
  OpenArchiveData.ArcName=(char *) filename.c_str();
  OpenArchiveData.CmtBuf=CmtBuf;
  OpenArchiveData.CmtBufSize=sizeof(CmtBuf);
  OpenArchiveData.OpenMode=RAR_OM_EXTRACT;
  OpenArchiveData.Callback=RARCallbackProc;
  OpenArchiveData.UserData=0;
  hArcData=RAROpenArchiveEx(&OpenArchiveData);

  if (OpenArchiveData.OpenResult!=0) {
    OutError(OpenArchiveData.OpenResult,ERR_OPEN);
    error=true;
    return false;
  }

  HeaderData.CmtBuf=NULL;
  ZeroMemory(&OpenArchiveData.Reserved,sizeof(OpenArchiveData.Reserved));
  return true;
 }

 bool Extract( const char *file, string *out ) {
  if ( !opened || error ) return false;
  Rewind();
  while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0) {
   if ( !str_cmp(file,HeaderData.FileName) ) {
    PFCode=RARProcessFile(hArcData,RAR_TEST,NULL,NULL);
    if (PFCode!=0) {
     OutError(PFCode,ERR_PROCESS);
     return false;
    }
    *out=RAR_File_Buffer;
    break;
   }
  }
  if ( RHCode == ERAR_END_ARCHIVE ) {
   OUTPUT("File not found!\n");
   return false;
  }
  else
  if ( RHCode == ERAR_BAD_DATA ) {
   OUTPUT("Archive broken\n");
   return false;
  }
  return true;
 }
 
 bool Extract( Strings *filenames, Strings *output ) {
  if ( !opened || error ) return false;
  Rewind();
  while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0) {
   bool found=false;
   EACH(filenames->first,String,s) {
    if ( !str_cmp(s->s.c_str(),HeaderData.FileName) ) {
     PFCode=RARProcessFile(hArcData,RAR_TEST,NULL,NULL);
     if (PFCode!=0) {
      OutError(PFCode,ERR_PROCESS);
      return false;
     }
     output->Append(new String(RAR_File_Buffer));
     found=true;
     break;
    }
   }
   if ( !found ) {
    OUTPUT("File not found, aborting.\n");
    return false;
   }
  }
  if ( RHCode == ERAR_END_ARCHIVE ) {
   OUTPUT("File not found!\n");
   return false;
  }
  else
  if ( RHCode == ERAR_BAD_DATA ) {
   OUTPUT("Archive broken\n");
   return false;
  }
  return true;
 }
 void Close() {
  if ( !opened ) {
   opened=false;
   RARCloseArchive(hArcData);
  }
  files.Clear();
 }
 void PrintInfo( unsigned int Flags ) {
  printf("\nVolume:\t\t%s",(Flags & 1) ? "yes":"no");
  printf("\nComment:\t%s",(Flags & 2) ? "yes":"no");
  printf("\nLocked:\t\t%s",(Flags & 4) ? "yes":"no");
  printf("\nSolid:\t\t%s",(Flags & 8) ? "yes":"no");
  printf("\nNew naming:\t%s",(Flags & 16) ? "yes":"no");
  printf("\nAuthenticity:\t%s",(Flags & 32) ? "yes":"no");
  printf("\nRecovery:\t%s",(Flags & 64) ? "yes":"no");
  printf("\nEncr.headers:\t%s",(Flags & 128) ? "yes":"no");
  printf("\nFirst volume:\t%s",(Flags & 256) ? "yes":"no or older than 3.0");
 }
};