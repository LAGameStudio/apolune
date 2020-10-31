#if defined(NEVER)
#include "Mesh.h"
#include "Vertex.h"
#include "Material.h"

// Wavefront .OBJ model files

#define _BSD_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

extern Materials materials;

/* This code can read Wavefront .OBJ model files (and referenced material libraries) with a bunch of
 * limitations and modifications:
 *
 *  - Only supports the basic v/vn/vt/f geometry keywords.
 *
 *  - Only supports 2 dimensional texture coordinates, if given, the w component will be ignored.
 *
 *  - All face vertex index references in the entire file have to be consistent in wether or not
 *    they reference texcoord and/or normals. The specification doesn't make it clear wether this
 *    restriction only goes for the vertex index references within a single face, or for the entire
 *    file, this code assumes the latter.
 *
 *  - Material library support is limited to the Kd, Ka, Ks, Ns, d (without -halo), map_Kd, map_Ks,
 *    and bump (used as normal map) keywords. For the K* keywords only RGB parameters are supported,
 *    for the map_* and bump directives only the filename parameter is supported.
 *
 *  - Non-standard keywords supported are:
 *    
 *    - normalize: normalize normal vectors.
 *    - scale <scalar>: to scale vertex coordinates.
 *    
 *    For material libraries:
 *    - tex_prefix <path>: prefix to for references to textues in material libraries.
 *    - vertex_shader <path>: path (relative to data dir) to vertex program.
 *    - fragment_shader <path>: path (relative to data dir) to fragment program.
 */


#define OBJ_LINE_BUFFER_SIZE 2000
#define OBJ_TOKEN_SIZE 100

// Increase filled-up buffers by these values * sizeof(vec3). On windows, anything above 1500
// doesn't give much improvement anymore. On Linux, any value works. Maybe realloc-ing isn't a good
// idea on Windows?
#define OBJ_VEC3_BUFFER_INC   5000

#define OBJ_VERTEX_BUFFER_INC  1000
#define OBJ_INDEX_BUFFER_INC   1000

// How far to search back when looking for shared vertices.
#define OBJ_VERTEX_SEARCH_WINDOW 512

#define OBJ_GROW_VERTICES 1
#define OBJ_GROW_INDICES  2

Material default_material;

const char *filename; // For printing diagnostic messages.

// Special processing options
unsigned int load_flags; // Load flags
float scale;

char tex_prefix[OBJ_TOKEN_SIZE];

int format_picked; // Wether or not a vertex format has been picked
int warned_inconsistency; // Used to only warn about inconsistent vertex format once.
unsigned int ignored_faces;

// Indicates the number of vertices each face is defined with. 0 means it hasn't been established
// yet how many vertices to process, 3 for triangles 4 for quads, anything else is invalid.
static unsigned int face_vertex_count;

// Pointers to memory where vertex data is temporarily stored as the file is parsed.
static Vec3 *vertices;
static Vec3 *normals;
static Vec3 *texcoords;

// Using seperate groups for each material enountered. Once done parsing I transform them into a
// single vertex/index array and fill mesh->groups. This way I can reuse existing groups more easily
// when lots of groups using the same materials are listed in a random order in the model file.
static struct {
    float *vertices;
    unsigned int *indices;
    unsigned int num_vertices;
    unsigned int num_indices;
    size_t indices_size;
    size_t vertices_size;
    Material *material;
} groups[MESH_MAXGROUPS];

static unsigned int num_groups;
static int cur_group;

// These hold the number of vec3's the currently allocated memory can hold.
static unsigned int vertices_size;
static unsigned int normals_size;
static unsigned int texcoords_size;

static unsigned int vertex_count;
static unsigned int normal_count;
static unsigned int texcoord_count;


// These keep track of where we are in the .obj file.
static unsigned int line_count;
static unsigned int triangle_count;


// Statically allocated buffers to hold lines/tokens being parsed.
static char line[OBJ_LINE_BUFFER_SIZE];
static char token[OBJ_TOKEN_SIZE];

// Current line position for token parsing.
static char *line_pos;



// Read a token from line into token. Returns number of chars read. Increases line_pos to point to
// the first character past the token.
static int parse_token_obj(void)
{
    char *p = line_pos, *o = token;
    int count = 0;

    // Skip whitespace.
    while (*p == ' ' || *p == '\t') p++;

    // Read chars to token and stop if we hit whitespace, newline, \0, or if we don't have enough
    // room in token to fit a char + \0.
    while ( *p != ' '  && *p != '\t' && *p != '\0' && *p != '\n' && *p != '\r') {

        // See if we have room left in token.
        if ( (OBJ_TOKEN_SIZE-count) > 1 ) {
            *o++ = *p++;
            count++;
        } else {
            OUTPUT("Unable to read entire token on line %u while parsing \"%s\", token buffer too small.\n", line_count, filename);
        }
    }

    *o = '\0';
    line_pos = p; // Save line position.
    return count;
}



// Parses a vec3 from a line after token has been read (and line_pos set to point to whatever
// follows the token) and stores it in the appropriate buffer as indicated by type.
#define OBJ_DATATYPE_VERTEX 0
#define OBJ_DATATYPE_NORMAL 1
#define OBJ_DATATYPE_TEXCOORD 2
static int parse_vec3(int type)
{
    Vec3 v = { 0.0f, 0.0f, 0.0f };
    Vec3 **buffer;
    unsigned int *buffer_size;  // buffer_size*sizeof(vec3) = size in bytes.
    unsigned int *buffer_count; // Number of vec3s in the buffer.

    // Wire up the buffer to hold the vec3 we're parsing.
    if (type == OBJ_DATATYPE_VERTEX) {
        buffer = &vertices;
        buffer_size = &vertices_size;
        buffer_count = &vertex_count;
    } else if (type == OBJ_DATATYPE_NORMAL) {
        buffer = &normals;
        buffer_size = &normals_size;
        buffer_count = &normal_count;
    } else if (type == OBJ_DATATYPE_TEXCOORD) {
        buffer = &texcoords;
        buffer_size = &texcoords_size;
        buffer_count = &texcoord_count;
    } else {
        assert(0 && "No valid vec3 type was given");
    }

    // Increase size of buffer if neccesary.
    if (*buffer_count >= *buffer_size) {

        if ( ( *buffer = (Vec3 *) 
                    realloc(*buffer, (*buffer_size+OBJ_VEC3_BUFFER_INC) * sizeof(Vec3)) ) == NULL) {

            OUTPUT("%s: Failed to (re)allocate more memory for datatype data while parsing \"%s\".\n",
                __func__, filename);
            exit(EXIT_FAILURE);
        }

        *buffer_size += OBJ_VEC3_BUFFER_INC;
    }

    // Parse vertex coordinates, if one or more component is not parsed due to a malformed string,
    // they will be left at their initial 0.0 values and added to the buffer anyway. This way there
    // will still be enough vertices/normals/texcoords once the faces' indices are dereferenced.
    sscanf(line_pos, "%f %f %f", &(v.x), &(v.y), &(v.z));

    // Scale vertices if a scale factor was set,
    if (type == OBJ_DATATYPE_VERTEX && scale != 0.0f) {
        v.x *= scale;
        v.y *= scale;
        v.z *= scale;
    } else if (type == OBJ_DATATYPE_NORMAL && (load_flags & MESH_LOAD_NORMALIZE)) {
        zNormalize3(&v);
    }

    (*buffer)[(*buffer_count)++] = v;

    return 1;
}



// Check if the given vertex format matches the aleady-established one. If it matches, returns 1,
// else 0. If no format has been established yet, it establishes the new format using the given
// values and returns 1.
//static int check_format(int has_t, int has_n) {
bool Mesh::checkFormat(unsigned int f) {

    if (!format_picked) {

        // Set new format.
        if ((flags & MESH_HAS_NORMALS)
         && (flags & MESH_HAS_TEXCOORDS))      elem_size = 8;
        else if ( flags & MESH_HAS_NORMALS )   elem_size = 6;
        else if ( flags & MESH_HAS_TEXCOORDS ) elem_size = 5;
        else                                   elem_size = 3;

        flags |= f;
        format_picked = true;
        return true;

    } else {

        // Check of given format matches established one.
        if ( (flags & (MESH_HAS_NORMALS | MESH_HAS_TEXCOORDS )) == f )
            return true;
    }

    return false;
}



// Dereference the indices to vertex, texcoord and normal data passed in by v, vt and vn and store
// the value in *v3v, *v3vt, and *v3vn respectively. v is required, vt and vn may be set to 0 so
// they will be ignored.
static inline int dereference_vertex(int v, int vt, int vn, Vec3 *v3v, Vec3 *v3vt, Vec3 *v3vn)
{
    // Dereference vertex.
    if (v > 0 && v <= (int) vertex_count)
        *v3v = vertices[v-1]; // Positive index.
    else if (v < 0 && -v <= (int) vertex_count)
        *v3v = vertices[vertex_count+v]; // Negative index.
    else
        return 0; // 0 or out-of-range index.

    // Dereference texcoord but happily ignore if 0.
    if (vt) {

        if (vt > 0 && vt <= (int) texcoord_count)
            *v3vt = texcoords[vt-1];
        else if (vt < 0 && -vt <= (int) texcoord_count)
            *v3vt = texcoords[texcoord_count+vt];
        else
            return 0;
    }

    // Same for the normal.
    if (vn) {

        if (vn > 0 && vn <= (int) normal_count)
            *v3vn = normals[vn-1];
        else if (vn < 0 && -vn <= (int) normal_count)
            *v3vn = normals[normal_count+vn];
        else
            return 0;
    }

    return 1;
}



// Try to find a matching vertex so it can be reused. Returns 1 and writes index if match is found,
// or else 0 is returned. I'm searching backward as that should be more likely to find a match early
// on..
bool Mesh::findMatchingVertex(float *new_vertex, unsigned int *index) {
    unsigned int start = 0, end;
    float *curvert;
    size_t size = elem_size * sizeof(float);

    if (groups[cur_group].count == 0) return false;

    if ( !(load_flags & MESH_LOAD_THOROUGH) && groups[cur_group].count > OBJ_VERTEX_SEARCH_WINDOW )
        start = groups[cur_group].count - OBJ_VERTEX_SEARCH_WINDOW;

    end = groups[cur_group].count-1;
    curvert = (float *) (groups[cur_group].start+(elem_size*end));

    // Loop through some number of vertices and try to find a match
    do {
       if ( memcmp(curvert, new_vertex, size) == 0 ) { *index = end; return true; }
       curvert -= elem_size;
    } while (end-- > start);

    return false;
}



// Grow buffers if needed for current group.
bool Mesh::growGroupBuffers(int type) {
    assert(format_picked);

    if (type == OBJ_GROW_VERTICES) {

        assert(groups[cur_group].count <= groups[cur_group].vertices_size);

        if (groups[cur_group].count == groups[cur_group].vertices_size) {

            float *tmp = (float *) realloc(groups[cur_group].vertices,
                (groups[cur_group].vertices_size + 
                OBJ_VERTEX_BUFFER_INC) * elem_size * sizeof(float) );

            if (!tmp) {
                OUTPUT("Failed to allocate memory for mesh vertex buffer.\n");
                return 0;
            }

            groups[cur_group].vertices = tmp;
            groups[cur_group].vertices_size += OBJ_VERTEX_BUFFER_INC;
        }

    } else if (type == OBJ_GROW_INDICES) {

        assert(groups[cur_group].num_indices <= groups[cur_group].indices_size);

        if (groups[cur_group].num_indices == groups[cur_group].indices_size) {

            unsigned int *tmp = (unsigned int *) realloc(groups[cur_group].indices,
                (groups[cur_group].indices_size + OBJ_INDEX_BUFFER_INC) * sizeof(unsigned int) );

            if (!tmp) {
                OUTPUT("Failed to allocate memory for mesh index buffer.\n");
                return 0;
            }

            groups[cur_group].indices = tmp;
            groups[cur_group].indices_size += OBJ_INDEX_BUFFER_INC;
        }

    } else {
        assert(0 && "Invalid buffer type given.");
    }

    return 1;
}



// Add given vertex to mesh. Returns 1 if an error occured (makes it easier to check for errors from
// multiple calls in one go.
bool Mesh::addVertex(Vec3 *v, Vec3 *vt, Vec3 *vn)
{
    float new_vertex[8];
    unsigned int match_index;

    if ( (flags & MESH_HAS_NORMALS) && (flags & MESH_HAS_TEXCOORDS) ) {
        new_vertex[0] = vt->x;  new_vertex[1] = vt->y;
        new_vertex[2] = vn->x;  new_vertex[3] = vn->y;  new_vertex[4] = vn->z;
        new_vertex[5] =  v->x;  new_vertex[6] =  v->y;  new_vertex[7] =  v->z;
    } else if ( flags & MESH_HAS_NORMALS ) {
        new_vertex[0] = vn->x;  new_vertex[1] = vn->y;  new_vertex[2] = vn->z;
        new_vertex[3] =  v->x;  new_vertex[4] =  v->y;  new_vertex[5] =  v->z;
    } else if ( flags & MESH_HAS_TEXCOORDS ) {
        new_vertex[0] = vt->x;  new_vertex[1] = vt->y;
        new_vertex[2] =  v->x;  new_vertex[3] =  v->y;  new_vertex[4] =  v->z;
    } else {
        new_vertex[0] =  v->x;  new_vertex[1] =  v->y;  new_vertex[2] =  v->z;
    }


    // Check for matching vertices (unless NOINDEX load flag is set), and if found, reuse index,
    // else add new.
    if ( !(load_flags & MESH_LOAD_NOINDEX) && (findMatchingVertex(new_vertex, &match_index)) ) {

        // Add just the matched index.
        if ( !growGroupBuffers(OBJ_GROW_INDICES) ) {
            OUTPUT("%s: Failed to grow group index buffer.\n", __func__);
            return true;
        }

        groups[cur_group].indices[groups[cur_group].num_indices] = match_index;
        groups[cur_group].num_indices++;

    } else {

        // Add new vertex / index, skip adding index if MESH_LOAD_NOINDEX was set in load_flags.
        if ( !(load_flags & MESH_LOAD_NOINDEX) ) {
            if ( !growGroupBuffers(OBJ_GROW_INDICES) ) {
                OUTPUT("%s: Failed to grow group index buffer.\n", __func__);
                return true;
            }

            groups[cur_group].indices[groups[cur_group].num_indices] = groups[cur_group].num_vertices;
            groups[cur_group].num_indices++;
        }


        // Add vertex.
        if ( !growGroupBuffers(OBJ_GROW_VERTICES) ) {
            // TODO: Try to clean up added index.
            OUTPUT("%s: Failed to grow mesh vertex buffer.\n", __func__);
            return true;
        }

        memcpy(groups[cur_group].vertices+(groups[cur_group].num_vertices*elem_size),
            new_vertex, mesh->elem_size*sizeof(float));
        groups[cur_group].num_vertices++;
    }

    return false;
}



// Parses a face and dereferences the vertex, normal, and texcoord indices to store them in the
// mesh's vertex buffer.
bool Mesh::parseFace(void)
{
    unsigned int face_vertex_count = 0, offset = 0, parsed_format = 0;
    int vertex_i = 0, texcoord_i = 0, normal_i = 0;

    // Temporary storage for up to 3 vertices. Once I have three vertices, I will be able to form a
    // triangle, and then another one for each additional vertex, by taking the first and then
    // previous vertex, in effect triangulating the polygon.
    Vec3 face_vertices[3];
    Vec3 face_texcoords[3];
    Vec3 face_normals[3];

    // Parse an unlimited amount of vertices, but once we have more than two, start saving triangles
    // to the mesh.
    while ( parse_token_obj() ) {

        face_vertex_count++;

        // If we've parsed 2 vertices, make sure we write into face_*[2].
        if (face_vertex_count > 2)
            offset = 2;
        else
            offset = face_vertex_count-1;

        // Try all the different formats (and also check if these are consistent with the already-
        // established format.
        if (sscanf(token, "%d/%d/%d", &vertex_i, &texcoord_i, &normal_i) == 3)  parsed_format = MESH_HAS_TEXCOORDS | MESH_HAS_NORMALS;
        else if (sscanf(token, "%d//%d", &vertex_i, &normal_i) == 2)            parsed_format = MESH_HAS_NORMALS;
        else if (sscanf(token, "%d/%d", &vertex_i, &texcoord_i) == 2)           parsed_format = MESH_HAS_TEXCOORDS;
        else if (sscanf(token, "%d", &vertex_i) == 1)                           parsed_format = 0;
        else {
            OUTPUT("Invalid vertex format on line %d in \"%s\".\n", line_count, filename);
            return false;
        }

        // Indices have been parsed, make sure format matches and dereference the indices.
        if (checkFormat(parsed_format)) {

            // Because the vertex format is always consistent, dereference_vertex can safely be used
            // like this because if normal_i/texcoord_i are left at 0 they won't be dereferenced.
            if ( !dereference_vertex(vertex_i, texcoord_i, normal_i,
                    face_vertices+offset, face_texcoords+offset, face_normals+offset ) ) {
                OUTPUT("Failed to dereference indices on line %u in \"%s\".\n", line_count, filename);
                return false;
            }
        } else {
            if (!warned_inconsistency) {
                OUTPUT("Inconsistent vertex format on line %u in \"%s\" (this warning is only"
                    " printed once).\n", line_count, filename);
                warned_inconsistency = 1;
            }
            ignored_faces++;
            return false;
        }

        // If I have now parsed more than 2 vertices, I can write a triangle to the mesh.
        if (face_vertex_count > 2) {

            int failed = 0;
            failed += addVertex(&(face_vertices[0]), &(face_texcoords[0]), &(face_normals[0]));
            failed += addVertex(&(face_vertices[1]), &(face_texcoords[1]), &(face_normals[1]));
            failed += addVertex(&(face_vertices[2]), &(face_texcoords[2]), &(face_normals[2]));
            
            if (failed) {
                // Failed to add one or more vertices, just abort this face.
                // TODO: Should rollback the failed vertices..
                OUTPUT("Failed to add one more vertices while processing face on line %u in \"%s\".\n", line_count, filename);
                return false;
            }

            // Copy the last vertex to face_*[1] so that it will be used to form any additional
            // triangle if we are triangulating.
            face_texcoords[1] = face_texcoords[2];
              face_normals[1] =   face_normals[2];
             face_vertices[1] =  face_vertices[2];

            triangle_count++;
        }
    }

    // If, at this point, I haven't actually written any triangles, this face was malformed and we
    // should reset the format so I don't base the format on a malformed face statement.
    if (triangle_count == 0) {

        format_picked = 0;
        OUTPUT("Not enough vertices to form triangle on line %u in \"%s\".\n", line_count, filename);
        return false;
    }

    return true;
}



// Parses a (uniform) scale factor for the vertex coords.
void Mesh::parseScale(void) {
 float s;
 parse_token_obj();
 if (sscanf(token, "%f", &s) == 1) scale = s;
 else OUTPUT("Unable to parse scale factor on line %d in \"%s\". Ignoring.\n", line_count, filename);
}



// Parse a color ( 3 floats) from line_pos. If 3 floats were succesfully parsed, the RGB values are
// written to result[], else nothing is done. Result must be a pointer to an array of 3 floats.
static void parse_mtl_color(float *result)
{
    float color[3];
    int res;

    // If only one value is supplied I should set R, G and B to this value.
    res = sscanf(line_pos, "%f %f %f", color, color+1, color+2);

    if (res == 1) {
        result[0] = color[0];
        result[1] = color[0];
        result[2] = color[0];
    } else if (res == 3) {
        result[0] = color[0];
        result[1] = color[1];
        result[2] = color[2];
    } else {
        return;
    }
}



// Parse shininess value for material. If succesful, the parsed value is written to *result, else it
// remains untouched.
static void parse_mtl_shininess(float *result)
{
    float Ns;

    if ( !sscanf(line_pos, "%f", &Ns) )
        return;

    // TODO: Make sure this conversion is correct, since the MTL spec says values up to 1000 are
    // normal but OpenGL errors on >128.

    // For now I clamp to 0-128..
    if (Ns < 0.0f) Ns = 0.0f;
    if (Ns > 128.0f) Ns = 128.0f;

    *result = Ns;
}



// Write texname prefixed with tex_prefix to dest. dest must be a char array of size
// RESOURCE_NAME_SIZE.
static void set_texname(char *dest, char *texname)
{
    int len;

    dest[0] = '\0';

    // Make sure the prefix and token lengths are < RESOURCE_NAME_SIZE
    len = strlen(tex_prefix);
    len += strlen(texname);

    if (len >= RESOURCE_NAME_SIZE) {
        OUTPUT("Texture name exceeded RESOURCE_NAME_SIZE, ignoring.\n");
        return;
    }

    strcat(dest, tex_prefix);
    strcat(dest, texname);
}



// Parse a material library.
static void parse_mtllib(void)
{
    const char *mtlpath;
    FILE *fd;
    unsigned int line_count = 0;
    Material *mat = NULL; // Pointer to most recently added material. Will be NULL initially or if
                           // there was an error parsing a newmtl directive.

    // Get filename.
    if ( !parse_token_obj() ) {
        OUTPUT("Unable to parse material file name on line %d in \"%s\". Ignoring.\n", line_count,
            filename);
        return;
    }

    // Get full path for material lib, relative to the .obj file currently being parsed.
    if ( !(mtlpath = zGetSiblingPath(filename, token)) ) {
        OUTPUT("Unable to open material library \"%s\" while parsing \"%s\".\n", token,
            filename);
        return;
    }

    // Open file, start parsing lines.
    if ( (fd = fopen(mtlpath, "rb")) == NULL ) {
        OUTPUT("Failed to open material library \"%s\".\n", mtlpath);
        return;
    }

    // If this becomes false, I need to check size of token when handling *_shader tokens..
    assert(RESOURCE_NAME_SIZE > OBJ_TOKEN_SIZE);

    while (fgets(line, OBJ_LINE_BUFFER_SIZE, fd)) {

        line_pos = line;
        line_count++;

        if (parse_token_obj()) {

            if (strcmp("newmtl", token) == 0) {
                if (parse_token_obj()) {
                    mat = add_new_material(token);
                } else {
                    OUTPUT("Failed to parse material name while parsing \"%s\" on line %u.\n",
                        mtlpath, line_count);
                    mat = NULL;
                }
            } else if (strcmp("tex_prefix", token) == 0) {
                // This is safe because sizeof(tex_prefix) == sizeof(token).
                if ( parse_token_obj() ) {
                    tex_prefix[0] = '\0';
                    strcat(tex_prefix, token);
                }
            } else if (!mat)
                // No material is active at this point, so no point in parsing any material
                // attributes..
                continue;
            else if (strcmp("Ns", token) == 0) parse_mtl_shininess(&(mat->shininess));
            else if (strcmp("Ka", token) == 0) parse_mtl_color(mat->ambient_color);
            else if (strcmp("Kd", token) == 0) parse_mtl_color(mat->diffuse_color);
            else if (strcmp("Ks", token) == 0) parse_mtl_color(mat->specular_color);

            // Parsing the filename this way is not quite right since there may be options between
            // the token and filename.. Unfortunately I can't simply read the last token on the line
            // either as it might be part of a comment. For now I'm just going to leave it like this.
            else if (strcmp("map_Kd", token) == 0) {
                if ( parse_token_obj() ) {
                    set_texname(mat->diffuse_map_name, token);
                }
            } else if (strcmp("map_Ks", token) == 0) {
                if ( parse_token_obj() ) {
                    set_texname(mat->specular_map_name, token);
                }
            } else if (strcmp("bump", token) == 0) {
                if ( parse_token_obj() ) {
                    set_texname(mat->normal_map_name, token);
                }
            } else if (strcmp("vertex_shader", token) == 0) {
                if ( parse_token_obj() ) {
                    mat->vertex_shader[0] = '\0';
                    strcat(mat->vertex_shader, token);
                }
            } else if (strcmp("fragment_shader", token) == 0) {
                if ( parse_token() ) {
                    mat->fragment_shader[0] = '\0';
                    strcat(mat->fragment_shader, token);
                }
            // Silently ignore a bunch of unsupported keywords:
            } else if (strcmp("#", token) == 0);
            else if (strcmp("illum", token) == 0);
            else if (strcmp("map_Bump", token) == 0); // Doesn't seem to be part of the spec?
            else if (strcmp("illum", token) == 0);
            else if (strcmp("d", token) == 0);
            else if (strcmp("Ni", token) == 0);
            else {
                OUTPUT("Unknown keyword \"%s\" encountered on line %d in file \"%s\"."
                    " Ignoring.\n", token, line_count, mtlpath);
            }
        }
    }

    fclose(fd);

}




// Parse a material name.
static void parse_usemtl(void)
{
    unsigned int i;
    Material *mat;

    // Lookup material.
    if ( !parse_token_obj() ) {
        OUTPUT("Failed to parse material name while parsing \"%s\" on line %u.\n", filename, line_count);
        return;
    }

    mat = materials.find(token);

    // Find if any existing group uses this material and switch to it if so,
    for (i = 0; i < num_groups; i++) {
        if (groups[i].material == mat) {
            cur_group = i;
            return;
        }
    }

    // Or if that fails, create a new group if the current one isn't still empty.
    if (groups[cur_group].num_vertices) {

        // Add group if there's room
        if (num_groups < MESH_MAXGROUPS) {
            cur_group = num_groups++;
            groups[cur_group].material = mat;
        } else {
            OUTPUT("Unable to process material \"%s\" for \"%s\", reached MAXGROUPS.\n", token, filename);
        }
    } else {
        // Reuse current group since it's empty.
        groups[cur_group].material = mat;
    }
}



// Transform the vertex/index data in groups into a single array in mesh. Returns 1 on success or 0
// if an error occurs. After this function completes it is guaranteed that the group vertex/index
// buffers are freed.
static int transform_groups_to_mesh(void)
{
    unsigned int i, j;

    // Figure out how much memory needs to be allocated for the unified vertex/index buffers.
    for (i = 0; i < num_groups; i++) {
        mesh->vertices_size += groups[i].num_vertices;
        mesh->indices_size += groups[i].num_indices;
    }

    // Allocated the arrays.
    if (mesh->vertices_size) {
        mesh->vertices = (float *) malloc(mesh->vertices_size * mesh-> elem_size * sizeof(float));
        if (!mesh->vertices)
            goto error_cleanup;
    }

    if (mesh->flags & MESH_VA_INDEXED && mesh->indices_size) {
        mesh->indices = (unsigned int *) malloc(mesh->indices_size * sizeof(unsigned int));
        if (!mesh->indices)
            goto error_cleanup;
    }

    // Iterate over all the groups, copy vertices / indices into arrays, update mesh group
    // start/counts.
    for (i = 0; i < num_groups; i++) {

        // Update mesh group.
        mesh->groups[i].material = groups[i].material;

        if (mesh->flags & MESH_VA_INDEXED) {
            mesh->groups[i].start = mesh->num_indices;
            mesh->groups[i].count = groups[i].num_indices;
        } else {
            mesh->groups[i].start = mesh->num_vertices;
            mesh->groups[i].count = groups[i].num_vertices;
        }
        mesh->num_groups++;

        // Append vertices.
        memcpy(mesh->vertices+(mesh->num_vertices*mesh->elem_size), groups[i].vertices,
            groups[i].num_vertices*mesh->elem_size*sizeof(float));

        // Add indices, be sure to add group offset.
        if (mesh->flags & MESH_VA_INDEXED) {
            for (j = 0; j < groups[i].num_indices; j++) {
                mesh->indices[mesh->num_indices++] = groups[i].indices[j] + mesh->num_vertices ;
                //mesh->indices[mesh->num_indices++] = groups[i].indices[j] + mesh->num_vertices;
            }
        }
        mesh->num_vertices += groups[i].num_vertices;

        // Free this group's buffers.
        free(groups[i].vertices);
        free(groups[i].indices);
    }
    return 1;

error_cleanup:

    OUTPUT("Failed to allocate mesh vertex or index buffer while parsing \"%s\".\n", filename);

    free(mesh->vertices);
    free(mesh->indices);
    mesh->indices = NULL;
    mesh->vertices = NULL;

    for (i = 0; i < num_groups; i++) {
        free(groups[i].vertices);
        free(groups[i].indices);
    }
    return 0;
}



// Load mesh from a Wavefront .obj file.
void Mesh::LoadWavefrontObj(const char *file, unsigned int flags)
{
    FILE *fd;
    filename = file;
    vertices_size = normals_size = texcoords_size = OBJ_VEC3_BUFFER_INC;
    vertex_count = normal_count = texcoord_count = ignored_faces = 0;
    warned_inconsistency = triangle_count = line_count = format_picked = face_vertex_count = 0;
    scale = 0.0f;
    tex_prefix[0] = '\0';

    if ( (fd = fopen(file, "rb")) == NULL ) {
        OUTPUT("Failed to open OBJ mesh \"%s\".", file);
        return;
    }

    load_flags = flags;

    vertices  = (float *) malloc(OBJ_VEC3_BUFFER_INC * sizeof(Vec3));
    normals   = (Vec3 *) malloc(OBJ_VEC3_BUFFER_INC * sizeof(Vec3));
    texcoords = (Vec3 *) malloc(OBJ_VEC3_BUFFER_INC * sizeof(Vec3));

    if ( !vertices || !normals || !texcoords ) {

        OUTPUT("%s: Failed to allocate memory while parsing \"%s\". Aborting.\n", __func__, filename);
        free(vertices);
        free(normals);
        free(texcoords);
		if ( fd ) fclose(fd);
        return;
    }

    // Initialize groups and setup initial group.
    memset(groups, '\0', sizeof(groups));
    num_groups = 1;
    cur_group = 0;
    groups[0].material = &default_material;

    // By default I use indexed vertex arrays, unless the NOINDEX load flags was given. If it later
    // turns out (after loading, see below) that no vertices were shared I remove the indices and
    // unset the VA_INDEXED bit again.
    if ( !(load_flags & MESH_LOAD_NOINDEX) )
        mesh->flags |= MESH_VA_INDEXED;

    // Process each line.
    while (fgets(line, OBJ_LINE_BUFFER_SIZE, fd)) {

        line_pos = line;
        line_count++;

        // Parse the data-type keyword (v, vn, vt, f, etc.).
        if ( parse_token() ) {

            if (strcmp("v", token) == 0)            parse_vec3(OBJ_DATATYPE_VERTEX);
            else if (strcmp("vn", token) == 0)      parse_vec3(OBJ_DATATYPE_NORMAL);
            else if (strcmp("vt", token) == 0)      parse_vec3(OBJ_DATATYPE_TEXCOORD);
            else if (strcmp("f", token) == 0)       parse_face();
            else if (strcmp("scale", token) == 0)   parse_scale();
            else if (strcmp("mtllib", token) == 0)  parse_mtllib();
            else if (strcmp("usemtl", token) == 0)  parse_usemtl();
            else if (strcmp("normalize", token) == 0) load_flags |= MESH_LOAD_NORMALIZE;

            // These are silently ignored.
            else if (strcmp("s", token) == 0);
            else if (strcmp("o", token) == 0);
            else if (strcmp("g", token) == 0);
            else if (strcmp("#", token) == 0);
            else {
                zWarning("Unknown keyword encountered on line %d in file \"%s\". Ignoring.",
                    line_count, filename);
            }
        }
    }

    // We're now done with parsing and dereferencing v/vn/vt so I can close the file and get rid of
    // those buffers.
    free(vertices);
    free(normals);
    free(texcoords);
    fclose(fd);

    if (transform_groups_to_mesh()) {
        // Get rid of index array if no vertices were shared..
        // XXX: Is this actually safe? Maybe indices and vertices are equal but vertices might not
        // be ordered right?
        if ( (mesh->flags & MESH_VA_INDEXED) && mesh->num_vertices == mesh->num_indices) {
            zDebug("While loading mesh \"%s\": mesh has no shared vertices, getting rid of index"
                " array.", file);
            mesh->flags &= ~MESH_VA_INDEXED;
            free(mesh->indices);
            mesh->indices = NULL;
            mesh->num_indices = 0;
        }
    } else {
        mesh = NULL;
    }

    if (ignored_faces) {
        zWarning("%u faces were ignored due to parsing errors in \"%s\".", ignored_faces, filename);
    }

    return mesh;
}

//-----------------------------------------------------------------------------------------------------
// Stanford .PLY files

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Parser for Stanford PLY models. 
 *
 * More info: http://local.wasp.uwa.edu.au/~pbourke/dataformats/ply/
 *
 * The header specifies what kind of elements are listed (vertices, faces), the order in which they
 * are listed and how many elements there are in the list. Following the specification of an element
 * listing, it gives a number of properties that each element consists of, the property specifics a
 * storage type (float, list) and it's purpose (x,y, z for vertices, vertex_indices for faces would
 * be common examples).
 *
 * The order in which the 'element' and 'property' keywords are listed specifies the order in which
 * the data is organized.
 *
 * Although I may not recognize the kind of element being listed or the prupose of some properties
 * (since these can be user-defined), as long as I know the storage type I can still process the
 * file since I will know how much data to read (and ignore).
 */

#define PLY_LINEBUF_SIZE  512
#define PLY_TOKEN_SIZE    512

// Some (hopefully) reasonably limits on the number of element lists and properties. Parsing will
// fail if these are exceeded so they are liberal. Mainly materials could have lots of properties (4
// for each color for example).
#define PLY_MAX_PROPS     40
#define PLY_MAX_ELEMLISTS 10

#define PLY_FORMAT_ASCII 1
#define PLY_FORMAT_BINLE 2
#define PLY_FORMAT_BINBE 3

// The element list types that I know how to parse.
// TODO: Add normals, texcoords?
#define PLY_ELEMTYPE_UNRECOGNIZED   0
#define PLY_ELEMTYPE_VERTEX         1
#define PLY_ELEMTYPE_FACE           2


// Purpose (meaning) of the property that I know what to do with.
#define PLY_PURPOSE_UNRECOGNIZED 0
#define PLY_PURPOSE_XCOORD       1
#define PLY_PURPOSE_YCOORD       2
#define PLY_PURPOSE_ZCOORD       3
#define PLY_PURPOSE_VINDICES     4


// Storage type of the property.
enum ply_proptype {
    PLY_TYPE_INVALID = 0,
    PLY_TYPE_CHAR,
    PLY_TYPE_UCHAR,
    PLY_TYPE_SHORT,
    PLY_TYPE_USHORT,
    PLY_TYPE_INT,
    PLY_TYPE_UINT,
    PLY_TYPE_FLOAT,
    PLY_TYPE_DOUBLE,
    PLY_TYPE_LIST,
    PLY_TYPE_NUM
};

static const size_t type_size[PLY_TYPE_NUM] = { 0, 1, 1, 2, 2, 4, 4, 4, 8, 0 };


// For diagnostic messages..
static const char *filename;
static unsigned int line_count;


// Temp buffers to store lines being read, and to read tokens from/into.
static char line[PLY_LINEBUF_SIZE];
static char *line_pos;
static char token[PLY_TOKEN_SIZE];


// Some macros to prevent RSI :p
#define PLY_CURELEM                (ply_header.elemlists[ply_header.num_elemlists-1])
#define PLY_CURPROP                (PLY_CURELEM.props[PLY_CURELEM.num_props-1])
#define PLY_ELEM(elemnum)          (ply_header.elemlists[(elemnum)])
#define PLY_PROP(elemnum, propnum) (ply_header.elemlists[(elemnum)].props[(propnum)])


typedef struct ply_property
{
    int type;    // Storage type of the property (float, int, list)
    int purpose; // Purpose of the propery (x/y/z coordinate, indices)

    // Only used for PLY_TYPE_LIST:
    int list_length_type;
    int list_member_type;

} ply_property;


typedef struct ply_elemlist
{
    int type; // Type of element listed (verts, faces, texcoords..)
    unsigned int count; // Number of elements in the list.

    // Properties to be parsed for each element in this list.
    unsigned int num_props;
    ply_property props[PLY_MAX_PROPS];

} ply_elemlist;


static struct
{
    int format;
    int version_major;
    int version_minor;

    // Element lists that need to be parsed.
    unsigned int num_elemlists;
    ply_elemlist elemlists[PLY_MAX_ELEMLISTS];

    unsigned int size; // Size of the header in bytes.

} ply_header;



// Read a token from line into token. Returns pointer to token on success or NULL on failure. After
// parse_token() returns, token is guaranteed to either be the token read, or an empty string. If
// the token being read from line was too large, the entire token in line will be skipped, and token
// will be set to an empty string.
static char *parse_token_ply(void)
{
    char *p = line_pos, *o = token;
    int count = 0, skipped = 0;

    // Skip whitespace.
    while (*p == ' ' || *p == '\t') p++;

    // Read chars to token and stop if we hit whitespace, newline, \0, or if we don't have enough
    // room in token to fit a char + \0.
    while ( *p != ' '  && *p != '\t' && *p != '\0' && *p != '\n' && *p != '\r') {

        // Only copy character if there's enough room left.
        //if ( (PLY_TOKEN_SIZE-count) > 1 ) {
        if ( o < (token+PLY_TOKEN_SIZE) ) {
            *o++ = *p++;
            count++;
        } else {
            skipped++;
        }
    }

    // Always update line position. This way even if a token was too long and ignored, we still move
    // ahead correctly so the next token on the line can be parsed..
    line_pos = p;

    // If token was too long and got truncated, make token an empty string and return 0.
    if (skipped) {
        OUTPUT("Unable to read entire token on line %u while parsing \"%s\", token buffer too"
            " small.\n", line_count, filename);
        token[0] = '\0';
        return NULL;
    }

    *o = '\0';
    return token;
}



// Parse format from header.
static void parse_format(void) 
{
    unsigned int version_major, version_minor;

    if (!parse_token())
        return;

    if (strcmp("binary_big_endian", token) == 0)
        ply_header.format = PLY_FORMAT_BINBE;
    else if (strcmp("binary_little_endian", token) == 0)
        ply_header.format = PLY_FORMAT_BINLE;
    else if (strcmp("ascii", token) == 0)
        ply_header.format = PLY_FORMAT_ASCII;

    if (!parse_token())
        return;

    if (sscanf(token, "%u.%u", &version_major, &version_minor) != 2)
        return;

    ply_header.version_major = version_major;
    ply_header.version_minor = version_minor;
}



// Translate string to type.
static int get_type(const char *name)
{
    if      (strcmp("char",   name) == 0) return PLY_TYPE_CHAR;
    else if (strcmp("uchar",  name) == 0) return PLY_TYPE_UCHAR;
    else if (strcmp("short",  name) == 0) return PLY_TYPE_SHORT;
    else if (strcmp("ushort", name) == 0) return PLY_TYPE_USHORT;
    else if (strcmp("int",    name) == 0) return PLY_TYPE_INT;
    else if (strcmp("uint",   name) == 0) return PLY_TYPE_UINT;
    else if (strcmp("float",  name) == 0) return PLY_TYPE_FLOAT;
    else if (strcmp("double", name) == 0) return PLY_TYPE_DOUBLE;
    else if (strcmp("list",   name) == 0) return PLY_TYPE_LIST;
    else return 0;
}



// Translate string to purpose.
static int get_purpose(const char *name)
{
    if      (strcmp("x",              name) == 0) return PLY_PURPOSE_XCOORD;
    else if (strcmp("y",              name) == 0) return PLY_PURPOSE_YCOORD;
    else if (strcmp("z",              name) == 0) return PLY_PURPOSE_ZCOORD;
    else if (strcmp("vertex_indices", name) == 0) return PLY_PURPOSE_VINDICES;
    else return 0;
}



// Read type/purpose from line into ply_header. Returns 0 on any error or 1 otherwise.
static int parse_property(void)
{
    // Make sure I don't exceed limits.
    if ( !ply_header.num_elemlists ) {
        OUTPUT("Malformed header - property without element on line %u while parsing \"%s\".\n",
            line_count, filename);
        return 0;
    }

    if ( PLY_CURELEM.num_props >= PLY_MAX_PROPS ) {
        OUTPUT("Exceeded maximum number of properties support on line %u while parsing \"%s\".\n",
            line_count, filename);
        return 0;
    }


    // Add new property to current elemlist and get a convenient pointer to it..
    PLY_CURELEM.num_props++;

    // Parse primary type.
    parse_token();
    if ( !(PLY_CURPROP.type = get_type(token)) )
        return 0;

    // If this was a list I need to parse two more types (for list-length and list-element).
    if (PLY_CURPROP.type == PLY_TYPE_LIST) {
        parse_token();
        PLY_CURPROP.list_length_type = get_type(token);
        parse_token();
        PLY_CURPROP.list_member_type = get_type(token);

        if ( !PLY_CURPROP.list_length_type ||
             !PLY_CURPROP.list_member_type)
            return 0;
    }

    // And finally the purpose
    parse_token();
    if ( !(PLY_CURPROP.purpose = get_purpose(token)) )
        return 0;

    return 1;
}



// Translate string to element type.
static int get_elemtype(const char *name)
{
    if (strcmp("vertex", token) == 0)
        return PLY_ELEMTYPE_VERTEX;
    else if (strcmp("face", token) == 0)
        return PLY_ELEMTYPE_FACE;
    else
        return PLY_ELEMTYPE_UNRECOGNIZED;
}



// Parse PLY header. Returns 0 on error, 1 on success. If this function fails, ply_header is
// may be useless and parsing should be aborted.
static int parse_header(FILE* fd)
{
    // Parse header.
    while (fgets(line, PLY_LINEBUF_SIZE, fd)) {

        line_count++;
        line_pos = line;

        if (parse_token()) {

            if (strcmp("format", token) == 0) {
                parse_format();

            } else if (strcmp("element", token) == 0) {

                if (ply_header.num_elemlists >= PLY_MAX_ELEMLISTS) {
                    zWarning("Exceeded maximum number of element lists supported on line %u while"
                        " parsing \"%s\".", line_count, filename);
                    return 0;
                }

                ply_header.num_elemlists++;
                parse_token();
                PLY_CURELEM.type = get_elemtype(token);
                parse_token();
                PLY_CURELEM.count = (unsigned int) atoi(token);

            } else if (strcmp("property", token) == 0) {

                if (!parse_property()) {
                    zWarning("Failed to parse property on line %u while parsing \"%s\".",
                        line_count, filename);
                    return 0;
                }

            } else if (strcmp("end_header", token) == 0) {
                break;

            } else if (strcmp("comment", token) == 0) { // Silently ignore comments.
            } else {
                // I warn and return on this to prevent attempting to parse the entire file as a
                // header if it is malformed in some way..
                zWarning("Header contains unrecognized keyword \"%s\" on line %u while parsing"
                    " \"%s\".", token, line_count, filename);
                return 0;
            }
        }
    }

    ply_header.size = (unsigned int) ftell(fd);
    return 1;
}



static void dump_header_info()
{
    unsigned int i, j;

    zDebug("PLY header:");
    zDebug("  format = %d, version = %d.%d", ply_header.format, ply_header.version_major,
        ply_header.version_minor);
    zDebug("  num_elemlists = %d", ply_header.num_elemlists);
    for (i = 0; i < ply_header.num_elemlists; i++) {
        zDebug("    element type = %d, count = %u, num_props = %u", ply_header.elemlists[i].type,
            ply_header.elemlists[i].count, ply_header.elemlists[i].num_props);
        for (j = 0; j < ply_header.elemlists[i].num_props; j++) {
            zDebug("      prop type = %d, purpose = %d", ply_header.elemlists[i].props[j].type,
                ply_header.elemlists[i].props[j].purpose);
            if (ply_header.elemlists[i].props[j].type == PLY_TYPE_LIST) {
                zDebug("        list types: length = %d, member = %d",
                        ply_header.elemlists[i].props[j].list_length_type,
                        ply_header.elemlists[i].props[j].list_member_type);
            }
        }
    }
}



Mesh *zLoadMeshPly(const char *file, unsigned int load_flags)
{
    FILE *fd;
    filename = file;
    line[0] = '\0';
    memset(&ply_header, 0, sizeof ply_header);

    // Open file, check magic bytes, parse header.
    if ( (fd = fopen(file, "rb")) == NULL ) {
        zError("Failed to open PLY mesh file \"%s\".", file);
        return NULL;
    }

    if ( !fgets(line, PLY_LINEBUF_SIZE, fd) || strcmp(line, "ply\n") != 0 ) {
        zError("Failed to load \"%s\", this does not seem to be a PLY mesh.", file);
        goto load_mesh_error_0;
    }
    line_count++;


    if (!parse_header(fd)) {
        zError("Failed to parse header for \"%s\".", filename);
        goto load_mesh_error_0;
    }

    dump_header_info(0);

    // Make sure we support the version. For now support everything, just emit a warning if it's not
    // 1.0, so that I can still read future backward-compatible versions..
    if (ply_header.version_major != 1 || ply_header.version_minor != 0)
        zWarning("Unsupported version (%u.%u) for PLY file format while parsing \"%s\", trying"
            " anyway.", ply_header.version_major, ply_header.version_minor, filename);


load_mesh_error_0:
    fclose(fd);
    return NULL;
}
#endif