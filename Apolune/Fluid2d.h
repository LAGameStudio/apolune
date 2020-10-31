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
#include "GLWindow.h"
#include "FBO.h"

typedef struct FSurface_ {
    GLuint FboHandle;
    GLuint TextureHandle;
    int NumComponents;
} FSurface;

typedef struct FSlab_ {
    FSurface Ping;
    FSurface Pong;
} FSlab;

typedef struct FVector2_ {
    int X;
    int Y;
} FVector2;

extern short Fpositions[];
    
#define SplatRadius ((float) GridWidth / 8.0f)

class Fluid2d {
public:
  int w,h;
  float CellSize;
  int GridWidth,GridHeight;
  float AmbientTemperature,
   ImpulseTemperature,
   ImpulseDensity,
   NumJacobiIterations,
   TimeStep,
   SmokeBuoyancy,
   SmokeWeight,
   GradientScale,
   TemperatureDissipation,
   VelocityDissipation,
   DensityDissipation;
 int PositionSlot;
 FVector2 ImpulsePosition;
 Crayon color;
 Fluid2d() {
  CellSize=1.25f;
  w=display.w;
  h=display.h;
  GridWidth=w/2;
  GridHeight=h/2;
  AmbientTemperature = 0.0f;
  ImpulseTemperature = 10.0f;
  ImpulseDensity = 1.0f;
  NumJacobiIterations = 40;
  TimeStep = 0.125f;
  SmokeBuoyancy = 1.0f;
  SmokeWeight = 0.05f;
  GradientScale = 1.125f / CellSize;
  TemperatureDissipation = 0.99f;
  VelocityDissipation = 0.99f;
  DensityDissipation = 0.9999f;
  PositionSlot = 0;
  ImpulsePosition.X=GridWidth / 2;
  ImpulsePosition.Y=- (int) SplatRadius / 2;
  color.Pick(alabaster);
 }
 void PezCheckCondition(int condition, char *fmt, ...);
 GLuint CreateQuad();
 void UseQuad() {
    // Set up the vertex layout:
    GLsizei stride = (GLsizei) 2 * sizeof(Fpositions[0]);
    glEnableVertexAttribArray(PositionSlot);
    glVertexAttribPointer(PositionSlot, 2, GL_SHORT, GL_FALSE, stride, 0);
 }
 void StopUsingQuad() {
    glDisableVertexAttribArray(PositionSlot);
 }
 GLuint CreateProgram(const char* vsKey, const char* gsKey, const char* fsKey);
 FSurface CreateSurface(GLsizei width, GLsizei height, int numComponents);
 FSlab CreateSlab(GLsizei width, GLsizei height, int numComponents);
 void CreateObstacles(FSurface dest, int width, int height);
 void InitSlabOps();
 void SwapSurfaces(FSlab* slab);
 void ClearSurface(FSurface s, float value);
 void Advect(FSurface velocity, FSurface source, FSurface obstacles, FSurface dest, float dissipation);
 void Jacobi(FSurface pressure, FSurface divergence, FSurface obstacles, FSurface dest);
 void SubtractGradient(FSurface velocity, FSurface pressure, FSurface obstacles, FSurface dest);
 void ComputeDivergence(FSurface velocity, FSurface obstacles, FSurface dest);
 void ApplyImpulse(FSurface dest, FVector2 position, float value);
 void ApplyBuoyancy(FSurface velocity, FSurface temperature, FSurface density, FSurface dest);
 void ResetState();
 GLuint QuadVao, VisualizeProgram;
 FSlab Velocity, Density, Pressure, Temperature;
 FSurface Divergence, Obstacles, HiresObstacles;
 void Resize(int ww, int hh ) {
  w=ww;
  h=hh;
  GridWidth=w/2;
  GridHeight=h/2;
  ImpulsePosition.X=GridWidth / 2;
  ImpulsePosition.Y=- (int) SplatRadius / 2;
 }
 void Init( int ww, int hh ) {
  Resize(ww,hh);
  Velocity = CreateSlab(GridWidth, GridHeight, 2);
  Density = CreateSlab(GridWidth, GridHeight, 1);
  Pressure = CreateSlab(GridWidth, GridHeight, 1);
  Temperature = CreateSlab(GridWidth, GridHeight, 1);
  Divergence = CreateSurface(GridWidth, GridHeight, 3);
  InitSlabOps();
  VisualizeProgram = CreateProgram("Fluid2d.Vertex", 0, "Fluid2d.Visualize");

  Obstacles = CreateSurface(GridWidth, GridHeight, 3);
  //CreateObstacles(Obstacles, GridWidth, GridHeight);

  int dw = w * 2;
  int dh = h * 2;
  HiresObstacles = CreateSurface(dw, dh, 1);
  //CreateObstacles(HiresObstacles, dw, dh);
  
  QuadVao = CreateQuad();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ClearSurface(Temperature.Ping, AmbientTemperature);
  glViewport(0,0,display.w,display.h);
 }
 void Update(unsigned int elapsedMicroseconds) {
  glViewport(0, 0, GridWidth, GridHeight);
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  Advect(Velocity.Ping, Velocity.Ping, Obstacles, Velocity.Pong, VelocityDissipation);
  SwapSurfaces(&Velocity);

  Advect(Velocity.Ping, Temperature.Ping, Obstacles, Temperature.Pong, TemperatureDissipation);
  SwapSurfaces(&Temperature);

  Advect(Velocity.Ping, Density.Ping, Obstacles, Density.Pong, DensityDissipation);
  SwapSurfaces(&Density);

  ApplyBuoyancy(Velocity.Ping, Temperature.Ping, Density.Ping, Velocity.Pong);
  SwapSurfaces(&Velocity);

  ApplyImpulse(Temperature.Ping, ImpulsePosition, ImpulseTemperature);
  ApplyImpulse(Density.Ping, ImpulsePosition, ImpulseDensity);

  ComputeDivergence(Velocity.Ping, Obstacles, Divergence);
  ClearSurface(Pressure.Ping, 0);

  for (int i = 0; i < NumJacobiIterations; ++i) {
   Jacobi(Pressure.Ping, Divergence, Obstacles, Pressure.Pong);
   SwapSurfaces(&Pressure);
  }

  SubtractGradient(Velocity.Ping, Pressure.Ping, Obstacles, Velocity.Pong);
  SwapSurfaces(&Velocity);
  ResetState();
 }
 void Render()
 {
  glPush();
    // Bind visualization shader and set up blend state:
    glUseProgram(VisualizeProgram);
    GLint fillColor = glGetUniformLocation(VisualizeProgram, "FillColor");
    GLint scale = glGetUniformLocation(VisualizeProgram, "Scale");
    glEnable(GL_BLEND);

    // Draw ink:
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Density.Ping.TextureHandle);
    glUniform3f(fillColor, color.floats[0], color.floats[1], color.floats[2]);
    glUniform2f(scale, 1.0f / (float) w, 1.0f / (float) h);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
  // Disable blending:
  //  glDisable(GL_BLEND);
  ResetState();
  glPop();
 }

};

class Fluid2dTest : public GLWindow {
public:
 Fluid2d *fluid2d;
 void OnLoad() {
  SetPositionAndSize(0,0,512,512);
  background.Pick(indigo);
  fluid2d=new Fluid2d;
  fluid2d->Init(512,512);
 }
 void Render() {
  fluid2d->UseQuad();
  fluid2d->Update(1);
  fbo512.Bind();
  fluid2d->Render();
  fbo512.Unbind();
  fbo512.Render(0,0,512,512);
  fluid2d->StopUsingQuad();
 }
 ~Fluid2dTest() {
  DeinitGLWindow();
  delete fluid2d;
 }
};