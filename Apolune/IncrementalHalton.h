#pragma once
/////////////////////////////////////////////////////////////////////////////////////
//  IncrementalHalton.cpp
//    
//  Calculates the halton sequence incrementally
//    
//  Andrew Willmott, public domain
/////////////////////////////////////////////////////////////////////////////////////

/*
    Notes:
    
    This is a reference implementation for clarity -- optimizations are possible!
    
    In particular the base 2 part can be done more quickly using bit tricks.
    (E.g., branchless bit reverse followed by or'ing the result into a float
    mantissa.)
*/

#include <stdint.h>
#include <stdio.h>

namespace
{
   const float kOneOverThree = float(1.0 / 3.0);
   const float kOneOverFive  = float(1.0 / 5.0);
}


/////////////////////////////////////////////////////////////////////////////////////
// cHaltonSequence3
//

struct cHaltonSequence3
/// This calculates the Halton sequence incrementally
/// for a base 2/3/5 triplet.
{
//    cFXVector3 mPoint;  ///< Current sample point.
    float mX;
    float mY;
    float mZ;
    
    uint32_t mBase2;
    uint32_t mBase3;
    uint32_t mBase5;
    
    cHaltonSequence3() : 
        mBase2(0),
        mBase3(0),
        mBase5(0),
        mX(0.0f),
        mY(0.0f),
        mZ(0.0f)
    {}
    
    int inc();
    ///< Advance to next point in the sequence. Returns the index of this point. 
    void reset();
    ///< Move back to first point in the sequence (i.e. the origin.)
};