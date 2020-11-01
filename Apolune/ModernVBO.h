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
#pragma warning (disable:4267) // conversion warning

#include <glm/glm.hpp>
#include "GLSetup.h"
#include "Crayon.h"

#include "VBOTriangles.h"

class PLY;
class ImmediateModel;
class VBO16;
class GLMCamera;


#if defined(BUFFER_OFFSET)
#undef BUFFER_OFFSET
#endif

// example was: #define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define BUFFER_OFFSET(bo_bytes)  (reinterpret_cast<const GLubyte *>(0) + bo_bytes)

#define preferred_type unsigned int
#define preferred_type_enum GL_UNSIGNED_INT

using namespace std;
#include <vector>

_ONE(MVBO,{
 vao=vbo=ibo=0;
 count=0;
 usage=GL_STATIC_DRAW;
 draw_method=GL_TRIANGLES;
 stride=0;
})
 size_t stride;
 GLsizei count;
 GLenum usage,draw_method;
 GLuint vao,vbo,ibo;
 virtual void Render() {}
_MANY(MVBO,MVBOs,{})
DONE(MVBO);

// similar to the VBO class, we allocate a
template <typename VBOTYPE>
class ModernVBO : public MVBO {
public:
 bool uploaded;
 vector<VBOTYPE> data;
 vector<preferred_type> indices;
 ModernVBO() : MVBO() {
  stride=sizeof(VBOTYPE);
  data.resize(0);
  indices.resize(0);
  uploaded=false;
  Constructor();
 }
 virtual void Constructor() {}
 
 // Binds the data to a GPU data stream, custom for each child
 virtual void BindToStreams() {}  virtual void UnbindFromStreams() {}
 virtual void BindToStreams2() {} virtual void UnbindFromStreams2() {}
 virtual void BindToStreams3() {} virtual void UnbindFromStreams3() {}
 virtual void BindToStreams4() {} virtual void UnbindFromStreams4() {}
 
 // Accessor function that imports and modify data for each child
 virtual void Precompute() {}
 virtual void Precompute( Vertex *a, Vertex *b, Vertex *c ) {}
 virtual void Precompute( Vertexd *a, Vertexd *b, Vertexd *c ) {}

 // Insert Vertex Data Formats (and index)
 virtual void Set( VBOTYPE *target, Vertex *source ) {}                                                // V (fully featured)
 virtual void Set( VBOTYPE *target, Vertex *source, Vertex *normal ) {}                                // V (fully featured)
 virtual void Get( VBOTYPE *target, Vertex *source, Crayon rgba ) {}                                   // VC (fully featured)
                      
 virtual void Set( VBOTYPE *target, Vertexd *source ) {}                                               // V
 virtual void Set( VBOTYPE *target, Vertexd *source, Crayon rgba ) {}                                  // VC
 virtual void Set( VBOTYPE *target, Vertexd *source, Vertexd *normal ) {}                              // VN
 virtual void Set( VBOTYPE *target, Vertexd *source, Vertexd *tc, bool isVT ) {}                       // VT
 virtual void Set( VBOTYPE *target, Vertexd *source, Vertexd *normal, Crayon rgba ) {}                 // VNC
 virtual void Set( VBOTYPE *target, Vertexd *source, Vertexd *normal, Vertexd *tc ) {}                 // VNT
 virtual void Set( VBOTYPE *target, Vertexd *source, Vertexd *normal, Vertexd *tc, Crayon rgba ) {}    // VNTC
 
 // Retreive Vertex Data Formats
 virtual void Get( VBOTYPE *target, Vertexd *source ) {}                                               // V
 virtual void Get( VBOTYPE *target, Vertexd *source, Crayon *out ) {}                                  // VC
 virtual void Get( VBOTYPE *target, Vertexd *source, Vertexd *normal ) {}                              // VN
 virtual void Get( VBOTYPE *target, Vertexd *source, Vertexd *tc, bool isVT ) {}                       // VT
 virtual void Get( VBOTYPE *target, Vertexd *source, Vertexd *normal, Crayon *out ) {}                 // VNC
 virtual void Get( VBOTYPE *target, Vertexd *source, Vertexd *normal, Vertexd *tc) {}                  // VNT
 virtual void Get( VBOTYPE *target, Vertexd *source, Vertexd *normal, Vertexd *tc, Crayon *out ) {}    // VNTC

 virtual void Postcompute( Vertex *a, Vertex *b, Vertex *c ) {}
 virtual void Postcompute( Vertexd *a, Vertexd *b, Vertexd *c ) {}
 virtual void Postcompute() {}

 virtual VBOTYPE *Get( int index, int point_a0_b1_c2 ) {
  return &data[index*3+point_a0_b1_c2];
 }

 virtual void Set( VBOTYPE *value, int index, int point_a0_b1_c2 ) {
  data[index*3+point_a0_b1_c2]=*value;
 }
 // Used by VBO_V / VBO_VC (for VBO-driven point sprites)
 virtual void PushVertex( Vertexd *a ) {
  int end=data.size();
  data.resize(end+1);
  indices.resize(end+1);
  Set(&data[end],a);
  indices[end]=end;
  count++;
 }
 // Used by VBO_V / VBO_VT2 / VBO_VC (for VBO-driven point sprites and quads)
 virtual void PushVertex( Vertex *a ) {
  int end=data.size();
  data.resize(end+1);
  indices.resize(end+1);
  Set(&data[end],a);
  indices[end]=end;
  count++;
 }
 
 virtual void PushTriangle( Vertex *a, Vertex *b, Vertex *c ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a);
  indices[end]=end;
  end++;
  Set(&data[end],b);
  indices[end]=end;
  end++;
  Set(&data[end],c);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertex *a, Vertex *b, Vertex *c, Vertex *n ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a,n);
  indices[end]=end;
  end++;
  Set(&data[end],b,n);
  indices[end]=end;
  end++;
  Set(&data[end],c,n);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertexd *a, Vertexd *b, Vertexd *c ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a);
  indices[end]=end;
  end++;
  Set(&data[end],b);
  indices[end]=end;
  end++;
  Set(&data[end],c);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertexd *a, Vertexd *b, Vertexd *c, Vertexd *n ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a,n);
  indices[end]=end;
  end++;
  Set(&data[end],b,n);
  indices[end]=end;
  end++;
  Set(&data[end],c,n);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertexd *a, Crayon cola, Vertexd *b, Crayon colb, Vertexd *c, Crayon colc ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a,cola);
  indices[end]=end;
  end++;
  Set(&data[end],b,colb);
  indices[end]=end;
  end++;
  Set(&data[end],c,colc);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertexd *a, Crayon cola, Vertexd *b, Crayon colb, Vertexd *c, Crayon colc, Vertexd *n ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a,n,cola);
  indices[end]=end;
  end++;
  Set(&data[end],b,n,colb);
  indices[end]=end;
  end++;
  Set(&data[end],c,n,colc);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertexd *a, Vertexd *tca, Vertexd *b, Vertexd *tcb, Vertexd *c, Vertexd *tcc ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a,tca);
  indices[end]=end;
  end++;
  Set(&data[end],b,tcb);
  indices[end]=end;
  end++;
  Set(&data[end],c,tcc);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertexd *a, Vertexd *tca, Vertexd *b, Vertexd *tcb, Vertexd *c, Vertexd *tcc, Vertexd *n ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a,n,tca);
  indices[end]=end;
  end++;
  Set(&data[end],b,n,tcb);
  indices[end]=end;
  end++;
  Set(&data[end],c,n,tcc);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }
 virtual void PushTriangle( Vertexd *a, Vertexd *tca, Crayon cola, Vertexd *b, Vertexd *tcb, Crayon colb,  Vertexd *c, Vertexd *tcc, Crayon colc, Vertexd *n ) {
  Precompute(a,b,c);
  int end=data.size();
  data.resize(end+3);
  indices.resize(end+3);
  Set(&data[end],a,n,tca,cola);
  indices[end]=end;
  end++;
  Set(&data[end],b,n,tcb,colb);
  indices[end]=end;
  end++;
  Set(&data[end],c,n,tcc,colc);
  indices[end]=end;
  Postcompute(a,b,c);
  count+=3;
 }

 // Pushes a single triangle 
 void PushTriangle( mPolygon *tri ) { 
  if ( tri->points.count < 3 ) {
   OUTPUT("ModernVBO::PushTriangle() reports not enough vertices in triangle.\n");
   return;
  }
  if ( tri->points.count > 3 ) {
   OUTPUT("ModernVBO::PushTriangle() reports too many vertices in triangle.\n");
   return;
  }
  Vertex *a=(Vertex *) tri->points.first,
         *b=(Vertex *) tri->points.first->next,
         *c=(Vertex *) tri->points.first->next->next;
  PushTriangle(a,b,c);
 }
 virtual void PushQuad( Vertex *a, Vertex *b, Vertex *c, Vertex *d ) {
  PushTriangle(b,c,a);
  PushTriangle(c,d,a);
 }
 /*
 void SetIndices( VBOTriangles *indexes ) {
  int i=0;
  indices.resize(indexes->count*3);
  EACH(indexes->first,VBOTriangle,t) {
   Integer *a=(Integer *)t->indices.first;
   Integer *b=(Integer *)t->indices.first->next;
   Integer *c=(Integer *)t->indices.first->next->next;
   indices[i*3+0]=(preferred_type) a->i;
   indices[i*3+1]=(preferred_type) b->i;
   indices[i*3+2]=(preferred_type) c->i;
   i++;
  }
 }
 */
 // Assumes all incoming are triangles
 virtual void fromModel( ImmediateModel *m ) {}
 virtual void fromModel( ImmediateModel *m, int group ) {}
 virtual void fromPLY( PLY *p ) {}
 virtual void fromPLY( PLY *p, bool create ) {}
 virtual void fromVBO( VBO16 *v ) {}
 virtual void Create() {
  if ( data.size() == 0 ) return;
  Precompute();
  Postcompute();
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (stride*data.size()), &data[0], usage);
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo );
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(preferred_type)*indices.size(), &indices[0], GL_STATIC_DRAW);
  uploaded=true;
  OUTPUT("ModernVBO::Create() glGenBuffers created: VBO %d of size %dx%d (%d), IBO %d of size %d.\n", 
   (int) vbo, (int) stride, (int) data.size(), (int) (stride*data.size()), (int) ibo, (int) indices.size() );
 }
 virtual void Bind() {
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  BindToStreams();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
 }
 virtual void Bind2() {
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  BindToStreams2();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
 }
 virtual void Bind3() {
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  BindToStreams3();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
 }
 virtual void Bind4() {
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  BindToStreams4();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
 }
 // Client states to enable/disable, if you're writing for an old deprecated VBO type.
 virtual void Enable() {}
 virtual void Disable() {}
 virtual void Render() {
  Enable();
  Bind();
//  glDrawRangeElements(GL_TRIANGLES, 0, data.size(), data.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  glDrawElements(draw_method,count,preferred_type_enum,BUFFER_OFFSET(0));
  Unbind();
  Disable();
 }
 virtual void Render( double x, double y, double z, double rx, double ry, double rz, double sx, double sy, double sz ) {
  glPush();
  glTranslated(x,y,z);
  glRotated(x,1,0,0);
  glRotated(y,0,1,0);
  glRotated(z,0,0,1);
  glScaled(sx,sy,sz);
  Enable();
  Bind();
//  glDrawRangeElements(GL_TRIANGLES, 0, data.size(), data.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  glDrawElements(draw_method,count,preferred_type_enum,BUFFER_OFFSET(0));
  Unbind();
  Disable();
  glPop();
 }
 virtual void Render2() {
  Enable();
  Bind2();
//  glDrawRangeElements(GL_TRIANGLES, 0, data.size(), data.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  glDrawElements(draw_method,count,preferred_type_enum,BUFFER_OFFSET(0));
  Unbind2();
  Disable();
 }
 virtual void Render3() {
  Enable();
  Bind3();
//  glDrawRangeElements(GL_TRIANGLES, 0, data.size(), data.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  glDrawElements(draw_method,count,preferred_type_enum,BUFFER_OFFSET(0));
  Unbind3();
  Disable();
 }
 virtual void Render4() {
  Enable();
  Bind4();
//  glDrawRangeElements(GL_TRIANGLES, 0, data.size(), data.size(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
  glDrawElements(draw_method,count,preferred_type_enum,BUFFER_OFFSET(0));
  Unbind4();
  Disable();
 }
 virtual void DebugRender() {}
 virtual void Unbind4() {
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  UnbindFromStreams4();
 }
 virtual void Unbind3() {
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  UnbindFromStreams3();
 }
 virtual void Unbind2() {
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  UnbindFromStreams2();
 }
 virtual void Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  UnbindFromStreams();
 }
 virtual void Release() {
  if ( vbo>0 ) glDeleteBuffers(1,&vbo);
  if ( ibo>0 ) glDeleteBuffers(1,&ibo);
  uploaded=false;
 }
 virtual void Clear() {
  if ( uploaded ) Release();
  data.resize(0);
  indices.resize(0);
 }
 virtual void ClearCPU() {
  data.resize(0);
  data.shrink_to_fit();
  indices.resize(0);
  indices.shrink_to_fit();
 }
 ~ModernVBO() { Clear(); }
};

/*

General VBO information on speed, performance, modification of dynamic buffers:

From: Performance Optimization 
Techniques for ATI Graphics
Hardware with DirectX 9.0
Revision: 1.0 
Original Date: Dec. 2002 
Author: Guennadi Riguer 

Quote:

For static vertex buffers 1-4Mb is a good size to start with, but it might vary depending on the amount of your resources, local video and AGP memory available. If amount of available video memory is low and a lot of resource swapping is expected, a smaller buffer size is a better choice. For dynamic buffers you should not allocate buffers bigger than the data streamed to the card per frame. In most cases 256Kb-1Mb dynamic buffers provide a good starting point for performance tweaking. In DirectX® 9 there is a way to specify byte offset in the vertex buffers when setting up vertex streams. This per-stream 
offset solves the problem of using multiple dynamic streams without wasting extra memory, which was a case in previous versions of DirectX®. Use this feature to pack multiple vertex formats into the same buffer and reduce the total number of buffers and their switches.

From: The Efficient Use of Vertex Buffers
Richard Huddy 
RichardH@nvidia.com
Quote:

For the GeForce family of GPUs there is no best size for a VB but the following rules apply: 
1. VB switches are expensive (much more expensive that on the regular HAL) so 
packing multiple objects into one VB is a good thing – simply because it saves the cost of the transition. 
2. Batches of primitives made up of fewer than about 200 triangles are necessarily sub-optimal. Very small batches (say 10 or fewer) should be regarded as a main target for optimization. 
3. Redundant information in the vertex format can bloat the VB and dramatically reduce the rate at which vertex data can be read across the AGP bus. 

Note that (1) and (3) can produce conflicting demands. Since switching VB is so expensive it can be advantageous to use a common vertex format throughout all or much of your app so that all vertex data can live in a small number of VBs. This is quite typical of optimization issues and makes the task significantly more complex.

Fast updates to VBOs: orphaning

Rob Barris (cited in OpenGL Wiki): across a PCIe bus, you can expect 1GB/sec transfer speeds

According to Rob Barris, MAP_INVALIDATE_RANGE_BIT​ in combination with the WRITE​ bit (but not the READ​ bit) basically says
to the driver that it doesn't need to contain any valid buffer data, and that you promise to write the entire range you map.
This lets the driver give you a pointer to scratch memory that hasn't been initialized. For instance, driver allocated
write-through uncached memory. See his post for more details.

Methods of using dynamic VBOs:

1) Explicit multiple buffering http://www.opengl.org/wiki/Buffer_Object_Streaming#Explicit_multiple_buffering

This solution is fairly simple. You simply create two or more buffer objects of the same length. While you are using one 
buffer object, you can be modifying another. Depending on how much parallelism your implementation can provide, you may
need more than two buffers to make this work.

The principle drawback to this solution is that it requires using a number of different buffer objects (separate buffer handles). 
If you are using this for uploading vertex data, you will therefore need more VAOs.

2)  Buffer re-specification

This solution is to reallocate the buffer object before you start modifying it.
This is termed buffer "orphaning". There are two ways to do it.

The first way is to call glBufferData​ with a NULL​ pointer, and the exact same size and usage hints it had before. This
allows the implementation to simply reallocate storage for that buffer object under-the-hood. Since allocating storage
is (likely) faster than the implicit synchronization, you gain significant performance advantages over synchronization.
And since you passed NULL, if there wasn't a need for synchronization to begin with, this can be reduced to a no-op.
The old storage will still be used by the OpenGL commands that have been sent previously. If you continue to use the same
size over-and-over, it is likely that the GL driver will not be doing any allocation at all, but will just be pulling an
old free block off the unused buffer queue and use it (though of course this isn't guaranteed), so it is likely to be
very efficient.

You can do the same thing when using glMapBufferRange​ with the GL_MAP_INVALIDATE_BUFFER_BIT​. You can also use
glInvalidateBufferData​, where available.

All of these give the GL implementation the freedom to orphan the previous storage and allocate a new one. Which is why
this is called "orphaning".

Whenever you see either of these, think of it as a directive to OpenGL to 1) detach the old block of storage and 2) give
you a new block of storage to work with, all behind the same buffer handle. The old block of storage will be put on a free
list by OpenGL and reused once there can be no draw commands in the queue which might be referring to it (e.g. once all queued
GL commands have finished executing).

Obviously, these methods detach the buffer storage from the client-accessible workspace, so they are only practical if there
is no further need to read or update this specific block of storage from the GL client side. Unless you plan to use
buffer updates in combination with this technique, then it is best if updates are done on a whole buffer rather than
parts of a buffer, and if you overwrite all of the data in that buffer each time.

One issue with this method is that it is implementation dependent. Just because an implementation has the freedom to do
something does not mean that it will.

3) Buffer updates
--

Rob Barris: Join Date May 2009 Posts 4,518	
Re: VBOs strangely slow?

Thought the whole purpose of MapBuffer NULL / UNSYNCHRONIZED is so the GPU can have multiple buffers in flight for the
same buffer handle, and thus pipeline the buffer uploads, avoiding stalls. So I'm not seeing where fences come in.

GL_UNSYNCHRONIZED is not the same as GL_INVALIDATE.
GL_INVALIDATE tells the implementation, "I don't care what was in the buffer before; I just want some memory!"
GL_UNSYNCHRONIZED says, "I don't care that you may currently be using the buffer, and that my attempt to modify
it while in use can have horrible consequences. I will take responsibility for making sure the buffer is not in
use when I modify it, so give me a pointer already!"

They're both solutions to the same basic problem (I rendered with a buffer last frame, and I want to change it
and use it this frame), but with different needs. GL_INVALIDATE/glBufferData(NULL) is ultimately giving you two
buffer objects: the one that's currently in use and the one you're writing to. GL_UNSYNCHRONIZED is all about
using only one piece of memory to avoid the synchronization.

The idea is that you fill up a buffer object, do something with it, and then set a fence. If you want to change
the buffer, you check your fence. If the fence has not passed yet, you go do something else (and therefore this
only works when you have "something else" that you could be doing). When the fence has passed, you can now fill
the buffer. 

GL_UNSYNCHRONIZED can allow for idioms where the client is generating a large number of small batches dynamically;
it makes it much more efficient to stack them up one after another within a smaller number of larger sized VBO's.
For example you could have a 4MB VBO, and be able to map/write/unmap/draw several hundred times using that storage,
before ever having to orphan or fence, if you are processing kilobyte-ish batches of data.

Concurrency goes up esp on a multi-threaded driver when you can use the cheap operation more frequently than
the expensive one (unsync map = cheap ... orphaning = less cheap).

Baughn (Dwarf Fortress) wrote:
What I came up with in the end for the actual application (code here, but there's way too much of it) is to use two
VBOs, for the variable data, which I switch between once per frame (using glMapBufferRange to invalidate if available,
glBufferData otherwise), and a static_draw VBO for the quite static vertex grid. This works well enough; it's as fast
as the ncurses output mode, which means about twice the speed of any other mode even counting immutable overhead.

Barris comes back:
workload: say the CPU wants to draw a series of batches where each one is based on data generated or unpacked right
before issuing of the draw request. Once written, the data is not going to be modified or read back by the CPU.
The goal is to efficiently let the GPU have access to the newly written data, and to avoid bogging down with excessive
allocation or synchronization on a per-draw basis.


( As a hypothetical example, say we're using the CPU to deform and draw hundreds of falling leaves, where the
leaf-shape algorithm runs on the CPU, and can be used to generate new batches of verts for each leaf at will )

So, you can do this with one VBO and no fences, and it can run really well. The magic is hiding in the buffer-orphaning step.
http://www.opengl.org/discussion_boards/showthread.php/170118-VBOs-strangely-slow/page4 (detail)

Again if your workload doesn't fit this model, you would need to do more explicit sync effort possibly using
fences to know "when" it is safe to touch any given region of storage. But if all you do is fill, fill, fill
and then orphan and start over - you never need to check or sync. The juggling of multiple blocks of storage
is all in the driver and not your problem. All you need to do is be careful about only writing each section
of the larger VBO once and then moving on, and you're fine.

If you are trying to manage the contents of a VBO such that some portions of it stay constant while other portions are
changing, that's a workload where you would probably have to start using fences or other heuristics to schedule overwrites
of pieces of it. (One heuristic is "has this chunk been used to draw anything in the last five frames" - if no, and you
know the driver has a three frame queuing limit say, then you can actually infer when it's safe to overwrite that region
without any sync effort, i.e. non blocking map, but you need to make sure you track carefully each segment and mark them
in your own data structure when they were last used for draw).

OTOH glBufferSubData will always be orderly and safe for a partial VBO replacement, no matter what has happened recently,
but you have to have the source data in copyable form, whereas with mapping you can combine decompression and
delivery into the buffer.

IMO the application usually knows more about its operational history than the driver does, and is in a better position
to make clever decisions about when sync is needed, which is why MapBufferRange has the unsynchronized option.

There are two kinds of invalidation that MapBufferRange can do, and they have very different purposes.

One is tied to MAP_INVALIDATE_BUFFER_BIT in the access parameter to MapBufferRange. This essentially means
"orphan". So in the usage I described, you could set this bit when you go back to offset 0 and get the same
effect as BufferData(NULL).

The other is a bit more subtle, and it is tied to MAP_INVALIDATE_RANGE_BIT. This may seem a bit redundant, but it is
important. It explicitly tells the driver up front "the range I am mapping - it does not need to contain valid data
that I can read" - it is a signal to the driver that it is free to replace every single byte in that range with whatever
is in your CPU-visible mapped buffer area upon unmap (or explicit flush).

The freedom this provides to the driver, if you have also set the WRITE bit but not the READ bit, is that it can
hand back a pointer to completely uninitialized scratch memory - which may well be driver allocated for write-through
uncached access etc. By opting into invalidation of the range, you eliminate any need for the driver to put a copy of
valid data in that range prior to returning the pointer. If an implementor wanted to keep system-memory images of buffers
to a minimum, this would let that driver provide scratchpad memory for maps using these bits (write + invalidate-range)
- and then transfer those bits to the final destination later, perhaps via DMA.

Restated more simply, think of MAP_INVALIDATE_RANGE_BIT as a
"promise to write the whole range, nothing but the range, and never read from the range" bit. 
*/