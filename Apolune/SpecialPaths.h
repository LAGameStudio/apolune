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
#include <Windows.h>
#include <Shlobj.h>

#include "string_functions.h"
#include "ZeroTypes.h"

class SpecialPaths {
public:
 Zstring out;
 Zstring working_directory; // set during app load using __getcwd
 Zstring data_folder;
 Zstring saved_documents;
 Zstring history_folder;
 SpecialPaths(void) {
#if defined(WIN32)
  PWSTR path;
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &path);
  if ( hr == E_FAIL ) {
   OUTPUT("SpecialPaths: SHGetKnownFolderPath() returned E_FAIL unexpectedly. Among other things, this value can indicate that the rfid parameter references a KNOWNFOLDERID which does not have a path (such as a folder marked as KF_CATEGORY_VIRTUAL).\n");
  } else if ( hr == E_INVALIDARG ) {
   OUTPUT("SpecialPaths: SHGetKnownFolderPath() returned E_INVALIDARG unexpectedly. Among other things, this value can indicate that the rfid parameter references a KNOWNFOLDERID that is not present on the system. Not all KNOWNFOLDERID values are present on all systems. Use IKnownFolderManager::GetFolderIds to retrieve the set of KNOWNFOLDERID values for the current system.\n" );
  }
  cvtLPW2stdstring(saved_documents.value,path); saved_documents.rewind();
  CoTaskMemFree(path);
  history_folder=saved_documents+string("\\ApplicationData\\");
  if ( make_folder(history_folder.c_str()) ) OUTPUT("SpecialPaths created the history folder, or it already existed.\n");
  else OUTPUT("SpecialPaths tried to create the history folder but was unsuccessful in doing so.\n");
#endif
  OUTPUT("SpecialPaths selected:\n[saved_documents> %s\n[history_folder> %s\n", saved_documents.c_str(), history_folder.c_str() );
 }
 string historic( const char *fn ) { return history_folder+string(fn); }
 string historic( string fn ) { return history_folder+fn; }
 string historic( Zstring& fn ) { return history_folder+fn.value; }
};

extern SpecialPaths specialPaths;