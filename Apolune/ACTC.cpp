#include "ACTC.h"
/*
 * $Header: /home/grantham/cvsroot/projects/modules/tc/tc.c,v 1.27 2000/10/10 16:09:23 grantham Exp $
 */

#include "ACTC.h"

size_t tableGetIteratorSize(void)
{
    return sizeof(ACTCTableIterator);
}

/* private tokens */
#define ACTC_NO_MATCHING_VERT		-0x3000
#define ACTC_FWD_ORDER			0
#define ACTC_REV_ORDER			1

#define MAX_STATIC_VERTS		10000000 /* buh? */

#if defined(DEBUG) || defined(INFO)

static void dumpTriangles(ACTCEdge *e, FILE *fp)
{
    int i;
    int c;
    char v[12];

    c = fprintf(fp, "      %d triangles: ", e->TriangleCount);
    for(i = 0; i < e->TriangleCount; i++) {
	if(c + 1 + sprintf(v, "%u", e->Triangles[i].FinalVert) > 78) {
	    fputs("\n", fp);
	    c = fprintf(fp, "        ");
	}
	c += fprintf(fp, " %s", v);
    }
    fputs("\n", fp);
}

static void dumpEdges(ACTCVertex *vert, FILE *fp)
{
    int i;
    int c;
    char v[26]; /* two signed ints plus x plus NUL */

    for(i = 0; i < vert->EdgeCount; i++) {
	fprintf(fp, "    %u->%u (%d times)\n", vert->V, vert->Edges[i].V2->V,
	    vert->Edges[i].Count);
	dumpTriangles(&vert->Edges[i], fp);
    }
    fputs("\n", fp);
}

static void dumpVertices(ACTCData *tc, FILE *fp)
{
    int i;
    ACTCVertex *v;

    if(!tc->UsingStaticVerts)
        tableResetIterator(tc->VertexIterator);

    fprintf(fp, "%d vertices in valences list\n", tc->VertexCount);
    if(tc->UsingStaticVerts) {
        for(i = 0; i < tc->VertRange; i++) {
	    v = &tc->StaticVerts[i];
	    if(v->Count > 0) {
		fprintf(fp, "  vertex %u, valence %d, %d edges\n", v->V,
		    v->Count, v->EdgeCount);
		dumpEdges(v, fp);
	    }
	}
    } else {
	for(i = 0; i < tc->VertexCount; i++) {
	    if(tableIterate(tc->Vertices, tc->VertexIterator, NULL,
		(void **)&v) == 0) {
		fprintf(fp, "ACTC::dumpVertices : fewer vertices in the table "
		    "than we expected!\n");
		fprintf(stderr, "ACTC::dumpVertices : fewer vertices in the table "
		    "than we expected!\n");
	    }
	    if(v == NULL) {
		fprintf(fp, "ACTC::dumpVertices : did not expect to get a NULL"
		    "Vertex from the table iterator!\n");
		fprintf(stderr, "ACTC::dumpVertices : did not expect to get a NULL"
		    "Vertex from the table iterator!\n");
	    }
	    fprintf(fp, "  vertex %u, valence %d, %d edges\n", v->V, v->Count,
		v->EdgeCount);
	    dumpEdges(v, fp);
	}
    }
}

static void dumpVertexBins(ACTCData *tc, FILE *fp)
{
    ACTCVertex *cur;
    int i;
    int c;
    char v[26]; /* two signed ints plus x plus NUL */

    fprintf(fp, "vertex bins:\n");
    if(tc->VertexBins == NULL) {
        fprintf(fp, "        empty.\n");
	return;
    }
    for(i = 1; i <= tc->CurMaxVertValence; i++) {
        cur = tc->VertexBins[i];
	c = fprintf(fp, "        bin %d -> ", i);
	while(cur != NULL) {
	    if(c + 1 + sprintf(v, "%ux%d", cur->V, cur->Count) > 78) {
		fputs("\n", fp);
		c = fprintf(fp, "          ");
	    }
	    c += fprintf(fp, " %s", v);
	    cur = cur->Next;
	}
	fputs("\n", fp);
    }
}

void actcDumpState(ACTCData *tc, FILE *fp)
{
    dumpVertices(tc, fp);
    dumpVertexBins(tc, fp);
}

#endif /* DEBUG || INFO */

#if defined(DEBUG)

static int abortWithOptionalDump(ACTCData *tc)
{
    ACTC_INFO_ASSERT(actcDumpState(tc, stderr));
    abort();
}

#endif /* defined(DEBUG) */


static void *reallocAndAppend(void **ptr, unsigned int *itemCount, size_t itemBytes,
    void *append)
{
    void *t;

    t = realloc(*ptr, itemBytes * (*itemCount + 1));
    if(t == NULL) return NULL;
    *ptr = t;

    memcpy((unsigned char *)*ptr + *itemCount * itemBytes, append, itemBytes);
    (*itemCount) += 1;

    return *ptr;
}
