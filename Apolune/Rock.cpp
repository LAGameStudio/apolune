#include "Rock.h"

Rock3dCollection rocks,rocksHiRes;

void InitAsteroids() {
 rocks.Generate(1,1,0.5f,0.5f,2.0f); // test
// rocks.Generate(3,3,0.5f,0.5f,2.0f); //low-res
// rocks.Generate(5,3,0.3f,0.3f,2.0f); //production
// rocks.Generate(10,4,0.5f,0.3f,2.0f); //hi-res
// rocks.Generate(10,5,0.3f,0.2f,2.11f); //super hi-res
// rocks.Generate(20,5,0.3f,0.2f,2.0f); //super hi-res

  /*
 char buf[128];
 if ( file_exists("data/troids/troid.0") ) {
  for ( int i=0; i<3000; i++ ) {
   FORMAT(buf,128,"data/troids/troid.%d",i);
   if ( !file_exists(buf) ) {
    OUTPUT("Could not read file `%s`...\n", buf);
    break;
   }
   BinaryFile *in=new BinaryFile(buf,false);
   Rock3d *rock=new Rock3d;
   rock->BinaryRead(in);
   rocks.Append(rock);
   delete in;
  }
  OUTPUT("InitAsteroids() read %d asteroids from the files.\n",rocks.count);
 } else {
  rocks.Generate(3,5,0.5f,0.3f,2.0f);
  int i=0;
  EACH(rocks.first,Rock3d,rock) {
   FORMAT(buf,128,"data/troids/troid.%d",i);
   i++;
   BinaryFile *out=new BinaryFile(buf,true);
   rock->BinaryWrite(out);
   delete out;
  }
 }
 */
}