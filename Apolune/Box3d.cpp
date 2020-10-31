#include "Box3d.h"

#include "VBOGroup.h"

Box3df unitCube(true,true,1.0f);
VBONormalsPolysGroup unitCubeVBOGroup;

/*    e+-----+f
 *    /     /|
 *  a+-----+b+g
 *   | h   | /
 *  d+-----+c
 */
void Box3df::toVBOGroup( VBONormalsPolysGroup *vbos, float alpha, Crayon tintA, Crayon tintB, Crayon tintC, Crayon tintD/*, Crayon tintE, Crayon tintF, Crayon tintG, Crayon tintH */
   ) {
 vbos->Clear();
 // Generate a cube based on the box described as x,y,z by w,h,d
 Vertexd A,B,C,D,E,F,G,H;
 A.Set(x,y,z);
 B.Set(x+w,y,z);
 C.Set(x+w,y+h,z);
 D.Set(x,y+h,z);
 E.Set(x,y,z+d);
 F.Set(x+w,y,z+d);
 G.Set(x+w,y+h,z+d);
 H.Set(x,y+h,z+d);

 tintA.Alpha(alpha);
 tintB.Alpha(alpha);
 tintC.Alpha(alpha);
 tintD.Alpha(alpha);

 // Quad ABCD Fore
 vbos->PushQuad(&A,&tintA,&B,&tintB,&C,&tintC,&D,&tintD);

 // Quad BFGC Right
 vbos->PushQuad(&B,&tintB,&F,&tintB,&G,&tintC,&C,&tintC);

 // Quad FEHG Back
 vbos->PushQuad(&F,&tintD,&E,&tintC,&H,&tintB,&G,&tintA);

 // Quad EADH Left
 vbos->PushQuad(&E,&tintA,&A,&tintB,&D,&tintC,&H,&tintD);

 // Quad EFBA Top
 vbos->PushQuad(&E,&tintA,&F,&tintB,&B,&tintB,&A,&tintA);

 // Quad ECHG Bottom
 vbos->PushQuad(&C,&tintC,&D,&tintD,&H,&tintD,&G,&tintC);
 vbos->CreateClear();
}


void Box3df::toVBOGroup( VBONormalsPolysGroup *vbos ) {
 vbos->Clear();
 // Generate a cube based on the box described as x,y,z by w,h,d
 Vertexd A,B,C,D,E,F,G,H;
 A.Set(x,y,z);
 B.Set(x+w,y,z);
 C.Set(x+w,y+h,z);
 D.Set(x,y+h,z);
 E.Set(x,y,z+d);
 F.Set(x+w,y,z+d);
 G.Set(x+w,y+h,z+d);
 H.Set(x,y+h,z+d);

 Crayon tintA(red255);
 Crayon tintB(green255);
 Crayon tintC(blue255);
 Crayon tintD(gold);

 // Quad ABCD Fore
 vbos->PushQuad(&A,&tintA,&B,&tintB,&C,&tintC,&D,&tintD);

 // Quad BFGC Right
 vbos->PushQuad(&B,&tintB,&F,&tintB,&G,&tintC,&C,&tintC);

 // Quad EFHG Back
 vbos->PushQuad(&E,&tintD,&F,&tintC,&G,&tintB,&H,&tintA);

 // Quad EADH Left
 vbos->PushQuad(&E,&tintA,&A,&tintB,&D,&tintC,&H,&tintD);

 // Quad EFBA Top
 vbos->PushQuad(&E,&tintA,&F,&tintB,&B,&tintB,&A,&tintA);

 // Quad ECHG Bottom
 vbos->PushQuad(&C,&tintC,&D,&tintD,&H,&tintD,&G,&tintC);
 vbos->CreateClear();
}


#include "Model.h"

void Box3df::toModel( ImmediateModel *out, float alpha, Crayon tintA, Crayon tintB, Crayon tintC, Crayon tintD/*, Crayon tintE, Crayon tintF, Crayon tintG, Crayon tintH */
   ) {
 out->Clear();
 // Generate a cube based on the box described as x,y,z by w,h,d
 Vertexd A,B,C,D,E,F,G,H;
 A.Set(x,y,z);
 B.Set(x+w,y,z);
 C.Set(x+w,y+h,z);
 D.Set(x,y+h,z);
 E.Set(x,y,z+d);
 F.Set(x+w,y,z+d);
 G.Set(x+w,y+h,z+d);
 H.Set(x,y+h,z+d);

 tintA.Alpha(alpha);
 tintB.Alpha(alpha);
 tintC.Alpha(alpha);
 tintD.Alpha(alpha);

 // Quad ABCD Fore
 out->PushQuad(&A,&tintA,&B,&tintB,&C,&tintC,&D,&tintD);

 // Quad BFGC Right
 out->PushQuad(&B,&tintB,&F,&tintB,&G,&tintC,&C,&tintC);

 // Quad EFHG Back
 out->PushQuad(&E,&tintD,&F,&tintC,&G,&tintB,&H,&tintA);

 // Quad EADH Left
 out->PushQuad(&E,&tintA,&A,&tintB,&D,&tintC,&H,&tintD);

 // Quad EFBA Top
 out->PushQuad(&E,&tintA,&F,&tintB,&B,&tintB,&A,&tintA);

 // Quad ECHG Bottom
 out->PushQuad(&C,&tintC,&D,&tintD,&H,&tintD,&G,&tintC);
}


void Box3df::toLines( ImmediateModel *out, float alpha, Crayon tintA, Crayon tintB, Crayon tintC, Crayon tintD/*, Crayon tintE, Crayon tintF, Crayon tintG, Crayon tintH */
   ) {
 out->Clear();
 // Generate a cube based on the box described as x,y,z by w,h,d
 Vertexd A,B,C,D,E,F,G,H;
 A.Set(x,y,z);
 B.Set(x+w,y,z);
 C.Set(x+w,y+h,z);
 D.Set(x,y+h,z);
 E.Set(x,y,z+d);
 F.Set(x+w,y,z+d);
 G.Set(x+w,y+h,z+d);
 H.Set(x,y+h,z+d);

 tintA.Alpha(alpha);
 tintB.Alpha(alpha);
 tintC.Alpha(alpha);
 tintD.Alpha(alpha);

 // Quad ABCD Fore
 out->PushLine(&A,&tintA,&B,&tintB);
 out->PushLine(&B,&tintB,&C,&tintC);
 out->PushLine(&C,&tintC,&D,&tintD);
 out->PushLine(&D,&tintD,&A,&tintA);

  // Quad EFHG Back
 out->PushLine(&E,&tintA,&F,&tintB);
 out->PushLine(&F,&tintB,&G,&tintC);
 out->PushLine(&G,&tintC,&H,&tintD);
 out->PushLine(&H,&tintD,&E,&tintA);

 // Quad BFGC Right
 out->PushLine(&B,&tintB,&F,&tintB);
 out->PushLine(&G,&tintC,&C,&tintC);

 // Quad EADH Left
 out->PushLine(&E,&tintA,&A,&tintA);
 out->PushLine(&D,&tintD,&H,&tintD);

}