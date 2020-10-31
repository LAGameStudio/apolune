#include "InfiniteTerrain.h"

bool operator < (const VBlock &b0, const VBlock &b1) { if (b0.z < b1.z) return true; if (b0.z > b1.z) return false; return (b0.x < b1.x); }
bool operator != (const VBlock &b0, const VBlock &b1) { return (b0.x != b1.x || b0.z != b1.z); }


InfiniteTerrainThread::InfiniteTerrainThread( InfiniteTerrain *it ) : pThread() {
 this->it=it;
}

bool InfiniteTerrainThread::Unlocked() {
 VBlockRequest *request=(VBlockRequest *) requests.first;
 request->block->Calculate();
 return true;
}

bool InfiniteTerrainThread::Locked() {
 VBlockRequest *request=(VBlockRequest *) requests.first;
 request->block->Upload();
 requests.Remove(request);
 delete request;
 return true;
}

#include "AppFramework.h"

void InfiniteTerrainThread::Main() {
#if defined(NEVER)
 while ( !done ) {
  if ( requests.count > 0 ) {
   wglMakeCurrent(app.game_dc, app.upload_rc);
   Unlocked();
   wglMakeCurrent(app.game_dc, app.game_rc);
   pthreads.Lock();
   Locked();
   pthreads.Unlock();
  }
 }
#endif
}
VBlock *InfiniteTerrain::GenerateBlock( int x, int z ){
 return blocks.New(x,z,&generator,true);
}

VBlock *InfiniteTerrain::RequestBlock(int x, int z) {
 VBlockRequest *requesting=thread->requests.Find(x,z);
 if ( !requesting ) {
  OUTPUT("RequestBlock is requesting %d,%d (%d requests already)\n",x,z, (int)thread->requests.count );
  VBlock *block = blocks.New(x,z,&generator);
  thread->requests.Add(block);
  return block;
 } else {
  OUTPUT("InfiniteTerrain::RequestBlock(%d,%d) Error: the code should never reach this point.\n",(int)x,(int)z);
  return blocks.FindBlock(x,z);
 }
}

InfiniteTerrain::InfiniteTerrain( bool threaded, double bSize, int bDensity, int blockTexRepeat, int mBlocks){
 this->threaded=threaded;
 if ( threaded ) {
  thread=new InfiniteTerrainThread(this);
  pthreads.New(thread);
 }
 blocks.viewDistance=5000.0;
 blocks.lodFactor=0.0013;
 blocks.blockSize = bSize;
 blocks.blockDensity = getClosestPowerOfTwo(bDensity) + 1;
 blocks.maxBlocks = mBlocks;
 blocks.texRepeat = blockTexRepeat;
 blocks.GenerateIndexArrays();
}

InfiniteTerrain::~InfiniteTerrain() {
 if ( threaded && thread ) {
  pthreads.Kill(thread);
  thread=null;
 }
}

void InfiniteTerrain::Render() {
 int minx = (int) roundf((float)((blocks.position.x - blocks.viewDistance) / blocks.blockSize));
 int maxx = (int) roundf((float)((blocks.position.x + blocks.viewDistance) / blocks.blockSize));
 int minz = (int) roundf((float)((blocks.position.z - blocks.viewDistance) / blocks.blockSize));
 int maxz = (int) roundf((float)((blocks.position.z + blocks.viewDistance) / blocks.blockSize));
 VBlock *block = NULL;
#if defined(DEBUG)
// int nDrawnVertices = 0;
#endif
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_NORMAL_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 glEnable(GL_CULL_FACE);
 glCullFace(GL_BACK);
 for (int z = minz; z <= maxz; z++) for (int x = minx; x <= maxx; x++) {
  double d = blocks.blockDistance(x,z,blocks.position.x,blocks.position.z);
  if (d < blocks.viewDistance){
   VBlock *block=blocks.FindBlock(x,z);
   if (!block) {
    if ( threaded ) block=RequestBlock(x,z);
    else block=GenerateBlock(x,z);
  	}
   blocks.Render(block,d);
#if defined(DEBUG)
//    nDrawnVertices += nIndices;
#endif
  }
 }
 glDisable(GL_CULL_FACE);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
 glDisableClientState(GL_VERTEX_ARRAY);
 blocks.PopFarthest();
}

void InfiniteTerrain::Render( HVertex *position ) {
 int minx = (int) roundf((float)((position->x - blocks.viewDistance) / blocks.blockSize));
 int maxx = (int) roundf((float)((position->x + blocks.viewDistance) / blocks.blockSize));
 int minz = (int) roundf((float)((position->z - blocks.viewDistance) / blocks.blockSize));
 int maxz = (int) roundf((float)((position->z + blocks.viewDistance) / blocks.blockSize));
 VBlock *block = NULL;
#if defined(DEBUG)
// int nDrawnVertices = 0;
#endif
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_NORMAL_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
// glEnable(GL_CULL_FACE);
// glCullFace(GL_BACK);
 for (int z = minz; z <= maxz; z++) for (int x = minx; x <= maxx; x++) {
  double d = blocks.blockDistance(x,z,position->x,position->z);
  if (d < blocks.viewDistance){
   block=blocks.FindBlock(x,z);
   if ( !block ) {
    if ( threaded ) block=RequestBlock(x,z);
    else block=GenerateBlock(x,z);
  	}
   blocks.Render(block,d);
#if defined(DEBUG)
//    nDrawnVertices += nIndices;
#endif
  }
 }
// glDisable(GL_CULL_FACE);
 glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
 glDisableClientState(GL_VERTEX_ARRAY);
 blocks.PopFarthest(position);
}
