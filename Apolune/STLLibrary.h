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
#include "LinkedList.h"
#include "STLFile.h"
#include "VBOGroup.h"

class STLLibrary : public LinkedList {
public:
 STLFile *Load(const char *filename) {
  FOREACH(STLFile,file) {
   if ( file->filename == filename ) return file;
  }
  STLFile *file=new STLFile;
  if ( !file ) {
   OUTPUT("STLLibrary:Load(`%s`) cannot allocate memory to load a file\n",filename);
   return null;
  }
  file->Load(filename);
  if ( file->error ) {
   OUTPUT("STLLibrary:Load(`%s`) reports an error with the file, it may not exist or is corrupt\n",filename);
   delete file;
   return null;
  }
  Append(file);
  return file;
 }
 CLEARLISTRESET(STLFile);
};

extern STLLibrary stls;