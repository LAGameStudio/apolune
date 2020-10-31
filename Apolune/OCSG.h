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

#if defined(USE_OPENCSG)

#include <OpenCSG.h>

class OCSG {
public:
 OCSG() {
  Automatic();
  OffscreenFBO();
 }
 // Activates "automatic algorithm selection" mode
 void Automatic() {
  OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Automatic);
 }
 // Activates "standard goldfeather"
 void GoldfeatherDepthComplexity() {
  OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Goldfeather);
  OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::DepthComplexitySampling);
 }
 void GoldfeatherNoDepthComplexity() {
  OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Goldfeather);
  OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::NoDepthComplexitySampling);
 }
 void GoldfeatherOcclusionQuery() {
  OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Goldfeather);
  OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::OcclusionQuery);
 }
 // Activates "standard scs"
 void SCSNoDepthComplexity() {
  OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::SCS);
  OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::NoDepthComplexitySampling);
 }
 void SCSDepthComplexity() {
  OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::SCS);
  OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::DepthComplexitySampling);
 }
 void SCSOcclusionQuery() {
  OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::SCS);
  OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::OcclusionQuery);
 }
 void OffscreenAutomatic() {
  OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::AutomaticOffscreenType);
 }
 void OffscreenFBO() {
  OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::FrameBufferObject);
 }
 void OffscreenPBuffer() {
  OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::PBuffer);
 }
 /* Optimizer settings */
 void OptimizationDefault() {
 }
 void OptimizationForceOn() {
 }
 void OptimizationOn() {
 }
 void OptimizationOff() {
 }
 void OptimizationUnused() {
 }
};

extern OCSG ocsg;

#endif