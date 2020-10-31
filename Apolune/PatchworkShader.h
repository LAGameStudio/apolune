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
#include "Art.h"
#include "GLSL2Shader.h"

/*
 * To keep VBO class requirements simple,
 * -all normalized patchwork shaders require VBOStaticVNCTTanBi or VBODynamicVNCTTanBi
 * -otherwise, patchwork shaders require VBOStaticVTC or VBODynamicVTC
 *
 * These basic types are static:
 *
 * Without normal maps:
 * PatchworkShader: Takes a sprite sheet as input and renders to the geometry using GLMCamera.
 * This is not implemented because you could use PhongShader or any other shading technique,
 * with a properly constructed (static or dynamic) grid VBO and compatible shader.
 * PatchworkInterp2Shader: Takes 2 sprite sheets and interpolates based on vertex color.
 * PatchworkInterp8Shader: Takes 8 sprite sheets and evaluates using branching
 * to pick based on vertex color "rgba" 1->5 2->6
 * PatchworkInterp4Shader: Takes 4 sprite sheets and evaluates an interpolation
 * based on the rgba value of the vertex color, allowing you to mix
 * up to 4 spritesheets that have layered detail shaders where each
 * cell/tile is a slight redux.
 * Patchwork10Shader: Takes 10 sprite sheets and evaluates using branching
 * to pick based on vertex color "r" (no interpolation) r 0.1,0.2...1.0 is the picker
 *
 * With normal maps:
 * PatchworkNormalShader: Takes 1 sprite sheet and its normal sheet or the output
 * of the normal mapping shader and renders with 1 light.
 * Patchwork2Normal: Similar to PatchWork2Shader except it has normal maps in 2 channels.
 * Patchwork4Normal: Similar to PatchWork4Shader except it has normal maps in 4 channels.
 *
 * The types are repeated with the Dynamic prefix for vbo dynamism.
 */

class PatchworkShader : public GLSLShader {
public:
 void Init() {
 }
};

class PatchworkNormalShader : public GLSLShader {
public:
};