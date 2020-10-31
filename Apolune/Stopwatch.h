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

class Stopwatch : public ListItem {
public:
 Zstring name;
 Zint started,stopped,duration;
 Zdouble seconds;
 Stopwatch() : ListItem() {}
 Stopwatch( const char *s ) : ListItem() { name=s; }
 void Go() {
  started=(int)GetTickCount64();
 }
 void Stop() {
  stopped=(int)GetTickCount64();  // MSVC 2015 Code Analyzer: c:\users\moops\documents\visual studio 2015\projects\framework\framework\stopwatch.h(27): warning C28159: Consider using 'GetTickCount64' instead of 'GetTickCount'. Reason: GetTickCount overflows roughly every 49 days.  Code that does not take that into account can loop indefinitely.  GetTickCount64 operates on 64 bit values and does not have that problem
  duration=stopped-started;
  seconds=(double)duration/1000.0;
 }
 int instant() {
  Stop(); return duration;
 }
 string toStringMS() {
  return FORMAT("%dm %ds", (int) (seconds/60.0), ((int)seconds)%60 );
 }
 string toStringS() {
  return FORMAT("%1.4f seconds", (double) seconds);
 }
 string toString() {
  return FORMAT("%d milliseconds", (int) duration);
 }
};

class Stopwatches : public LinkedList {
public:
 void Add(const char *s) {
  Append(new Stopwatch(s));
 }
 void Add() { Append(new Stopwatch); }
 double Seconds() {
  double tally=0.0;
  FOREACH(Stopwatch,s) tally+=s->seconds;
  return tally;
 }
 int Millis() {
  int tally=0;
  FOREACH(Stopwatch,s) tally+=(int)s->duration;
  return tally;
 }
};