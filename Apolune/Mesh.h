/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once
#if defined(NEVER)
#include "LinkedList.h"
#include "ListItem.h"
#include "Material.h"

#pragma pack(push,4)
typedef struct Vec2 { float x, y; } Vec2;
typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct Vec4 { float x, y, z, w; } Vec4;
// Some structs to make accessing vertex and tangent arrays less painful. These need to be tightly
// packed or else it won't match the OpenGL vertex array format and bad stuff will happen
typedef struct VertexV   { Vec3 v; } VertexV;
typedef struct VertexNV  { Vec3 vn; Vec3 v; } VertexNV;
typedef struct VertexTV  { Vec2 vt; Vec3 v; } VertexTV;
typedef struct VertexTNV { Vec2 vt; Vec3 vn; Vec3 v; } VertexTNV;
typedef struct TangentT  { Vec3 t; } TangentT;
typedef struct TangentTB { Vec3 t; Vec3 b; } TangentTB;
#pragma pack(pop)

// Load flags (zLoadMesh)
#define MESH_LOAD_NORMALIZE  1  // Normalize normal vectors
#define MESH_LOAD_THOROUGH   2  // Load thoroughly. This may take much longer, but give better
                                // results for things like vertex sharing. Meant to be used when
                                // converting from one format to another.
#define MESH_LOAD_NOINDEX    4  // A hint to the loader to not use indexed vertex arrays, may be
                                // ignored. To be sure wether or not the loaded mesh uses an indexed
                                // vertex array, check for Z_MESH_VA_INDEXED in mesh.flags.
#define MESH_LOAD_TANGENTS   8  // Calculate tangent/bitangent vectors for mesh if it has normals
                                // and didn't supply them itself.


// Data format flags - i.e. vertex array format. (Mesh.flags)
#define MESH_HAS_NORMALS     1
#define MESH_HAS_TEXCOORDS   2
#define MESH_VA_INDEXED      4
#define MESH_HAS_TANGENTS    8 // For now tangents/bitangents are stored in a seperate (but inter-
#define MESH_HAS_BITANGENTS 16 // leaved if mesh has both) array from the vertices/normals/
                               // texcoords so that I can use a standard interleaved format for
                               // those.

// Draw flags (zDrawMesh)
#define MESH_DRAW_FILL      1
#define MESH_DRAW_NORMALS   2
#define MESH_DRAW_LINES     4
#define MESH_DRAW_POINTS    8
#define MESH_DRAW_TANGENTS 16
#define MESH_DRAW_DEFAULT (MESH_DRAW_FILL)

// Buffer grow flags (zGrowMeshBuffers)
#define MESH_GROW_VERTICES 1
#define MESH_GROW_INDICES  2


#define MESH_MAXGROUPS 128 // Maximum number of groups a mesh can have, can probably be lowered.

#define MESH_VERTICES_BUFINC 2000 // By how much buffers are resized during loading.
#define MESH_INDICES_BUFINC  2000



// ZMeshGroup - A grouping of vertices or indices (depending on wether Z_MESH_VA_INDEXED is set or
// not) associated with a material.
typedef struct MeshGroup
{
    unsigned int start; // First index/vertex.
    unsigned int count; // Number of indices/vertices to dereference/draw for this group.
    Material *material;
} MeshGroup;

class Mesh : public ListItem {
public:
 string name;
 unsigned int flags;         // Data format flags.
 unsigned int num_groups;    // Number of vertex groups.
 unsigned int num_vertices;  // Number of vertices in the vertex buffer and tangents in the
                             // tangent buffer.
 unsigned int num_indices;   // Number of indices in the index buffer.
 unsigned int vertices_size; // Element size of vertex buffer.
 unsigned int indices_size;  // Element size of index buffer.
 unsigned int elem_size;     // Size of each vertex in number of floats.

 bool format_picked;

 int is_resident;            // Wether or not the vertex data has been uploaded to OpenGL.
 GLuint vertex_vbo_name;     // Names of OpenGL VBOs. 0 means not loaded
 GLuint tangent_vbo_name;
 GLuint index_vbo_name;
 
 float *vertices;            // Vertex array.
 float *tangents;            // Tangent/bitangent array.
 unsigned int *indices;      // Index array.
 
 MeshGroup groups[MESH_MAXGROUPS];

 void zMeshInit(void);

 void zMeshDeinit(void);

 void zBuildTangentArray(ZMesh *mesh, int bitangent);
 void LoadWavefrontObj(const char *file, unsigned int f);
 bool checkFormat(unsigned int f);
 bool findMatchingVertex(float *new_vertex, unsigned int *index);
 bool growGroupBuffers(int type);
 bool addVertex(Vec3 *v, Vec3 *vt, Vec3 *vn);
 bool parseFace();
 void parseScale();

 void zIterMeshes(void (*iter)(ZMesh *, void *), void *data);

 void zDrawMesh(ZMesh *mdl, unsigned int draw_flags);

 int zGrowMeshBuffers(ZMesh *mesh, int type);

 void zMeshInfo(ZMesh *mesh);

 void zMakeMeshNonResident(ZMesh *mesh, void *ignored); // Last param makes this directly callable by
                                                       // zIterMeshes.
 void zDeleteMesh(ZMesh *mesh);

 Mesh(void);
 ~Mesh(void);
};

class Meshes : public LinkedList {
public:
 Mesh *find(char *name) {
 }
};

#endif