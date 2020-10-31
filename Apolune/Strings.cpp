
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/
#include "Strings.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#pragma warning( push )
#pragma warning( disable : 4101 )

extern bool USE_SYSTEM_CURSOR_CONTROL;

// String

const char *String::c_str() {
 return this->s.c_str();
}

// Strings
#include "BinaryFile.h"

void Strings::ls_( const char *targetPath, bool listFiles, bool prependPath, bool filterOSFiles ) {
 Strings *folder=ls(targetPath,listFiles,prependPath,filterOSFiles);
 if ( folder ) {
  Concat(folder);
  delete folder;
 }
}

void Strings::Load( const char *filename )
{
 Clear();
 BinaryFile *bf=new BinaryFile(filename,false);
 char ch='\0';
 Zstring line;
 while ( !bf->is_eof() ) {
  if ( !bf->read(&ch) ) break;
  if ( ch == '\r' ) continue;
  if ( ch == '\n' ) {
   String *t=null;
   NEW(t,String(line.c_str()));
   Append(t);
   line.Clear();
   continue;
  }
  line+=ch;
 }
 delete bf;
 if ( line.length > 0 ) {
  String *t=null;
  NEW(t,String(line.c_str()));
 }
}

void Strings::Save( const char *filename )
{
 Clear();
 FILE *f;
 fopen_s(&f,filename, "w");
 if(f)
 {
  fclose(f);
  std::fstream filestr;
  filestr.open(filename, std::fstream::in | std::fstream::out );
  FOREACH(String,s) filestr<<s->s.c_str()<<endl;
  filestr.close();
 }  
 else OUTPUT("File not writeable: %s\n", filename);
}

bool Strings::inList( string s ) {
 FOREACH(String,sL) if ( s == sL->s ) return true;
 return false;
}

String *Strings::matchPrefix( const char *s ) {
 FOREACH(String,sL) if ( sL->s.length() > 0 && strlen(s) > 0 && !str_prefix(sL->c_str(),s) ) return sL;
 return null;
}

void Strings::matchesPrefix( const char *s, Strings *out ) {
 FOREACH(String,sL) {
  if ( sL->s.length() > 0 && strlen(s) > 0 && !str_prefix(sL->c_str(),s) ) {
   out->Push(sL->s.c_str());
  }
 }
}

String *Strings::match( const char *s ) {
 FOREACH(String,sL) if ( !str_cmp(sL->c_str(),s) ) return sL;
 return null;
}

String *Strings::find( string s ) {
 FOREACH(String,sL) if ( s == sL->s ) return sL; 
 return null;
}

String *Strings::find( const char *s ) {
 string f=string(s);
 FOREACH(String,sL) if ( f == sL->s ) return sL; 
 return null;
}

String *Strings::findCaseless( const char *s ) {
 string f=string(s);
 FOREACH(String,sL) if ( !str_cmp(s, sL->s.c_str() ) ) return sL; 
 return null;
}

String *Strings::findCaseless( string s ) {
 string f=string(s);
 FOREACH(String,sL) if ( !str_cmp((char *) s.c_str(), (char *) sL->s.c_str() ) ) return sL; 
 return null;
}

 String *Strings::findSubstring( const char *s ) {
  FOREACH(String,sL) if ( !str_prefix(sL->s.c_str(),s) ) return sL; 
  return null;
 }

 String *Strings::findNextSubstring( String *start, const char *s ) {
  String *sL=start;
  while ( sL && string_infix(s,sL->s) ) sL=(String *) sL->next;
  return sL;
 }

 string Strings::concatToNextSubstring( String *start, const char *end ) {
  String *e = this->findNextSubstring(start,end);
  string x=string("");
  String *sL=start;
  while ( sL && sL != e ) { x+=sL->s+string("\n"); sL=(String *) sL->next; }
  return x;
 }

// Returns -1 if not in list
int Strings::IndexOf( String *s ) {
 int i=0;
 FOREACH(String,sL) {
  if ( s == sL ) return i;
  else i++;
 }
 return -1;
}

// Returns duplicated concatenations of Strings
string Strings::Concatenate( char seperator ) {
 string e = string("");
 FOREACH(String,sL) {
  e=e+sL->s;
  e.append( 1, seperator );
 }
 return e;
}

// Returns duplicated concatenations of Strings
string Strings::Concatenate( having Seamless ) {
 string e = string("");
 FOREACH(String,sL) {
  e=e+sL->s;
 }
 return e;
}

void Strings::appendFromString( const char *s, char c ) {
 static char temp[STRING_SIZE];
 const char *p= s;
 int i=0;
 while ( *p != '\0' ) {
  if ( *p==c ) {
   temp[i]='\0';
   String *t; NEW(t,String(temp));
   if (!t) {
    OUTPUT("appendFromString: error on allocation, Could not allocate new string.\n");
    return;
   }
   Append(t);
   i=0;
  } else
  temp[i++] =*p;
  p++;
 }
}

void Strings::appendFromString( string s, char c ) {
 appendFromString( (char *) s.c_str(), c );
}

void Strings::show() 
{ 
 FOREACH(String,s) OUTPUT( "%s\n", s->s.c_str() ); 
}

void Strings::show( const char *sep, const char *end ) 
{ 
 FOREACH(String,s) OUTPUT( "%s%s", s->s.c_str(), (s->next ? sep : "") );
 OUTPUT("%s",end);
}

// Splits using string_argument
void Strings::SplitPush( const char *s ) {
 const char *p=s;
 while ( *p != '\0' ) {
  string t=string("");
  p=string_argument(p,&t);
  if ( t.length() > 0 ) Push(t);
 }
}


// Splits using string_argument_case
void Strings::SplitPushCase( const char *s ) {
 const char *p=s;
 while ( *p != '\0' ) {
  string t=string("");
  p=string_argument_case(p,&t);
  if ( t.length() > 0 ) Push(t);
 }
}

// Splits using string_argument_case_ignorequotes, saves " = , ' for writing.
void Strings::SplitPushCaseIgnoreQuotes( const char *s ) {
 const char *p=s;
 while ( *p != '\0' ) {
  string t=string("");
  p=string_argument_case_ignorequotes(p,&t);
  if ( t.length() > 0 ) Push(t);
 }
}


// Splits using string_argument_case
void Strings::SplitPushCase( having keepQuotes, const char *s ) {
 const char *p=s;
 while ( *p != '\0' ) {
  string t=string("");
  p=string_argument_case(true,p,&t);
  if ( t.length() > 0 ) Push(t);
 }
}

// Splits by char_in(), like char_isof, default is newline/space
void Strings::SplitPush( const char *s, const char *delimiterList ) {
 const char *p=s;
 Zstring t;
 while ( *p != '\0' ) {
  if ( char_in(*p,delimiterList) ) if ( t.length > 0 ) { Add(t.c_str()); t=""; }
  else t+=(*p);
  p++;
 }
}

void Strings::SplitPush( string s, char seperator ) {
 string t=string("");
 static char buf[2];
 buf[0]=seperator;
 buf[1]='\0';
 const char *p=s.c_str();
 while ( *(p=gather_to(p,buf,&t)) != '\0' ) Push(t);
 if ( t.length() > 0 ) Push(t);
}



void Strings::SplitPush( const char *s, char seperator ) {
 string t=string("");
 static char buf[2];
 buf[0]=seperator;
 buf[1]='\0';
 const char *p=s;
 while ( *(p=gather_to(p,buf,&t)) != '\0' ) Push(t);
 if ( t.length() > 0 ) Push(t);
}


void Strings::SplitBy( const char *source, const char *delim ) {
 string t=string("");
 static char buf[2];
 const char *p=source;
 while ( *(p=gather_to(p,delim,&t)) != '\0' ) Push(t);
 if ( t.length() > 0 ) Push(t);
}


// Splits using string_argument
void Strings::ScriptPush( const char *s ) { SplitPush(s); }

// Splits using FSstring_argument()
void Strings::FScriptPush( const char *s ) {
 const char *p=s;
 Zstring accum;
 bool hit_spacer_eol=false;
 while ( *p != '\0' ) {
  Zstring t;
  p=FSstring_argument(p,&hit_spacer_eol,&t.value);
  t.rewind();
  if ( t.length > 0 ) {
   if ( // contains any character from _FILLER()
       t.contains(' ')  || t.contains('=')
    || t.contains('\r') || t.contains(',')
    || t.contains('\t') || t.contains('\n') ) {
    accum+=string(" {")+t+string("} ");
   } else {
    if ( accum.length > 0 ) accum+=string(" ");
    accum+=t;
   }
  }
  if ( hit_spacer_eol && accum.length > 0 ) {
   Push(accum.c_str());
   accum="";
  }
 }
 if ( accum.length > 0 ) Push(accum.c_str());
}

void Strings::Push( string s ) {
 String *t; NEW(t,String(s));
 if (!t) {
  OUTPUT("Push: error on allocation, Could not allocate new string.\n");
  return;
 }
 Append(t);
}

void Strings::Push( const char *s ) {
 String *t; NEW(t,String(s));
 if (!t) {
  OUTPUT("Push: error on allocation, Could not allocate new string.\n");
  return;
 }
 Append(t);
}

String *Strings::PushAndGet( const char *s ) {
 String *x; NEW(x,String(string(s)));
 if (!x) {
  OUTPUT("PushAndGet: error on allocation, Could not allocate new string.\n");
  return null;
 }
 Append(x);
 return x;
}

void Strings::DuplicateAndPush( const char *s ) { // No longer different
 String *t; NEW(t,String(s));
 if (!t) {
  OUTPUT("DuplicateAndPush: error on allocation, Could not allocate new string.\n");
  return;
 }
 Append( t );
}

#include <Windows.h> // for ls()
#include <tchar.h> // for ls()

// List directory contents
// http://msdn.microsoft.com/en-us/library/aa365200%28VS.85%29.aspx
// When listFiles parameter is false, find only subfolder names.
Strings *ls( const char *targetPath, bool listFiles, bool prependPath, bool filterOSFiles ) {
 WIN32_FIND_DATA ffd;
 TCHAR szDir[MAX_PATH];
 size_t length_of_arg;
 HANDLE hFind=INVALID_HANDLE_VALUE;
 DWORD dwError=0;
 string endsWithSlash=trailing_backslash(slash_to_backslash(targetPath).c_str());

 // Check that the input path plus 3 is not longer than MAX_PATH.
 // Three characters are for the "\*" plus NULL appended below.
 length_of_arg = strlen(targetPath);
 if (length_of_arg > (MAX_PATH - 3)) {
  OUTPUT("Strings.h ls() reports directory path is too long.\n");
  return null;
 }

 // Prepare string for use with FindFile functions.  First, copy the
 // string to a buffer, then append '\*' to the directory name.
 char path[MAX_PATH+1];
 FORMAT(path,MAX_PATH+1,"%s*",endsWithSlash.c_str());
 for ( int i=0; i<MAX_PATH; i++ ) szDir[i]=(TCHAR) path[i];

 // Find the first file in the directory.
 hFind = FindFirstFile(szDir, &ffd);

 if (INVALID_HANDLE_VALUE == hFind) {
  OUTPUT("String.h ls() reported error: Error was %d\n", (int) dwError );
  return null;
 }

 Strings *list; NEW(list,Strings);
   
 // List all the files in the directory with some info about them.
 do {
  bool isFolder=true && (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
  char buf[260];
  FORMAT(buf,260,"%s",ffd.cFileName);
  if ( filterOSFiles ) {
   if ( !str_cmp(buf,".") ) continue;
   if ( !str_cmp(buf,"..") ) continue;
   if ( !str_cmp(buf,"Thumbs.db") ) continue;
  }
  if (!listFiles && isFolder) {
   if ( prependPath ) {
    char buf2[260+1024];
    FORMAT(buf2,260+1024,"%s%s",endsWithSlash.c_str(),buf);
    String *n; NEW(n,String(buf2));
    list->Append(n);
   } else {
    String *n; NEW(n,String(buf));
    if (!n) {
     OUTPUT("ls: error on allocation, Could not allocate new string.\n");
     return null;
    }
    list->Append(n);
   }
  } else if ( !isFolder ) {
   if ( prependPath  ) {
    char buf2[260+1024];
    FORMAT(buf2,260+1024,"%s%s",endsWithSlash.c_str(),buf);
    String *n; NEW(n,String(buf2));
    if (!n) {
     OUTPUT("ls: error on allocation, Could not allocate new string.\n");
     return null;
    }
    list->Append(n);
   } else {
    String *n; NEW(n,String(buf));
    if (!n) {
     OUTPUT("ls: error on allocation, Could not allocate new string.\n");
     return null;
    }
    list->Append(n);
   }
  }
 }
 while (FindNextFile(hFind, &ffd) != 0); 
 dwError = GetLastError();
 if (dwError != ERROR_NO_MORE_FILES) OUTPUT("Strings.h ls() reported an unexpected error (code %d).\n", (int) dwError );
 FindClose(hFind);
 return list;
}

Strings *ls( const char *targetPath, bool listFiles, bool prependPath, Strings *filters ) {
 WIN32_FIND_DATA ffd;
 size_t length_of_arg;
 HANDLE hFind=INVALID_HANDLE_VALUE;
 DWORD dwError=0;
 
 string endsWithSlash=trailing_backslash(slash_to_backslash(targetPath).c_str());

 // Check that the input path plus 3 is not longer than MAX_PATH.
 // Three characters are for the "*" plus NULL appended below.
 length_of_arg = strlen(targetPath);
 if (length_of_arg > (MAX_PATH - 3)) {
  OUTPUT("Strings.h ls() reports directory path is too long.\n");
  return null;
 }

 // Prepare string for use with FindFile functions.  First, copy the
 // string to a buffer, then append '*' to the directory name.
 char path[MAX_PATH+1];
 FORMAT(path,MAX_PATH+1,"%s*",endsWithSlash.c_str());
 TCHAR szDir[MAX_PATH];
 for ( int i=0; i<MAX_PATH; i++ ) szDir[i]=(TCHAR) path[i];

 // Find the first file in the directory.
 hFind = FindFirstFile(szDir, &ffd);

 if (INVALID_HANDLE_VALUE == hFind) {
  OUTPUT("String.h ls() reported error: Error was %d\n", (int) dwError );
  return null;
 }

 Strings *list; NEW(list,Strings);
 if (!list) {
  OUTPUT("ls: could not allocate list.\n");
  return null;
 }
   
 // List all the files in the directory with some info about them.
 do {
  bool isFolder=true && (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
  char buf[260];
  FORMAT(buf,260,"%s",ffd.cFileName);
  if ( filters ) {
   bool found=false;
   EACH(filters->first,String,filter) if ( !str_infix(buf,filter->c_str()) ) found=true;
   if ( found ) continue;
  }
  if (!listFiles && isFolder) {
   if ( prependPath ) {
    static char buf2[260+1024];
    FORMAT(buf2,260+1024,"%s%s",endsWithSlash.c_str(),buf);
    String *n; NEW(n,String(buf2));
    if (!list) {
     OUTPUT("ls: could not allocate list.\n");
     return null;
    }
    list->Append(n);
   } else {
    String *n; NEW(n,String(buf));
    if (!n) {
     OUTPUT("ls: could not allocate string.\n");
     return null;
    }
    list->Append(n);
   }
  } else if ( !isFolder ) {
   if ( prependPath  ) {
    static char buf2[260+1024];
    FORMAT(buf2,260+1024,"%s/%s",targetPath,buf);
    String *n; NEW(n,String(buf2));
    if (!n) {
     OUTPUT("ls: could not allocate string.\n");
     return null;
    }
    list->Append(n);
   } else {
    String *n; NEW(n,String(buf));
    if (!n) {
     OUTPUT("ls: could not allocate string.\n");
     return null;
    }
    list->Append(n);
   }
  }
 }
 while (FindNextFile(hFind, &ffd) != 0); 
 dwError = GetLastError();
 if (dwError != ERROR_NO_MORE_FILES) OUTPUT("Strings.h ls() reported an unexpected error (code %d).\n", (int) dwError );
 FindClose(hFind);
 return list;
}

#include <ShlObj.h>
string dialog_get_folder( const char *msg1, const char *msg2, bool newFolderButton ) {
 string txt=string(msg1)+string("\n")+string(msg2);
 ShowCursor(TRUE);
 TCHAR szBuffer[MAX_PATH] = {0};
 BROWSEINFO bi;
 ZeroMemory(&bi,sizeof(BROWSEINFO));
 bi.hwndOwner = NULL;
 bi.pszDisplayName = szBuffer;
 bi.lpszTitle = _T(txt.c_str());
 bi.ulFlags = BIF_RETURNONLYFSDIRS |  BIF_NEWDIALOGSTYLE;
 if ( !newFolderButton ) bi.ulFlags |= BIF_NONEWFOLDERBUTTON;
 LPITEMIDLIST idl = SHBrowseForFolder(&bi);
 if (NULL == idl) return string("");
 SHGetPathFromIDList(idl,szBuffer);
 if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
 OUTPUT("PICKED: `%s`\n",string(szBuffer).c_str());
 return string(szBuffer);
}

string dialog_get_file_or_folder( const char *msg1, const char *msg2, bool newFolderButton ) {
 string txt=string(msg1)+string("\n")+string(msg2);
 ShowCursor(TRUE);
 TCHAR szBuffer[MAX_PATH] = {0};
 BROWSEINFO bi;
 ZeroMemory(&bi,sizeof(BROWSEINFO));
 bi.hwndOwner = NULL;
 bi.pszDisplayName = szBuffer;
 bi.lpszTitle = _T(txt.c_str());
 bi.ulFlags = BIF_BROWSEINCLUDEFILES | BIF_NEWDIALOGSTYLE;
 if ( !newFolderButton ) bi.ulFlags |= BIF_NONEWFOLDERBUTTON;
 LPITEMIDLIST idl = SHBrowseForFolder(&bi);
 if (NULL == idl) return string("");
 SHGetPathFromIDList(idl,szBuffer);
 if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
 OUTPUT("PICKED: `%s`\n",string(szBuffer).c_str());
 return string(szBuffer);
}

string dialog_open( const char *msg1, const char *msg2, const char *allow ) {
 char allowables[STRING_SIZE];
 const char *p=allow;
 char *q=allowables;
 while (*p != '\0') {
  if ( *p == '\n' ) (*q) = '\0';
  else (*q) = (*p);
  q++;
  p++;
 }
 *q='\0'; q++; *q='\0'; // Double null terminated.
 Strings pieces; pieces.SplitPush("\n",allow);
 ShowCursor(TRUE);
 string txt=string(msg1)+string("\n")+string(msg2);
 OPENFILENAME ofn = { sizeof ofn };
 TCHAR buffer[STRING_SIZE];
 buffer[0] = '\0';
 ofn.lpstrFilter = allowables;
 ofn.lpstrFile = buffer;
 ofn.nMaxFile = STRING_SIZE;
 ofn.Flags =  OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
 GetOpenFileName(&ofn);
 OUTPUT("PICKED: `%s`\n",string(buffer).c_str());
 string path = string(buffer);
 if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
 return path;
}

string dialog_saveas( const char *msg1, const char *msg2 ) {
 ShowCursor(TRUE);
 string txt=string(msg1)+string("\n")+string(msg2);
 OPENFILENAME ofn = { sizeof ofn };
 TCHAR buffer[STRING_SIZE];
 buffer[0] = '\0';
 ofn.lpstrFile = buffer;
 ofn.nMaxFile = STRING_SIZE;
 ofn.Flags =  OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
 GetSaveFileName(&ofn);
 OUTPUT("PICKED: `%s`\n",string(buffer).c_str());
 string path = string(buffer);
 if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
 return path;
}


string dialog_multiselect( Strings *out, const char *msg1, const char *msg2 ) {
 ShowCursor(TRUE);
 string txt=string(msg1)+string("\n")+string(msg2);
 OPENFILENAME ofn = { sizeof ofn };
 TCHAR buffer[STRING_SIZE];
 buffer[0] = '\0';
 ofn.lpstrFile = buffer;
 ofn.nMaxFile = STRING_SIZE;
 ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER;
 GetOpenFileName(&ofn);
 OUTPUT("PICKED: `%s`\n",string(buffer).c_str());
 TCHAR* str = ofn.lpstrFile;
 string directory = str;
 str += ( directory.length() + 1 );
 string path = string(buffer);
 int multifilesPicked=0;
 while ( *str ) {
   multifilesPicked++;
   string filename = str;
   str += ( filename.length() + 1 );
   out->Add(path+string("/")+filename);
 }
 if ( multifilesPicked == 0 ) out->Add(path.c_str());
 if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
 return path;
}

void select_in_explorer( const char *s, Strings *selected ) {
 if ( !selected || ( selected && selected->count == 0 ) ) {
  ITEMIDLIST *pidl = ILCreateFromPath(s);
  if(pidl) {
   SHOpenFolderAndSelectItems(pidl,0,0,0);
   ILFree(pidl);
  }
 } else {
  ITEMIDLIST *pidl = ILCreateFromPath(s);
  if(pidl) {
   SHOpenFolderAndSelectItems(pidl,0,0,0);
   ILFree(pidl);
  }
 }
}

void open_explorer( const char *s ) {
 ShellExecute(NULL, "open", s, NULL, NULL, SW_SHOWDEFAULT);
}

#include <strsafe.h>

void dialog_last_error( const char *premsg ) {
 ShowCursor(TRUE);
 LPVOID lpMsgBuf;
 LPVOID lpDisplayBuf;
 DWORD dw=GetLastError();
 FormatMessage(
  FORMAT_MESSAGE_ALLOCATE_BUFFER | 
  FORMAT_MESSAGE_FROM_SYSTEM |
  FORMAT_MESSAGE_IGNORE_INSERTS,
  NULL,
  dw,
  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
  (LPTSTR) &lpMsgBuf,
  0, NULL 
 );  
 lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + strlen(premsg) + 80) * sizeof(TCHAR)); 
 StringCchPrintf((LPTSTR)lpDisplayBuf, 
  LocalSize(lpDisplayBuf) / sizeof(TCHAR),
  TEXT("%s failed with error %d: %s"), 
  premsg, dw, lpMsgBuf
 ); 
 MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
 LocalFree(lpMsgBuf);
 LocalFree(lpDisplayBuf);
 if ( !USE_SYSTEM_CURSOR_CONTROL ) ShowCursor(FALSE);
}

#pragma warning( pop )