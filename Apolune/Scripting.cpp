#include "Scripting.h"

Consulate consulate;

FunctionStore boundFunctions;
ProcedureStore procedures;

void FunctionStore::Init() {
 Add(new BoundArea);
 Add(new BoundRectangle);
 Add(new BoundStretch);
 Add(new BoundWindow);
 Add(new BoundPresentation);
 Add(new BoundDiscard);
 Add(new BoundLinewidth);
 Add(new BoundStringLoad);
 Add(new BoundStringSave);
 Add(new BoundStringChar);
 Add(new BoundStringInteger);
 Add(new BoundStringDecimal);
 Add(new BoundStringWord);
 Add(new BoundStringWords);
 Add(new BoundStringMatches);
 Add(new BoundStringLine);
 Add(new BoundStringLines);
 Add(new BoundStringConcat);
 Add(new BoundStringConcatLine);
 Add(new BoundStringReplace);
 Add(new BoundStringReplace1);
 Add(new BoundMathAdd);
 Add(new BoundMathSub);
 Add(new BoundMathDiv);
 Add(new BoundMathMod);
 Add(new BoundMathMul);
 Add(new BoundMathRad);
 Add(new BoundMathDeg);
 Add(new BoundMathLeast);
 Add(new BoundMathGreatest);
 Add(new BoundMathDice);
 Add(new BoundMathRandom);
 Add(new BoundMathPseudo);
 Add(new BoundMathLerp);
 Add(new BoundMathRange);
 Add(new BoundBoolEqual);
 Add(new BoundBoolLess);
 Add(new BoundBoolMore);
 Add(new BoundBoolLessEqual);
 Add(new BoundBoolMoreEqual);
 Add(new BoundBoolOdd);
 Add(new BoundBoolEven);
 Add(new BoundTypeIs);
 Add(new BoundTypeInteger);
 Add(new BoundTypeDecimal);
 Add(new BoundTypeString);
 Add(new BoundTypeBlend);
 Add(new BoundTypeImage);
 Add(new BoundTypeCrayon);
 Add(new BoundTypeCartesian);
 Add(new BoundTypeTween);
 Add(new BoundTweenInstant);
 Add(new BoundCartesianX);
 Add(new BoundCartesianY);
 Add(new BoundCartesianW);
 Add(new BoundCartesianH);
 Add(new BoundCartesianX2);
 Add(new BoundCartesianY2);
 Add(new BoundCrayonR);
 Add(new BoundCrayonG);
 Add(new BoundCrayonB);
 Add(new BoundCrayonA);
 Add(new BoundCrayonR255);
 Add(new BoundCrayonG255);
 Add(new BoundCrayonB255);
 Add(new BoundCrayonA255);
 Add(new BoundImageName);
 Add(new BoundGeoDist1d);
 Add(new BoundGeoDist2d);
 Add(new BoundGeoDist3d);
 Add(new BoundGeoBoxBox);
 Add(new BoundGeoBoxBoxIntersection);
 Add(new BoundGeoBoxLine);
 Add(new BoundGeoLineLine);
 Add(new BoundGeoLineLineIntersection);
 Add(new BoundGeoPointCircle);
 Add(new BoundGeoLineCircle);
 Add(new BoundGeoLineCircleIntersection);
 Add(new BoundGeoRotate);
}

void CodeBlockAssignment::Execute( Consulate *es ) {
 switch ( hint ) {
  case v_is_integer: 
    es->variables->Set(reference->expression.c_str(),atoi(expression->expression.c_str()));
   break;
  case v_is_decimal:
    es->variables->Set(reference->expression.c_str(),atof(expression->expression.c_str()));
   break;
  case v_cartesian:
    {
     Cartesian *c=new Cartesian;
     c->fromString(expression->expression.c_str());
     es->variables->Set(reference->expression.c_str(),c);
    }
   break;
  /*
  case v_vertex:
    {
     Vertexd *v=new Vertexd;
     v->fromString(expression->expression.c_str());
     es->variables.Set(reference->expression.c_str(),v);
    }
   break;
   */
  case v_string:
    es->variables->Set(reference->expression.c_str(),expression->expression.c_str());
   break;
  case v_crayon:
   {
    Crayon c;
    c.fromString(expression->expression.c_str());
    es->variables->Set(reference->expression.c_str(),c);
   }
   break;
  case v_unknown: es->errors.printf("Unknown type `%s`: ",expression->expression.c_str());
  default: es->errors.printf("Invalid assignment.\n"); break;
 }
}

/*
 * Allows you to modify existing settings on certain types of objects
 */
void CodeBlockWith::Execute( Consulate *es ) {
 if ( !str_cmp(hint.c_str(),"glwindow") ) {
  EACH(windows.first,GLWindow,g) if ( !str_cmp(search.c_str(),g->name.c_str()) ) {
   g->fromString(code.c_str());
   break;
  }
 } else if ( !str_cmp(hint.c_str(),"window") ) {
  EACH(windows.first,GLWindow,g) if ( !str_cmp(search.c_str(),g->name.c_str()) ) {
   if ( !str_cmp(g->classDescription.c_str(),"GLWindowScriptable") ) {
    GLWindowScriptable *gs=(GLWindowScriptable *) g;
    gs->fromString(code.c_str());
   }
   break;
  }
 } else if ( !str_cmp(hint.c_str(),"presentation") ) {
  EACH(windows.first,GLWindow,g) if ( !str_cmp(search.c_str(),g->name.c_str()) ) {
   if ( !str_cmp(g->classDescription.c_str(),"Presentation") ) {
    Presentation *p=(Presentation *) g;
    p->fromString(code.c_str());
   }
   break;
  }
 } else if ( !str_cmp(hint.c_str(),"viewer") ) {
  EACH(windows.first,GLWindow,g) if ( !str_cmp(search.c_str(),g->name.c_str()) ) {
   if ( !str_cmp(g->classDescription.c_str(),"FireAndForgetLimitedWrappingProce55Viewer2d") ) {
    FireAndForgetLimitedWrappingProce55Viewer2d *p=(FireAndForgetLimitedWrappingProce55Viewer2d *) g;
    p->fromString(code.c_str());
   }
   break;
  }
 }
}

void CodeBlockExecute::Execute( Consulate *es ) {
 // Find procedure at time of execution
 if ( !procedure ) procedure=procedures.Find(this->procedureName.c_str());
 if ( procedure && procedure->lines.count > 0 ) { // Procedure is valid.
  CodeExecutionStackItem *instance=es->execution.Push(&procedure->lines);
  instance->Execute(es);
  es->execution.Pop();
 }
}

void CodeBlockElse::Execute( Consulate *es ) {
 if ( no_expression || !expression ) {
  if ( lines.count > 0 ) {
   CodeExecutionStackItem *instance=es->execution.Push(&lines);
   instance->Execute(es);
   es->execution.Pop();
  }
 } else if ( expression->Evaluate(es) ) {
  if ( lines.count > 0 ) {
   CodeExecutionStackItem *instance=es->execution.Push(&lines);
   instance->Execute(es);
   es->execution.Pop();
  }
 }
}

void CodeBlockInvoke::Execute( Consulate *es ) {
// evaluatedParameters.Recycle(es->variables.Evaluate(parameters.c_str()));
 function->fromString(es,parameters.c_str());
 function->Execute();
}

void CodeBlockWhen::Execute( Consulate *es ) {
 if ( expression->Evaluate(es) ) {
  if ( lines.count > 0 ) {
   CodeExecutionStackItem *instance=es->execution.Push(&lines);
   instance->Execute(es);
   es->execution.Pop();
  }
 }
}

bool CodeExecutionStackItem::Step( Consulate *es ) {
 line->Execute(es);
 line=(CodeBlock *) line->next;
 return (line != null);
}

void CodeExecutionStackItem::Execute( Consulate *es ) {
 if ( line ) while ( Step(es) );
}

void CodeBlocks::Populate( const char *code_ ) {
 Zstring code(code_);
 code.rewind();
 const char *w=code.Next();
 while ( *w != '\0' ) {
  if ( *w == '>' ) { // Execute
   w++;
   CodeBlockExecute *execute=new CodeBlockExecute;
   execute->procedureName(w);
   Append(execute);
  } else if ( *w == '^' ) { // Set
   CodeBlockAssignment *assign=new CodeBlockAssignment;
   assign->reference.Recycle(new CodeReference(w));
   w=code.Next();
   assign->hint=HintToType(w);
   if ( assign->hint > v_unknown ) w=code.Next();
   assign->expression.Recycle(new CodeExpression(w));
   Append(assign);
  } else if ( !str_cmp(w,"set") ) {
   w=code.Next();
   CodeBlockAssignment *assign=new CodeBlockAssignment;
   assign->reference.Recycle(new CodeReference(w));
   w=code.Next();
   assign->hint=HintToType(w);
   if ( assign->hint > v_unknown ) w=code.Next();
   assign->expression.Recycle(new CodeExpression(w));
   Append(assign);
  } else if ( !str_cmp(w,"else") ) {
   if ( !when ) {
    errors.printf("Invalid logic: else before when");
    w=code.Next();
   } else {
    w=code.Next();
    if ( !str_cmp(w,"when") ) {
     w=code.Next();
     Zstring expression(w);
     w=code.Next();
     when->elses.Add(expression.c_str(),w);
    } else {
     when->elses.Add(w);
    }
   }
  } else if ( !str_cmp(w,"if") || !str_cmp(w,"when") ) {
   when=new CodeBlockWhen;
   w=code.Next();
   when->expression.Recycle(new CodeExpression(w));
   w=code.Next();
   when->lines.Populate(w);
   Append(when);
  } else if ( !str_cmp(w,"with") || !str_cmp(w,"as") ) {
   w=code.Next();
   CodeBlockWith *with=new CodeBlockWith;
   with->hint=w;
   w=code.Next();
   with->search=w;
   w=code.Next();
   with->code=w;
   with->consulate->scripts.New(w);
   Append(with);
  } else if ( !str_cmp(w,"execute") || !str_cmp(w,"run") || !str_cmp(w,"go") || !str_cmp(w,"goto") ) {
   w=code.Next();
   CodeBlockExecute *execute=new CodeBlockExecute;
   execute->procedureName(w);
   Append(execute);
  } else {
   BoundFunction *invoked=boundFunctions.Find(w);
   if ( !invoked ) {
    errors.printf("Unknown invocation: `%s`",w);
   } else {
    CodeBlockInvoke *invoke=new CodeBlockInvoke;
    invoke->function=invoked;
    w=code.Next();
    invoke->parameters=w;
    Append(invoke);
   }
  }
  w=code.Next();
 } 
}

/************************************/

#include "Scripting.h"

GLWindowScriptable::GLWindowScriptable( Consulate *es ) : GLWindow() {
 this->classDescription="GLWindowScriptable";
 consulate=es;
}

void GLWindowScriptable::OnLoad() {
 load->Execute();
}

void GLWindowScriptable::Between() {
 if ( visible ) {
  between->Execute();
 }
}

void GLWindowScriptable::Render() {
 if ( visible ) {
  render->Execute();
  fx.Render();
 }
}

#include "FastGUIProcessors.h"
#include "fx_MixedBag.h"

void GLWindowScriptable::fromString( const char *in ) {
 Zstring words(in);
 const char *w=words.Next();
 while ( *w != '\0' ) {
  if ( !str_cmp(w, "drop" ) ) {
   w=words.Next();
   fx.Drop(string(w));
  } else if ( !str_cmp(w, "discard" ) ) {
   this->destroyMe=true;
  } else if ( !str_cmp(w,"name") ) {
   w=words.Next();
   Name(w);
  } else if ( !str_cmp(w,"id") || !str_cmp(w,"code") ) {
   w=words.Next();
   code=atoi(w);
  } else if ( !str_cmp(w,"load") ) {
   w=words.Next();
   load.Recycle();
   load->variables=consulate->variables;
   load->scripts.New(w);
  } else if ( !str_cmp(w,"between") ) {
   w=words.Next();
   between.Recycle();
   between->variables=consulate->variables;
   between->scripts.New(w);
  } else if ( !str_cmp(w,"render") ) {
   w=words.Next();
   render.Recycle();
   render->variables=consulate->variables;
   render->scripts.New(w);
  } else if ( !str_cmp(w,"fullscreen") ) {
   Fullscreen();
  } else if ( !str_cmp(w,"position") ) {
   w=words.Next();
   Zstring subwords(w);
   const char *p=subwords.Next();
   int xx=consulate->variables->EvaluateInteger(p);
   p=subwords.Next();
   int yy=consulate->variables->EvaluateInteger(p);
   this->MoveTo(xx,yy);
  } else if ( !str_cmp(w,"size") ) {
   w=words.Next();
   Zstring subwords(w);
   const char *p=subwords.Next();
   x2=x+consulate->variables->EvaluateInteger(p);
   p=subwords.Next();
   y2=y2+consulate->variables->EvaluateInteger(p);
  } else if ( !str_cmp(w,"centered") ) {
   w=words.Next();
   Zstring subwords(w);
   const char *p=subwords.Next();
   int ww=consulate->variables->EvaluateInteger(p);
   p=subwords.Next();
   int hh=consulate->variables->EvaluateInteger(p);
   SizeAndCenter(ww,hh);
  } else if ( !str_cmp(w,"bg") || !str_cmp(w,"background") ) {
   w=words.Next();
   consulate->variables->EvaluateCrayon(w,&background);
  } else if ( !str_cmp(w,"hide") ) {
   visible=false;
  } else if ( !str_cmp(w,"show") ) {
   visible=true;
  } else if ( !str_cmp(w,"line") ) {
  } else if ( !str_cmp(w,"box") ) {
  } else if ( !str_cmp(w,"circle") ) {
  } else if ( !str_cmp(w,"image") ) {
  } else if ( !str_cmp(w,"button") ) {
   fx_FastGUIButtonScriptable *f=new fx_FastGUIButtonScriptable(consulate);
   fx.Add(f,this);
   w=words.Next();
   f->ScriptModify(w);
  } else if ( !str_cmp(w,"icon") ) {
   fx_FastGUIIconScriptable *f=new fx_FastGUIIconScriptable(consulate);
   fx.Add(f,this);
   w=words.Next();
   f->ScriptModify(w);
  } else if ( !str_cmp(w,"text") ) {
   fx_FastGUITextScriptable *f=new fx_FastGUITextScriptable(consulate);
   fx.Add(f,this);
   w=words.Next();
   f->ScriptModify(w);
  }
  w=words.Next();
 }
};
