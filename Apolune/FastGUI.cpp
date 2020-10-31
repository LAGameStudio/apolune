#include "FastGUI.h"

#include "VirtualWindow.h"

FastGUI fast;


int FastGUI::bestrectforaspect( double w, double h ) {
 double asp=w/h;
 double invasp=h/w;
 if ( asp > 1.0 ) return art.stash.rr_Transition+URANGE(0,(int)(invasp*19.0),18);
 else return art.stash.rr_Transition+URANGE(0,(int)((1.0-asp)*19.0),18);
}

void FullscreenStringEditor::Render() {
 if ( visible ) {
  art.PushViewport(fbo);
  fbo->Bind();
  (*out)=fast.textbox(font,fontBlend,labelFont,labelFontBlend,label.c_str(),(*out).value,32,32,75,limited,(fbo->h)/32,2,4,true,0,false);
  if ( fast.button("Back",fbo->w-8-48,8,48,24,false) ) {
   fbo->Unbind();
   fast.ignoreNextRelease=true;
   this->destroyMe=true;
   back_to->visible=true;
   return;
  }
  fbo->Unbind();
  art.PopViewport();
  fbo->RenderUpsideDown(0,0,display->w,display->h);
 }
}


void FullscreenStringTerminalEditor::Render() {
 if ( visible ) {
  if ( helpF1 ) {
   fbo->Bind();
   WriteTexFontColorCodes(termFont,crayons.Pick(alabaster),1.0f,transparency,8,8,20,26,2,helpText.c_str(),-6);
   fbo->Unbind();
   fbo->RenderUpsideDown(0,0,display->w,display->h);
   return;
  }
  art.PushViewport(fbo);
  fbo->Bind();
  Editor();
  fast.text(FORMAT("---------- Column %3d      Line %3d      F1=help",(int)cursorPosition.x+1,(int)cursorPosition.y+1).c_str(),16,fbo->h-20,false,cursorColor);
  if ( fast.button("Back",fbo->w-8-48,8,48,24,false) ) {
   fbo->Unbind();
   fast.ignoreNextRelease=true;
   this->destroyMe=true;
   back_to->visible=true;
   return;
  }
  fbo->Unbind();
  art.PopViewport();
  fbo->RenderUpsideDown(0,0,display->w,display->h);
 }
}


void FullscreenStringClassEditor::Render() {
 if ( visible ) {
  fbo->Bind();
  out->s=fast.textbox(font,fontBlend,labelFont,labelFontBlend,label.c_str(),out->s,32,32,75,limited,(fbo->h)/32,2,4,true,0,false);
  if ( fast.button("BACK",fbo->w-8-48,8,48,24,false) ) {
   fbo->Unbind();
   fast.ignoreNextRelease=true;
   this->destroyMe=true;
   back_to->visible=true;
   return;
  }
  fbo->Unbind();
  fbo->RenderUpsideDown(0,0,display->w,display->h);
 }
}


void FullscreenStringClassTerminalEditor::Render() {
 if ( visible ) {
  if ( helpF1 ) {
   fbo->Bind();
   WriteTexFontColorCodes(termFont,crayons.Pick(alabaster),1.0f,transparency,8,8,20,26,2,helpText.c_str(),-6);
   fbo->Unbind();
   fbo->RenderUpsideDown(0,0,display->w,display->h);
   return;
  }
  fbo->Bind();
  Editor();
  fast.text(FORMAT("---------- Column %3d Line %3d F1:help",(int)cursorPosition.x+1,(int)cursorPosition.y+1).c_str(),16,fbo->h-20,false,cursorColor);
  if ( fast.button("BACK",fbo->w-8-48,8,48,24,false) ) {
   fbo->Unbind();
   fast.ignoreNextRelease=true;
   this->destroyMe=true;
   back_to->visible=true;
   return;
  }
  fbo->Unbind();
  fbo->RenderUpsideDown(0,0,display->w,display->h);
 }
}

 void FastGUI::GetState(int x, int y, int w, int h, bool dontIgnoreNextRelease ) {
  lastDimensions.SetRect(x,y,w,h);
  bool out_of_scope=false;
  mouse.Set(input.mxd*vdisplay.w,input.myd*vdisplay.h);
  mouse.x-=scope.x;
  mouse.y-=scope.y;
  mouse.x/=scope.w;
  mouse.y/=scope.h;
  if ( WITHIN(mouse.x,mouse.y,0.0,0.0,1.0,1.0) ) {
   mouse.x*=vdisplay.w;
   mouse.y*=vdisplay.h;
   mouse.x-=(double)x;
   mouse.y-=(double)y;
   mouse.x/=(double)w;
   mouse.y/=(double)h;
   if ( WITHIN(mouse.x,mouse.y,0.0,0.0,1.0,1.0) ) {
    point.x=mouse.x;
    point.y=mouse.y;
   } else out_of_scope=true;
  } else out_of_scope=true;
  if ( vwindows && member ) {
   if ( vwindows->focused != member ) out_of_scope=true;
  }
  if ( out_of_scope ) {
   point.x=-1.0;
   point.y=-1.0;
   hovering=false;
   cancelling=false;
   clicking=false;
   cancelled=false;
   clicked=false;
   return;
  } else {
   hovering   = input.Contact();
   if ( hovering ) hovered=true;
   clicking   = hovering && (input.left>0);
   cancelling = hovering && (input.right>0);
   clicked    = hovering && (input.leftReleased() || (input.hasTouch && (input.left>0))) && (ignoreNextRelease == 0.0);
   cancelled  = hovering && input.rightReleased()  && (ignoreNextRelease == 0.0);
   if ( !dontIgnoreNextRelease && (clicked || cancelled) ) {
    ignoreNextRelease=FAST_BUTTON_TIMEOUT;
   }
  }
  SetLastState(hovering,clicking,cancelling,clicked,cancelled);
 }

#include "FileDialog.h"

// Use these...
 
 GLImage *FastGUI::imagepick( GLImage *value, const char *label, FileDialog *picker, Zstring *bound, Crayon imageTint, int x, int y, int w, int h, bool wrapImages, bool allowNull ) {
  image( value, x,y,w,h, imageTint );
  picker->w=w;
  picker->MoveTo(x,y+h);
  picker->Render();
  Text(digicaps,crayons.Pick(alabaster),additive,(x),y-(int)fontSize.y,(int)fontSize.x,(int)fontSize.y,0,0,label);
  if ( picker->changed ) {
   if ( bound->ending(".png") || bound->ending(".jpg") ) {
    Zp<GLImage> newImage=wrapImages ? library.Load(bound->c_str(),tllto_Tiling) : library.Load(bound->c_str());
    picker->changed=false;
    if ( newImage ) return newImage;
   }
  }
  if ( allowNull && smbutton("X",x+w-8-16,y+8,16,16) ) {
   return null;
  }
  return value;
 }
 GLImage *FastGUI::imagepick( TexFont *font, GLImage *value, const char *label, FileDialog *picker, Zstring *bound, Crayon imageTint, int x, int y, int w, int h, bool wrapImages, bool allowNull ) {
  image( value, x,y,w,h, imageTint );
  picker->w=w;
  picker->MoveTo(x,y+h);
  picker->Render();
  Text(font,crayons.Pick(alabaster),tblend,(x),y-tfont.h,tfont.w,fast.tfont.h,0,0,label);
  if ( picker->changed ) {
   if ( bound->ending(".png") || bound->ending(".jpg") ) {
    Zp<GLImage> newImage=wrapImages ? library.Load(bound->c_str(),tllto_Tiling) : library.Load(bound->c_str());
    picker->changed=false;
    if ( newImage ) return newImage;
   }
  }   
  if ( allowNull && smbutton("X",x+w-8-16,y+8,16,16) ) {
   return null;
  }
  return value;
 }
  
 GLImage *FastGUI::imagepick( GLImage *value, const char *label, FileDialog *picker, Crayon imageTint, int x, int y, int w, int h, bool wrapImages, bool allowNull ) {
  static Zstring b;
  image( value, x,y,w,h, imageTint );
  picker->w=w;
  picker->MoveTo(x,y+h);
  picker->Render();
  Text(digicaps,crayons.Pick(alabaster),additive,(x),y-(int)fontSize.y,(int)fontSize.x,(int)fontSize.y,0,0,label);
  if ( picker->changed ) {
   b=picker->bound->c_str();
   if ( b.ending(".png") || b.ending(".jpg") ) {
    Zp<GLImage> newImage=wrapImages ? library.Load(b.c_str(),tllto_Tiling) : library.Load(b.c_str());
    picker->changed=false;
    if ( newImage ) return newImage;
   }
  }
  if ( allowNull && smbutton("X",x+w-8-16,y+8,16,16) ) {
   return null;
  }
  return value;
 }
 
 GLImage *FastGUI::imagepick( TexFont *font, GLImage *value, const char *label, FileDialog *picker, Crayon imageTint, int x, int y, int w, int h, bool wrapImages, bool allowNull ) {
  static Zstring b;
  image( value, x,y,w,h, imageTint );
  picker->w=w;
  picker->MoveTo(x,y+h);
  picker->Render();
  Text(font,crayons.Pick(alabaster),tblend,(x),y-tfont.h,tfont.w,fast.tfont.h,0,0,label);
  if ( picker->changed ) {
   b=picker->bound->c_str();
   if ( b.ending(".png") || b.ending(".jpg") ) {
    Zp<GLImage> newImage=wrapImages ? library.Load(b.c_str(),tllto_Tiling) : library.Load(b.c_str());
    picker->changed=false;
    if ( newImage ) return newImage;
   }
  }   
  if ( allowNull && smbutton("X",x+w-8-16,y+8,16,16) ) {
   return null;
  }
  return value;
 }

bool FastGUI::fileDialog(FileDialog *in, int x, int y, int w, int h) {
 in->w=w;
 in->h=h;
 in->MoveTo(x,y);
 in->Render();
 return in->changed;
}

GLImage *FastGUI::imagePicker(FileDialog *in, GLImage *value,int x, int y, int w, int h, int bh,bool xToNull) {
 image(value,x,y,w,h,crayons.Pick(alabaster));
 in->w=w;
 in->h=bh;
 in->MoveTo(x,y+h);
 in->Render();
 GLImage *result=value;
 if ( in->changed ) {
  Zstring v((in->bound)->c_str());
  if ( v.ending(".png") || v.ending(".jpg") ) {
   GLImage *newImage=library.Load((in->bound)->c_str());
   if ( newImage ) result=newImage;
  }
  in->changed=false;
 }
 if ( xToNull && fast.smbutton("X",x+w-4-16,y+4,16,16) ) result=null;
 return result;
}

GLImage *FastGUI::imagePicker(FileDialog *in, GLImage *value,Crayon tint,int x, int y, int w, int h, int bh,bool xToNull) {
 image(value,x,y,w,h,tint);
 in->w=w;
 in->h=bh;
 in->MoveTo(x,y+h);
 in->Render();
 GLImage *result=value;
 if ( in->changed ) {
  Zstring v((in->bound)->c_str());
  if ( v.ending(".png") || v.ending(".jpg") ) {
   GLImage *newImage=library.Load((in->bound)->c_str());
   if ( newImage ) result=newImage;
  }
  in->changed=false;
 }
 if ( xToNull && fast.smbutton("X",x+w-4-16,y+4,16,16) ) result=null;
 return result;
}

GLImage *FastGUI::imagePicker(FileDialog *in, GLImage *value,Crayon tint,Blends b,int x, int y, int w, int h, int bh,bool xToNull) {
 image(value,x,y,w,h,tint,b);
 in->w=w;
 in->h=bh;
 in->MoveTo(x,y+h);
 in->Render();
 GLImage *result=value;
 if ( in->changed ) {
  Zstring v((in->bound)->c_str());
  if ( v.ending(".png") || v.ending(".jpg") ) {
   GLImage *newImage=library.Load((in->bound)->c_str());
   if ( newImage ) result=newImage;
  }
  in->changed=false;
 }
 if ( xToNull && fast.smbutton("X",x+w-4-16,y+4,16,16) ) result=null;
 return result;
}

/// box ////////////////////////////////////////////////////////////////////////////////////////////

 void FastGUI::box( FastExtents e ) { box( e.x, e.y, e.w, e.h ); }
 void FastGUI::box( int x, int y, int w, int h ) {
//  mD.Render2dVC((float)x,(float)y,(float)w,(float)h);
 }
 void FastGUI::box( FastGuiBoxTypes fg, FastExtents e ) { box( fg, e.x, e.y, e.w, e.h ); }
 void FastGUI::box( FastGuiBoxTypes fg, int x, int y, int w, int h ) {
  box( color, fg, x, y, w, h ); 
 } 
 void FastGUI::box( Crayon c, FastGuiBoxTypes fg, FastExtents e ) { box( c, fg, e.x, e.y, e.w, e.h ); }
 void FastGUI::box( Crayon c, FastGuiBoxTypes fg, int x, int y, int w, int h, Blends blend ) {
  box(c,fg,(double)x,(double)y,(double)w,(double)h,blend);
 }
 void FastGUI::box( Crayon c, FastGuiBoxTypes fg, double x, double y, double w, double h, Blends blend ) {
  switch ( fg ) {
   case fg_C: // Round Rect (TV screen)
     bestrect(c,blend,x,y,w,h);
//     art.Element(c,blend,art.stash.rr_Transition+10,x,y,w,h);
    break;
   case fg_D: // Round Rect (elongated)
     bestrect(c,blend,x,y,w,h);
//     art.Element(c,blend,art.stash.rr_Transition+10,x,y,w,h);
    break;
   default:
   case fg_K:
   case fg_E: // Round Rect (Filled Square)
     bestrect(c,blend,x,y,w,h);
//     art.Element(c,blend,art.stash.rr_Transition+20,x,y,w,h);
    break;
   case fg_F: // Round Rect (Empty Checkbox)
     art.Element2(c,crayons.Pick(black),blend,stashed_ThickSquare,x,y,w,h,2.0);
    break;
   case fg_H: case fg_I: case fg_J: case fg_G: // Empty Square
     art.Element2(c,crayons.Pick(black),blend,stashed_ThickSquare,x,y,w,h);
    break;
   case fg_L: // Switch (Up)
     art.Element2(c,crayons.Pick(black),blend,stashed_TallRect,x,y,w,h,2.0);
     art.Element(c,blend,stashed_squareRect,x+3.0,y+3.0,w-6.0,h/2.0-3.0);
    break;
   case fg_M: // Switch (Down))
     art.Element2(c,crayons.Pick(black),blend,stashed_TallRect,x,y,w,h,2.0);
     art.Element(c,blend,stashed_squareRect,x+3.0,y+h/2.0,w-6.0,h/2.0-3.0);
    break;
  }  
 }
 
/// bargraph ////////////////////////////////////////////////////////////////////////////////////////////

 void FastGUI::bargraph( Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled ) {
  if ( disabled ) {
   art.Element(crayons.Pick(gray),none,art.stash.rr_Button,(double)x,(double)y,(double)w,(double)h);//mlD->Render2d(crayons.Pick(gray), (double)x,(double)y,(double)w,(double)h,vdisplay.w,vdisplay.h); 
  } else {
   int fw=(int)(filled*(double)w);
   art.Element(empty,none,fast.stashed_BarGraphRect,(double)x,(double)y,(double)w,(double)h); 
   art.Element(full,none,fast.stashed_BarGraphRect,(double)x,(double)y,(double)fw,(double)h);
  }
 }
 void FastGUI::bargraphv( Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled ) {
  if ( disabled ) {
   art.Element(crayons.Pick(gray),transparency,art.stash.rr_Vertical,(double)x,(double)y,(double)w,(double)h);
  } else {
   int fh=(int)(filled*(double)h);
   art.Element(empty,transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)w,(double)h); 
   art.Element(full,transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)w,(double)fh);
  }
 }
 void FastGUI::bargraph( const char *label, Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled ) {
  if ( disabled ) {
   art.Element(crayons.Pick(gray),transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)w,(double)h);
  } else {
   int fw=(int)(filled*(double)w);
   art.Element(empty,transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)w,(double)h); 
   art.Element(full,transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)fw,(double)h);
   glLineWidth(3.0);
   text(label,x+w/2,y+h/2,true,empty.Anti());
   glLineWidth(1.0);
   text(label,x+w/2,y+h/2,true,full.Anti());
  }
 }
 void FastGUI::bargraphv( const char *label, Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled ) {
  if ( disabled ) {
   art.Element(crayons.Pick(gray),transparency,art.stash.rr_Vertical,(double)x,(double)y,(double)w,(double)h);
  } else {
   int fh=(int)(filled*(double)h);
   art.Element(empty,transparency,art.stash.rr_Vertical,(double)x,(double)y,(double)w,(double)h); 
   art.Element(full,transparency,art.stash.rr_Vertical,(double)x,(double)(y+(h-fh)),(double)w,(double)fh);
   text(label,x+w/2,y+h/2,true,empty.Anti());
   text(label,x+w/2,y+h/2,true,full.Anti());
  }
 }
 void FastGUI::bargraphcap( const char *label, Crayon empty, Crayon full, double filled, int x, int y, int w, int h, bool disabled ) {
  if ( disabled ) {
   art.Element(crayons.Pick(gray),transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)w,(double)h);
  } else {
   int fw=(int)(filled*(double)w);
   art.Element(empty,transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)w,(double)h/2); 
   art.Element(full,transparency,fast.stashed_BarGraphRect,(double)x,(double)y,(double)fw,(double)h/2);
   text(label,x,y+h/2,false,crayons.Pick(alabaster));
  }
 }

 
/// button ////////////////////////////////////////////////////////////////////////////////////////////
 // backless button

 bool FastGUI::bbutton( FastGuiBoxTypes fg, Crayon c, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  if ( selection ) selected.gl(); else if ( hovering ) activated.gl(); else color.gl();
  box(c,fg,x,y,w,h );
//  Stretchi( rounded, hovering ? hover : color,    1.0f, none,     x,y,w,h );
  ( hovering ? (clicking ? excited : highlight) : (selection ? crayons.Pick(alabaster) : tint) ).gl(); 
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }
 bool FastGUI::bbutton( FastGuiBoxTypes fg, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  if ( selection ) selected.gl(); else if ( hovering ) activated.gl(); else color.gl();
  box(fg, x,y,w,h );
//  Stretchi( rounded, hovering ? hover : color,    1.0f, none,     x,y,w,h );
  ( hovering ? (clicking ? excited : highlight) : (selection ? crayons.Pick(alabaster) : tint) ).gl(); 
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }
 bool FastGUI::bbutton( FastGuiBoxTypes fg, const char *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  if ( hovering ) activated.gl(); else color.gl();
  box(fg,x,y,w,h);
  Text(label,( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 // roundrect button
 bool FastGUI::rrbutton( GLImage *label, FastExtents e ) { return rrbutton( label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::rrbutton( GLImage *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  if ( hovering ) rrect(rr_hover_border,rr_hover_inner,rr_hover_outer,x,y,w,h);
  else if ( clicking ) rrect(rr_select_border,rr_select_inner,rr_select_outer,x,y,w,h);
  else rrect(rr_border,rr_inner,rr_outer,x,y,w,h);
  Stretchi( label, ( hovering ? (clicking ? rr_select_text : rr_hover_text) : rr_text ), 1.0f, additive, x,y,w,h );
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }
 bool FastGUI::rrbutton( GLImage *label, FastExtents e, bool selection ) { return rrbutton( label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::rrbutton( GLImage *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  if ( hovering ) {
   if ( selection ) rrect(rr_hover_active_border,rr_hover_active_inner,rr_hover_active_outer,x,y,w,h);
   else rrect(rr_hover_border,rr_hover_inner,rr_hover_outer,x,y,w,h);
  } else if ( clicking ) {
   if ( selection ) rrect(rr_active_select_border,rr_active_select_inner,rr_active_select_outer,x,y,w,h);
   else rrect(rr_select_border,rr_select_inner,rr_select_outer,x,y,w,h);
  } else if ( selection ) rrect(rr_border,rr_select_inner,rr_select_outer,x,y,w,h);
  else rrect(rr_border,rr_inner,rr_outer,x,y,w,h);
  if ( label ) Stretchi( label, ( hovering ? (selection?rr_hover_active_text:rr_hover_text) : (clicking? (selection?rr_active_select_text:rr_select_text) : rr_text) ), 1.0f, additive, x,y,w,h );
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }
 bool FastGUI::rrbutton( const char *label, FastExtents e ) { return rrbutton( label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::rrbutton( const char *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  if ( hovering ) rrect(rr_hover_border,rr_hover_inner,rr_hover_outer,x,y,w,h);
  else if ( clicking ) rrect(rr_select_border,rr_select_inner,rr_select_outer,x,y,w,h);
  else rrect(rr_border,rr_inner,rr_outer,x,y,w,h);
  Text(label,( hovering ? (clicking ? rr_select_text : rr_hover_text) : rr_text ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::rrbutton( const char *label, FastExtents e, bool selection ) { return rrbutton( label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::rrbutton( const char *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  if ( hovering ) {
   if ( selection ) rrect(rr_hover_active_border,rr_hover_active_inner,rr_hover_active_outer,x,y,w,h);
   else rrect(rr_hover_border,rr_hover_inner,rr_hover_outer,x,y,w,h);
  } else if ( clicking ) {
   if ( selection ) rrect(rr_active_select_border,rr_active_select_inner,rr_active_select_outer,x,y,w,h);
   else rrect(rr_select_border,rr_select_inner,rr_select_outer,x,y,w,h);
  } else if ( selection ) rrect(rr_border,rr_select_inner,rr_select_outer,x,y,w,h);
  else rrect(rr_border,rr_inner,rr_outer,x,y,w,h);
  Text(label,( hovering ? (selection?rr_hover_active_text:rr_hover_text) : (clicking? (selection?rr_active_select_text:rr_select_text) : rr_text) ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::rrbutton( const char *label, FastExtents e, bool selection, int indent ) { return rrbutton( label, e.x, e.y, e.w, e.h, selection, indent ); }
 bool FastGUI::rrbutton( const char *label, int x, int y, int w, int h, bool selection, int indent ) {
  GetState(x,y,w,h);
  if ( hovering ) {
   if ( selection ) rrect(rr_hover_active_border,rr_hover_active_inner,rr_hover_active_outer,x,y,w,h);
   else rrect(rr_hover_border,rr_hover_inner,rr_hover_outer,x,y,w,h);
  } else if ( clicking ) {
   if ( selection ) rrect(rr_active_select_border,rr_active_select_inner,rr_active_select_outer,x,y,w,h);
   else rrect(rr_select_border,rr_select_inner,rr_select_outer,x,y,w,h);
  } else if ( selection ) rrect(rr_border,rr_select_inner,rr_select_outer,x,y,w,h);
  else rrect(rr_border,rr_inner,rr_outer,x,y,w,h);
  if ( indent==0 ) {
   Text(label,( hovering ? (selection?rr_hover_active_text:rr_hover_text) : (clicking? (selection?rr_active_select_text:rr_select_text) : rr_text) ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  } else {
   Text(label,( hovering ? (selection?rr_hover_active_text:rr_hover_text) : (clicking? (selection?rr_active_select_text:rr_select_text) : rr_text) ),transparency,(double)(x+indent),(double)y+(double)h/2.0+(double)h/4.0,fontSize.x,fontSize.y,charSpace,lineSpace,false);
  }
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 
 // round rectangle small button

 bool FastGUI::rrsmbutton( const char *label, FastExtents e ) { return rrsmbutton( label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::rrsmbutton( const char *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  if ( hovering ) rrect(rr_hover_border,rr_hover_inner,rr_hover_outer,x,y,w,h);
  else if ( clicking ) rrect(rr_select_border,rr_select_inner,rr_select_outer,x,y,w,h);
  else rrect(rr_border,rr_inner,rr_outer,x,y,w,h);
  Text(label,( hovering ? (clicking ? rr_select_text : rr_hover_text) : rr_text ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::rrsmbutton( const char *label, FastExtents e, bool selection ) { return rrsmbutton( label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::rrsmbutton( const char *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  if ( hovering ) {
   if ( selection ) rrect(rr_hover_active_border,rr_hover_active_inner,rr_hover_active_outer,x,y,w,h);
   else rrect(rr_hover_border,rr_hover_inner,rr_hover_outer,x,y,w,h);
  } else if ( clicking ) {
   if ( selection ) rrect(rr_active_select_border,rr_active_select_inner,rr_active_select_outer,x,y,w,h);
   else rrect(rr_select_border,rr_select_inner,rr_select_outer,x,y,w,h);
  } else if ( selection ) rrect(rr_border,rr_select_inner,rr_select_outer,x,y,w,h);
  else rrect(rr_border,rr_inner,rr_outer,x,y,w,h);
  Text(label,( hovering ? (selection?rr_hover_active_text:rr_hover_text) : (clicking? (selection?rr_active_select_text:rr_select_text) : rr_text) ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 
/// area ////////////////////////////////////////////////////////////////////////////////////////////

 bool FastGUI::hoverarea( int x, int y, int w, int h ) {
  GetState(x,y,w,h,true);
  lastSize.SetRect(x,y,w,h);
  return hovering;
 }
 bool FastGUI::buttonarea( int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }

// button 


 bool FastGUI::button( GLImage *label, FastExtents e ) { return button( label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::button( GLImage *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  art.Element(hovering?activated:color,none,art.stash.rr_Button,(double)x,(double)y,(double)w,(double)h);
//  Stretchi( rounded, hovering ? hover : color,    1.0f, none,     x,y,w,h );
  Stretchi( label, ( hovering ? (clicking ? excited : highlight) : tint ), 1.0f, additive, x,y,w,h );
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }
 bool FastGUI::button( GLImage *label, FastExtents e, bool selection ) { return button( label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::button( GLImage *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  art.Element(( selection ? selected : hovering ? activated : color ),none,art.stash.rr_Button,(double)x,(double)y,(double)w,(double)h);
//  Stretchi( rounded, hovering ? hover : color,    1.0f, none,     x,y,w,h );
  if ( label ) Stretchi( label, ( hovering ? (clicking ? excited : highlight) : (selection ? crayons.Pick(alabaster) : tint) ), 1.0f, additive, x,y,w,h );
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }
 bool FastGUI::button( FastExtents e ) { return button( e.x, e.y, e.w, e.h ); }
 bool FastGUI::button( int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  bestrect((hovering?(clicking?excited:activated):color),none,(double)x,(double)y,(double)w,(double)h);
  ( hovering ? (clicking ? highlight : excited ) : tint ).gl();
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }
 bool FastGUI::button( const char *label, FastExtents e ) { return button( label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::button( const char *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  bestrect((hovering?(clicking?excited:activated):color),none,(double)x,(double)y,(double)w,(double)h);
  Text(label,( hovering ? (clicking ? highlight : excited ) : tint ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::tbutton( const char *label, FastExtents e, bool selection ) { return button( label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::tbutton( const char *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  bestrect(( selection ? selected : hovering ? activated : color ),none,(double)x,(double)y,(double)w,(double)h);
  Zstring txt(label);
  txt.toUpper();
  Text((txt.c_str()),( hovering ? (clicking ? excited : highlight) : (selection ? crayons.Pick(black) : tint) ),transparency,(double)(x+w/2),(double)(y+h/2),10.0,12.0,0.0,0.0,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::button( const char *label, FastExtents e, bool selection ) { return button( label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::button( const char *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  bestrect(( selection ? selected : hovering ? activated : color ),none,(double)x,(double)y,(double)w,(double)h);
  Text(label,( hovering ? (clicking ? excited : highlight) : (selection ? indicated : tint) ),transparency,
   (double)(x+w/2),(double)y+(double)(h/2),
   fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int gw=(int)(fontSize.x*(double)strlen(label));
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::button( const char *label, FastExtents e, bool selection, int indent ) { return button( label, e.x, e.y, e.w, e.h, selection, indent ); }
 bool FastGUI::button( const char *label, int x, int y, int w, int h, bool selection, int indent ) {
  GetState(x,y,w,h);
  bestrect(hovering?activated:color,(double)x,(double)y,(double)w,(double)h);
  if ( indent==0 ) {
   Text(label,( hovering ? (clicking ? excited : selected.Anti() ) : (selection ? selected.Anti() : tint) ),transparency,(double)(x+w/2),(double)y+(double)(h/2.0-fontSize.y/2.0),fontSize.x,fontSize.y,charSpace,lineSpace,true);
  } else {
   Text(label,( hovering ? (clicking ? excited : selected.Anti() ) : (selection ? selected.Anti() : tint) ),transparency,(double)(x+indent),(double)y+(double)(h/2.0-fontSize.y/2.0),fontSize.x,fontSize.y,charSpace,lineSpace,false);
  }
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 // small button

 bool FastGUI::smbutton( const char *label, FastExtents e ) { return smbutton( label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::smbutton( const char *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  bestrect(hovering?activated:color,(double)x,(double)y,(double)w,(double)h);
  Text(label,( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::smbutton( const char *label, FastExtents e, bool selection ) { return smbutton( label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::smbutton( const char *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  bestrect(( selection ? selected : hovering ? activated : color ),(double)x,(double)y,(double)w,(double)h);
  Text(label,( hovering ? (clicking ? excited : highlight) : (selection ? indicated : tint) ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 
 // button

 bool FastGUI::button(  GLImage *backing, const char *label, FastExtents e, bool selection ) { return button( backing, label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::button( GLImage *backing, const char *label, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  art.Stretch( backing, (selection ? selected : ( hovering ? activated : color )), none, (double)x, (double)y, (double)w, (double)h );
  Text(label,( hovering ? (clicking ? excited : highlight) : (selection ? selected.Anti() : tint) ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::button( TexFont *font, const char *label, FastExtents e, bool selection ) { return button( font, label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::button( TexFont *font, const char *label, int x, int y, int w, int h, bool selection, int charspace ) {
  if ( charspace == -1024 ) charspace=charSpace;
  GetState(x,y,w,h);
  int gw=WidthTexFont(font,tfont.w,charspace,label);
  bestrect(( selection ? selected : hovering ? activated : color ),(double)x,(double)y,(double)w,(double)h);
  Text(font, ( hovering ? (clicking ? excited : highlight) : (selection ? selected.Anti() : tint) ), 
   font->blend, (x+w/2)-gw/2, (y+h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   tfont.w,tfont.h,lineSpace.value,charspace,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }
 bool FastGUI::button( TexFont *font, const char *label, int cx, int cy ) {
  int w=WidthTexFont(font,tfont.w,charSpace,label);
  int h=tfont.h+lineSpace*2;
  int x=cx-w/2;
  int y=cy-h/2;
  GetState(x,y,w,h);
  bestrect(hovering?activated:color,(double)x,(double)y,(double)w,(double)h);
  Text(font, ( hovering ? (clicking ? excited : highlight) : tint ), 
   font->blend, x, (y+h-lineSpace-tfont.h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   tfont.w,tfont.h,lineSpace.value,charSpace,label
  );
  lastSize.SetRect(x,y,w,h);
  return clicked;
 }

 // left justified button

 bool FastGUI::ljustbutton( TexFont *font, const char *label, int x, int y, int w, int h, bool selection, int charspace ) {
  if ( charspace == -1024 ) charspace=fontCharSpace;
  GetState(x,y,w,h);
  int gw=WidthTexFont(font,tfont.w,charspace,label);
  box(( selection ? selected : hovering ? activated : color ),fg_D,(double)x,(double)y,(double)w,(double)h);
  Text(font, ( hovering ? (clicking ? excited : highlight) : (selection ? selected.Anti() : tint) ), 
   font->blend, (x+tfont.w), (y+h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   tfont.w,tfont.h,lineSpace.value,charspace,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 // button

 bool FastGUI::button( TexFont *font, const char *label, Crayon c, FastExtents e, bool selection ) { return button( font, label, e.x, e.y, e.w, e.h, selection ); }
 bool FastGUI::button( TexFont *font, const char *label, Crayon c, int x, int y, int w, int h, bool selection ) {
  GetState(x,y,w,h);
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  box( (selection ? hover : ( hovering ? activated : color )), fg_D, (double)x, (double)y, (double)w, (double)h );
  Text(font, ( hovering ? (clicking ? excited : highlight) : (selection ? indicated : tint) ), 
   tblend, (x+w/2)-gw/2, (y+h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   tfont.w,tfont.h,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 bool FastGUI::button( TexFont *font, const char *label, FastExtents e, int fw, int fh, bool selection ) { return button( font, label, e.x, e.y, e.w, e.h, fw, fh, selection ); }
 bool FastGUI::button( TexFont *font, const char *label, int x, int y, int w, int h, int fw, int fh, bool selection ) {
  GetState(x,y,w,h);
  int gw=WidthTexFont(font,fw,fontCharSpace.value,label);
  box(( selection ? selected : hovering ? activated : color ),  fg_D, (double)x, (double)y, (double)w, (double)h );
  Text(font, ( hovering ? (clicking ? excited : highlight) : (selection ? indicated : tint) ), 
   font->blend, (x+w/2)-gw/2, (y+h/2)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

  // disabled button
 void FastGUI::dbutton( GLImage *label, FastExtents e ) { dbutton( label, e.x, e.y, e.w, e.h ); }
 void FastGUI::dbutton( GLImage *label, int x, int y, int w, int h ) {
  bestrect( deactivated, none, (double)x, (double)y, (double)w, (double)h );
  art.Stretch(label,color, (double)x, (double)y, (double)w, double(h));
  lastSize.SetRect(x,y,w,h);
 }
 
 void FastGUI::dbutton( TexFont *font, const char *label, FastExtents e ) { dbutton( font, label, e.x, e.y, e.w, e.h ); }
 void FastGUI::dbutton( TexFont *font, const char *label, int x, int y, int w, int h ) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  box( deactivated, fg_D, (double)x, (double)y, (double)w, (double)h );
  Text(font, hover, font->blend, (x+w/2)-gw/2, (y+h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   tfont.w,tfont.h,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
 }
  
 bool FastGUI::dbbutton( TexFont *font, const char *label, FastExtents e ) { return dbbutton( font, label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::dbbutton( TexFont *font, const char *label, int x, int y, int w, int h ) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  box( deactivated, fg_D, (double)x, (double)y, (double)w, (double)h );
  Text(font, hover, font->blend, (x+w/2)-gw/2, (y+h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   tfont.w,tfont.h,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  GetState(x,y,w,h);
  return clicked;
 }

 void FastGUI::dbutton( TexFont *font, const char *label, FastExtents e, int fw, int fh ) { dbutton( font, label, e.x, e.y, e.w, e.h, fw, fh ); }
 void FastGUI::dbutton( TexFont *font, const char *label, int x, int y, int w, int h, int fw, int fh ) {
  int gw=WidthTexFont(font,fw,fontCharSpace.value,label);
  box( deactivated, fg_D, (double)x, (double)y, (double)w, (double)h );
  Text(font, crayons.Pick(black), font->blend, (x+w/2)-gw/2, (y+h/2)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
 }

 // button

 bool FastGUI::button( GLImage *backing, TexFont *font, const char *label, FastExtents e, bool selection, Blends b ) { return button( backing, font, label, e.x, e.y, e.w, e.h, selection, b ); }
 bool FastGUI::button( GLImage *backing, TexFont *font, const char *label, int x, int y, int w, int h, bool selection, Blends b ) {
  GetState(x,y,w,h);
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  art.Stretch( backing, (selection ? selected : ( hovering ? activated : color )), b, (double)x, (double)y, (double)w, (double)h );
  Text(font, ( hovering ? (clicking ? excited : highlight) : (selection ? indicated : tint) ), 
   font->blend, (x+w/2)-gw/2, (y+h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   tfont.w,tfont.h,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 bool FastGUI::button( GLImage *backing, TexFont *font, const char *label, FastExtents e, int limitedw, bool selection, Blends b ) { return button( backing, font, label, e.x, e.y, e.w, e.h, limitedw, selection, b ); }
 bool FastGUI::button( GLImage *backing, TexFont *font, const char *label, int x, int y, int w, int h, int limitedw, bool selection, Blends b ) {
  GetState(x,y,w,h);
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  if ( backing ) Stretchi( backing, (selection ? selected : ( hovering ? activated : color )), 1.0f, b, x, y, w, h );
  Text(font, ( hovering ? (clicking ? excited : highlight) : (selection ? indicated : tint) ), 
   font->blend, (x+w/2)-gw/2, (y+h/2)-(int)((float)tfont.h*(font->baselineOffset))/2,
   limitedw,tfont.w,tfont.h,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 bool FastGUI::button( GLImage *icon, GLImage *backing, FastExtents e, Crayon b, Crayon i, Crayon d, bool disabled, Blends blend  ) { return button(icon,backing,e.x,e.y,e.w,e.h,b,i,d,disabled,blend); }
 bool FastGUI::button( GLImage *icon, GLImage *backing, int x, int y, int w, int h, Crayon b, Crayon i, Crayon d, bool disabled, Blends blend  ) {
  lastSize.SetRect(x,y,w,h);
  if ( disabled ) {
   art.Stretch(backing,d,blend,(double)x,(double)y,(double)w,(double)h);
   art.Stretch(icon,i,blend,(double)x,(double)y,(double)w,(double)h);
   return false;
  } else {
   GetState(x,y,w,h);
   art.Stretch(backing,(clicking?excited:b),blend,(double)x,(double)y,(double)w,(double)h);
   art.Stretch(icon,i,blend,(double)x,(double)y,(double)w,(double)h);
   return clicked;
  }
 }

 // backless button

 bool FastGUI::backlessbutton( const char *label, FastExtents e ) { return button( label, e.x, e.y, e.w, e.h ); }
 bool FastGUI::backlessbutton( const char *label, int x, int y, int w, int h ) {
  GetState(x,y,w,h);
  if ( hovering ) box(activated,fg_D,(double)x,(double)y,(double)w,(double)h);
  Text(label,( hovering ? (clicking ? highlight : excited ) : tint ),transparency,(double)(x+w/2),(double)y+(double)h/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 // square button

 bool FastGUI::squarebutton( GLImage *icon, GLImage *backing, FastExtents e, Crayon b, Crayon i, Crayon d, bool disabled, Blends blend  ) { return squaredbutton(icon,backing,e.x,e.y,e.w,e.h,b,i,d,disabled,blend); }
 bool FastGUI::squarebutton( GLImage *icon, GLImage *backing, int x, int y, int w, int h, Crayon b, Crayon i, Crayon d, bool disabled, Blends blend  ) {
  lastSize.SetRect(x,y,w,h);
  if ( disabled ) {
   art.Stretch(backing,d,blend,(double)x,(double)y,(double)w,(double)h);
   art.Stretch(icon,i,blend,(double)x,(double)y,(double)w,(double)h);
   return false;
  } else {
   GetState(x,y,w,h);
   art.Stretch(backing,(clicking?excited:hovering?i:b),blend,(double)x,(double)y,(double)w,(double)h);
   art.Stretch(icon,(hovering?i:activated),blend,(double)x,(double)y,(double)w,(double)h);
   return clicked;
  }
 }

 bool FastGUI::squaredbutton( GLImage *icon, GLImage *backing, FastExtents e, Crayon b, Crayon i, Crayon d, bool disabled, Blends blend  ) { return squaredbutton(icon,backing,e.x,e.y,e.w,e.h,b,i,d,disabled,blend); }
 bool FastGUI::squaredbutton( GLImage *icon, GLImage *backing, int x, int y, int w, int h, Crayon b, Crayon i, Crayon d, bool disabled, Blends blend  ) {
  int squaredh=h/2+h/4;
  lastSize.SetRect(x,y,w,h);
  if ( disabled ) {
   art.Stretch(backing,d,blend,x,y,w,h);
   art.Stretch(icon,i,blend,x+w/4,y+(h-squaredh)/2,w/2,squaredh);
   return false;
  } else {
   GetState(x,y,w,h);
   art.Stretch(backing,(clicking?excited:b),blend,(double)x,(double)y,(double)w,(double)h);
   art.Stretch(icon,i,blend,(double)(x+w/4),(double)y+(double)(h-squaredh)/2.0,(double)w/2.0,(double)squaredh);
   return clicked;
  }
 }

 // button

 bool FastGUI::button( GLImage *backing, TexFont *font, const char *label, FastExtents e, int fw, int fh, bool selection, Blends b  ) { return button(backing,font,label,e.x,e.y,e.w,e.h,fw,fh,selection,b); }
 bool FastGUI::button( GLImage *backing, TexFont *font, const char *label, int x, int y, int w, int h, int fw, int fh, bool selection, Blends b ) {
  GetState(x,y,w,h);
  int gw=WidthTexFont(font,fw,fontCharSpace.value,label);
  art.Stretch( backing, (selection ? hover : ( hovering ? activated : color )), b, (double)x, (double)y, (double)w, (double)h );
  ( hovering ? (clicking ? excited : highlight) : (selection ? color : tint) ).gl();
  Text(font, ( hovering ? (clicking ? excited : highlight) : (selection ? indicated : tint) ), 
   tblend, (x+w/2)-gw/2, (y+h/2)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,lineSpace.value,fontCharSpace.value,label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return clicked;
 }

 // toggle

 bool FastGUI::toggle( const char *label, bool value, FastExtents e, Blends b ) { return toggle(label,value,e.x,e.y,e.w,e.h,b); }
 bool FastGUI::toggle( const char *label, bool value, int  x, int y, int w, int h, Blends blending ) {
  GetState(x,y,w,h);
  if ( value ) box((hovering?activated:color),fg_D,(double)x,(double)y,(double)w,(double)h,blending);
  /*else*/     box((hovering?activated:color),fg_F,(double)x,(double)y,(double)w,(double)h,blending);
  Text(label, ( hovering ? (clicking ? excited : highlight) : tint ), transparency, (double)(x+w)+fontSize.x, (double)(y+lineSpace+fontSize.y/2), fontSize.x,fontSize.y,charSpace,lineSpace,false);
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw+w,h+fontH);
  if ( clicked ) return !value;
  return value;
 }

 bool FastGUI::toggle( const char *labelTop, const char *labelBottom, const char *label, bool value, FastExtents e ) { return toggle(labelTop,labelBottom,label,value,e.x,e.y,e.w,e.h); }
 bool FastGUI::toggle( const char *labelTop, const char *labelBottom, const char *label, bool value, int  x, int y, int w, int h ) {
  GetState(x,y,w,h);
  if ( value )box((hovering?activated:color),fg_L,(double)x,(double)y,(double)w,(double)h);
  else        box((hovering?activated:color),fg_M,(double)x,(double)y,(double)w,(double)h);
  Text(label,      ( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w)+fontSize.x,(double)y+h-((double)h-fontSize.y)/2.0,fontSize.x,fontSize.y,charSpace,lineSpace,false);
  Text(labelTop,   ( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w/2),(double)y-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  Text(labelBottom,( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w/2),(double)(y+h)+smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  lastSize.SetRect( x,y, w/2+fontW, (y+h)+fontH );
  if ( clicked ) return !value;
  return value;
 }

/// numeric  ////////////////////////////////////////////////////////////////////////////////////////////
 
 int FastGUI::numeric( const char *label, int value, int lowest, int highest, FastExtents e, int characters ) { return numeric(label,value,lowest,highest,e.x,e.y,e.w,e.h,characters); }
 int FastGUI::numeric( const char *label, int value, int lowest, int highest, int x, int y, int w, int h, int characters ) {
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) && value != INT_MAX ) {
   value-=1;
   if ( value < lowest ) value = highest;
   return value;
  }
  if ( button( upArrow, x + bw, y, bw, h ) && value != INT_MAX  ) {
   value+=1;
   if ( value > highest ) value = lowest;
   return value;
  }
  string wasnumber= value == INT_MAX ? "" : to_string(value);
  string number=textbox( label, wasnumber, x + w + smallFontW, y, characters );
  if ( number.length() == 0 ) {
   return INT_MAX;
  }
  int typed=value;
  if ( str_cmp(number.c_str(),wasnumber.c_str()) ) typed=atoi(number.c_str());
  if ( typed > highest ) typed=highest;
  if ( typed < lowest ) typed=lowest;
  lastSize.w+=w+smallFontW.value;
  lastSize.x-=w+smallFontW.value;
  return typed;
 }

 int FastGUI::integer( const char *label, int value, int lowest, int highest, int delta, FastExtents e, int characters, bool deltaOk ) { return integer(label,value,lowest,highest,delta,e.x,e.y,e.w,e.h,characters,deltaOk); }
 int FastGUI::integer( const char *label, int value, int lowest, int highest, int delta, int x, int y, int w, int h, int characters, bool deltaOk ) {
  int deltaD=deltaOk?integer_delta(delta):delta;
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) && value != INT_MAX ) {
   value-=deltaD;
   if ( value < lowest ) value = highest;
   return value;
  }
  if ( hovering ) display_delta( -deltaD );
  if ( button( upArrow, x + bw, y, bw, h ) && value != INT_MAX ) {
   value+=deltaD;
   if ( value > highest ) value = lowest;
   return value;
  }
  if ( hovering ) display_delta( deltaD );
  string wasnumber= value == INT_MAX ? "" : to_string(value);
  string number=textbox( label, wasnumber, x + w + smallFontW, y, characters );
  if ( number.length() == 0 ) {
   return INT_MAX;
  }
  int typed=value;
  if ( str_cmp(number.c_str(),wasnumber.c_str()) ) typed=atoi(number.c_str());
  if ( typed > highest ) typed=highest;
  if ( typed < lowest ) typed=lowest;
  lastSize.w+=w+smallFontW.value;
  lastSize.x-=w+smallFontW.value;
  return typed;
 }

 int FastGUI::integerNL( const char *label, int value, int delta, FastExtents e, int characters, bool deltaOk ) { return integerNL(label,value,delta,e.x,e.y,e.w,e.h,characters,deltaOk); }
 int FastGUI::integerNL( const char *label, int value, int delta, int x, int y, int w, int h, int characters, bool deltaOk ) {
  int deltaD=deltaOk?integer_delta(delta):delta;
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) && value != INT_MAX ) {
   value-=deltaD;
   return value;
  }
  if ( hovering ) display_delta( -deltaD );
  if ( button( upArrow, x + bw, y, bw, h ) && value != INT_MAX ) {
   value+=deltaD;
   return value;
  }
  if ( hovering ) display_delta( deltaD );
  string wasnumber= value == INT_MAX ? "" : to_string(value);
  string number=textbox( label, wasnumber, x + w + smallFontW, y, characters );
  if ( number.length() == 0 ) {
   return INT_MAX;
  }
  int typed=value;
  if ( str_cmp(number.c_str(),wasnumber.c_str()) ) typed=atoi(number.c_str());
  lastSize.w+=w+smallFontW.value;
  lastSize.x-=w+smallFontW.value;
  return typed;
 }

 int FastGUI::integerL( const char *label, int value, int lowest, int delta, FastExtents e, int characters, bool deltaOk ) { return integerL(label,value,delta,e.x,e.y,e.w,e.h,characters,deltaOk); }
 int FastGUI::integerL( const char *label, int value, int lowest, int delta, int x, int y, int w, int h, int characters, bool deltaOk ) {
  int deltaD=deltaOk?integer_delta(delta):delta;
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) && value != INT_MAX ) {
   value-=deltaD;
   if ( value < lowest ) value = lowest;
   return value;
  }
  if ( hovering ) display_delta( -deltaD );
  if ( button( upArrow, x + bw, y, bw, h ) && value != INT_MAX ) {
   value+=deltaD;
   return value;
  }
  if ( hovering ) display_delta( deltaD );
  string wasnumber= value == INT_MAX ? "" : to_string(value);
  string number=textbox( label, wasnumber, x + w + smallFontW, y, characters );
  if ( number.length() == 0 ) {
   return INT_MAX;
  }
  int typed=value;
  if ( str_cmp(number.c_str(),wasnumber.c_str()) ) typed=atoi(number.c_str());
  lastSize.w+=w+smallFontW.value;
  lastSize.x-=w+smallFontW.value;
  return typed;
 }

 double FastGUI::decimal( const char *label, double value, double lowest, double highest, double delta, FastExtents e, int characters, bool deltaOk ) { return decimal(label,value,lowest,highest,delta,e.x,e.y,e.w,e.h,characters,deltaOk); }
 double FastGUI::decimal( const char *label, double value, double lowest, double highest, double delta, int x, int y, int w, int h, int characters, bool deltaOk ) {
  if ( deltaOk ) delta=number_delta(delta);
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) && value != 123456789.87654321 ) {
   value-=delta;
   if ( value < lowest ) value = highest;
   return value;
  }
  if ( hovering ) display_delta( -delta );
  if ( button( upArrow, x + bw, y, bw, h ) && value != 123456789.87654321 ) {
   value+=delta;
   if ( value > highest ) value = lowest;
   return value;
  }
  if ( hovering ) display_delta( delta );
  string wasnumber= value == 123456789.87654321 ? "" : to_string(value);
  string number=textbox( label, wasnumber, x + w + smallFontW, y, characters );
  if ( number.length() == 0 ) {
   return 123456789.87654321;
  }
  double typed=value;
  if ( str_cmp(number.c_str(),wasnumber.c_str()) ) typed=atoi(number.c_str());
  if ( typed > highest ) typed=highest;
  if ( typed < lowest ) typed=lowest;
  lastSize.w+=w+smallFontW.value;
  lastSize.x-=w+smallFontW.value;
  return typed;
 }

 double FastGUI::precise( const char *label, double value, double lowest, double highest, double delta, FastExtents e, int characters, bool deltaOk ) { return precise(label,value,lowest,highest,delta,e.x,e.y,e.w,e.h,characters,deltaOk); }
 double FastGUI::precise( const char *label, double value, double lowest, double highest, double delta, int x, int y, int w, int h, int characters, bool deltaOk ) {
  int bw=w/2;
  if ( deltaOk ) delta=number_delta(delta);
  if ( button( downArrow, x, y, bw, h ) && value != 123456789.87654321 ) {
   value-=delta;
   if ( value < lowest ) value = highest;
   return value;
  }
  if ( hovering ) display_delta( -delta );
  if ( button( upArrow, x + bw, y, bw, h ) && value != 123456789.87654321 ) {
   value+=delta;
   if ( value > highest ) value = lowest;
   return value;
  }
  if ( button( "-", x + bw*2-3, y+h, 6,6 ) ) {
   return 0.0;
  }
  if ( hovering ) display_delta( delta );
  string wasnumber= value == 123456789.87654321 ? "" : to_string(value);
  string number=textbox( label, wasnumber, x + w + smallFontW, y, characters );
  if ( number.length() == 0 ) {
   return 123456789.87654321;
  }
  double typed=value;
  if ( str_cmp(number.c_str(),wasnumber.c_str()) ) typed=atoi(number.c_str());
  if ( typed > highest ) typed=highest;
  if ( typed < lowest ) typed=lowest;
  lastSize.w+=w+smallFontW.value;
  lastSize.x-=w+smallFontW.value;
  return typed;
 }
 
/// enumerated  ////////////////////////////////////////////////////////////////////////////////////////////

 int FastGUI::enumerated( Strings *values, const char *label, int value, FastExtents e, int characters ) { return enumerated(values,label,value,e.x,e.y,e.w,e.h,characters); }
 int FastGUI::enumerated( Strings *values, const char *label, int value, int x, int y, int w, int h, int characters ) {
  int lowest=0;
  int highest=values->count-1;
  String *e=(String *) values->Element(value);
  string content=e ? e->s : string("");
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) ) {
   value-=1;
   if ( value < lowest ) value = highest;
  }
  bool wasHovering = hovering;
  if ( button( upArrow, x + bw, y, bw, h ) ) {
   value+=1;
   if ( value > highest ) value = lowest;
  }
  Text(label, ( wasHovering || hovering ? highlight : tint ), transparency, (double)(x+w/2),(double)y-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  ( hovering ? (clicking ? excited : highlight) : tint ).gl();
  Text( content.c_str(), ( wasHovering || hovering ? highlight : tint ), transparency, x + w + smallFontW, y, fontSize.x, fontSize.y,charSpace,lineSpace,false );
  lastSize.SetRectf( (float)x, (float)(y-smallFontH), (float)(smallFontW*characters), (float)(smallFontH+h) );
  return value;
 }

 int FastGUI::enumerated( Strings *values, const char *label, int value, FastExtents e, having values_integer, int characters ) { return enumerated(values,label,value,e.x,e.y,e.w,e.h,characters,values_integer); }
 int FastGUI::enumerated( Strings *values, const char *label, int value, int x, int y, int w, int h, having values_integer, int characters ) {
  String *e=null;
  int returning=value;
  EACH(values->first,String,se) if ( se->integer == value ) e=se;
  string content=e ? e->s : string("");
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) ) {
   if ( e->prev ) e=(String *)e->prev;
   else e=(String *)values->last;
   returning= ( e?e->integer:value );
  }
  bool wasHovering = hovering;
  if ( button( upArrow, x + bw, y, bw, h ) ) {
   if ( e->next ) e=(String *)e->next;
   else e=(String *)values->first;
   returning= ( e?e->integer:value );
  }
  Text(label, ( wasHovering || hovering ? highlight : tint ), transparency, (double)(x+w/2),(double)y-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  Text( content.c_str(), ( hovering ? (clicking ? excited : highlight) : tint ), transparency, x + w + smallFontW, y, fontSize.x, fontSize.y, charSpace, lineSpace, false );
  lastSize.SetRectf( (float)x, (float)(y-smallFontH), (float)(smallFontW*characters), (float)(smallFontH+h) );
  return returning;
 }

 int FastGUI::enumerated( Strings *values, const char *label, int value, int lowest, int highest, FastExtents e, int characters ) { return enumerated(values,label,value,lowest,highest,e.x,e.y,e.w,e.h,characters); }
 int FastGUI::enumerated( Strings *values, const char *label, int value, int lowest, int highest, int x, int y, int w, int h, int characters ) {
  String *e=(String *) values->Element(value);
  string content=e ? e->s : string("");
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) ) {
   value-=1;
   if ( value < lowest ) value = highest;
  }
  bool wasHovering = hovering;
  if ( button( upArrow, x + bw, y, bw, h ) ) {
   value+=1;
   if ( value > highest ) value = lowest;
  }
  Text(label, ( wasHovering || hovering ? highlight : tint ), transparency, (double)(x+w/2),(double)y-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  Text( content.c_str(), ( hovering ? (clicking ? excited : highlight) : tint ), transparency, x + w + smallFontW, y, fontSize.x, fontSize.y,charSpace,lineSpace,false );
  lastSize.SetRectf( (float)x, (float)(y-smallFontH), (float)(smallFontW*characters), (float)(smallFontH+h) );
  return value;
 }

 String *FastGUI::enumerated( Strings *values, const char *label, String *value, FastExtents e, int characters ) { return enumerated(values,label,value,e.x,e.y,e.w,e.h,characters); }
 String *FastGUI::enumerated( Strings *values, const char *label, String *value, int x, int y, int w, int h, int characters ) {
  if ( !value ) return value;
  string content=value ? value->s : string("");
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) ) {
   if ( value->prev == null ) value = (String *) values->last;
   else value=(String *) value->prev;
  }
  bool wasHovering = hovering;
  if ( button( upArrow, x + bw, y, bw, h ) ) {
   if ( value->next == null ) value = (String *) values->first;
   else value=(String *) value->next;
  }
  Text(label, ( wasHovering || hovering ? highlight : tint ), transparency, (double)(x+w/2),(double)y-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  Text( content.c_str(), ( hovering ? (clicking ? excited : highlight) : tint ), transparency, x + w + smallFontW, y, fontSize.x, fontSize.y,charSpace,lineSpace,false );
  lastSize.SetRectf( (float)x, (float)(y-smallFontH), (float)(smallFontW*characters), (float)(smallFontH+h) );
  return value;
 }

 
/// blend selector  ////////////////////////////////////////////////////////////////////////////////////////////

 Blends FastGUI::blend( const char *label, Blends value, int x, int y, int w, int h, int characters ) {
  int bw=w/2;
  if ( button( downArrow, x, y, bw, h ) ) {
   value.previous();
  }
  bool wasHovering = hovering;
  if ( button( upArrow, x + bw, y, bw, h ) ) {
   value.next();
  }
  Text(label, ( wasHovering || hovering ? highlight : tint ), transparency, (double)(x+w/2),(double)y-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  Text( value.toString().c_str(), ( hovering ? (clicking ? excited : highlight) : tint ), transparency, x + w + smallFontW, y, fontSize.x, fontSize.y, charSpace, lineSpace, false );
  lastSize.SetRectf( (float)x, (float)(y-smallFontH), (float)(smallFontW*characters), (float)(smallFontH+h) );
  return value;
 }
 
/// horizontal range select  ////////////////////////////////////////////////////////////////////////////////////////////
 
 void FastGUI::hrange( const char *label, double *value1, double *value2, double lowest, double highest, int x, int y, int w, int h, bool simple ) {
  Text(label,( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x),(double)y-(double)h/4-(double)h/8-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,false);
  if ((*value1)<lowest) (*value1) = lowest; else if ((*value1)>highest) (*value1) = highest;
  if ((*value2)<lowest) (*value2) = lowest; else if ((*value2)>highest) (*value2) = highest;
  if ( (*value2) < (*value1) ) { double temp=(*value1); (*value1)=(*value2); (*value2)=temp; }
  GetState(x,y,w,h);
  if ( hovering ) activated.gl(); else color.gl();
  box(fg_C,x,y,w,h/2);
  int tenth = (int)((double)w*0.1);
  int leftX = x+tenth;
  if (clicking) {
   int detectionWidth = w;
   double midPoint=((((*value2)-(*value1))/2.0+(*value1)-lowest)/(highest-lowest));
   int detectedPoint=(int)(point.x*(double)w);
   double detectedPerc = iratiod(detectedPoint,detectionWidth);
   if ( detectedPerc <= midPoint ) (*value1)=detectedPerc*(highest-lowest)+lowest;
   else (*value2)=detectedPerc*(highest-lowest)+lowest;
  }
  double perc1 = ((*value1)-lowest)/(highest-lowest);
  double perc2 = ((*value2)-lowest)/(highest-lowest);
  int lowX = x+(int)(perc1*(double)(w));
  int highX = x+(int)(perc2*(double)(w));
  int midX=x+((highX-x)+(lowX-x))/2;
  int rwidth= highX-lowX;
  float old_rect_borderwidth=this->rect_borderwidth;
  this->rect_borderwidth=1.0;
  rrect(
   ( hovering ? (clicking ? excited : highlight) : color ), (clicking?excited:color), (clicking?excited:color),
   lowX,y+h/4,rwidth,h/4-2
  );
  glLineWidth(3.0);
  DrawLine(crayons.Pick(black),midX,y,midX,y+h/2);
  glLineWidth(1.0);
  this->rect_borderwidth=old_rect_borderwidth;
//  box(fg_E,newX,y-h,tenth,h*2+h/2);
  string number1=FORMAT("%.2f",(*value1));
  string number2=FORMAT("%.2f",(*value2));
  string lowValue=FORMAT("%.2f",lowest);
  string hiValue=FORMAT("%.2f",highest);
  if (!simple) {
   Text(number1.c_str(),crayons.Pick(red255),transparency,(double)lowX,(double)y+h/4-smallFontSize.y*1.5,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(number2.c_str(),crayons.Pick(green255),transparency,(double)highX,(double)y+(h-h/4)+2+smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(lowValue.c_str(),( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x-strlen(lowValue.c_str())*smallFontSize.x),(double)y+smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(hiValue.c_str(),( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w+strlen(hiValue.c_str())*smallFontSize.x),(double)y+smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  } else {
   Text(number1.c_str(),crayons.Pick(red255),transparency,(double)lowX,(double)y+h/4-smallFontSize.y*1.5,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(number2.c_str(),crayons.Pick(green255),transparency,(double)highX,(double)y+(h-h/4)+2+smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  }
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
 }

 
 /// horizontal slider  ////////////////////////////////////////////////////////////////////////////////////////////
 
 double FastGUI::hslider( const char *label, double value, double lowest, double highest, int x, int y, int w, int h, bool simple) {
  if (value<lowest) value = lowest; else if (value>highest) value = highest;
  GetState(x,y,w,h);
  box(hovering?activated:color,fg_C,x,y+h/4,w,h/2);
  double toReturn = value;
  double perc = (value-lowest)/(highest-lowest);
  int tenth = (int)((double)w*0.1);
  int leftX = x+tenth;
  if (clicking) {
   if ( point.x < 0.025 ) perc=0.0;
   if ( point.x > 1.0-0.025 ) perc=1.0;
   perc=point.x;
   toReturn = lowest + (highest - lowest) * perc;
  }
  DrawLine(red255,(int)(perc*(double)w+x),y+h/4,(int)(perc*(double)w+x),y+h/2+h/4);
  int highX = (int)((x+w)-(tenth*2));
  int newX = (int)( (double) leftX + (double) (highX-leftX) * perc);
  float old_rect_borderwidth=this->rect_borderwidth;
  int indicatorWidthdiv2=UMIN(tenth,32);
  box(( hovering ? (clicking ? excited : highlight) : tint ),fg_E,newX-indicatorWidthdiv2/2,y-(int)(smallFontSize.y/2),indicatorWidthdiv2,h);
  Text(label,( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w/2),(double)y-(double)h/2-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  if (!simple) {
   string number=FORMAT("%.2f",toReturn);
   string lowValue=FORMAT("%.2f",lowest);
   string hiValue=FORMAT("%.2f",highest);
   Text(number.c_str(),crayons.Pick(black),transparency,(double)(newX),(double)y+h/2-smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(lowValue.c_str(),( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x-strlen(lowValue.c_str())*smallFontSize.x),(double)y+smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(hiValue.c_str(),( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w+strlen(hiValue.c_str())*smallFontSize.x),(double)y+smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  } else {
   string number=
    tenth < 16 ? FORMAT("%d",(int)toReturn)
               : FORMAT("%.2f",toReturn);
   Text(number.c_str(),crayons.Pick(black),transparency,(double)(newX),(double)y+h/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  }
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return toReturn;
 }

 int FastGUI::hslider( const char *label, int value, int lowest, int highest, int x, int y, int w, int h, bool simple) {
  if (value<lowest) value = lowest; else if (value>highest) value = highest;
  GetState(x,y,w,h);
  box(hovering?activated:color,fg_C,x,y,w,h/2);
  int toReturn = value;
  double perc = iratiod(value-lowest,highest-lowest);
  int tenth = (int)((double)w*0.1);
  int leftX = x+tenth;
  if (clicking) {
   int detectionWidth = (int)((double)w*0.8);
   perc = CLAMPTO(iratiod((int)(point.x*(double)w),detectionWidth),0.0,1.0);
   toReturn = (int)((double)lowest + (double)(highest - lowest) * perc);
  }
  int highX = (int)((x+w)-(tenth*2));
  int newX = (int)( (double) leftX + (double) (highX-leftX) * perc);
  float old_rect_borderwidth=this->rect_borderwidth;
  box(( hovering ? (clicking ? excited : highlight) : tint ),fg_E,newX-tenth/2,y-(int)(smallFontSize.y/2),tenth*2,h);
  Text(label,( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w/2),(double)y-(double)h/2-smallFontSize.y,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  string number=FORMAT("%d",toReturn);
  string lowValue=FORMAT("%d",lowest);
  string hiValue=FORMAT("%d",highest);
  if (!simple) {
   Text(number.c_str(),crayons.Pick(black),transparency,(double)(newX+tenth/2),(double)y+h/2-smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(lowValue.c_str(),( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x-strlen(lowValue.c_str())*smallFontSize.x),(double)y+smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
   Text(hiValue.c_str(),( hovering ? (clicking ? excited : highlight) : tint ),transparency,(double)(x+w+strlen(hiValue.c_str())*smallFontSize.x),(double)y+smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  } else {
   Text(number.c_str(),crayons.Pick(black),transparency,(double)(newX+tenth/2),(double)y+h/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  }
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return toReturn;
 }
 
/// vertical slider  ////////////////////////////////////////////////////////////////////////////////////////////

 double FastGUI::vslider( const char *label, double value, double lowest, double highest, int x, int y, int w, int h, bool simple, bool superSimple ) {
  if (value<lowest) value = lowest; else if (value>highest) value = highest;
  GetState(x,y,w,h);
  box(hovering?activated:color,fg_C,x+w/4,y,w/2,h);
  double toReturn = value;
  double perc = (value-lowest)/(highest-lowest);
  int tenth = (int)((double)h*0.1);
  int startY = y;
  if (clicking) {
   int detectionWidth = (int)((double)(h-tenth));
   perc = CLAMPTO(1.0-iratiod((int)(point.y*(double)h),detectionWidth),0.0,1.0);
   toReturn = lowest + (highest - lowest) * perc;
  }
  int highY = (int)((y+h-tenth));
  int newY = (int)(startY + (highY-startY) * (1.0-perc));
  box(( hovering ? (clicking ? excited : highlight) : tint ),fg_E,x,newY,w,tenth);
  if ( hovering ) activated.gl(); else 
  Text(label,color,transparency,(double)(x+w/2),(double)y-fontSize.y*2,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  string number=FORMAT("%.2f",toReturn);
  string lowValue=FORMAT("%.2f",lowest);
  string hiValue=FORMAT("%.2f",highest);
  if (!simple) {
   Text(number.c_str(),color,transparency,(double)(x+strlen(number.c_str())*fontSize.x),newY+tenth/2,fontSize.x,fontSize.y,charSpace,lineSpace,true);
   Text(lowValue.c_str(),color,transparency,(double)x,(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
   Text(hiValue.c_str(),color,transparency,(double)x,(double)y+(double)h+fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  } else {
   if (!superSimple) Text(number.c_str(),color,transparency,(double)x,(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  }
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return toReturn;
 }

 double FastGUI::vslider( having upIsDown, const char *label, double value, double lowest, double highest, int x, int y, int w, int h, bool simple, bool superSimple ) {
  if (value<lowest) value = lowest; else if (value>highest) value = highest;
  GetState(x,y,w,h);
  box(hovering?activated:color,fg_C,x+w/4,y,w/2,h);
  double toReturn = value;
  double perc = (value-lowest)/(highest-lowest);
  int tenth = (int)((double)h*0.1);
  int startY = y;
  if (clicking) {
   int detectionWidth = (int)((double)(h-tenth));
   perc = CLAMPTO(iratiod((int)(point.y*(double)h),detectionWidth),0.0,1.0);
   toReturn = lowest + (highest - lowest) * perc;
  }
  int highY = (int)((y+h-tenth));
  int newY = (int)(startY + (highY-startY) * (perc));
  box(( hovering ? (clicking ? excited : highlight) : tint ),fg_E,x,newY,w,tenth);
  if ( hovering ) activated.gl(); else 
  color.gl();
  Text(label,color,transparency,(double)(x+w/2),(double)y-fontSize.y*2,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  string number=FORMAT("%.2f",toReturn);
  string lowValue=FORMAT("%.2f",lowest);
  string hiValue=FORMAT("%.2f",highest);
  if (!simple) {
   Text(number.c_str(),color,transparency,(double)(x+strlen(number.c_str())*fontSize.x),newY+tenth/2,fontSize.x,fontSize.y,charSpace,lineSpace,true);
   Text(lowValue.c_str(),color,transparency,(double)x,(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
   Text(hiValue.c_str(),color,transparency,(double)x,(double)y+(double)h+fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  } else {
   if (!superSimple) Text(number.c_str(),color,transparency,(double)x,(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  }
  int gw=fontW.value*strlen(label);
  lastSize.SetRect(x,y,gw<w?w:gw,h);
  return toReturn;
 }

 
/// menu  ////////////////////////////////////////////////////////////////////////////////////////////

 int FastGUI::menu(TexFont *font,Strings *options,int x, int y, int cw, int oh, int itemSpacing, const char *sound, float pitch, float gain ) {
  int result=-1;
  int i=0;
  int yo=(oh-tfont.h)/2;
  EACH(options->first,String,o) {
   GetState(x,y+i*oh+i*itemSpacing,cw,oh);
   lastSize.SetRect(x,y+i*oh+i*itemSpacing,cw,oh);
   int gw=WidthTexFont(font,tfont.w,fontCharSpace,o->c_str());
   art.Element( whitePixel, ( hovering ? activated : color ), none, art.stash.rr_Button, x*reciprocalRatio.x,lastSize.y*reciprocalRatio.y, cw*reciprocalRatio.x,oh*reciprocalRatio.y );
   Text(font, ( hovering ? (clicking ? excited : highlight) : tint ), 
    font->blend, (x+cw/2)-gw/2, lastSize.y+yo,
    tfont.w,tfont.h,lineSpace.value,fontCharSpace,o->c_str());
   if ( clicked ) {
    audio.manager.CueAndPlay(sound,pitch,gain,false,true);
    result=i;
   }
   i++;
  }
  return result;
 }

 
/// tandem  ////////////////////////////////////////////////////////////////////////////////////////////

 void FastGUI::tandem(const char *label1, const char *label2, int x, int y, int cw, int lines, Strings* left, Integers * leftSelected, int * leftScroll, Strings * right, Integers * rightSelected, int * rightScroll, bool uniqueOnly, bool useButton ) {
  Cartesian extents, leftCol, rightCol;
  leftCol.SetRect(x,y,cw, (int)(lines*(fontSize.y+7)) );
  rightCol.SetRect(x+cw+60, y, cw, (int)(lines*(fontSize.y+7)) );
  if (strlen(label2)>0)
   Text(label1,color,transparency,(double)(x+cw/2),(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  else
   Text(label1,color,transparency,(double)(x+(rightCol.x+cw-x)/2),(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  Text(label2,color,transparency,(double)(rightCol.x+cw/2),(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  box(fg_C,leftCol.x,leftCol.y,leftCol.w,leftCol.h);
  box(fg_C,rightCol.x,rightCol.y,rightCol.w,rightCol.h);
  int rowHeight = (int)(fontSize.y+5);
  int i=0;
  EACH(left->first,String,s) {
   if (i-*leftScroll >=0 && i-*leftScroll < lines) {
    bool selected = leftSelected->find(left->Element(s))!=null;
    if (smbutton(s->s.c_str(),leftCol.x,(int)(leftCol.y+rowHeight*(i-*leftScroll))+5,leftCol.w,(int)(fontSize.y),selected)) {
     if (!selected) leftSelected->Add(left->Element(s));
     else if (selected) leftSelected->Remove((leftSelected->Element(left->Element(s))));
    }
   }
   i++;
  }
  if (smbutton(">>",leftCol.x+cw+25,leftCol.y+25,20,15)) {
   int existing = right->count;
   right->Concat(left);
   leftSelected->Clear();
   rightSelected->Clear();
   for (i=existing; i<right->count; i++) rightSelected->Append(new Integer(i));
  }
  int rightCount = right->count;
  int count=0;
  if (button(rightArrow,leftCol.x+cw+25,leftCol.y,20,20)) {
   if(leftSelected->count>0) {
    rightSelected->Clear();
    EACHN(leftSelected->first,Integer,selection,{
     String * selected=(String*)(left->Element(selection->i-count));
     if(selected) {
      left->Remove(selected);
      right->Append(selected);
      count+=1;
     }
     leftSelected->Remove(selection);
     rightSelected->Append(new Integer(rightCount));
     rightCount+=1;
    });
   }
  }
  i=0;
  EACH(right->first,String,s) {
   if (i-*rightScroll >=0 && i-*rightScroll < lines) {
    bool selectedstr = rightSelected->find(right->Element(s))!=null;
    if (smbutton(s->s.c_str(),rightCol.x,(int)(rightCol.y+rowHeight*(i-*rightScroll)+5),rightCol.w,(int)(fontSize.y),selectedstr)) {
     if(!selectedstr) rightSelected->Add(right->Element(s));
     else if (selectedstr) rightSelected->Remove(rightSelected->Element(right->Element(s)));
    }
   }
   ++i;
  }  
  if (smbutton("<<",leftCol.x+cw+25,rightCol.y+80,20,15)) {
   int existing = left->count;
   left->Concat(right);
   leftSelected->Clear();
   rightSelected->Clear();
   for ( i = existing; i< left->count; i++) leftSelected->Append(new Integer(i));
  }
  if (smbutton("X",leftCol.x+cw+27,rightCol.y+105,15,15)) {
   leftSelected->Clear();
   rightSelected->Clear();
  }
  int leftCount = left->count;
  count=0;
  if (button(leftArrow,leftCol.x+cw+25,rightCol.y+55,20,20)) {
   if(rightSelected->count>0) {
    leftSelected->Clear();
    EACHN(rightSelected->first,Integer,selection,{
     String *selected=(String*)(right->Element(selection->i-count));
     if(selected) {
      right->Remove(selected);
      left->Append(selected);
      count+=1;
     }
     rightSelected->Remove(selection);
     leftSelected->Append(new Integer(leftCount));
     leftCount+=1;
    });
   }
  }
  if(left->count<lines) *leftScroll=0;
  if(right->count<lines) *rightScroll=0;  
  if (left->count - lines > 0) {
   if (button(upArrow,leftCol.x+leftCol.w+3,leftCol.y,15,15)) *leftScroll-=1;
   if (button(downArrow,leftCol.x+leftCol.w+3,leftCol.y+leftCol.h-15,15,15)) *leftScroll+=1;
   (*leftScroll)=(int)vslider("",(double)(*leftScroll),0.0,left->count-lines,leftCol.x+leftCol.w+10,leftCol.y+20,5,leftCol.h-40,true,true);
  }
  if(right->count-lines > 0) {
   if (button(upArrow,rightCol.x+rightCol.w+3,rightCol.y,15,15)) *rightScroll-=1;
   if (button(downArrow,rightCol.x+rightCol.w+3,rightCol.y+rightCol.h-15,15,15)) *rightScroll+=1;
   (*rightScroll)=(int)vslider("",(double)(*rightScroll),0.0,right->count-lines,rightCol.x+leftCol.w+10,leftCol.y+20,5,leftCol.h-40,true,true);
  }
 }
 
/// list selection  ////////////////////////////////////////////////////////////////////////////////////////////

 int FastGUI::list(LinkedList * values, int x, int y, int cw, int lines, int *scroll) {
  color.gl();
  int h=(int)(lines*(fontSize.y+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(fontSize.y+5);
  EACH(values->first,ListItem,s) {
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (smbutton(I2S(i+1),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(fontSize.y))) )
     result = values->Element(s);
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 int FastGUI::list(LinkedList * values, int x, int y, int cw, int lines, int *selected, int *scroll ) {
  color.gl();
  int h=(int)(lines*(fontSize.y+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(fontSize.y+5);
  EACH(values->first,ListItem,s) {
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (smbutton(I2S(i+1),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(fontSize.y),selectedStr)) )
     result = values->Element(s);
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 int FastGUI::list(const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll) {
  Text(label,color,transparency,(double)(x+cw/2),(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int h=(int)(lines*(fontSize.y+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(fontSize.y+5);
  EACH(values->first,String,s) {
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (smbutton(s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(fontSize.y))) )
     result = values->Element(s);
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 int FastGUI::list(const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll) {
  Text(label,color,transparency,(double)(x+cw/2),(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int h=(int)(lines*(fontSize.y+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(fontSize.y+5);
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (smbutton(s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(fontSize.y),selectedStr)) )
     result = values->Element(s);
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 int FastGUI::list(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, int linespace, int charspace, bool showRectangle, int limitString ) {
  int gw=WidthTexFont(font,tfont.w,charspace,label);
  int h=(int)(lines*(tfont.h+15));
  Text(font,color,tblend,(x+cw/2)-gw/2,y+(h-tfont.h)/2,tfont.w,tfont.h,linespace,charspace,label);
  //box(fg_C,x,y,cw,h);
  Blending(none);
  if ( showRectangle ) Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+5);
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(font,(limitString>0?s->s.substr(0,limitString):s->s).c_str(),
                x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h),selectedStr,charspace)) ) {
    result = values->Element(s);
    *selected=result;
   }
   i++;
  }
  if(values->count - lines > 0) {
   h=(int)(lines*rowHeight);
   if (button(upArrow,x+cw+3,y,32,32)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-32,32,32)) *scroll+=1;
   (*scroll)=(int)vslider(true,"",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+40,20,h-80,true,true);
  }
  GetState(x,y,cw,lines*rowHeight);
  if ( hovering ) {
   if ( input.wheelUp ) (*scroll)=UMAX(0,(*scroll)-1);
   if ( input.wheelDown ) (*scroll)=UMIN(values->count-(lines),(*scroll)+1);
  }
  return result;
 }

 
/// reorderable list  ////////////////////////////////////////////////////////////////////////////////////////////

 void FastGUI::reorder( LinkedList * values, int x, int y, int cw, int lines, Zp<ListItem> *selected, int *scroll ) {
  Blending(none);
  int i=0;
  int rowHeight = (int)(tfont.h+5);
  EACHN(values->first,ListItem,s,{
   int rh=(int)(y+rowHeight*(i-*scroll)+5);
   if ( !(i-*scroll >=0 && i-*scroll < lines) ) continue;
   if ( (button(font,I2S(i+1),x,(int)rh,cw-32,(int)(tfont.h),(selected->pointer==s),charSpace)) ) {
    selected->pointer=s;
   } else if ( button(upArrow, x+cw-32,rh,16,16) ) {
    values->sendUp(selected->pointer);
   } else if ( button(downArrow, x+cw-16,rh,16,16) ) {
    values->sendDown(selected->pointer);
   }
   i++;
  });
  if(values->count - lines > 0) {
   int h=(int)(lines*rowHeight);
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider(true,"",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
 }

 int FastGUI::reorder(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, int linespace, int charspace, bool showRectangle, int limitString ) {
  int gw=WidthTexFont(font,tfont.w,charspace,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  //box(fg_C,x,y,cw,h);
  Blending(none);
  int h=(int)(lines*(tfont.h+15));
  if ( showRectangle ) Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+5);
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(font,(limitString>0?s->s.substr(0,limitString):s->s).c_str(),
                x,(int)(y+rowHeight*(i-*scroll)+5),cw-32,(int)(tfont.h),selectedStr,charspace)) ) {
    result = values->Element(s);
    *selected=result;
   } else if ( button(upArrow, x+cw-32,(int)(y+rowHeight*(i-*scroll)+5),16,16) ) {
    return -2;
   } else if ( button(downArrow, x+cw-16,(int)(y+rowHeight*(i-*scroll)+5),16,16) ) {
    return -3;
   }
   i++;
  }
  if(values->count - lines > 0) {
   h=(int)(lines*rowHeight);
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider(true,"",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }
 
/// lists  ////////////////////////////////////////////////////////////////////////////////////////////

 // selectable list of strings that contain valid double values
 void FastGUI::decimalstringlist(TexFont *font, const char * label, Strings * values, int x, int y, int w, int lines, int *scroll, double lowest, double highest, double delta, int linespace, int charspace, bool canAdd, int limitString, const char *placeholder ) {
  int gw=WidthTexFont(font,tfont.w,charspace,label);
  Text(font,color,tblend,(x+w/2)-gw/2,y-tfont.h-24,tfont.w,tfont.h,linespace,charspace,label);
  Blending(none);
  int i=0;
  int rowHeight = 48;
  int h=(int)(lines*rowHeight);
  EACH(values->first,String,s) {
   if ( (i-*scroll >=0 && i-*scroll < lines) ) {
    s->s = textbox(font,transparency,font,additive,"",s->s,x+128,y+rowHeight*(i-*scroll),UMIN(limitString,((w-128)/2)/10),true,1,2,4,false,-4);
    s->floating=(float)decimal("",(double)s->floating,lowest,highest,delta,x,y+rowHeight*(i-*scroll)+8,64,32);
   }
   i++;
  }
  if((values->count - lines) > 0) {
   if (button(upArrow,x+w+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+w+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider(true,"",(double)(*scroll),0.0,values->count-lines,x+w+10,y+20,5,h-40,true,true);
  }
  if ( canAdd ) {
   if ( button(font,"+",x+w-32,y-32,16,16) ) {
    values->Add(placeholder,(float)lowest);
   }
  }
 }

 int FastGUI::list(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, bool tailEnd, int charLimit, int linespace, int charspace) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  int h=(int)(lines*(tfont.h+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+5);
  if ( charLimit == -1 ) charLimit=cw/(int)(tfont.w+2);
  Zstring lbl;
  EACH(values->first,String,s) {
   lbl=s->s;
   int start=(int)s->s.length()-(charLimit-3);
   if ( start < 0 ) start = 0;
   lbl=string("...")+lbl(start,charLimit-3);
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(font,lbl.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h),selectedStr)) ) {
     result = values->Element(s);
     if ( selectedStr ) *selected=-1;
     else *selected=i;
    }
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 // scrollable clickable left justified button list
 int FastGUI::ljustlist( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll, int scrollw, int linespace, int charspace, bool no_rectangle ) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  int rowHeight = (int)(tfont.h+linespace);
  int h=(int)(lines*rowHeight);
  //box(fg_C,x,y,cw,h);
  Blending(none);
  if ( !no_rectangle ) Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  EACH(values->first,String,s) {
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && ljustbutton(font,s->s.c_str(),x,y+(rowHeight*(i-(*scroll))+linespace/2),cw,(int)tfont.h) )
     result = i;
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,scrollw,scrollw)) *scroll+=1;
   if (button(downArrow,x+cw+3,y+h-scrollw,scrollw,scrollw)) *scroll-=1; 
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines/2,
     x+cw+3+2,y+scrollw+linespace,
    scrollw,h-(scrollw+linespace)*2,
    true,true
   );
  }
  return result;
 }

 // scrollable clickable left justified button list
 int FastGUI::ljustlist( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, int scrollw, int linespace, int charspace, bool no_rectangle ) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  int rowHeight = (int)(tfont.h+linespace);
  int h=(int)(lines*rowHeight);
  //box(fg_C,x,y,cw,h);
  Blending(none);
  if ( !no_rectangle ) Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  EACH(values->first,String,s) {
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && ljustbutton(font,s->s.c_str(),x,y+(rowHeight*(i-(*scroll))+linespace/2),cw,(int)tfont.h,(selected&&(*selected)==i?true:false)) ) {
    result = i;
    if ( selected ) (*selected)=i;
   }
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,scrollw,scrollw)) *scroll+=1;
   if (button(downArrow,x+cw+3,y+h-scrollw,scrollw,scrollw)) *scroll-=1; 
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines/2,
     x+cw+3+2,y+scrollw+linespace,
    scrollw,h-(scrollw+linespace)*2,
    true,true
   );
  }
  return result;
 }

 // clickable, scrollable center justified button list
 int FastGUI::list( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll, int linespace, int charspace, bool no_rectangle ) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  int h=(int)(lines*(fontSize.y+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  if ( !no_rectangle ) Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(fontSize.y+15);
  EACH(values->first,String,s) {
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(font,s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h+2))) )
     result = i;
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 // scrollable clickable centered list with title using a texfont with custom backing
 int FastGUI::list( GLImage *backing, TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll, int linespace, int charspace ) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  int h=(int)(lines*(tfont.h+15));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+15);
  EACH(values->first,String,s) {
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(backing,font,s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h+2))) )
     result = values->Element(s);
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 // selectable scrollable clickable centered list with title using a texfont with custom backing
 int FastGUI::list( GLImage *backing, TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, int linespace, int charspace ) { 
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  int h=(int)(lines*(tfont.h+15));
  Text(font,color,tblend,(x+cw/2)-gw/2,y-(int)(tfont.h)-3,(int)tfont.w,(int)tfont.h,linespace,charspace,label);
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+15);
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(backing,font,s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h+2),selectedStr)) )
     result = values->Element(s);    
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 // multi-selectable scrollable clickable centered list with title using a texfont with custom backing
 bool FastGUI::list( GLImage *backing, TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, Numbers *selected, int *scroll, int linespace, int charspace ) { 
  bool changed=false;
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-(int)tfont.h,(int)tfont.w,(int)tfont.h,linespace,charspace,label);
  int h=(int)(lines*(tfont.h+15));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  if ( backing ) art.Stretch(backing,color,transparency,x,y,cw,h);
  else Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+15);
  EACH(values->first,String,s) {
   Number *selection=selected->find(i);
   bool selectedStr = selection != null;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(backing,font,s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h+2),cw-8)) ) {
    if ( selection ) selected->Remove(selection);
    else selected->Add(i);
    changed=true;
   }
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return changed;
 }

 // multi-selectable scrollable clickable centered list with title using a texfont
 bool FastGUI::list( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, Numbers *selected, int *scroll, int linespace, int charspace ) { 
  bool changed=false;
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-(int)tfont.h,(int)tfont.w,(int)tfont.h,linespace,charspace,label);
  int h=(int)(lines*(tfont.h+15));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+15);
  EACH(values->first,String,s) {
   Number *selection=selected->find(i);
   bool selectedStr = selection != null;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(rounded,font,s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h+2),cw-8)) ) {
    if ( selection ) selected->Remove(selection);
    else selected->Add(i);
    changed=true;
   }
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return changed;
 }
  
 
/// toggle list  ////////////////////////////////////////////////////////////////////////////////////////////

 int FastGUI::togglelist( TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *scroll) {
  Text(label,color,transparency,(double)(x+cw/2),(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int h=(int)(lines*(tfont.h+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+5);
  Cartesian placement;
  EACH(values->first,String,s) {
   placement.SetRect(x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(tfont.h));
   if ( (i-*scroll >=0 && i-*scroll < lines) ) {
    s->integer=toggle("",s->integer>0?true:false,placement.x+2,placement.y,placement.h,placement.h)?1:0;
    if ( button(rounded,font,s->s.c_str(),placement.x+2+placement.h,placement.y,placement.w-placement.h-2,placement.h) ) {
     result = values->Element(s);
    }
   }
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 
/// horizontal list (tab-like)  ////////////////////////////////////////////////////////////////////////////////////////////

 int FastGUI::hlist(const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll, int linespace, int charspace, bool showRectangle ) {
  int gw=strlen(label)*((int)fontSize.x+2);
  Blending(tblend);
  text(label,(x+cw/2)-gw/2,y-(int)(fontSize.y/2.0),false,color,none);
  int h=(int)(lines*(fontSize.y+15.0));
  Blending(none);
  if ( showRectangle ) Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight=(int)(fontSize.y+5.0);
  EACH(values->first,String,s) {
   bool selectedStr = selected?(i==*selected):false;
   if ( (!scroll||(i-*scroll >=0 && i-*scroll < lines))
     && (button(s->s.c_str(),x+(cw+8)*i,(int)(y),cw,(int)(tfont.h),selectedStr)) ) {
    result = values->Element(s);
    if (selected) *selected=result;
   }
   i++;
  }
  if(scroll && values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 int FastGUI::hlist(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int *scroll,int linespace, int charspace, bool showRectangle ) {
  int gw=WidthTexFont(font,tfont.w,fontCharSpace.value,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  int h=(int)(lines*(tfont.h+15));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  if ( showRectangle ) Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int result=-1;
  int rowHeight = (int)(tfont.h+5);
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (button(font,s->s.c_str(),x+(cw+8)*i,(int)(y),cw,(int)(tfont.h),selectedStr)) ) {
    result = values->Element(s);
    *selected=result;
   }
   i++;
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
  return result;
 }

 
/// tabs  ////////////////////////////////////////////////////////////////////////////////////////////

 int FastGUI::tabs(const char * label, Strings * values, int x, int y, int cw, int *selected, int *scroll, int linespace,int charspace, int limitString ) {
  int gw=strlen(label)*(int)(charspace+fontSize.x);
  text(label,(x+cw/2)-gw/2,y-(int)fontSize.y,false,color,none);
  //box(fg_C,x,y,cw,h);
  Blending(none);
  int i=0;
  int result=-1;
  Crayon oldColor; oldColor=color;
  Crayon oldHighlight;
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( selectedStr ) color.Pick(black);
   if ( (button((limitString>0?s->s.substr(0,limitString):s->s).c_str(),
                x+(int)(cw*(i-*scroll)+4),y,cw-2,(int)(fontSize.y+linespace),selectedStr,charspace)) ) {
    result = values->Element(s);
    *selected=result;
   }
   if ( selectedStr ) color=oldColor;
   i++;
  }
  return result;
 }

 int FastGUI::tabs(TexFont *font, const char * label, Strings * values, int x, int y, int cw, int *selected, int *scroll, int linespace, int charspace, int limitString ) {
  int gw=WidthTexFont(font,tfont.w,charspace,label);
  Text(font,color,tblend,(x+cw/2)-gw/2,y-tfont.h,tfont.w,tfont.h,linespace,charspace,label);
  //box(fg_C,x,y,cw,h);
  Blending(none);
  int i=0;
  int result=-1;
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( (button(font,(limitString>0?s->s.substr(0,limitString):s->s).c_str(),
                x+(int)(cw*(i-*scroll)+4),y,cw-2,(int)(tfont.h),selectedStr,charspace)) ) {
    result = values->Element(s);
    *selected=result;
   }
   i++;
  }
  return result;
 }

 int FastGUI::tabs(  Strings *options, int selected, int x, int y, int cw, int bh ) {
  int result=selected;
  int sx=x;
  int sy=y;
  EACH(options->first,String,s) {
   if ( button(s->s.c_str(),sx,sy,cw,bh,selected==s->integer) ) {
    result=s->integer;
   }
   sx+=cw;
  }
  return result;
 }

 
 int FastGUI::tabs( TexFont *f, Strings *options, int selected, int x, int y, int cw, int bh, int spacing ) {
  int result=selected;
  int sx=x;
  int sy=y;
  EACH(options->first,String,s) {
   if ( button(f,s->s.c_str(),sx,sy,cw,bh,selected==s->integer) ) {
    result=s->integer;
   }
   sx+=cw+spacing;
  }
  return result;
 }

 // quicktabs

 int FastGUI::quicktabs( TexFont *font, const char *label, LinkedList *list, int x, int y, int cw, ListItem *selected, int linespace, int charspace, int limitString, bool canAdd ) {
  if ( canAdd && button(font,"+",x,y,cw-2,tfont.h+5) ) return -2;
  int selection=list->IndexOf(selected);
  Strings values;
  for ( int i=0; i<list->count; i++ ) values.Add(FORMAT("%d",(i+1)));
  int scroll=0;
  return tabs(font,label,&values,x+cw,y,cw,&selection,&scroll,linespace,charspace,limitString);
 }

 int FastGUI::quicktabs( const char *label, LinkedList *list, int x, int y, int cw, ListItem *selected, int linespace, int charspace, int limitString, bool canAdd ) {
  if ( canAdd && button("+",x,y,cw-2,tfont.h+5) ) return -2;
  int selection=list->IndexOf(selected);
  Strings values;
  for ( int i=0; i<list->count; i++ ) values.Add(FORMAT("%d",(i+1)));
  int scroll=0;
  return tabs(label,&values,x+cw,y,cw,&selection,&scroll,linespace,charspace,limitString);
 }


 /// list /////////////////////////////////////////////////////////////////////////////

 // quick list

 int FastGUI::quicklist( TexFont *font, const char *label, LinkedList *L, int x, int y, int cw, ListItem *selected, int lines, int *scroll, int linespace, int charspace, bool canAdd ) {
  if ( canAdd && button(font,"+",x,y,cw-2,tfont.h+5) ) return -2;
  int selection=L->IndexOf(selected);
  Strings values;
  for ( int i=0; i<L->count; i++ ) values.Add(FORMAT("%d",(i+1)));
  if ( scroll ) {
   return list( font, label, &values, x+cw, y, cw, lines, &selection, scroll, linespace, charspace=2);
  } else {
   int scrolled=0;
   return list( font, label, &values, x+cw, y, cw, lines, &selection, &scrolled, linespace, charspace=2);
  }
 }

 // orderable list

 void FastGUI::order(const char * label, Strings * values, int x, int y, int cw, int lines, int *selected, int * scroll) {
  Text(label,color,transparency,(double)(x+cw/2),(double)y-fontSize.y,fontSize.x,fontSize.y,charSpace,lineSpace,true);
  int h=(int)(lines*(fontSize.y+7));
  //box(fg_C,x,y,cw,h);
  Blending(none);
  Rectangle(color,x,y,x+cw,y+h);
  int i=0;
  int rowHeight = (int)(fontSize.y+5);
  EACH(values->first,String,s) {
   bool selectedStr = i==*selected;
   if ( (i-*scroll >=0 && i-*scroll < lines)
     && (smbutton(s->s.c_str(),x,(int)(y+rowHeight*(i-*scroll)+5),cw,(int)(fontSize.y),selectedStr)) )
     *selected = values->Element(s);
   i++;
  }
  if (button(downArrow,x,y+h+5,20,20)) {
   ListItem *selection = values->Element(*selected);
   if(selection && selection != values->last ) {
    ListItem *p=selection->next;
    values->Remove(selection);
    values->InsertAfter(selection,p);
    *selected+=1;
   }
  }
  if (button(upArrow,x+cw-20,y+h+5,20,20)) {
   ListItem *selection = values->Element(*selected);
   if(selection && selection != values->first ) {
    ListItem *p=selection->prev;
    values->Remove(selection);
    values->InsertBefore(selection,p);
    *selected-=1;
   }
  }
  if(values->count - lines > 0) {
   if (button(upArrow,x+cw+3,y,15,15)) *scroll-=1;
   if (button(downArrow,x+cw+3,y+h-15,15,15)) *scroll+=1;
   (*scroll)=(int)vslider("",(double)(*scroll),0.0,values->count-lines,x+cw+10,y+20,5,h-40,true,true);
  }
 }

 
 /// textbox /////////////////////////////////////////////////////////////////////////////

 ////
 string FastGUI::textbox( const char *label, string value, FastExtents e, int characters, bool limited, int lines, int lineSpace, int border ) {
  return textbox(label,value,e.x,e.y,characters,limited,lines,lineSpace,border);
 }
 string FastGUI::textbox( const char *label, string value, int x, int y, int characters, bool limited, int lines, int lineSpace, int border ) {
  int w= border*2+characters*fontW;
  int h= border*2+lines*fontH+lineSpace*(lines-1);
  GetState(x,y,w,h);
  Crayon t=color;
  if ( cancelled ) {
   value="";
  } else if ( hovering ) {
   char key=input.keyboard();
   if ( LOWER(key) != lastKey && key != '\0' ) {
    value+=key;
   }
   if ( (input.KeyUp(DX_BACKSPACE) || input.KeyUp(DX_DELETE)) ) {
    value = value.substr(0, value.size()-1);
   }
   if ( input.KeyUp(DX_ENTER) ) {
    if ( lines == 1 ) { // Trigger unfocused flag
     last.Set(fg_unfocused,true);
    } else if ( lastKey != '\n' ) {
     value+='\n';
    }
   }
   lastKey=LOWER(key);
   t=activated;
  } else t=deactivated;
  art.BevelRect(t,8.0,(double)x,(double)y,(double)w,(double)h);
  Text(label, crayons.Pick(alabaster),none, (double)(x+w/2),(double)y-smallFontSize.y/2,smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  if ( hovering ) {
   art.Element2(t,crayons.Pick(clear),bestrectforaspect(w,h),(double)x,(double)y,(double)w,(double)h);
  } else {
  }  
  Text(value.c_str(),( hovering ? crayons.Pick(green255) : crayons.Pick(alabaster) ),transparency,x+border,border+y,fontSize.x,fontSize.y,charSpace,(double)lineSpace,false);
  lastSize.SetRect( x,y-smallFontH, w, h+border+smallFontH );
  return value;
 }

 string FastGUI::textbox( TexFont *font, TexFont *labelFont, const char *label, string value, FastExtents e, int characters, bool limited, int lines, int lineSpace, int border, bool alwaysKeys, int kern, Zstring *goFullscreen, GLWindow *back_to, bool subcaption, bool textCloserMode, bool useTerminal ) {
  return textbox(font,tblend,labelFont,tblend,label,value,e.x,e.y,characters,limited,lines,lineSpace,border,alwaysKeys,kern,goFullscreen,back_to,subcaption,textCloserMode);
 }

 string FastGUI::textbox( TexFont *font, Blends fontBlend, TexFont *labelFont, Blends labelFontBlend, const char *label, string value, int x, int y, int characters, bool limited, int lines, int lineSpace, int border, bool alwaysKeys, int kern, Zstring *goFullscreen, GLWindow *back_to, bool subcaption, bool textCloserMode, bool useTerminal ) {
  int w= border*2+characters*(tfont.w+kern);
  int h= border*2+lines*tfont.h+lineSpace*(lines-1);
  GetState(x,y,w,h);
  if ( cancelled ) {
   value="";
  } else if ( hovering || alwaysKeys ) {
   char key=input.keyboard();
   bool holding=input.KeyDown(DX_BACKSPACE) || input.KeyDown(DX_DELETE);
   holding = holding && (FRAME%(FPS/2)==0);
   if ( (input.KeyUp(DX_BACKSPACE) || input.KeyUp(DX_DELETE)) || holding ) {
    value = value.substr(0, value.size()-1);
   } else if ( last_line_length(value.c_str()) <= characters && (key) != lastKey && key != '\0' ) {
    if ( last_line_length(value.c_str()) == characters ) {
      if ( linefeeds(value.c_str()) < lines ) {
       if (key == ' ') value+="\n";
       else {
        value+=key;
        value=word_wrap_for_textbox(value.c_str());
       }
      } else {}
    } else value+=key;
   }
   if ( input.KeyUp(DX_ENTER) ) {
    if ( lines == 1 ) { // Trigger unfocused flag
     last.Set(fg_unfocused,true);
    } else if ( lastKey != '\n' ) {
     if ( linefeeds(value.c_str()) < lines ) value+='\n';
    }
   }
   lastKey=LOWER(key);
  } else deactivated.gl();
  art.BevelRect2(rr_border, hovering ? activated : deactivated,none,8.0,3.0,x,y,w,h);
//  mD.Render2dVC((float)x,(float)y,(float)w,(float)h+border);
  int tw=WidthTexFont(labelFont,tfont.w,0,label);
  if ( subcaption ) {
   if ( textCloserMode ) Text(labelFont,highlight,labelFontBlend,(x+w/2)-tw/2,y+(tfont.h*lines+1)+lineSpace+tfont.h/2,tfont.w,tfont.h,lineSpace,0,label);
   else Text(labelFont,highlight,labelFontBlend,(x+w/2)-tw/2+4,y+tfont.h*(lines+1)-4,tfont.w,tfont.h,lineSpace,0,label);
  } else {
   if ( textCloserMode ) Text(labelFont,highlight,labelFontBlend,(x+w/2)-tw/2,y-(int)((double)tfont.h*1.25),tfont.w,tfont.h,lineSpace,0,label);
   else Text(labelFont,highlight,labelFontBlend,(x+w/2)-tw/2+4,y-tfont.h*2-4,tfont.w,tfont.h,lineSpace,0,label);
  }
  WriteTexFont(
   font,( hovering ? crayons.Pick(green255) : crayons.Pick(alabaster) ),
   fontBlend,x+border,border+y+lineSpace/2,tfont.w,tfont.h,lineSpace,
   (hovering||alwaysKeys)&&(FRAME%120>60)?(value+"|").c_str():value.c_str(),
   kern
  );
  lastSize.SetRect( x,y-smallFontH, w, h+border+smallFontH );
  if ( goFullscreen ) {
   if ( button(rightArrow, x+w-16, y-16, 16, 16) ) {
    if ( !useTerminal ) {
     FullscreenStringEditor *fse=new FullscreenStringEditor(back_to);
     fse->out=goFullscreen;
     fse->limited=limited;
     fse->font=font;
     fse->fontBlend=fontBlend;
     fse->labelFont=labelFont;
     fse->labelFontBlend=labelFontBlend;
     fse->label=label;
     windows.Add(fse);
     back_to->visible=false;
     return value;
    } else {
     FullscreenStringTerminalEditor *fse=new FullscreenStringTerminalEditor(back_to);
     fse->out=goFullscreen;
     fse->limited=limited;
     fse->font=font;
     fse->fontBlend=fontBlend;
     fse->labelFont=labelFont;
     fse->labelFontBlend=labelFontBlend;
     fse->label=label;
     windows.Add(fse);
     back_to->visible=false;
     return value;
    }
   }
  }
  return value;
 }

 // stringbox

 string FastGUI::stringbox( TexFont *font, Blends fontBlend, TexFont *labelFont, Blends labelFontBlend, const char *label, string value, int x, int y, int characters, bool limited, int lines, int lineSpace, int border, bool alwaysKeys, int kern, String *goFullscreen, GLWindow *back_to, bool subcaption, bool textCloserMode ) {
  int w= border*2+characters*(tfont.w+kern);
  int h= border*2+lines*tfont.h+lineSpace*(lines-1);
  GetState(x,y,w,h);
  if ( cancelled ) {
   value="";
  } else if ( hovering || alwaysKeys ) {
   char key=input.keyboard();
   bool holding=input.KeyDown(DX_BACKSPACE) || input.KeyDown(DX_DELETE);
   holding = holding && (FRAME%(FPS/2)==0);
   if ( (input.KeyUp(DX_BACKSPACE) || input.KeyUp(DX_DELETE)) || holding ) {
    value = value.substr(0, value.size()-1);
   } else if ( last_line_length(value.c_str()) <= characters && (key) != lastKey && key != '\0' ) {
    if ( last_line_length(value.c_str()) == characters ) {
      if ( linefeeds(value.c_str()) < lines ) {
       if (key == ' ') value+="\n";
       else {
        value+=key;
        value=word_wrap_for_textbox(value.c_str());
       }
      } else {}
    } else value+=key;
   }
   if ( input.KeyUp(DX_ENTER) ) {
    if ( lines == 1 ) { // Trigger unfocused flag
     last.Set(fg_unfocused,true);
    } else if ( lastKey != '\n' ) {
     if ( linefeeds(value.c_str()) < lines ) value+='\n';
    }
   }
   lastKey=LOWER(key);
  } else deactivated.gl();
  rrect(rr_border, hovering ? activated : deactivated, hovering ? activated : deactivated,x,y,w,h);
//  mD.Render2dVC((float)x,(float)y,(float)w,(float)h+border);
  int tw=WidthTexFont(labelFont,tfont.w,0,label);
  if ( subcaption ) {
   if ( textCloserMode ) Text(labelFont,color,labelFontBlend,(x+w/2)-tw/2,y+h+tfont.h+lineSpace,tfont.w,tfont.h,lineSpace,0,label);
   else Text(labelFont,color,labelFontBlend,(x+w/2)-tw/2,y+tfont.h*(lines+1),tfont.w,tfont.h,lineSpace,0,label);
  } else {
   if ( textCloserMode ) Text(labelFont,color,labelFontBlend,(x+w/2)-tw/2,y-(int)((double)tfont.h*(lines)),tfont.w,tfont.h,lineSpace,0,label);
   else Text(labelFont,color,labelFontBlend,(x+w/2)-tw/2,y-(tfont.h*2+border),tfont.w,tfont.h,lineSpace,0,label);
  }
  WriteTexFont(
   font,( hovering ? crayons.Pick(green255) : crayons.Pick(alabaster) ),
   fontBlend,x+border,border+y+lineSpace/2,tfont.w,tfont.h,lineSpace,
   (hovering||alwaysKeys)&&(FRAME%120>60)?(value+"|").c_str():value.c_str(),
   kern
  );
  lastSize.SetRect( x,y-smallFontH, w, h+border+smallFontH );
  if ( goFullscreen ) {
   if ( button(rightArrow, x+w-16, y-16, 16, 16) ) {
    FullscreenStringClassTerminalEditor *fse=new FullscreenStringClassTerminalEditor(back_to);
    fse->out=goFullscreen;
    fse->limited=limited;
    fse->font=font;
    fse->fontBlend=fontBlend;
    fse->labelFont=labelFont;
    fse->labelFontBlend=labelFontBlend;
    fse->label=label;
    windows.Add(fse);
    back_to->visible=false;
    return value;
   }
  }
  return value;
 }

  
 /// rectangles / gui shapes  /////////////////////////////////////////////////////////////////////////////
 ////
 void FastGUI::rect( int rectType, FastExtents e ) { rect( rectType, e.x,e.y,e.w,e.h ); }
 void FastGUI::rect( int rectType, int x, int y, int w, int h ) {
  switch ( rectType ) {
   default:
    case 0: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 1: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 2: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 3: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 4: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 5: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 6: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 7: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 8: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
    case 9: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
   case 10: art.Element(art.stash.rr_Standard+10, (float)x,(float)y,(float)w,(float)h ); break;
  }
 }

 void FastGUI::rect( Crayon c, int rectType, FastExtents e ) { rect( c, rectType, e.x,e.y,e.w,e.h ); }
 void FastGUI::rect( Crayon c, int rectType, int x, int y, int w, int h ) {
  switch ( rectType ) {
   default: art.Element( c, art.stash.rectangle, (double)x,(double)y,(double)w,(double)h ); break;
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: 
    case 9: art.Element( c, art.stash.rr_Standard+rectType*4, (double)x,(double)y,(double)w,(double)h ); break;
   case 10: art.Element( c, art.stash.rr_Standard+rectType*4-1, (double)x,(double)y,(double)w,(double)h ); break;
  }
 }

 void FastGUI::rrect( Crayon border, Crayon inner, Crayon outer, FastExtents e ) { rrect( border,inner,outer, e.x,e.y,e.w,e.h ); }
 void FastGUI::rrect( Crayon border, Crayon inner, Crayon outer, int x, int y, int w, int h ) {
  art.Element2(border,inner,fast.stashed_FilledSquare,(double)x,(double)y,(double)w,(double)h,(double)rect_borderwidth);
 }
 
 /// text drawing /////////////////////////////////////////////////////////////////////////////
 //// Text drawing.

 void FastGUI::textsize( Dim2i *out, TexFont *font, const char *txt ) {
  int len=strlen(txt);
  out->w=WidthTexFont(font,tfont.w,fontCharSpace,txt);
  out->h=(tfont.h+fontLineSpace)*UMIN(1,linefeeds(txt));
 }

 void FastGUI::textsize( Dim2i *out, TexFont *font, const char *txt, int charspace ) {
  int len=strlen(txt);
  out->w=WidthTexFont(font,tfont.w,charspace,txt);
  out->h=(tfont.h+fontLineSpace)*UMIN(1,linefeeds(txt));
 }

 void FastGUI::textsize( Dim2i *out, TexFont *font, const char *txt, int charspace, int linespace ) {
  int len=strlen(txt);
  out->w=WidthTexFont(font,tfont.w,charspace,txt);
  out->h=(tfont.h+linespace)*UMIN(1,linefeeds(txt));
 }

 void FastGUI::textsize( Dim2d *out, const char *txt, double fw, double fh, double linespace ) {
  int len=strlen(txt);
  out->w=(2.0+fw)*(double)len;
  out->h=((double)linespace+fh)*(double)UMIN(1,linefeeds(txt));
 }

 void FastGUI::textsize( Dim2d *out, const char *txt, double fw, double fh ) {
  int len=strlen(txt);
  out->w=(2.0+fw)*(double)len;
  out->h=((double)lineSpace+fh)*(double)UMIN(1,linefeeds(txt));
 }

 void FastGUI::textsize( Dim2d *out, const char *txt ) {
  int len=strlen(txt);
  out->w=(2.0+fontSize.x)*(double)len;
  out->h=((double)lineSpace+fontSize.y)*(double)UMIN(1,linefeeds(txt));
 }
 
 /// font selector /////////////////////////////////////////////////////////////////////////////

 TexFont *FastGUI::fontSelect( TexFont *value, int x, int y, int w, int h ) {
  TexFont *result=value;
  if ( !result ) result=glowTermFont;
  if ( button("",x,y,w,h) ) {
   TexFontHandle *handle=tfonts.find(result,true);
   if ( handle ) {
    handle = (TexFontHandle *) (handle->next);
   }
   if ( !handle ) handle=(TexFontHandle *) tfonts.first;
   result=handle?handle->p:null;
  }
  if ( result ) {
   Blending(tblend);
   text(result,"Abc1234",x+w/2,y+h/2,w/7,h,0,0,( hovering ? (clicking ? highlight : excited ) : tint ));
   Text(result->name.c_str(),color,transparency,(double)(x+w+2),(double)y+h,(double)h*0.8,(double)h,charSpace,lineSpace,false);
  }
  return result;
 }
 
 /// text drawing /////////////////////////////////////////////////////////////////////////////

 void FastGUI::text( const char *label, int x, int y, bool centered, Crayon c, Blends b ) {
  Text(label,c,b,(double)x,(double)y,fontSize.x,fontSize.y,charSpace,lineSpace,centered);
 }

 void FastGUI::boxedtext( const char *label, int x, int y, bool centered, Crayon c, Crayon bord, Crayon back, int padding, bool above ) {
  int charWidth=longest_line_length(label);
  int lineHeight=linefeeds(label);
  if ( centered ) {
   Blending(none);
   int tw=(charWidth*((int)fontSize.x+1));
   int th=(lineHeight*((int)fontSize.y-1));
   int w2=tw/2;
   int h2=th/2;
   int bw=tw+padding*2;
   int bh=th+padding*2;
   int offsetY=above ? -bh : 0;
   Cartesian area(x-w2-padding,y-h2-padding+offsetY,bw,bh);
   Area(back,area.x,area.y,area.x2,area.y2);
   Rectangle(bord,area.x,area.y,area.x2,area.y2);
   Text(label,c,transparency,(double)(x-w2),(double)(y+padding)-fontSize.y/2.0+(double)offsetY,fontSize.x,fontSize.y,0.1,1.0,false);
  } else {
   Blending(none);
   int tw=(charWidth*((int)fontSize.x+1));
   int th=(lineHeight*((int)fontSize.y-1));
   int bw=tw+padding*2;
   int bh=th+padding*2;
   int offsetY=above ? -bh : 0;
   Cartesian area(x-padding,y-padding+offsetY,bw,bh);
   Area(back,&area);
   Rectangle(bord,&area);
   Text(label,c,transparency,(double)x,(double)(y+offsetY),fontSize.x,fontSize.y,0.1,1.0,false);
  }
 }

 void FastGUI::boxedtextrjust( const char *label, int x, int y, bool centered, Crayon c, Crayon bord, Crayon back ) {
  int charWidth=longest_line_length(label)-1;
  int lineHeight=linefeeds(label);
  int Cw=charWidth*((int)fontSize.x+2);
  int Lh=lineHeight*((int)fontSize.y+lineSpace);
  Blending(none);
  Area(back,x-4-(8+Cw),y-4,x+4,y-4+8+Lh);
  Rectangle(bord,x-4-(8+Cw),y-4,x+4,y-4+8+Lh);
  if ( centered ) {
   Text(label,c,transparency,(double)(x-(8+Cw)/2),(double)y+(double)(Lh/2.0-fontSize.y/2.0),fontSize.x,fontSize.y,charSpace,lineSpace,centered);
  } else {
   Text(label,c,transparency,(double)(x-(8+Cw)),(double)y,fontSize.x,fontSize.y,charSpace,lineSpace,centered);
  }
 }

 // typing animation

 int FastGUI::types( int typing, TexFont *font, const char *text, Crayon tint, int x, int y, int fw, int fh, int cs, int ls ) {
  typing++;  
  Zstring content(text);
  content=content.value.substr(0,(size_t)(int)typing);
  WriteTexFont(digicaps,crayons.Pick(gold),font->blend,x,y,fw,fh,ls,content.c_str(),cs);
  return typing;
 }

 void FastGUI::text( TexFont *font, const char *label, FastExtents e, int cs, int ls, Crayon c, Blends b ) {
  text(font,label,e.x,e.y,e.w,e.h,cs,ls,c,b);
 }

 void FastGUI::text( TexFont *font, const char *label, int x, int y, int fw, int fh, int cs, int ls, Crayon c, Blends b ) {
  int gw=WidthTexFont(font,fw,fontCharSpace.value,label);
  Text(font, c, b,
   (x)-gw/2, (y)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,ls,cs,label
  );
 }

 void FastGUI::text( TexFont *font, const char *label, int x, int y, Crayon c, Blends b, int cs, int ls ) {
  int fw=tfont.w;
  int fh=tfont.h;
  int gw=WidthTexFont(font,fw,fontCharSpace.value,label);
  Text(font, c, b,
   (x)-gw/2, (y)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,ls,cs,label
  );
 }

 void FastGUI::ljust( TexFont *font, const char *label, int x, int y, int fw, int fh, int cs, int ls, Crayon c, Blends b ) {
  Text(font, c, b,
   (x), (y)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,ls,cs,label
  );
 }

 void FastGUI::ljust( TexFont *font, const char *label, int x, int y, int cs, int ls, Crayon c, Blends b ) {
  int fw=tfont.w;
  int fh=tfont.h;
  Text(font, c, b,
   (x), (y)-(int)((float)fh*(font->baselineOffset))/2,
   fw,fh,ls,cs,label
  );
 }

 
 /// image display /////////////////////////////////////////////////////////////////////////////

 void FastGUI::image( GLImage *i, int x, int y, int w, int h, Crayon t, Blends b ) {
  GLImage *displaying = (i ? i : unknown);
  Cartesian size;
  if ( displaying->width < w && displaying->height < h ) {
   size.SetRect(
    x+(w-displaying->width)/2,
    y+(h-displaying->height)/2,
    displaying->width,
    displaying->height
   );
  } else size.SetRect(x,y,w,h);
   /* TODO: fit: you should implement this else if ( displaying->width > w && displaying->height > h ) {
   if ( w > h ) {
    double aspect=iratiod(h,w);
   } else {
    double aspect=iratiod(w,h);
   }
  } else {
   if ( displaying->width > w ) {

   } else {
   }
  } */
  art.Stretch(displaying, t, b, size.x,size.y,size.w,size.h );
 }

 
 /// arrow controlpad /////////////////////////////////////////////////////////////////////////////


 int FastGUI::arrows( int x, int y, int bw, bool disabled, bool showDelta, int delta ) {
  int result=-1;
  if ( squarebutton(leftArrow, rounded,x-bw/2-bw,y,        bw,bw,color,tint,deactivated,disabled) ) result=DX_LEFT;
  if ( showDelta && hovering ) display_delta( -delta );
  if ( squarebutton(upArrow,   rounded,x-bw/2,   y-bw/2-bw,bw,bw,color,tint,deactivated,disabled) ) result=DX_UP;
  if ( showDelta && hovering ) display_delta( -delta );
  if ( squarebutton(downArrow, rounded,x-bw/2,   y+bw/2,   bw,bw,color,tint,deactivated,disabled) ) result=DX_DOWN;
  if ( showDelta && hovering ) display_delta( delta );
  if ( squarebutton(rightArrow,rounded,x+bw/2,   y,        bw,bw,color,tint,deactivated,disabled) ) result=DX_RIGHT;
  if ( showDelta && hovering ) display_delta( delta );
  return result;
 }

 int FastGUI::arrows( const char *label, int x, int y, int bw, bool disabled, bool showDelta, double delta ) {
  int result=-1;
  if ( squarebutton(leftArrow, rounded,x-bw,y,bw,bw,   color,tint,deactivated,disabled) ) result=DX_LEFT;
  if ( showDelta && hovering ) display_delta( -delta );
  if ( squarebutton(upArrow,   rounded,x,   y-bw,bw,bw,color,tint,deactivated,disabled) ) result=DX_UP;
  if ( showDelta && hovering ) display_delta( -delta );
  if ( squarebutton(downArrow, rounded,x,   y+bw,bw,bw,color,tint,deactivated,disabled) ) result=DX_DOWN;
  if ( showDelta && hovering ) display_delta( delta );
  if ( squarebutton(rightArrow,rounded,x+bw,y,bw,bw,   color,tint,deactivated,disabled) ) result=DX_RIGHT;
  if ( showDelta && hovering ) display_delta( delta );
  Text(label,tint,transparency,(double)(x),(double)(y+bw*2+smallFontSize.y),smallFontSize.x,smallFontSize.y,charSpace,lineSpace,true);
  return result;
 }

 void FastGUI::arrows( const char *label, Cartesian *xy, int x, int y, int bw, bool disabled, int delta ) {
  delta=integer_delta(delta);
  int factor=delta;
  Text(FORMAT("%s\nx %d\ny %d",label,xy->x,xy->y).c_str(),color,transparency,(double)x+(double)bw*2.0+2.0,(double)y,fontSize.x,fontSize.y,charSpace,lineSpace,false);
  int ar=arrows("",x,y,bw,disabled,true,delta);
  if ( ar >= 0 ) {
   switch ( ar ) {
     case DX_LEFT: xy->MoveBy((int)-factor,0); break;
    case DX_RIGHT: xy->MoveBy((int)factor,0); break;
       case DX_UP: xy->MoveBy(0,(int)-factor); break;
     case DX_DOWN: xy->MoveBy(0,(int)factor); break;
   }
  }
 }

 void FastGUI::arrows( const char *label, Cartesiand *xy, int x, int y, int bw, bool disabled, double delta ) {
  delta=number_delta(delta);
  double factor=delta;
  Text(FORMAT("%s\nx %1.1f\ny %1.1f",label,xy->x,xy->y).c_str(),color,transparency,(double)x+(double)bw*2.0+2.0,(double)y,fontSize.x,fontSize.y,lineSpace,false);
  int ar=arrows("",x,y,bw,disabled,true,delta);
  if ( ar >= 0 ) {
   switch ( ar ) {
     case DX_LEFT: xy->MoveBy(-factor,0.0); break;
    case DX_RIGHT: xy->MoveBy(factor,0.0); break;
       case DX_UP: xy->MoveBy(0.0,-factor); break;
     case DX_DOWN: xy->MoveBy(0.0,factor); break;
   }
  }
 }

 
 /// color picker /////////////////////////////////////////////////////////////////////////////

 Crayon FastGUI::pick( const char *label, Crayon value, int x, int y ) {
  int w=32;
  int deltay=32+4;
  int decimalW=deltay/2;
  int dy=y;
  Text(label, crayons.Pick(alabaster),transparency, (double)(x),(double)y-smallFontSize.y,smallFontSize.x,smallFontSize.y,2.0,1.0,false);
  Crayon R,G,B,A;
  R.Float(value.rf,0.0f,0.0f,1.0f);
  G.Float(0.0f,value.gf,0.0f,1.0f);
  B.Float(0.0f,0.0f,value.bf,1.0f);
  A.Float(value.af,value.af,value.af,1.0f);
  StretchiNoAlpha(rounded,value,transparency,x,y,deltay*5,deltay*4);
  int left=x+w+4;
  value.rf=(float)decimal("R",(double)value.rf,0.00f,1.0f,0.1f,left,dy,32,16,7);
  StretchiNoAlpha(rounded,R,transparency,x+w+64+32,dy,w-8,w-8);
  dy+=deltay;
  value.gf=(float)decimal("G",(double)value.gf,0.00f,1.0f,0.1f,left,dy,32,16,7);
  StretchiNoAlpha(rounded,G,transparency,x+w+64+32,dy,w-8,w-8);
  dy+=deltay;
  value.bf=(float)decimal("B",(double)value.bf,0.00f,1.0f,0.1f,left,dy,32,16,7);
  StretchiNoAlpha(rounded,B,transparency,x+w+64+32,dy,w-8,w-8);
  dy+=deltay;
  value.af=(float)decimal("A",(double)value.af,0.00f,1.0f,0.1f,left,dy,32,16,7);
  StretchiNoAlpha(rounded,A,transparency,x+w+64+32,dy,w-8,w-8);
  dy+=deltay;
  value.Float(value.rf,value.gf,value.bf,value.af);
  return value;
 }

 Zint colorPickerFrame;
 int FastGUI::colorPicker( int value, int x, int y, int cellSize, bool Alpha, GLfloat rectThickness ) {
  Blending(none);
  int returning=value;
  int wh=16*cellSize;
  GetState(x,y,wh,wh);
  colorPickerFrame++;
  for ( int i=0; i<16; i++ ) {
   for ( int j=0; j<16; j++ ) {
    Cartesiand rect;
    int vector=i+j*16;
    if ( vector == value ) rect.SetRect(x+i*cellSize,y+j*cellSize,cellSize,cellSize);
    else rect.SetRect(x+i*cellSize+1,y+j*cellSize+1,cellSize-2,cellSize-2);
    Crayon c=crayons.fansi(i+j*16);
    Area(c,&rect);
    rect.SetRect(x+i*cellSize,y+j*cellSize,cellSize,cellSize);
    if ( rect.within((double)(x)+(double)(wh)*point.x,(double)y+(double)(wh)*point.y) ) {
     glLineWidth(rectThickness);
     c=crayons.fansi(colorPickerFrame);
     Rectangle(c,&rect);
     glLineWidth(1.0);
     if ( clicking ) returning=vector;
    }
   }
  }
  return returning;
 }

 Crayon FastGUI::colorPicker( Crayon cvalue, int x, int y, int cellSize, bool Alpha, GLfloat rectThickness ) {
  Blending(none);
  int value=crayons.fansi(cvalue);
  int returning=value;
  int wh=16*cellSize;
  GetState(x,y,wh,wh);
  colorPickerFrame++;
  for ( int i=0; i<16; i++ ) {
   for ( int j=0; j<16; j++ ) {
    Cartesiand rect;
    int vector=i+j*16;
    if ( vector == value ) rect.SetRect(x+i*cellSize,y+j*cellSize,cellSize,cellSize);
    else rect.SetRect(x+i*cellSize+1,y+j*cellSize+1,cellSize-2,cellSize-2);
    Crayon c=crayons.fansi(i+j*16);
    Area(c,&rect);
    rect.SetRect(x+i*cellSize,y+j*cellSize,cellSize,cellSize);
    if ( rect.within((double)(x)+(double)(wh)*point.x,(double)y+(double)(wh)*point.y) ) {
     glLineWidth(rectThickness);
     c=crayons.fansi(colorPickerFrame);
     Rectangle(c,&rect);
     glLineWidth(1.0);
     if ( clicking ) returning=vector;
    }
   }
  }
  return crayons.fansi(returning);
 }

 int FastGUI::c16x16Picker( int value, int x, int y, int cellSize, bool Alpha, GLfloat rectThickness ) {
  Blending(none);
  int returning=value;
  int wh=16*cellSize;
  GetState(x,y,wh,wh);
  colorPickerFrame++;
  for ( int i=0; i<16; i++ ) {
   for ( int j=0; j<16; j++ ) {
    Cartesiand rect;
    int vector=i+j*16;
    if ( vector == value ) rect.SetRect(x+i*cellSize,y+j*cellSize,cellSize,cellSize);
    else rect.SetRect(x+i*cellSize+1,y+j*cellSize+1,cellSize-2,cellSize-2);
    Crayon c=crayons.c16x16(i+j*16);
    Area(c,&rect);
    rect.SetRect(x+i*cellSize,y+j*cellSize,cellSize,cellSize);
    if ( rect.within((double)(x)+(double)(wh)*point.x,(double)y+(double)(wh)*point.y) ) {
     glLineWidth(rectThickness);
     c=crayons.c16x16(colorPickerFrame);
     Rectangle(c,&rect);
     glLineWidth(1.0);
     if ( clicking ) returning=vector;
    }
   }
  }
  return returning;
 }

 int FastGUI::c8x16Picker( int value, int x, int y, int cellSize, bool Alpha, GLfloat rectThickness ) {
  Blending(none);
  int returning=value;
  int wh=16*cellSize;
  GetState(x,y,wh,wh);
  colorPickerFrame++;
  for ( int i=0; i<16; i+=2 ) {
   for ( int j=0; j<16; j++ ) {
    Cartesiand rect;
    int vector=i+j*16;
    if ( vector == value ) rect.SetRect(x+i*cellSize/2,y+j*cellSize,cellSize,cellSize);
    else rect.SetRect(x+i*cellSize/2+1,y+j*cellSize+1,cellSize-2,cellSize-2);
    Crayon c=crayons.c16x16(i+j*16);
    Area(c,&rect);
    rect.SetRect(x+i*cellSize/2,y+j*cellSize,cellSize,cellSize);
    if ( rect.within((double)(x)+(double)(wh)*point.x,(double)y+(double)(wh)*point.y) ) {
     glLineWidth(rectThickness);
     c=crayons.c16x16(colorPickerFrame);
     Rectangle(c,&rect);
     glLineWidth(1.0);
     if ( clicking ) returning=vector;
    }
   }
  }
  return returning;
 }

 int FastGUI::c4x16Picker( int value, int x, int y, int cellSize, bool Alpha, GLfloat rectThickness ) {
  Blending(none);
  int returning=value;
  int wh=16*cellSize;
  GetState(x,y,wh,wh);
  colorPickerFrame++;
  for ( int i=2; i<16; i+=4 ) {
   for ( int j=0; j<16; j++ ) {
    Cartesiand rect;
    int vector=i+j*16;
    if ( j > 2 && i < 4 ) vector+=2;
    if ( vector == value ) rect.SetRect(x+i*cellSize/4,y+j*cellSize,cellSize,cellSize);
    else rect.SetRect(x+i*cellSize/4+1,y+j*cellSize+1,cellSize-2,cellSize-2);
    Crayon c=crayons.c16x16(i+j*16);
    Area(c,&rect);
    rect.SetRect(x+i*cellSize/4,y+j*cellSize,cellSize,cellSize);
    if ( rect.within((double)(x)+(double)(wh)*point.x,(double)y+(double)(wh)*point.y) ) {
     glLineWidth(rectThickness);
     c=crayons.c16x16(colorPickerFrame);
     Rectangle(c,&rect);
     glLineWidth(1.0);
     if ( clicking ) returning=vector;
    }
   }
   if ( i == 2 ) i=0;
  }
  return returning;
 }

 int FastGUI::c8x8Picker( int value, int x, int y, int cellSize, bool Alpha, GLfloat rectThickness ) {
  Blending(none);
  int returning=value;
  int wh=16*cellSize;
  GetState(x,y,wh,wh);
  colorPickerFrame++;
  int k=0;
  for ( int i=0; i<8; i+=2 ) {
   for ( int j=0; j<8; j+=2 ) {
    Cartesiand rect;
    int vector=k; k++;
    if ( vector == value ) rect.SetRect(x+i*cellSize/2,y+j*cellSize/2,cellSize,cellSize);
    else rect.SetRect(x+i*cellSize/2+1,y+j*cellSize/2+1,cellSize-2,cellSize-2);
    Crayon c=crayons.c16x16(i+j*16);
    Area(c,&rect);
    crayons.AnySolid();
    rect.SetRect(x+i*cellSize/2,y+j*cellSize/2,cellSize,cellSize);
    if ( rect.within((double)(x)+(double)(wh)*point.x,(double)y+(double)(wh)*point.y) ) {
     glLineWidth(rectThickness);
     c=crayons.c16x16(colorPickerFrame);
     Rectangle(c,&rect);
     glLineWidth(1.0);
     if ( clicking ) returning=vector;
    }
   }
  }
  return returning;
 }

 fx_UglyRGBPal _rgbPal;
 Crayon FastGUI::colorPickerTiny( Crayon tint, int x, int y, int w, int h ) {
  Blending(none);
  GetState(x,y,w,h);
  Crayon temp=tint;
  Cartesiand mouseScreen(input.mxd*vdisplay.w,input.myd*vdisplay.h);
  _rgbPal.Bind(&temp);
  _rgbPal.SetExtents(x,y,w,h);
  _rgbPal.Between(&mouseScreen);
  _rgbPal.Render();
  temp.Alpha(1.0f);
  rgbPalHovering=_rgbPal.hoverB||_rgbPal.hoverG||_rgbPal.hoverR;
  cPalHovering=_rgbPal.hoverC;
  return temp;
 }
 
 /// tween selector /////////////////////////////////////////////////////////////////////////////

 // Tween selector
 Tween *FastGUI::tween( Tween *selected, int x, int y, int w, int h ) {
  Tween *result=selected;
  button("",x,y,w,h);
  GetState(x,y,w,h);
  if ( hovering ) {
   if ( input.KeyDown(DX_1) ) return tweens.find("One");
   if ( input.leftReleased() || input.wheelUp ) {
    if ( result ) result=(Tween *) ( result->next ); 
    if ( !result ) result=(Tween *) tweens.first;
   } else if ( input.rightReleased() || input.wheelDown ) {
    if ( result ) result=(Tween *) ( result->prev ); 
    if ( !result ) result=(Tween *) tweens.last;
   } else if ( input.middleReleased() || !result ) {
    result=tweens.one;
   }
  }
  if ( result ) result->Render(x,y,w,h,crayons.Pick(alabaster));
  return result;
 }
 
 /// coordinate controllers /////////////////////////////////////////////////////////////////////////////

 void FastGUI::xy( Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=decimal("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=decimal("y",inOut->y,xymin,xymax,delta,x+128,y+16,32,16,7,false);
 }

 void FastGUI::xy( Cartesian *inOut, int x, int y, int xymin, int xymax, const char *label ) {
  int delta=integer_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=integer("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,5,false);
  inOut->y=integer("y",inOut->y,xymin,xymax,delta,x+80,y+16,32,16,5,false);
 }

 void FastGUI::xy( having precision, Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta(0.1);
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=precise("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=precise("y",inOut->y,xymin,xymax,delta,x+128,y+16,32,16,7,false);
 }

 void FastGUI::xy( ScreenPosition *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  Cartesiand c;
  inOut->Get(&c);
  xy(&c,x,y,xymin,xymax,label);
  inOut->Set(&c);
 }

 void FastGUI::wh( Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->w=decimal("w",inOut->w,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->h=decimal("h",inOut->h,xymin,xymax,delta,x+128,y+16,32,16,7,false);
 }

 void FastGUI::wh( Cartesian *inOut, int x, int y, int xymin, int xymax, const char *label ) {
  int delta=integer_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->w=integer("w",inOut->w,xymin,xymax,delta,x,y+16,32,16,5,false);
  inOut->h=integer("h",inOut->h,xymin,xymax,delta,x+80,y+16,32,16,5,false);
 }

 void FastGUI::wh( having precision, Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta(0.1);
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->w=precise("w",inOut->w,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->h=precise("h",inOut->h,xymin,xymax,delta,x+128,y+16,32,16,7,false);
 }

 void FastGUI::wh( ScreenPosition *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  Cartesiand c;
  inOut->Get(&c);
  wh(&c,x,y,xymin,xymax,label);
  inOut->Set(&c);
 }

 void FastGUI::xywhi( Cartesiand *inOut, int x, int y, int xymin, int xymax, const char *label ) {
  int delta=(int) integer_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=(double)integer("x",(int)inOut->x,xymin,xymax,delta,x,y+16,32,16);
  inOut->y=(double)integer("y",(int)inOut->y,xymin,xymax,delta,x+96,y+16,32,16);
  inOut->w=(double)integer("w",(int)inOut->w,xymin,xymax,delta,x+96*2,y+16,32,16);
  inOut->h=(double)integer("h",(int)inOut->h,xymin,xymax,delta,x+96*3,y+16,32,16);
  inOut->SetRect(inOut->x,inOut->y,inOut->w,inOut->h);
 }

 void FastGUI::xyz( Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=decimal("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=decimal("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,7,false);
  inOut->z=decimal("z",inOut->z,xymin,xymax,delta,x+112*2,y+16,32,16,7,false);
 }

 void FastGUI::xywh( Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=decimal("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=decimal("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,7,false);
  inOut->w=decimal("w",inOut->w,xymin,xymax,delta,x+112*2,y+16,32,16,7,false);
  inOut->h=decimal("h",inOut->h,xymin,xymax,delta,x+112*3,y+16,32,16,7,false);
  inOut->SetRect(inOut->x,inOut->y,inOut->w,inOut->h);
 }

 void FastGUI::xywh( Cartesiand *inOut, having differentRangeWH, int x, int y, double xymin, double xymax, double whmin, double whmax, const char *label ) {
  double delta=number_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=decimal("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=decimal("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,7,false);
  inOut->w=decimal("w",inOut->w,whmin,whmax,delta,x+112*2,y+16,32,16,7,false);
  inOut->h=decimal("h",inOut->h,whmin,whmax,delta,x+112*3,y+16,32,16,7,false);
  inOut->SetRect(inOut->x,inOut->y,inOut->w,inOut->h);
 }

 void FastGUI::xywh( Cartesiand *inOut, having scaleWH, int x, int y, double xymin, double xymax, double whmin, double whmax, double whDeltaFactor, const char *label ) {
  double delta=number_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=decimal("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=decimal("y",inOut->y,xymin,xymax,delta,x+128,y+16,32,16,7,false);
  inOut->w=decimal("w",inOut->w,whmin,whmax,delta*whDeltaFactor,x+128*2,y+16,32,16,7,false);
  inOut->h=decimal("h",inOut->h,whmin,whmax,delta*whDeltaFactor,x+128*3,y+16,32,16,7,false);
  inOut->SetRect(inOut->x,inOut->y,inOut->w,inOut->h);
 }

 void FastGUI::xyzw( Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=decimal("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=decimal("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,7,false);
  inOut->z=decimal("z",inOut->z,xymin,xymax,delta,x+112*2,y+16,32,16,7,false);
  inOut->w=decimal("w",inOut->w,xymin,xymax,delta,x+112*3,y+16,32,16,7,false);
 }

 void FastGUI::xyz( having preciseDecimal, Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta(0.1);
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=precise("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=precise("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,7,false);
  inOut->z=precise("z",inOut->z,xymin,xymax,delta,x+112*2,y+16,32,16,7,false);
 }

 void FastGUI::xywh( having preciseDecimal, Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta(0.1);
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=precise("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=precise("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,7,false);
  inOut->w=precise("w",inOut->w,xymin,xymax,delta,x+112*2,y+16,32,16,7,false);
  inOut->h=precise("h",inOut->h,xymin,xymax,delta,x+112*3,y+16,32,16,7,false);
  inOut->SetRect(inOut->x,inOut->y,inOut->w,inOut->h);
 }

 void FastGUI::xyzw( having preciseDecimal, Cartesiand *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  double delta=number_delta(0.1);
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=precise("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,7,false);
  inOut->y=precise("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,7,false);
  inOut->z=precise("z",inOut->z,xymin,xymax,delta,x+112*2,y+16,32,16,7,false);
  inOut->w=precise("w",inOut->w,xymin,xymax,delta,x+112*3,y+16,32,16,7,false);
 }

 void FastGUI::xywh( Cartesian *inOut, int x, int y, int xymin, int xymax, const char *label ) {
  int delta=(int) integer_delta();
  Text(label,(double)x,(double)y+10.0,5.0,7.0,false);
  inOut->x=integer("x",inOut->x,xymin,xymax,delta,x,y+16,32,16,5,false);
  inOut->y=integer("y",inOut->y,xymin,xymax,delta,x+112,y+16,32,16,5,false);
  inOut->w=integer("w",inOut->w,xymin,xymax,delta,x+112*2,y+16,32,16,5,false);
  inOut->h=integer("h",inOut->h,xymin,xymax,delta,x+112*3,y+16,32,16,5,false);
  inOut->SetRect(inOut->x,inOut->y,inOut->w,inOut->h);
 }

 void FastGUI::xywh( ScreenPosition *inOut, int x, int y, double xymin, double xymax, const char *label ) {
  Cartesiand c;
  inOut->Get(&c);
  xywh(&c,x,y,xymin,xymax,label);
  inOut->Set(&c);
 }
 
 /// angle modifier /////////////////////////////////////////////////////////////////////////////

 void FastGUI::xyAngle( double *xx, double *yy, int x, int y, int maxX, int maxY ) {
  int w=maxX*2;
  int h=maxY*2;
  GetState(x,y,w,h);
  Cartesiand mp(point.x-0.5,point.y-0.5);
  if ( hovering && clicking ) {
   (*xx)=mp.x*(double)maxX;
   (*yy)=mp.y*(double)maxY;
  }
  DrawLine2Color(hovering?excited:activated,hovering?activated:selected,
   (x+w/2),(y+h/2),
   (x+w/2)+(int)(*xx),
   (y+h/2)+(int)(*yy)
  );
  Text(FORMAT("%.2f",(*xx)).c_str(),
   (double)(x+w/2)+(*xx)+8.0, (double)(y+h/2)+(*yy),
   fontSize.x, fontSize.y, false
  );
  Text(FORMAT("%.2f",(*yy)).c_str(),
   (double)(x+w/2)+(*xx), (double)(y+h/2)+(*yy)+fontSize.y+4.0,
   fontSize.x, fontSize.y, false
  );
 }

 double FastGUI::angleDegrees( double in, int x, int y, int r ) {
  double result=in;
  GetState(x-r,y-r,r*2,r*2);
  (hovering?(clicking?excited:activated):tint).gl();
  if ( clicking ) {
   Line mouseLine;
   mouseLine.Set(0.5,0.5,mouse.x,mouse.y);
   result=rad2deg(mouseLine.LineAngle());
  }
  DrawCircle((double)x,(double)y,(double)r,16);
  DrawAngle(hovering?excited:highlight,x,y,(double)r,0.0,in,36.0);
  Text(FORMAT("%.2f",(double)in).c_str(),
   (double)(x+r), (double)(y+r)+fontSize.y+4.0,
   fontSize.x, fontSize.y, false
  );  
  double delta=number_delta(0.1);
  if ( button(leftArrow,(x+r),y-r,16,16,false) ) return in-delta;
  if ( button(rightArrow,(x+r+16),y-r,16,16,false) ) return in+delta;
  if ( button("O",(x+r),y,16,16,false) ) return 0.0;
  if ( button("@",(x+r+16),y,16,16,false) ) return in > 180.0 ? in-180.0 : in+180.0;
  return result;
 }
 
 /// film picker /////////////////////////////////////////////////////////////////////////////

 Film *FastGUI::FilmPicker( std::string *va, std::string *fi, double *instant, int *scrolled, int x, int y, int w, int h, const char *label ) {
  (*instant)+=FRAMETIME;
  Film *result=null;
  FilmLibrary *lib=null;
  Film *showing=null;
  if ( va->length() > 0 ) {
   lib=vault.find(va->c_str());
   if ( !lib ) (*va)=string("");
  }
  if ( va->length() == 0 || !lib ) { // We're showing vault selection
   Zdis<Strings> libraries;
   libraries.Recycle(vault.asStringList());
   int selected=lib?vault.IndexOf(lib):-1;
   int i=list(label,libraries.pointer,x,y,w/2-12,h/(((int)fontSize.y+7)),&selected,scrolled);
   if ( i >= 0 ) {
    (*va)=libraries->Get(i);
    (*scrolled)=0;
   }
  } else { // We're showing films in a vault
   Zdis<Strings> films;
   films.Recycle(lib->asFilmStringList());
   showing=lib->films.find(*fi);
   int selected=showing?lib->films.IndexOf(showing):-1;
   int wasSelected=selected;
   int i=list((string("In ")+lib->title).c_str(),films.pointer,x,y,w/2-12,h/(((int)fontSize.y+7)),&selected,scrolled);
   if ( i >= 0 ) {
    if ( i == wasSelected ) (*fi)=string("");
    else {
     (*fi)=films->Get(i);
    }
   }
  }
  Cartesian rect(x+(w/2),y,w/2,w/2);
  GetState(rect.x,rect.y,rect.w,rect.h);
  if ( showing && showing->frames.count > 0 ) {
   int frame=(int)((double)showing->frames.count*(*instant))%showing->frames.count;
   ImageHandle *ih=(ImageHandle *) showing->frames.Element(frame);
   if ( ih && ih->i ) image(ih->i,rect.x,rect.y,rect.w,rect.h,crayons.Pick(alabaster));
  } else {
   Area(filmPreviewBacking,&rect);
  }
  if ( button("<< Vaults",x+(w/2),y+w/2,w/2,(h-w/2)/2) ) {
   (*va)=string("");
   (*fi)=string("");
   (*scrolled)=0;
  }
  glLineWidth(2.0);
  Rectangle(hovering?selected:color,&rect);
  glLineWidth(1.0);
  result=vault.find((*va).c_str(),(*fi).c_str());
  return result;
 }

#if defined(NEVER)
 void Film1Picker( Zp<Film> *out, int *scrolled, int x, int y, int w=384, int h=256 ) {
  if ( !out ) return;
  int limitString=(w/2)/(charSpace+tfont.w)-1;
  Film *result=null;
  FilmLibrary *lib=null;
  Film *showing=null;
  static Zstring va;
  static Zstring fi;
  if ( out->pointer ) {
   lib=out->pointer->GetLibrary();
   if ( lib ) va=lib->title;
   fi=out->pointer->title;
  }
  if ( va.length == 0 || !lib ) { // We're showing vault selection
   Zdis<Strings> libraries;
   libraries.Recycle(vault.asStringList());
   int selected=lib?vault.IndexOf(lib):-1;
   int i=list(font,"Film Vaults",libraries.pointer,x,y,w/2-24,h/((tfont.h+6)),&selected,scrolled,lineSpace,charSpace,false,limitString);
   if ( i >= 0 ) {
    va=libraries->Get(i);
    (*scrolled)=0;
   }
  } else { // We're showing films in a vault
   Zdis<Strings> films;
   films.Recycle(lib->asFilmStringList());
   showing=lib->films.find(out->pointer->);
   int selected=showing?lib->films.IndexOf(showing):-1;
   int wasSelected=selected;
   int i=list(font,(string("In ")+lib->title).c_str(),films.pointer,x,y,w/2-24,h/((tfont.h+6)),&selected,scrolled,lineSpace,charSpace,false,limitString);
   if ( i >= 0 ) {
    if ( i == wasSelected ) fi=string("");
    else fi=films->Get(i);
   }
  }
 }
#endif
 
 Film *FastGUI::FilmPicker( std::string *va, std::string *fi, int *scrolled, int x, int y, int w, int h ) {
  return FilmPicker( termFont, va, fi, null, scrolled, x, y, w, h );
 }

 Film *FastGUI::FilmPicker( TexFont *font, std::string *va, std::string *fi, double *instant, int *scrolled, int x, int y, int w, int h, int linespace,int charspace,bool showrectangle ) {
  int limitString=(w/2)/(charspace+tfont.w)-1;
  if ( instant ) (*instant)+=FRAMETIME;
  Film *result=null;
  FilmLibrary *lib=null;
  Film *showing=null;
  if ( va->length() > 0 ) {
   lib=vault.find(va->c_str());
   if ( !lib ) (*va)=string("");
  }
  if ( va->length() == 0 || !lib ) { // We're showing vault selection
   Zdis<Strings> libraries;
   libraries.Recycle(vault.asStringList());
   int selected=lib?vault.IndexOf(lib):-1;
   int i=list(font,"Film Vaults",libraries.pointer,x,y,w/2-24,h/((tfont.h+6)),&selected,scrolled,linespace,charspace,showrectangle,limitString);
   if ( i >= 0 ) {
    (*va)=libraries->Get(i);
    (*scrolled)=0;
   }
  } else { // We're showing films in a vault
   Zdis<Strings> films;
   films.Recycle(lib->asFilmStringList());
   showing=lib->films.find(*fi);
   int selected=showing?lib->films.IndexOf(showing):-1;
   int wasSelected=selected;
   int i=list(font,(string("In ")+lib->title).c_str(),films.pointer,x,y,w/2-24,h/((tfont.h+6)),&selected,scrolled,linespace,charspace,showrectangle,limitString);
   if ( i >= 0 ) {
    if ( i == wasSelected ) (*fi)=string("");
    else (*fi)=films->Get(i);
   }
  }
  Cartesian rect(x+(w/2),y,w/2,w/2);
  GetState(rect.x,rect.y,rect.w,rect.h);
  if ( showing && showing->frames.count > 0 ) {
   int frame=(int)((double)showing->frames.count*(instant?(*instant):this->instant))%showing->frames.count;
   ImageHandle *ih=(ImageHandle *) showing->frames.Element(frame);
   if ( ih && ih->i ) image(ih->i,rect.x,rect.y,rect.w,rect.h,crayons.Pick(alabaster));
  } else {
   Area(crayons.Pick(black),&rect);
  }
  if ( button(font,"<< Vaults",x+(w/2),y+w/2,w/2,(h-w/2)/2) ) {
   (*va)=string("");
   (*fi)=string("");
   (*scrolled)=0;
  }
  glLineWidth(2.0);
  Rectangle(hovering?selected:color,&rect);
  glLineWidth(1.0);
  return showing;
 }
