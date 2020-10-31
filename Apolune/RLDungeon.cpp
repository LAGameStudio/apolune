#include "RLDungeon.h"

RLEDirection RandomDirection( int seed ) { return (RLEDirection) randoms.Upto(RLEDirectionMax,seed,seed); }

