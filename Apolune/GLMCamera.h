/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

#include "GLSetup.h"
#if defined(USE_GLFW)
#include "Hardware.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Display.h"
#include "LookAtPerspective.h"
#include "UglyFont.h"

class GLMCamera {
public:
 glm::vec3 position;
 glm::vec3 scale;
 glm::vec3 direction; 	
 glm::vec3 right; 	
 glm::vec3 up;
 float horizontal,vertical,initialfov,fov,speed,mouseSpeed;
 glm::mat4 ViewMatrix,ViewInverse,ProjectionMatrix,ModelMatrix,ModelViewMatrix,MVP;
 glm::mat3 ModelView3x3Matrix,NormalMatrix;
#if defined(USE_GLFW)
 Zdouble lastTime;
#endif
 int cumulativeMouseWheel;
 float Near,Far,Aspect;
 GLMCamera() {
  position.x=0;
  position.y=0;
  position.z=5;
  up.x=0;
  up.y=1;
  up.z=0;
  horizontal=3.14f;
  vertical=0.0f;
  initialfov=45.0f;
  speed=3.0f;
  mouseSpeed=0.005f;
#if defined(USE_GLFW)
  lastTime.value=glfwGetTime();
#else
#endif
  cumulativeMouseWheel=0;
  fov=0.0f;
  Near=0.001f;
  Far=100.0f;
  Aspect=4.0f/3.0f; // set later to display.aspect or window aspect
 	ModelMatrix = glm::mat4(1.0);
  scale.x=scale.y=scale.z=1;
 }
 void Apply() { Upload(); }
 void Upload() {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((const GLfloat*)&ProjectionMatrix[0]);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((const GLfloat*)&ModelViewMatrix[0]);
 }
 void MouseChangeOrientation( float deltaT ) {
#if defined(USE_GLFW)
  //double current=glfwGetTime();
  //deltaT=(float) (current-lastTime);
  //lastTime=current;
#else
//  float deltaT=FRAMETIME;
#endif
  // Compute new orientation
  horizontal = 3.14f + mouseSpeed * float(display.w2-input.mxi);
  vertical   = mouseSpeed * float(display.h2-input.myi);
  
  // Direction : Spherical coordinates to Cartesian coordinates conversion
  direction.x=	cos(vertical) * sin(horizontal);
  direction.y= sin(vertical);
  direction.z=	cos(vertical) * cos(horizontal);
  right.x=	sin(horizontal - HALF_PIf);
  right.y=	0;
  right.z=	cos(horizontal - HALF_PIf); 	
  up = glm::cross( right, direction );
 
  // Move forward
  if ( input.KeyDown(DX_W) || input.KeyDown(DX_UP) )  		position += direction * deltaT * speed;
  // Move backward
  if ( input.KeyDown(DX_S) || input.KeyDown(DX_DOWN) )	 position -= direction * deltaT * speed;
  // Strafe right
  if ( input.KeyDown(DX_D) || input.KeyDown(DX_RIGHT) ) position += right * deltaT * speed;
  // Strafe left
  if ( input.KeyDown(DX_A) || input.KeyDown(DX_LEFT) ) 	position -= right * deltaT * speed;
  
  // "zoom"
  if ( input.wheelDown ) cumulativeMouseWheel++;
  else if ( input.wheelUp ) cumulativeMouseWheel--;
  
  fov = initialfov - 5.0f * (float) cumulativeMouseWheel;
  UpdateMatrices();
 }
 void UpdateMatrices() {
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	 ProjectionMatrix = glm::perspective(fov, Aspect, Near,Far);
	 // Camera matrix
	 ViewMatrix       = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
	 );
  ModelMatrix = glm::mat4(
   scale.x,0.0f,0.0f,0.0f,
   0.0f,scale.y,0.0f,0.0f,
   0.0f,0.0f,scale.z,0.0f,
   0.0f,0.0f,0.0f,1.0f
  );
		ModelViewMatrix = ViewMatrix * ModelMatrix;
		ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
  NormalMatrix = glm::transpose(glm::inverse(ModelView3x3Matrix));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
  ViewInverse = glm::inverse(ViewMatrix);
 }
 void fromLap( LookAtPerspective *lap ) {
  up.x=(float)lap->up.x;
  up.y=(float)lap->up.y;
  up.z=(float)lap->up.z;
  position.x=(float)lap->eye.x;
  position.y=(float)lap->eye.y;
  position.z=(float)lap->eye.z;
  direction.x=(float)lap->center.x;
  direction.y=(float)lap->center.y;
  direction.z=(float)lap->center.z;
  fov=(float)lap->halfFOV;
  UpdateMatrices();
 }
 void Orbit( double radius, double headingDegrees, double rollDegrees, bool positiveY, double lerp ) {
  Vertex v;
  float fradius=(float)(radius);
  v.Set(fradius,0.0f,0.0f);
  v.Rotate((float)DEG2RAD(headingDegrees),(positiveY?1.0f:-1.0f)*(float)(lerp*TWO_PI),(float)DEG2RAD(rollDegrees));
  position.x=v.x;
  position.y=v.y;
  position.z=v.z;
  UpdateMatrices();
 }
 void OrbitAcclimate( double radius, double lerp1, double rollDegrees, bool positiveY, double lerp2 ) {
  Vertex v;
  float fradius=(float)(radius);
  v.Set(fradius,0.0f,0.0f);
  v.Rotate((float)(lerp1*TWO_PI),(positiveY?1.0f:-1.0f)*(float)(lerp2*TWO_PI),(float)DEG2RAD(rollDegrees));
  position.x=v.x;
  position.y=v.y;
  position.z=v.z;
  UpdateMatrices();
 }
 void Debug() {
  Text(
   FORMAT(buf,1024,
    "GLMCamera:\nPOS: %f,%f,%f DIR: %f,%f,%f RIGHT: %f,%f,%f UP: %f,%f,%f\nHORIZ: %f VERT: %f initial-FOV: %f FOV: %f SPEED: %f mouseSPEED: %f\n"
    "V: %1.3f %1.3f %1.3f %1.3f P: %1.3f %1.3f %1.3f %1.3f M: %1.3f %1.3f %1.3f %1.3f MV: %1.3f %1.3f %1.3f %1.3f MVP: %1.3f %1.3f %1.3f %1.3f\n"
    "   %1.3f %1.3f %1.3f %1.3f    %1.3f %1.3f %1.3f %1.3f    %1.3f %1.3f %1.3f %1.3f     %1.3f %1.3f %1.3f %1.3f      %1.3f %1.3f %1.3f %1.3f\n"
    "   %1.3f %1.3f %1.3f %1.3f    %1.3f %1.3f %1.3f %1.3f    %1.3f %1.3f %1.3f %1.3f     %1.3f %1.3f %1.3f %1.3f      %1.3f %1.3f %1.3f %1.3f\n"
    "   %1.3f %1.3f %1.3f %1.3f    %1.3f %1.3f %1.3f %1.3f    %1.3f %1.3f %1.3f %1.3f     %1.3f %1.3f %1.3f %1.3f      %1.3f %1.3f %1.3f %1.3f\n",
    position.x, position.y, position.z,
    direction.x, direction.y, direction.z,
    right.x, right.y, right.z,
    up.x, up.y, up.z,
    horizontal,vertical,initialfov,fov,speed,mouseSpeed,
    ViewMatrix.value[0][0],       ViewMatrix.value[0][1],       ViewMatrix.value[0][2],        ViewMatrix.value[0][3],
    ProjectionMatrix.value[0][0], ProjectionMatrix.value[0][1], ProjectionMatrix.value[0][2],  ProjectionMatrix.value[0][3],
    ModelMatrix.value[0][0],      ModelMatrix.value[0][1],      ModelMatrix.value[0][2],       ModelMatrix.value[0][3],
    ModelViewMatrix.value[0][0],  ModelViewMatrix.value[0][1],  ModelViewMatrix.value[0][2],   ModelViewMatrix.value[0][3],
    MVP[0][0],                    MVP[0][1],                    MVP[0][2],                     MVP[0][3],
    ViewMatrix.value[1][0],       ViewMatrix.value[1][1],       ViewMatrix.value[1][2],        ViewMatrix.value[1][3],
    ProjectionMatrix.value[1][0], ProjectionMatrix.value[1][1], ProjectionMatrix.value[1][2],  ProjectionMatrix.value[1][3],
    ModelMatrix.value[1][0],      ModelMatrix.value[1][1],      ModelMatrix.value[1][2],       ModelMatrix.value[1][3],
    ModelViewMatrix.value[1][0],  ModelViewMatrix.value[1][1],  ModelViewMatrix.value[1][2],   ModelViewMatrix.value[1][3],
    MVP[1][0],                    MVP[1][1],                    MVP[1][2],                     MVP[1][3],
    ViewMatrix.value[2][0],       ViewMatrix.value[2][1],       ViewMatrix.value[2][2],        ViewMatrix.value[2][3],
    ProjectionMatrix.value[2][0], ProjectionMatrix.value[2][1], ProjectionMatrix.value[2][2],  ProjectionMatrix.value[2][3],
    ModelMatrix.value[2][0],      ModelMatrix.value[2][1],      ModelMatrix.value[2][2],       ModelMatrix.value[2][3],
    ModelViewMatrix.value[2][0],  ModelViewMatrix.value[2][1],  ModelViewMatrix.value[2][2],   ModelViewMatrix.value[2][3],
    MVP[2][0],                    MVP[2][1],                    MVP[2][2],                     MVP[2][3],
    ViewMatrix.value[3][0],       ViewMatrix.value[3][1],       ViewMatrix.value[3][2],        ViewMatrix.value[3][3],
    ProjectionMatrix.value[3][0], ProjectionMatrix.value[3][1], ProjectionMatrix.value[3][2],  ProjectionMatrix.value[3][3],
    ModelMatrix.value[3][0],      ModelMatrix.value[3][1],      ModelMatrix.value[3][2],       ModelMatrix.value[3][3],
    ModelViewMatrix.value[3][0],  ModelViewMatrix.value[3][1],  ModelViewMatrix.value[3][2],   ModelViewMatrix.value[3][3],
    MVP[3][0],                    MVP[3][1],                    MVP[3][2],                     MVP[3][3]
  ), crayons.jami(0,255,212,86,255),none,10.0, 100.0, 7.0, 8.0, 2.0, 1.0, false
 );
// 	glm::mat3 ModelView3x3Matrix;
 }
};

extern GLMCamera glmCamera;