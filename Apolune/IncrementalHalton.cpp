#include "IncrementalHalton.h"

int cHaltonSequence3::inc()
{
    /////////////////////////////////////
    // base 2
    
    uint32_t oldBase2 = mBase2;
    mBase2++;
    uint32_t diff = mBase2 ^ oldBase2;

    // bottom bit always changes, higher bits
    // change less frequently.
    float s = 0.5f;

    // diff will be of the form 0*1+, i.e. one bits up until the last carry.
    // expected iterations = 1 + 0.5 + 0.25 + ... = 2
    do
    {
        if (oldBase2 & 1)
            mX -= s;
        else
            mX += s;
        
        s *= 0.5f;
        
        diff = diff >> 1;
        oldBase2 = oldBase2 >> 1;
    }
    while (diff);

    
    /////////////////////////////////////
    // base 3: use 2 bits for each base 3 digit.
    
    uint32_t mask = 0x3;  // also the max base 3 digit
    uint32_t add  = 0x1;  // amount to add to force carry once digit==3
    s = kOneOverThree;

    mBase3++;

    // expected iterations: 1.5
    while (1)
    {
        if ((mBase3 & mask) == mask)
        {
            mBase3 += add;          // force carry into next 2-bit digit
            mY -= 2 * s;
            
            mask = mask << 2;
            add  = add  << 2;
            
            s *= kOneOverThree;
        }
        else 
        {
            mY += s;     // we know digit n has gone from a to a + 1
            break;
        }
    };

    /////////////////////////////////////
    // base 5: use 3 bits for each base 5 digit.
    mask = 0x7;
    add  = 0x3;  // amount to add to force carry once digit==dmax
    uint32_t dmax = 0x5;  // max digit
    
    s = kOneOverFive;

    mBase5++;
    
    // expected iterations: 1.25
    while (1)
    {
        if ((mBase5 & mask) == dmax)
        {
            mBase5 += add;          // force carry into next 3-bit digit
            mZ -= 4 * s;
            
            mask = mask << 3;
            dmax = dmax << 3;
            add  = add  << 3;
            
            s *= kOneOverFive;
        }
        else 
        {
            mZ += s;     // we know digit n has gone from a to a + 1
            break;
        }
    };

    return mBase2; // return the index of this sequence point
}

void cHaltonSequence3::reset()
{
    mBase2 = 0;
    mBase3 = 0;
    mBase5 = 0;
    mX = 0.0f;
    mY = 0.0f;
    mZ = 0.0f;
}


// for comparison purposes

float HaltonSequence(int n, int b)
/// return term i of the base b Halton sequence
/// You can think of this as, for example, just a generalization of Heckbert's bit-
/// reversal distribution trick.
/// E.g., when b=3, write n as a base 3 number, digit 0 -> which third of interval the
/// sample is in, 1 -> which third of that, 2 -> which third of that, etc.
{
    float result = 0;
    float ip = 1.0f / b;     
    float p = ip;

    while (n > 0)
    {
        result += (n % b) * p;
        n = n / b;
        p *= ip;
    }

    return result;
}

/*
int main(int argc, char* argv[])
{
    cHaltonSequence3 seq;
    
    for (int i = 0; i < 100; i++)
    {
        printf("%d = (%g, %g, %g)\n", i, seq.mX, seq.mY, seq.mZ);
        
        seq.inc();        
    }   
}
*/
