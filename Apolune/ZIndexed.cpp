#include "ZIndexed.h"

#include "ZeroTypes.h"

void ZIndexed_Rigorous_Test() {
 OUTPUT("ZIndexed_Rigorous_Test: Phase I: allocating, initializing and destroying.\n");
 for ( unsigned int i=0; i<100; i++ ) {
  ZIndexed<unsigned int> b;
  b.Size(1024*1024);
  for ( unsigned int j=0; j<1024*1024; j++ ) {
   b(j)=j;
  }
 }
 OUTPUT("ZIndexed_Rigorous_Test: Phase II: resizing.\n");
 ZIndexed<unsigned int> b;
 for ( unsigned int i=0; i<100; i++ ) {
  b.Size(i*i*i);
  for ( unsigned int j=0; j<i*i*i; j++ ) {
   b(j)=j;
  }
 }
 OUTPUT("ZIndexed_Rigorous_Test: Phase III: complex Zero-memory tasks.\n");
 for ( unsigned int i=0; i<100; i++ ) {
  ZIndexed<Zpointer<unsigned int>> c;
   c.Size(i*i*i);
  for ( unsigned int j=0; j<i*i*i; j++ ) {
   c(j)=&i;
  }
 }
 OUTPUT("ZIndexed_Rigorous_Test: Phase IV: complex disposable-memory tasks.\n");
 for ( unsigned int i=0; i<100; i++ ) {
  if ( i % 2 == 0 ) OUTPUT(".");
  ZIndexed<Zdisposable<ZIndexed<unsigned int>>> c;
   c.Size(i);
  for ( unsigned int j=0; j<i; j++ ) {
   c(j).Recycle();
   c(j)->Size(j);
//   for ( unsigned int k=0; k<j; j++ ) {
   // (*c(j)->Element(k))=k;
//   }
  }
 }
 OUTPUT("ZIndexed_Rigorous_Test: Phase V: complex disposable-memory tasks with linear increase.\n");
 ZIndexed<Zdisposable<Zstring>> c;
 for ( unsigned int i=0; i<100; i++ ) {
  c.Increase();
  if ( i % 2 == 0 ) OUTPUT(".");
  for ( unsigned int j=0; j<i+1; j++ ) {
   c(j).Recycle();
//   for ( unsigned int k=0; k<j; j++ ) {
   // (*c(j)->Element(k))=k;
//   }
  }
 }
 OUTPUT("\n");
 OUTPUT("Indexed_Rigorous_Test: Complete.\n");
}