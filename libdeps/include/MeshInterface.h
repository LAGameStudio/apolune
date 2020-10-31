// ============================================================================
//			Copyright (c) 2014 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef _MESH_INTERFACE_H_
#define _MESH_INTERFACE_H_

class MeshInterface
{
public:
	
	//Constructor
	MeshInterface() {};
	
	//Destructor
	virtual ~MeshInterface(){}
	
	//This method gives extents of the mesh
	virtual int GetExtents(double &xMin, double &xMax, double &yMin, double &yMax, double &zMin,double &zMax)const = 0;

	//This method returns the number of facets in the mesh
	virtual size_t GetNumFacets()const = 0;

	//This method returns the number of edges in the mesh
	virtual size_t GetNumEdges()const = 0;
	
	//This method returns number of vertices in the mesh
	virtual size_t GetNumVertices()const = 0;

	//This method returns the three vertices of the facet
	virtual int GetFacetVertices(size_t facetIndex, double fVertices[9])const = 0;

	//This method gets the vector components of the normal of the facet
	virtual int GetFacetNormal(size_t facetIndex, double &i, double &j, double &k)const = 0;

	//This method gives the indices of three vertices of the facet
	virtual int GetVertexIndicesForFacet(size_t facetIndex, size_t vIndices[3])const = 0;

	//This method returns the two vertices of the edge
	virtual int GetEdgeVertices(size_t edgeIndex, double eVertices[6])const = 0;

	//This method gives the indices of two vertices of the edge
	virtual int GetVertexIndicesForEdge(size_t edgeIndex, size_t vIndices[2])const = 0;

	//This method returns the coordinates of vertex
	virtual int GetVertexCoordinates(size_t vertexIndex, double &x, double &y, double &z)const = 0;
};

#endif
