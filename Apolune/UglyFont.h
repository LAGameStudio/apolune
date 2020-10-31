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
#include "ZeroTypes.h"
#include "ListItem.h"
#include "Crayon.h"

void CreateUglyFont();

double TextLength(string t, double textWidth=5.0, double textHeight=7.0, double kern=0.05, double lineSpaceMultiple=1.0 );
double TextLength(const char *t, double textWidth=5.0, double textHeight=7.0, double kern=0.05, double lineSpaceMultiple=1.0 );

void Text(string t, Crayon tint, Blends blend, double x, double y, double textWidth=5.0, double textHeight=7.0, double kern=0.05, double lineSpaceMultiple=1.0, bool centered=false );
void Text(const Zstring& t, Crayon tint, Blends blend, double x, double y, double textWidth=5.0, double textHeight=7.0, double kern=0.05, double lineSpaceMultiple=1.0, bool centered=false );
void Text(const char * t, Crayon tint, Blends blend, double x, double y, double textWidth=5.0, double textHeight=7.0, double kern=0.05, double lineSpaceMultiple=1.0, bool centered=false );
void UglyText(const char * t, Crayon tint, Blends blend, double x, double y, double textWidth=5.0, double textHeight=7.0, double kern=0.05, double lineSpaceMultiple=1.0, bool centered=false );

void Text( const char *t, double x, double y );
void Text( const char *t, double x, double y, double textWidth, double textHeight );
void Text( const char *t, double x, double y, double textWidth, double textHeight, bool centered );
void Text( const char *t, double x, double y, double textWidth, double textHeight, double kern, double lineSpaceMultiple );
void Text( const char *t, Crayon tint, double x, double y );
void Text( const char *t, Crayon tint, double x, double y, double textWidth, double textHeight );
void Text( const char *t, Crayon tint, double x, double y, double textWidth, double textHeight, double kern, double lineSpaceMultiple );

void Text( string t, double x, double y );
void Text( string t, double x, double y, double textWidth, double textHeight );
void Text( string t, double x, double y, double textWidth, double textHeight, bool centered );
void Text( string t, double x, double y, double textWidth, double textHeight, double kern, double lineSpaceMultiple );
void Text( string t, Crayon tint, double x, double y );
void Text( string t, Crayon tint, double x, double y, double textWidth, double textHeight );
void Text( string t, Crayon tint, double x, double y, double textWidth, double textHeight, double kern, double lineSpaceMultiple );

void Text( const Zstring &t, double x, double y );
void Text( const Zstring &t, double x, double y, double textWidth, double textHeight );
void Text( const Zstring &t, double x, double y, double textWidth, double textHeight, bool centered );
void Text( const Zstring &t, double x, double y, double textWidth, double textHeight, double kern, double lineSpaceMultiple );
void Text( const Zstring &t, Crayon tint, double x, double y );
void Text( const Zstring &t, Crayon tint, double x, double y, double textWidth, double textHeight );
void Text( const Zstring &t, Crayon tint, double x, double y, double textWidth, double textHeight, double kern, double lineSpaceMultiple );