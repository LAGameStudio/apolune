#pragma once

/*
 * $Header: /home/grantham/cvsroot/projects/modules/tc/tc.h,v 1.14 2000/10/03 07:19:20 grantham Exp $
 */

#if !defined(_ACTC_H_)
#define _ACTC_H_

#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <limits.h>

#if defined(ACTC_DEBUG)
#define ACTC_DEBUG_ASSERT(a) a
#else
#define ACTC_DEBUG_ASSERT(a)
#endif

#if defined(ACTC_INFO)
#define ACTC_INFO_ASSERT(a) a
#else
#define ACTC_INFO_ASSERT(a)
#endif

/*
    Abbreviated:
    	vertex		vert
	primitive	prim
	maximum		max
	minimum		min
	parameter	param
 */

#define ACTC_NO_ERROR			0
#define ACTC_ALLOC_FAILED		-0x2000
#define ACTC_DURING_INPUT		-0x2001
#define ACTC_DURING_OUTPUT		-0x2002
#define ACTC_IDLE			-0x2003
#define ACTC_INVALID_VALUE		-0x2004
#define ACTC_DATABASE_EMPTY		-0x2005
#define ACTC_DATABASE_CORRUPT		-0x2006
#define ACTC_PRIM_COMPLETE		-0x2007

#define ACTC_OUT_MIN_FAN_VERTS		0x1000
#define ACTC_OUT_HONOR_WINDING		0x1001
#define ACTC_OUT_MAX_PRIM_VERTS		0x1004
#define ACTC_IN_MIN_VERT		0x1005
#define ACTC_IN_MAX_VERT		0x1006
#define ACTC_IN_MAX_VERT_SHARING	0x1007
#define ACTC_IN_MAX_EDGE_SHARING	0x1008
#define ACTC_MINOR_VERSION		0x1009
#define ACTC_MAJOR_VERSION		0x1010

#define ACTC_PRIM_FAN			0x2000
#define ACTC_PRIM_STRIP			0x2001

#define ACTC_TRUE			1
#define ACTC_FALSE			0



#if !defined(MEM_CHART)
#define chartedSetLabel(a)
#endif

#define LEVEL1COUNT 16384
#define LEVEL2COUNT 1024
#define LEVEL3COUNT 256


#define ACTC_CHECK(a) \
    { \
        int theErrorNow; \
	theErrorNow = (a); \
	if(theErrorNow < 0) \
	    return theErrorNow; \
    }


struct ACTCTableLevel3
{
    int EntryCount;
    void *Table[LEVEL3COUNT];
    char IsSet[LEVEL3COUNT];
};

struct ACTCTableLevel2
{
    int EntryCount;
    ACTCTableLevel3 *Table[LEVEL2COUNT];
};

struct ACTCTableRoot
{
 size_t EntryCount;
 size_t TotalEntryCount;
 size_t TotalAllocatedBytes;
 int EmptyEntryCount;
 ACTCTableLevel2 *Table[LEVEL1COUNT];

 ACTCTableRoot(void) {
  EntryCount=TotalEntryCount=TotalAllocatedBytes=0;
  EmptyEntryCount=0;
  for ( int i=0; i<LEVEL1COUNT; i++ ) Table[i]=NULL;
  chartedSetLabel("table root");
 }

 int Retrieve(unsigned int a, void **ref)
 {
  int i1 = a / (LEVEL2COUNT * LEVEL3COUNT);
  int i2 = (a / LEVEL3COUNT) % LEVEL2COUNT;
  int i3 = a % LEVEL3COUNT;
 
  if(Table[i1] == NULL) return 0;
  if(Table[i1]->Table[i2] == NULL) return 0;
  if(!Table[i1]->Table[i2]->IsSet[i3]) return 0;
  if(ref != NULL) *ref = Table[i1]->Table[i2]->Table[i3];
  return 1;
 }
 
 int Insert(unsigned int a, void *ref) {
  int i1 = a / (LEVEL2COUNT * LEVEL3COUNT);
  int i2 = (a / LEVEL3COUNT) % LEVEL2COUNT;
  int i3 = a % LEVEL3COUNT;
 
  if(Table[i1] == NULL) {
  	chartedSetLabel("table level 2");
   Table[i1] = new ACTCTableLevel2;
  	TotalAllocatedBytes += sizeof(ACTCTableLevel2);
   if(Table[i1] == NULL) return 0;
  }
  if(Table[i1]->Table[i2] == NULL) {
  	chartedSetLabel("table level 3");
   Table[i1]->Table[i2] = new ACTCTableLevel3;
 	 TotalAllocatedBytes += sizeof(ACTCTableLevel3);
   if(Table[i1]->Table[i2] == NULL) return 0;
 	 Table[i1]->EntryCount++;
 	 TotalEntryCount += LEVEL3COUNT;
 	 EmptyEntryCount += LEVEL3COUNT;
  }
  if(!Table[i1]->Table[i2]->IsSet[i3]) {
  	Table[i1]->Table[i2]->EntryCount++;
 	 EmptyEntryCount --;
 	 Table[i1]->Table[i2]->IsSet[i3] = 1;
  }
  Table[i1]->Table[i2]->Table[i3] = ref;
  return 1;
 }
 
 int Remove(unsigned int a, void **wasref)
 {
  int i1 = a / (LEVEL2COUNT * LEVEL3COUNT);
  int i2 = (a / LEVEL3COUNT) % LEVEL2COUNT;
  int i3 = a % LEVEL3COUNT;
 
  if(Table[i1] == NULL)	return 0;
  if(Table[i1]->Table[i2] == NULL)	return 0;
  if(!Table[i1]->Table[i2]->IsSet[i3]) return 0;
  if(wasref != NULL) *wasref = Table[i1]->Table[i2]->Table[i3];
  Table[i1]->Table[i2]->IsSet[i3] = 0;
  EmptyEntryCount ++;
  if(--Table[i1]->Table[i2]->EntryCount == 0) {
  	EmptyEntryCount -= LEVEL3COUNT;
  	TotalEntryCount -= LEVEL3COUNT;
  	free(Table[i1]->Table[i2]);
  	TotalAllocatedBytes -= sizeof(ACTCTableLevel3);
  	Table[i1]->Table[i2] = NULL;
  	if(--Table[i1]->EntryCount == 0) {
 	  TotalAllocatedBytes -= sizeof(ACTCTableLevel2);
 	  free(Table[i1]);
 	  Table[i1] = NULL;
  	}
  }
  return 1;
 }

 void Delete() {
  int i1, i2, i3;
  for(i1 = 0; i1 < LEVEL1COUNT; i1++) {
   if(Table[i1] != NULL) {
	   for(i2 = 0; i2 < LEVEL2COUNT; i2++) {
	   	if(Table[i1]->Table[i2] != NULL) {
		    for(i3 = 0; i3 < LEVEL3COUNT; i3++) {
	     	if(Table[i1]->Table[i2]->IsSet[i3]) delete Table[i1]->Table[i2]->Table[i3];
		    }
		    delete Table[i1]->Table[i2];
	   	}
	   }
	   delete Table[i1];
	   Table[i1] = NULL;
  	}
  }
  TotalEntryCount = 0;
  EmptyEntryCount = 0;
  TotalAllocatedBytes = 0;
}

 void GetStats(size_t *totalBytes, size_t *emptyCount, size_t *totalCount) {
  if(emptyCount != NULL) *emptyCount = EmptyEntryCount;
  if(totalCount != NULL) *totalCount = TotalEntryCount;
  if(totalBytes != NULL) *totalBytes = TotalAllocatedBytes;
 }

};

struct ACTCTableIterator {
 int i1, i2, i3;
 unsigned int i;
 ACTCTableLevel3 *CurLevel3;
 int CheckLevel1, CheckLevel2;

 ACTCTableIterator(void) {
  i1=i2=i3=0;
  i=0;
  CurLevel3=NULL;
  CheckLevel1=CheckLevel2=0;
  chartedSetLabel("table iterator");
  Reset();
 }

 void Reset() {
  i1 = 0;
  i2 = 0;
  i3 = 0;
  i = 0;
  CheckLevel1 = 1;
  CheckLevel2 = 1;
 }

 int Iterate(ACTCTableRoot *table, unsigned int *I, void **ref) {
  int done = 0;
  while(i1 < LEVEL1COUNT) {
   if(CheckLevel1 && table->Table[i1] == NULL) {
    i += LEVEL2COUNT * LEVEL3COUNT;
    i1++;
    continue;
  	} else CheckLevel1 = 0;
   if(CheckLevel2 && table->Table[i1]->Table[i2] == NULL) {
	   i += LEVEL3COUNT;
	   if(++i2 >= LEVEL2COUNT) {
	   	i2 = 0;
	   	i1++;
	   	CheckLevel1 = 1;
	   }
	   continue;
  	} else CheckLevel2 = 0;
	  if(i3 == 0) CurLevel3 = table->Table[i1]->Table[i2];
  	if(CurLevel3->IsSet[i3]) {
    if(ref != NULL)	*ref = CurLevel3->Table[i3];
	    if(i != NULL) *I = i;
	    done = 1;
   }
   i++;
   if(++i3 >= LEVEL3COUNT) {
	   i3 = 0;
	   CheckLevel2 = 1;
	   if(++i2 >= LEVEL2COUNT) {
	   	i2 = 0;
   		i1++;
   		CheckLevel1 = 1;
	   }
   }
   if(done) return 1;
  }
  return 0;
 }
};


struct ACTCTriangle {
 struct ACTCVertex *FinalVert;

};

struct ACTCEdge {
 struct ACTCVertex *V2;
 int Count;
 unsigned int TriangleCount;
 ACTCTriangle *Triangles;
 
 inline size_t Allocated() { return sizeof(ACTCTriangle) * TriangleCount; }
};

struct ACTCVertex {
 unsigned int V;
 int Count;
 struct ACTCVertex *PointsToMe;
 struct ACTCVertex *Next;
 unsigned int EdgeCount;
 ACTCEdge *Edges;	

 inline size_t Allocated() {
  unsigned int i;
  size_t size = sizeof(ACTCEdge) * EdgeCount;
  for(i = 0; i < EdgeCount; i++) {
  	size += Edges[i].Allocated();
  }
  return size;
 }
};

class ACTC {
 
 /* vertex and edge database */
 unsigned int VertexCount;
 ACTCTableRoot *Vertices;
 ACTCTableIterator *VertexIterator;
 int CurMaxVertValence;
 int CurMinVertValence;
 ACTCVertex *VertexBins;

 /* alternate vertex array if range small enough */
 ACTCVertex *StaticVerts;
 int UsingStaticVerts;
 unsigned int VertRange;

 /* During consolidation */
 int CurWindOrder;
 int PrimType;
 ACTCVertex *V1;
 ACTCVertex *V2;
 int VerticesSoFar;

 /* Error and state handling */
 int IsInputting;
 int IsOutputting;
 int Error;

 /* actcParam-settable parameters */
 unsigned int MinInputVert;
 unsigned int MaxInputVert;
 int MaxVertShare;
 int MaxEdgeShare;
 int MinFanVerts;
 int MaxPrimVerts;
 int HonorWinding;
 
 ACTC(void) {
#if defined(ACTC_DEBUG) || defined(ACTC_INFO)
   static int didPrintVersion = 0;
 
   if(!didPrintVersion) {
   	int verMinor, verMajor;
   	didPrintVersion = 1;
    actcGetParami(tc, ACTC_MAJOR_VERSION, &verMajor);
    actcGetParami(tc, ACTC_MINOR_VERSION, &verMinor);
   	fprintf(stderr, "TC Version %d.%d\n", verMajor, verMinor);
   }
#endif /* defined(DEBUG) || defined(INFO) */
 
   chartedSetLabel("the tc struct");
 
   Vertices = new ACTCTableRoot;
   VertexIterator = new ACTCTableIterator;
 
   MinFanVerts = INT_MAX;
   MaxPrimVerts = INT_MAX;
   MaxInputVert = INT_MAX;
   MaxEdgeShare = INT_MAX;
   MaxVertShare = INT_MAX;
   HonorWinding = 1;
     /* seed = 0 handled by calloc */
     /* XXX grantham 20000615 - seed ignored for now */
 }
 
 int GetError() {
  int error = Error;
  Error = ACTC_NO_ERROR;
  return error;
 }
 
/*
 * Call only during input; changes vertices' valences and does not
 * fix the bins that are ordered by vertex valence.  (It's usually cheaper
 * to traverse the vertex list once after all are added, since that's
 * linear in the NUMBER OF UNIQUE VERTEX INDICES, which is almost always
 * going to be less than the number of vertices.)
 */
 int incVertexValence(unsigned int v, ACTCVertex **found) {
  ACTCVertex *vertex;
  if(UsingStaticVerts) {
   vertex = &StaticVerts[v];
	  vertex->Count++;
	  if(vertex->Count == 1) {
	   vertex->V = v;
	   VertexCount++;
	  }
  } else {
  	if(Vertices->Retrieve(v, (void **)&vertex) == 1) {
	   if(vertex->V != v) {
		   ACTC_DEBUG_ASSERT(
		    fprintf(stderr, "ACTC::incVertexValence : Got vertex %d when looking for vertex %d?!?\n", vertex->V, v);
		    abortWithOptionalDump(tc);
   		)
	   	return Error = ACTC_DATABASE_CORRUPT;
    }
	   vertex->Count++;
	  } else {
	   chartedSetLabel("new Vertex");
	   vertex = new ACTCVertex;
	   vertex->V = v;
	   vertex->Count = 1;
	   vertex->Edges = NULL;
	   vertex->EdgeCount = 0;
	   if(Vertices->Insert(v, vertex) == 0) {
	   	ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::incVertexValence : Failed to insert vertex into table\n");)
   		return Error = ACTC_ALLOC_FAILED;
	   }
	   VertexCount++;
	  }
  }
  if(vertex->Count > CurMaxVertValence)	CurMaxVertValence = vertex->Count;
  *found = vertex;
  return ACTC_NO_ERROR;
 }

 int decVertexValence(ACTCVertex **vptr) {
  ACTCVertex *v = *vptr;
  v->Count--;
  if(v->Count < 0) {
  	ACTC_DEBUG_ASSERT(
    fprintf(stderr, "ACTC::decVertexValence : Valence went negative?!?\n");
	   abortWithOptionalDump(tc);
  	)
	  return Error = ACTC_DATABASE_CORRUPT;
  }
    
  if(v->PointsToMe != NULL) {
  	*v->PointsToMe = v->Next;
  	if(v->Next != NULL) v->Next->PointsToMe = v->PointsToMe;
  	v->Next = NULL;
  }

  if(v->Count == 0) {
  	VertexCount--;
  	if(v->Edges != NULL) delete v->Edges;
   if(!UsingStaticVerts) {
	   Vertices->Remove(v->V, NULL);
    delete v;
  	}
	  *vptr = NULL;
  } else {
	  if(VertexBins != NULL) {
	   v->Next = &VertexBins[v->Count];
	   v->PointsToMe = &VertexBins[v->Count];
	   if(v->Next != NULL) v->Next->PointsToMe = v->Next;
	   VertexBins[v->Count] = *v;
	   if(v->Count < CurMinVertValence) CurMinVertValence = v->Count;
   }
  }
  return ACTC_NO_ERROR;
 }

 int findNextFanVertex(ACTCVertex **vert) {
  if(CurMaxVertValence < MinFanVerts) return ACTC_NO_MATCHING_VERT;
  while(VertexBins[CurMaxVertValence] == NULL) {
  	CurMaxVertValence--;
 	 if(CurMaxVertValence < CurMinVertValence) {
	   if(VertexCount > 0) {
		   ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::findNextFanVertex : no more vertices in bins but VertexCount > 0\n");)
   		return Error = ACTC_DATABASE_CORRUPT;
	   }
	   return ACTC_NO_MATCHING_VERT;
	  }
  }
  *vert = VertexBins[CurMaxVertValence];
  return ACTC_NO_ERROR;
 }

 int findNextStripVertex(ACTCVertex **vert) {
  while(VertexBins[CurMinVertValence] == NULL) {
  	CurMinVertValence++;
	  if(CurMinVertValence > CurMaxVertValence) {
	   if(VertexCount > 0) {
   		ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::findNextStripVertex : no more vertices in bins but VertexCount > 0\n");)
  	 	return Error = ACTC_DATABASE_CORRUPT;
    }
    return ACTC_NO_MATCHING_VERT;
  	}
  }
  *vert = &VertexBins[CurMinVertValence];
  return ACTC_NO_ERROR;
 }

 int GetIsDuringInput() {
  return IsInputting;
 }

 int Begin() {
  if(IsOutputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::Begin : called within BeginOutput/EndOutput\n");)
  	return Error = ACTC_DURING_INPUT;
  }
  if(IsInputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::Begin : called within BeginInput/EndInput\n");)
  	return Error = ACTC_DURING_INPUT;
  }
  IsInputting = 1;
  CurMaxVertValence = 0;
  if(MaxInputVert < MAX_STATIC_VERTS - 1) {
  	size_t byteCount;
	  UsingStaticVerts = 1;
	  VertRange = MaxInputVert + 1;
  	byteCount = sizeof(ACTCVertex) * VertRange;
  	chartedSetLabel("static verts");
  	StaticVerts = new ACTCVertex[VertRange];
  	if(StaticVerts == NULL) {
    ACTC_INFO_ASSERT(printf("Couldn't allocate static %d vert block of %u bytes\n", VertRange, byteCount);)
    UsingStaticVerts = 0;
  	}
  } else UsingStaticVerts = 0;
  return ACTC_NO_ERROR;
 }

 int End() {
  if(IsOutputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::End : called within BeginOutput/EndOutput\n");)
  	return Error = ACTC_DURING_OUTPUT;
  }
  if(!IsInputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::End : called outside BeginInput/EndInput\n");)
  	return Error = ACTC_IDLE;
  }
  IsInputting = 0;
  return ACTC_NO_ERROR;
 }

 int actcGetIsDuringOutput() {
  return IsOutputting;
 }

 int actcBeginOutput()
 {
  ACTCVertex *v;
  unsigned int i;
  if(IsInputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcBeginOutput : called within BeginInput/EndInput\n");)
  	return Error = ACTC_DURING_INPUT;
  }
  if(IsOutputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcBeginOutput : called within BeginOutput/EndOutput\n");)
  	return tc->Error = ACTC_DURING_OUTPUT;
  }
  IsOutputting = 1;
  CurMinVertValence = INT_MAX;
  chartedSetLabel("vertex bins");
  VertexBins = new ACTCVertex[CurMaxVertValence + 1];
  if(VertexBins == NULL) {
	  ACTC_DEBUG_ASSERT(fprintf(stderr, "actcBeginOutput : couldn't allocate %d bytes for Vertex Bins\n",
    sizeof(ACTCVertex *) * tc->CurMaxVertValence);)
  	return tc->Error = ACTC_ALLOC_FAILED;
  }
  if(UsingStaticVerts) {
   double edgeTotal=0.0;
	  for(i = 0; i < VertRange; i++) {
	   v = &StaticVerts[i];
	   if(v->Count > 0) {
		   v->Next = &VertexBins[v->Count];
	  	 v->PointsToMe = &VertexBins[v->Count];
		   VertexBins[v->Count] = v;
	  	 if(v->Next != NULL) v->Next->PointsToMe = &v->Next;
	  	 if(v->Count < CurMinVertValence) CurMinVertValence = v->Count;
  		 edgeTotal += v->EdgeCount;
	   }
	  }
  } else {
  	VertexIterator->Reset();
  	for(i = 0; i < VertexCount; i++) {
	   if(VertexIterator->Iterate(Vertices, NULL, (void **)&v)	== 0) {
   		ACTC_DEBUG_ASSERT(fprintf(stderr, "actcBeginOutput : fewer vertices in the table than expected!\n");)
   		return Error = ACTC_DATABASE_CORRUPT;
    }
    v->Next = VertexBins[v->Count];
    v->PointsToMe = &VertexBins[v->Count];
    VertexBins[v->Count] = v;
    if(v->Next != NULL) v->Next->PointsToMe = &v->Next;
	   if(v->Count < CurMinVertValence) CurMinVertValence = v->Count;
  	}
  }
  return ACTC_NO_ERROR;
 }

 int actcEndOutput() {
  if(IsInputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcEndOutput : called within BeginInput/EndInput\n");)
  	return Error = ACTC_DURING_INPUT;
  }
  if(!IsOutputting) {
  	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcEndOutput : called outside BeginOutput/EndOutput\n");)
  	return Error = ACTC_IDLE;
  }
  IsOutputting = 0;
  if(UsingStaticVerts) {
	  delete StaticVerts;
	  StaticVerts = NULL;
	  UsingStaticVerts = 0;
  }
  delete VertexBins;
  VertexBins = NULL;
  return ACTC_NO_ERROR;
 }
 
 size_t Allocated() {
  unsigned int i;
  unsigned int size=0;
  ACTCVertex *v;

  if(!UsingStaticVerts) {
   VertexIterator->Reset();
  	for(i = 0; i < VertexCount; i++) {
	    VertexIterator->Iterate(Vertices, NULL, (void **)&v);
	    size += v->Allocated();
	  }
  } else {
   size = VertRange * sizeof(ACTCVertex);
   for(i = 0; i < VertRange; i++) {
    v = &StaticVerts[i];
    if(v->Count > 0) size += v->Allocated();
  	}
  } 
  return size;
 }

 int GetMemoryAllocation(size_t *bytesAllocated)
 {
  size_t tableBytes;
  Vertices->GetStats(NULL, NULL, &tableBytes);
  *bytesAllocated = sizeof(ACTC);
  *bytesAllocated += tableBytes;
  *bytesAllocated += Allocated(); /* recurses */

  return ACTC_NO_ERROR;
 }

 void freeVertex(ACTCVertex *p) {
  ACTCVertex *v = p;
  unsigned int i;

  for(i = 0; i < v->EdgeCount; i++)
      free(v->Edges[i].Triangles);
  free(v->Edges);
  free(v);
 }

 int actcMakeEmpty() {
  VertexCount = 0;
  if(!UsingStaticVerts) Vertices->Delete(freeVertex);
   if(VertexBins != NULL) delete VertexBins;
  	VertexBins = NULL;
  }
  IsOutputting = 0;
  IsInputting = 0;
  return ACTC_NO_ERROR;
 }

void actcDelete(ACTCData *tc)
{
    actcMakeEmpty(tc);
    free(tc->VertexIterator);
    free(tc->Vertices);
    free(tc);
}

int actcParami(ACTCData *tc, unsigned int param, unsigned int value)
{
    if(tc->IsInputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcParami : within BeginInput/"
	    "EndInput\n");)
	return tc->Error = ACTC_DURING_INPUT;
    }
    if(tc->IsOutputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcParami : within BeginOutput/"
	    "EndOutput\n");)
	return tc->Error = ACTC_DURING_OUTPUT;
    }
    switch(param) {
	case ACTC_OUT_MIN_FAN_VERTS:
	    tc->MinFanVerts = value;
	    break;

	case ACTC_IN_MAX_VERT:
	    if(value < tc->MinInputVert) {
		ACTC_DEBUG_ASSERT(fprintf(stderr, "actcParami : tried to set "
		    "MAX_INPUT_VERT to %d, less than MIN_INPUT_VERT (%d)\n",
		    value, tc->MinInputVert);)
		return tc->Error = ACTC_INVALID_VALUE;
	    }
	    tc->MaxInputVert = value;
	    break;

	case ACTC_IN_MIN_VERT:
	    if(value > tc->MaxInputVert) {
		ACTC_DEBUG_ASSERT(fprintf(stderr, "actcParami : tried to set "
		    "MIN_INPUT_VERT to %d, greater than MAX_INPUT_VERT (%d)\n",
		    value, tc->MaxInputVert);)
		return tc->Error = ACTC_INVALID_VALUE;
	    }
	    tc->MinInputVert = value;
	    break;

	case ACTC_IN_MAX_EDGE_SHARING:
	    tc->MaxEdgeShare = value;
	    break;

	case ACTC_IN_MAX_VERT_SHARING:
	    tc->MaxVertShare = value;
	    break;

	case ACTC_OUT_HONOR_WINDING:
	    tc->HonorWinding = value;
	    break;

	case ACTC_OUT_MAX_PRIM_VERTS:
	    if(value < 3) {
		ACTC_DEBUG_ASSERT(fprintf(stderr, "actcParami : tried to set "
		    "MAX_PRIM_VERTS to %d (needed to be 3 or more)\n", value);)
		return tc->Error = ACTC_INVALID_VALUE;
	    }
	    tc->MaxPrimVerts = value;
	    break;

    }
    return ACTC_NO_ERROR;
}

int actcParamu(ACTCData *tc, unsigned int param, unsigned int value)
{
    /*
     * XXX - yes, this is questionable, but I consulted industry
     * experts and we agreed that most common behavior is to copy the
     * bits directly, which is what I want.
     */
    return actcParami(tc, param, (int)value);
}

int actcGetParami(ACTCData *tc, int param, int *value)
{
    switch(param) {
	case ACTC_MAJOR_VERSION:
	    *value = 1;
	    break;

	case ACTC_MINOR_VERSION:
	    *value = 1;
	    break;

	case ACTC_IN_MAX_VERT:
	    *value = tc->MaxInputVert;
	    break;

	case ACTC_IN_MIN_VERT:
	    *value = tc->MinInputVert;
	    break;

	case ACTC_IN_MAX_EDGE_SHARING:
	    *value = tc->MaxEdgeShare;
	    break;

	case ACTC_IN_MAX_VERT_SHARING:
	    *value = tc->MaxVertShare;
	    break;

	case ACTC_OUT_MIN_FAN_VERTS:
	    *value = tc->MinFanVerts;
	    break;

	case ACTC_OUT_HONOR_WINDING:
	    *value = tc->HonorWinding;
	    break;

	case ACTC_OUT_MAX_PRIM_VERTS:
	    *value = tc->MaxPrimVerts;
	    break;

	default:
	    *value = 0;
	    return tc->Error = ACTC_INVALID_VALUE;
	    /* break; */
    }
    return ACTC_NO_ERROR;
}

int actcGetParamu(ACTCData *tc, int param, unsigned int *value)
{
    return actcGetParami(tc, param, (int *)value);
}

static int mapEdgeTriangle(ACTCData *tc, ACTCEdge *edge, ACTCVertex *v3)
{
    ACTCTriangle tmp;
    void *r;

    tmp.FinalVert = v3;
    chartedSetLabel("triangle list");
    r = reallocAndAppend((void **)&edge->Triangles, &edge->TriangleCount,
        sizeof(tmp), &tmp);
    if(r == NULL) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::mapEdgeTriangle : Couldn't allocate "
	    "%d bytes for triangles\n", sizeof(tmp) *
	    (edge->TriangleCount + 1));)
	return tc->Error = ACTC_ALLOC_FAILED;
    }

    return ACTC_NO_ERROR;
}

static int unmapEdgeTriangle(ACTCData *tc, ACTCEdge *edge, ACTCVertex *v3)
{
    unsigned int i;

    for(i = 0; i < edge->TriangleCount; i++)
	if(edge->Triangles[i].FinalVert == v3)
	    break;

    if(i == edge->TriangleCount) {
	ACTC_DEBUG_ASSERT(
	    fprintf(stderr, "ACTC::unmapEdgeTriangle : Couldn't find third vertex"
	        " from edge in order to delete it?!?\n");
	    abortWithOptionalDump(tc);
	)
	return tc->Error = ACTC_DATABASE_CORRUPT;
    }

    edge->Triangles[i] = edge->Triangles[edge->TriangleCount - 1];
    edge->TriangleCount --;

    return ACTC_NO_ERROR;
}

static int mapVertexEdge(ACTCData *tc, ACTCVertex *v1, ACTCVertex *v2, ACTCEdge **edge)
{
    unsigned int i;
    ACTCEdge tmp;
    void *r;

    for(i = 0; i < v1->EdgeCount; i++)
        if(v1->Edges[i].V2 == v2) {
	    v1->Edges[i].Count++;
	    break;
	}

    if(i == v1->EdgeCount) {

	tmp.V2 = v2;
	tmp.Count = 1;
	tmp.Triangles = NULL;
	tmp.TriangleCount = 0;

	chartedSetLabel("vert-to-edge mapping");
	r = reallocAndAppend((void **)&v1->Edges, &v1->EdgeCount,
	    sizeof(tmp), &tmp);
	if(r == NULL) {
	    ACTC_DEBUG_ASSERT(fprintf(stderr, "ACTC::mapVertexEdge : Couldn't reallocate "
	        "to %d bytes for vertex's edge list\n", sizeof(tmp) *
		v1->EdgeCount);)
	    return tc->Error = ACTC_ALLOC_FAILED;
	}
    }
    *edge = &v1->Edges[i];

    return ACTC_NO_ERROR;
}

static int unmapVertexEdge(ACTCData *tc, ACTCVertex *v1, ACTCVertex *v2)
{
    unsigned int i;

    for(i = 0; i < v1->EdgeCount; i++)
	if(v1->Edges[i].V2 == v2)
	    break;

    if(i == v1->EdgeCount) {
	ACTC_DEBUG_ASSERT(
	    fprintf(stderr, "ACTC::unmapVertexEdge : Couldn't find edge %d,%d"
	        " from vertex in order to unmap it?!?\n", v1->V, v2->V);
	    abortWithOptionalDump(tc);
	)
	return tc->Error = ACTC_DATABASE_CORRUPT;
    }

    v1->Edges[i].Count --;
    if(v1->Edges[i].Count == 0) {
        if(v1->Edges[i].Triangles != NULL)
	    free(v1->Edges[i].Triangles);
	v1->Edges[i] = v1->Edges[v1->EdgeCount - 1];
	v1->EdgeCount --;
    }

    return ACTC_NO_ERROR;
}

int actcAddTriangle(ACTCData *tc, unsigned int v1, unsigned int v2, unsigned int v3)
{
    ACTCVertex *vertexRec1;
    ACTCVertex *vertexRec2;
    ACTCVertex *vertexRec3;

    ACTCEdge *edge12;
    ACTCEdge *edge23;
    ACTCEdge *edge31;

    if(tc->IsOutputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcAddTriangle : inside "
	    "BeginOutput/EndOutput\n");)
	return tc->Error = ACTC_IDLE;
    }
    if(!tc->IsInputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcAddTriangle : outside "
	    "BeginInput/EndInput\n");)
	return tc->Error = ACTC_DURING_INPUT;
    }

    if(incVertexValence(tc, v1, &vertexRec1) != ACTC_NO_ERROR) goto returnError1;
    if(incVertexValence(tc, v2, &vertexRec2) != ACTC_NO_ERROR) goto free1;
    if(incVertexValence(tc, v3, &vertexRec3) != ACTC_NO_ERROR) goto free2;

    if(mapVertexEdge(tc, vertexRec1, vertexRec2, &edge12) != ACTC_NO_ERROR)
        goto free3;
    if(mapVertexEdge(tc, vertexRec2, vertexRec3, &edge23) != ACTC_NO_ERROR)
        goto free4;
    if(mapVertexEdge(tc, vertexRec3, vertexRec1, &edge31) != ACTC_NO_ERROR)
        goto free5;

    if(mapEdgeTriangle(tc, edge12, vertexRec3) != ACTC_NO_ERROR) goto free6;
    if(mapEdgeTriangle(tc, edge23, vertexRec1) != ACTC_NO_ERROR) goto free7;
    if(mapEdgeTriangle(tc, edge31, vertexRec2) != ACTC_NO_ERROR) goto free8;

    return ACTC_NO_ERROR;

    /*
     * XXX Unfortunately, while backing out during the following
     * statements, we might encounter errors in the database which
     * will not get returned properly to the caller; I take heart in
     * the fact that if such an error occurs, TC is just a moment from
     * core dumping anyway. XXX grantham 20000615
     */

free8:
    unmapEdgeTriangle(tc, edge23, vertexRec1);
free7:
    unmapEdgeTriangle(tc, edge12, vertexRec3);
free6:
    unmapVertexEdge(tc, vertexRec3, vertexRec1);
free5:
    unmapVertexEdge(tc, vertexRec2, vertexRec3);
free4:
    unmapVertexEdge(tc, vertexRec1, vertexRec2);
free3:
    decVertexValence(tc, &vertexRec3);
free2:
    decVertexValence(tc, &vertexRec2);
free1:
    decVertexValence(tc, &vertexRec1);
returnError1:
    return tc->Error;
}

int actcStartNextPrim(ACTCData *tc, unsigned int *v1Return, unsigned int *v2Return)
{
    ACTCVertex *v1 = NULL;
    ACTCVertex *v2 = NULL;
    int findResult;

    if(tc->IsInputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcStartNextPrim : within "
	    "BeginInput/EndInput\n");)
	return tc->Error = ACTC_DURING_INPUT;
    }
    if(!tc->IsOutputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcStartNextPrim : outside "
	    "BeginOutput/EndOutput\n");)
	return tc->Error = ACTC_IDLE;
    }

    findResult = findNextFanVertex(tc, &v1);
    if(findResult == ACTC_NO_ERROR)
	tc->PrimType = ACTC_PRIM_FAN;
    else if(findResult != ACTC_NO_MATCHING_VERT) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcStartNextPrim : internal "
	    "error finding next appropriate vertex\n");)
	return tc->Error = findResult;
    } else {
	findResult = findNextStripVertex(tc, &v1);
	if(findResult != ACTC_NO_ERROR && findResult != ACTC_NO_MATCHING_VERT) {
	    ACTC_DEBUG_ASSERT(fprintf(stderr, "actcStartNextPrim : internal "
		"error finding next appropriate vertex\n");)
	    return tc->Error = findResult;
	}
	tc->PrimType = ACTC_PRIM_STRIP;
    }

    if(findResult == ACTC_NO_MATCHING_VERT) {
	*v1Return = -1;
	*v2Return = -1;
	return tc->Error = ACTC_DATABASE_EMPTY;
    }

    v2 = v1->Edges[0].V2;

    tc->CurWindOrder = ACTC_FWD_ORDER;
    tc->VerticesSoFar = 2;

    tc->V1 = v1;
    tc->V2 = v2;

    *v1Return = v1->V;
    *v2Return = v2->V;

    ACTC_INFO_ASSERT(printf("starting with edge %u, %u\n", tc->V1->V, tc->V2->V);)

    return tc->PrimType;
}

static int findEdge(ACTCVertex *v1, ACTCVertex *v2, ACTCEdge **edge)
{
    unsigned int i;

    for(i = 0; i < v1->EdgeCount; i++)
        if(v1->Edges[i].V2 == v2) {
	    *edge = &v1->Edges[i];
	    return 1;
	}
    return 0;
}

int unmapEdgeTriangleByVerts(ACTCData *tc, ACTCVertex *v1, ACTCVertex *v2,
    ACTCVertex *v3)
{
    ACTCEdge *e;

    ACTC_CHECK(findEdge(v1, v2, &e));
    unmapEdgeTriangle(tc, e, v3);
    return ACTC_NO_ERROR;
}

int actcGetNextVert(ACTCData *tc, unsigned int *vertReturn)
{
    ACTCEdge *edge;
    int wasEdgeFound = 0;
    ACTCVertex *thirdVertex;
    int wasFoundReversed;

    if(tc->IsInputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcGetNextVert : within BeginInput/"
	    "EndInput\n");)
	return tc->Error = ACTC_DURING_INPUT;
    }
    if(!tc->IsOutputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcGetNextVert : outside BeginOutput/"
	    "EndOutput\n");)
	return tc->Error = ACTC_IDLE;
    }
    if(tc->PrimType == -1) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcGetNextVert : Asked for next vertex "
	    "without a primitive (got last\n    vertex already?)\n");)
	return tc->Error = ACTC_INVALID_VALUE;
    }

    if(tc->VerticesSoFar >= tc->MaxPrimVerts) {
	tc->PrimType = -1;
	return tc->Error = ACTC_PRIM_COMPLETE;
    }

    if(tc->V1 == NULL || tc->V2 == NULL) {
	tc->PrimType = -1;
	return tc->Error = ACTC_PRIM_COMPLETE;
    }

    ACTC_INFO_ASSERT(printf("looking for edge %u, %u\n", tc->V1->V, tc->V2->V);)

    wasFoundReversed = 0;

    if(findEdge(tc->V1, tc->V2, &edge) != 0) {
	wasEdgeFound = 1;
    } else if(!tc->HonorWinding) {
	wasFoundReversed = 1;
	if(findEdge(tc->V2, tc->V1, &edge) != 0) {
	    wasEdgeFound = 1;
	}
    }

    if(!wasEdgeFound) {
	tc->PrimType = -1;
	return tc->Error = ACTC_PRIM_COMPLETE;
    }

    thirdVertex = edge->Triangles[edge->TriangleCount - 1].FinalVert;

    ACTC_INFO_ASSERT(printf("third vertex = %u\n", thirdVertex->V);)
    *vertReturn = thirdVertex->V;

    if(wasFoundReversed) {
	ACTC_CHECK(unmapEdgeTriangle(tc, edge, thirdVertex));
	ACTC_CHECK(unmapEdgeTriangleByVerts(tc, tc->V1, thirdVertex, tc->V2));
	ACTC_CHECK(unmapEdgeTriangleByVerts(tc, thirdVertex, tc->V2, tc->V1));
	ACTC_CHECK(unmapVertexEdge(tc, tc->V2, tc->V1));
	ACTC_CHECK(unmapVertexEdge(tc, tc->V1, thirdVertex));
	ACTC_CHECK(unmapVertexEdge(tc, thirdVertex, tc->V2));
    } else {
	ACTC_CHECK(unmapEdgeTriangle(tc, edge, thirdVertex));
	ACTC_CHECK(unmapEdgeTriangleByVerts(tc, tc->V2, thirdVertex, tc->V1));
	ACTC_CHECK(unmapEdgeTriangleByVerts(tc, thirdVertex, tc->V1, tc->V2));
	ACTC_CHECK(unmapVertexEdge(tc, tc->V1, tc->V2));
	ACTC_CHECK(unmapVertexEdge(tc, tc->V2, thirdVertex));
	ACTC_CHECK(unmapVertexEdge(tc, thirdVertex, tc->V1));
    }
    ACTC_CHECK(decVertexValence(tc, &tc->V1));
    ACTC_CHECK(decVertexValence(tc, &tc->V2));
    ACTC_CHECK(decVertexValence(tc, &thirdVertex));

    if(tc->PrimType == ACTC_PRIM_FAN) {
        tc->V2 = thirdVertex;
    } else /* PRIM_STRIP */ {
	if(tc->CurWindOrder == ACTC_FWD_ORDER)
	    tc->V1 = thirdVertex;
	else
	    tc->V2 = thirdVertex;
	tc->CurWindOrder = !tc->CurWindOrder;
    }

    tc->VerticesSoFar++;
    return ACTC_NO_ERROR;
}

int actcTrianglesToPrimitives(ACTCData *tc, int triangleCount,
    unsigned int (*triangles)[3], int primTypes[], int primLengths[], unsigned int vertices[],
    int maxBatchSize)
{
    int r;
    int curTriangle;
    int curPrimitive;
    unsigned int curVertex;
    int prim;
    unsigned int v1, v2, v3;
    int lastPrim;
    int passesWithoutPrims;
    int trisSoFar;

    if(tc->IsInputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcTrianglesToPrimitives : within BeginInput/"
	    "EndInput\n");)
	return tc->Error = ACTC_DURING_INPUT;
    }
    if(tc->IsOutputting) {
	ACTC_DEBUG_ASSERT(fprintf(stderr, "actcTrianglesToPrimitives : within"
	    "BeginOutput/EndOutput\n");)
	return tc->Error = ACTC_DURING_OUTPUT;
    }
    curTriangle = 0;
    curPrimitive = 0;
    curVertex = 0;
    passesWithoutPrims = 0;

    actcMakeEmpty(tc);

    ACTC_CHECK(actcBeginInput(tc));
    trisSoFar = 0;
    while(curTriangle < triangleCount) {
	r = actcAddTriangle(tc, triangles[curTriangle][0],
	    triangles[curTriangle][1], triangles[curTriangle][2]);
	trisSoFar++;
	curTriangle++;
	if((trisSoFar >= maxBatchSize) ||
	    (r == ACTC_ALLOC_FAILED && curTriangle != triangleCount) ||
	    (r == ACTC_NO_ERROR && curTriangle == triangleCount)) {

	    /* drain what we got */
	    trisSoFar = 0;
	    ACTC_CHECK(actcEndInput(tc));
	    ACTC_CHECK(actcBeginOutput(tc));
	    lastPrim = curPrimitive;
	    while((prim = actcStartNextPrim(tc, &v1, &v2)) != ACTC_DATABASE_EMPTY) {
		ACTC_CHECK(prim);
		primTypes[curPrimitive] = prim;
		primLengths[curPrimitive] = 2;
		vertices[curVertex++] = v1;
		vertices[curVertex++] = v2;
		while((r = actcGetNextVert(tc, &v3)) != ACTC_PRIM_COMPLETE) {
		    ACTC_CHECK(r);
		    vertices[curVertex++] = v3;
		    primLengths[curPrimitive]++;
		}
		curPrimitive++;
	    }
	    ACTC_CHECK(actcEndOutput(tc));
	    if(r == ACTC_ALLOC_FAILED && curPrimitive == lastPrim) {
	        if(passesWithoutPrims == 0) {
		    /* not enough memory to add a triangle and */
		    /* nothing in the database, better free everything */
		    /* and try again */
		    actcMakeEmpty(tc);
		} else {
		    /* cleaned up and STILL couldn't get a triangle in; */
		    /* give up */
		    return tc->Error = ACTC_ALLOC_FAILED;
		}
		passesWithoutPrims++;
	    }
	    ACTC_CHECK(actcBeginInput(tc));
	} else
	    ACTC_CHECK(r);
	if(r == ACTC_ALLOC_FAILED)
	    curTriangle--;
    }
    ACTC_CHECK(actcEndInput(tc));

    actcMakeEmpty(tc);

    return curPrimitive;
}

/* vi:tabstop=8
 */

 *actcNew(void);
int actcParami(ACTCData *tc, unsigned int param, unsigned int value);
int actcGetParami(ACTCData *tc, unsigned int param, unsigned int *value);
int actcParamu(ACTCData *tc, unsigned int param, unsigned int value);
int actcGetParamu(ACTCData *tc, unsigned int param, unsigned int *value);
int actcGetError(ACTCData *tc);
int actcMakeEmpty(ACTCData *tc);
void actcDelete(ACTCData *tc);
void actcDumpState(ACTCData *tc, FILE *fp);

int actcBeginInput(ACTCData *tc);
int actcGetIsDuringInput(ACTCData *tc);
int actcAddTriangle(ACTCData *tc, unsigned int v1, unsigned int v2, unsigned int v3);
int actcEndInput(ACTCData *tc);

int actcBeginOutput(ACTCData *tc);
int actcGetIsDuringOutput(ACTCData *tc);
int actcStartNextPrim(ACTCData *tc, unsigned int *v1Return, unsigned int *v2Return);
int actcGetNextVert(ACTCData *tc, unsigned int *vReturn);
int actcEndOutput(ACTCData *tc);

int actcGetMemoryAllocation(ACTCData *tc, size_t *bytesAllocated);

int actcTrianglesToPrimitives(ACTCData *tc, int triangleCount,
    unsigned int (*triangles)[3], unsigned int primTypes[], unsigned int primLengths[], unsigned int vertices[],
    unsigned int maxBatchSize);

#endif /* _ACTC_H_ */

/* vi:tabstop=8
 */
