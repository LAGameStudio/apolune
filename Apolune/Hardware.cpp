#include "Hardware.h"

#if defined(USE_GLFW)

#include "GLImage.h" // For mousecursors

namespace LA {

Hardware hardware;
 
/* http://www.glfw.org/docs/latest/intro_guide.html#reentrancy
GLFW event processing and object creation and destruction are not reentrant. This means that the following functions must not be called from any callback function:

glfwCreateWindow
glfwDestroyWindow
glfwCreateCursor
glfwCreateStandardCursor
glfwDestroyCursor
glfwPollEvents
glfwWaitEvents
glfwWaitEventsTimeout
glfwTerminate

These functions may be made reentrant in future minor or patch releases, but functions not on this list will not be made non-reentrant.
*/


// ERROR HANDLING
void glfw_callback_error(int error, const char* description) {
 OUTPUT("GLFW Error-->\n%s\n<--GLFW Error Message Ends\n",description);
 String *s=new String();
 s->s= string(description);
 s->integer=error;
 hardware.errors.Append(s);
}

// PATH DROP
void glfw_callback_drop(GLFWwindow* window, int count, const char** paths){
 Window *w = hardware.windows.Find(window);
 for (int i = 0;  i < count;  i++) w->_OnPathDrop(paths[i]);
}

// MONITOR CONNECT / DISCONNECT
void glfw_callback_monitor(GLFWmonitor* monitor, int event) {
 if (event == GLFW_CONNECTED) {
  Monitor *m=hardware.monitors.Find(monitor);
  if ( m ) m->connected=true;
  else {
   m=new Monitor;
   m->Set(monitor);
   m->connected=true;
   hardware.monitors.Append(m);
  }
 } else if (event == GLFW_DISCONNECTED) {
  Monitor *m=hardware.monitors.Find(monitor);
  if ( m != hardware.monitors.first ) {
   hardware.monitors.Remove(m);
   delete m;
  } else m->connected=false;
 }
}

// JOYSTICK CONNECT / DISCONNECT
void glfw_callback_joystick(int joy, int event) { 
 if ( hardware.joysticks.initialized ) {
  if (event == GLFW_CONNECTED) hardware.joysticks.device[joy]->connected=true;
  else if (event == GLFW_DISCONNECTED) hardware.joysticks.device[joy]->connected=false;
 }
}

void glfw_callback_mouseenterleave(GLFWwindow* window, int entered) {
 Window *w = hardware.windows.Find(window);
 if ( !w || !w->mouse.initialized ) return;
 if ( entered == GL_TRUE ) w->_OnMouseEnter();
 else w->_OnMouseLeave();
}

void glfw_callback_mousemove(GLFWwindow* window, double xpos, double ypos) {
 Window *w = hardware.windows.Find(window);
 if ( !w || !w->mouse.initialized ) return;
 w->_OnMouseMove(xpos,ypos);
}

void glfw_callback_mousebutton(GLFWwindow* window, int button, int action, int mods) {
 Window *w = hardware.windows.Find(window);
 if ( !w || !w->mouse.initialized ) return;
 w->_OnMouseButton(button,(action == GLFW_PRESS),mods);
}

void glfw_callback_scrollwheel(GLFWwindow* window, double xoffset, double yoffset) {
 Window *w = hardware.windows.Find(window);
 if ( !w || !w->mouse.initialized ) return;
 w->_OnMouseWheel(xoffset,yoffset);
}

void glfw_callback_window_focusblur(GLFWwindow *window, int action) {
 Window *w = hardware.windows.Find(window);
 if ( !w ) return;
 if ( action == GL_TRUE ) w->_OnWindowFocus();
 else w->_OnWindowBlur();
}

void glfw_callback_window_iconify(GLFWwindow *window, int action) {
 Window *w = hardware.windows.Find(window);
 if ( !w ) return;
 w->_OnWindowIconify();
 if ( action == GL_TRUE ) w->_OnWindowIconify();
 else w->_OnWindowDeiconify();
}


void glfw_callback_window_refresh(GLFWwindow *window) {
 Window *w = hardware.windows.Find(window);
 if ( !w ) return;
 w->_OnWindowRefresh();
}

void glfw_callback_window_position(GLFWwindow *window, int newx, int newy) {
 Window *w = hardware.windows.Find(window);
 if ( !w ) return;
 w->_OnWindowPosition(newx,newy);
}

void glfw_callback_window_size(GLFWwindow *window, int neww, int newh) {
 Window *w = hardware.windows.Find(window);
 if ( !w ) return;
 w->_OnWindowSize(neww,newh);
}

void glfw_callback_window_buffer_size(GLFWwindow *window, int neww, int newh) {
 Window *w = hardware.windows.Find(window);
 if ( !w ) return;
 w->_OnWindowBufferSize(neww,newh);
}

void glfw_callback_window_close(GLFWwindow *window) {
 Window *w = hardware.windows.Find(window);
 if ( !w ) return;
 w->_OnWindowClose();
}

void MouseCursor::Set(Window *in) {
 glfwSetCursor(in->window,cursor);
}

 void MouseCursors::Set(Window *in, const char *name ) {
  MouseCursor *toset=this->firstNamed(name);
  if ( toset ) {
   toset->Set(in);
  }
 }
 void MouseCursors::Add(const char *name, CPUImage *in, int xhot, int yhot) {
 }


} // namespace LA

#endif