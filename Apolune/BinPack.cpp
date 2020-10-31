#include "BinPack.h"



#ifdef _DEBUG
/// debug_assert is an assert that also requires debug mode to be defined.
#define debug_assert(x) assert(x)
#else
#define debug_assert(x)
#endif

/// overriding minwindef.h
#undef min
#undef max

namespace rbp {

bool IsContainedIn(const Cartesian &a, const Cartesian &b)
{
	return a.x >= b.x && a.y >= b.y 
		&& a.x+a.w <= b.x+b.w 
		&& a.y+a.h <= b.y+b.h;
}

}

/** @file GuillotineBinPack.cpp
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the GUILLOTINE data structure.

	This work is released to Public Domain, do whatever you want with it.
*/
#include <utility>
#include <iostream>
#include <limits>

#include <cassert>
#include <cstring>
#include <cmath>


namespace rbp {

using namespace std;

GuillotineBinPack::GuillotineBinPack() : binWidth(0), binHeight(0) {
}

GuillotineBinPack::GuillotineBinPack(int width, int height) {
	Init(width, height);
}

void GuillotineBinPack::Init(int width, int height)
{
	binWidth = width;
	binHeight = height;

#ifdef _DEBUG
	disjointRects.Clear();
#endif

	// Clear any memory of previously packed rectangles.
	usedRectangles.clear();

	// We start with a single big free rectangle that spans the whole bin.
	Cartesian n;
	n.x = 0;
	n.y = 0;
	n.w = width;
	n.h = height;

	freeRectangles.clear();
	freeRectangles.push_back(n);
}

void GuillotineBinPack::Insert(std::vector<Cartesian> &rects, bool merge, 
	FreeRectChoiceHeuristic rectChoice, GuillotineSplitHeuristic splitMethod)
{
	// Remember variables about the best packing choice we have made so far during the iteration process.
	int bestFreeRect = 0;
	int bestRect = 0;
	bool bestFlipped = false;

	// Pack rectangles one at a time until we have cleared the rects array of all rectangles.
	// rects will get destroyed in the process.
	while(rects.size() > 0)
	{
		// Stores the penalty score of the best rectangle placement - bigger=worse, smaller=better.
		int bestScore = std::numeric_limits<int>::max();

		for(size_t i = 0; i < freeRectangles.size(); ++i)
		{
			for(size_t j = 0; j < rects.size(); ++j)
			{
				// If this rectangle is a perfect match, we pick it instantly.
				if (rects[j].w == freeRectangles[i].w && rects[j].h == freeRectangles[i].h)
				{
					bestFreeRect = i;
					bestRect = j;
					bestFlipped = false;
					bestScore = std::numeric_limits<int>::min();
					i = freeRectangles.size(); // Force a jump out of the outer loop as well - we got an instant fit.
					break;
				}
				// If flipping this rectangle is a perfect match, pick that then.
				else if (rects[j].h == freeRectangles[i].w && rects[j].w == freeRectangles[i].h)
				{
					bestFreeRect = i;
					bestRect = j;
					bestFlipped = true;
					bestScore = std::numeric_limits<int>::min();
					i = freeRectangles.size(); // Force a jump out of the outer loop as well - we got an instant fit.
					break;
				}
				// Try if we can fit the rectangle upright.
				else if (rects[j].w <= freeRectangles[i].w && rects[j].h <= freeRectangles[i].h)
				{
					int score = ScoreByHeuristic(rects[j].w, rects[j].h, freeRectangles[i], rectChoice);
					if (score < bestScore)
					{
						bestFreeRect = i;
						bestRect = j;
						bestFlipped = false;
						bestScore = score;
					}
				}
				// If not, then perhaps flipping sideways will make it fit?
				else if (rects[j].h <= freeRectangles[i].w && rects[j].w <= freeRectangles[i].h)
				{
					int score = ScoreByHeuristic(rects[j].h, rects[j].w, freeRectangles[i], rectChoice);
					if (score < bestScore)
					{
						bestFreeRect = i;
						bestRect = j;
						bestFlipped = true;
						bestScore = score;
					}
				}
			}
		}
		// If we didn't manage to find any rectangle to pack, abort.
		if (bestScore == std::numeric_limits<int>::max())	return;
		// Otherwise, we're good to go and do the actual packing.
		Cartesian newNode;
		newNode.x = freeRectangles[bestFreeRect].x;
		newNode.y = freeRectangles[bestFreeRect].y;
		newNode.w = rects[bestRect].w;
		newNode.h = rects[bestRect].h;
		if (bestFlipped)	std::swap(newNode.w, newNode.h);
		// Remove the free space we lost in the bin.
		SplitFreeRectByHeuristic(freeRectangles[bestFreeRect], newNode, splitMethod);
		freeRectangles.erase(freeRectangles.begin() + bestFreeRect);
		// Remove the rectangle we just packed from the input list.
		rects.erase(rects.begin() + bestRect);
		// Perform a Rectangle Merge step if desired.
		if (merge)	MergeFreeList();
		// Remember the new used rectangle.
		usedRectangles.push_back(newNode);
		// Check that we're really producing correct packings here.
		debug_assert(disjointRects.Add(newNode) == true);
	}
}

/// @return True if r fits inside freeRect (possibly rotated).
bool Fits(const Cartesian &r, const Cartesian &freeRect) {
	return (r.w <= freeRect.w && r.h <= freeRect.h) ||	(r.h <= freeRect.w && r.w <= freeRect.h);
}

/// @return True if r fits perfectly inside freeRect, i.e. the leftover area is 0.
bool FitsPerfectly(const Cartesian &r, const Cartesian &freeRect) {
	return (r.w == freeRect.w && r.h == freeRect.h) ||	(r.h == freeRect.w && r.w == freeRect.h);
}

Cartesian GuillotineBinPack::Insert(int width, int height, bool merge, FreeRectChoiceHeuristic rectChoice, 
	GuillotineSplitHeuristic splitMethod)
{
	// Find where to put the new rectangle.
	int freeNodeIndex = 0;
	Cartesian newRect = FindPositionForNewNode(width, height, rectChoice, &freeNodeIndex);

	// Abort if we didn't have enough space in the bin.
	if (newRect.h == 0)
		return newRect;

	// Remove the space that was just consumed by the new rectangle.
	SplitFreeRectByHeuristic(freeRectangles[freeNodeIndex], newRect, splitMethod);
	freeRectangles.erase(freeRectangles.begin() + freeNodeIndex);

	// Perform a Rectangle Merge step if desired.
	if (merge)
		MergeFreeList();

	// Remember the new used rectangle.
	usedRectangles.push_back(newRect);

	// Check that we're really producing correct packings here.
	debug_assert(disjointRects.Add(newRect) == true);

	return newRect;
}

/// Computes the ratio of used surface area to the total bin area.
float GuillotineBinPack::Occupancy() const
{
	///\todo The occupancy rate could be cached/tracked incrementally instead
	///      of looping through the list of packed rectangles here.
	unsigned long usedSurfaceArea = 0;
	for(size_t i = 0; i < usedRectangles.size(); ++i)
		usedSurfaceArea += usedRectangles[i].w * usedRectangles[i].h;

	return (float)usedSurfaceArea / (binWidth * binHeight);
}

/// Returns the heuristic score value for placing a rectangle of size width*height into freeRect. Does not try to rotate.
int GuillotineBinPack::ScoreByHeuristic(int width, int height, const Cartesian &freeRect, FreeRectChoiceHeuristic rectChoice)
{
	switch(rectChoice)
	{
	case RectBestAreaFit: return ScoreBestAreaFit(width, height, freeRect);
	case RectBestShortSideFit: return ScoreBestShortSideFit(width, height, freeRect);
	case RectBestLongSideFit: return ScoreBestLongSideFit(width, height, freeRect);
	case RectWorstAreaFit: return ScoreWorstAreaFit(width, height, freeRect);
	case RectWorstShortSideFit: return ScoreWorstShortSideFit(width, height, freeRect);
	case RectWorstLongSideFit: return ScoreWorstLongSideFit(width, height, freeRect);
	default: assert(false); return std::numeric_limits<int>::max();
	}
}

int GuillotineBinPack::ScoreBestAreaFit(int width, int height, const Cartesian &freeRect)
{
	return freeRect.w * freeRect.h - width * height;
}

int GuillotineBinPack::ScoreBestShortSideFit(int width, int height, const Cartesian &freeRect)
{
	int leftoverHoriz = abs(freeRect.w - width);
	int leftoverVert = abs(freeRect.h - height);
	int leftover = min(leftoverHoriz, leftoverVert);
	return leftover;
}

int GuillotineBinPack::ScoreBestLongSideFit(int width, int height, const Cartesian &freeRect)
{
	int leftoverHoriz = abs(freeRect.w - width);
	int leftoverVert = abs(freeRect.h - height);
	int leftover = max(leftoverHoriz, leftoverVert);
	return leftover;
}

int GuillotineBinPack::ScoreWorstAreaFit(int width, int height, const Cartesian &freeRect)
{
	return -ScoreBestAreaFit(width, height, freeRect);
}

int GuillotineBinPack::ScoreWorstShortSideFit(int width, int height, const Cartesian &freeRect)
{
	return -ScoreBestShortSideFit(width, height, freeRect);
}

int GuillotineBinPack::ScoreWorstLongSideFit(int width, int height, const Cartesian &freeRect)
{
	return -ScoreBestLongSideFit(width, height, freeRect);
}

Cartesian GuillotineBinPack::FindPositionForNewNode(int width, int height, FreeRectChoiceHeuristic rectChoice, int *nodeIndex)
{
	Cartesian bestNode;

	int bestScore = std::numeric_limits<int>::max();

	/// Try each free rectangle to find the best one for placement.
	for(size_t i = 0; i < freeRectangles.size(); ++i)
	{
		// If this is a perfect fit upright, choose it immediately.
		if (width == freeRectangles[i].w && height == freeRectangles[i].h)
		{
			bestNode.x = freeRectangles[i].x;
			bestNode.y = freeRectangles[i].y;
			bestNode.w = width;
			bestNode.h = height;
			bestScore = std::numeric_limits<int>::min();
			*nodeIndex = i;
			debug_assert(disjointRects.Disjoint(bestNode));
			break;
		}
		// If this is a perfect fit sideways, choose it.
		else if (height == freeRectangles[i].w && width == freeRectangles[i].h)
		{
			bestNode.x = freeRectangles[i].x;
			bestNode.y = freeRectangles[i].y;
			bestNode.w = height;
			bestNode.h = width;
			bestScore = std::numeric_limits<int>::min();
			*nodeIndex = i;
			debug_assert(disjointRects.Disjoint(bestNode));
			break;
		}
		// Does the rectangle fit upright?
		else if (width <= freeRectangles[i].w && height <= freeRectangles[i].h)
		{
			int score = ScoreByHeuristic(width, height, freeRectangles[i], rectChoice);

			if (score < bestScore)
			{
				bestNode.x = freeRectangles[i].x;
				bestNode.y = freeRectangles[i].y;
				bestNode.w = width;
				bestNode.h = height;
				bestScore = score;
				*nodeIndex = i;
				debug_assert(disjointRects.Disjoint(bestNode));
			}
		}
		// Does the rectangle fit sideways?
		else if (height <= freeRectangles[i].w && width <= freeRectangles[i].h)
		{
			int score = ScoreByHeuristic(height, width, freeRectangles[i], rectChoice);

			if (score < bestScore)
			{
				bestNode.x = freeRectangles[i].x;
				bestNode.y = freeRectangles[i].y;
				bestNode.w = height;
				bestNode.h = width;
				bestScore = score;
				*nodeIndex = i;
				debug_assert(disjointRects.Disjoint(bestNode));
			}
		}
	}
	return bestNode;
}

void GuillotineBinPack::SplitFreeRectByHeuristic(const Cartesian &freeRect, const Cartesian &placedRect, GuillotineSplitHeuristic method)
{
	// Compute the lengths of the leftover area.
	const int w = freeRect.w - placedRect.w;
	const int h = freeRect.h - placedRect.h;

	// Placing placedRect into freeRect results in an L-shaped free area, which must be split into
	// two disjoint rectangles. This can be achieved with by splitting the L-shape using a single line.
	// We have two choices: horizontal or vertical.	

	// Use the given heuristic to decide which choice to make.

	bool splitHorizontal;
	switch(method)
	{
	case SplitShorterLeftoverAxis:
		// Split along the shorter leftover axis.
		splitHorizontal = (w <= h);
		break;
	case SplitLongerLeftoverAxis:
		// Split along the longer leftover axis.
		splitHorizontal = (w > h);
		break;
	case SplitMinimizeArea:
		// Maximize the larger area == minimize the smaller area.
		// Tries to make the single bigger rectangle.
		splitHorizontal = (placedRect.w * h > w * placedRect.h);
		break;
	case SplitMaximizeArea:
		// Maximize the smaller area == minimize the larger area.
		// Tries to make the rectangles more even-sized.
		splitHorizontal = (placedRect.w * h <= w * placedRect.h);
		break;
	case SplitShorterAxis:
		// Split along the shorter total axis.
		splitHorizontal = (freeRect.w <= freeRect.h);
		break;
	case SplitLongerAxis:
		// Split along the longer total axis.
		splitHorizontal = (freeRect.w > freeRect.h);
		break;
	default:
		splitHorizontal = true;
		assert(false);
	}

	// Perform the actual split.
	SplitFreeRectAlongAxis(freeRect, placedRect, splitHorizontal);
}

/// This function will add the two generated rectangles into the freeRectangles array. The caller is expected to
/// remove the original rectangle from the freeRectangles array after that.
void GuillotineBinPack::SplitFreeRectAlongAxis(const Cartesian &freeRect, const Cartesian &placedRect, bool splitHorizontal)
{
	// Form the two new rectangles.
	Cartesian bottom;
	bottom.x = freeRect.x;
	bottom.y = freeRect.y + placedRect.h;
	bottom.h = freeRect.h - placedRect.h;

	Cartesian right;
	right.x = freeRect.x + placedRect.w;
	right.y = freeRect.y;
	right.w = freeRect.w - placedRect.w;

	if (splitHorizontal)
	{
		bottom.w = freeRect.w;
		right.h = placedRect.h;
	}
	else // Split vertically
	{
		bottom.w = placedRect.w;
		right.h = freeRect.h;
	}

	// Add the new rectangles into the free rectangle pool if they weren't degenerate.
	if (bottom.w > 0 && bottom.h > 0)
		freeRectangles.push_back(bottom);
	if (right.w > 0 && right.h > 0)
		freeRectangles.push_back(right);

	debug_assert(disjointRects.Disjoint(bottom));
	debug_assert(disjointRects.Disjoint(right));
}

void GuillotineBinPack::MergeFreeList()
{
#ifdef _DEBUG
	DisjointRectCollection test;
	for(size_t i = 0; i < freeRectangles.size(); ++i)
		assert(test.Add(freeRectangles[i]) == true);
#endif

	// Do a Theta(n^2) loop to see if any pair of free rectangles could me merged into one.
	// Note that we miss any opportunities to merge three rectangles into one. (should call this function again to detect that)
	for(size_t i = 0; i < freeRectangles.size(); ++i)
		for(size_t j = i+1; j < freeRectangles.size(); ++j)
		{
			if (freeRectangles[i].w == freeRectangles[j].w && freeRectangles[i].x == freeRectangles[j].x)
			{
				if (freeRectangles[i].y == freeRectangles[j].y + freeRectangles[j].h)
				{
					freeRectangles[i].y -= freeRectangles[j].h;
					freeRectangles[i].h += freeRectangles[j].h;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
				else if (freeRectangles[i].y + freeRectangles[i].h == freeRectangles[j].y)
				{
					freeRectangles[i].h += freeRectangles[j].h;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
			}
			else if (freeRectangles[i].h == freeRectangles[j].h && freeRectangles[i].y == freeRectangles[j].y)
			{
				if (freeRectangles[i].x == freeRectangles[j].x + freeRectangles[j].w)
				{
					freeRectangles[i].x -= freeRectangles[j].w;
					freeRectangles[i].w += freeRectangles[j].w;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
				else if (freeRectangles[i].x + freeRectangles[i].w == freeRectangles[j].x)
				{
					freeRectangles[i].w += freeRectangles[j].w;
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
			}
		}

#ifdef _DEBUG
	test.Clear();
	for(size_t i = 0; i < freeRectangles.size(); ++i)
		assert(test.Add(freeRectangles[i]) == true);
#endif
}

}
