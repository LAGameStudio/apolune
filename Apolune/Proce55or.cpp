
/***********************************************************************************************
 * Herbian Art and Animation Engine ("HAAE")                                                   *
 ***********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to   *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III   *
 ***********************************************************************************************
 * Copyright (c) 2010 Herbert Elwood Gilliland III ; All rights reserved.                      *
 ***********************************************************************************************/
#include "Proce55or.h"

#include "Presentation.h"
#include "ZonedProce55ors.h"

void Proce55or::Constructor() {
 unRezoned=true;
}

void Proce55or::Translate( ScreenPosition *screen ) {
 translated.Width(w.value);
 translated.Height(h.value);
 translated.PositionX(x.value);
 translated.PositionY(y.value);
 translated.MoveBy(-screen->dx,-screen->dy);
}

bool Proce55or::OnScreenTranslated() {
 return ( translated.x >= -w && translated.x2 <= display.w+w ) && ( translated.y >= -h && translated.y2 <= display.h+h );
}


bool Proce55or::within( int mx, int my ) { return ( mx > x && my > y && mx < (x2) && my < (y2) ); }

bool Proce55or::Receive( Proce55or *sender, int code, void *message ) { return true; }

void Proce55or::Redraw() { Render(); }
void Proce55or::Render() {
 if ( presentationParent ) {
  Slide *s=presentationParent->current;
  if ( presentationParent->expired >= delay && presentationParent->expired < delayplusduration )
   Draw(this->parent);
 } //else Render();
}
void Proce55or::Redraw(GLWindow *target) {
 Draw(target);
}
void Proce55or::Render(GLWindow *target) {
 Draw(target);
}
void Proce55or::OnReset( bool paused )    {
 delayplusduration=delay+duration;
 Reset(paused);
}

void Proce55or::BetweenFrames() {
 if ( presentationParent ) {
  Slide *s=presentationParent->current;
  if ( presentationParent->expired >= delay && presentationParent->expired < delayplusduration ) Between();
 } else Between();
}

void Proce55or::GetNormalizedZonedPosition( ZonedProce55orsGrid *grid, double *outx, double *outy ) {
 if ( grid->extents.x < 0 ) {
  (*outx)=((double)x+abs(grid->extents.x.value))/grid->extents.w.value;
 } else {
  (*outx)=(double)x/grid->extents.w;
 }
 if ( grid->extents.x < 0 ) {
  (*outy)=((double)y+abs(grid->extents.y.value))/grid->extents.h.value;
 } else {
  (*outy)=(double)y/grid->extents.h;
 }
}

void Proce55or::Draw(GLWindow *surface)    { }

#include "Proce55Viewer2d.h"

void Proce55or::Rezone() {
 rezoneMe=true;
 // Do not call (recursive):
// ZSortedFAFZonedProce55Viewer2d *p=(ZSortedFAFZonedProce55Viewer2d *) this->parent;
// p->grid.Rezone(this);
}

void Proce55or::RepopulateOthers() {
 proce55ors->Repopulate();
}

//////////////////////////////////////////////////////////////////////////////////////////



#include "Presentation.h"

 void Proce55ors::Add( Proce55or *p, Presentation *pp, Proce55or *placeBehind ) {
  lastParentAdded=pp;
  p->parent=pp;
  p->presentationParent=pp;
  p->proce55ors=this;
  if ( placeBehind == null || !ListIteminList(placeBehind) ) Append(p);
  else InsertBefore(p,placeBehind);
  p->Attach();
  if ( Zsort ) ZSort();
 }

 void Proce55ors::Add( Proce55or *p, GLWindow *pp, Proce55or *placeBehind ) {
  lastParentAdded=pp;
  p->parent=pp;
  p->proce55ors=this;
  if ( placeBehind == null || !ListIteminList(placeBehind) ) Append(p);
  else InsertBefore(p,placeBehind);
  p->Attach();
  if ( Zsort ) ZSort();
 }

 void Proce55ors::Add( Proce55or *p, Proce55or *placeBehind ) {
  p->parent=lastParentAdded;
  p->proce55ors=this;
  if ( placeBehind == null || !ListIteminList(placeBehind) ) Append(p);
  else InsertBefore(p,placeBehind);
  p->Attach();
  if ( Zsort ) ZSort();
 }

 // Needs a parent window provided, executes Attach() virtual, prepends to list.
 void Proce55ors::AddPrepend( Proce55or *p, Presentation *pp ) {
  lastParentAdded=pp;
  p->parent=pp;
  p->presentationParent=pp;
  p->proce55ors=this;
  Prepend(p);
  p->Attach();
  if ( Zsort ) ZSort();
 }

 void Proce55ors::AddPrepend( Proce55or *p, GLWindow *pp ) {
  lastParentAdded=pp;
  p->parent=pp;
  p->proce55ors=this;
  Prepend(p);
  p->Attach();
  if ( Zsort ) ZSort();
 }

 void Proce55ors::AddPrepend( Proce55or *p ) {
  p->parent=lastParentAdded;
  p->proce55ors=this;
  Prepend(p);
  p->Attach();
  if ( Zsort ) ZSort();
 }

 void Proce55ors::Drop( Proce55orHandles *ps ) {
  EACH(ps->first,Proce55orHandle,p) {
   Drop(p->p);
  }
  ps->Clear();
 }

 
 
 void FarWorldStorage::Audit( Proce55ors *inout, Cartesiand * moved ) {
  int iStart=0;
  int jStart=0;
  int iEnd=(int)farMap.w;
  int jEnd=(int)farMap.h;
  for ( int i=iStart; i<iEnd; i++ ) {
   for ( int j=jStart; j<jEnd; j++ ) {
    FarWorldArea *fwa=Get(i,j);
    fwa->Restore(inout,moved);
   }
  }
 }
 
 void FarWorldStorage::StoreOrRetrieve_recentered( Proce55ors *inout, Cartesiand * moved ) {
  Cartesian worldPosPlayer(
   (int)-moved->x+centerCellOffset.x,
   (int)-moved->y+centerCellOffset.y
  ); 
  Cartesian worldTilePlayer(worldPosPlayer.x/cellSize.w,worldPosPlayer.y/cellSize.h);
  Cartesian worldTilesActive(worldTilePlayer.x-3,worldTilePlayer.y-3,7,7);
  Cartesian activeWorldBounds(
   worldTilesActive.x*cellSize.w,
   worldTilesActive.y*cellSize.h,
   cellSize.w*7,cellSize.h*7
  );
  Cartesian activeWorldScreen(
   -cellSize.w*3,
   -cellSize.h*3,
   cellSize.w*7,
   cellSize.h*7
  );
//  activeWorldScreenBounds.MoveBy(worldSize.x,worldSize.y);
  Proce55orHandles selected;
  inout->FindOutside(&selected,&activeWorldScreen);
  EACH(selected.first,Proce55orHandle,ph) if ( !ph->p->perpetual ) {
   int i=(ph->p->x-moved->x)/cellSize.w+(int)(farMap.w/2);
   int j=(ph->p->y-moved->y)/cellSize.h+(int)(farMap.h/2);
   if ( i < 0 ) i=0; if ( i > (int)farMap.w-1 ) i=farMap.w-1;
   if ( j < 0 ) j=0; if ( j > (int)farMap.h-1 ) j=farMap.h-1;
   inout->Remove(ph->p);
   bool stored=Store(ph->p,i,j,moved);
   if ( !stored ) inout->Add(ph->p);
  }
  int iStart=URANGE(0,worldTilesActive.x,(int)farMap.w);  // removed -1 from farMap.w-1 ...
  int jStart=URANGE(0,worldTilesActive.y,(int)farMap.h);
  int iEnd=URANGE(0,worldTilesActive.x2,(int)farMap.w);
  int jEnd=URANGE(0,worldTilesActive.y2,(int)farMap.h);
  for ( int i=iStart; i<iEnd; i++ ) {
   for ( int j=jStart; j<jEnd; j++ ) {
    FarWorldArea *fwa=Get(i,j);
    fwa->Restore(inout,moved);
   }
  }
 }
 void FarWorldStorage::StoreOrRetrieve( Proce55ors *inout, int iStart, int jStart, int iEnd, int jEnd, Cartesiand * moved ) {
  if ( recenter.value ) StoreOrRetrieve_recentered( inout, moved );
  if ( iStart == iEnd || jStart == jEnd ) return;
  Cartesian activeWorldScreenBounds;
  activeWorldScreenBounds.SetRect(
   iStart*cellSize.w,
   jStart*cellSize.h,
   (iEnd-iStart)*cellSize.w,
   (jEnd-jStart)*cellSize.h
  );
  activeWorldScreenBounds.MoveByd(moved->x,moved->y);
  Proce55orHandles selected;
  inout->FindOutside(&selected,&activeWorldScreenBounds);
  EACH(selected.first,Proce55orHandle,ph) if ( !ph->p->perpetual ) {
   int i=(int)((ph->p->x-moved->x)/cellSize.w);
   int j=(int)((ph->p->y-moved->y)/cellSize.h);
   if ( i < 0 ) i=0; if ( i > (int)farMap.w-1 ) i=(int)farMap.w-1;
   if ( j < 0 ) j=0; if ( j > (int)farMap.h-1 ) j=(int)farMap.h-1;
   inout->Remove(ph->p);
   bool stored=Store(ph->p,i,j,moved);
   if ( !stored ) inout->Add(ph->p);
  }
  for ( int i=iStart; i<iEnd; i++ ) {
   for ( int j=jStart; j<jEnd; j++ ) {
    FarWorldArea *fwa=Get(i,j);
    fwa->Restore(inout,moved);
   }
  }
 }

 
 bool FarWorldStorage::Store( Proce55or *p, int i, int j, Cartesiand *moved ) {
  // put in world space
  FarWorldArea *fwa=Get(i,j);
  if ( fwa ) {
   p->MoveBy(-moved->x,-moved->y);
   p->farStorage=true;
   p->storedFar=true;
   p->storedI=i;
   p->storedJ=j;
   fwa->stored.Append(p);
   p->OnStore(i,j);
   return true;
  } else return false;
 }

 
 void FarWorldArea::Restore( Proce55ors *out, Cartesiand *moved ) {
  EACHN(stored.first,Proce55or,p,{
   stored.Remove(p);
   p->OnRetrieve();
   p->MoveBy(
    moved->x,
    moved->y
   );
   p->farStorage=false;
   p->storedFar=false;
   p->storedI=-1;
   p->storedJ=-1;
   out->Append(p);
  });
 }