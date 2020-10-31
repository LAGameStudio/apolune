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
#include "BinaryFile.h"
#include "Crayon.h"
#include "GLImage.h"
#include "TextureLibrary.h"
#include "Vertex.h"
#include "Vertexd.h"
#include "Cartesian.h"

// values appropriate for "int atomic"
enum KeyedTypes {
 keyed_type=-1,
 keyed_none=0,
 keyed_string=1,
 keyed_integer=2,
 keyed_float=3,
 keyed_double=4,
 keyed_integer_array=5,
 keyed_float_array=6,
 keyed_double_array=7,
 keyed_crayon=8,
 keyed_image=9,
 keyed_vertex=10,
 keyed_vertexd=11,
 keyed_cartesian=12,
 keyed_cartesiand=13
};

class KeyedData : public ListItem {
public:
 Zstring key;
 KeyedData() : ListItem() {}
 virtual void Retrieve( BinaryFile *open ) {
  open->read(&key);
 }
 virtual void Store( BinaryFile *open ) {
  int atomic=keyed_none;
  open->write(&atomic);
  open->write(key);
 }
};

class KeyedDataString : public KeyedData {
public:
 Zstring value;
 KeyedDataString() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  open->read(&value);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_string;
  open->write(&atomic);
  open->write(key);
  open->write(value);
 }
};

class KeyedDataInteger : public KeyedData {
public:
 Zint value;
 KeyedDataInteger() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  open->read(&value);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_integer;
  open->write(&atomic);
  open->write(key);
  open->write(&value);
 }
};

class KeyedDataFloat : public KeyedData {
public:
 Zfloat value;
 KeyedDataFloat() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  open->read(&value);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_float;
  open->write(&atomic);
  open->write(key);
  open->write(&value);
 }
};

class KeyedDataDouble : public KeyedData {
public:
 Zdouble value;
 KeyedDataDouble() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  open->read(&value);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_double;
  open->write(&atomic);
  open->write(key);
  open->write(&value);
 }
};

class KeyedDataIntegerArray : public KeyedData {
public:
 size_t length;
 int *values;
 KeyedDataIntegerArray() : KeyedData() {
  length=0;
  values=null;
 }
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  open->read(values,length);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_integer_array;
  open->write(&atomic);
  open->write(key);
  open->write(length,values);
 }
};

class KeyedDataFloatArray : public KeyedData {
public:
 size_t length;
 float *values;
 KeyedDataFloatArray() : KeyedData() {
  length=0;
  values=null;
 }
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  open->read(values,length);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_float_array;
  open->write(&atomic);
  open->write(key);
  open->write(length,values);
 }
};

class KeyedDataDoubleArray : public KeyedData {
public:
 size_t length;
 double *values;
 KeyedDataDoubleArray() : KeyedData() {
  length=0;
  values=null;
 }
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  open->read(values,length);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_double_array;
  open->write(&atomic);
  open->write(key);
  open->write(length,values);
 }
};

class KeyedDataCrayon : public KeyedData {
public:
 Crayon crayon;
 KeyedDataCrayon() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  crayon.BinaryRead(open);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_crayon;
  open->write(&atomic);
  crayon.BinaryWrite(open);
 }
};

class KeyedDataImage : public KeyedData {
public:
 Zpointer<GLImage> image;
 KeyedDataImage() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  string in;
  open->read(&in);
  image=library.find((char *)in.c_str());
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_crayon;
  open->write(&atomic);
  open->scrawl(key);
  string out=string(image->filename);
  open->scrawl(out);
 }
};

class KeyedDataVertex : public KeyedData {
public:
 Vertex vertex;
 KeyedDataVertex() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  vertex.BinaryRead(open);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_vertex;
  open->write(&atomic);
  open->scrawl(key);
  vertex.BinaryWrite(open);
 }
};

class KeyedDataVertexd : public KeyedData {
public:
 Vertexd vertexd;
 KeyedDataVertexd() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  vertexd.BinaryRead(open);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_vertexd;
  open->write(&atomic);
  open->scrawl(key);
  vertexd.BinaryWrite(open);
 }
};

class KeyedDataCartesian : public KeyedData {
public:
 Cartesian cartesian;
 KeyedDataCartesian() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  cartesian.BinaryRead(open);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_cartesian;
  open->write(&atomic);
  open->scrawl(key);
  cartesian.BinaryWrite(open);
 }
};

class KeyedDataCartesiand : public KeyedData {
public:
 Cartesiand cartesiand;
 KeyedDataCartesiand() : KeyedData() {}
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  cartesiand.BinaryRead(open);
 }
 void Store( BinaryFile *open ) {
  int atomic=keyed_cartesiand;
  open->write(&atomic);
  open->scrawl(key);
  cartesiand.BinaryWrite(open);
 }
};

class KeyedDataType : public KeyedData {
 ListItem *target;
 KeyedDataType( ListItem *target ) : KeyedData() {
  this->target=target;
 }
 void Retrieve( BinaryFile *open ) {
  open->read(&key);
  target->BinaryRead(open);
 }
 void Store( BinaryFile *open, int code ) {
  int atomic=keyed_type;
  open->scrawl(atomic);
  open->scrawl(code);
  open->scrawl(key);
  target->BinaryWrite(open);
 }
};

class KeyedDataStore : public LinkedList {
public:
 KeyedData *Find( string key ) {
  FOREACH(KeyedData,k) if ( !str_cmp_case(k->key.c_str(),key.c_str()) ) return k;
 }
 KeyedData *find( string key ) {
  FOREACH(KeyedData,k) if ( !str_cmp(k->key.c_str(),key.c_str()) ) return k;
 }
 virtual void Reading( BinaryFile *open, int code ) {
  // would contain atomic coded readers for specialized types
  switch ( code ) { case 0:default:break; }
 }
 virtual void Writing( BinaryFile *open ) {
  // would contain atomic coded writers for specialized types
 }
 void Read( BinaryFile *open ) {
  int keys, atomic;
  open->read(&keys);
  for ( int i=0; i<keys; i++ ) {
   open->read(&atomic);
   switch ( (KeyedTypes) atomic ) {
    case keyed_none:
     {
      KeyedData *k=new KeyedData;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_string:
     {
      KeyedDataString *k=new KeyedDataString;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_integer:
     {
      KeyedDataInteger *k=new KeyedDataInteger;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_float:
     {
      KeyedDataFloat *k=new KeyedDataFloat;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_double:
     {
      KeyedDataDouble *k=new KeyedDataDouble;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_integer_array:
     {
      KeyedDataIntegerArray *k=new KeyedDataIntegerArray;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_float_array:
     {
      KeyedDataFloatArray *k=new KeyedDataFloatArray;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_double_array:
     {
      KeyedDataDoubleArray *k=new KeyedDataDoubleArray;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_crayon:
     {
      KeyedDataCrayon *k=new KeyedDataCrayon;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_image:
     {
      KeyedDataImage *k=new KeyedDataImage;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_vertex:
     {
      KeyedDataVertex *k=new KeyedDataVertex;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_vertexd:
     {
      KeyedDataVertexd *k=new KeyedDataVertexd;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_cartesian:
     {
      KeyedDataCartesian *k=new KeyedDataCartesian;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_cartesiand:
     {
      KeyedDataCartesiand *k=new KeyedDataCartesiand;
      k->Retrieve(open);
      Append(k);
     }
     break;
    case keyed_type:
     {
      int code;
      open->read(&code);
      Reading(open,code);
     }
     break;
   }
  }
 }
 void Write( BinaryFile *open ) {
  FOREACH(KeyedData,k) k->Store(open);
 }
 void read( string key, string *target ) { read( key, target, "" ); }
 void read( string key, string *target, string notFoundValue ) {
  KeyedData *search=find(key);
  if ( !search ) {
   *target=notFoundValue;
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataString") ) {
   KeyedDataString *data=(KeyedDataString *) search;
   *target=data->value.value;
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataString`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, int *target, int notFoundValue=0 ) {
  KeyedData *search=find(key);
  if ( !search ) {
   *target=notFoundValue;
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataInteger") ) {
   KeyedDataInteger *data=(KeyedDataInteger *) search;
   *target=data->value;
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataInteger`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, float *target, float notFoundValue=0.0f ) {
  KeyedData *search=find(key);
  if ( !search ) {
   *target=notFoundValue;
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataFloat") ) {
   KeyedDataFloat *data=(KeyedDataFloat *) search;
   *target=data->value;
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataFloat`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, double *target, double notFoundValue=0.0 ) {
  KeyedData *search=find(key);
  if ( !search ) {
   *target=notFoundValue;
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataDouble") ) {
   KeyedDataDouble *data=(KeyedDataDouble *) search;
   *target=data->value;
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataDouble`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, int *targets, size_t length ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataIntegerArray") ) {
   KeyedDataIntegerArray *data=(KeyedDataIntegerArray *) search;
   for ( int i=0; i<(int)length; i++ ) targets[i]=data->values[i];
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataIntegerArray`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, float *targets, size_t length ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataFloatArray") ) {
   KeyedDataFloatArray *data=(KeyedDataFloatArray *) search;
   for ( int i=0; i<(int)length; i++ ) targets[i]=data->values[i];
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataFloatArray`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, double *targets, size_t length ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataDoubleArray") ) {
   KeyedDataDoubleArray *data=(KeyedDataDoubleArray *) search;
   for ( int i=0; i<(int)length; i++ ) targets[i]=data->values[i];
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataDoubleArray`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, Crayon *target ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataCrayon") ) {
   KeyedDataCrayon *data=(KeyedDataCrayon *) search;
   target->fromCrayon(data->crayon);
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataCrayon`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, Vertex *target ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataVertex") ) {
   KeyedDataVertex *data=(KeyedDataVertex *) search;
   data->vertex.Duplicate(target);
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataVertex`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, Vertexd *target ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataVertexd") ) {
   KeyedDataVertexd *data=(KeyedDataVertexd *) search;
   data->vertexd.Duplicate(target);
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataVertexd`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, Cartesian *target ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataCartesian") ) {
   KeyedDataCartesian *data=(KeyedDataCartesian *) search;
   data->cartesian.Duplicate(target);
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataCartesian`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void read( string key, Cartesiand *target ) {
  KeyedData *search=find(key);
  if ( !search ) {
   OUTPUT("::read(,string,) Key was not found when expected: `%s`\n",key.c_str());
   return;
  }
  if ( search->is("KeyedDataCartesiand") ) {
   KeyedDataCartesiand *data=(KeyedDataCartesiand *) search;
   data->cartesiand.Duplicate(target);
  } else {
   string classname=search->identity();
   OUTPUT("::read(,string,) Searched for key `KeyedDataCartesiand`, found mismatched type: `%s`\n",classname.c_str());
  }
 }
 void write( string key, string source ) {
  KeyedDataString *kv=new KeyedDataString;
  kv->value=source;
  kv->key=key;
  Append(kv);
 }
 void write( string key, int source ) {
  KeyedDataInteger *kv=new KeyedDataInteger;
  kv->value=source;
  kv->key=key;
  Append(kv);
 }
 void write( string key, float source ) {
  KeyedDataFloat *kv=new KeyedDataFloat;
  kv->value=source;
  kv->key=key;
  Append(kv);
 }
 void write( string key, double source ) {
  KeyedDataDouble *kv=new KeyedDataDouble;
  kv->value=source;
  kv->key=key;
  Append(kv);
 }
 void write( string key, int *sources, size_t length ) {
  KeyedDataIntegerArray *kv=new KeyedDataIntegerArray;
  kv->values=new int[length];
  for ( int i=0; i<(int)length; i++ ) kv->values[i]=sources[i];
  kv->key=key;
  Append(kv);
 }
 void write( string key, float *sources, size_t length ) {
  KeyedDataFloatArray *kv=new KeyedDataFloatArray;
  kv->values=new float[length];
  for ( int i=0; i<(int)length; i++ ) kv->values[i]=sources[i];
  kv->key=key;
  Append(kv);
 }
 void write( string key, double *sources, size_t length ) {
  KeyedDataDoubleArray *kv=new KeyedDataDoubleArray;
  kv->values=new double[length];
  for ( int i=0; i<(int)length; i++ ) kv->values[i]=sources[i];
  kv->key=key;
  Append(kv);
 }
 void write( string key, Crayon *source ) {
  KeyedDataCrayon *kv=new KeyedDataCrayon;
  kv->crayon.fromCrayon(*source);
  kv->key=key;
  Append(kv);
 }
 void write( string key, Vertex *source ) {
  KeyedDataVertex *kv=new KeyedDataVertex;
  kv->vertex.Set(source);
  kv->key=key;
  Append(kv);
 }
 void write( string key, Vertexd *source ) {
  KeyedDataVertexd *kv=new KeyedDataVertexd;
  kv->vertexd.Set(source->x,source->y,source->z);
  kv->key=key;
  Append(kv);
 }
 void write( string key, Cartesian *source ) {
  KeyedDataCartesian *kv=new KeyedDataCartesian;
  kv->cartesian.Set(source);
  kv->key=key;
  Append(kv);
 }
 void write( string key, Cartesiand *source ) {
  KeyedDataCartesiand *kv=new KeyedDataCartesiand;
  kv->cartesiand.Set(source);
  kv->key=key;
  Append(kv);
 }
 KeyedDataStore(void) {}
 ~KeyedDataStore(void) {}
};