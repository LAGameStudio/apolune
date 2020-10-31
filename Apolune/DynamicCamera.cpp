#include "DynamicCamera.h"
#include "Display.h"

extern Display display;

DynamicCamera::DynamicCamera()
{
	camSrc = Vector(0,0,0);
	camAng = Vector(0,0,0); //.Set(0, 90, 0); // radians?
	viewMat.Identity();

	camFov = 60.f;
	camRoll = 0;

	angSpeed = 0.1f;
	moveSpeed = 4.f;

    Near=0.1;
    Far=1500000.0;
	newAngSpeed = angSpeed * FRAMETIME;
}

DynamicCamera::DynamicCamera(Vector &vecSrc)
{
	camSrc = vecSrc;
	camAng = Vector(0,0,0); //.Set(0, 90, 0); // radians?
	viewMat.Identity();

	camFov = 60.f;
	camRoll = 0;

	angSpeed = 0.1f;
	moveSpeed = 4.f;

    Near=1.0;
    Far=1500000.0;
	newAngSpeed = angSpeed * FRAMETIME;
}

bool DynamicCamera::Keys() {
 bool result=false;
 sideVec.Set(viewMat.p[0], viewMat.p[1], viewMat.p[2]);
 upVec.Set(viewMat.p[3], viewMat.p[4], viewMat.p[5]);
 dirVec.Set(viewMat.p[6], viewMat.p[7], viewMat.p[8]);

 // Controls
 if (GetAsyncKeyState(VK_RIGHT)) { result=true; camAng.x -= newAngSpeed; }
 if (GetAsyncKeyState(VK_LEFT))  { result=true; camAng.x += newAngSpeed; }
 if (GetAsyncKeyState(VK_UP))    { result=true; camAng.y -= newAngSpeed; }
 if (GetAsyncKeyState(VK_DOWN))  { result=true; camAng.y += newAngSpeed; }
 if (GetAsyncKeyState(VK_PRIOR)) { result=true; camAng.z -= newAngSpeed; }
 if (GetAsyncKeyState(VK_HOME))  { result=true;	camAng.z += newAngSpeed; }

 // forward-backward
 if (GetAsyncKeyState('A')) { result=true; camVel += dirVec*moveSpeed; }
 if (GetAsyncKeyState('Z')) { result=true; camVel -= dirVec*moveSpeed; }

 // up-down
 if (GetAsyncKeyState(VK_SUBTRACT)) { result=true; camVel += upVec*moveSpeed; }
 if (GetAsyncKeyState(VK_ADD))      { result=true; camVel -= upVec*moveSpeed; }

 // left-right
 if (GetAsyncKeyState(VK_NEXT)) { result=true; camVel += sideVec*moveSpeed; }
 if (GetAsyncKeyState(VK_END))  { result=true; camVel -= sideVec*moveSpeed; }
 return result;
}

void DynamicCamera::Between() {
 camAng *= expf(-2.0f*FRAMETIME); 
 camRoll += camAng.z*70; 
 camSrc += camVel * FRAMETIME;
 camVel *= expf(-2.0f*FRAMETIME); 
}

void DynamicCamera::View()
{
// glMatrixMode (GL_PROJECTION); // why is this group of GL commands used?  it came from the original constructor
// glLoadIdentity();
//
 glLoadIdentity();
 gluPerspective(camFov, (GLdouble)display.w/(GLdouble)display.h, Near,Far);
 glMatrixMode(GL_MODELVIEW);
 Matrix angRotMat(camAng.x, camAng.y, camAng.z);

 //glRotatef(camRoll, 0, 0, 1);
 /*gluLookAt(	camSrc.x,			camSrc.y,			camSrc.z,			// src
  		camSrc.x+dirVec.x,	camSrc.y+dirVec.y,	camSrc.z+dirVec.z,	// dst
  		0,					1,					0);// up*/
  
 float m[16] = {
  viewMat.p[0],viewMat.p[3],-viewMat.p[6],0,//-(viewMat.p[0]*camSrc.x + viewMat.p[3]*camSrc.y + viewMat.p[6]*camSrc.z),
  viewMat.p[1],viewMat.p[4],-viewMat.p[7],0,//-(viewMat.p[1]*camSrc.x + viewMat.p[4]*camSrc.y + viewMat.p[7]*camSrc.z),
  viewMat.p[2],viewMat.p[5],-viewMat.p[8],0,//-(viewMat.p[2]*camSrc.x + viewMat.p[5]*camSrc.y + viewMat.p[8]*camSrc.z),
  //0,0,0,1
  -(viewMat.p[0]*camSrc.x + viewMat.p[1]*camSrc.y + viewMat.p[2]*camSrc.z),
  -(viewMat.p[3]*camSrc.x + viewMat.p[4]*camSrc.y + viewMat.p[5]*camSrc.z),
  (viewMat.p[6]*camSrc.x + viewMat.p[7]*camSrc.y + viewMat.p[8]*camSrc.z),1
 };
 glLoadMatrixf(m);
 viewMat = angRotMat*viewMat;
}