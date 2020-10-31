#pragma once
#include "ListItem.h"
#include "Interface.h"
#include "Delay.h"

extern Interface input;

enum HIDControlOptions {
  hid_WhenPressed=0,
  hid_WhenDown=1,
  hid_WhenReleased=2,
  hid_HeldForHalfSec=3,
  hid_HeldForSec=4,
  hid_HeldFor1AndHalfSec=5,
  hid_HeldFor2Secs=6,
  hid_HeldFor3Secs=7
};

ONE(HIDControlMapping,{
 option=hid_WhenDown;
 xin_keys=xi_A;
 xin_buttons=pad_A;
})
 Zstring longname;
 HIDControlOptions option;
 Zbool keyboard,mouse,controller,logitech; // which input type
 Zbool gesture;
 Zint dx_key,dx_key2;  // DX_NONE is 0
 Zbool hold; // Must be held, not released (default trigger), set by game designer
 Zbool mouseGesture; //
 Zbool left,right,middle,wheelUp,wheelDown;
 Zdouble heldFor;
 Zbool detected;
 Delay lastDelay;
 Zbool delayBetweenUses;
 Zbool xinKey,xinButton;
 XInputDeviceKeys xin_keys;
 GamepadButtons xin_buttons;
 WORDKEYS({
 })
 KEYWORDS({
 })
 // This is used to "set" this mapping based on what's going on in the interface class.
 bool DetectForConfiguration() {
  bool continuing=true; // We just keep waiting until the user inputs something.
  ///
  // Let's do all of our detections here. 
  // Did we hit a key?
  // Did we use the mouse?
  // Did we use a controller button or d-pad?
  // Do we have a touch device?
  // Did we touch the screen?
  // Do we have the Leap?
  // Did we gesture with the Leap?
  // Do we have logitech?
  // Did we press a logitech extended button?
  ///
  if ( !continuing ) return true;
  return false; // we're going to keep searching (waiting for command)
 }
 void Between() {
  heldFor+=FRAMETIME;
  detected=Detected();
  if ( delayBetweenUses ) {
   if ( detected ) lastDelay.Reset();
   else if ( lastDelay ) lastDelay.Between();
  }
 }
 // Called each frame to probe the Interface class for whether or not this mapping is true.
 bool Detected() {
  detected=false;
  if ( !detected && keyboard ) {
   switch ( option ) {
    case hid_WhenPressed:
      detected= input.KeyPressed(dx_key) || input.KeyPressed(dx_key2) ? true : false;
     break;
    case hid_WhenDown:
      detected= input.KeyDown(dx_key) || input.KeyDown(dx_key2) ? true : false;
     break;
    case hid_WhenReleased:
      detected= input.KeyUp(dx_key) || input.KeyUp(dx_key2) ? true : false;
     break;
   }
  } 
  if ( !detected && mouse ) {
   if ( wheelUp && input.wheelUp ) detected= true;
   else if ( wheelDown && input.wheelDown ) detected= true;
   else switch ( option ) {
    case hid_WhenDown:
      if ( left && input.left ) detected= true;
      else if ( right && input.right ) detected= true;
      else if ( middle && input.middle ) detected= true;
     break;
    case hid_WhenReleased:
      if ( left && input.leftReleased() ) detected= true;
      else if ( right && input.rightReleased() ) detected= true;
      else if ( middle && input.middleReleased() ) detected= true;
     break;
   }
  } else if ( !detected && controller ) {
   switch ( option ) {
    case hid_WhenDown:
      detected= (xinKey && input.gamepads.Down(xin_keys)) || (xinButton && input.gamepads.Button(xin_buttons));
     break;
    case hid_WhenReleased:
      detected= (xinKey && input.gamepads.Released(xin_keys)) || (xinButton && input.gamepads.ButtonReleased(xin_buttons));
     break;
   }
  } else if ( !detected && logitech ) {
   switch ( option ) {
    case hid_WhenDown:
     break;
    case hid_WhenReleased:
     break;
   }
  }
  return detected;
 }
MANY(HIDControlMapping,HIDControlMappingHandle,HIDControlMappingHandles,"HIDControlMapping",HIDControlMappings,{ Defaults(); })
 HIDControlMapping *PadKeys(const char *named, XInputDeviceKeys dkey,  int key, int key2, HIDControlOptions opt=hid_WhenDown ) {
  HIDControlMapping *k=new HIDControlMapping;
  k->name=named;
  k->option=opt;
  k->xinKey=true;
  k->xin_keys=dkey;
  k->controller=true;
  k->dx_key=key;
  k->dx_key2=key2;
  k->keyboard=true;
  Append(k);
  return k;
 }
 HIDControlMapping *PadKey(const char *named, XInputDeviceKeys dkey,  int key, HIDControlOptions opt=hid_WhenDown ) {
  HIDControlMapping *k=new HIDControlMapping;
  k->name=named;
  k->option=opt;
  k->xinKey=true;
  k->xin_keys=dkey;
  k->controller=true;
  k->dx_key=key;
  k->keyboard=true;
  Append(k);
  return k;
 }
 HIDControlMapping *Pad(const char *named, XInputDeviceKeys key, HIDControlOptions opt=hid_WhenDown ) {
  HIDControlMapping *k=new HIDControlMapping;
  k->name=named;
  k->option=opt;
  k->xinKey=true;
  k->xin_keys=key;
  k->controller=true;
  Append(k);
  return k;
 }
 HIDControlMapping *Key(const char *named, int key, HIDControlOptions opt=hid_WhenDown ) {
  HIDControlMapping *k=new HIDControlMapping;
  k->name=named;
  k->option=opt;
  k->dx_key=key;
  k->keyboard=true;
  Append(k);
  return k;
 }
 HIDControlMapping *Mouse(const char *named, HIDControlOptions opt=hid_WhenDown ) {
  HIDControlMapping *k=new HIDControlMapping;
  k->name=named;
  k->option=opt;
  k->mouse=true;
  Append(k);
  return k;
 }
 void Defaults() { // Set up the default control schemes.
  HIDControlMapping *c=null;
  ///...
  c=PadKeys("L",xi_Left,DX_A,DX_LEFT); c->longname="Move Left";
  c=PadKeys("R",xi_Right,DX_D,DX_RIGHT); c->longname="Move Right";
  c=PadKeys("U",xi_Up,DX_W,DX_UP); c->longname="Move Forward/Up";
  c=PadKeys("D",xi_Down,DX_S,DX_DOWN); c->longname="Move Back/Down";
  c=PadKey("L2",xi_LThumb_Left,DX_NUMPAD_LEFT); c->longname="Fly Left";
  c=PadKey("R2",xi_LThumb_Right,DX_NUMPAD_RIGHT); c->longname="Fly Right";
  c=PadKey("U2",xi_LThumb_Up,DX_NUMPAD_UP); c->longname="Fly Forward/Up";
  c=PadKey("D2",xi_LThumb_Down,DX_NUMPAD_DOWN); c->longname="Fly Back/Down";
  c=PadKey("LL",xi_RThumb_L,DX_J); c->longname="Look Left";
  c=PadKey("LR",xi_RThumb_R,DX_L); c->longname="Look Right";
  c=PadKey("LU",xi_RThumb_U,DX_I); c->longname="Look Forward/Up";
  c=PadKey("LD",xi_RThumb_D,DX_K); c->longname="Look Back/Down";
  c=PadKey("Walk",xi_RShoulder,DX_LCTRL); c->longname="Walk";
  c=PadKey("Cru",xi_LThumb,DX_C); c->longname="Cruise Toggle"; c->option=hid_WhenReleased;
  c=PadKey("After",xi_LShoulder,DX_LSHIFT); c->longname="Afterburn / Sprint";
  c=PadKey("Fi1",xi_A,DX_LCTRL,   hid_WhenPressed); c->longname="Fire Primary";
  /* Mouse */ c->mouse=true; c->left=true;
  c=PadKey("Fi2",xi_B,DX_LALT,    hid_WhenPressed); c->longname="Fire Secondary";
  /* Mouse */ c->mouse=true; c->right=true;
  c=PadKey("NW",xi_RShoulder,DX_RBRACKET, hid_WhenPressed); c->longname="Next Primary Weapon";
  /* Mouse */ c->mouse=true; c->wheelUp=true;
  c=PadKey("PW",xi_LShoulder,DX_LBRACKET, hid_WhenPressed); c->longname="Prev Primary Weapon";
  /* Mouse */ c->mouse=true; c->wheelDown=true;
  c=PadKey("NW2",xi_LThumb_Left,DX_PERIOD,  hid_WhenPressed); c->longname="Next Secondary Weapon";
  /* Mouse */ c->mouse=true; c->middle=true;
  c=PadKey("PW2",xi_LThumb_Right,DX_COMMA,   hid_WhenPressed); c->longname="Prev Secondary Weapon";
  c=PadKey("Bk",xi_Back,DX_ESCAPE,hid_WhenPressed); c->longname="Back / Cancel";
 }
 HIDControlMappingHandle *Detected( const char *name ) {
  EACH(detections.first,HIDControlMappingHandle,h) if ( h->p->name == name ) return h;
  return null;
 }
 bool did( const char *query ) {
  HIDControlMapping *mapping=firstNamed(query);
  if ( mapping->Detected() ) return true;
  return false;
 }
 HIDControlMappingHandles detections;
 CALLEACH1(HIDControlMapping,Between,{GetDetected(&detections);});
 void GetDetected( HIDControlMappingHandles *out ) {
  out->Clear();
  FOREACH(HIDControlMapping,hcm) if ( hcm->detected ) out->Add(hcm);
 }
DONE(HIDControlMapping);

extern HIDControlMappings controls;