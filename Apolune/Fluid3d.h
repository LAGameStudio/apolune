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
#include "GLWindow.h"
#include <vector>
#include "vmath.hpp"
#include "glsw.h"
#include <string.h>
#include <math.h>

using namespace vmath;

class Trackball {
 public:
  Trackball();
  Trackball(float width, float height, float radius);
  void MouseDown(int x, int y);
  void MouseUp(int x, int y);
  void MouseMove(int x, int y);
  void ReturnHome();
  vmath::VMatrix3 GetRotation() const;
  void Update(unsigned int microseconds);
  float GetZoom() const;
 private:
  vmath::VVector3 MapToSphere(int x, int y);
  vmath::VVector3 m_startPos;
  vmath::VVector3 m_currentPos;
  vmath::VVector3 m_previousPos;
  vmath::VVector3 m_axis;
  vmath::VQuat m_quat;
  bool m_active;
  float m_radius;
  float m_radiansPerSecond;
  float m_distancePerSecond;
  float m_width;
  float m_height;
  float m_zoom;
  float m_startZoom;
  int m_startY;
  unsigned int m_currentTime;
  unsigned int m_previousTime;

  struct VoyageHome {
      bool Active;
      vmath::VQuat DepartureQuat;
      float DepartureZoom;
      unsigned int microseconds;
  } m_voyageHome;

  struct Inertia {
      bool Active;
      vmath::VVector3 Axis;
      float RadiansPerSecond;
      float DistancePerSecond;
  } m_inertia;
};

using namespace vmath;
using std::string;

enum AttributeSlot {
    SlotPosition,
    SlotTexCoord,
};

struct TexturePod {
    GLuint Handle;
    GLsizei Width;
    GLsizei Height;
};

struct SurfacePod {
    GLuint FboHandle;
    GLuint ColorTexture;
    GLsizei Width;
    GLsizei Height;
    GLsizei Depth;
};

struct SlabPod {
    SurfacePod Ping;
    SurfacePod Pong;
};

static struct {
    GLuint Advect;
    GLuint Jacobi;
    GLuint SubtractGradient;
    GLuint ComputeDivergence;
    GLuint ApplyImpulse;
    GLuint ApplyBuoyancy;
} Programs;

static struct {
    SlabPod Velocity;
    SlabPod Density;
    SlabPod Pressure;
    SlabPod Temperature;
} Slabs;

static struct {
    SurfacePod Divergence;
    SurfacePod Obstacles;
    SurfacePod HiresObstacles;
} Surfaces;

static struct {
    VMatrix4 Projection;
    VMatrix4 Modelview;
    VMatrix4 View;
    VMatrix4 ModelviewProjection;
} VMatrices;

static struct {
    GLuint CubeCenter;
    GLuint FullscreenQuad;
} Vbos;

class Fluid3d {
public:
 float CellSize;
 int ViewportWidth;
 int ViewportHeight;
 int GridWidth;
 int GridHeight;
 int GridDepth;
 float SplatRadius;
 float AmbientTemperature;
 float ImpulseTemperature;
 float ImpulseDensity;
 int NumJacobiIterations;
 float TimeStep;
 float SmokeBuoyancy;
 float SmokeWeight;
 float GradientScale;
 float TemperatureDissipation;
 float VelocityDissipation;
 float DensityDissipation;
 vmath::VVector3 ImpulsePosition;
 VPoint3 EyePosition;
 GLuint RaycastProgram;
 float FieldOfView;
 bool SimulateFluid;
 Trackball trackball;
 Fluid3d() {
  CellSize = 1.25f;
  ViewportWidth = 320;
  GridWidth = 96;
  ViewportHeight = ViewportWidth;
  GridHeight = GridWidth;
  GridDepth = GridWidth;
  SplatRadius = GridWidth / 8.0f;
  AmbientTemperature = 0.0f;
  ImpulseTemperature = 10.0f;
  ImpulseDensity = 1.0f;
  NumJacobiIterations = 40;
  TimeStep = 0.25f;
  SmokeBuoyancy = 1.0f;
  SmokeWeight = 0.0125f;
  GradientScale = 1.125f / CellSize;
  TemperatureDissipation = 0.99f;
  VelocityDissipation = 0.99f;
  DensityDissipation = 0.9995f;
  VVector3 ImpulsePosition( GridWidth / 2.0f, GridHeight - (int) SplatRadius / 2.0f, GridDepth / 2.0f);
  VPoint3 EyePosition;
  RaycastProgram=0;
  FieldOfView = 0.7f;
  SimulateFluid = true;
 }
 void PezCheckCondition(int condition, char *fmt, ...);
 GLuint LoadProgram(const char* vsKey, const char* gsKey, const char* fsKey);
 void SetUniform(const char* name, int value);
 void SetUniform(const char* name, float value);
 void SetUniform(const char* name, float x, float y);
 void SetUniform(const char* name, vmath::VMatrix4 value);
 void SetUniform(const char* name, vmath::VMatrix3 value);
 void SetUniform(const char* name, vmath::VVector3 value);
 void SetUniform(const char* name, vmath::VPoint3 value);
 void SetUniform(const char* name, vmath::VVector4 value);
 TexturePod LoadTexture(const char* path);
 SurfacePod CreateSurface(int width, int height, int numComponents = 4);
 SurfacePod CreateVolume(int width, int height, int depth, int numComponents = 4);
 GLuint CreatePointVbo(float x, float y, float z);
 GLuint CreateQuadVbo();
 void CreateObstacles(SurfacePod dest);
 SlabPod CreateSlab(GLsizei width, GLsizei height, GLsizei depth, int numComponents);
 void ResetState();
 void InitSlabOps();
 void SwapSurfaces(SlabPod* slab);
 void ClearSurface(SurfacePod s, float v);
 void Advect(SurfacePod velocity, SurfacePod source, SurfacePod obstacles, SurfacePod dest, float dissipation);
 void Jacobi(SurfacePod pressure, SurfacePod divergence, SurfacePod obstacles, SurfacePod dest);
 void SubtractGradient(SurfacePod velocity, SurfacePod pressure, SurfacePod obstacles, SurfacePod dest);
 void ComputeDivergence(SurfacePod velocity, SurfacePod obstacles, SurfacePod dest);
 void ApplyImpulse(SurfacePod dest, vmath::VVector3 position, float value);
 void ApplyBuoyancy(SurfacePod velocity, SurfacePod temperature, SurfacePod density, SurfacePod dest);
 void Init() {
  OUTPUT("Initializing Fluid3d.\n");
  RaycastProgram = LoadProgram("Raycast.VS", "Raycast.GS", "Raycast.FS");
  Vbos.CubeCenter = CreatePointVbo(0, 0, 0);
  Vbos.FullscreenQuad = CreateQuadVbo();

  Slabs.Velocity = CreateSlab(GridWidth, GridHeight, GridDepth, 3);
  Slabs.Density = CreateSlab(GridWidth, GridHeight, GridDepth, 1);
  Slabs.Pressure = CreateSlab(GridWidth, GridHeight, GridDepth, 1);
  Slabs.Temperature = CreateSlab(GridWidth, GridHeight, GridDepth, 1);
  Surfaces.Divergence = CreateVolume(GridWidth, GridHeight, GridDepth, 3);
  InitSlabOps();
  Surfaces.Obstacles = CreateVolume(GridWidth, GridHeight, GridDepth, 3);
  CreateObstacles(Surfaces.Obstacles);
  ClearSurface(Slabs.Temperature.Ping, AmbientTemperature);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
 }
 void Render() {
  Between();
  glPush();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnableVertexAttribArray(SlotPosition);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBindBuffer(GL_ARRAY_BUFFER, Vbos.CubeCenter);
  glVertexAttribPointer(SlotPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glBindTexture(GL_TEXTURE_3D, Slabs.Density.Ping.ColorTexture);
  glUseProgram(RaycastProgram);
  SetUniform("ModelviewProjection", VMatrices.ModelviewProjection);
  SetUniform("Modelview", VMatrices.Modelview);
  SetUniform("ViewMatrix", VMatrices.View);
  SetUniform("ProjectionMatrix", VMatrices.Projection);
  SetUniform("RayStartPoints", 1);
  SetUniform("RayStopPoints", 2);
  SetUniform("EyePosition", EyePosition);
  SetUniform("RayOrigin", VVector4(transpose(VMatrices.Modelview) * EyePosition).getXYZ());
  SetUniform("FocalLength", 1.0f / std::tan(FieldOfView / 2));
  SetUniform("WindowSize", float(display.w), float(display.h));
  glDrawArrays(GL_POINTS, 0, 1);
  glPop();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0,0,display.w,display.h);
 }
 void Between() {
  EyePosition = VPoint3(0, 0, 3.5f + trackball.GetZoom());
  VVector3 up(0, 1, 0);
  VPoint3 target(0);
  VMatrices.View = VMatrix4::lookAt(EyePosition, target, up);
  VMatrix4 modelMatrix(transpose(trackball.GetRotation()), VVector3());
  modelMatrix *= VMatrix4::rotationY(0.5f);
  VMatrices.Modelview = VMatrices.View * modelMatrix;  
  VMatrices.Projection = VMatrix4::perspective(
      FieldOfView,
      float(display.w) / float(display.h), // Aspect Ratio
      0.0f,   // Near Plane
      1.0f);  // Far Plane

  VMatrices.ModelviewProjection = VMatrices.Projection * VMatrices.Modelview;

  if (SimulateFluid) {
   glBindBuffer(GL_ARRAY_BUFFER, Vbos.FullscreenQuad);
   glVertexAttribPointer(SlotPosition, 2, GL_SHORT, GL_FALSE, 2 * sizeof(short), 0);
   glViewport(0, 0, GridWidth, GridHeight);
   Advect(Slabs.Velocity.Ping, Slabs.Velocity.Ping, Surfaces.Obstacles, Slabs.Velocity.Pong, VelocityDissipation);
   SwapSurfaces(&Slabs.Velocity);
   Advect(Slabs.Velocity.Ping, Slabs.Temperature.Ping, Surfaces.Obstacles, Slabs.Temperature.Pong, TemperatureDissipation);
   SwapSurfaces(&Slabs.Temperature);
   Advect(Slabs.Velocity.Ping, Slabs.Density.Ping, Surfaces.Obstacles, Slabs.Density.Pong, DensityDissipation);
   SwapSurfaces(&Slabs.Density);
   ApplyBuoyancy(Slabs.Velocity.Ping, Slabs.Temperature.Ping, Slabs.Density.Ping, Slabs.Velocity.Pong);
   SwapSurfaces(&Slabs.Velocity);
   ApplyImpulse(Slabs.Temperature.Ping, ImpulsePosition, ImpulseTemperature);
   ApplyImpulse(Slabs.Density.Ping, ImpulsePosition, ImpulseDensity);
   ComputeDivergence(Slabs.Velocity.Ping, Surfaces.Obstacles, Surfaces.Divergence);
   ClearSurface(Slabs.Pressure.Ping, 0);
   for (int i = 0; i < NumJacobiIterations; ++i) {
    Jacobi(Slabs.Pressure.Ping, Surfaces.Divergence, Surfaces.Obstacles, Slabs.Pressure.Pong);
    SwapSurfaces(&Slabs.Pressure);
   }
   SubtractGradient(Slabs.Velocity.Ping, Slabs.Pressure.Ping, Surfaces.Obstacles, Slabs.Velocity.Pong);
   SwapSurfaces(&Slabs.Velocity);
  }
  glViewport(0,0,display.w,display.h);
  glPop();
 }
 void MouseDown( int x, int y ) {
  trackball.MouseDown(x, y);
 }
 void MouseUp( int x, int y ) {
  trackball.MouseUp(x, y);
 }
 void MouseMove( int x, int y ) {
  trackball.MouseMove(x, y);
 }
 void ResetMouse() {
  trackball.ReturnHome();
 }
};

extern Fluid3d fluid3d;