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

#include "Scripting.h"

class ScriptableProce55or : public Proce55or {
public:
 Zp<Consulate> consulate;
 ScriptableProce55or( Consulate *es ) : Proce55or() {
  consulate=es;
 }
 void Proce55or_ScriptModify( Zstring *words, const char **in ) {
  if ( !str_cmp( (*in), "by" ) || !str_cmp( (*in), "move" ) || !str_cmp( (*in), "moveby" ) ) {
   (*in)=words->Next();
   int by_x=consulate->variables->EvaluateInteger((*in));
   (*in)=words->Next();
   int by_y=consulate->variables->EvaluateInteger((*in));
   MoveBy(by_x,by_y);
  } else if ( !str_cmp( (*in), "to" ) || !str_cmp( (*in), "moveto" ) || !str_cmp( (*in), "reposition" ) ) {
   (*in)=words->Next();
   int to_x=consulate->variables->EvaluateInteger((*in));
   (*in)=words->Next();
   int to_y=consulate->variables->EvaluateInteger((*in));
   MoveTo(to_x,to_y);
  } else if ( !str_cmp( (*in), "duration" ) ) {
   (*in)=words->Next();
   duration=consulate->variables->EvaluateDecimal((*in));
  } else if ( !str_cmp( (*in), "delay" ) ) {
   (*in)=words->Next();
   duration=consulate->variables->EvaluateDecimal((*in));
  } else if ( !str_cmp( (*in), "hide" ) ) {
   hidden=true;
  } else if ( !str_cmp( (*in), "show" ) ) {
   hidden=false;
  } else if ( !str_cmp( (*in), "group") ) {
   (*in)=words->Next();
   grouping=consulate->variables->EvaluateInteger((*in));
  } else if ( !str_cmp( (*in), "name") ) {
   (*in)=words->Next();
   name=consulate->variables->EvaluateString((*in));
  } 
 }
};