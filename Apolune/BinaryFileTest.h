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
#include "GLWindow.h"

class BinaryFileTest : public GLWindow {
public:
 void OnLoad() {
  FileWrite();
  FileRead();
 }
 void FileRead() {
  BinaryFile read1("data/out/test1.bin",false);
  float f1,f2;
  read1.read(&f1);
  for ( int k=0; k<100; k++ ) read1.read(&f2);
  OUTPUT("F1=%f F2=%f ", f1,f2);
  BinaryFile read2("data/out/test2.bin",false);
  unsigned int i,j;
  for ( int k=0; k<101; k++ ) read2.read(&i);
  for ( int k=0; k<102; k++ ) read2.read(&j);
  OUTPUT("i=%d j=%d ", i,j);
  BinaryFile read3("data/out/test3.bin",false);
  uint32 a,b;
  for ( int k=0; k<103; k++ ) read3.read(&a);
  for ( int k=0; k<104; k++ ) read3.read(&b);
  OUTPUT("a=%d b=%d\n",(unsigned int) a, (unsigned int) b);
 }
 void FileWrite() {
  BinaryFile file1("data/out/test1.bin",true);
  file1.scrawl(1.111f);
  float f=2.222f;
  for ( int k=0; k<100; k++ ) file1.scrawl(f);
  BinaryFile file2("data/out/test2.bin",true);
  for ( int k=0; k<101; k++ ) file2.scrawl((unsigned int) 1);
  unsigned int i=2;
  for ( int k=0; k<102; k++ ) file2.scrawl(i);
  BinaryFile file3("data/out/test3.bin",true);
  unsigned int j=2;
  for ( int k=0; k<103; k++ ) file3.scrawl( (uint32) 1 );
  for ( int k=0; k<104; k++ ) file3.scrawl( (uint32) j );
 }
};
