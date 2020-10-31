#include "Fluid2d.h"
#include "glsw.h"

short Fpositions[] = {
    -1, -1,
     1, -1,
    -1,  1,
     1,  1,
};

void Fluid2d::PezCheckCondition(int condition, char *fmt, ...)
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

struct ProgramsRec {
    GLuint Advect;
    GLuint Jacobi;
    GLuint SubtractGradient;
    GLuint ComputeDivergence;
    GLuint ApplyImpulse;
    GLuint ApplyBuoyancy;
} Programs;

void Fluid2d::ResetState()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
    glViewport(0,0,display.w,display.h);
}

GLuint Fluid2d::CreateQuad()
{
    // Create the VAO:
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create the VBO:
    GLuint vbo;
    GLsizei size = sizeof(Fpositions);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, Fpositions, GL_STATIC_DRAW);

    // Set up the vertex layout:
    GLsizei stride = 2 * sizeof(Fpositions[0]);
    glEnableVertexAttribArray(PositionSlot);
    glVertexAttribPointer(PositionSlot, 2, GL_SHORT, GL_FALSE, stride, 0);

    return vao;
}

GLuint Fluid2d::CreateProgram(const char* vsKey, const char* gsKey, const char* fsKey)
{   
    const char* vsSource = glswGetShader(vsKey);
    const char* gsSource = glswGetShader(gsKey);
    const char* fsSource = glswGetShader(fsKey);

    char* msg = "Can't find %s shader: '%s'.\n";
    PezCheckCondition(vsSource != 0, msg, "vertex", (char *) vsKey);
    PezCheckCondition(gsKey == 0 || gsSource != 0, msg, "geometry", (char *) gsKey);
    PezCheckCondition(fsKey == 0 || fsSource != 0, msg, "fragment", (char *) fsKey);
    
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
    
    return programHandle;
}


FSlab Fluid2d::CreateSlab(GLsizei width, GLsizei height, int numComponents)
{
    FSlab slab;
    slab.Ping = CreateSurface(width, height, numComponents);
    slab.Pong = CreateSurface(width, height, numComponents);
    return slab;
}

FSurface Fluid2d::CreateSurface(GLsizei width, GLsizei height, int numComponents)
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

    const int UseHalfFloats = 1;
    if (UseHalfFloats) {
        switch (numComponents) {
            case 1: glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_HALF_FLOAT, 0); break;
            case 2: glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_HALF_FLOAT, 0); break;
            case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_HALF_FLOAT, 0); break;
            case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, 0); break;
            default: OUTPUT("Illegal slab format.\n");
        }
    } else {
        switch (numComponents) {
            case 1: glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0); break;
            case 2: glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, 0); break;
            case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0); break;
            case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0); break;
            default: OUTPUT("Illegal slab format.\n");
        }
    }

    PezCheckCondition(GL_NO_ERROR == glGetError(), "Unable to create normals texture");

    GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
    PezCheckCondition(GL_NO_ERROR == glGetError(), "Unable to attach color buffer");
    
    PezCheckCondition(GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER), "Unable to create FBO.");
    FSurface surface = { fboHandle, textureHandle, numComponents };

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OUTPUT("Fluid2d::CreateSurface created fbo %d texid %d colorbuffer %d components\n",
     (int)fboHandle, (int)textureHandle, (int)colorbuffer, (int)numComponents );
    return surface;
}

void Fluid2d::CreateObstacles(FSurface dest, int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint program = CreateProgram("Fluid.Vertex", 0, "Fluid.Fill");
    glUseProgram(program);

    const int DrawBorder = 1;
    if (DrawBorder) {
        #define T 0.9999f
        float positions[] = { -T, -T, T, -T, T,  T, -T,  T, -T, -T };
        #undef T
        GLuint vbo;
        GLsizei size = sizeof(positions);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
        GLsizei stride = 2 * sizeof(positions[0]);
        glEnableVertexAttribArray(PositionSlot);
        glVertexAttribPointer(PositionSlot, 2, GL_FLOAT, GL_FALSE, stride, 0);
        glDrawArrays(GL_LINE_STRIP, 0, 5);
        glDeleteBuffers(1, &vbo);
    }

    const int DrawCircle = 1;
    if (DrawCircle) {
        const int slices = 64;
        float positions[slices*2*3];
        float twopi = 8*atan(1.0f);
        float theta = 0;
        float dtheta = twopi / (float) (slices - 1);
        float* pPositions = &positions[0];
        for (int i = 0; i < slices; i++) {
            *pPositions++ = 0;
            *pPositions++ = 0;

            *pPositions++ = 0.25f * cos(theta) * height / width;
            *pPositions++ = 0.25f * sin(theta);
            theta += dtheta;

            *pPositions++ = 0.25f * cos(theta) * height / width;
            *pPositions++ = 0.25f * sin(theta);
        }
        GLuint vbo;
        GLsizei size = sizeof(positions);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
        GLsizei stride = 2 * sizeof(positions[0]);
        glEnableVertexAttribArray(PositionSlot);
        glVertexAttribPointer(PositionSlot, 2, GL_FLOAT, GL_FALSE, stride, 0);
        glDrawArrays(GL_TRIANGLES, 0, slices * 3);
        glDeleteBuffers(1, &vbo);
    }

    // Cleanup
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);

}

void Fluid2d::InitSlabOps()
{
    Programs.Advect = CreateProgram("Fluid2d.Vertex", 0, "Fluid2d.Advect");
    Programs.Jacobi = CreateProgram("Fluid2d.Vertex", 0, "Fluid2d.Jacobi");
    Programs.SubtractGradient = CreateProgram("Fluid2d.Vertex", 0, "Fluid2d.SubtractGradient");
    Programs.ComputeDivergence = CreateProgram("Fluid2d.Vertex", 0, "Fluid2d.ComputeDivergence");
    Programs.ApplyImpulse = CreateProgram("Fluid2d.Vertex", 0, "Fluid2d.Splat");
    Programs.ApplyBuoyancy = CreateProgram("Fluid2d.Vertex", 0, "Fluid2d.Buoyancy");
}

void Fluid2d::SwapSurfaces(FSlab* slab)
{
    FSurface temp = slab->Ping;
    slab->Ping = slab->Pong;
    slab->Pong = temp;
}

void Fluid2d::ClearSurface(FSurface s, float v)
{
    glBindFramebuffer(GL_FRAMEBUFFER, s.FboHandle);
    glClearColor(v, v, v, v);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Fluid2d::Advect(FSurface velocity, FSurface source, FSurface obstacles, FSurface dest, float dissipation)
{
    GLuint p = Programs.Advect;
    glUseProgram(p);

    GLint inverseSize = glGetUniformLocation(p, "InverseSize");
    GLint timeStep = glGetUniformLocation(p, "TimeStep");
    GLint dissLoc = glGetUniformLocation(p, "Dissipation");
    GLint sourceTexture = glGetUniformLocation(p, "SourceTexture");
    GLint obstaclesTexture = glGetUniformLocation(p, "Obstacles");

    glUniform2f(inverseSize, 1.0f / GridWidth, 1.0f / GridHeight);
    glUniform1f(timeStep, TimeStep);
    glUniform1f(dissLoc, dissipation);
    glUniform1i(sourceTexture, 1);
    glUniform1i(obstaclesTexture, 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, source.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obstacles.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void Fluid2d::Jacobi(FSurface pressure, FSurface divergence, FSurface obstacles, FSurface dest)
{
    GLuint p = Programs.Jacobi;
    glUseProgram(p);

    GLint alpha = glGetUniformLocation(p, "Alpha");
    GLint inverseBeta = glGetUniformLocation(p, "InverseBeta");
    GLint dSampler = glGetUniformLocation(p, "Divergence");
    GLint oSampler = glGetUniformLocation(p, "Obstacles");

    glUniform1f(alpha, -CellSize * CellSize);
    glUniform1f(inverseBeta, 0.25f);
    glUniform1i(dSampler, 1);
    glUniform1i(oSampler, 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pressure.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, divergence.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obstacles.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void Fluid2d::SubtractGradient(FSurface velocity, FSurface pressure, FSurface obstacles, FSurface dest)
{
    GLuint p = Programs.SubtractGradient;
    glUseProgram(p);

    GLint gradientScale = glGetUniformLocation(p, "GradientScale");
    glUniform1f(gradientScale, GradientScale);
    GLint halfCell = glGetUniformLocation(p, "HalfInverseCellSize");
    glUniform1f(halfCell, 0.5f / CellSize);
    GLint sampler = glGetUniformLocation(p, "Pressure");
    glUniform1i(sampler, 1);
    sampler = glGetUniformLocation(p, "Obstacles");
    glUniform1i(sampler, 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pressure.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, obstacles.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void Fluid2d::ComputeDivergence(FSurface velocity, FSurface obstacles, FSurface dest)
{
    GLuint p = Programs.ComputeDivergence;
    glUseProgram(p);

    GLint halfCell = glGetUniformLocation(p, "HalfInverseCellSize");
    glUniform1f(halfCell, 0.5f / CellSize);
    GLint sampler = glGetUniformLocation(p, "Obstacles");
    glUniform1i(sampler, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obstacles.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void Fluid2d::ApplyImpulse(FSurface dest, FVector2 position, float value)
{
    GLuint p = Programs.ApplyImpulse;
    glUseProgram(p);

    GLint pointLoc = glGetUniformLocation(p, "Point");
    GLint radiusLoc = glGetUniformLocation(p, "Radius");
    GLint fillColorLoc = glGetUniformLocation(p, "FillColor");

    glUniform2f(pointLoc, (float) position.X, (float) position.Y);
    glUniform1f(radiusLoc, SplatRadius);
    glUniform3f(fillColorLoc, value, value, value);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void Fluid2d::ApplyBuoyancy(FSurface velocity, FSurface temperature, FSurface density, FSurface dest)
{
    GLuint p = Programs.ApplyBuoyancy;
    glUseProgram(p);

    GLint tempSampler = glGetUniformLocation(p, "Temperature");
    GLint inkSampler = glGetUniformLocation(p, "Density");
    GLint ambTemp = glGetUniformLocation(p, "AmbientTemperature");
    GLint timeStep = glGetUniformLocation(p, "TimeStep");
    GLint sigma = glGetUniformLocation(p, "Sigma");
    GLint kappa = glGetUniformLocation(p, "Kappa");

    glUniform1i(tempSampler, 1);
    glUniform1i(inkSampler, 2);
    glUniform1f(ambTemp, AmbientTemperature);
    glUniform1f(timeStep, TimeStep);
    glUniform1f(sigma, SmokeBuoyancy);
    glUniform1f(kappa, SmokeWeight);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, temperature.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, density.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}
