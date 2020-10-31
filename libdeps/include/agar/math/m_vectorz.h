/*	Public domain	*/

typedef struct m_vectorz {
	Uint n;
	int *v;
} M_VectorZ;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC M_VectorZ *M_VectorNewZ(Uint);
extern DECLSPEC void M_VectorSetZ(M_VectorZ *, int);
extern DECLSPEC void M_VectorCopyZ(const M_VectorZ *, M_VectorZ *);
extern DECLSPEC void M_VectorFreeZ(M_VectorZ *);
extern DECLSPEC void M_VectorAddZv(M_VectorZ *, const M_VectorZ *);
extern DECLSPEC void M_VectorSubZv(M_VectorZ *, const M_VectorZ *);
extern DECLSPEC void M_VectorScaleZv(M_VectorZ *, M_Real);
extern DECLSPEC void M_VectorResizeV(M_VectorZ *, Uint);
extern DECLSPEC void M_VectorPrintZ(const M_VectorZ *);
__END_DECLS
/* Close generated block */
