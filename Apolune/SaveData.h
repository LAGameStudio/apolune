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

#include "Strings.h"

enum SaveData {
 ___int=0,
 ___float=1,
 ___double=2,
 ___string=3,
 ___char=4,
 ___text=5,
 ___color=6,
 ___tween=7,
 ___primitive=8,
 ___subclass=9
};

class SaveElement : public ListItem {
public:
 bool serial;
 int series;
 string key;
 SaveElement( string name, int i ) {
  key=name;
  series=1;
 }
 SaveElement( string name, int a, int b ) {
  key=name;
  series=1;
 }
 SaveElement( string name, int a, int b, int c ) {
  key=name;
  series=1;
 }
 SaveElement( string name, int a, int b, int c, int d ) {
  key=name;
  series=1;
 }
 SaveElement( string name, float f ) {
  key=name;
  series=1;
 }
 SaveElement( string name, float *a, float *b ) {
  key=name;
  series=2;
 }
 SaveElement( string name, float *a, float *b, float *c ) {
  key=name;
  series=3;
 }
 SaveElement( string name, float *a, float *b, float *c, float *d ) {
  key=name;
  series=4;
 }
 SaveElement( string name, double *d ) {
  key=name;
  series=1;
 }
 SaveElement( string name, double *a, double *b ) {
  key=name;
  series=2;
 }
 SaveElement( string name, double *a, double *b, double *c ) {
  key=name;
  series=3;
 }
 SaveElement( string name, double *a, double *b, double *c, double *d ) {
  key=name;
  series=4;
 }
 SaveElement( string name, string *s ) {
  key=name;
 }
 SaveElement( string name, char *c ) {
  key=name;
 }
 SaveElement( string name, char *c, int length ) {
  key=name;
 }

 virtual void Lookup() {
 }
};

class SaveText {
public:
};