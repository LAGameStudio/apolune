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

#include "LinkedList.h"
#include "Cartesian.h"
#include "Vertex.h"
#include "Crayon.h"
#include "Art.h"
#include "Tweens.h"
#include "Model.h"
#include "PackedString.h"
#include "TextureLibrary.h"
#include "Tweens.h"

enum VariadicType {
 v_is_reference=-4,
 v_is_decimal=-3,
 v_is_integer=-2,
 v_unknown=-1,
 v_string=0,
 v_integer=1,
 v_decimal=2,
 v_cartesian=3,
 v_vertex=4,
 v_crayon=5,
 v_image=6,
 v_tween=7,
 v_blend=8
 //v_model
 //v_numbers
 //v_strings
};

VariadicType HintToType( const char *hint );
VariadicType GuessType( const char *hint );

class Variadic : public ListItem {
public:
 Zstring name;
 PackedString name_packed;
 VariadicType type;
 Zdisposable<Cartesian> cartesian;
 Zdisposable<Vertex> vertex;
 Zdisposable<Vertexd> vertexd;
 Zdisposable<Crayon> crayon;
 Zdisposable<Numbers> numbers;
 Zdisposable<KeyValuePair> kv;
 Zdisposable<Strings> strings;
 Zdisposable<Integer> integer;
 Zdisposable<Float> float_;
 Zdisposable<Floats> floats;
 Zdisposable<Zstring> string_value;
 Zp<GLImage> image;
 Zint integer_value;
 Zdouble decimal_value;
 Zp<Tween> tween;
 Zp<mPolygon> polygon;
 Blends blend;
 Zdisposable<ImmediateModel> model;
 Variadic() : ListItem() {
  type=v_string;
 }
 Variadic( const char *name, const char *s ) : ListItem() { Set(name,s); }
 void Set( const char *name, const char *s ) {
  Name(name);
  type=v_string;
  /*
   Further processing of string is important here.
  */
  string_value.Recycle(new Zstring(s));
 }
 Variadic( const char *name, int i ) : ListItem() { Set(name,i); }
 void Set( const char *name, int i ) {
  Name(name);
  type=v_integer;
  integer_value=i;
 }
 Variadic( const char *name, float f ) : ListItem() { Set(name,f); }
 void Set( const char *name, float f ) {
  Name(name);
  type=v_decimal;
  decimal_value=(double)f;
 }
 Variadic( const char *name, double d ) : ListItem() { Set(name,d); }
 void Set( const char *name, double d ) {
  Name(name);
  type=v_decimal;
  decimal_value=d;
 }
 Variadic( const char *name, Cartesian *c ) : ListItem() { Set(name,c); }
 void Set( const char *name, Cartesian *c ) {
  Name(name);
  type=v_cartesian;
  cartesian.Recycle(c);
 }
 Variadic( const char *name, int x, int y, int w, int h ) : ListItem() { Set(name,x,y,w,h); }
 void Set( const char *name, int x, int y, int w, int h ) {
  Name(name);
  type=v_cartesian;
  cartesian.Recycle();
  cartesian.pointer->SetRect(x,y,w,h);
 }
 Variadic( const char *name, Crayon *c ) : ListItem() { Set(name,c); }
 void Set( const char *name, Crayon *c ) {
  Name(name);
  type=v_crayon;
  crayon.Recycle(c);
 }
 Variadic( const char *name, Crayon c ) : ListItem() { Set(name,c); }
 void Set( const char *name, Crayon c ) {
  Name(name);
  type=v_crayon;
  crayon.Recycle();
  crayon->fromCrayon(c);
 }
 Variadic( const char *name, Colors c ) : ListItem() { Set(name,c); }
 void Set( const char *name, Colors c ) {
  Name(name);
  type=v_crayon;
  crayon.Recycle();
  crayon->Pick(c);
 }
 Variadic( const char *name, GLImage *g ) : ListItem() { Set(name,g); }
 void Set( const char *name, GLImage *g ) {
  Name(name);
  type=v_image;
  image=g;
 }
 Variadic( const char *name, Tween *t ) : ListItem() { Set(name,t); }
 void Set( const char *name, Tween *t ) {
  Name(name);
  type=v_tween;
  tween=t;
 }
 Variadic( const char *name, Blends b ) : ListItem() { Set(name,b); }
 void Set( const char *name, Blends b ) {
  Name(name);
  type=v_blend;
  blend=b;
 }
 void Name( const char *s ) {
  name=s;
  name_packed=s;
 }
 bool named( PackedString *p ) {
  return ( this->name_packed.exactly(p) );
 }
 string toString() {
  switch( type ) {
    case v_string: return string_value->value; break;
   case v_integer: return integer_value.toString(); break;
   case v_decimal: return decimal_value.toString(); break;
 case v_cartesian: return cartesian->toString(); break;
    case v_vertex: return vertex->toString(); break;
    case v_crayon: return crayon->toString(); break;
     case v_image: return image->filename; break;
     case v_tween: return tween->name; break;
     case v_blend: return blend.toString(); break;
          default: return string("Unknown Type");
  }
 }
};

HANDLES(Variadic,VariadicHandle,VariadicHandles,"Variadic");

class Variadics : public LinkedList {
public:
 Variadic *find( PackedString *name ) {
  FOREACH(Variadic,v) if ( v->name_packed.exactly(name) ) return v;
  return null;
 }
 CLEARLISTRESET(Variadic);
};

#include "HashStore.h"


class VariableStore : public HashStore<Variadic,Variadics> {
public:
 VariableStore() : HashStore() {
  Set( "pi",      PI );
  Set( "2pi",     TWO_PI );
  Set( "hpi",     HALF_PI );
  Set( "qpi",     QUARTER_PI );
  Set( "3qpi",    THREE_QUARTERS_PI );
  Set( "epsilon", EPSILON );
  Set( "display.w", display.w );
  Set( "display.h", display.h );
  Set( "display.cx", display.w2 );
  Set( "display.cy", display.h2 );
 }
 void Clear() {
  for ( unsigned int i=0; i<this->store.length; i++ ) {
   this->store(i).Clear();
  }
 }
 void Add( Variadic *v ) {
  this->Append(v,&v->name_packed);
 }
 Variadic *Get( const char *name ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  return v;
 }
 Variadic *Set( const char *name, const char *value ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,value));
  else if ( v->type == v_integer ) v->string_value.Recycle(new Zstring(value));
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,value));
  return v;
 }
 Variadic *Set( const char *name, int i ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,i));
  else if ( v->type == v_integer ) v->integer_value=i;
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,i));
  return v;
 }
 Variadic *Set( const char *name, float f ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,f));
  else if ( v->type == v_decimal ) v->integer_value=v->decimal_value=(double)f;
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,f));
  return v;
 }
 Variadic *Set( const char *name, double d ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,d));
  else if ( v->type == v_decimal ) v->decimal_value=d;
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,d));
  return v;
 }
 Variadic *Set( const char *name, Cartesian *c ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,c));
  else if ( v->type == v_cartesian ) v->cartesian.Recycle(c);
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,c));
  return v;
 }
 Variadic *Set( const char *name, int x, int y, int w, int h ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,x,y,w,h));
  else if ( v->type == v_cartesian ) v->cartesian->SetRect(x,y,w,h);
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,x,y,w,h));
  return v;
 }
 Variadic *Set( const char *name, Crayon *c ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,c));
  else if ( v->type == v_crayon ) v->crayon.Recycle(c);
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,c));
  return v;
 }
 Variadic *Set( const char *name, Crayon c ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,c));
  else if ( v->type == v_crayon ) v->crayon->fromCrayon(c);
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,c));
  return v;
 }
 Variadic *Set( const char *name, Colors c ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,c));
  else if ( v->type == v_crayon ) v->crayon->Pick(c);
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,c));
  return v;
 }
 Variadic *Set( const char *name, GLImage *image ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,image));
  else if ( v->type == v_image ) v->image=image;
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,image));
  return v;
 }
 Variadic *Set( const char *name, Tween *tween ) {
  PackedString packed(name);
  Variadics *v_collection=Bucket(&packed);
  Variadic *v=v_collection->find(&packed);
  if ( !v ) v_collection->Append(v=new Variadic(name,tween));
  else if ( v->type == v_image ) v->tween=tween;
  else return (Variadic *)v_collection->ReplaceListItem(v,new Variadic(name,tween));
  return v;
 }
 int EvaluateInteger( const char *w ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v ) {
    switch( v->type ) {
     case v_integer:   return v->integer_value.value;
     case v_decimal:   return (int) v->decimal_value.value;
     case v_cartesian: return (int) v->cartesian->Distancei();
     case v_crayon:    return (int) v->crayon->Luminance();
     case v_string:    return atoi(v->string_value->c_str());
              default: return 0;
    }
   } else return 0;
  } else {
   return atoi(w);
  }
 }
 double EvaluateDecimal( const char *w ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v ) {
    switch( v->type ) {
     case v_integer:   return (double) v->integer_value.value;
     case v_decimal:   return v->decimal_value.value;
     case v_cartesian: return (double) v->cartesian->Distancei();
     case v_crayon:    return (double) v->crayon->Luminance();
     case v_string:    return atof(v->string_value->c_str());
              default: return 0.0;
    } 
   } else return 0.0;
  } else {
   return atof(w);
  }
 }
 string EvaluateString( const char *w ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v ) {
    switch( v->type ) {
     case v_integer:   return v->integer_value.toString();
     case v_decimal:   return v->decimal_value.toString();
     case v_cartesian: return v->cartesian->toString();
     case v_crayon:    return v->crayon->toCSS();
     case v_string:    return v->string_value->value;
              default: return string("");
    } 
    return string("");
   } else return string("");
  } else {
   return string(w);
  }
 }
 void EvaluateCrayon( const char *w, Crayon *out ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v ) {
    switch( v->type ) {
      case v_crayon: out->fromCrayon(*(v->crayon.pointer)); return;
     case v_integer: out->Int( v->integer_value, v->integer_value, v->integer_value, 255 ); return;
     case v_decimal: out->Double( v->decimal_value, v->decimal_value, v->decimal_value, 1.0 ); return;
      case v_string: out->fromString(v->string_value->c_str()); return;
            default: return;
    }
   } else return;
  } else {
   out->fromString(w);
  }
 }
 void EvaluateCartesian( const char *w, Cartesian *out ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v && v->type == v_cartesian ) {
    out->Set(v->cartesian.pointer);
   }
  } else {
   out->fromString(w,this);
  }
 }
 void EvaluateImage( const char *w, GLImage **out ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v ) {
    switch ( v->type ) {
      case v_image: (*out)=v->image; break;
     case v_string: (*out)=library.Load(v->string_value->c_str()); break;
    }
    if ( !(*out) ) (*out)=library.Load("data/images/elements/invis.png");
    return;
   } else (*out)=library.Load("data/images/elements/invis.png");
  } else {
   (*out)=library.Load(w);
   if ( !(*out) ) (*out)=library.Load("data/images/elements/invis.png");
  }
 }
 void EvaluateTween( const char *w, Tween **out ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v ) {
    switch ( v->type ) {
      case v_image: (*out)=v->tween; break;
     case v_string: (*out)=tweens.find(v->string_value->c_str()); break;
    }
    if ( !(*out) ) (*out)=tweens.find("Humanized In Out Slow");
   } else (*out)=tweens.find("Humanized In Out Slow");
  } else {
   (*out)=tweens.find(w);
   if ( !(*out) ) (*out)=tweens.find("Humanized In Out Slow");
  }
 }
 Blends EvaluateBlend( const char *w ) {
  if ( *w == '^' )  {
   w++;
   Variadic *v=Get(w);
   if ( v ) {
    switch ( v->type ) {
       case v_blend: return v->blend;
     case v_integer: {
      Blends temp; temp.blend=(Blendings) URANGE(0,v->integer_value.value,13);
      return temp;   } 
      case v_string: { Blends temp; temp=v->string_value->value; return temp; }
            default: return none;
    }
   } else return none;
  } else {
   Blends temp;
   if ( is_integer(w) ) {
    temp=atoi(w);
   } else {
    temp.fromString(w);
   }
   return temp;
  }
 }
};

extern VariableStore variableStore;

/*

#include "GLWindow.h"
class VariadicTest : public GLWindow {
public:
 void OnLoad() {
  variableStore.Set("player.color",alabaster);
  OUTPUT( "player.color=%s", variables.Get("player.color")->toString().c_str() );
  variableStore.Set("player.name","dude");
  OUTPUT( "player.name=%s", variables.Get("player.name")->toString().c_str() );
  variableStore.Set("player.color",red255);
  OUTPUT( "player.color=%s", variables.Get("player.color")->toString().c_str() );
  variableStore.Set("player.color",orange);
  OUTPUT( "player.color=%s", variables.Get("player.color")->toString().c_str() );
  variableStore.Set("player.xy",12,12,64,64);
  OUTPUT( "player.xy=%s", variables.Get("player.xy")->toString().c_str() );
 }
};

*/