#include "VBO.h"

// Basic VBOs for fast drawing of quads used by MapObject, Art.h, FBO etc

VBOStaticVT
 vboQuad,
 vboQuadUpsideDown,
 vboQuadReversed,
 vboQuadReversedUpsideDown,
 vboQuad5x5;
VBOStaticVNT vboQuadVNT;
VBOStaticVNTTan vboQuadVNTTan;
VBOStaticVNTTanBi vboQuadVNTTanBi;
VBOStaticVT vbo_QuadXInverted,vbo_QuadYInverted,vbo_QuadXYInverted;

void InitBasicVBOs() {
 OUTPUT("Creating basic quad VBOs:\n-----------------------------");

 Vertex a,b,c,d;

 OUTPUT("FBO related quads:\n");
 a.x=1.0f; a.y=0.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x=0.0f; b.y=0.0f; b.z=0.0f; b.q=1.0f; b.r=0.0f;
 c.x=0.0f; c.y=1.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=1.0f; d.y=1.0f; d.z=0.0f; d.q=0.0f; d.r=1.0f;
 vbo_QuadXInverted.PushTriangle(&a,&b,&c);
 vbo_QuadXInverted.PushTriangle(&a,&c,&d);
 vbo_QuadXInverted.Create();

 a.x=0.0f; a.y=1.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x=0.0f; b.y=1.0f; b.z=0.0f; b.q=1.0f; b.r=0.0f;
 c.x=1.0f; c.y=0.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=1.0f; d.y=0.0f; d.z=0.0f; d.q=0.0f; d.r=1.0f;
 vbo_QuadYInverted.PushTriangle(&a,&b,&c);
 vbo_QuadYInverted.PushTriangle(&a,&c,&d);
 vbo_QuadYInverted.Create();

 a.x=1.0f; a.y=1.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x=0.0f; b.y=1.0f; b.z=0.0f; b.q=1.0f; b.r=0.0f;
 c.x=0.0f; c.y=0.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=1.0f; d.y=0.0f; d.z=0.0f; d.q=0.0f; d.r=1.0f;
 vbo_QuadXYInverted.PushTriangle(&a,&b,&c);
 vbo_QuadXYInverted.PushTriangle(&a,&c,&d);
 vbo_QuadXYInverted.Create();
 
 OUTPUT("QuadVT2 0->1\n _original_\n");
 a.x=0.0f; a.y=0.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x=1.0f; b.y=0.0f; b.z=0.0f; b.q=1.0f; b.r=0.0f;
 c.x=1.0f; c.y=1.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=0.0f; d.y=1.0f; d.z=0.0f; d.q=0.0f; d.r=1.0f;
 vboQuad.PushTriangle(&a,&b,&c);
 vboQuad.PushTriangle(&a,&c,&d);
 vboQuad.Create();

 OUTPUT(" _upside down_\n");
 a.x=0.0f; a.y=0.0f; a.z=0.0f; a.q=0.0f; a.r=1.0f;
 b.x=1.0f; b.y=0.0f; b.z=0.0f; b.q=1.0f; b.r=1.0f;
 c.x=1.0f; c.y=1.0f; c.z=0.0f; c.q=1.0f; c.r=0.0f;
 d.x=0.0f; d.y=1.0f; d.z=0.0f; d.q=0.0f; d.r=0.0f;
 vboQuadUpsideDown.PushTriangle(&a,&b,&c);
 vboQuadUpsideDown.PushTriangle(&a,&c,&d);
 vboQuadUpsideDown.Create();

 OUTPUT(" _reversed_\n");
 a.x=0.0f; a.y=0.0f; a.z=0.0f; a.q=1.0f; a.r=0.0f;
 b.x=1.0f; b.y=0.0f; b.z=0.0f; b.q=0.0f; b.r=0.0f;
 c.x=1.0f; c.y=1.0f; c.z=0.0f; c.q=0.0f; c.r=1.0f;
 d.x=0.0f; d.y=1.0f; d.z=0.0f; d.q=1.0f; d.r=1.0f;
 vboQuadReversed.PushTriangle(&a,&b,&c);
 vboQuadReversed.PushTriangle(&a,&c,&d);
 vboQuadReversed.Create();

 OUTPUT(" _reversed and upside down\n");
 a.x=0.0f; a.y=0.0f; a.z=0.0f; a.q=1.0f; a.r=1.0f;
 b.x=1.0f; b.y=0.0f; b.z=0.0f; b.q=0.0f; b.r=1.0f;
 c.x=1.0f; c.y=1.0f; c.z=0.0f; c.q=0.0f; c.r=0.0f;
 d.x=0.0f; d.y=1.0f; d.z=0.0f; d.q=1.0f; d.r=0.0f;
 vboQuadReversedUpsideDown.PushTriangle(&a,&b,&c);
 vboQuadReversedUpsideDown.PushTriangle(&a,&c,&d);
 vboQuadReversedUpsideDown.Create();

 OUTPUT("Quad 5x5:\n _original_\n");
 a.x=-5.0f; a.y=-5.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x= 5.0f; b.y=-5.0f; b.z=0.0f; b.q=0.0f; b.r=1.0f;
 c.x= 5.0f; c.y= 5.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=-5.0f; d.y= 5.0f; d.z=0.0f; d.q=1.0f; d.r=0.0f;
 vboQuad5x5.PushTriangle(&a,&b,&c);
 vboQuad5x5.PushTriangle(&a,&c,&d);
 vboQuad5x5.Create();

 OUTPUT("QuadVNT 0->1\n"); 
 a.x=0.0f; a.y=0.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x=1.0f; b.y=0.0f; b.z=0.0f; b.q=1.0f; b.r=0.0f;
 c.x=1.0f; c.y=1.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=0.0f; d.y=1.0f; d.z=0.0f; d.q=0.0f; d.r=1.0f;
 vboQuadVNT.PushTriangle(&a,&b,&c);
 vboQuadVNT.PushTriangle(&a,&c,&d);
 vboQuadVNT.Create();

 OUTPUT("QuadVNTTan 0->1\n"); 
 a.x=0.0f; a.y=0.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x=1.0f; b.y=0.0f; b.z=0.0f; b.q=1.0f; b.r=0.0f;
 c.x=1.0f; c.y=1.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=0.0f; d.y=1.0f; d.z=0.0f; d.q=0.0f; d.r=1.0f;
 vboQuadVNTTan.PushTriangle(&a,&b,&c);
 vboQuadVNTTan.PushTriangle(&a,&c,&d);
 vboQuadVNTTan.Create();

 OUTPUT("QuadVNTTanBi 0->1\n"); 
 a.x=0.0f; a.y=0.0f; a.z=0.0f; a.q=0.0f; a.r=0.0f;
 b.x=1.0f; b.y=0.0f; b.z=0.0f; b.q=1.0f; b.r=0.0f;
 c.x=1.0f; c.y=1.0f; c.z=0.0f; c.q=1.0f; c.r=1.0f;
 d.x=0.0f; d.y=1.0f; d.z=0.0f; d.q=0.0f; d.r=1.0f;
 a.normal.Recycle(a.TriNormal(&a,&b,&c));
 a.TangentBasis(&b,&c);
 b.normal.Recycle(a.TriNormal(&a,&b,&c));
 b.TangentBasis(&a,&c);
 c.normal.Recycle(a.TriNormal(&a,&b,&c));
 c.TangentBasis(&a,&b);
 vboQuadVNTTanBi.PushTriangle(&a,&b,&c);
 a.normal.Recycle(a.TriNormal(&a,&c,&d));
 a.TangentBasis(&c,&d);
 c.normal.Recycle(a.TriNormal(&a,&c,&d));
 c.TangentBasis(&a,&d);
 d.normal.Recycle(a.TriNormal(&a,&c,&d));
 d.TangentBasis(&a,&c);
 vboQuadVNTTanBi.PushTriangle(&a,&c,&d);
 vboQuadVNTTanBi.Create();

 OUTPUT("------------------------- end InitBasicVBOs\n");
}