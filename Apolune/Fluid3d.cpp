#include "Fluid3d.h"

using namespace vmath;

Fluid3d fluid3d;

const float CellSize = 1.25f;
const int ViewportWidth = 320;
const int GridWidth = 96;
const int ViewportHeight = ViewportWidth;
const int GridHeight = GridWidth;
const int GridDepth = GridWidth;
const float SplatRadius = GridWidth / 8.0f;
const float AmbientTemperature = 0.0f;
const float ImpulseTemperature = 10.0f;
const float ImpulseDensity = 1.0f;
const int NumJacobiIterations = 40;
const float TimeStep = 0.25f;
const float SmokeBuoyancy = 1.0f;
const float SmokeWeight = 0.0125f;
const float GradientScale = 1.125f / CellSize;
const float TemperatureDissipation = 0.99f;
const float VelocityDissipation = 0.99f;
const float DensityDissipation = 0.9995f;
const VVector3 ImpulsePosition( GridWidth / 2.0f, GridHeight - (int) SplatRadius / 2.0f, GridDepth / 2.0f);
VPoint3 EyePosition;
GLuint RaycastProgram;
float FieldOfView = 0.7f;
bool SimulateFluid = true;

void Fluid3d::PezCheckCondition(int condition, char *fmt, ...)
{
 if (condition) return;
 static char buf [32768];
 buf[0] = '\0';
 va_list args;
 va_start (args, fmt);
 vsprintf_s (buf, fmt, args);
 va_end (args);
 OutputDebugString(TEXT(buf));
}

/*
 * Obstacles. It has three components: the red component is essentially 
 * a boolean for solid versus empty, and the green/blue channels represent
 * the obstacle’s velocity.
 */
void Fluid3d::CreateObstacles(SurfacePod dest)
{
    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glViewport(0, 0, dest.Width, dest.Height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    OUTPUT("Fluid3d:CreateObstacles() Vertex Array id %d\n", vao );
    GLuint program = LoadProgram("Fluid.Vertex", 0, "Fluid.Fill");
    glUseProgram(program);

    GLuint lineVbo;
    glGenBuffers(1, &lineVbo);
    GLuint circleVbo;
    glGenBuffers(1, &circleVbo);
    glEnableVertexAttribArray(SlotPosition);

    for (int slice = 0; slice < dest.Depth; ++slice) {

        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dest.ColorTexture, 0, dest.Depth - 1 - slice);
        float z = dest.Depth / 2.0f;
        z = abs(slice - z) / z;
        float fraction = 1 - sqrt(z);
        float radius = 0.5f * fraction;

        if (slice == 0 || slice == dest.Depth - 1) {
            radius *= 100;
        }

        const bool DrawBorder = true;
        if (DrawBorder && slice != 0 && slice != dest.Depth - 1) {
            #define T 0.9999f
            float positions[] = { -T, -T, T, -T, T,  T, -T,  T, -T, -T };
            #undef T
            GLsizei size = sizeof(positions);
            glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
            glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
            GLsizei stride = 2 * sizeof(positions[0]);
            glVertexAttribPointer(SlotPosition, 2, GL_FLOAT, GL_FALSE, stride, 0);
            glDrawArrays(GL_LINE_STRIP, 0, 5);
        }

        const bool DrawSphere = false;
        if (DrawSphere || slice == 0 || slice == dest.Depth - 1) {
            const int slices = 64;
            float positions[slices*2*3];
            float twopi = 8*atan(1.0f);
            float theta = 0;
            float dtheta = twopi / (float) (slices - 1);
            float* pPositions = &positions[0];
            for (int i = 0; i < slices; i++) {
                *pPositions++ = 0;
                *pPositions++ = 0;

                *pPositions++ = radius * cos(theta);
                *pPositions++ = radius * sin(theta);
                theta += dtheta;

                *pPositions++ = radius * cos(theta);
                *pPositions++ = radius * sin(theta);
            }
            GLsizei size = sizeof(positions);
            glBindBuffer(GL_ARRAY_BUFFER, circleVbo);
            glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
            GLsizei stride = 2 * sizeof(positions[0]);
            glVertexAttribPointer(SlotPosition, 2, GL_FLOAT, GL_FALSE, stride, 0);
            glDrawArrays(GL_TRIANGLES, 0, slices * 3);
        }
    }

    // Cleanup
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &lineVbo);
    glDeleteBuffers(1, &circleVbo);
}

GLuint Fluid3d::LoadProgram(const char* vsKey, const char* gsKey, const char* fsKey)
{   
    const char* vsSource = glswGetShader(vsKey);
    const char* gsSource = glswGetShader(gsKey);
    const char* fsSource = glswGetShader(fsKey);

    char* msg = "Can't find %s shader: '%s'.\n";
    PezCheckCondition(vsSource != 0, msg, "vertex", vsKey);
    PezCheckCondition(gsKey == 0 || gsSource != 0, msg, "geometry", gsKey);
    PezCheckCondition(fsKey == 0 || fsSource != 0, msg, "fragment", fsKey);
    
    GLint compileSuccess;
    GLchar compilerSpew[256];
    GLuint programHandle = glCreateProgram();

    GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsHandle, 1, &vsSource, 0);
    glCompileShader(vsHandle);
    glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(vsHandle, sizeof(compilerSpew), 0, compilerSpew);
    PezCheckCondition(compileSuccess, "Can't compile %s:\n%s", vsKey, compilerSpew);
    glAttachShader(programHandle, vsHandle);

    GLuint gsHandle;
    if (gsKey) {
        gsHandle = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gsHandle, 1, &gsSource, 0);
        glCompileShader(gsHandle);
        glGetShaderiv(gsHandle, GL_COMPILE_STATUS, &compileSuccess);
        glGetShaderInfoLog(gsHandle, sizeof(compilerSpew), 0, compilerSpew);
        PezCheckCondition(compileSuccess, "Can't compile %s:\n%s", gsKey, compilerSpew);
        glAttachShader(programHandle, gsHandle);
    }
    
    GLuint fsHandle;
    if (fsKey) {
        fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fsHandle, 1, &fsSource, 0);
        glCompileShader(fsHandle);
        glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &compileSuccess);
        glGetShaderInfoLog(fsHandle, sizeof(compilerSpew), 0, compilerSpew);
        PezCheckCondition(compileSuccess, "Can't compile %s:\n%s", fsKey, compilerSpew);
        glAttachShader(programHandle, fsHandle);
    }

    glBindAttribLocation(programHandle, SlotPosition, "Position");
    glBindAttribLocation(programHandle, SlotTexCoord, "TexCoord");
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    glGetProgramInfoLog(programHandle, sizeof(compilerSpew), 0, compilerSpew);

    if (!linkSuccess) {
        OUTPUT("Link error.\n");
        if (vsKey) OUTPUT("Vertex Shader: %s\n", vsKey);
        if (gsKey) OUTPUT("Geometry Shader: %s\n", gsKey);
        if (fsKey) OUTPUT("Fragment Shader: %s\n", fsKey);
        OUTPUT("%s\n", compilerSpew);
    }
    else OUTPUT("Fluid3d:Init() Vertex, Geometry, Fragment compiled successfully.\n");
    return programHandle;
}

SlabPod Fluid3d::CreateSlab(GLsizei width, GLsizei height, GLsizei depth, int numComponents)
{
    SlabPod slab;
    slab.Ping = CreateVolume(width, height, depth, numComponents);
    slab.Pong = CreateVolume(width, height, depth, numComponents);
    return slab;
}

SurfacePod Fluid3d::CreateSurface(GLsizei width, GLsizei height, int numComponents)
{
    GLuint fboHandle;
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (numComponents) {
        case 1:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_HALF_FLOAT, 0);
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_HALF_FLOAT, 0);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_HALF_FLOAT, 0);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            break;
    }

    PezCheckCondition(GL_NO_ERROR == glGetError(), "Unable to create normals texture");

    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
    PezCheckCondition(GL_NO_ERROR == glGetError(), "Unable to attach color buffer");
    
    PezCheckCondition(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "Unable to create FBO.");
    SurfacePod surface = { fboHandle, textureHandle };

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    surface.Width = width;
    surface.Height = height;
    surface.Depth = 1;
    return surface;
}

SurfacePod Fluid3d::CreateVolume(GLsizei width, GLsizei height, GLsizei depth, int numComponents)
{
    GLuint fboHandle;
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_3D, textureHandle);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (numComponents) {
        case 1:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, width, height, depth, 0, GL_RED, GL_HALF_FLOAT, 0);
            break;
        case 2:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16F, width, height, depth, 0, GL_RG, GL_HALF_FLOAT, 0);
            break;
        case 3:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, width, height, depth, 0, GL_RGB, GL_HALF_FLOAT, 0);
            break;
        case 4:
            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, width, height, depth, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            break;
    }

    PezCheckCondition(GL_NO_ERROR == glGetError(), "Unable to create volume texture");

    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureHandle, 0);
    PezCheckCondition(GL_NO_ERROR == glGetError(), "Unable to attach color buffer");
    OUTPUT("CreateVolume: Colorbuffer FBO id %d created with RBO id %d, texture %d\n",
     (int)fboHandle, (int)colorbuffer, (int)textureHandle );

    PezCheckCondition(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "Unable to create FBO.");
    SurfacePod surface = { fboHandle, textureHandle };

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    surface.Width = width;
    surface.Height = height;
    surface.Depth = depth;
    return surface;
}

void Fluid3d::ResetState()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_3D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
}

void Fluid3d::InitSlabOps()
{
    Programs.Advect = LoadProgram("Fluid.Vertex", "Fluid.PickLayer", "Fluid.Advect");
    Programs.Jacobi = LoadProgram("Fluid.Vertex", "Fluid.PickLayer", "Fluid.Jacobi");
    Programs.SubtractGradient = LoadProgram("Fluid.Vertex", "Fluid.PickLayer", "Fluid.SubtractGradient");
    Programs.ComputeDivergence = LoadProgram("Fluid.Vertex", "Fluid.PickLayer", "Fluid.ComputeDivergence");
    Programs.ApplyImpulse = LoadProgram("Fluid.Vertex", "Fluid.PickLayer", "Fluid.Splat");
    Programs.ApplyBuoyancy = LoadProgram("Fluid.Vertex", "Fluid.PickLayer", "Fluid.Buoyancy");
}

void Fluid3d::SwapSurfaces(SlabPod* slab)
{
    SurfacePod temp = slab->Ping;
    slab->Ping = slab->Pong;
    slab->Pong = temp;
}

void Fluid3d::ClearSurface(SurfacePod s, float v)
{
    glBindFramebuffer(GL_FRAMEBUFFER, s.FboHandle);
    glClearColor(v, v, v, v);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Fluid3d::Advect(SurfacePod velocity, SurfacePod source, SurfacePod obstacles, SurfacePod dest, float dissipation)
{
    GLuint p = Programs.Advect;
    glUseProgram(p);

    SetUniform("InverseSize", recipPerElem(VVector3(float(GridWidth), float(GridHeight), float(GridDepth))));
    SetUniform("TimeStep", TimeStep);
    SetUniform("Dissipation", dissipation);
    SetUniform("SourceTexture", 1);
    SetUniform("Obstacles", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, source.ColorTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState();
}

void Fluid3d::Jacobi(SurfacePod pressure, SurfacePod divergence, SurfacePod obstacles, SurfacePod dest)
{
    GLuint p = Programs.Jacobi;
    glUseProgram(p);

    SetUniform("Alpha", -CellSize * CellSize);
    SetUniform("InverseBeta", 0.1666f);
    SetUniform("Divergence", 1);
    SetUniform("Obstacles", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, pressure.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, divergence.ColorTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState();
}

void Fluid3d::SubtractGradient(SurfacePod velocity, SurfacePod pressure, SurfacePod obstacles, SurfacePod dest)
{
    GLuint p = Programs.SubtractGradient;
    glUseProgram(p);

    SetUniform("GradientScale", GradientScale);
    SetUniform("HalfInverseCellSize", 0.5f / CellSize);
    SetUniform("Pressure", 1);
    SetUniform("Obstacles", 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, pressure.ColorTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState();
}

void Fluid3d::ComputeDivergence(SurfacePod velocity, SurfacePod obstacles, SurfacePod dest)
{
    GLuint p = Programs.ComputeDivergence;
    glUseProgram(p);

    SetUniform("HalfInverseCellSize", 0.5f / CellSize);
    SetUniform("Obstacles", 1);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, obstacles.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState();
}

void Fluid3d::ApplyImpulse(SurfacePod dest, VVector3 position, float value)
{
    GLuint p = Programs.ApplyImpulse;
    glUseProgram(p);

    SetUniform("Point", position);
    SetUniform("Radius", SplatRadius);
    SetUniform("FillColor", VVector3(value, value, value));

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glEnable(GL_BLEND);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState();
}

void Fluid3d::ApplyBuoyancy(SurfacePod velocity, SurfacePod temperature, SurfacePod density, SurfacePod dest)
{
    GLuint p = Programs.ApplyBuoyancy;
    glUseProgram(p);

    SetUniform("Temperature", 1);
    SetUniform("Density", 2);
    SetUniform("AmbientTemperature", AmbientTemperature);
    SetUniform("TimeStep", TimeStep);
    SetUniform("Sigma", SmokeBuoyancy);
    SetUniform("Kappa", SmokeWeight);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, velocity.ColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, temperature.ColorTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, density.ColorTexture);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, dest.Depth);
    ResetState();
}

GLuint Fluid3d::CreatePointVbo(float x, float y, float z)
{
    float p[] = {x, y, z};
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(p), &p[0], GL_STATIC_DRAW);
    OUTPUT("Point VBO created with id %d\n", (int) vbo );
    return vbo;
}

void Fluid3d::SetUniform(const char* name, int value)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

void Fluid3d::SetUniform(const char* name, float value)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

void Fluid3d::SetUniform(const char* name, VMatrix4 value)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, 0, (float*) &value);
}

void Fluid3d::SetUniform(const char* name, VMatrix3 nm)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    float packed[9] = {
        nm.getRow(0).getX(), nm.getRow(1).getX(), nm.getRow(2).getX(),
        nm.getRow(0).getY(), nm.getRow(1).getY(), nm.getRow(2).getY(),
        nm.getRow(0).getZ(), nm.getRow(1).getZ(), nm.getRow(2).getZ() };
    glUniformMatrix3fv(location, 1, 0, &packed[0]);
}

void Fluid3d::SetUniform(const char* name, VVector3 value)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    glUniform3f(location, value.getX(), value.getY(), value.getZ());
}

void Fluid3d::SetUniform(const char* name, float x, float y)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    glUniform2f(location, x, y);
}

void Fluid3d::SetUniform(const char* name, VVector4 value)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    glUniform4f(location, value.getX(), value.getY(), value.getZ(), value.getW());
}

void Fluid3d::SetUniform(const char* name, VPoint3 value)
{
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    GLint location = glGetUniformLocation(program, name);
    glUniform3f(location, value.getX(), value.getY(), value.getZ());
}

GLuint Fluid3d::CreateQuadVbo()
{
    short positions[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };
    GLuint vbo;
    GLsizeiptr size = sizeof(positions);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
    OUTPUT("Fullscreen Quad VBO created with id %d\n", (int) vbo );
    return vbo;
}

Trackball::Trackball() {
    m_currentTime = 0;
    m_inertia.Active = false;
    m_voyageHome.Active = false;
    m_active = false;
    m_quat = vmath::VQuat::identity();
    m_radius = (float) (display.h/2);
    m_startPos = m_currentPos = m_previousPos = VVector3(0);
    m_width = (float)(display.w);
    m_height = (float)(display.h);
    m_startZoom = m_zoom = 0;
}

Trackball::Trackball(float width, float height, float radius)
{
    m_currentTime = 0;
    m_inertia.Active = false;
    m_voyageHome.Active = false;
    m_active = false;
    m_quat = vmath::VQuat::identity();
    m_radius = radius;
    m_startPos = m_currentPos = m_previousPos = VVector3(0);
    m_width = width;
    m_height = height;
    m_startZoom = m_zoom = 0;
}

void Trackball::MouseDown(int x, int y)
{
    m_radiansPerSecond = 0;
    m_distancePerSecond = 0;
    m_previousPos = m_currentPos = m_startPos = MapToSphere(x, y);
    m_active = true;
    m_startZoom = m_zoom;
    m_startY = y;
}

void Trackball::MouseUp(int x, int y)
{
    if (m_active) {
        float deltaDistance = (y - m_startY) * 0.01f;
        m_zoom = m_startZoom + deltaDistance;
        m_startZoom = m_zoom;
        m_startY = y;
        m_active = false;
    }

    VQuat q = VQuat::rotation(m_startPos, m_currentPos);
    m_quat = rotate(q, m_quat);

    if (m_radiansPerSecond > 0 || m_distancePerSecond != 0) {
        m_inertia.Active = true;
        m_inertia.RadiansPerSecond = m_radiansPerSecond;
        m_inertia.DistancePerSecond = m_distancePerSecond;
        m_inertia.Axis = m_axis;
    }
}

void Trackball::MouseMove(int x, int y)
{
    m_currentPos = MapToSphere(x, y);

    float radians = acos(dot(m_previousPos, m_currentPos));
    unsigned int microseconds = m_currentTime - m_previousTime;
    
    if (radians > 0.01f && microseconds > 0) {
        m_radiansPerSecond = 1000000.0f * radians / microseconds;
        m_axis = normalize(cross(m_previousPos, m_currentPos));
    } else {
        m_radiansPerSecond = 0;
    }
/*
    if (m_active) {
        float deltaDistance = (y - m_startY) * 0.01f;
        if (std::abs(deltaDistance) > 0.03f && microseconds > 0) {
            m_distancePerSecond = 1000000.0f * deltaDistance / microseconds;
        } else {
            m_distancePerSecond = 0;
        }

        m_zoom = m_startZoom + deltaDistance;
    }
*/
    m_startZoom = m_zoom;
    m_startY = y;

    m_previousPos = m_currentPos;
    m_previousTime = m_currentTime;
}

VMatrix3 Trackball::GetRotation() const
{
    if (!m_active)
        return VMatrix3(m_quat);

    VQuat q = VQuat::rotation(m_startPos, m_currentPos);
    return VMatrix3(rotate(q, m_quat));
}

VVector3 Trackball::MapToSphere(int x, int y)
{
    x = int(m_width) - x;
    const float SafeRadius = m_radius * 0.99f;
    float fx = x - m_width / 2.0f;
    float fy = y - m_height / 2.0f;

    float lenSqr = fx*fx+fy*fy;
    
    if (lenSqr > SafeRadius*SafeRadius) {
        float theta = atan2(fy, fx);
        fx = SafeRadius * cos(theta);
        fy = SafeRadius * sin(theta);
    }
    
    lenSqr = fx*fx+fy*fy;
    float z = sqrt(m_radius*m_radius - lenSqr);
    return VVector3(fx, fy, z) / m_radius;
}

void Trackball::Update(unsigned int microseconds)
{
    m_currentTime += microseconds;

    if (m_voyageHome.Active) {
        m_voyageHome.microseconds += microseconds;
        float t = m_voyageHome.microseconds / 200000.0f;
        
        if (t > 1) {
            m_quat = VQuat::identity();
            m_startZoom = m_zoom = 0;
            m_voyageHome.Active = false;
            return;
        }

        m_quat = slerp(t, m_voyageHome.DepartureQuat, VQuat::identity());
        m_startZoom = m_zoom = m_voyageHome.DepartureZoom * (1-t);
        m_inertia.Active = false;
    }

    if (m_inertia.Active) {
        m_inertia.RadiansPerSecond -= 0.00001f * microseconds;

        if (m_inertia.RadiansPerSecond < 0) {
            m_radiansPerSecond = 0;
        } else {
            VQuat q = VQuat::rotation(m_inertia.RadiansPerSecond * microseconds * 0.000001f, m_inertia.Axis);
            m_quat = rotate(q, m_quat);
        }

        m_inertia.DistancePerSecond *= 0.75f;

        if (fabs(m_inertia.DistancePerSecond) < 0.0001f) {
            m_distancePerSecond = 0.0f;
        } else {
            m_zoom += m_distancePerSecond * 0.001f;
        }

        if (fabs(m_inertia.DistancePerSecond) < 0.0001f && m_inertia.RadiansPerSecond < 0.0f)
            m_inertia.Active = false;
    }
}

void Trackball::ReturnHome()
{
    m_voyageHome.Active = true;
    m_voyageHome.DepartureQuat = m_quat;
    m_voyageHome.DepartureZoom = m_zoom;
    m_voyageHome.microseconds = 0;
}

float Trackball::GetZoom() const
{
    return m_zoom;
}

//ITrackball* CreateTrackball(float width, float height, float radius)
//{
//    return new Trackball(width, height, radius);
//}
