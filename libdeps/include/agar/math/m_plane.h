/*	Public domain	*/

/* Begin generated block */
__BEGIN_DECLS
extern DECLSPEC M_Plane3 M_PlaneFromNormal3(M_Vector3, M_Real);
extern DECLSPEC M_Plane3 M_PlaneFromPts3(M_Vector3, M_Vector3, M_Vector3);
extern DECLSPEC M_Plane3 M_PlaneAtDistance3(M_Plane3, M_Real);
extern DECLSPEC M_Plane3 M_PlaneRead3(AG_DataSource *);
extern DECLSPEC void M_PlaneWrite3(AG_DataSource *, M_Plane3 *);
extern DECLSPEC int M_PlaneIsValid3(M_Plane3);
extern DECLSPEC M_Vector3 M_PlaneNorm3(M_Plane3);
extern DECLSPEC M_Vector3 M_PlaneNorm3p(const M_Plane3 *);
extern DECLSPEC M_Real M_PlaneVectorAngle3(M_Plane3, M_Vector3);
__END_DECLS
/* Close generated block */
