/*	Public domain	*/

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC M_Line2 M_LineRead2(AG_DataSource *);
extern DECLSPEC M_Line3 M_LineRead3(AG_DataSource *);
extern DECLSPEC void M_LineWrite2(AG_DataSource *, M_Line2 *);
extern DECLSPEC void M_LineWrite3(AG_DataSource *, M_Line3 *);
extern DECLSPEC M_Line2 M_LineFromPtDir2(M_Vector2, M_Vector2, M_Real);
extern DECLSPEC M_Line3 M_LineFromPtDir3(M_Vector3, M_Vector3, M_Real);
extern DECLSPEC M_Line2 M_LineFromPts2(M_Vector2, M_Vector2);
extern DECLSPEC M_Line3 M_LineFromPts3(M_Vector3, M_Vector3);
extern DECLSPEC M_Line2 M_LineProject2(M_Line3);
extern DECLSPEC M_Line3 M_LineProject3(M_Line2);
extern DECLSPEC M_Line2 M_LineParallel2(M_Line2, M_Real);
extern DECLSPEC M_Line3 M_LineParallel3(M_Line3, M_Real);
extern DECLSPEC M_Real M_LinePointDistance2(M_Line2, M_Vector2);
extern DECLSPEC M_Real M_LinePointDistance3(M_Line3, M_Vector3);
extern DECLSPEC M_Real M_LineLineAngle2(M_Line2, M_Line2);
extern DECLSPEC M_Real M_LineLineAngle3(M_Line3, M_Line3);
extern DECLSPEC M_GeomSet2 M_IntersectLineLine2(M_Line2, M_Line2);

static __inline__ void
M_LineToPts2(M_Line2 L, M_Vector2 *p1, M_Vector2 *p2)
{
	*p1 = L.p;
	*p2 = M_VecAdd2(L.p, M_VecScale2p(&L.d, L.t));
}

static __inline__ void
M_LineToPts3(M_Line3 L, M_Vector3 *p1, M_Vector3 *p2)
{
	*p1 = L.p;
	*p2 = M_VecAdd3(L.p, M_VecScale3p(&L.d, L.t));
}

static __inline__ int
M_LineIsRay2(M_Line2 L)
{
	return (L.t == M_INFINITY);
}

static __inline__ int
M_LineIsRay3(M_Line3 L)
{
	return (L.t == M_INFINITY);
}

static __inline__ M_Vector2
M_LineFirstPt2(M_Line2 L)
{
	return (L.p);
}

static __inline__ M_Vector2
M_LineSecondPt2(M_Line2 L)
{
	return M_VecAdd2(L.p, M_VecScale2p(&L.d, L.t));
}

static __inline__ M_Real
M_LinePointSide2(M_Line2 L, M_Vector2 p)
{
	M_Vector2 p1, p2;
	
	M_LineToPts2(L, &p1, &p2);
	return (p2.x - p1.x)*(p.y - p1.y) - (p.x - p1.x)*(p2.y - p1.y);
}
__END_DECLS
/* Close generated block */
