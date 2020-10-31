#pragma once

#include "GLSetup.h"

enum VBOPrecisionTypes {
 vbo_int=0,
 vbo_float=1,
 vbo_double=2
};

enum VBOStreamTypes {
 vbo_V=0,
 vbo_Vw=1,
 vbo_VN=2,
 vbo_VC=3,
 vbo_VNC=4,
 vbo_VNCT=5
};

class NewVBO {
public:
 Zbool dynamic;
 VBOPrecisionTypes precision;
 NewVBO() {
 }
 ~NewVBO() {
 }
};

