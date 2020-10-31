#include "AStarTest.h"


TestAStarThread::TestAStarThread(AStarThreadedTest *win, AStarPathfinder *astar, int x, int y, int x2, int y2) : AStarThread(astar,x,y,x2,y2) {
 this->win=win;
}
 
void TestAStarThread::OnComplete() {
 this->win->path=this->path;
 this->win->doneCalculating=true;
 this->win->needsRedrawn=true;
 this->deleteMe=true;
}