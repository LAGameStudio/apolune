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

#include "VBO_V.h"
#include "VBO_Vw.h"
#include "VBO_VT.h"
#include "VBO_VC.h"
#include "VBO_VCLines.h"
#include "VBO_VN.h"
#include "VBO_VNC.h"
#include "VBO_VNT.h"
#include "VBO_VNTC.h"
#include "VBO_VNTCTan.h"
#include "VBO_VNTTan.h"
#include "VBO_VNTTanBi.h"

#include "Dynamic_VBO_VC.h"

template<class VBOTYPE>
class ModernVBOs : public LinkedList {
public:
 CLEARLISTRESET(ModernVBO<VBOTYPE>);
};

class VBOStaticVCs : public ModernVBOs<VBOStaticVC> {};
