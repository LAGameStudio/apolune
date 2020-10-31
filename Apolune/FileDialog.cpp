#include "FileDialog.h"

void FileDialog::Activate() {
 if ( only ) limbo.Push();
 activated=true;
 windows.Add(modal=new ModalFileDialogWindow(this));
}

void FileDialog::Deactivate() {
 if ( only ) limbo.Pop();
 activated=false;
 modal->destroyMe=true;
 modal=null;
}