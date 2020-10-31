#pragma once

// Implements Milazzo Visibility
// source: http://www.adammil.net/resume.html

#if defined(REFERENCE_ONLY)
class MilazzoVisibility : Visibility
{
  /// blocksLight: A function that accepts the X and Y coordinates of a tile and determines whether the
  /// given tile blocks the passage of light. The function must be able to accept coordinates that are out of bounds.

  /// setVisible: A function that sets a tile to be visible, given its X and Y coordinates. The function
  /// must ignore coordinates that are out of bounds.

  /// getDistance: A function that takes the X and Y coordinate of a point where X >= 0,
  /// Y >= 0, and X >= Y, and returns the distance from the point to the origin (0,0).
  public MyVisibility(Func<int, int, bool> blocksLight, Action<int, int> setVisible, Func<int, int, int> getDistance)
  {
    _blocksLight = blocksLight;
    GetDistance  = getDistance;
    _setVisible  = setVisible;
  }

  public override void Compute(LevelPoint origin, int rangeLimit)
  {
    _setVisible(origin.X, origin.Y);
    for(uint octant=0; octant<8; octant++) Compute(octant, origin, rangeLimit, 1, new Slope(1, 1), new Slope(0, 1));
  }

  struct Slope // represents the slope Y/X as a rational number
  {
    public Slope(uint y, uint x) { Y = y; X = x; }

    public bool Greater(uint y, uint x) { return Y*x > X*y; } // this > y/x
    public bool GreaterOrEqual(uint y, uint x) { return Y*x >= X*y; } // this >= y/x
    public bool Less(uint y, uint x) { return Y*x < X*y; } // this < y/x
    //public bool LessOrEqual(uint y, uint x) { return Y*x <= X*y; } // this <= y/x

    public readonly uint X, Y;
  }

  void Compute(uint octant, LevelPoint origin, int rangeLimit, uint x, Slope top, Slope bottom)
  {
    // throughout this function there are references to various parts of tiles. a tile's coordinates refer to its
    // center, and the following diagram shows the parts of the tile and the vectors from the origin that pass through
    // those parts. given a part of a tile with vector u, a vector v passes above it if v > u and below it if v < u
    //    g         center:        y / x
    // a------b   a top left:      (y*2+1) / (x*2-1)   i inner top left:      (y*4+1) / (x*4-1)
    // |  /\  |   b top right:     (y*2+1) / (x*2+1)   j inner top right:     (y*4+1) / (x*4+1)
    // |i/__\j|   c bottom left:   (y*2-1) / (x*2-1)   k inner bottom left:   (y*4-1) / (x*4-1)
    //e|/|  |\|f  d bottom right:  (y*2-1) / (x*2+1)   m inner bottom right:  (y*4-1) / (x*4+1)
    // |\|__|/|   e middle left:   (y*2) / (x*2-1)
    // |k\  /m|   f middle right:  (y*2) / (x*2+1)     a-d are the corners of the tile
    // |  \/  |   g top center:    (y*2+1) / (x*2)     e-h are the corners of the inner (wall) diamond
    // c------d   h bottom center: (y*2-1) / (x*2)     i-m are the corners of the inner square (1/2 tile width)
    //    h
    for(; x <= (uint)rangeLimit; x++) // (x <= (uint)rangeLimit) == (rangeLimit < 0 || x <= rangeLimit)
    {
      // compute the Y coordinates of the top and bottom of the sector. we maintain that top > bottom
      uint topY;
      if(top.X == 1) // if top == ?/1 then it must be 1/1 because 0/1 < top <= 1/1. this is special-cased because top
      {              // starts at 1/1 and remains 1/1 as long as it doesn't hit anything, so it's a common case
        topY = x;
      }
      else // top < 1
      {
        // get the tile that the top vector enters from the left. since our coordinates refer to the center of the
        // tile, this is (x-0.5)*top+0.5, which can be computed as (x-0.5)*top+0.5 = (2(x+0.5)*top+1)/2 =
        // ((2x+1)*top+1)/2. since top == a/b, this is ((2x+1)*a+b)/2b. if it enters a tile at one of the left
        // corners, it will round up, so it'll enter from the bottom-left and never the top-left
        topY = ((x*2-1) * top.Y + top.X) / (top.X*2); // the Y coordinate of the tile entered from the left
        // now it's possible that the vector passes from the left side of the tile up into the tile above before
        // exiting from the right side of this column. so we may need to increment topY
        if(BlocksLight(x, topY, octant, origin)) // if the tile blocks light (i.e. is a wall)...
        {
          // if the tile entered from the left blocks light, whether it passes into the tile above depends on the shape
          // of the wall tile as well as the angle of the vector. if the tile has does not have a beveled top-left
          // corner, then it is blocked. the corner is beveled if the tiles above and to the left are not walls. we can
          // ignore the tile to the left because if it was a wall tile, the top vector must have entered this tile from
          // the bottom-left corner, in which case it can't possibly enter the tile above.
          //
          // otherwise, with a beveled top-left corner, the slope of the vector must be greater than or equal to the
          // slope of the vector to the top center of the tile (x*2, topY*2+1) in order for it to miss the wall and
          // pass into the tile above
          if(top.GreaterOrEqual(topY*2+1, x*2) && !BlocksLight(x, topY+1, octant, origin)) topY++;
        }
        else // the tile doesn't block light
        {
          // since this tile doesn't block light, there's nothing to stop it from passing into the tile above, and it
          // does so if the vector is greater than the vector for the bottom-right corner of the tile above. however,
          // there is one additional consideration. later code in this method assumes that if a tile blocks light then
          // it must be visible, so if the tile above blocks light we have to make sure the light actually impacts the
          // wall shape. now there are three cases: 1) the tile above is clear, in which case the vector must be above
          // the bottom-right corner of the tile above, 2) the tile above blocks light and does not have a beveled
          // bottom-right corner, in which case the vector must be above the bottom-right corner, and 3) the tile above
          // blocks light and does have a beveled bottom-right corner, in which case the vector must be above the
          // bottom center of the tile above (i.e. the corner of the beveled edge).
          // 
          // now it's possible to merge 1 and 2 into a single check, and we get the following: if the tile above and to
          // the right is a wall, then the vector must be above the bottom-right corner. otherwise, the vector must be
          // above the bottom center. this works because if the tile above and to the right is a wall, then there are
          // two cases: 1) the tile above is also a wall, in which case we must check against the bottom-right corner,
          // or 2) the tile above is not a wall, in which case the vector passes into it if it's above the bottom-right
          // corner. so either way we use the bottom-right corner in that case. now, if the tile above and to the right
          // is not a wall, then we again have two cases: 1) the tile above is a wall with a beveled edge, in which
          // case we must check against the bottom center, or 2) the tile above is not a wall, in which case it will
          // only be visible if light passes through the inner square, and the inner square is guaranteed to be no
          // larger than a wall diamond, so if it wouldn't pass through a wall diamond then it can't be visible, so
          // there's no point in incrementing topY even if light passes through the corner of the tile above. so we
          // might as well use the bottom center for both cases.
          uint ax = x*2; // center
          if(BlocksLight(x+1, topY+1, octant, origin)) ax++; // use bottom-right if the tile above and right is a wall
          if(top.Greater(topY*2+1, ax)) topY++;
        }
      }

      uint bottomY;
      if(bottom.Y == 0) // if bottom == 0/?, then it's hitting the tile at Y=0 dead center. this is special-cased because
      {                 // bottom.Y starts at zero and remains zero as long as it doesn't hit anything, so it's common
        bottomY = 0;
      }
      else // bottom > 0
      {
        bottomY = ((x*2-1) * bottom.Y + bottom.X) / (bottom.X*2); // the tile that the bottom vector enters from the left
        // code below assumes that if a tile is a wall then it's visible, so if the tile contains a wall we have to
        // ensure that the bottom vector actually hits the wall shape. it misses the wall shape if the top-left corner
        // is beveled and bottom >= (bottomY*2+1)/(x*2). finally, the top-left corner is beveled if the tiles to the
        // left and above are clear. we can assume the tile to the left is clear because otherwise the bottom vector
        // would be greater, so we only have to check above
        if(bottom.GreaterOrEqual(bottomY*2+1, x*2) && BlocksLight(x, bottomY, octant, origin) &&
           !BlocksLight(x, bottomY+1, octant, origin))
        {
          bottomY++;
        }
      }

      // go through the tiles in the column now that we know which ones could possibly be visible
      int wasOpaque = -1; // 0:false, 1:true, -1:not applicable
      for(uint y = topY; (int)y >= (int)bottomY; y--) // use a signed comparison because y can wrap around when decremented
      {
        if(rangeLimit < 0 || GetDistance((int)x, (int)y) <= rangeLimit) // skip the tile if it's out of visual range
        {
          bool isOpaque = BlocksLight(x, y, octant, origin);
          // every tile where topY > y > bottomY is guaranteed to be visible. also, the code that initializes topY and
          // bottomY guarantees that if the tile is opaque then it's visible. so we only have to do extra work for the
          // case where the tile is clear and y == topY or y == bottomY. if y == topY then we have to make sure that
          // the top vector is above the bottom-right corner of the inner square. if y == bottomY then we have to make
          // sure that the bottom vector is below the top-left corner of the inner square
          bool isVisible =
            isOpaque || ((y != topY || top.Greater(y*4-1, x*4+1)) && (y != bottomY || bottom.Less(y*4+1, x*4-1)));
          // NOTE: if you want the algorithm to be either fully or mostly symmetrical, replace the line above with the
          // following line (and uncomment the Slope.LessOrEqual method). the line ensures that a clear tile is visible
          // only if there's an unobstructed line to its center. if you want it to be fully symmetrical, also remove
          // the "isOpaque ||" part and see NOTE comments further down
          // bool isVisible = isOpaque || ((y != topY || top.GreaterOrEqual(y, x)) && (y != bottomY || bottom.LessOrEqual(y, x)));
          if(isVisible) SetVisible(x, y, octant, origin);

          // if we found a transition from clear to opaque or vice versa, adjust the top and bottom vectors
          if(x != rangeLimit) // but don't bother adjusting them if this is the last column anyway
          {
            if(isOpaque)
            {
              if(wasOpaque == 0) // if we found a transition from clear to opaque, this sector is done in this column,
              {                  // so adjust the bottom vector upward and continue processing it in the next column
                // if the opaque tile has a beveled top-left corner, move the bottom vector up to the top center.
                // otherwise, move it up to the top left. the corner is beveled if the tiles above and to the left are
                // clear. we can assume the tile to the left is clear because otherwise the vector would be higher, so
                // we only have to check the tile above
                uint nx = x*2, ny = y*2+1; // top center by default
                // NOTE: if you're using full symmetry and want more expansive walls (recommended), comment out the next line
                if(BlocksLight(x, y+1, octant, origin)) nx--; // top left if the corner is not beveled
                if(top.Greater(ny, nx)) // we have to maintain the invariant that top > bottom, so the new sector
                {                       // created by adjusting the bottom is only valid if that's the case
                  // if we're at the bottom of the column, then just adjust the current sector rather than recursing
                  // since there's no chance that this sector can be split in two by a later transition back to clear
                  if(y == bottomY) { bottom = new Slope(ny, nx); break; } // don't recurse unless necessary
                  else Compute(octant, origin, rangeLimit, x+1, top, new Slope(ny, nx));
                }
                else // the new bottom is greater than or equal to the top, so the new sector is empty and we'll ignore
                {    // it. if we're at the bottom of the column, we'd normally adjust the current sector rather than
                  if(y == bottomY) return; // recursing, so that invalidates the current sector and we're done
                }
              }
              wasOpaque = 1;
            }
            else
            {
              if(wasOpaque > 0) // if we found a transition from opaque to clear, adjust the top vector downwards
              {
                // if the opaque tile has a beveled bottom-right corner, move the top vector down to the bottom center.
                // otherwise, move it down to the bottom right. the corner is beveled if the tiles below and to the right
                // are clear. we know the tile below is clear because that's the current tile, so just check to the right
                uint nx = x*2, ny = y*2+1; // the bottom of the opaque tile (oy*2-1) equals the top of this tile (y*2+1)
                // NOTE: if you're using full symmetry and want more expansive walls (recommended), comment out the next line
                if(BlocksLight(x+1, y+1, octant, origin)) nx++; // check the right of the opaque tile (y+1), not this one
                // we have to maintain the invariant that top > bottom. if not, the sector is empty and we're done
                if(bottom.GreaterOrEqual(ny, nx)) return;
                top = new Slope(ny, nx);
              }
              wasOpaque = 0;
            }
          }
        }
      }

      // if the column didn't end in a clear tile, then there's no reason to continue processing the current sector
      // because that means either 1) wasOpaque == -1, implying that the sector is empty or at its range limit, or 2)
      // wasOpaque == 1, implying that we found a transition from clear to opaque and we recursed and we never found
      // a transition back to clear, so there's nothing else for us to do that the recursive method hasn't already. (if
      // we didn't recurse (because y == bottomY), it would have executed a break, leaving wasOpaque equal to 0.)
      if(wasOpaque != 0) break;
    }
  }

  // NOTE: the code duplication between BlocksLight and SetVisible is for performance. don't refactor the octant
  // translation out unless you don't mind an 18% drop in speed
  bool BlocksLight(uint x, uint y, uint octant, LevelPoint origin)
  {
    uint nx = origin.X, ny = origin.Y;
    switch(octant)
    {
      case 0: nx += x; ny -= y; break;
      case 1: nx += y; ny -= x; break;
      case 2: nx -= y; ny -= x; break;
      case 3: nx -= x; ny -= y; break;
      case 4: nx -= x; ny += y; break;
      case 5: nx -= y; ny += x; break;
      case 6: nx += y; ny += x; break;
      case 7: nx += x; ny += y; break;
    }
    return _blocksLight((int)nx, (int)ny);
  }

  void SetVisible(uint x, uint y, uint octant, LevelPoint origin)
  {
    uint nx = origin.X, ny = origin.Y;
    switch(octant)
    {
      case 0: nx += x; ny -= y; break;
      case 1: nx += y; ny -= x; break;
      case 2: nx -= y; ny -= x; break;
      case 3: nx -= x; ny -= y; break;
      case 4: nx -= x; ny += y; break;
      case 5: nx -= y; ny += x; break;
      case 6: nx += y; ny += x; break;
      case 7: nx += x; ny += y; break;
    }
    _setVisible((int)nx, (int)ny);
  }

  readonly Func<int, int, bool> _blocksLight;
  readonly Func<int, int, int> GetDistance;
  readonly Action<int, int> _setVisible;
}
#endif