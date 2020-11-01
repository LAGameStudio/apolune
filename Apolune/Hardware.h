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
#if defined(USE_GLFW)

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#undef APIENTRY

#include "ZeroTypes.h"
#include "ZIndexed.h"
#include "LinkedList.h"
#include "Strings.h"
#include "Cartesian.h"

namespace LA {

 // Callbacks assigned to various GLFW events.
void glfw_callback_error(int error, const char* description);
void glfw_callback_drop(GLFWwindow* window, int count, const char** paths);
void glfw_callback_monitor(GLFWmonitor* monitor, int event);
void glfw_callback_joystick(int joy, int event);

void glfw_callback_mouseenterleave(GLFWwindow* window, int entered);
void glfw_callback_mousemove(GLFWwindow* window, double xpos, double ypos);
void glfw_callback_mousebutton(GLFWwindow* window, int button, int action, int mods);
void glfw_callback_scrollwheel(GLFWwindow* window, double xoffset, double yoffset);

void glfw_callback_key();
void glfw_callback_text();
void glfw_callback_char_mods();

void glfw_callback_window_focusblur(GLFWwindow *window, int action);
void glfw_callback_window_iconify(GLFWwindow *window, int action);
void glfw_callback_window_refresh(GLFWwindow *window);
void glfw_callback_window_position(GLFWwindow *window, int newx, int newy);
void glfw_callback_window_size(GLFWwindow *window, int neww, int newh);
void glfw_callback_window_buffer_size(GLFWwindow *window, int neww, int newh);
void glfw_callback_window_close(GLFWwindow *window);
// Abstracts the hardware layer to implement GLFW instead of WinAPI.  Everything here must be done on the main thread pretty much.  See GLFW.org for details.

class HardwareSettings {
public:
 HardwareSettings() {
 }
 Zbool handleJoystick, handleKeyboard, handleMouse;
 
 void DebugInfo() {
  OUTPUT("HardwareSettings: -- DebugInfo (Start)\n");
  if ( handleJoystick ) OUTPUT("Handling Joysticks.\n"); else  OUTPUT("NOT handling Joysticks.\n"); 
  if ( handleKeyboard ) OUTPUT("Handling Keyboard.\n");  else  OUTPUT("NOT handling Keyboard.\n");  
  if ( handleMouse )    OUTPUT("Handling Mouse.\n");     else  OUTPUT("NOT handling Mouse.\n");     
  OUTPUT("HArdwareSettings: -- DebugInfo (End)\n");
 }
};

// Input: Joystick

ONE(JoystickAxis,{})
 Zdouble value;
MANY(JoystickAxis,JoystickAxisHandle,JoystickAxisHandles,"JoystickAxis",JoystickAxes,{})
 void Update( int joy_id ) {
  int counter=0;
  const float *data = glfwGetJoystickAxes(joy_id,&counter);
  if ( counter != count ) {
   while ( counter < count ) Append(new JoystickAxis);
   while ( counter > count && last ) {
    JoystickAxis *a = (JoystickAxis *)last;
    Remove(last);
    delete last;
   }
  }
  JoystickAxis *a=(JoystickAxis *) first;
  for ( int i=0; i<counter; i++ ) {
   a->value = (double) data[i];
   a=(JoystickAxis *) a->next;
  }
 }
DONE(JoystickAxis);


ONE(JoystickButton,{})
 Zdouble value;
MANY(JoystickButton,JoystickButtonHandle,JoystickButtonHandles,"JoystickButton",JoystickButtons,{})
 void Update( int joy_id ) {
  int counter=0;
  const unsigned char *data = glfwGetJoystickButtons(joy_id,&counter);
  if ( counter != count ) {
   while ( counter < count ) Append(new JoystickButton);
   while ( counter > count && last ) {
    JoystickButton *a = (JoystickButton *)last;
    Remove(last);
    delete last;
   }
  }
  JoystickButton *a=(JoystickButton *) first;
  for ( int i=0; i<counter; i++ ) {
   a->value = (double) data[i];
   a=(JoystickButton *) a->next;
  }
 }
DONE(JoystickButton);

ONE(Joystick,{})
 int id;
 Zbool connected;
 JoystickAxes axes;
 JoystickButtons buttons;
 void Set( int id ) {
  this->id=id;
  name = glfwGetJoystickName(id);
  this->connected = glfwJoystickPresent(id);
 }
 void Update() {
  if ( !connected ) return;
  axes.Update(id);
  buttons.Update(id);
 }
 void DebugInfo( int count ) {
  OUTPUT("Joystick #%d --------------\n", (int)count);
  OUTPUT("Name: %s\n",name.c_str());
  OUTPUT("Connected: %s\n", connected?"Yes":"No");
  OUTPUT("Axes: %3d   Buttons: %3d\n", (int) axes.count, (int)buttons.count );
 }
MANY(Joystick,JoystickHandle,JoystickHandles,"Joystick",Joysticks,{
 device.Size(GLFW_JOYSTICK_LAST+1);
 for ( int i=0; i<=GLFW_JOYSTICK_LAST; i++ ) {
  Joystick *j=new Joystick;
  device[i] = j;
  Append(device[i]);
 }
})
 void DebugInfo() {
  OUTPUT("GLFW: Joysticks\n");
  int i=0; FOREACH(Joystick,j) { j->DebugInfo(i); i++; }
 }
 Zbool initialized;
 ZIndexed<Zp<Joystick>> device;
 void Init() {
  int i=0;
  FOREACH(Joystick,j) { j->Set(i); i++; }
  initialized=true;
 }
 void Update() {
  FOREACH(Joystick,j) { j->Update(); }
 }
DONE(Joystick);

// Input: Mouse

enum MouseCursorModes {
 mc_Normal,
 mc_Hidden,
 mc_Disabled
};

class Window;
class CPUImage;
ONE(MouseCursor,{})
 Cartesian hotspot;
 Zp<GLFWcursor> cursor;
 Zbool standard;
 void Set(Window *in);
 void Create(CPUImage *image);
MANY(MouseCursor,MouseCursorHandle,MouseCursorHandles,"MouseCursor",MouseCursors,{ mode = mc_Normal; })
 MouseCursorModes mode;
 void Init() {
  MouseCursor *mc=new MouseCursor;
  mc->cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  mc->standard = true;
  mc->name = "arrow";
  Append(mc);
  mc=new MouseCursor;
  mc->cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
  mc->standard = true;
  mc->name = "beam";
  Append(mc);
  mc=new MouseCursor;
  mc->cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
  mc->standard = true;
  mc->name = "crosshair";
  Append(mc);
  mc=new MouseCursor;
  mc->cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
  mc->standard = true;
  mc->name = "hand";
  Append(mc);
  mc=new MouseCursor;
  mc->cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  mc->standard = true;
  mc->name = "horizontal";
  Append(mc);
  mc=new MouseCursor;
  mc->cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
  mc->standard = true;
  mc->name = "vertical";
  Append(mc);
 }
 void Hide() {
 }
 void Set(Window *in, const char *name );
 void Add(const char *name, CPUImage *in, int xhot=0, int yhot=0);
DONE(MouseCursor);

ONE(Mouse,{ mode = mc_Normal; })
 MouseCursorModes mode;
 Zbool left,right,middle;
 Zbool four,five,six,seven,eight;
 Zint buttons;
 MouseCursors cursors;
 Zbool initialized;
 Zbool shift,control,alt,super;
 Cartesiand position;
 void Init( GLFWwindow *window ) {
  cursors.Init();
  initialized=true;
 }
 void MouseMoved( double x, double y ) {
  position.Set(x,y);
 }
 void ButtonChange( int button, bool pressed, int modifiers ) {}
MANY(Mouse,MouseHandle,MouseHandles,"Mouse",Mice,{})
DONE(Mouse);

// Input: Keyboard

ONE(Keyboard,{})
 Zbool initialized;
 void Init( GLFWwindow *window ) {
  initialized=true;
 }
 void DebugInfo( int count ) {
  OUTPUT("Keyboard #%d -----------\n", (int)count);
  OUTPUT("Name: %s\n",name.c_str());
  OUTPUT("Connected: %s\n", initialized?"Yes":"No");
 }
MANY(Keyboard,KeyboardHandle,KeyboardHandles,"Keyboard",Keyboards,{})
 void DebugInfo() {
  OUTPUT("GLFW: Keyboards\n");
  int i=0; FOREACH(Keyboard,k) { k->DebugInfo(i); i++; }
 }
DONE(Keyboard);


// Display device detection

ONE(VideoMode,{})
 Zint red,green,blue;  // Bits
 Cartesian size;
 Zint rate;  // Refresh rate
 void Set(const GLFWvidmode *in) {
  red = in->redBits;
  green = in->greenBits;
  blue = in->blueBits;
  size.SetRect(in->width,in->height,in->width,in->height);
  name = FORMAT("%dx%d",(int)size.x,(int)size.y);
 }
MANY(VideoMode,VideoModeHandle,VideoModeHandles,"VideoMode",VideoModes,{})
 VideoMode *Add(const GLFWvidmode *in) {
  VideoMode *vm=new VideoMode;
  vm->Set(in);
  Append(vm);
  return vm;
 }
DONE(VideoMode);

ONE(Monitor,{
 red.Size(256);
 green.Size(256);
 blue.Size(256);
})
 ZIndexed<unsigned short> red,green,blue; // Gamma Ramps that you're going to be setting
 ZIndexed<unsigned short> orig_red,orig_green,orig_blue; // Gamma Ramps from the original settings
 Zbool connected,primary;
 VideoModes modes;
 Cartesian position,millimetres,resolution;
 Zp<GLFWmonitor> monitor;
 void Set(GLFWmonitor *in) { 
  monitor=in;
  name = glfwGetMonitorName(in);
  int counter=0;
  modes.Clear();
  const GLFWvidmode *v=glfwGetVideoModes(in,&counter);
  if ( v ) { for ( int i=0; i<counter; i++ ) modes.Add(&(v[i])); }
  else OUTPUT("GLFW: Error getting video modes for Monitor: %s.\n",name.c_str());
  const GLFWgammaramp *ramp = glfwGetGammaRamp(in);
  orig_red.Size(ramp->size);
  orig_green.Size(ramp->size);
  orig_blue.Size(ramp->size);
  for ( int i=0; i<(int)ramp->size; i++ ){
   orig_red[i] = ramp->red[i];
   orig_green[i] = ramp->green[i];
   orig_blue[i] = ramp->blue[i];
  }
  int x=0,y=0;
  glfwGetMonitorPhysicalSize(in,&x,&y);
  millimetres.Set(x,y); // Windows: calculates the returned physical size from the current resolution and system DPI instead of querying the monitor EDID data.
  glfwGetMonitorPos(in,&x,&y);
  v = glfwGetVideoMode(in);
  if ( v ) resolution.SetRect(v->width,v->height,v->width,v->height);
  else OUTPUT("GLFW: Error getting current mode for Monitor: %s.\n",name.c_str());
  position.SetRect(x,y,resolution.x,resolution.y);
  connected=true;
 }
 void SetGamma( double g ) {
  if ( g <= 0.0 ) g=EPSILON;
  glfwSetGamma(monitor,(float)g);
 }
 // Windows: The gamma ramp size must be 256.
 void SetGammaRamp(){
  GLFWgammaramp ramp;
  ramp.size=256;
  ramp.red = red.list;
  ramp.green = green.list;
  ramp.blue = blue.list;
  glfwSetGammaRamp(monitor,&ramp);
 }
 // Windows: The gamma ramp size must be 256.
 void RevertGammaRamp() {
  GLFWgammaramp ramp;
  ramp.size=(unsigned int) orig_red.length;
  ramp.red = orig_red.list;
  ramp.green = orig_green.list;
  ramp.blue = orig_blue.list;
  glfwSetGammaRamp(monitor,&ramp);
 }
 void DebugInfo( int count ) {
  OUTPUT("Monitor #%d ----------------\n", (int)count);
  OUTPUT("Name: %s\n",name.c_str());
  OUTPUT("Connected: %s\n", connected?"Yes":"No");
  OUTPUT("Primary? %s\n", primary?"Yes":"No");
  OUTPUT("Video Modes: %d\n", (int)modes.count);
  OUTPUT("Position: %s\n", position.toString().c_str());
  OUTPUT("Size: %s (millimetres)\n", millimetres.toString().c_str());
  OUTPUT("Resolution: %s\n", resolution.toString().c_str());
 }
MANY(Monitor,MonitorHandle,MonitorHandles,"Monitor",Monitors,{})
 void DebugInfo() {
  OUTPUT("GLFW: Monitors\n");
  int i=0; FOREACH(Keyboard,k) { k->DebugInfo(i); i++; }
 }
 Zp<Monitor> primary;
 void Init() {
  Clear();
  int counter=0;
  GLFWmonitor **monitors=glfwGetMonitors(&counter);
  for ( int i=0; i<counter; i++ ) {
   Monitor *m=new Monitor;
   if ( i==0 ) m->primary=true;
   m->Set(monitors[i]);
  }
 }
 Monitor *Find( GLFWmonitor *by ) {
  FOREACH(Monitor,m) if ( m->monitor == by ) return m;
  return null;
 }
 Monitor *Find( int resx, int resy ) {
  FOREACH(Monitor,m) if ( m->resolution.x == resx && m->resolution.y == resy ) return m;
  return null;
 }
DONE(Monitor);

// Types of events handled through callbacks and queued in the Hardware Event Queue

enum WindowEventTypes {
 we_Path_Drop,
 we_Mouse,
 we_Key,
 we_Text,
 we_Joystick
};

ONE(WindowEvent,{})
 Zstring data;
MANY(WindowEvent,WindowEventHandle,WindowEventHandles,"WindowEvent",WindowEvents,{})
DONE(WindowEvent);


// Hardware/window settings loaded from a file

ONE(WindowSettings,{
 mouseInput=true;
 joystickInput=true;
 keyboardInput=true;
 acceptDroppedFiles=true;
 startfullscreen=true;
 resizable=true;
 visible=true;
 decorated=true;
 focused=true;
 autoiconify=true;
 floating=false;
 maximized=false;
 red=8;
 green=8;
 blue=8;
 alpha=8;
 depth=24;
 stencil=8;
 samples=GLFW_DONT_CARE;
 refreshrate=GLFW_DONT_CARE;
 stereo=false;
 srgbcapable=false;
 doublebuffer=true;
 clientapi=GLFW_OPENGL_API;
 contextcreationapi = GLFW_NATIVE_CONTEXT_API;
 major = 1.0;
 minor = 0.0;
 contextrobustness = GLFW_NO_ROBUSTNESS;
 contextreleasebehavior = GLFW_ANY_RELEASE_BEHAVIOR;
 forwardcompatible=false;
 debugcontext=false;
 openglprofile=GLFW_OPENGL_ANY_PROFILE;
})
 Zp<HardwareSettings> hardware;
 Zbool mouseInput;
 Zbool joystickInput;
 Zbool keyboardInput;
 Zbool acceptDroppedFiles;
 Zbool startfullscreen,resizable,visible,decorated,focused,autoiconify,floating,maximized;
 Zbool stereo,srgbcapable,doublebuffer;
 Zint clientapi,contextcreationapi,major,minor;
 Zbool forwardcompatible,debugcontext,openglprofile;
 Zint red,green,blue,alpha,depth,stencil,samples,refreshrate;
 Zint contextrobustness, contextreleasebehavior;
 void Set(HardwareSettings *in) {
  hardware=in;
  if ( !in->handleJoystick ) joystickInput=false;
  if ( !in->handleKeyboard ) keyboardInput=false;
  if ( !in->handleMouse ) mouseInput=false;
 }
 void Load(const char *configfile) {
 }
 void Set(WindowSettings *other) {
  mouseInput             = other->mouseInput;
  joystickInput          = other->joystickInput;
  keyboardInput          = other->keyboardInput;
  acceptDroppedFiles     = other->acceptDroppedFiles;
  startfullscreen        = other->startfullscreen;
  resizable              = other->resizable;
  visible                = other->visible;
  decorated              = other->decorated;
  focused                = other->focused;
  autoiconify            = other->autoiconify;
  floating               = other->floating;
  maximized              = other->maximized;
  stereo                 = other->stereo;
  srgbcapable            = other->srgbcapable;
  doublebuffer           = other->doublebuffer;
  clientapi              = other->clientapi;
  contextcreationapi     = other->contextcreationapi;
  major                  = other->major;
  minor                  = other->minor;
  forwardcompatible      = other->forwardcompatible;
  debugcontext           = other->debugcontext;
  openglprofile          = other->openglprofile;
  red                    = other->red;
  green                  = other->green;
  blue                   = other->blue;
  alpha                  = other->alpha;
  depth                  = other->depth;
  stencil                = other->stencil;
  samples                = other->samples;
  refreshrate            = other->refreshrate;
  contextrobustness      = other->contextrobustness;
  contextreleasebehavior = other->contextreleasebehavior;
 }
 void Apply() {
  glfwWindowHint(GLFW_RESIZABLE, resizable ?	GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE	: GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, decorated ?	GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUSED, focused ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_AUTO_ICONIFY, autoiconify ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_FLOATING, floating ?	GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_MAXIMIZED, maximized ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS,	(int)red);
  glfwWindowHint(GLFW_GREEN_BITS,	(int)green);
  glfwWindowHint(GLFW_BLUE_BITS,	(int)blue);
  glfwWindowHint(GLFW_ALPHA_BITS,	(int)alpha);
  glfwWindowHint(GLFW_DEPTH_BITS,	(int)depth);
  glfwWindowHint(GLFW_STENCIL_BITS,	(int)stencil);
  /* Not implemented:
  glfwWindowHint(GLFW_ACCUM_RED_BITS	...)
  glfwWindowHint(GLFW_ACCUM_GREEN_BITS...)
  glfwWindowHint(GLFW_ACCUM_BLUE_BITS...)
  glfwWindowHint(GLFW_ACCUM_ALPHA_BITS...)
  glfwWindowHint(GLFW_AUX_BUFFERS...)
  */
  glfwWindowHint(GLFW_SAMPLES, (int)samples);
  glfwWindowHint(GLFW_REFRESH_RATE, (int)refreshrate);
  glfwWindowHint(GLFW_STEREO, stereo	? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_SRGB_CAPABLE, srgbcapable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, doublebuffer ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_CLIENT_API, (int)clientapi);
  glfwWindowHint(GLFW_CONTEXT_CREATION_API, (int)contextcreationapi);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (int)major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (int)minor);
  glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, (int)contextrobustness);
  glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, (int)contextreleasebehavior);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forwardcompatible	? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debugcontext ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, (int)openglprofile);
 }
MANY(WindowSettings,WindowSettingsHandle,WindowSettingsHandles,"WindowSettings",WindowSettingses,{})
DONE(WindowSettings);

ONE(Window,{})
 ~Window() {
  if ( !closed ) Close();
 }
 Mouse mouse;
 Keyboard keyboard;
 WindowSettings settings,state;
 Zp<GLFWwindow> window;
 Zbool fullscreen, closed, hidden;
 // Apply settings from a file
 void Init(const char *configfile) {
  WindowSettings settings;
  settings.Load(configfile);
  Init(&settings);
 }
 // Initialize before loading or preparing a window
 void Init(WindowSettings *in) {
  if ( in ) settings.Set(in);
  Init();
 }
 // Initialize before loading or preparing a window
 void Init(WindowSettings *in, HardwareSettings *hw) {
  settings.Set(hw);
  if ( in ) settings.Set(in);
  Init();
 }
 // Takes into account window settings and hardware settings to build window
 void Init() {
  glfwDefaultWindowHints();
  settings.Apply();
  state.Set(&settings);
 }
 // Call to signal close
 void Close() {
  if ( !closed ) _OnWindowClose();
 }
 void Hide() { if ( !hidden ) _OnHide(); }
 void Show() { if ( hidden) _OnShow(); }

 Zstring clipboard;
 const char *Clipboard() { clipboard=glfwGetClipboardString(window); return clipboard.c_str(); }
 // Event Processing
 void _OnWindowFocus() { OnWindowFocus(); }
 void _OnWindowBlur() { OnWindowBlur(); }
 void _OnWindowIconify() { OnWindowIconify(); }
 void _OnWindowDeiconify() { OnWindowDeiconify(); }
 void _OnWindowRefresh() { OnWindowRefresh(); }
 void _OnWindowPosition( int newx, int newy ) { OnWindowPosition(newx,newy); }
 void _OnWindowSize( int neww, int newh ) { OnWindowSize(neww,newh); }
 void _OnWindowBufferSize( int neww, int newh ) { OnWindowBufferSize(neww,newh); }
 void _OnWindowClose() { closed=true; if ( !OnClose() ) { glfwSetWindowShouldClose(window,GLFW_FALSE); closed=false; } }
 void _OnMouseEnter() { if ( state.mouseInput ) OnMouseEnter(); }
 void _OnMouseMove( double mx, double my ) { mouse.MouseMoved(mx,my); if ( state.mouseInput ) OnMouseMove(mx,my); }
 void _OnMouseWheel( double wx, double wy) { if (state.mouseInput) OnMouseWheel(wx,wy); }
 void _OnMouseButton(int button, bool pressed, int modifiers) { mouse.ButtonChange(button,pressed,modifiers); if ( state.mouseInput ) OnMouseButton(button,pressed,modifiers); }
 void _OnMouseLeave() { if (state.mouseInput) OnMouseLeave(); }
 void _OnKey() { OnKey(); }
 void _OnText() { OnText(); }
 void _OnPathDrop( const char *path ) { OnPathDrop(path); }
 void _OnHide() { if ( !hidden ) { if ( OnHide() ) { glfwHideWindow(window); hidden=true; } } }
 void _OnShow() { if ( hidden ) { if ( OnShow() ) { glfwShowWindow(window); hidden=false; } } }

 virtual void OnWindowFocus() {}
 virtual void OnWindowBlur() {}
 virtual void OnWindowIconify() {}
 virtual void OnWindowDeiconify() {}
 virtual void OnWindowRefresh() {}
 virtual void OnWindowPosition( int newx, int newy ) {}
 virtual void OnWindowSize( int neww, int newh ) {}
 virtual void OnWindowBufferSize( int neww, int newh ) {}
 virtual void OnMouseEnter() {}
 virtual void OnMouseMove( double mx, double my ) {}
 virtual void OnMouseWheel( double wx, double wy) {}
 virtual void OnMouseButton(int button, bool pressed, int modifiers) {}
 virtual void OnMouseLeave() {}
 virtual void OnKey() {}
 virtual void OnText() {}
 virtual void OnPathDrop( const char *path ) {}
 virtual bool OnClose() { return true; }
 virtual bool OnHide() { return true; }
 virtual bool OnShow() { return true; }
 
 // Called between frames
 void _Between() {}
 virtual void Between() {}

 // Called at frame end
 void _EndFrame() {}
 void EndFrame() {}

 void Terminate() {
  closed=true;
 }
MANY(Window,WindowHandle,WindowHandles,"Window",Windows,{})
 Window *Find( GLFWwindow *by ) {
  FOREACH(Window,w) if ( w->window == by ) return w;
  return null;
 }
 Window *Create( WindowSettings *in ) {
  Window *w=new Window;
  w->Init(in);
  Append(w);
 }
 Window *Create( WindowSettings *in, HardwareSettings *hw ) {
  Window *w=new Window;
  w->Init(in,hw);
  Append(w);
 }
 Window *Add(Window *w) {
  Append(w);
  w->Init();
 }
 Window *Add(Window *w, WindowSettings *in) {
  Append(w);
  w->Init(in);
 }
 Window *Add(Window *w, WindowSettings *in, HardwareSettings *hw) {
  Append(w);
  w->Init(in,hw);
 }
 void Between() {
  FOREACHN(Window,w,{
   if ( w->closed ) {
    Remove(w);
    delete w;
    continue;
   }
   w->_Between();
  });
 }
 void EndFrame() {
  FOREACH(Window,w) if ( !w->closed ) w->_EndFrame();
 }
 void Terminate() {
  FOREACH(Window,w) w->Terminate();
  Clear();
 }
DONE(Window);


class Hardware {
public:
 HardwareSettings settings;
 Joysticks joysticks;
 Monitors monitors;
 Windows windows;
 Zbool initialized;
 Strings errors;
 Zint glfw_Major, glfw_Minor, glfw_Revision;
 Zint frequency;
 Zdouble seconds,last;
 Hardware() {
  glfwSetErrorCallback(glfw_callback_error);
  glfwSetMonitorCallback(glfw_callback_monitor);
 }
 bool Init() {
  OUTPUT("GLFW compilation version: %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
  glfwGetVersion(&glfw_Major.value,&glfw_Minor.value,&glfw_Revision.value);
  OUTPUT("hardware.Init() glfwGetVersion() said GLFW %i.%i.%i\n", (int)glfw_Major, (int)glfw_Minor, (int)glfw_Revision);
  OUTPUT("glfwGetVersionString() said: %s\n", glfwGetVersionString());
  int result = glfwInit();
  if ( result == GLFW_TRUE ) initialized = true;
  else {
   OUTPUT("GLFW had an error!\n");
   return false;
  }
  monitors.Init(); // Gathers all of the data on the monitors and their video modes.
  if ( settings.handleJoystick ) joysticks.Init();
  frequency=(int)glfwGetTimerFrequency();
  return true;
 }
 void Open( WindowSettings *in=null ) {
  windows.Create(in,&settings);
 }
 void CopyToClip( const char *out ) {

 }
 // Process anything between frames
 void Between() {
  last = seconds;
  seconds = glfwGetTime();
  windows.Between();
 }
 // Process anything at the end of the frame
 void EndFrame() {
  windows.EndFrame();
 }
 // Call _onexit()
 void Deinit() {
  if ( initialized ) glfwTerminate();
  windows.Terminate();
 }
 void DebugInfo() {
  OUTPUT("=================================GLFW: Debug Info (Start)\n");
  settings.DebugInfo();
  joysticks.DebugInfo();
  monitors.DebugInfo();
//  windows.DebugInfo();
  if ( initialized ) OUTPUT("Hardware is initialized.\n");
  OUTPUT("Errors:\n");
  errors.Output();
  OUTPUT("Frequency: %d\n",(int)frequency);
  OUTPUT("Seconds: %f, Last: %f\n", (double)seconds, (double)last);
  OUTPUT("=================================GLFW: Debug Info (End)\n");
 }
};


} // namespace LA

extern LA::Hardware hardware;
#endif