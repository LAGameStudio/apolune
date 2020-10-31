#include "fx_UglyText.h"

UglyDrawStyle uglydraw_fromString(const char *s) {
 if ( !str_cmp(s,"3d") ) return ugly3d;
 if ( !str_cmp(s,"outlined") ) return uglyOutlined;
 if ( !str_cmp(s,"shadowed") ) return uglyShadowed;
 if ( !str_cmp(s,"wire") ) return uglyWire;
 return uglyWire;
}

bool fx_UglyTextBox_is_focused;
fx_UglyTextBox *fx_UglyTextBoxFocused;

void fx_UglyTextBoxInit() {
 fx_UglyTextBox_is_focused=false;
 fx_UglyTextBoxFocused=null;
}

void fx_UglyTextBox::Focus() {
 if ( fx_UglyTextBox_is_focused && fx_UglyTextBoxFocused )
  fx_UglyTextBoxFocused->Unfocus();
 focused=true;
 fx_UglyTextBoxFocused=this;
 fx_UglyTextBox_is_focused=true;
 cursorPosition=(int)out.length-1;
 if ( cursorPosition<0 ) cursorPosition=0;
 WhenFocused();
}
void fx_UglyTextBox::Unfocus() {
 focused=false;
 fx_UglyTextBox_is_focused=false;
 fx_UglyTextBoxFocused=null;
}

// Does not turn off individual `focused` flags
void UnfocusAllUglyTextboxes() {
 fx_UglyTextBox_is_focused=false;
 if ( fx_UglyTextBoxFocused ) {
  fx_UglyTextBoxFocused->focused=false;
  fx_UglyTextBoxFocused=null;
 }
}