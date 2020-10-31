#include "ScriptableFX.h"

#include "Scripting.h"


void ScriptableVisual::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"in") ) {
   w=words.Next();
   this->in=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"out") ) {
   w=words.Next();
   out=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"movement") || !str_cmp(w,"moves") || !str_cmp(w,"path") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   while ( *p != '\0' ) {
    if ( !str_cmp(p,"point") || !str_cmp(p,"add") || !str_cmp(p,"waypoint") ) {
     location.mode=pc_lines;
     p=inner.Next();
     Zstring inside(p);
     const char *q=inside.Next();
     int Ax=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int Ay=es->variables->EvaluateInteger(q);
     q=inside.Next();
     if ( *q != '\0' ) {
      location.Waypoint(Ax,Ay,tweens.find(q));
     } else {
      location.Waypoint(Ax,Ay);
     }
    } else if ( !str_cmp(p,"curved") || !str_cmp(p,"curve") ) {
     location.mode=pc_curve;
     p=inner.Next();
     Zstring inside(p);
     const char *q=inside.Next();
     double Ax=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     double Ay=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     double Bx=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     double By=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     double Cx=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     double Cy=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     double Dx=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     double Dy=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     location.Curve(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy);
    } else if ( !str_cmp(p,"line") ) {
     location.mode=pc_line;
     p=inner.Next();
     Zstring inside(p);
     const char *q=inside.Next();
     int Ax=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int Ay=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int Bx=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int By=es->variables->EvaluateInteger(q);
     q=inside.Next();
     location.Path(Ax,Ay,Bx,By);
    } else if ( !str_cmp(p,"tween") ) {
     p=inner.Next();
     location.t=tweens.find(p);
    } else if ( !str_cmp(p,"orbit") ) {
     location.mode=pc_orbit;
     p=inner.Next();
     Zstring inside(p);
     const char *q=inside.Next();
     int Ax=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int Ay=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int Bx=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int By=es->variables->EvaluateInteger(q);
     q=inside.Next();
     location.Path(Ax,Ay,Bx,By);
    }
    p=inner.Next();
   }
  } else if ( !str_cmp(w,"scale") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   while ( *p != '\0' ) {
    if ( !str_cmp(p,"size") || !str_cmp(p,"dim") || !str_cmp(p,"point") ) {
     p=inner.Next();
     Zstring inside(p);
     const char *q=inside.Next();
     int Ax=es->variables->EvaluateInteger(q);
     q=inside.Next();
     int Ay=es->variables->EvaluateInteger(q);
     q=inside.Next();
     if ( *q != '\0' ) {
      size.Add(Ax,Ay,tweens.find(q));
     } else {
      size.Add(Ax,Ay);
     }
    }
    p=inner.Next();
   }
  } else if ( !str_cmp(w,"rotation") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   while ( *p != '\0' ) {
    if ( !str_cmp(p,"angle") ) {
     p=inner.Next();
     Zstring inside(p);
     const char *q=inside.Next();
     double start=es->variables->EvaluateDecimal(q);
     q=inside.Next();
     if ( *q != '\0' ) {
      rotation.mode=ac_spin;
      double end=es->variables->EvaluateDecimal(q);
      rotation.Set(start,end);
     } else {
      rotation.mode=ac_fixed;
      rotation.Set(start,start);
     }
    } else if ( !str_cmp(p,"spin") ) {
     rotation.mode=ac_spin;
     p=inner.Next();
     double spin=es->variables->EvaluateDecimal(p);
     rotation.Set(spin);
    } else if ( !str_cmp(p,"teeter") ) {
     rotation.mode=ac_teeter;
    } else if ( !str_cmp(p,"oscillate") ) {
     rotation.mode=ac_oscillate;
    } else if ( !str_cmp(p,"tween") ) {
     p=inner.Next();
     location.t=tweens.find(p);
    } else if ( !str_cmp(p,"pivot") ) {
     p=inner.Next();
     es->variables->EvaluateCartesian(p,&pivot);
    }
    p=inner.Next();
   }
  } else if ( !str_cmp(w,"tint") || !str_cmp(w,"crayon") || !str_cmp(w,"color") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   while ( *p != '\0' ) {
    if ( !str_cmp(p,"tint") || !str_cmp(p,"color") || !str_cmp(p,"crayon") ) {
     p=inner.Next();
     Zstring inside(p);
     const char *q=inside.Next();
     Crayon C;
     es->variables->EvaluateCrayon(q,&C);
     q=inside.Next();
     if ( *q != '\0' ) {
      color.Add(C,tweens.find(q));
     } else {
      color.Add(C);
     }
    }
    p=inner.Next();
   }
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
 if ( size.sizes.count == 0 ) {
  size.Add(image->width,image->height);
 }
}


void ScriptableVisualAid::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
   this->visible=true;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
   this->visible=false;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
   this->sx=xy.x;
   this->sy=xy.y;
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
   dx=xy.w;
   dy=xy.h;
  } else if ( !str_cmp(w,"in") ) {
   w=words.Next();
   this->in=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"out") ) {
   w=words.Next();
   out=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintC);
   this->Tints(tintA,tintB,tintC);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.mid") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_mid);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"opacity") ) {
   w=words.Next();
   Opacity(es->variables->EvaluateInteger(w));
  } else if ( !str_cmp(w,"position") ) {
   w=words.Next();
   Cartesian position;
   es->variables->EvaluateCartesian(w,&position);
   sx=position.x;
   sy=position.y;
   dx=0;
   dy=0;
   x=position.x;
   y=position.y;
   this->w=w2=position.w;
   this->h=h2=position.h;
  } else if ( !str_cmp(w,"delta") ) {
   w=words.Next();
   Cartesian position;
   es->variables->EvaluateCartesian(w,&position);
   dx=position.x;
   dy=position.y;
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableVisualAid2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
   this->visible=true;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
   this->visible=false;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   x2=x+this->w;
   y2=y+this->h;
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
   dx=xy.w;
   dy=xy.h;
  } else if ( !str_cmp(w,"in") ) {
   w=words.Next();
   this->in=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"out") ) {
   w=words.Next();
   out=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintC);
   this->Tints(tintA,tintB,tintC);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.mid") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_mid);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"opacity") ) {
   w=words.Next();
   Opacity(es->variables->EvaluateInteger(w));
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableAid::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableVisualAidAdditive::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
   this->visible=true;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
   this->visible=false;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   x2=x+this->w;
   y2=y+this->h;
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } /*else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } */else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
   dx=xy.w;
   dy=xy.h;
  } else if ( !str_cmp(w,"in") ) {
   w=words.Next();
   this->in=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"out") ) {
   w=words.Next();
   out=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintC);
   this->Tints(tintA,tintB,tintC);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.mid") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_mid);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"opacity") ) {
   w=words.Next();
   Opacity(es->variables->EvaluateInteger(w));
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableVisualRotated::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
   this->visible=true;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
   this->visible=false;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
   sx=xy.x;
   sy=xy.y;
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   x2=x+this->w;
   y2=y+this->h;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
   dx=xy.w;
   dy=xy.h;
  } else if ( !str_cmp(w,"in") ) {
   w=words.Next();
   this->in=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"out") ) {
   w=words.Next();
   out=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintC);
   this->Tints(tintA,tintB,tintC);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.mid") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_mid);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"opacity") ) {
   w=words.Next();
   Opacity(es->variables->EvaluateInteger(w));
  } else if ( !str_cmp(w,"angle") ) {
   w=words.Next();
   start_angle=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"spin") ) {
   w=words.Next();
   spin=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"pivot") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&pivot);
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableVisualSkewed::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"in") ) {
   w=words.Next();
   this->in=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"out") ) {
   w=words.Next();
   out=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintC);
   this->Tints(tintA,tintB,tintC);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.mid") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_mid);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"opacity") ) {
   w=words.Next();
   Opacity(es->variables->EvaluateInteger(w));
  } else if ( !str_cmp(w,"start.A") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&sA);
  } else if ( !str_cmp(w,"start.B") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&sB);
  } else if ( !str_cmp(w,"start.C") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&sC);
  } else if ( !str_cmp(w,"start.D") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&dD);
  } else if ( !str_cmp(w,"delta.A") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&dA);
  } else if ( !str_cmp(w,"delta.B") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&dB);
  } else if ( !str_cmp(w,"delta.C") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&dC);
  } else if ( !str_cmp(w,"delta.D") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&dD);
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableWobblingVisualAid2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   this->Tints(tintA,tintB);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"wander.speed") ) {
   w=words.Next();
   wanderSpeed=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.speed.limit") ) {
   w=words.Next();
   wanderSpeedLimit=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.accel") ) {
   w=words.Next();
   wanderAccel=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"size.mod") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   widthMod=xy.w;
   heightMod=xy.h;
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableWanderingVisualAid2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   this->Tints(tintA,tintB);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"wander.speed") ) {
   w=words.Next();
   wanderSpeed=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.speed.minimum") ) {
   w=words.Next();
   wanderSpeedMinimum=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.speed.limit") ) {
   w=words.Next();
   wanderSpeedLimit=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.accel") ) {
   w=words.Next();
   wanderAccel=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"size.mod") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   widthMod=xy.w;
   heightMod=xy.h;
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableDriftingVisualAid2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   this->Tints(tintA,tintB);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"wander.speed") ) {
   w=words.Next();
   wanderSpeed=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.mod") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   xDirection=xy.x;
   yDirection=xy.y;
   widthMod=xy.w;
   heightMod=xy.h;
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableDriftingStretchingVisualAid2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   this->Tints(tintA,tintB);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"wander.speed") ) {
   w=words.Next();
   wanderSpeed=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.mod") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   xDirection=xy.x;
   yDirection=xy.y;
   widthMod=xy.w;
   heightMod=xy.h;
  } else if ( !str_cmp(w,"delta") ) {
   w=words.Next();
   delta=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tinting") ) {
   w=words.Next();
   tinting=(float)es->variables->EvaluateDecimal(w);
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}


void ScriptableDriftingRotatingVisualAid2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"fullsize") ) {
   if ( !image ) image=library.Load("data/images/elements/invis.png");
   else {
    this->w=w2=image->width;
    h=h2=image->height;
   }
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"path") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   sx=xy.x;
   sy=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&newTint);
   this->Tint(newTint);
  } else if ( !str_cmp(w,"tints") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   Crayon tintA,tintB,tintC;
   es->variables->EvaluateCrayon(p,&tintA);
   p=inner.Next();
   es->variables->EvaluateCrayon(p,&tintB);
   this->Tints(tintA,tintB);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_start);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&tint_end);
  } else if ( !str_cmp(w,"wander.speed") ) {
   w=words.Next();
   wanderSpeed=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"wander.mod") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   xDirection=xy.x;
   yDirection=xy.y;
   widthMod=xy.w;
   heightMod=xy.h;
  } else if ( !str_cmp(w,"delta") ) {
   w=words.Next();
   delta=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"angle") ) {
   w=words.Next();
   angle=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delta.angle") ) {
   w=words.Next();
   deltaAngle=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tinting") ) {
   w=words.Next();
   tinting=(float)es->variables->EvaluateDecimal(w);
  }
  w=words.Next();
 }
 if ( !image ) image=library.Load("data/images/elements/invis.png");
}

// Not yet implemented.
void ScriptableDriftingStretchingRotatingVisualAid2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_Glow::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") || !str_cmp(w,"flare") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&flare.pointer);
   if ( !flare ) flare=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x;
   h2=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&this->tint);
  } else if ( !str_cmp(w,"maxop") ) {
   w=words.Next();
   maxOp=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tween") || !str_cmp(w,"easing") ) {
   w=words.Next();
   string vname=es->variables->EvaluateString(w);
   w=words.Next();
   float lo=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float hi=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float duration=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   string tname=es->variables->EvaluateString(w);
   Tween *t=tweens.find(tname.c_str());
   if ( !t ) t=tweens.find("Humanized In Out Slow");
   controller.variables.NewfT(vname.c_str(),lo,hi,duration,t);
  } else if ( !str_cmp(w,"period") ) {
   w=words.Next();
   controller.period=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"float") || !str_cmp(w,"decimal") ) {
   w=words.Next();
   string vname=es->variables->EvaluateString(w);
   w=words.Next();
   float lo=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float hi=(float)es->variables->EvaluateDecimal(w);
   controller.variables.Newf(vname.c_str(),lo,hi);
  } else if ( !str_cmp(w,"erratic") ) {
   this->erratic_flaresource=!this->erratic_flaresource;
  } else if ( !str_cmp(w,"move_leftright") ) {
   this->tweenW=!this->tweenW;
  } else if ( !str_cmp(w,"move_updown") ) {
   this->tweenW=!this->tweenH;
  } else if ( !str_cmp(w,"leftright") ) {
   w=words.Next();
   tweenLR=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"updown") ) {
   w=words.Next();
   tweenUD=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"delta") ) {
   w=words.Next();
   H=(float)es->variables->EvaluateDecimal(w);
  }
  w=words.Next();
 }
 if ( !flare ) flare=library.Load("data/images/elements/invis.png");
}


void Scriptablefx_Glow2::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") || !str_cmp(w,"flare") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&flare.pointer);
   if ( !flare ) flare=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x;
   h2=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&this->tint);
  } else if ( !str_cmp(w,"maxop") ) {
   w=words.Next();
   maxOp=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tween") || !str_cmp(w,"easing") ) {
   w=words.Next();
   string vname=es->variables->EvaluateString(w);
   w=words.Next();
   float lo=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float hi=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float duration=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   string tname=es->variables->EvaluateString(w);
   Tween *t=tweens.find(tname.c_str());
   if ( !t ) t=tweens.find("Humanized In Out Slow");
   controller.variables.NewfT(vname.c_str(),lo,hi,duration,t);
  } else if ( !str_cmp(w,"period") ) {
   w=words.Next();
   controller.period=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"float") || !str_cmp(w,"decimal") ) {
   w=words.Next();
   string vname=es->variables->EvaluateString(w);
   w=words.Next();
   float lo=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float hi=(float)es->variables->EvaluateDecimal(w);
   controller.variables.Newf(vname.c_str(),lo,hi);
  } else if ( !str_cmp(w,"erratic") ) {
   this->erratic_flaresource=!this->erratic_flaresource;
  } else if ( !str_cmp(w,"move_leftright") ) {
   this->tweenW=!this->tweenW;
  } else if ( !str_cmp(w,"move_updown") ) {
   this->tweenW=!this->tweenH;
  } else if ( !str_cmp(w,"leftright") ) {
   w=words.Next();
   tweenLR=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"updown") ) {
   w=words.Next();
   tweenUD=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"delta") ) {
   w=words.Next();
   H=(float)es->variables->EvaluateDecimal(w);
  }
  w=words.Next();
 }
}


void Scriptablefx_Glow3::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"image") || !str_cmp(w,"flare") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&flare.pointer);
   if ( !flare ) flare=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"stretches") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x2;
   h2=xy.y2;
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->w=xy.x;
   h=xy.y;
   w2=xy.x;
   h2=xy.y;
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   Crayon newTint;
   es->variables->EvaluateCrayon(w,&this->tint);
  } else if ( !str_cmp(w,"maxop") ) {
   w=words.Next();
   maxOp=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"tween") || !str_cmp(w,"easing") ) {
   w=words.Next();
   string vname=es->variables->EvaluateString(w);
   w=words.Next();
   float lo=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float hi=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float duration=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   string tname=es->variables->EvaluateString(w);
   Tween *t=tweens.find(tname.c_str());
   if ( !t ) t=tweens.find("Humanized In Out Slow");
   controller.variables.NewfT(vname.c_str(),lo,hi,duration,t);
  } else if ( !str_cmp(w,"period") ) {
   w=words.Next();
   controller.period=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"float") || !str_cmp(w,"decimal") ) {
   w=words.Next();
   string vname=es->variables->EvaluateString(w);
   w=words.Next();
   float lo=(float)es->variables->EvaluateDecimal(w);
   w=words.Next();
   float hi=(float)es->variables->EvaluateDecimal(w);
   controller.variables.Newf(vname.c_str(),lo,hi);
  } else if ( !str_cmp(w,"delta") ) {
   w=words.Next();
   H=(float)es->variables->EvaluateDecimal(w);
  }
  w=words.Next();
 }
}


void Scriptablefx_Image::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"extents") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   x=xy.x;
   y=xy.y;
   x2=xy.x2;
   y2=xy.y2;
   this->w=xy.w;
   h=xy.h;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"image") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&image.pointer);
   if ( !image ) image=library.Load("data/images/elements/invis.png");
  }
  w=words.Next();
 }
}


void Scriptablefx_Box::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"extents") ) {
   w=words.Next();
   Cartesian points;
   es->variables->EvaluateCartesian(w,&points);
   x=points.x;
   y=points.y;
   x2=points.x2;
   y2=points.y2;
   this->w=points.w;
   h=points.h;
  } else if ( !str_cmp(w,"tint.outside") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->tint_outside);
  } else if ( !str_cmp(w,"tint.inside") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->tint_inside);
  } else if ( !str_cmp(w,"tint.start") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->tint_start);
  } else if ( !str_cmp(w,"tint.end") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->tint_end);
  } else if ( !str_cmp(w,"tint.interior") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->interior);
  } else if ( !str_cmp(w,"fill") || !str_cmp(w,"filled") ) {
   filled=!filled;
  } else if ( !str_cmp(w,"pulse") ) {
   w=words.Next();
   pulsespeed=(float)es->variables->EvaluateDecimal(w);
  }
  w=words.Next();
 }
}


void Scriptablefx_UglyTextBox::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"extents") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&extents);
   x=extents.x; position.x=extents.x;
   y=extents.y; position.y=extents.y;
   x2=extents.x2;
   y2=extents.y2;
   this->w=extents.w;
   h=extents.h;
  } else if ( !str_cmp(w,"backing.extents") ) {
   w=words.Next();
   es->variables->EvaluateCartesian(w,&backingExtents);
  } else if ( !str_cmp(w,"backing") ) {
   w=words.Next();
   es->variables->EvaluateImage(w,&backing.pointer);
   if ( !backing ) backing=library.Load("data/images/elements/invis.png");
  } else if ( !str_cmp(w,"tint.hover") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->hover);
  } else if ( !str_cmp(w,"tint.focus") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->focus);
  } else if ( !str_cmp(w,"tint.click") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->click);
  } else if ( !str_cmp(w,"tint.cursor") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->cursor);
  } else if ( !str_cmp(w,"characters") ) {
   w=words.Next();
   maxChars=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"position") ) {
   w=words.Next();
   Zstring inner(w);
   const char *p=inner.Next();
   position.x=atof(p);
   p=inner.Next();
   position.y=atof(p);
  } else if ( !str_cmp(w,"value") ) {
   w=words.Next();
   out=es->variables->EvaluateString(w);
  } else if ( !str_cmp(w,"font.size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   fw=xy.w;
   fh=xy.h;
   lineSpace=xy.y;
  } else if ( !str_cmp(w,"border") ) {
   w=words.Next();
   border=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"centered") ) {
   centered=true;
  } else if ( !str_cmp(w,"left") ) {
   centered=false;
  } else if ( !str_cmp(w,"password.character") ) {
   w=words.Next();
   string result=es->variables->EvaluateString(w);
   passwordChar=(*result.c_str());
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"blend.shading") ) {
   w=words.Next();
   this->blendShading=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"blend.cursor") ) {
   w=words.Next();
   this->cursorBlend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"blend.hover") ) {
   w=words.Next();
   this->hoverBlend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"blend.focus") ) {
   w=words.Next();
   this->focusBlend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->tint);
  } else if ( !str_cmp(w,"shade") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->shade);
  } else if ( !str_cmp(w,"style") ) {
   w=words.Next();
   UglyDrawStyle s=uglydraw_fromString(w);
   SetStyle(s);
  } else if ( !str_cmp(w,"completed") ) {
  } else if ( !str_cmp(w,"focused") ) {
  }
  w=words.Next();
 }
 if ( !backing ) backing=library.Load("data/images/elements/invis.png");
}


void Scriptablefx_Circle2d::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"tint") ) {
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"radius") ) {
   w=words.Next();
   this->radius=(float)es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"position") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   x=xy.x; 
   y=xy.y;
  } else if ( !str_cmp(w,"segments") ) {
   w=words.Next();
   this->segments=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"extents") ) {
   w=words.Next();
   Cartesian extents;
   es->variables->EvaluateCartesian(w,&extents);
   x=extents.x; 
   y=extents.y; 
   x2=extents.x2;
   y2=extents.y2;
   this->w=extents.w;
   h=extents.h;
   this->radius=(float)extents.w;
  }
  w=words.Next();
 }
}


void Scriptablefx_TexFont::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"font") ) {
   w=words.Next();
   string fname=es->variables->EvaluateString(w);
   font=texfonts.find(w);
   if ( !font ) font=defaultTexFont;
  } else if ( !str_cmp(w,"content") || !str_cmp(w,"text") ) {
  } else if ( !str_cmp(w,"font.size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   fw=xy.w;
   fh=xy.h;
   lineSpace=xy.y;
  } else if ( !str_cmp(w,"characters") ) {
   w=words.Next();
   maxChars=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"delta.in") ) {
   w=words.Next();
   this->deltaIn=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delta.out") ) {
   w=words.Next();
   deltaOut=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delta.duration") ) {
   w=words.Next();   
   deltaDuration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delta.delay") ) {
   w=words.Next();
   deltaDelay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"relative") ) {
  } else if ( !str_cmp(w,"blend") ) {
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->color);
  }
  w=words.Next();
 }
 if ( !font ) font=defaultTexFont;
}


void Scriptablefx_TexFontFader::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"show") ) {
   this->hidden=false;
  } else if ( !str_cmp(w,"hide") ) {
   this->hidden=true;
  } else if ( !str_cmp(w,"move") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   this->MoveBy(xy.x,xy.y);
  } else if ( !str_cmp(w,"font") ) {
  } else if ( !str_cmp(w,"content") || !str_cmp(w,"text") ) {
  } else if ( !str_cmp(w,"font.size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   fw=xy.w;
   fh=xy.h;
   lineSpace=xy.y;
  } else if ( !str_cmp(w,"characters") ) {
   w=words.Next();
   maxChars=es->variables->EvaluateInteger(w);
  } else if ( !str_cmp(w,"delta.in") ) {
   w=words.Next();
   this->deltaIn=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delta.out") ) {
   w=words.Next();
   deltaOut=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delta.duration") ) {
   w=words.Next();   
   deltaDuration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delta.delay") ) {
   w=words.Next();
   deltaDelay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"duration") ) {
   w=words.Next();   
   duration=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"delay") ) {
   w=words.Next();
   delay=es->variables->EvaluateDecimal(w);
  } else if ( !str_cmp(w,"relative") ) {
  } else if ( !str_cmp(w,"blend") ) {
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->color);
  }
  w=words.Next();
 }
}


void Scriptablefx_TexFontBox::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_TexFontBoxString::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_TexNumberToggle::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_XFade::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_CoolButton::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_UglyCoolButton::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_DirectButton::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_UglyButton::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_TexFontCoolButton::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_CoolButtonToggle::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_CoolToggle::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_ToggleBox::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_ToggleIcon::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_UglyToggleBox::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_IconSelect::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_IntegerSelect::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_FloatSelect::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_CoolRGBAPal4f::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_CoolRGBPal3f::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_SimpleRGBPal3f::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_UglyRGBPal::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_SectionalPercentages::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_RangeSelector::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_CoordPlane::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_XYPad::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_TweenSelect::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_Knob::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_Scroller::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_VScroller::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_TabView::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_Shader2d::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_LabeledSlider::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_UglySlider::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_VerticalSlider::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_ReadOnlySlider::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_UglyText::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w,"position") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   x=xy.x;
   y=xy.y;
   this->w=xy.w;
   h=xy.h;
   x2=xy.x2;
   y2=xy.y2;
   position.Set((double)x,(double)y);
  } else if ( !str_cmp(w,"content") || !str_cmp(w,"text") ) {
   w=words.Next();
   this->out=es->variables->EvaluateString(w);
  } else if ( !str_cmp(w,"font.size") ) {
   w=words.Next();
   Cartesian xy;
   es->variables->EvaluateCartesian(w,&xy);
   fw=xy.w;
   fh=xy.h;
   lineSpace=xy.y;
  } else if ( !str_cmp(w,"left") ) {
   centered=false;
  } else if ( !str_cmp(w,"centered") ) {
   centered=true;
  } else if ( !str_cmp(w,"blend") ) {
   w=words.Next();
   this->blend=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"blend.shading") ) {
   w=words.Next();
   this->blendShading=es->variables->EvaluateBlend(w);
  } else if ( !str_cmp(w,"tint") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->tint);
  } else if ( !str_cmp(w,"shade") ) {
   w=words.Next();
   es->variables->EvaluateCrayon(w,&this->shade);
  } else if ( !str_cmp(w,"style") ) {
   w=words.Next();
   UglyDrawStyle s=uglydraw_fromString(w);
   SetStyle(s);
  }
  w=words.Next();
 }
}


void Scriptablefx_Film::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_FilmLoop::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_FilmLoopFader::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_FilmLineFader::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_FilmIsotrapf::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_Lightning2d::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_LineGraph::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_FadeIn::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_FadeOut::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}


void Scriptablefx_MovingLine::fromString( const char *in, Consulate *es ) {
 this->consulate=es;
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  w=words.Next();
 }
}
