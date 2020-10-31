#include "STLFile.h"

const char STLHeader[80] = {
   0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
   0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
   0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
   0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0
};

#include "VBOGroup.h"

void STLFacets::PushTri( VBONormalsPolysGroup *vbos, Vertexd *a, Vertexd *b, Vertexd *c ) {
 Crayon color;
 mPolygon tri;
 Vertex normalVector;
 Vertex *A,*B,*C;
 color.Pick(alabaster); color.ScaleByAlphaf( 0.5f+0.5f*(float)(b->z/extents.z));
 B=tri.points.Add(b->x,b->y,b->z); /*B->Color(0.0,1.0,1.0,1.0);*/ B->ColorAbs(color.rf,color.gf,color.bf,1.0); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
 color.Pick(alabaster); color.ScaleByAlphaf( 0.5f+0.5f*(float)(c->z/extents.z));
 C=tri.points.Add(c->x,c->y,c->z); /*C->Color(1.0,0.0,1.0,1.0);*/ C->ColorAbs(color.rf,color.gf,color.bf,1.0); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
 color.Pick(alabaster); color.ScaleByAlphaf( 0.5f+0.5f*(float)(a->z/extents.z));
 A=tri.points.Add(a->x,a->y,a->z); /*A->Color(1.0,0.0,0.0,1.0);*/ A->ColorAbs(color.rf,color.gf,color.bf,1.0); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
//  vbo.PushTriangle(&tri);
 vbos->PushTri(&tri);
}

void STLFacets::PushTri( VBONormalsPolysGroup *vbos, Vertexd *a, Vertexd *b, Vertexd *c, Crayon tint ) {
 Crayon color;
 mPolygon tri;
 Vertex normalVector;
 Vertex *A,*B,*C;
 color.Pick(alabaster); color.ScaleByAlphaf( 0.5f+0.5f*(float)(b->z/extents.z));
 B=tri.points.Add(b->x,b->y,b->z); B->ColorAbs(color.rf*tint.rf,color.gf*tint.gf,color.bf*tint.bf,1.0f*tint.af); B->q=1.0; B->r=0.0; //B->Normal();// B->normal->Invert();
 color.Pick(alabaster); color.ScaleByAlphaf( 0.5f+0.5f*(float)(c->z/extents.z));
 C=tri.points.Add(c->x,c->y,c->z); C->ColorAbs(color.rf*tint.rf,color.gf*tint.gf,color.bf*tint.bf,1.0f*tint.af); C->q=1.0; C->r=0.0; //C->Normal();// C->normal->Invert();
 color.Pick(alabaster); color.ScaleByAlphaf( 0.5f+0.5f*(float)(a->z/extents.z));
 A=tri.points.Add(a->x,a->y,a->z); A->ColorAbs(color.rf*tint.rf,color.gf*tint.gf,color.bf*tint.bf,1.0f*tint.af); A->q=0.0; A->r=0.0; //A->Normal();// A->normal->Invert();
//  vbo.PushTriangle(&tri);
 vbos->PushTri(&tri);
}

void STLFacets::toVBOGroup( VBONormalsPolysGroup *vbos, bool reverseWinding ) {
 OUTPUT("STLFacets: Calculating extents.\n");
 CalculateExtents();
 OUTPUT("STLFacets: Preparing VBO Group...");
 vbos->Clear();
 if ( reverseWinding ) FOREACH(STLFacet,facet) PushTri( vbos, &(facet->a), &(facet->c), &(facet->b) );
 else FOREACH(STLFacet,facet) PushTri( vbos, &(facet->a), &(facet->b), &(facet->c) );
 vbos->Create();
 OUTPUT("ready.\n");
}

void STLFacets::toVBOGroup( VBONormalsPolysGroup *vbos, bool reverseWinding, Crayon tint ) {
 OUTPUT("STLFacets: Calculating extents.\n");
 CalculateExtents();
 OUTPUT("STLFacets: Preparing VBO Group...");
 vbos->Clear();
 if ( reverseWinding ) FOREACH(STLFacet,facet) PushTri( vbos, &(facet->a), &(facet->c), &(facet->b), tint );
 else FOREACH(STLFacet,facet) PushTri( vbos, &(facet->a), &(facet->b), &(facet->c), tint );
 vbos->Create();
 OUTPUT("ready.\n");
}

/*
 * Renders the STL with a coordinator and grid in a window using an FBO
 * FBO has a clear background, at the 3rd person perspective angle
 */
#include "FBO.h"

void STLFile::VBOGroupRender( LookAtPerspective *lap, Cartesiand *position, Cartesiand *rotation, double x, double y, double w, double h ) {
 FBOColorDepth *fbo=&fboScreen1;
 Cartesiand offset;
 offset.x=-facets.extents.x/2.0;
 offset.z=-facets.extents.z/2.0;

 fbo->background.Pick(clear);
 fbo->Bind();

  Blending(none);
 //glPush();
  lap->Apply();
  glTranslatef((GLfloat) position->x, (GLfloat)position->y, (GLfloat) position->z );
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();///
  glEnable(GL_DEPTH_TEST);
  glClear(GL_DEPTH_BUFFER_BIT);
   glDepthRange(0.001,1.0);
///   glDepthRange(1.0,0.001);
//   glDepthFunc(GL_GREATER);
   glDepthFunc(GL_LEQUAL);
   glRotatef((GLfloat) rotation->x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation->y, 0.0, 0.0, 1.0);
   glColor3d(0.65,0.1,0.65);
//   glDisable(GL_CULL_FACE);
//   glEnable(GL_CULL_FACE);//Enable(GL_CULL_FACE);
///   glCullFace(GL_BACK);
   glPolygonMode(GL_FRONT,GL_FILL);//_AND_BACK,GL_FILL);
   glEnable(GL_POLYGON_SMOOTH);
//   glTranslatef((GLfloat) offset.x, (GLfloat) offset.y, (GLfloat) offset.z);
   if ( vboGroup ) vboGroup->Render();
  glPopMatrix();
  glPushMatrix();///
   glRotatef((GLfloat) rotation->x, 1.0, 0.0, 0.0);
   glRotatef((GLfloat) rotation->y, 0.0, 0.0, 1.0);
   glLineWidth(4.2f);
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) (facets.extents.x*0.7), 0.0f,1.0f);
    Text("RIGHT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef((GLfloat) -(facets.extents.x*0.7), 0.0f,1.0f);
    Text("LEFT",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) -(facets.extents.y*0.7), 1.0f);
    Text("BACK",0,0,2,3,true,0);
   glPopMatrix();
   glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, (GLfloat) (facets.extents.y*0.7), 1.0);
    Text("FRONT",0,0,2,3,true,0);
   glPopMatrix();
   glLineWidth(2.2f);
   glPushMatrix();//
    glTranslatef((GLfloat) -(facets.extents.x/2)-10.0f, (GLfloat) -(facets.extents.y*0.5), 0.0f);
    GLfloat cm=0.0f;
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("%-2d cm _____",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > facets.extents.y ) break;
    }
   glPopMatrix();//
   glPushMatrix();//
    glTranslatef( (GLfloat) -(facets.extents.x*0.5), (GLfloat) -(facets.extents.y*0.5), 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f );
    for ( unsigned int t=0; t<30; t++ ) {
     cm=t*10.0f;
     glPushMatrix();
      glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
      glTranslatef(0.0f, -cm, 0.0f);
      Text(FORMAT("____ %-2d cm",(int)t).c_str(),0,0,1,1.6,false,0);
     glPopMatrix();
     if ( cm > facets.extents.x ) break;
    }
   glPopMatrix();//
   glDisable(GL_DEPTH_TEST);
   Coordinator();
  glPopMatrix();///
  //glPop();

 fbo->Unbind();
 fbo->RenderfUpsideDown((float)x,(float)y,(float)(x+w),(float)(y+h));
}