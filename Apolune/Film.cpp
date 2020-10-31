#include "Film.h"

FilmLibraries vault;

FilmLibrary *Film::GetLibrary() {
 EACH(vault.first,FilmLibrary,L) {
  if ( L->films.ListIteminList(this) ) return L;
 }
 return null;
}


void FilmLibraries::Load( const char *indexfile ) {
 FilmLibrary *films=null;
 Film *f=null;
 bool transparentize=false,keep=false,wrap=false;
 static char buf[STRING_SIZE];
 Strings file;
 file.Load(indexfile);
 EACH(file.first,String,s) {
  const char *line=s->c_str();
  line=one_argcase(line,buf);
  if ( strlen(buf) == 0 || buf[0] == '\0' ) continue;
  if ( !str_cmp(buf,"discard") ) { // Discard previously kept CPU image data
   line=one_argcase(line,buf);
   if ( strlen(buf) > 0 ) {
    GLImage *del=library.find(buf);
    if ( del ) del->DiscardCPUData();
    else OUTPUT( "FilmLibraries::Load() failed to discard unloaded image `%s`\n", buf );
   } else OUTPUT("Discard called on no image..\n");
  } else if ( !str_cmp(buf,"vault") ) { // Define or lookup a film vault
   line=one_argcase(line,buf);
   films=vault.findOrAdd(buf);
   OUTPUT( "FilmLibraries::Load() created a new vault called `%s`\n", films->title.c_str() );
  } else if ( !str_cmp(buf,"film") ) { // Define and title a new film
   if ( !films ) { // Attempting to add a new frame with no collection selected
    OUTPUT("FilmLibraries::Load() warns: attempting to add a new film with no library selected.  Adding to `Default` library\n");
    films=vault.findOrAdd("Default");
   }
   films->films.Append(f=new Film);
   line=one_argcase(line,buf);
   f->title=string(buf);
   OUTPUT( "FilmLibraries::Load() created a new film called `%s`\n", f->title.c_str() );
   line=one_argcase(line,buf); // Advance to special keyword
   if ( !str_cmp( buf, "wrap" ) ) {
    OUTPUT("Film is set to wrap for tiling.\n");
    f->wrap=true;
    wrap=true;
   } else if ( !str_cmp( buf, "transparentize" ) ) {
    OUTPUT("Film frames will be transparentized with ip_BlackBackgroundToTransparent\n");
    keep=false;
    transparentize=true;
   } else if ( !str_cmp( buf, "keep" ) ) {
    OUTPUT("Film frames will keep their CPU-side image data.\n");
    keep=true;
    transparentize=false;
   } else {
    keep=false;
    transparentize=false;
   }
  } else if ( file_exists(buf) ) { // Add a new frame
   if ( !f ) { // Attempting to add a new frame with no film selected
    OUTPUT("FilmLibraries::Load() warns: attempting to add a new frame with no film selected.  Creating new untitled standard film\n");
    if ( !films ) { // Attempting to add a new frame with no collection selected
     OUTPUT("FilmLibraries::Load() warns: attempting to add a new film with no library selected.  Adding to `Default` library\n");
     films=vault.findOrAdd("Default");
    }
    films->films.Append(f=new Film);
   }
   if ( transparentize ) {
    if ( wrap ) {
     f->frames.AddAtEnd(library.Load(library.find(buf,tllto_Tiling),&ip_BlackBackgroundToTransparent,false));
    } else {
     f->frames.AddAtEnd(library.Load(library.find(buf),&ip_BlackBackgroundToTransparent,false));
    }
   } else if ( keep ) {
    if ( wrap ) {
     f->frames.AddAtEnd(library.Load(buf,tllto_Tiling,true));
    } else {
     f->frames.AddAtEnd(library.Load(buf,tllto_Default,true));
    }
   } else {
    if ( wrap ) {
     f->frames.AddAtEnd(library.Load(buf,tllto_Tiling));
    } else {
     f->frames.AddAtEnd(library.Load(buf));
    }
   }
  } else OUTPUT("FilmLibraries::Load() warns: file not found or invalid key `%s`\n", buf );
 }
}