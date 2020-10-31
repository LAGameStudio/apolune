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
