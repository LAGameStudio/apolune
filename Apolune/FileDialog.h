#pragma once
#include "Proce55or.h"
#include "SpecialPaths.h"

#include "FastGUI.h"
#include "GridSplitter.h"

class FileDialog : public Proce55or {
public:
 Crayon background;
 Zstring description, path;
 Strings includedLocations;  // additional paths you want to search
 Zp<FBOColorDepth> dest;
 Zp<string> bound;
 Zbool changed,activated;
 ScreenPosition screen;
 Zstring result; // Contains full path to selected element when changed is true
 GLWindowStack limbo;
 Zp<GLWindow> modal;
 Zbool only; // Only allow interaction with the modal (hide other windows)
 Zbool noFolders; // Don't show folder names in the list.
 FileDialog( const char *path ) : Proce55or() {
  this->path=path;
  Constructor();
  SetExtents(0,0,128,32);
 }
 FileDialog( const char *path, const char *description ) : Proce55or() {
  this->path=path;
  this->description=description;
  Constructor();
  SetExtents(0,0,128,32);
 }
 FileDialog() : Proce55or() {
  Constructor();
  SetExtents(0,0,128,32);
 }
 FileDialog( int x, int y, int w, int h ) : Proce55or() {
  Constructor();
  SetExtents(x,y,w,h);
 }
 FileDialog( const char *path, const char *description, int x, int y, int w, int h ) : Proce55or() {
  this->path=path;
  this->description=description;
  Constructor();
  SetExtents(x,y,w,h);
 }
 void Constructor() {
  dest=&vg.fbo1;
  activated=false;
  only=true;
 }
 void Render() {
  if ( !hidden ) {
   if ( !activated && fast.button("Pick",x,y,w,h) ) Activate();
  }
 }
 void Activate();
 void Deactivate();
};

#include "GLWindow.h"

class ModalFileDialogWindow : public GLWindow {
public:
 Zp<FileDialog> widget;
 Zp<FBOColorDepth> fbo;
 Zstring path,origin;
 Zstring location;
 Zdis<Strings> files,folders;
 Strings basenames;
 Zbool valid;
 Strings favorites;
 Zp<KeyValuePair> scrolled_to;
 Zint rows,columns;
 Zdisposable<GridSplitter> splitter;
 Zbool noFolders;
 Zint depth;
 Cartesian menu;
 Zint scrolled;
 ModalFileDialogWindow( FileDialog *widget ) : GLWindow() {
  this->widget=widget;
  this->fbo=widget->dest;
  SetPath(widget->path);
  rows=24;
  columns=5;
  noFolders=widget->noFolders;
  menu.SetRect(0,64,vg.w-64,vg.h-64);
 }
 bool SetPath( const char *path, bool keepOrigin=false ) {
  this->path = trailing_backslash(slash_to_backslash(path).c_str());
  scrolled_to=null;
  if ( !keepOrigin ) {
   this->origin = this->path;
  } else {
  }
  if ( folder_exists(path) ) {
   valid=true;
   this->location=path;
   Refresh();
   return true;
  } else {
   OUTPUT("Not a valid path: `%s`\n",path);
   return false;
  }
 }
 void OnLoad() {
  Fullscreen();
  background.fromCrayon(widget->background);
  splitter.Recycle(new GridSplitter(4,64-4,vg.w-64,vg.h-64,rows.value,columns.value,true));
 }
 void ScrollDown() {
  if ( basenames.count < rows*columns ) return;
  if ( scrolled_to ) {
   scrolled_to=(KeyValuePair *)scrolled_to->next;
   scrolled++;
  }
  if ( !scrolled_to ) {
   scrolled_to=(KeyValuePair *)basenames.first;
   scrolled==0;
  }
 }
 void ScrollUp() { // won't let you scroll past 0
  if ( basenames.count < rows*columns ) return;
  if ( !scrolled_to ) {
   scrolled_to=(KeyValuePair *)basenames.first;
   scrolled=0;
  }
  if ( scrolled_to ) {
   if ( scrolled_to->prev ) { 
    scrolled_to=(KeyValuePair *)scrolled_to->prev;
    scrolled--;
   }
  }
  if ( !scrolled_to ) {
   scrolled_to=(KeyValuePair *)basenames.first;
   scrolled=0;
  }
 }
 void Between() {
  if ( input->wheelUp ) {
   ScrollDown();
  } else if ( input->wheelDown ) {
   ScrollUp();
  }
 }
 void Render() {
  if ( visible ) {
   fast.member=null;
   art.PushViewport(fbo);
   fbo->background.fromCrayon(background);
   fbo->blend=transparency;
   fbo->tint.Pick(alabaster);
   fbo->Bind();
   KeyValuePair *label = scrolled_to ? scrolled_to : (KeyValuePair *) basenames.first;
   GridRect *rect=(GridRect *) splitter->sectors.first;
   Crayon oldTint(fast.tint);
   Crayon oldColor(fast.color);
   if ( depth > 0 ) {
    fast.tint.Pick(salmon);
    fast.color.Pick(black);
    if ( fast.button("<- Up one folder",(int)rect->X()+4,(int)rect->Y()+4,(int)rect->W()-8,(int)rect->H()-8,false) ) {
     UpFolder();
     fbo->Unbind();
     art.PopViewport();
     fast.tint=oldTint;
     fast.color=oldColor;
     return;
    }
    rect=(GridRect *)(rect->next);
   }
   if ( label ) {
    Cartesian e;
    while ( label && rect ) {
     if ( label->integer == 1 ) { // Folder, change location
      fast.tint.Pick(mauvelous);
      fast.color.Pick(darkChocolate2);
      if ( label->s.at(0) == '>' ) {
       fast.color.ScaleByAlphaf(0.5f);
      }
      if ( fast.button((string("[")+label->s+string("]")).c_str(),(int)rect->X()+4,(int)rect->Y()+4,(int)rect->W()-12,(int)rect->H()-8,false) ) {
       Enter(label->value.c_str());
       depth++;
       fbo->Unbind();
       fast.tint=oldTint;
       fast.color=oldColor;
       return;
      }
     } else { // File
      fast.tint.Pick(peachOrange);
      fast.color.Pick(darkBrown);
      if ( label->s.at(0) == '>' ) {
       fast.color.ScaleByAlphaf(0.5f);
      }
      if ( fast.button(label->c_str(),(int)rect->X()+4,(int)rect->Y()+4,(int)rect->W()-16,(int)rect->H()-8,false) ) {
       widget->changed=true;
       widget->result=label->value;
       if ( widget->bound ) (*widget->bound)=label->value;
       widget->Deactivate();
       fbo->Unbind();
       art.PopViewport();
       fast.ignoreNextRelease=true;
       fast.tint=oldTint;
       fast.color=oldColor;
       return;
      }
     }
     label=(KeyValuePair *)label->next;
     rect=(GridRect *)rect->next;
    }
   }
   fast.tint=oldTint;
   fast.color=oldColor;
   if ( fast.button("Back",fbo->w-8-32,8,32,16,false) ) {
    widget->changed=false;
    widget->Deactivate();
    fbo->Unbind();
    art.PopViewport();
    fast.ignoreNextRelease=true;
    return;
   }
   if ( basenames.count > rows*columns ) {
    int scrolling=(int)fast.vslider(true,"",(double)scrolled,0.0,basenames.count-1.0,fbo->w-8-32,8+32,32,fbo->h-32-8-16,true,true);
    if ( scrolling != scrolled ) {
     scrolled_to=(KeyValuePair *) basenames.Element(scrolling);
     if ( !scrolled_to ) {
      scrolled_to=(KeyValuePair *) basenames.first;
      scrolled=0;
     } else scrolled=scrolling;
    }
   }
   fbo->Unbind();
   art.PopViewport();
   fbo->RenderUpsideDown(0,0,display->w,display->h);
   vg.Start();
   Text(widget->description.c_str(),4.0,4.0+14.0,10.0,14.0);
   vg.End();
  }
 }
 void Select( const char *dir ) {
 }
 void Refresh() {
  OUTPUT("Entering folder: `%s`\n",location.c_str());
  Strings *strings=ls(location.c_str(),true,true,true);
  if ( strings ) files.Recycle(strings); else files.Recycle();
  strings=ls(location.c_str(),false,true,true);
  if ( strings ) folders.Recycle(strings); else folders.Recycle();
  basenames.Clear();
  EACH(folders->first,String,inode) if ( folder_exists((inode->s).c_str()) ) {
   KeyValuePair *bn=basenames.addKeyValue(basename(inode->c_str()).c_str(),inode->c_str());
   bn->integer = 1; // Folder signal
  }
  if ( widget->includedLocations.count > 0 ) EACH(widget->includedLocations.first,String,il) {
   Zdis<Strings> fi,fo;
   fo.Recycle(ls(il->s.c_str(),false,true,true));
   EACH(fo->first,String,inode) if ( folder_exists((inode->s).c_str()) ) {
    KeyValuePair *bn=basenames.addKeyValue((string("> ")+basename(inode->c_str())).c_str(),inode->c_str());
    bn->integer = 1; // Folder signal
   }
  }
  EACH(files->first,String,inode) {
   KeyValuePair *bn=basenames.addKeyValue(basename(inode->c_str()).c_str(),inode->c_str());
   bn->integer = 0; // File signal
  }
  if ( widget->includedLocations.count > 0 ) EACH(widget->includedLocations.first,String,il) {
   Zdis<Strings> fi,fo;
   fi.Recycle(ls(il->s.c_str(),true,true,true));
   EACH(fi->first,String,inode) {
    KeyValuePair *bn=basenames.addKeyValue((string("> ")+basename(inode->c_str())).c_str(),inode->c_str());
    bn->integer = 0; // File signal
   }
  }
  scrolled_to=(KeyValuePair *) (basenames.first);
  scrolled=0;
 }
 void UpFolder() {
  if ( depth == 0 ) return;
  scrolled_to=null;
  Strings pieces;
  pieces.SplitPush(location.c_str(),'\\');
  if ( pieces.count == 0 ) return;
  String *here=(String *)pieces.last;
  pieces.Remove(here);
  delete here;
  if ( Enter(pieces.Concatenate('\\').c_str()) ) {
   depth--;
  }
 }
 bool Enter( const char *folder ) {
  string candidate=trailing_backslash(slash_to_backslash(folder).c_str());
  if ( folder_exists(candidate.c_str()) ) {
   return SetPath(candidate.c_str(),true);
  } else {
   OUTPUT("Could not enter folder: ``%s`` as `%s`\n",folder,candidate.c_str());
   return false;
  }
 }
 void Favorite( const char *fullPath ) {
  if ( !favorites.match(fullPath) && folder_exists(fullPath) ) favorites.Add(fullPath);
 }
 void Unfavorite( const char *fullPath ) {
  String *favorite=favorites.match(fullPath);
  if ( favorite ) {
   favorites.Remove(favorite);
   delete favorite;
  }
 }
};

