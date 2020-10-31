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

#include "GLSetup.h"

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <string.h>

#include "GLMCamera.h"

// Source: vboindexer.hpp/cpp (GLM-related tutorials)
struct GLMPackedVertex{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const GLMPackedVertex that) const {
  return memcmp((void*)this, (void*)&that, sizeof(GLMPackedVertex))>0;
 }
};

class GLMVBO {
public:
 std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;

 // Returns true iif v1 can be considered equal to v2
 bool is_near(float v1, float v2) {	return fabs( v1-v2 ) < 0.01f; }

 // Searches through all already-exported vertices
 // for a similar one.
 // Similar = same position + same UVs + same normal
 bool getSimilarVertexIndex( 
 	glm::vec3 & in_vertex, 
 	glm::vec2 & in_uv, 
 	glm::vec3 & in_normal, 
 	std::vector<glm::vec3> & out_vertices,
 	std::vector<glm::vec2> & out_uvs,
 	std::vector<glm::vec3> & out_normals,
 	unsigned short & result ) {
  // Lame linear search
  for ( unsigned int i=0; i<out_vertices.size(); i++ ){
  	if (
  		is_near( in_vertex.x , out_vertices[i].x ) &&
  		is_near( in_vertex.y , out_vertices[i].y ) &&
  		is_near( in_vertex.z , out_vertices[i].z ) &&
  		is_near( in_uv.x     , out_uvs     [i].x ) &&
  		is_near( in_uv.y     , out_uvs     [i].y ) &&
  		is_near( in_normal.x , out_normals [i].x ) &&
  		is_near( in_normal.y , out_normals [i].y ) &&
  		is_near( in_normal.z , out_normals [i].z )
  	) {
	 	 result = i;
	 		return true;
	 	}
	 }
 	// No other vertex could be used instead.
 	// Looks like we'll have to add it to the VBO.
 	return false;
 }

 void indexVBO_slow(
 	std::vector<glm::vec3> & in_vertices,
 	std::vector<glm::vec2> & in_uvs,
 	std::vector<glm::vec3> & in_normals,
 	std::vector<unsigned short> & out_indices,
 	std::vector<glm::vec3> & out_vertices,
 	std::vector<glm::vec2> & out_uvs,
 	std::vector<glm::vec3> & out_normals
 ) {
 	// For each input vertex
 	for ( unsigned int i=0; i<in_vertices.size(); i++ ) {
 		// Try to find a similar vertex in out_XXXX
 		unsigned short index;
 		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);
 		if ( found ){ // A similar vertex is already in the VBO, use it instead !
 			out_indices.push_back( index );
 		} else { // If not, it needs to be added in the output data.
	 		out_vertices.push_back( in_vertices[i]);
	 		out_uvs     .push_back( in_uvs[i]);
	 		out_normals .push_back( in_normals[i]);
	 		out_indices .push_back( (unsigned short)out_vertices.size() - 1 );
	 	}
	 }
 }

 bool getSimilarVertexIndex_fast( 
 	GLMPackedVertex & packed, 
 	std::map<GLMPackedVertex,unsigned short> & VertexToOutIndex,
 	unsigned short & result
 ) {
 	std::map<GLMPackedVertex,unsigned short>::iterator it = VertexToOutIndex.find(packed);
 	if ( it == VertexToOutIndex.end() ){
 		return false;
 	} else {
 		result = it->second;
	 	return true;
	 }
 }

 void indexVBO(
 	std::vector<glm::vec3> & in_vertices,
 	std::vector<glm::vec2> & in_uvs,
 	std::vector<glm::vec3> & in_normals,
 	std::vector<unsigned short> & out_indices,
 	std::vector<glm::vec3> & out_vertices,
	 std::vector<glm::vec2> & out_uvs,
	 std::vector<glm::vec3> & out_normals
 ) {
	 std::map<GLMPackedVertex,unsigned short> VertexToOutIndex;
 	// For each input vertex
 	for ( unsigned int i=0; i<in_vertices.size(); i++ ){
 		GLMPackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};	
 		// Try to find a similar vertex in out_XXXX
 		unsigned short index;
 		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);
 		if ( found ) { // A similar vertex is already in the VBO, use it instead !
 			out_indices.push_back( index );
 		} else { // If not, it needs to be added in the output data.
 			out_vertices.push_back( in_vertices[i]);
 			out_uvs     .push_back( in_uvs[i]);
 			out_normals .push_back( in_normals[i]);
 			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
 			out_indices .push_back( newindex );
 			VertexToOutIndex[ packed ] = newindex;
 		}
 	}
 }

 void fread( FILE *fp, const char *filename, size_t length, void *target, size_t size ) {
  clearerr(fp);
  size_t total=length*size;
  size=fread_s(target, total, size, length, fp);
  if ( size != length ) {
   OUTPUT("GLMVBO::fread(`%s`) warns, fread_s: %d read values did not meet the expected size of %d\n",
    filename, (int) size, (int) length );
   if ( feof(fp) ) return;
   else {
    OUTPUT("ferror() reports %d, errno=%d\n", ferror(fp), (int) errno );
    OUTPUT("Error message was "); print_fopen_err((int)errno);
   }
   OUTPUT("If no EOF detected, possible causes: opening a file for reading when writing or vice versa (calling read or write in the wrong place),\n actual file i/o error, a modicum of other possibilities\n");
  } else
  if ( ferror(fp) != 0 ) {
   OUTPUT("ferror() reports %d, errno=%d\n", ferror(fp), (int) errno );
   OUTPUT("Error message was "); print_fopen_err((int)errno);
  }
 }

 bool loadOBJ(
 	const char * filename, 
 	std::vector<glm::vec3> & out_vertices, 
 	std::vector<glm::vec2> & out_uvs,
 	std::vector<glm::vec3> & out_normals
 ){
 	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
 	std::vector<glm::vec3> temp_vertices; 
 	std::vector<glm::vec2> temp_uvs;
 	std::vector<glm::vec3> temp_normals;
 	OUTPUT("Loading OBJ file %s...\n", filename);
  Strings file;
  file.Load(filename);
  Zint line;
  EACH(file.first,String,s) {
   line++;
   const char *p=s->s.c_str();
   if ( *p == '#' ) continue;
   if ( *p == 'm' ) {
   } else
   if ( *p == 'u' ) {
   } else
   if ( *p == 's' ) {
   } else
   if ( *p == 'f' ) {
    Strings words;
    words.SplitPush(p,' ');
    if ( words.count == 4) {
     String *word=(String *)words.first;
     String *one=(String *)word->next;
     String *two=(String *)one->next;
     String *three=(String *)two->next;
     Strings set1,set2,set3;
     set1.SplitPush(one->s.c_str(),'/');
     set2.SplitPush(two->s.c_str(),'/');
     set3.SplitPush(three->s.c_str(),'/');

     unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
     vertexIndex[0]=vertexIndex[1]=vertexIndex[2]=uvIndex[0]=uvIndex[1]=uvIndex[2]=normalIndex[0]=normalIndex[1]=normalIndex[2]=0;

     one=(String *) set1.first;
     two=(String *) one->next;
     three=(String *) two->next;

     vertexIndex[0]=atoi(one->s.c_str());
     uvIndex[0]    =atoi(two->s.c_str());
     normalIndex[0]=atoi(three->s.c_str());

     one=(String *) set2.first;
     two=(String *) one->next;
     three=(String *) two->next;

     vertexIndex[1]=atoi(one->s.c_str());
     uvIndex[1]    =atoi(two->s.c_str());
     normalIndex[1]=atoi(three->s.c_str());

     one=(String *) set3.first;
     two=(String *) one->next;
     three=(String *) two->next;

     vertexIndex[2]=atoi(one->s.c_str());
     uvIndex[2]    =atoi(two->s.c_str());
     normalIndex[2]=atoi(three->s.c_str());

  			vertexIndices.push_back(vertexIndex[0]);
  			vertexIndices.push_back(vertexIndex[1]);
  			vertexIndices.push_back(vertexIndex[2]);
  			uvIndices    .push_back(uvIndex[0]);
  			uvIndices    .push_back(uvIndex[1]);
  			uvIndices    .push_back(uvIndex[2]);
  			normalIndices.push_back(normalIndex[0]);
  			normalIndices.push_back(normalIndex[1]);
  			normalIndices.push_back(normalIndex[2]);
    }
   } else
   if ( *p == 'v' ) {
    Strings words;
    words.SplitPush(p,' ');
    String *word=(String *) words.first;
    if ( words.count == 4 && !str_cmp(word->c_str(),"v") ) {
     glm::vec3 vertex;
     String *one=(String *)word->next;
     String *two=(String *)one->next;
     String *three=(String *)two->next;
     vertex.x=(float)atof(one->s.c_str());
     vertex.y=(float)atof(two->s.c_str());
     vertex.z=(float)atof(three->s.c_str());
  			temp_vertices.push_back(vertex);
    } else if ( words.count == 3 && !str_cmp(word->c_str(),"vt") ) {
     glm::vec2 tc;
     String *one=(String *)word->next;
     String *two=(String *)one->next;
     tc.x=(float)atof(one->s.c_str());
     tc.y=(float)atof(two->s.c_str());
  			temp_uvs.push_back(tc);
    } else if ( words.count == 4 && !str_cmp(word->c_str(),"vn") ) {
     glm::vec3 normal;
     String *one=(String *)word->next;
     String *two=(String *)one->next;
     String *three=(String *)two->next;
     normal.x=(float)atof(one->s.c_str());
     normal.y=(float)atof(two->s.c_str());
     normal.z=(float)atof(three->s.c_str());
  			temp_normals.push_back(normal);
    }
//    else OUTPUT("Error parsing file on line %d:\n> %s\n", line, p);
   }
  }
 
 	// For each vertex of each triangle
 	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
 
 		// Get the indices of its attributes
 		unsigned int vertexIndex = vertexIndices[i];
 		unsigned int uvIndex = uvIndices[i];
 		unsigned int normalIndex = normalIndices[i];
 		
 		// Get the attributes thanks to the index
 		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
 		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
 		glm::vec3 normal = temp_normals[ normalIndex-1 ];
 		
 		// Put the attributes in buffers
 		out_vertices.push_back(vertex);
 		out_uvs     .push_back(uv);
 		out_normals .push_back(normal); 	
 	}
 
 	return true;
 }


 void LoadOBJ( const char *filename ) {
  bool result=loadOBJ(filename,vertices,uvs,normals);
  if ( !result ) OUTPUT( "GLMVBO:LoadOBJ(`%s`) Error loading .OBJ file\n", filename );
 	computeTangentBasis(
 		vertices, uvs, normals, // input
 		tangents, bitangents    // output
 	);
  indexVBO_TBN(
	 	vertices, uvs, normals, tangents, bitangents, 
	 	indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
	 );
 }

 void CreateQuadTBN() {
  const float _vertexes[6][3] = {  
   0.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,   1.0f,1.0f,0.0f,  
   1.0f,0.0f,0.0f,   1.0f,1.0f,0.0f,   0.0f,1.0f,0.0f  };
  const float _texcoords[6][2] = { 
   0.0f,0.0f,   1.0f,0.0f,   1.0f,1.0f, 
   1.0f,0.0f,   1.0f,1.0f,   0.0f,1.0f  };
  const float _normals[6][3] = {
   0.0f,0.0f,1.0f,
   0.0f,0.0f,1.0f,
   0.0f,0.0f,1.0f,
   0.0f,0.0f,1.0f,
   0.0f,0.0f,1.0f,
   0.0f,0.0f,1.0f  };
  
  for ( int i=0; i<6; i++ ) {
  	glm::vec3 vertex(_vertexes[i][0],_vertexes[i][1],_vertexes[i][2]);
  	glm::vec2 uv(_texcoords[i][0],_texcoords[i][1]);
  	glm::vec3 normal(_normals[i][0],_normals[i][1],_normals[i][2]);
   vertices.push_back(vertex);
 		uvs     .push_back(uv);
 		normals .push_back(normal);
  }

 	computeTangentBasis(
 		vertices, uvs, normals, // input
 		tangents, bitangents    // output
 	);
  indexVBO_TBN(
	 	vertices, uvs, normals, tangents, bitangents, 
	 	indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
	 );
 }

 void indexVBO_TBN(
 	std::vector<glm::vec3> & in_vertices,
 	std::vector<glm::vec2> & in_uvs,
 	std::vector<glm::vec3> & in_normals,
 	std::vector<glm::vec3> & in_tangents,
 	std::vector<glm::vec3> & in_bitangents,

 	std::vector<unsigned short> & out_indices,
 	std::vector<glm::vec3> & out_vertices,
 	std::vector<glm::vec2> & out_uvs,
 	std::vector<glm::vec3> & out_normals,
 	std::vector<glm::vec3> & out_tangents,
 	std::vector<glm::vec3> & out_bitangents
 ){
 	// For each input vertex
 	for ( unsigned int i=0; i<in_vertices.size(); i++ ){
 
 		// Try to find a similar vertex in out_XXXX
 		unsigned short index;
 		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i],     out_vertices, out_uvs, out_normals, index);
 
 		if ( found ){ // A similar vertex is already in the VBO, use it instead !
 			out_indices.push_back( index );
 
 			// Average the tangents and the bitangents
 			out_tangents[index] += in_tangents[i];
 			out_bitangents[index] += in_bitangents[i];
 		}else{ // If not, it needs to be added in the output data.
 			out_vertices.push_back( in_vertices[i]);
 			out_uvs     .push_back( in_uvs[i]);
 			out_normals .push_back( in_normals[i]);
 			out_tangents .push_back( in_tangents[i]);
 			out_bitangents .push_back( in_bitangents[i]);
 			out_indices .push_back( (unsigned short)out_vertices.size() - 1 );
 		}
 	}
 }

 void computeTangentBasis(
 	// inputs
 	std::vector<glm::vec3> & vertices,
 	std::vector<glm::vec2> & uvs,
 	std::vector<glm::vec3> & normals,
 	// outputs
 	std::vector<glm::vec3> & tangents,
 	std::vector<glm::vec3> & bitangents
 ) {
 
 	for (unsigned int i=0; i<vertices.size(); i+=3 ){
 
 		// Shortcuts for vertices
 		glm::vec3 & v0 = vertices[i+0];
 		glm::vec3 & v1 = vertices[i+1];
 		glm::vec3 & v2 = vertices[i+2];
 
 		// Shortcuts for UVs
 		glm::vec2 & uv0 = uvs[i+0];
 		glm::vec2 & uv1 = uvs[i+1];
 		glm::vec2 & uv2 = uvs[i+2];
 
 		// Edges of the triangle : postion delta
 		glm::vec3 deltaPos1 = v1-v0;
 		glm::vec3 deltaPos2 = v2-v0;
 
 		// UV delta
 		glm::vec2 deltaUV1 = uv1-uv0;
 		glm::vec2 deltaUV2 = uv2-uv0;
 
 		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
 		glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
 		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;
 
 		// Set the same tangent for all three vertices of the triangle.
 		// They will be merged later, in vboindexer.cpp
 		tangents.push_back(tangent);
 		tangents.push_back(tangent);
 		tangents.push_back(tangent);
 
 		// Same thing for binormals
 		bitangents.push_back(bitangent);
 		bitangents.push_back(bitangent);
 		bitangents.push_back(bitangent);
 
 	}
 
 	// See "Going Further"
 	for (unsigned int i=0; i<vertices.size(); i+=1 )
 	{
 		glm::vec3 & n = normals[i];
 		glm::vec3 & t = tangents[i];
 		glm::vec3 & b = bitangents[i];
 		
 		// Gram-Schmidt orthogonalize
 		t = glm::normalize(t - n * glm::dot(n, t));
 		
 		// Calculate handedness
 		if (glm::dot(glm::cross(n, t), b) < 0.0f){
 			t = t * -1.0f;
 		}
 	}
 }

	GLuint VertexArrayID;
 GLuint vertexbuffer;
 GLuint uvbuffer;
 GLuint normalbuffer;
 GLuint tangentbuffer;
 GLuint bitangentbuffer;
 GLuint elementbuffer;
 Zbool uploaded;

 void CreateQuadTBNVBO() {
  CreateQuadTBN();
  CreateTBNVBO();
 }

 void BindToStreamsTBN(
   GLuint vertexPosition_modelspaceID,
   GLuint vertexUVID,
   GLuint vertexNormal_modelspaceID,
   GLuint vertexTangent_modelspaceID,
   GLuint vertexBitangent_modelspaceID
  ) {
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			vertexNormal_modelspaceID,    // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 4th attribute buffer : tangents
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glVertexAttribPointer(
			vertexTangent_modelspaceID,    // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 5th attribute buffer : bitangents
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glVertexAttribPointer(
			vertexBitangent_modelspaceID,    // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
 }

 void UnbindFromStreamsTBN() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
 }

 // Load it into a VBO
 void CreateTBNVBO() {
 	glGenVertexArrays(1, &VertexArrayID);
 	glBindVertexArray(VertexArrayID);

	 glGenBuffers(1, &vertexbuffer);
	 glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	 glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
  
	 glGenBuffers(1, &uvbuffer);
	 glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	 glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
  
	 glGenBuffers(1, &normalbuffer);
	 glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	 glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
  
	 glGenBuffers(1, &tangentbuffer);
	 glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	 glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);
  
	 glGenBuffers(1, &bitangentbuffer);
	 glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	 glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);
  
	 // Generate a buffer for the indices as well
	 glGenBuffers(1, &elementbuffer);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
  glReportError(glGetError());
  uploaded=true;
 }

 void Render() {		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			(GLsizei) indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*)0           // element array buffer offset
		);
 }

 void DebugRender( glm::vec3 lightPos, GLMCamera *camera ) {

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((const GLfloat*)&camera->ProjectionMatrix[0]);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((const GLfloat*)&camera->ModelViewMatrix[0]);
  
  Coordinator();

		// normals
		glColor3f(0,0,1);
		glBegin(GL_LINES);
		for (unsigned int i=0; i<indices.size(); i++){
			glm::vec3 p = indexed_vertices[indices[i]];
			glVertex3fv(&p.x);
			glm::vec3 o = glm::normalize(indexed_normals[indices[i]]);
			p+=o*0.1f;
			glVertex3fv(&p.x);
		}
		glEnd();
		// tangents
		glColor3f(1,0,0);
		glBegin(GL_LINES);
		for (unsigned int i=0; i<indices.size(); i++){
			glm::vec3 p = indexed_vertices[indices[i]];
			glVertex3fv(&p.x);
			glm::vec3 o = glm::normalize(indexed_tangents[indices[i]]);
			p+=o*0.1f;
			glVertex3fv(&p.x);
		}
		glEnd();
		// bitangents
		glColor3f(0,1,0);
		glBegin(GL_LINES);
		for (unsigned int i=0; i<indices.size(); i++){
			glm::vec3 p = indexed_vertices[indices[i]];
			glVertex3fv(&p.x);
			glm::vec3 o = glm::normalize(indexed_bitangents[indices[i]]);
			p+=o*0.1f;
			glVertex3fv(&p.x);
		}
		glEnd();
		// light pos
		glColor3f(1,1,1);
		glBegin(GL_LINES);
			glVertex3fv(&lightPos.x);
			lightPos+=glm::vec3(1,0,0)*0.1f;
			glVertex3fv(&lightPos.x);
			lightPos-=glm::vec3(1,0,0)*0.1f;
			glVertex3fv(&lightPos.x);
			lightPos+=glm::vec3(0,1,0)*0.1f;
			glVertex3fv(&lightPos.x);
		glEnd();
 }
};