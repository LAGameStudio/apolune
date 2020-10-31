/*
 * Public domain.
 * Operations on m*n matrices (SPARSE version).
 */
typedef struct m_matrix_sp {
	struct m_matrix _inherit;
	char *d; /* data to be used by SPARSE */
} M_MatrixSP;

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC const M_MatrixOps mMatOps_SP;
extern DECLSPEC M_Real *M_GetElement_SP(void *pM, Uint i, Uint j);
extern DECLSPEC M_Real M_GetValue_SP(void *pM, Uint i, Uint j);
extern DECLSPEC M_Real M_Get_SP(void *, Uint, Uint);
extern DECLSPEC int M_MatrixResize_SP(void *pA, Uint m, Uint n);
extern DECLSPEC void M_MatrixFree_SP(void *pA);
extern DECLSPEC void *M_MatrixNew_SP(Uint m, Uint n);
extern DECLSPEC void M_MatrixPrint_SP(void *pA);
extern DECLSPEC void M_MatrixSetZero_SP(void *pA);
extern DECLSPEC int M_FactorizeLU_SP(void *pA);
extern DECLSPEC void M_BacksubstLU_SP(void *pA, void *pV);
extern DECLSPEC void M_MNAPreorder_SP(void *pA);
extern DECLSPEC void M_AddToDiag_SP(void *pA, M_Real g);
extern DECLSPEC void *M_MatrixRead_SP(AG_DataSource *buf);
extern DECLSPEC void M_MatrixWrite_SP(AG_DataSource *buf, const void *pA);
extern DECLSPEC void M_MatrixToFloats_SP(float *fv, const void *pA);
extern DECLSPEC void M_MatrixToDoubles_SP(double *dv, const void *pA);
extern DECLSPEC void M_MatrixFromFloats_SP(void *pA, const float *fv);
extern DECLSPEC void M_MatrixFromDoubles_SP(void *pA, const double *fv);
__END_DECLS
/* Close generated block */
