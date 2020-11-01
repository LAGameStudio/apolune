/*********************************************************************************************
*  __    __________________   ________________________________   __________  ________       *
* /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
* \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
*  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
*   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
*    --------------------------------------------------------------------------------       *
*     Lost Astronaut Game Development Framework (c) 2007-2020 H. Elwood Gilliland III       *
*********************************************************************************************
* This software is copyrighted software.  Use of this code is given only with permission to *
* parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
* This software is licensed by permissions of the following open source LICENSE:  AGPL 3.0  *
* See https://www.gnu.org/licenses/agpl-3.0.en.html for details on this license.            *
*********************************************************************************************/
#pragma once
#pragma warning (disable:4100)

#include "macros.h"
#include "moremath.h"
#include "string_functions.h"

#include "ListItem.h"

//#include "utlist.h" // DL_SORT, doesn't work right.  Came from a public domain academic source.

#define CLONES(single,plural) void Duplicate( plural *in ) { EACH(in->first,single,s) Append(s->Duplicate()); }

// Unfortunately, the following macro does not work. See COLLECTION() as a legacy example of the root of this ONE-MANY-DONE concept.
//#define PLURAL(single,plural,definition) class plural : public LinkedList { public: CLEARLISTRESET(single); definition };
// So, we're going to use triplets instead.

// MSVC 2015 NOTE:  Sometimes the linter will incorrectly lint the MANY() _MANY() PLURAL() and related clauses with "Expected a declaration"  This usually goes away eventually.


// These are just renamings of the of the referenced ones below.
// Anonymous
#define _ONE(type_name,code1)        SINGLE(type_name,code1)    
#define _MANY(single,plural,code2)   PLURAL(single,plural,code2)
#define _DONE(type_name)             ENDSET(type_name)          
// Named (usually preferred)
#define ONE(type_name,code1)                                           SINGLE_NAMED(type_name,code1)
#define MANY(single,singleHandle,singleHandles,keyname,plural,code2)   PLURAL_NAMED(single,singleHandle,singleHandles,keyname,plural,code2)
#define DONE(type_name)                                                ENDSET_NAMED(type_name)    

#define VERYMANY(single,singleHandle,singleHandles,singleHandlesHandle,singleHandlesHandles,keyname,plural,code2)  \
 ENDITEM(single); HANDLES(single,singleHandle,singleHandles,keyname); \
 HANDLEHANDLES(singleHandles,singleHandlesHandle,singleHandlesHandles,keyname); \
 LIST(plural,code2) single *named(const char *k) { FOREACH(single,s) if ( !str_cmp(s->name.c_str(),k) ) return s; return null; } single *firstNamed( const char *k ) { FOREACH(single,s) if ( s->name.inside(k) ) return s; return null; } void nameSearch( singleHandles *out, const char *k ) { FOREACH(single,s) if ( s->name.inside(k) ) out->Add(s); } void nameMatches( singleHandles *out, const char *k ) { FOREACH(single,s) if ( !str_cmp(k,s->name.c_str()) ) out->Add(s); } void nameGroup( singleHandles *out, const char *k ) { FOREACH(single,s) if ( s->name.begins(k) ) out->Add(s); }
// Add Zint id; after ONE(), after MANY() use this to generate NextID
#define AUTOKEY(single) int GetNextAvailableID() { int next_id=0; bool found=true; while ( found ) { next_id++; found=false; FOREACH(single,p) if ( p->id == next_id ) { found=true; break; } } return next_id; } single *GetByID( int id ) { FOREACH(single,s) if ( s->id == id ) return s; return null; }
#define ADDAPPEND(single) single *Add() { single *p=new single; p->id=GetNextAvailableID(); Append(p); return p; }
#define DROPHANDLED(single,handle,handles) void Drop( handles *in ) { EACH(in->first,handle,h) { Remove(h->p); delete h->p; } in->Clear(); }
// For childs of ListItem
#define C_ONE(type_name,child,code1) CSINGLE(type_name,child,code1)    
// Childs named (usually preferred)
#define CONEN(type_name,child,code1) CSINGLE_NAMED(type_name,child,code1)    

// Indexed by id (not sorted)
#define IONE(type_name,code1)                                           SINGLE_NAMED(type_name,code1) Zint id;
#define IMANY(single,singleHandle,singleHandles,keyname,plural,code2)   PLURAL_NAMED(single,singleHandle,singleHandles,keyname,plural,code2) AUTOKEY(single); ADDAPPEND(single); KEYWORDSGroup(keyname,single); WORDKEYSGroup(keyname,single); DROPHANDLED(single,singleHandle,singleHandles);
#define IDONE(type_name)                                                ENDSET_NAMED(type_name)    

// Instead of using a template, use these macros to quickly define 'blocks' of one-and-many collection code.
// One
#define ITEM(type_name,code1)  class type_name : public ListItem { public: type_name() : ListItem() { {code1} }
#define ENDITEM(type_name)     };
// Many
#define LIST(type_name,code1)  class type_name : public LinkedList { public: type_name() : LinkedList() { {code1} }
#define ENDLIST(type_name)     CLEARLISTRESET(type_name) };
// Custom Child of One
#define CITEM(type_name,child,code1)  class type_name : public child { public: type_name() : child() { {code1} }
#define CENDITEM(type_name)     };
// One-Many-Done structure
#define SINGLE(type_name,code1)     ITEM(type_name,code1)
#define PLURAL(single,plural,code2) ENDITEM(single); LIST(plural,code2)
#define ENDSET(type_name)           ENDLIST(type_name)
// You can use the following to create a named instance instead of an anonymous instance (default) which is searchable by some default properties.
#define SINGLE_NAMED(type_name,code1)      ITEM(type_name,code1) Zstring name; type_name( const char *n ) { name=n; {code1} } 
#define PLURAL_NAMED(single,singleHandle,singleHandles,keyname,plural,code2)  ENDITEM(single); HANDLES(single,singleHandle,singleHandles,keyname); LIST(plural,code2) single *named(const char *k) { FOREACH(single,s) if ( !str_cmp(s->name.c_str(),k) ) return s; return null; } single *firstNamed( const char *k ) { FOREACH(single,s) if ( s->name.inside(k) ) return s; return null; } void nameSearch( singleHandles *out, const char *k ) { FOREACH(single,s) if ( s->name.inside(k) ) out->Add(s); } void nameMatches( singleHandles *out, const char *k ) { FOREACH(single,s) if ( !str_cmp(k,s->name.c_str()) ) out->Add(s); } void nameGroup( singleHandles *out, const char *k ) { FOREACH(single,s) if ( s->name.begins(k) ) out->Add(s); }
#define ENDSET_NAMED(type_name)            ENDLIST(type_name)

// These are used to expedite override of fromString() code, WORD and TAG are strung together with elses
#define KEYWORDS(code)                  void fromString( const char *s, const char *path_prefix="" ) { OUTPUT("in[%s] ",(typeid(*this).name()) ); Zstring _in(s); const char *w=_in.Next(); while ( *w != '\0' ) { {code} NEXTWORD } }
#define KEYWORDSCustom(fun,code)        void fun( const char *s, const char *path_prefix="" )        { OUTPUT("in[%s] ",(typeid(*this).name()) ); Zstring _in(s); const char *w=_in.Next(); while ( *w != '\0' ) { {code} NEXTWORD } }
#define KEYWORDS_debugStart       void fromString( const char *s, const char *path_prefix="" ) { OUTPUT("in[%s] ",(typeid(*this).name()) ); Zstring _in(s); const char *w=_in.Next(); while ( *w != '\0' ) {
#define KEYWORDS_debugEnd         } }
#define NEXTWORD w=_in.Next();
#define KEYWORD(constcharstring,code)   if ( !str_cmp(w,constcharstring) ) { NEXTWORD {code} }
#define TILEWORD(constcharstring,img)    KEYWORD(constcharstring,{img=library.Load(w,tllto_Tiling);})
#define NEARESTWORD(constcharstring,img)    KEYWORD(constcharstring,{img=library.Load(w,tllto_Tiling_Nearest);})
#define IMGWORD(constcharstring,img)     KEYWORD(constcharstring,{img=library.Load(w);})
#define TINTWORD(constcharstring,cray)   KEYWORD(constcharstring,{cray.CSS(w);})
#define FILMWORD(constcharstring,film)   KEYWORD(constcharstring,{Zstring _fin=w; Zstring _va=_fin.Next(); Zstring _fi=_fin.Next(); film=vault.find(_va.c_str(),_fi.c_str());})
#define TWEENWORD(constcharstring,tween) KEYWORD(constcharstring,{tween=tweens.find(w);})
#define SUBWORDdis(constcharstring,zdis) KEYWORD(constcharstring,{zdis.Recycle(); zdis->fromString(w);})
#define SUBWORD(constcharstring,object)  KEYWORD(constcharstring,{object.fromString(w);})
#define SSUBWORD(constcharstring,object) KEYWORD(constcharstring,{object.fromString(w);})
#define KEYTAG(constcharstring,code)    if ( !str_cmp(w,constcharstring) ) {code}
#define TAGWORD(constcharstring,zbool)   KEYTAG(constcharstring,{zbool=true;})
#define NUMWORD(constcharstring,zvalue)  KEYWORD(constcharstring,{zvalue=w;})
#define TXTWORD(constcharstring,zvalue)  KEYWORD(constcharstring,{zvalue=b64k_decode(w);})
#define ENUMWORD(constcharstring,value,type)   KEYWORD(constcharstring,{value=(type)atoi(w);})
#define BLENDWORD(constcharstring,zvalue)  KEYWORD(constcharstring,{zvalue=w;})
#define SUBFILE(constcharstring,object)  KEYWORD(constcharstring,{string fs=file_as_string(w);object.fromString(fs.c_str());})
#define KEYWORDS_INCLUDE(filenam3,staticclassvar) { OUTPUT("<--Loading file:`%s`---\n",filenam3);  string f=file_as_string(filenam3); staticclassvar.fromString(f.c_str()); }
#define KEYWORDS_PATH_INCLUDE(pathnam3,staticclassvar) { OUTPUT("<--Loading all files from path:`%s`---\n",pathnam3); Strings *list=ls(pathnam3,true,true,true); if ( !list ) return; EACH(list->first,String,s) KEYWORDS_INCLUDE(s->s.c_str(),staticclassvar); delete list; }
#define KEYWORDSCustom_INCLUDE(fun,filenam3,staticclassvar) { OUTPUT("<--Loading file:`%s`---\n",filenam3);  string f=file_as_string(filenam3); staticclassvar.fun(f.c_str()); }
#define KEYWORDSCustom_PATH_INCLUDE(fun,pathnam3,staticclassvar) { OUTPUT("<--Loading all files from path:`%s`---\n",pathnam3); Strings *list=ls(pathnam3,true,true,true); if ( !list ) return; EACH(list->first,String,s) KEYWORDSCustom_INCLUDE(fun,s->s.c_str(),staticclassvar); delete list; }
// Same as above but lets you put some code for post processing after the key-scanning loop
#define KEYWORDS_POST(code,post) void fromString( const char *s, const char *path_prefix="" ) { OUTPUT("in[%s] ",(typeid(*this).name()) ); Zstring _in(s); const char *w=_in.Next(); while ( *w != '\0' ) { {code} /* do not call NEXTWORD to support KEYWORDS_INCLUDE, call manually */ } {post} }
#define KEYWORDSCustom_POST(fun,code,post) void fun( const char *s, const char *path_prefix="" ) { OUTPUT("in[%s] ",(typeid(*this).name()) ); Zstring _in(s); const char *w=_in.Next(); while ( *w != '\0' ) { {code} /* do not call NEXTWORD to support KEYWORDS_INCLUDE, call manually */ } {post} }
#define BADKEY(t)                       { OUTPUT("\n>< %s:fromString() Bad keyword `%s`, advancing to next\n",t,w); }
#define BADKEYWORD                      { OUTPUT("\n>< Bad keyword `%s`, advancing to next\n",w); }
// Just some aliases
#define WORD(constcharstring,code) KEYWORD(constcharstring,code)
#define TAG(constcharstring,code)  KEYTAG(constcharstring,code)

// Now for toString() code
#define WORDKEYS(code)                  string toString( const char *path_prefix="" ) { OUTPUT("out[%s] ",(typeid(*this).name()) ); Zstring _out; {code} return _out.value; }
#define WORDKEYS_PRE(pre,code)          string toString( const char *path_prefix="" ) { OUTPUT("out[%s] ",(typeid(*this).name()) ); Zstring _out; {pre} {code} return _out.value; }
#define WORDKEYS_debugStart       string toString( const char *path_prefix="" ) { OUTPUT("out[%s] ",(typeid(*this).name()) ); Zstring _out; {
#define WORDKEYS_debugEnd         } return _out.value; }
#define WORDKEY(constcharstring,value)  { _out+=constcharstring+string(" {")+value+string("}\n"); }
#define TXTKEY(constcharstring,value)   { if ( strlen(value.c_str()) > 0 ) { _out+=constcharstring+string(" {")+b64k_encode(value.c_str())+string("}\n"); } }
#define NUMBKEY(constcharstring,value)  { _out+=constcharstring+string(" {")+value.toString()+string("}\n"); }
#define ENUMKEY(constcharstring,value)  { _out+=constcharstring+FORMAT(" %d",(int)value)+string("\n"); }
#define TAGKEY(constcharstring,value)   { if ( value ) _out+=constcharstring+string("\n"); }
#define IMGKEY(constcharstring,value)   { if ( value ) _out+=constcharstring+string(" {")+value->filename+string("}\n"); }
#define TILEKEY(constcharstring,value)  IMGKEY(constcharstring,value)
#define TINTKEY(constcharstring,cray)   { _out+=constcharstring+string(" {")+cray.toCSS()+string("}\n"); }
#define FILMKEY(constcharstring,value)  { if ( value ) { FilmLibrary *_flib=value->GetLibrary(); if ( _flib ) { _out+=constcharstring+string("{{")+_flib->title+string("} {")+value->title+string("}}\n"); } } }
#define TWEENKEY(constcharstring,value) { if ( value ) _out+=constcharstring+string(" {")+value->name+string("} "); }
#define SUBKEY(constcharstring,object)  { _out+=constcharstring+string(" {")+object->toString()+string("}\n"); }
#define SUBKEYCustom(constcharstring,object,call) { _out+=constcharstring+string(" {")+object->call()+string("}\n"); }
#define SSUBKEY(constcharstring,object) { _out+=constcharstring+string(" {")+object.toString()+string("}\n"); }
#define BLENDKEY(constcharstring,blend) SSUBKEY(constcharstring,blend)
#define SSUBKEYCustom(constcharstring,object,call) { _out+=constcharstring+string(" {")+object.call()+string("}\n"); }
#define SUBSETKEY(constcharstring,set,handle) { Zstring subset; EACH(set.first,handle,s) subset+=s->SingleKey()+string(" {")+s->p->toString()+string("}\n"); _out+=constcharstring+string(" {")+subset+string("}\n"); }
#define SUBSETFILE(filenam3,set,handle) { OUTPUT("---Saving file:`%s`-->\n",filenam3); Zstring subset; EACH(set.first,handle,s) subset+=s->SingleKey()+string(" {")+s->p->toString()+string("}\n"); string_as_file(subset.c_str(),filenam3); }
#define SUBSETPATH(pathnam3,prefix,postfix,set,handle) { OUTPUT("---Saving files to path:`%s`-->\n",pathnam3); Zstring subset; Zstring filenam3; int fi=0; EACH(set.first,handle,s) { fi++; subset=s->SingleKey()+string(" {")+s->p->toString()+string("}\n"); filenam3=string(pathnam3)+string(prefix)+I2S(fi)+string(postfix); string_as_file(subset.c_str(),filenam3.c_str()); } }

// Same as above but let you provide a name for the function instead of the default toString and fromString
#define NKEYWORDS(fun,code) void fun( const char *s ) { Zstring in(s); const char *w=in.c_str(); while ( *w != '\0' ) { {code} NEXTWORD } }
#define NWORDKEYS(fun,code) string fun() { Zstring _out; {code} return _out.value; }

#define KEYWORDSGroup(keyname,type) void fromString( const char *s ) { OUTPUT("in<<%s<< ",(typeid(*this).name()) ); int loaded=0; Zstring in(s); const char*w=in.Next(); while ( *w != '\0' ) { if ( !str_cmp(w,keyname) ) { w=in.Next(); type *k=new type; k->fromString(w); Append(k); loaded++; } w=in.Next(); OUTPUT(" < Loaded %d %s\n",loaded,keyname ); } }
#define WORDKEYSGroup(keyname,type) string toString() { OUTPUT("out>>%s>> ",(typeid(*this).name()) ); string out=string("\n"); FOREACH(type,k) out+=keyname+string(" {")+k->toString()+string("}\n"); OUTPUT(" > Saved %d %s\n",count.value,keyname ); return out; }
#define KEYSWORDSGroups(keyname,type) KEYWORDSGroup(keyname,type) WORDKEYSGroup(keyname,type)

// BinaryFile integration ----->
#define BINARY_INCustom(fun,code) void fun( BinaryFile *open ) { OUTPUT("b_in[%s][<--",(typeid(*this).name()) ); {code} OUTPUT("-->]\n"); }
#define BINARY_IN(code)           void BinaryRead( BinaryFile *open ) { OUTPUT("b_in[%s][<--",(typeid(*this).name()) ); {code} OUTPUT("-->]\n"); }
#define BINARY_IN_POST(code,post) void BinaryRead( BinaryFile *open ) { OUTPUT("b_in[%s][<--",(typeid(*this).name()) ); {code} {post} OUTPUT("-->]\n"); }
#define B_IN_SUBFILE(object)      { B_IN_WORD({ BinaryFile other(w.c_str(),true); OUTPUT("<--Loading binary sub-file:`%s`---\n",w.c_str());  object.BinaryRead(&other); }) }
#define B_IN_INCLUDE(filenam3,staticclassvar) { OUTPUT("<--Loading binary file:`%s`---\n",filenam3); if ( file_exists(filenam3) ) { BinaryFile included(filenam3,false); staticclassvar.BinaryRead(&included); OUTPUT("Loaded %d from binary file.\n",staticclassvar.count.value); } }
#define B_IN_PATH_INCLUDE(pathnam3,staticclassvar) { OUTPUT("<--Loading all binary files from path:`%s`---\n",pathnam3); Strings *list=ls(pathnam3,true,true,true); if ( list ) { EACH(list->first,String,s) B_IN_INCLUDE(s->s.c_str(),staticclassvar); delete list; } }

#define B_IN_WORD(code)           { Zstring w; open->read(&w); {code} }
#define B_IN_STRING(zstr)         { open->read(&zstr); zstr.rewind(); }
#define B_IN_POD(podtype)         { open->read(&podtype); }
#define B_IN_NUM(ztype)           { open->read(&ztype.value); }
#define B_IN_TXT(txt)             { open->read(&txt); txt=base64_decode(txt.c_str()); }
#define B_IN_TINT(crayon)         { crayon.BinaryRead(open); }
#define B_IN_TILE(img)            { Zstring _w; open->read(&_w); if ( _w.length > 0 ) img=library.Load(_w.c_str(),tllto_Tiling); }
#define B_IN_NEAREST(img)         { Zstring _w; open->read(&_w); if ( _w.length > 0 ) img=library.Load(_w.c_str(),tllto_Tiling_Nearest); }
#define B_IN_IMAGE(img)           { Zstring _w; open->read(&_w); if ( _w.length > 0 ) img=library.Load(_w.c_str()); }
#define B_IN_TWEEN(tw)            { Zstring _w; open->read(&_w); tw=tweens.find(_w.c_str()); }
#define B_IN_FILM(film)           { Zstring _w; Zstring _v; open->read(&_w); open->read(&_v); film=vault.find(_w.c_str(),_v.c_str());  }
#define B_IN_SUB(prop)            { int nullval=0; open->read((&nullval))); if ( nullval == 1 ) prop->BinaryRead(open); }
#define B_IN_SSUB(prop)           { prop.BinaryRead(open); }
#define B_IN_SSUBCustom(prop,fun) { prop.fun(open); }
#define B_IN_Zdis(prop)           { int nullval=0;  open->read((&nullval));  if ( nullval == 1 ) { prop.Recycle(); prop->BinaryRead(open); } }
#define B_IN_ENUM(etype,prop)     { int enumval=0;  open->read((&enumval));  prop=(etype)enumval; }
#define B_IN_BOOL(prop)           { int boolval=0;  open->read((&boolval));  prop=(boolval>0 ? true : false); }
#define B_IN_BLEND(prop)          { int blendval=0; open->read((&blendval)); prop=(Blendings) blendval; }

#define BINARY_OUTCustom(fun,code)  void fun( BinaryFile *open ) { OUTPUT("b_out[%s][-->",(typeid(*this).name()) ); {code} OUTPUT("<--]\n"); }
#define BINARY_OUT(code)            void BinaryWrite( BinaryFile *open ) { OUTPUT("b_out[%s][-->",(typeid(*this).name()) ); {code} OUTPUT("<--]\n"); }
#define BINARY_OUT_POST(code,post)  void BinaryWrite( BinaryFile *open ) { OUTPUT("b_out[%s][-->",(typeid(*this).name()) ); {code} {post} OUTPUT("<--]\n"); }
#define B_OUT_SUBSETFILE(filenam3,set,handle) { OUTPUT("---Saving binary file:`%s`-->\n",filenam3); BinaryFile subsetfile(filenam3,true);  subsetfile.scrawl(set.count.value); EACH(set.first,handle,_unit) _unit->p->BinaryWrite(&subsetfile); }
#define B_OUT_SUBSETPATH(pathnam3,prefix,postfix,set,handle) { OUTPUT("---Saving binary files to path:`%s`-->\n",pathnam3); int fi=0; EACH(set->first,handle,i) { BinaryFile other((string(pathnam3)+string(prefix)+I2S(fi)+string(postfix)).c_str(),true); i->p->BinaryWrite(&other); fi++; } }
#define B_OUT_SUBSETPATHCustom(pathnam3,prefix,postfix,set,handle,call) { OUTPUT("---Saving binary files to path:`%s`-->\n",pathnam3); int fi=0; EACH(set->first,handle,i) { BinaryFile other((string(pathnam3)+string(prefix)+I2S(fi)+string(postfix)).c_str(),true); i->p->call(&other); fi++; } }

#define B_OUT_WORD(data)           { open->scrawl(data); }
#define B_OUT_STRING(zstr)         { open->scrawl(zstr.c_str()); }
#define B_OUT_POD(podtype)         { open->scrawl(podtype); }
#define B_OUT_NUM(ztype)           { open->scrawl(ztype.value); }
#define B_OUT_TXT(txt)             { open->scrawl(base64_encode(txt)); }
#define B_OUT_TINT(crayon)         { crayon.BinaryWrite(open); }
#define B_OUT_TILE(img)            { open->scrawl(img?img->filename:""); }
#define B_OUT_NEAREST(img)         { open->scrawl(img?img->filename:""); }
#define B_OUT_IMAGE(img)           { open->scrawl(img?img->filename:""); }
#define B_OUT_TWEEN(tw)            { open->scrawl(tween?tween->name:""); }
#define B_OUT_FILM(film)           { if ( film ) { FilmLibrary *_flib=film->GetLibrary(); if ( _flib ) { out->scrawl(_flib->title); out->scrawl(film->title); } else { out->scrawl(""); out->scrawl(""); } } }
#define B_OUT_SUB(prop)            { if ( prop ) { open->scrawl(1); prop->BinaryWrite(open); } else { open->scrawl(0); } }
#define B_OUT_SSUB(prop)           { prop.BinaryWrite(open); }
#define B_OUT_SSUBCustom(prop,fun) { prop.fun(open); }
#define B_OUT_Zdis(prop)           { if ( prop ) { open->scrawl(1); prop->BinaryWrite(open); } else { open->scrawl(0); } }
#define B_OUT_ENUM(type,prop)      { open->scrawl((int)(prop));  }
#define B_OUT_BOOL(prop)           { open->scrawl(prop?1:0);  }
#define B_OUT_BLEND(prop)          { open->scrawl((int)prop.blend); }
#define B_OUT_SUBFILE(object)      { B_IN_WORD({ BinaryFile other(w.c_str(),true); OUTPUT("<--Writing sub-file:`%s`---\n",w.c_str());  object.BinaryWrite(&other); }) }
#define B_OUT_INCLUDE(filenam3,staticclassvar) { OUTPUT("<--Writing file:`%s`---\n",filenam3);  BinaryFile included(filename,true); staticclassvar.BinaryWrite(&included); }
#define B_OUT_SUBLIST(otype,list)  { if ( !list ) { open->scrawl(0); } else { open->scrawl(1); open->scrawl(list->count.value); EACH(list.first,otype,_unit) _unit->BinaryWrite(open); } }
#define B_OUT_SSUBLIST(otype,list) { open->scrawl(list.count.value); EACH(list.first,otype,_unit) _unit->BinaryWrite(open); }
#define B_OUT_HANDLES(list,handle) { open->scrawl(list.count.value); EACH(list.first,handle,_unit) _unit->p->BinaryWrite(open); }

#define BINARY_INOUT(single) void BinaryWrite( BinaryFile *open) { open->scrawl(count.value); FOREACH(single,t) t->BinaryWrite(open); } void BinaryRead( BinaryFile *open ) { int temp=0; open->read(&temp); OUTPUT("Reading %d from file ... ",temp); for ( int i=0; i<temp; i++ ) { single *t=new single; Append(t); t->BinaryRead(open); } }
// BinaryFile integration <--

// These macros do not work due to problems with Parent->Child virtuals not being called...
//#define SORTABLE(single,code) int Compare( ListItem *a, ListItem *b ) { single *A=(single *)a; single *B=(single *)b; int result=0; {code} return result; }
//#define COMPARABLE(single,code,comparator) int Compare( ListItem *a, ListItem *b, void *value ) { single *A=(single *)a; single *B=(single *)b; comparator *comparing=(comparator *)value; int result=0; {code} return result; }
// The below macro works, because it implements the function on the list.  Better because you can have multiple sorts on the same list.
// Use in the following way:  SORTING(NameOfSort,{/*precomputation*/},{}
#define SORTING(single,SortName,precomputation,compare_neg1_means_earlier,postcomputation)   void SortName() {                    \
  {precomputation}                    \
  if ( count < 2 ) return;            \
  LinkedList list;                    \
  list.first=first; first=null;       \
  list.last=last; last=null;          \
  list.count=count; count=0;          \
  ListItem *q=list.first;             \
  list.Remove(q);                     \
  Append(q);                          \
  while(list.first) {                 \
   ListItem *a=list.first;            \
   list.Remove(a);                    \
   bool inserted=false;               \
   EACH(this->first,ListItem,b) {     \
    int result=0;                     \
    single *A=(single *)a;            \
    single *B=(single *)b;            \
    {compare_neg1_means_earlier}      \
    if ( result < 0 ) {               \
     this->InsertBefore(a,b);         \
     inserted=true;                   \
     break;                           \
    }                                 \
   }                                  \
   if ( !inserted ) Append(a);        \
  }                                   \
  {postcomputation}                   } 


// Class template:
// SINGLE_NAMED();
// PLURAL_NAMED();
// ENDSET_NAMED();
// results in NAMED() class instantiator, which does not work due to C++ language constraints.
// evidence: #define NAMED(single,construct1,class1,plural,construct2,class2) SINGLE_NAMED(single,construct1); class1 PLURAL_NAMED(single,plural,construct2); class2 ENDSET_NAMED(single);

// This version permits a child:
#define CSINGLE(type_name,child,code1)       CITEM(type_name,child,code1)
// For named searchable lists:
#define CSINGLE_NAMED(type_name,child,code1) CITEM(type_name,child,code1) Zstring name; type_name( const char *n ) { name=n; code1 } 

// Example usage and base override:
// SINGLE(single,{});          (put methods,etc relevant to a single element here)
// PLURAL(single,plural,{});   (put methods,etc relevant to the group here)
// ENDSET(plural);             (ends the section)
// HANDLED(single,...          (extension functions for creation secondary reference, auto generates code for you)
// HANDLED(plural,...          (" ")

#define CLEARLIST(t)   void Clear() { \
   t *n; for ( t *p=(t *) first; p; p=n ) { \
    n=(t *)(p->next); delete p; \
   } \
  }
#define CLEARLISTRESET(t)   void Clear() { \
   t *n; for ( t *p=(t *) first; p; p=n ) { \
    n=(t *)(p->next); delete p; \
   } \
   first=null; last=null; count=0; \
  }
#define ADDLIST(t,r,h)  void Add( t *p ) { h *s= new h(); s->r=p; Append(s); }
#define ADDLISTNAMED(a,t,r,h) void Add( t *p ) { h *s= new h(); s->r=p; s->name=a; Append(s); }
#define FOREACH(t,v)  for ( t *v=(t *) first; v; v=(t *) v->next )
/* Will not work: ??? */
#define FOREACHN(t,v,c) { t *n; for ( t *v=(t *) first; v; v=n ) { n=(t *) v->next; {c} } }
#define FOREACHREV(t,v) for ( t *v=(t *) last; v; v=(t *) v->prev )
#define EACH(head,t,v)  for ( t *v=(t *) head; v; v=(t *) v->next )
#define EACHREV(last,t,v)  for ( t *v=(t *) last; v; v=(t *) v->prev )
#define EACHN(head,t,v,c) { t *n; for ( t *v=(t *) head; v; v=n ) { n=(t *) v->next; {c} } }
#define EACHPAIR(head1,t,v,head2,tt,v2,c)  { t *v=(t *) head1; tt *v2=(tt *)head2; \
 while ( v && v2 ) { \
 {c} \
 v=(t *)v->next; v2=(tt *)v2->next; \
 } }

/* Does not work: */
//#define EACH2(head,t,v,head2,t2,v2,c)  { t *v=(t *) head; t2 *v2=(t2 *)head2; while ( v && v2 ) { c { v=(t *)v->next; v2=(t2 *)v2->next ); } }; }

// These macros are the way to make all objects in a list call the same simple function, such as Between() or Render() or Init() etc.
#define CALLEACH(single,function) void function() { FOREACH(single,s) s->function(); }
#define CALLEACH1(single,function,code_after) void function() { FOREACH(single,s) { s->function(); } code_after }
#define CALLEACH2(single,function,code_each) void function() { FOREACH(single,s) { s->function(); code_each } }
#define CALLEACH3(single,function,code_each,code_after) void function() { FOREACH(single,s) { s->function(); code_each } code_after }
#define CALLEACH4(single,_prototype,_call,code_after) void _prototype { FOREACH(single,s) { _call } code_after }

// These macros do the same as above but preserve the 'next' pointer so that the object can be removed from the list in-transit.
#define CALLEACHN(single,function) void function() { FOREACHN(single,s) s->function(); }
#define CALLEACHN1(single,function,code_after) void function() { FOREACHN(single,s) { s->function(); } code_after }
#define CALLEACHN2(single,function,code_each) void function() { FOREACHN(single,s) { s->function(); code_each } }
#define CALLEACHN3(single,function,code_each,code_after) void function() { FOREACHN(single,s) { s->function(); code_each } code_after }
#define CALLEACHN4(single,_prototype,_call,code_after) void _prototype { FOREACHN(single,s) { _call } code_after }

// Specialized versions of callback used in the scripting language (see references)
#define RENDERLIST(ts,t) class ts : public LinkedList { public: CLEARLISTRESET(t); void Render( GLWindow *surface ) { FOREACH(t,g) g->Render(surface); } void Between() { FOREACH(t,g) g->Between(); } };
#define RENDERLIST2(t) RENDERLIST(ts,t);

// Legacy version of the ONE MANY DONE concept.
#define COLLECTION(t,i) class t : public LinkedList { public: CLEARLISTRESET(i); }

// Allows you to define a simple customized sort method for a collection.
#define LL_SORTER(sort,plural,single,compare) \
 void sort() { \
  if ( count < 2 ) return; \
  plural list; list.first=first; list.last=last; list.count=count; \
  single *q=(single *) list.first; list.Remove(q); Append(q); \
  while(list.first) { \
   single *p=(single *) list.first; list.Remove(p); \
   bool inserted=false; \
   EACH(this->first,single,t) { \
    int result=this->compare(p,t); \
    if ( result <= 0 ) { \
     this->InsertBefore(p,t); inserted=true; break; \
    } \
   } \
   if ( !inserted ) Append(p); \
  } \
 }

class LinkedList : public ListItem {
public:
	Zint count;
	ListItem *first;
	ListItem *last;

	LinkedList(void) : ListItem() { first=last=null; }

 virtual void BinaryRead( BinaryFile *open ); // default works in most cases
 virtual void BinaryWriteData( BinaryFile *open ) {}
 virtual void BinaryReadData( BinaryFile *open ) {}
 virtual void ReadAppend( BinaryFile *open ) {}
 virtual void BinaryWrite( BinaryFile *open ); // default works in most cases

 virtual string identity() { ///RTTI///
  string classname=string(typeid(*this).name());
  string discard;
  const char *q=string_argument((char *)classname.c_str(),&discard); // discards the words "class or struct"
  return string(q);
 }
 virtual string identity( bool useChar ) { ///RTTI///
  const char *classname=typeid(*this).name();
  string discard;
  const char *q=string_argument(classname,&discard); // discards the words "class or struct"
  return string(q);
 }
 bool ListIteminList( ListItem *item ) {
  FOREACH(ListItem,i) if ( i==item ) return true;
  return false;
 }
 void sendBack(ListItem *L) { if ( !L->prev ) return; Remove(L); Prepend(L); }
 void sendFront(ListItem *L) { if ( !L->next ) return; Remove(L); Append(L); }
 void sendUp(ListItem *L, bool wrap=true);
 void sendDown(ListItem *L, bool wrap=true);
 virtual void Append(ListItem *L);
 void _Append(ListItem *L);
 void Prepend(ListItem *L);
 ListItem *Element(int i);
 ListItem *Element(int i, bool wrap);
 int Element(ListItem *item);
 virtual ListItem *Any() {
  if ( count==0 ) return null;
  if ( count==1 ) return first;
  return Element(upto((int)count*1234)%count.value);
 }
 virtual ListItem *Any( int seed );
 // Returns i==count on failure to find, use ListIteminList()
 int IndexOf( ListItem *L ) { int i=0; FOREACH(ListItem,item) { if ( item == L ) break; i++; } return i; }
 void InsertBefore(ListItem *insert,ListItem *before);
 void InsertAfter(ListItem *insert, ListItem *after);
 void InsertAt(ListItem *insert, int i);
 void _Remove(ListItem *L);
 void Remove(ListItem *L) { _Remove(L); }
 void RemoveAll(void);
 ListItem *ReplaceListItem(ListItem *L,ListItem *N) {
  N->prev=L->prev;
  N->next=L->next;
  if ( N->prev ) N->prev->next=N;
  if ( N->next ) N->next->prev=N;
  L->next=null;
  L->prev=null;
  delete L;
  return N;
 }
 void Delete(ListItem *L);
 void Delete(ListItem **L);
 void AppendAndDispose( LinkedList *toAppend ) {
  Concat(toAppend);
  delete toAppend;
 }
 //void DeleteAll(void);//never, use CLEARLIST(class) macro

 void RandomizeOrder();
 void RandomizeOrder(int seed);
 void ReverseOrder() {
  LinkedList temporary;
  EACHN(first,ListItem,L,{
   Remove(L);
   temporary.Prepend(L);
  });
  this->first=temporary.first;
  this->last=temporary.last;
  this->count=temporary.count;
  temporary.first=temporary.last=NULL;
  temporary.count=0;
 }
 // Moves L to current list, empties L
 void Concat( LinkedList *L ) {
  if ( L->count <= 0 ) return;
  if ( count == 0 ) {
   count=L->count;
   first=L->first;
   last=L->last;
  } else {
   count+=L->count;
   last->next=L->first;
   L->first->prev=last;
   last=L->last;
  }
  L->first=null;
  L->last=null;
  L->count=0;
 }

 virtual void Clear() {
//#if defined(DEBUG)
  if ( first ) EACHN(first,ListItem,L,{
   Remove(L);
   delete L;
  });
  count=0;
  last=first=null;
//#endif
 }
 virtual ~LinkedList() { 
  Clear();
 }
};

class Handles : public LinkedList {
public:
};

#if defined(NEVER)
// See HANDLE and related macro options in LinkedList.h
template <class T,class H> class Handles : public LinkedList {
public:
 Handles() : LinkedList() {}
 void Add( T *item ) {
  H *o=new H;
  H->p=item;
  Append((ListItem *) o);
 }
 H *find( string s ) { return find((char*)s.c_str()); }
 virtual T *find( char *s ) {
 }
 virtual CLEARLISTRESET(H);
 virtual ~Handles() { Clear(); }
};
#endif

/// Template ///

/// You can use TLinkedList only when you don't want hand-holding with BinaryFile, ///
///    and no default Any(seed) ///

template<class V>
class TLinkedList : public ListItem {
public:
	Zint count;
	V *first;
	V *last;

	TLinkedList(void) : ListItem() { first=last=null; }

 virtual string identity() { ///RTTI///
  string classname=typeid(*this).name();
  string discard;
  const char *q=string_argument((char *)classname.c_str(),&discard); // discards the words "class or struct"
  return string(q);
 }

 void Up(V *L, bool wrap) {} // not yet implemented
 void Down(V *L, bool wrap) {} // not yet implemented
 virtual void Append(V *L) {
  _Append(L);
 }
 void _Append(V *L) {
  count++;
  if ( last ) { 
   last->next=L;
   L->prev=last; 
   L->next=null;
   last=L; 
   return; 
  }
  L->next=L->prev=null;
  first=last=L;
 }
 void Prepend(V *L) {
  count++;
  if ( first ) {
   first->prev=L;
   L->next=first;
   L->prev=null;
   first=L;
   return;
  }
  L->next=L->prev=null;
  first=last=L;
 }
 V *Element(int i) {
  V *l;
  int c=0;
  for ( l=first; l; l=(V *)l->next ) {
   if ( c==i ) return l;
   c++;
  }
  return null;
 }
 virtual V *Any() {
  if ( count==0 ) return null;
  return Element(upto(count*1234)%count);
 }
 int IndexOf( V *L ) { int i=0; FOREACH(V,item) { if ( item == L ) break; i++; } return i; }
 void InsertBefore(V *insert,V *before) {
  if ( !before ) { Prepend(insert); return; }
  insert->prev=before->prev;
  if ( insert->prev ) insert->prev->next=insert;
  insert->next=before;
  before->prev=insert;
  if ( before == first ) first=insert;
  count++;
 }
 void InsertAfter(V *insert, V *after) {
  if ( !after ) { Prepend(insert); return; }
  insert->next=after->next;
  if ( insert->next ) insert->next->prev=insert;
  after->next=insert;
  insert->prev=after;
  if ( after == last ) last=insert;
  count++;
 }
 void InsertAt(V *insert, int i) {
  V *p=Element(i);
  InsertBefore(insert,p);
 }
 void Remove(V *L)  {
  if ( first==L ) {
   if ( first==last ) {
    first=last=null; 
    L->next=L->prev=null;
    count=0;
    return;
   }
   first=(V *)first->next;
   if ( first ) first->prev=null;
   L->next=L->prev=null;
   count--;
   return;
  }
  if ( last==L ) {
   last=(V *)last->prev;
   if ( last ) last->next=null;
   L->next=L->prev=null;
   count--;
   return;
  }
  V *p,*n;
  p=(V *)L->prev;
  n=(V *)L->next;
  if ( p ) p->next=n;
  if ( n ) n->prev=p;
  L->next=L->prev=null;
  count--;
  return;
 }
 void RemoveAll(void) {
  first=last=null;
  count=0;
 }
 void Delete(V *L) {
  Remove(L);
  delete L;
 }
 void AppendAndDispose( TLinkedList *toAppend ) {
  Concat(toAppend);
  delete toAppend;
 }
 //void DeleteAll(void);//never, use CLEARLIST(class) macro

 void Concat( TLinkedList *L ) {
  if ( L->count <= 0 ) return;
  if ( count == 0 ) {
   count=L->count;
   first=L->first;
   last=L->last;
  } else {
   count+=L->count;
   last->next=L->first;
   last=L->last;
  }
  L->first=null;
  L->last=null;
  L->count=0;
 }

 virtual void Clear() {
   V *n=null;
   for ( V *p=(V *) first; p; p=n ) {
    n=(V *)(p->next);
    delete p;
   }
   first=last=null;
   count=0;
 }

 virtual ~TLinkedList() { 
  Clear();
 }
};


/// HANDLES ///

// Internal unnamed handles
#define HANDLING(single,handle,singlekey) \
 class handle:public ListItem{public: \
  Zp<single> p; \
  handle():ListItem() { } \
  void fromString(char *st) { \
   p.Delete(); \
   p=new single; \
   p->fromString(st); \
  } \
  std::string SingleKey() { return string(singlekey); }\
  virtual string toString() { \
   if(p) return p->toString(); \
   else return string(""); \
  } \
 };

#define HANDLINGS(single,handle,handles,singlekey) \
 class handles:public LinkedList{public: \
  virtual handle*Add(single*q) { \
   handle*n=new handle; \
   n->p=q; \
   Append(n); \
   return n; \
  } \
  virtual handle*Add(single*q,having uniqueness) { \
   handle *w=find(q,true); if ( q ) return w; \
   handle*n=new handle; \
   n->p=q; \
   Append(n); \
   return n; \
  } \
  single *find(single *q) { FOREACH(handle,s) if ( s->p==q ) return s->p; return null; } \
  handle *find(single *q, having returnHandle) { FOREACH(handle,s) if ( s->p==q ) return s; return null; } \
  std::string SingleKey() { return string(singlekey); }\
  bool Includes(single *q) { FOREACH(handle,s) if ( s->p==q ) return true; return false; } \
  void Delete(single*r){ FOREACH(handle,n) if(n->p==r){ Remove(n);delete n;return;} } \
  void Drop(single*r){ FOREACH(handle,n) if(n->p==r){ Remove(n);delete n;return;} } \
  void Duplicate( handles * in ) { EACH(in->first,handle,h) Add(h->p); } \
  CLEARLISTRESET(handle); \
  SKEYSTART SKEY(singlekey,{SPARAMNode(single);}) SKEYEND(identity(true)); \
 };

// Mega-handle without names
#define HANDLED(single,handle,handles,handleshandle,handleshandles) \
 class handles; class handleshandle; class handleshandles; \
 class handle:public ListItem{ public:\
  Zp<single> p; \
  Zp<handles> parent; \
  virtual void fromString(char *st) { p->fromString(st); } \
  virtual void fromString(string k) { p->fromString((char*)k.c_str()); } \
  virtual string toString() { return p->toString(); } \
 }; \
 class handles:public LinkedList{ public:\
  virtual handle*Add(single*q) {\
   handle*n=new handle; \
   n->p=q; \
   n->parent=this; \
   Append(n); \
   return n; \
  } \
  single *find(single *q) { FOREACH(handle,s) if ( s->p==q ) return s->p; return null; } \
  handle *find(single *q,bool returnHandle) { FOREACH(handle,s) if ( s->p==q ) return s; return null; } \
  bool Includes(single *q) { FOREACH(handle,s) if ( s->p==q ) return true; return false; } \
  void Delete(single*r){ FOREACH(handle,n) if(n->p==r){Remove(n);delete n;return;} } \
  void Drop(single*r){ FOREACH(handle,n) if(n->p==r){Remove(n);delete n;return;} } \
  CLEARLISTRESET(handle); \
  ~handles(){Clear();} \
 }; \
 class handleshandle:public ListItem{ public:\
  Zp<handles> p; \
  Zp<handleshandles> parent; \
 }; \
 class handleshandles:public LinkedList{ public: \
  handleshandle*Add(handles*q){ \
   handleshandle*n=new handleshandle; \
   n->p=q; \
   n->parent=this; \
   Append(n); \
   return n; \
  } \
  handles *find(handles *q) { FOREACH(handleshandle,s) if ( s->p==q ) return s->p; return null; } \
  void Delete(handles *r) { FOREACH(handleshandle,s)if(s->p==r){Remove(s);delete s;return;} } \
  void Drop(handles *r) { FOREACH(handleshandle,s)if(s->p==r){Remove(s);delete s;return;} } \
  CLEARLISTRESET(handleshandle); \
  ~handleshandles(){Clear();} \
 };

// Class Name, Class Handle Class Name, Plural Handle Class Name, Plural Plural Handles Class Name
#define HANDLES(single,handle,handles,singlekey) HANDLING(single,handle,singlekey) HANDLINGS(single,handle,handles,singlekey);
#define HANDLEHANDLES(single,handle,handles,singlekey) HANDLES(single,handle,handles,singlekey);

// Defines classes under a named handle
#define NAMED(single,handle,handles) \
 class handles; \
 class handle:public ListItem{ public: \
  Zstring name; \
  Zp<single> p; \
  Zp<handles> parent; \
 }; \
 class handles:public LinkedList{ public: \
 virtual handle*Add(single*q,string m) { \
  handle*n=new handle; \
  n->name=m; \
  n->p=q; \
  n->parent=this; \
  Append(n); \
  return n; \
 } \
 handle*find(string k){ \
  FOREACH(handle,n) if(!str_cmp((char*)n->name.c_str(),(char*)k.c_str())) return n; \
  return null; \
 } \
 void Delete(single*r){ \
  FOREACH(handle,n) if(n->p==r){Remove(n);delete n;return;} \
 } \
 CLEARLISTRESET(handle); \
 ~handles(){Clear();} \
 };

// Generic Handle
HANDLES(ListItem,ListItemHandle,ListItemHandles,"ListItem");

// Optimization for real-time type information
#define RONE(type_name,rtid_code,code1)   SINGLE_NAMED(type_name,{ rtid=rtid_code; code1 })
#define RC_ONE(type_name,rtid_code,childof,code1) C_ONE(type_name,childof,{ rtid=rtid_code; code1})
