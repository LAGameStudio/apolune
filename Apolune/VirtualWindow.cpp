#include "VirtualWindow.h"

#include "EditGUI.h"

VirtualWindowStyle virtualStyleDefault;

VirtualWindowSettingsHeap virtualWindowSettingsHeap;

void VirtualWindow::SizeAndNear( double sw, double sh, VirtualWindow *vw ) {
 Cartesiand bonded(&vw->position);
 Cartesiand rect(0.0,0.0,sw,sh);
 rect.MoveBy(bonded.x2+4,bonded.y); // Start at top right corner of bonded window.
 int retries=64;
 while ( retries>0 ) {
  retries--;
  // Starting at top right, walk down the right side of the window until we're in the bottom right
  rect.MoveTo(bonded.x2+32,bonded.y);
  while ( rect.y < bonded.y-sw+32 ) {
   rect.MoveBy(0,32);
   if ( holder->WindowCanBePlacedHere(rect.x,rect.y,rect.w,rect.h) ) {
    SizeAndPosition(rect.x,rect.y,rect.w,rect.h);
    return;
   }
  }
  // Now walk left until we've reached bonded.x-sw
  rect.MoveTo(bonded.x2+32,bonded.y2+32);
  while ( rect.x > bonded.x-sw-32 ) {
   rect.MoveBy(-32,0);
   if ( holder->WindowCanBePlacedHere(rect.x,rect.y,rect.w,rect.h) ) {
    SizeAndPosition(rect.x,rect.y,rect.w,rect.h);
    return;
   }
  }
  // Now walk up until we've reached bonded.y-sh-4-vw->titleBar.h
  rect.MoveTo(bonded.x-rect.w-32,bonded.y2+32);
  while ( rect.y > bonded.y-sh-32 ) {
   rect.MoveBy(0,-32);
   if ( holder->WindowCanBePlacedHere(rect.x,rect.y,rect.w,rect.h) ) {
    SizeAndPosition(rect.x,rect.y,rect.w,rect.h);
    return;
   }
  }
  // Now walk right until we've reached the position right above our first position (x aligns).
  rect.MoveTo(bonded.x-rect.w-32,bonded.y-rect.h-32);
  while ( rect.x < bonded.x2+32 ) {
   rect.MoveBy(32,0);
   if ( holder->WindowCanBePlacedHere(rect.x,rect.y,rect.w,rect.h) ) {
    SizeAndPosition(rect.x,rect.y,rect.w,rect.h);
    return;
   }
  }
  // If we reached this point, we've got to retry, so expand the size of the central,
  // bonded rectangle and repeat the process.
  bonded.SetRect(bonded.x-32,bonded.y-32,bonded.w+64,bonded.h+64);
 }
 // Well, we gave up so let's just set the size and position staggered (cascaded) off the top right side of the window.
 SizeAndPosition(bonded.x2+4,bonded.y+holder->Attached(vw)*32,sw,sh);
}

void VirtualGUI::Add( MoveableEditorWindow *w ) {
 Add(&w->v);
 if ( proce55ors && !proce55ors->ListIteminList((ListItem *)w) )
  proce55ors->Add(w,glWindow);
}

void VirtualGUI::Remove( MoveableEditorWindow *w, bool keep ) {
 if ( windows.ListIteminList(&w->v) ) {
  Remove(&w->v);
 }
 if ( proce55ors && proce55ors->ListIteminList((ListItem *)w) )
  keep ? proce55ors->Remove(w) : proce55ors->Drop(w);
}

void VirtualGUI::Reorder( Proce55ors *in_out ) {
 EACH(windows.first,VirtualWindowHandle,vwh) {
  if ( in_out->ListIteminList(vwh->p->encapsulated) ) {
   in_out->Remove(vwh->p->encapsulated);
  }
  // By keeping the Append() here outside of above, you can save a step and add the window automatically when created.  Useful for helping the Attach() call.
  in_out->Append(vwh->p->encapsulated);
 }
}

#include "GridSplitter.h"

void VirtualGUI::Arrange( GridRects *in, int margin ) {
 if ( in->count < windows.count ) {
 } else {
  VirtualWindowHandle *win=(VirtualWindowHandle *) windows.first;
  EACH(in->first,GridRect,gr) {
   while ( win && ( win->p->noMove || win->p->noResize ) ) win=(VirtualWindowHandle *) win->next;
   if ( win ) {
    win->p->SizeAndPosition( gr->X(), gr->Y(), (gr->W()-margin), (gr->H()-margin) );
    win=(VirtualWindowHandle *) win->next;
   } else break;
  }
 }
 arranging=true;
}

void VirtualWindow::Unminimize() {
 if ( holder ) holder->Unminimize(this);
}

void VirtualWindow::Between() {
 wasMoving=moving;
 wasResizing=resizing;
 wasDragging=dragging;
 wasPosition.Set(&position);
 maximizing=false;
 minimizing=false;
 moving=false;
 resizing=false;
 dragging=false;
 if ( attachedTo && attachedTo->isMinimized && !isMinimized ) {
  minimizing=true;
 }
 oldMouseScreen.Set(mouseScreen.x,mouseScreen.y);
 mouseScreen.Set((int)((double)input.mx*vdisplay.x),(int)((double)input.my*vdisplay.y));
 mouse.Set(mouseScreen.x-interior.x,mouseScreen.y-interior.y);
 mouseRatio.Set(mouse.x/interior.w,mouse.y/interior.h);
 if ( holder->focused == this ) {
  within=position.within(mouseScreen.x,mouseScreen.y);
  withinResize=b_resize.within(mouseScreen.x,mouseScreen.y);
  withinClose=b_close.within(mouseScreen.x,mouseScreen.y);
  withinCenter=b_center.within(mouseScreen.x,mouseScreen.y);
  withinReturnTo=b_returnTo.within(mouseScreen.x,mouseScreen.y);
  withinTitlebar=titlebar.within(mouseScreen.x,mouseScreen.y);
  withinMinimize=b_minimize.within(mouseScreen.x,mouseScreen.y);
  withinMaximize=b_maximize.within(mouseScreen.x,mouseScreen.y);
  inside=interior.within(mouseScreen.x,mouseScreen.y);
  inside &= within && !withinResize && !withinClose && !withinTitlebar && !withinMaximize && !withinMinimize;
 } else {
  within=false;
  withinResize=false;
  withinClose=false;
  withinCenter=false;
  withinReturnTo=false;
  withinTitlebar=false;
  withinMinimize=false;
  withinMaximize=false;
  inside=false;
 }
 leftReleased=input.leftReleased();
 rightReleased=input.rightReleased();
 released=(leftReleased || rightReleased);
 wasClicking=clicking;
 clicking=(input.left || input.right || input.touches.count > 0 );
 leftClicking=(!leftReleased && (input.left > 0));
 rightClicking=(!rightReleased && (input.right > 0));
 if ( holder->focused == this && clicking ) {
  if ( wasClicking && wasDragging && (wasMoving || wasResizing) ) {
   dragging=true;
   if ( !noMove && wasMoving ) {
    moving=true;
    moved.Set(mouseScreen.x-oldMouseScreen.x,mouseScreen.y-oldMouseScreen.y);
    position.SetRect(position.x+moved.x,position.y+moved.y,position.w,position.h);
    Calculate();
   } else if ( !noResize && wasResizing ) {
    resizing=true; // Signal
    moved.Set(mouseScreen.x-oldMouseScreen.x,mouseScreen.y-oldMouseScreen.y);
    if ( resizeVerticalOnly )
         position.SetRect(position.x,position.y,(position.w),(position.h+moved.y));
    else if ( resizeHorizontalOnly )
         position.SetRect(position.x,position.y,(position.w+moved.x),(position.h));
    else position.SetRect(position.x,position.y,(position.w+moved.x),(position.h+moved.y));
    if ( position.w < minimumSize.x ) position.SetRect(position.x,position.y,minimumSize.x,position.h);
    if ( position.h < minimumSize.y ) position.SetRect(position.x,position.y,position.w,minimumSize.y);
    if ( position.w > maximumSize.x ) position.SetRect(position.x,position.y,maximumSize.x,position.h);
    if ( position.h > maximumSize.y ) position.SetRect(position.x,position.y,position.w,maximumSize.y);
    Calculate();
   }
  } else if ( !noResize && withinResize ) {
   dragging=true; // Signal
   resizing=true; // Signal
   moved.Set(mouseScreen.x-oldMouseScreen.x,mouseScreen.y-oldMouseScreen.y);
   if ( resizeVerticalOnly )
        position.SetRect(position.x,position.y,(position.w),(position.h+moved.y));
   else if ( resizeHorizontalOnly )
        position.SetRect(position.x,position.y,(position.w+moved.x),(position.h));
   else position.SetRect(position.x,position.y,(position.w+moved.x),(position.h+moved.y));
   Calculate();
  } else if ( !noMove && withinTitlebar ) {
   dragging=true; // Signal
   moving=true; // Signal
   moved.Set(mouseScreen.x-oldMouseScreen.x,mouseScreen.y-oldMouseScreen.y);
   position.SetRect(position.x+moved.x,position.y+moved.y,position.w,position.h);
   if ( position.w < minimumSize.x ) position.SetRect(position.x,position.y,minimumSize.x,position.h);
   if ( position.h < minimumSize.y ) position.SetRect(position.x,position.y,position.w,minimumSize.y);
   if ( position.w > maximumSize.x ) position.SetRect(position.x,position.y,maximumSize.x,position.h);
   if ( position.h > maximumSize.y ) position.SetRect(position.x,position.y,position.w,maximumSize.y);
   Calculate();
  }
 } else if ( holder->focused == this && released ) {
  if ( withinReturnTo && attachedTo ) {
   holder->SlideTo(attachedTo);
  } else
  if ( !noClose && withinClose ) {
   closing=true; // Signal
  } else if ( (!noMinimize && withinMinimize) ) {
   minimizing=true; // Signal
  } else if ( !noMaximize && withinMaximize ) {
   maximizing=true; // Signal
  } else if ( !noCenter && withinCenter ) {
   holder->SlideTo(this);
  }
 }
 wasCreated=false;
}

#include "EditGUI.h"

void VirtualGUI::HandleNavWindow() {
 if ( windows.count == 0 ) return; // Can't create, no need.
 if ( !proce55ors ) return; // Can't create the window.
 EACHN(windows.first,VirtualWindowHandle,vwh,{
  if ( vwh->p->title == "Window Navigator" ) {
   vwh->p->closing=true;
   return;
  }
 });
 VirtualWindowNavigator *nav=new VirtualWindowNavigator;
 Add(nav);
 nav->Setup();
}

void VirtualWindow::RenderBack() {
  glLineWidth(1.0f);
  Blending(none);
  if ( attachedTo && alwaysOnTop ) {
   Rectangle(crayons.Pick(alabaster),
    attachedTo->interior.x,attachedTo->interior.y,attachedTo->interior.x2,attachedTo->interior.y2
   );
   Line line(
    interior.CenterX(),interior.CenterY(),
    ((double)target.x+attachedTo->interior.x),
    ((double)target.y+attachedTo->interior.y)
   );
   if ( !position.within(line.x2,line.y2) ) {
    Box us(&position);
    BoxCollisionResults boxline;
    line.Collides(&us,&boxline);
    Cartesiand start(boxline.x,boxline.y);
    Circle them(line.x2,line.y2,attachmentRadius!=-1.0?attachmentRadius:style->attachmentCircleSize);
    Vertexd rim;
    double angle=line.LineAngle();
    them.Point(((angle+PI)/TWO_PI),&rim);
    glLineWidth((float)style->attachmentCircleThickness);
    style->attachmentColor.gl();
    DrawCircle(them.x,them.y,them.R,8);
    glLineWidth((float)style->attachmentLineThickness);
    DrawLine2Color(style->windowEdge,style->attachmentColor,start.x,start.y,rim.x,rim.y);
    glLineWidth(1.0f);
   }
//   DrawLine2Color(crayons.Pick(red255),crayons.Pick(blue255),line.x,line.y,line.x2,line.y2);
  }
  if ( !noBacking ) {
   if ( noTitleBar || (noMove && noTitle) ) Area(style->windowBacking,(int)interior.x,(int)(interior.y),(int)interior.x2,(int)(interior.y2));
   else Area(style->windowBacking,(int)position.x,(int)position.y,(int)position.x2,(int)position.y2);
  }
  if ( !noTitle ) {
   Area(
    (withinTitlebar&&!noMove)?(clicking?style->buttonSelected:style->buttonHover):style->windowTitle,
    (int)titlebar.x,(int)titlebar.y,(int)titlebar.x2,(int)titlebar.y2
   );
   if ( !noClipTitle ) scissors.Push((int)titlebar.x,(int)titlebar.y,(int)titlebar.w,(int)titlebar.h);
   WriteTexFont(style->windowTitleFont,style->windowTitleText,style->windowTitleFontBlend,
    (int)(titlebar.x+style->firstTextLineOffset.x),
    (int)(titlebar.y+style->firstTextLineOffset.y), //+style->smallText.y),
    (int)style->smallText.x,(int)style->smallText.y,
    4,title.c_str(),style->windowTitleKern
   );
   if ( !noClipTitle ) scissors.Pop();
  } else if ( !noTitleBar ) DrawSmallestButton(">",titlebar.x,titlebar.y,withinTitlebar);
  if ( !noEdge ) {
   if ( noTitleBar ) Rectangle(style->windowEdge,(int)interior.x,(int)interior.y,(int)interior.x2,(int)interior.y2);
   else Rectangle(style->windowEdge,(int)edge.x,(int)edge.y,(int)edge.x2,(int)edge.y2);
  }
  if ( !noBackground ) {
   Blending(transparency);
   Area(background,(int)interior.x,(int)interior.y,(int)interior.x2-1,(int)interior.y2);
  }
  if ( attachedTo ) {
   DrawSmallestButton("^",b_returnTo.x,b_returnTo.y,withinReturnTo);
  }
  if ( !noCenter ) {
   DrawSmallestButton("=",b_center.x,b_center.y,withinCenter);
  }
  if ( !noClose ) {
   DrawSmallestButton("X",b_close.x,b_close.y,withinClose);
  }
  if ( !noMinimize ) {
   DrawSmallestButton("_",b_minimize.x,b_minimize.y,withinMinimize);
   if ( withinMinimize && released ) {
    isMinimized=true;
    minimized.x=(double)titleTextWidth+style->firstTextLineOffset.x*4.0;
   }
  }
  if ( !noMaximize ) {
   wasMaximized=maximizing;
   DrawSmallestButton("O",b_maximize.x,b_maximize.y,withinMaximize);
   if ( withinMaximize && released ) {
    maximizeClicks++;
    if ( maximizeClicks % 2 == 1 ) {
     maximizedMemory.Set(&position);
     Resize(maximumSize.x,maximumSize.y);
    } else {
     Resize(maximizedMemory.w,maximizedMemory.h);
    }
    maximizing=true;
   }
  }
 }