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

#include "GLWindow.h"
#include "Slide.h"
#include "Presentation.h"
#include "FireAndForgetLimitedWrappingProce55Viewer2d.h"

#include "Functions.h"

/***************************************/

class Consulate;

class CodeExpression : public ListItem {
public:
 Zstring expression;
 CodeExpression() : ListItem() {}
 CodeExpression( const char *expression ) : ListItem() {
  this->expression=expression;
  Simplify();
 }
 // Simplifies the constants during initial processing
 void Simplify() {
 }
 bool Evaluate( Consulate *es ) {
  return true;
 }
};

class CodeReference : public ListItem {
public:
 Zstring expression;
 CodeReference() : ListItem() {}
 CodeReference( const char *expression ) : ListItem() {
  this->expression=expression;
 }
};

/***********************************/

enum CodeBlockTypes {
 codeblock_when,
 codeblock_set,
 codeblock_type
};

class CodeBlock : public ListItem {
public:
 CodeBlock() : ListItem() {}
 virtual void Execute( Consulate *es ) {}
};

class CodeBlockWhen;
class CodeBlocks : public LinkedList {
public:
 Strings errors;
 Zp<CodeBlockWhen> when;
 Zp<CodeBlock> line;
 Zint integer;
 Zdouble decimal;
 void Populate( const char *code );
 void Reset() {
  line=(CodeBlock *)first;
 }
 bool Step( Consulate *es ) {
  if ( !line ) return true;
  line->Execute(es);
  line=(CodeBlock *) line->next;
  return (line == null);
 }
 CLEARLISTRESET(CodeBlock);
};

class CodeBlockElse : public ListItem {
public:
 Zbool no_expression;
 Zdisposable<CodeExpression> expression;
 CodeBlocks lines;
 CodeBlockElse( const char *code ) : ListItem() {
  no_expression=true;
  lines.Populate(code);
 }
 CodeBlockElse( const char *expression, const char *code ) {
  this->expression.Recycle(new CodeExpression(expression));
  lines.Populate(code);
 }
 void Execute( Consulate *es );
};

class CodeBlockElses : public LinkedList {
public:
 void Add( const char *expression, const char *code ) {
  Append(new CodeBlockElse(expression,code));
 }
 void Add( const char *code ) {
  Append(new CodeBlockElse(code));
 }
 CLEARLISTRESET(CodeBlockElse);
};

class CodeBlockWhen : public CodeBlock {
public:
 Zdisposable<CodeExpression> expression;
 CodeBlocks lines;
 CodeBlockElses elses;
 void Else( const char *expression, const char *code ) {
  elses.Add(expression,code);
 }
 void Else( const char *code ) {
  elses.Add(code);
 }
 void Execute( Consulate *es );
};

class CodeBlockWith : public CodeBlock {
public:
 Zstring hint,search;
 Zstring code;
 Zdisposable<Consulate> consulate;
 CodeBlockWith() : CodeBlock() {
  consulate.Instantiate();
 }
 void Execute( Consulate *es );
};

class CodeBlockAssignment : public CodeBlock {
public:
 VariadicType hint;
 Zdisposable<CodeReference> reference;
 Zdisposable<CodeExpression> expression;
 CodeBlockAssignment() : CodeBlock() {
  hint=v_unknown;
 }
 void Execute( Consulate *es );
};

class CodeBlockInvoke : public CodeBlock {
public:
 Zp<BoundFunction> function;
 Zstring parameters;
 void Execute( Consulate *es );
};

class Procedure;
class CodeBlockExecute : public CodeBlock {
public:
 Zstring procedureName;
 Zp<Procedure> procedure;
 void Execute( Consulate *es );
};

/* A snippet of code */
class Procedure : public ListItem {
public:
 Zint id;
 Zstring filename;
 Zstring name;
 PackedString name_packed;
 Zstring code;
 CodeBlocks lines;
 Strings errors;
 Procedure() : ListItem() {}
 void Name( const char *s ) {
  name=s;
  name_packed.Set(s);
 }
 void Interpret() {
  lines.Populate(code.c_str());
 }
 void Reset() {
  lines.Reset();
 }
 void Step( Consulate *es ) {
  lines.Step(es);
 }
};

class Procedures : public LinkedList {
public:
 CLEARLISTRESET(Procedure);
 void Interpret() {
  FOREACH(Procedure,p) p->Interpret();
 }
 Procedure *find( PackedString *name ) {
  FOREACH(Procedure,p) if ( p->name_packed.exactly(name) ) return p;
  return null;
 }
};

class ProcedureStore : public HashStore<Procedure,Procedures> {
public:
 Procedure *Find( const char *name ) {
  PackedString packed(name);
  Procedures *procedures=Bucket(&packed);
  return procedures->find(&packed);
 }
 void Interpret() {
  unsigned int depth=this->depth*this->depth;
  for( unsigned int i=0; i<depth; i++ ) {
   Procedures *p=this->Get(i);
   p->Interpret();
  }
 }
 int Total() {
  int total=0;
  size_t length=SQ(depth);
  for ( unsigned int i=0; i<length; i++ ) total+=store(i).count.value;
  return total;
 }
};

extern ProcedureStore procedures;

#include "string_functions.h"

class Script : public ListItem {
public:
 Zint id;
 Zstring filename;
#if defined(DEBUG)
 Zstring code;
#endif
 Procedure global;
 Script( const char *in ) : ListItem() {
#if defined(DEBUG)
  code=in;
#endif
  Interpret(in);
 }
 string RemoveComments( const char *in ) {
  const char *p=in;
  Zstring out;
  while ( *p != '\0' ) {
   if ( *p == '\r' ) {
    p++; continue;
   }
   if ( _NESTERS(*p) || _SEP(*p) ) { // Handle nested {} [] (), or quotes "" '' `` (do not remove comments within)
    int nests=1;
    static char cStart=*p;
    static char cEnd='\0';
    out+=(*p);
    p++;
    switch ( cStart ) {
     case '{': cEnd = '}'; break;
     case '[': cEnd = ']'; break;
     case '(': cEnd = ')'; break;
    case '\'': cEnd = '\''; break;
     case '"': cEnd = '"'; break;
     case '`': cEnd = '`'; break;
    }
    while ( *p != '\0' && nests > 0 ) {
     if ( *p == cEnd ) {
      nests--;
      out+=(*p);
      p++;
      if ( nests == 0 ) break;
     } else if ( *p == cStart ) {
      nests++;
      out+=(*p);
      p++;
     } else {
      out+=(*p);
      p++;
     }
    }
    if ( *p == '\0' ) break;
   } else if ( *p == '/' && *(p+1) == '/' ) { //  // style comments
    p++;
    while ( *p != '\0' && (*p) != '\n' ) p++;
    if ( *p == '\0' ) break;
   } else if ( *p == '/' && *(p+1) == '*' ) { //   / and * style comments
    p++;
    while ( *p != '\0' && !(*p == '*' && *(p+1) == '/') ) p++; 
    if ( *p == '\0' ) break;
   } else if ( *p == '#' ) { // # style comments
    p++;
    while ( *p != '\0' && (*p) != '\n' ) p++;
    if ( *p == '\0' ) break;
    else out+=(*p);
   } else out+=(*p);
   p++;
  }
  return out.value;
 }
 string IncludeFiles( const char *in ) {
  Zstring words(in);
  Strings lines;
  lines.SplitPush(words.c_str(),'\n');
  Zstring out;
  EACH(lines.first,String,line) {
   Zstring words(line->s.c_str());
   const char *w=words.Next();
   if ( !str_cmp(w,"include") ) {
    w=words.Next();
    while ( *w != '\0' ) {
     out+=IncludeFiles(RemoveComments(file_as_string(w).c_str()).c_str());
     out+="\n";
     w=words.Next();
    }
   } else {
    out+=line->s.c_str();
    out+="\n";
   }
  }
  return out.value;
 }
 void IsolateProcedures( const char *in ) {
  Zstring words(in);
  Zp<Procedure> procedure=&global;
  const char *w=words.c_str();
  while ( *w != '\0' ) {
   if ( _FILLER(*w) ) {
    procedure->code+=(*w);
    w++;
   } else if ( *w == ':' ) {
    w++;
    Zstring name;
    while ( *w != '\0' && *w != '\n' ) {
     name+=(*w);
     w++;
    }
    procedure=new Procedure();
    procedure->Name(name.c_str());
    if ( *w == '\n' ) w++;
    procedures.Append(procedure,&procedure->name_packed);
   } else if ( *w == '<' ) {
    procedure=&global;
    w++;
   } else if ( LOWER(*w) == 'e' && LOWER(*(w+1)) == 'n' && LOWER(*(w+2)) == 'd' ) {
    procedure=&global;
    w+=3;
   } else {
    while ( *w != '\0' && *w != '\n' ) {
     procedure->code+=(*w);
     w++;
    }
    if ( *w == '\n' ) {
     procedure->code+="\n";
     w++;
    }
   }
  }
 }
 void Interpret( const char *in ) {
  Zstring included(IncludeFiles(RemoveComments(in).c_str()));
  IsolateProcedures(included.c_str());
  procedures.Interpret();
  global.Interpret();
 }
};

class Scripts : public LinkedList {
public:
 Script *New(const char *code) {
  Script *s=new Script(code);
  Append(s);
  return s;
 }
 Script *Load(const char *rootfile) {
  if ( file_exists(rootfile) ) {
   Script *s=new Script(file_as_string(rootfile).c_str());
   Append(s);
   return s;
  } else return null;
 }
 CLEARLISTRESET(Script);
};

/*
#include "GLWindow.h"
class ScriptTest : public GLWindow {
public:
 void OnLoad() {
  Zstring test;
  test+="// This is a comment.\n";
  test+="# This is another comment.\n";
  test+=":Procedure1\n";
  test+="this is some code\n";
  test+=":Procedure2\n";
  test+="end\n";
  test+="this is some global code\n";
  test+=":Procedure Four\n";
  test+="this is some code\n";
  test+="<\n";
  Script *s=new Script(test.c_str());
 }
};
 */

/*
 * Goal of the interpreter is to simplify the language down to the most optimal
 * data structure that makes the execution go the fastest with a speed tradeoff
 * only for simplicity.  Some optimizations have been made, but it is not true
 * bytecode.  Expressions are simplified, but later should be converted to a
 * better structure to expedite executions.
 *
 * Interpreter takes a string as input, presumably from the "first mover" or apriori 
 * execution.  This string is then parsed and additional files are inserted, with
 * an eye kept on cyclical includes.  Another source for this string may be the console.
 *
 * After assembling the complete code picture, the interpreter then splinters it into
 * all of its constituant procedures including the global procedure assembled from
 * code that is not grouped into sub procedures.
 *
 * Then, the procedures are parsed and converted into interpreted bytecode to be executed
 * on demand.  Code can be appended to the interpreter, compounding the interpreted
 * bytecode which provides for script-driven dynamic addition/modification of code segments.
 * (Procedures can be overwritten or the overall procedure library expanded, and new
 * code can be introduced through the virtual console.)
 *
 * The "consulate" is the "running instance" of all scripts, the "World State",
 * of the entire application.  Interpreted is compiled to bytecode -- that is,
 * to the abstraction classes that connect individual functions and their
 * order of execution.
 *
 * The executionist is executing the bytecode and connecting the code to world
 * state and to its environment, and to variadics.
 */
class CodeExecutionStackItem : public ListItem {
public:
 Zp<CodeBlock> line;
 Zp<CodeBlocks> executing;
 CodeExecutionStackItem( CodeBlocks *executing ) : ListItem() {
  this->executing=executing;
  this->line=(CodeBlock *) executing->first;
 }
 bool Step( Consulate *es );
 void Execute( Consulate *es );
};

class CodeExecutionStack : public LinkedList {
public:
 CodeExecutionStackItem *Executing() {
  return (CodeExecutionStackItem *) first;
 }
 CodeExecutionStackItem *Push( CodeBlocks *executing ) {
  CodeExecutionStackItem *pushed=new CodeExecutionStackItem(executing);
  Prepend(pushed);
  return pushed;
 }
 void Pop() {
  if ( first ) {
   CodeExecutionStackItem *one=(CodeExecutionStackItem *) first;
   Remove(one);
   delete one;
  }
 }
 CLEARLISTRESET(CodeExecutionStackItem);
};

/*
 * Specialty classes (Window, Slide, Presentation) .. these are
 * containers which are pushed and popped and determine scope
 * of certain bound functions.
 */
/*
#include "GLWindow.h"
#include "Slide.h"
#include "Presentation.h"
#include "FireAndForgetLimitedWrappingProce55Viewer2d.h"
class ContainerStackItem : public ListItem {
public:
 Zp<GLWindow> window;
 Zp<Presentation> presentation;
 Zp<Slide> slide;
 Zp<FireAndForgetLimitedWrappingProce55Viewer2d> viewer;
 ContainerStackItem( GLWindow *w ) : ListItem() {
  this->window=w;
 }
 ContainerStackItem( Presentation *p ) : ListItem() {
  this->presentation=p;
 }
 ContainerStackItem( Slide *s ) : ListItem() {
  this->slide=s;
 }
};

class ContainerStack : public LinkedList {
public:
 ContainerStackItem *Executing() { return (ContainerStackItem *) first; }
 GLWindow *Push( GLWindow *executing ) {
  Prepend(new ContainerStackItem(executing));
  return executing;
 }
 Presentation *Push( Presentation *executing ) {
  Prepend(new ContainerStackItem(executing));
  return executing;
 }
 Slide *Push( Slide *executing ) {
  Prepend(new ContainerStackItem(executing));
  return executing;
 }
 void Pop() {
  if ( first ) {
   ContainerStackItem *one=(ContainerStackItem *) first;
   Remove(one);
   delete one;
  }
 }
 CLEARLISTRESET(ContainerStackItem);
};
*/

/*
 * The executive
 */
#define MAX_SCRIPT_ERRORS_COUNT 99
class Consulate {
public:
 Zp<VariableStore> variables;
 Scripts scripts;
 Zp<Script> executed;
 CodeExecutionStack execution;
// ContainerStack containment;
 Strings errors;
 Consulate() {
  variables=&variableStore;
 }
 void Load( const char *script_filename ) {
  scripts.Load(script_filename);
 }
 void ShowErrors() {
  string_as_file(
   (string("Global errors:\n")+errors.Concatenate('\n')).c_str(),
   "stderr.txt"
  );
 }
 void Execute() {
  EACH(scripts.first,Script,s) {
   execution.Push(&s->global.lines);
   CodeExecutionStackItem *executing=(CodeExecutionStackItem *) execution.first;
   executing->Execute(this);
   execution.Pop();
   if ( errors.count > MAX_SCRIPT_ERRORS_COUNT ) {
    break;
   }
  }
 }
 void DefaultVariableStore() {
  variables=&variableStore;
 }
 void Clear() {
  execution.Clear();
  errors.Clear();
  scripts.Clear();
 }
};


extern Consulate consulate;