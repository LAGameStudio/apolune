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

#include "Art.h"
#include "LookAtPerspective.h"
#include "Camera.h"
#include "GLSL2Shader.h"

class ProtoShader : public GLSLShader {
public:
};

class ParallaxProtoShader : public GLSLShader {
public:
 GLuint *color,*normal,*height;
 GLuint 
  Ulight_direction, Ulight_ambient, Ulight_diffuse, Ulight_specular,
  Umaterial_ambient, Umaterial_diffuse, Umaterial_specular, Umaterial_shininess,
  Ucolor,Unormalmap,Uheight,
  Uprojection,Umodel,Uview,Unormal,
  UscaleBias;
 GLint posAttrib,texAttrib;
 gCamera camera;
 ImmediateLight light;
 GLMaterial material;
 float scaleBias[2];
 Matrix4 projectionMatrix;
 Matrix4 modelMatrix;
 Matrix3 normalMatrix;
 Quaternionf q;
 float rotationDegrees;
 float rotationSpeed;
 Vector3 rotationAxis;
 Vector3 up, target;
 void Perspective(float fovx, float aspect, float znear, float zfar) {
 	float e = 1.0f / tanf(dhpowareMath::degreesToRadians(fovx) / 2.0f);
 	float aspectInv = 1.0f / aspect;
 	float fovy = 2.0f * atanf(aspectInv / e);
 	float xScale = 1.0f / tanf(0.5f * fovy);
 	float yScale = xScale / aspectInv;
 	projectionMatrix[0][0] = xScale; 	projectionMatrix[0][1] = 0.0f;   	projectionMatrix[0][2] = 0.0f;                                  	 projectionMatrix[0][3] = 0.0f;
 	projectionMatrix[1][0] = 0.0f; 	  projectionMatrix[1][1] = yScale; 	projectionMatrix[1][2] = 0.0f;                                  	 projectionMatrix[1][3] = 0.0f;
 	projectionMatrix[2][0] = 0.0f; 	  projectionMatrix[2][1] = 0.0f;   	projectionMatrix[2][2] = (zfar + znear) / (znear - zfar); 	       projectionMatrix[2][3] = -1.0f;
 	projectionMatrix[3][0] = 0.0f; 	  projectionMatrix[3][1] = 0.0f;   	projectionMatrix[3][2] = (2.0f * zfar * znear) / (znear - zfar);  projectionMatrix[3][3] = 0.0f;
 }
 void Init() {
  color=normal=height=null;
  scaleBias[0]=0.04f;
  scaleBias[1]=-0.03f;
  up.set(0.0f,1.0f,0.0f);
  target.set(0.0f,0.0f,0.0f);
  rotationAxis.set(0.0f,0.0f,0.0f);
  rotationDegrees=0.0f;
  camera.Setup(display.w,display.h,&(camera.origin));
  light.direction.Set(0.0f, 0.0f, -1.0f, 0.0f);
  light.ambient.Set(1.0f, 1.0f, 1.0f, 1.0f);
  light.diffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular.Set(1.0f, 1.0f, 1.0f, 1.0f);
  material.ambient.Set(0.3f, 0.3f, 0.3f, 1.0f);
  material.diffuse.Set(0.7f, 0.7f, 0.7f, 1.0f);
  material.specular.Set(0.5f, 0.5f, 0.5f, 1.0f);
  material.shininess[0]=60.0f;
  OUTPUT("ParallaxProtoShader::Loading\n");
  string vert=
   string("#version 130\n")+
   string("precision highp float;\n")+
   string("uniform struct DirectionalLight {\n")+
   string("	vec3 direction;\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("} DirectionalLight1;\n")+
   string("uniform struct Material {\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("	float shininess;\n")+
   string("} Material1;\n")+
   string("uniform mat4 ProjectionMatrix;\n")+
   string("uniform mat4 ViewMatrix;\n")+
   string("uniform mat4 ModelMatrix;\n")+
   string("uniform mat3 NormalMatrix;\n")+
   string("in vec3 VertexPosition;\n")+
   string("in vec3 VertexNormal;\n")+
   string("in vec2 VertexTexCoord;\n")+
   string("in vec4 VertexTangent;\n")+
   string("out vec3 LightDir;\n")+
   string("out vec3 ViewDir;\n")+
   string("out vec2 TexCoord;\n")+
   string("void main() {\n")+
   string("	vec3 n = normalize(NormalMatrix * VertexNormal);\n")+
   string(" vec3 t = normalize(NormalMatrix * VertexTangent.xyz);\n")+
   string(" vec3 b = cross(n, t) * VertexTangent.w;\n")+
   string(" mat3 tbnMatrix = mat3(t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z);\n")+
   string("	mat4 modelViewMatrix = ViewMatrix * ModelMatrix;\n")+
   string(" mat4 mvpMatrix = ProjectionMatrix * modelViewMatrix;\n")+
   string(" vec4 pos = vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);\n")+
   string(" gl_Position = mvpMatrix * pos;\n")+
   string(" pos = modelViewMatrix * pos;\n")+
   string("	ViewDir = -(pos.xyz / pos.w);\n")+
   string("	ViewDir = tbnMatrix * ViewDir;\n")+
   string("	LightDir = vec3(ViewMatrix * vec4(-DirectionalLight1.direction, 0.0f));\n")+
   string("	LightDir = tbnMatrix * LightDir;\n")+
   string(" TexCoord = VertexTexCoord;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 130\n")+
   string("precision highp float;\n")+
   string("uniform struct DirectionalLight {\n")+
   string("	vec3 direction;\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("} DirectionalLight1;\n")+
   string("uniform struct Material {\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("	float shininess;\n")+
   string("} Material1;\n")+
   string("uniform sampler2D ColorMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D HeightMap;\n")+
   string("uniform vec2 ParallaxScaleBias;\n")+
   string("in vec3 LightDir;\n")+
   string("in vec3 ViewDir;\n")+
   string("in vec2 TexCoord;\n")+
   string("out vec4 FragColor;\n")+
   string("void main() {\n")+
   string("	vec3 L = normalize(LightDir);\n")+
   string("	vec3 V = normalize(ViewDir);\n")+
   string("	float height = texture(HeightMap, TexCoord).r;\n")+
   string("	height = height * ParallaxScaleBias.x + ParallaxScaleBias.y;\n")+
   string("	vec2 texCoord = TexCoord + (height * V.xy);\n")+
   string("	vec3 N = normalize(texture(NormalMap, texCoord.st).xyz * 2.0 - 1.0);\n")+
   string("	vec3 R = normalize(-reflect(L, N));\n")+
   string("	float nDotL = max(0.0, dot(N, L));\n")+
   string("	float rDotV = max(0.0, dot(R, V));\n")+
   string("	vec4 ambient = DirectionalLight1.ambient * Material1.ambient;\n")+
   string("	vec4 diffuse = DirectionalLight1.diffuse * Material1.diffuse * nDotL;\n")+
   string("	vec4 specular = DirectionalLight1.specular * Material1.specular * pow(rDotV, Material1.shininess);\n")+
   string("	vec4 texel = texture(ColorMap, texCoord);\n")+
   string(" FragColor = (ambient + diffuse + specular) * texel;\n")+
   string("}\n")
  ;
  Perspective(rad2deg((float)camera.fov), (float)(display.wd / display.hd), 0.1f, 20000.0f);
 	camera.LookAt(Vector3(camera.cPosition.x, camera.cPosition.y, camera.cPosition.z), target, up);
  Strings binds; binds.Add("outColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ulight_direction = glGetUniformLocation(program,"DirectionalLight1.direction");
  Ulight_ambient = glGetUniformLocation(program,"DirectionalLight1.ambient");
  Ulight_diffuse = glGetUniformLocation(program,"DirectionalLight1.diffuse");
  Ulight_specular = glGetUniformLocation(program,"DirectionalLight1.specular");
  Umaterial_ambient = glGetUniformLocation(program,"Material1.ambient");
  Umaterial_diffuse = glGetUniformLocation(program,"Material1.diffuse");
  Umaterial_specular = glGetUniformLocation(program,"Material1.specular");
  Umaterial_shininess = glGetUniformLocation(program,"Material1.shininess");
  Ucolor = glGetUniformLocation(program,"ColorMap");
  Unormalmap = glGetUniformLocation(program,"NormalMap");
  Uheight = glGetUniformLocation(program,"HeightMap");
  Uprojection = glGetUniformLocation(program,"ProjectionMatrix");
  Umodel = glGetUniformLocation(program,"ModelMatrix");
  Uview = glGetUniformLocation(program,"ViewMatrix");
  Unormal = glGetUniformLocation(program,"NormalMatrix");
  UscaleBias = glGetUniformLocation(program,"ParallaxScaleBias");
 }
 void UpdateUniforms() {
  Perspective(rad2deg((float)camera.fov), (float)(display.wd / display.hd), 0.1f, 20000.0f);
 	camera.LookAt(Vector3(camera.cPosition.x, camera.cPosition.y, camera.cPosition.z), target, up);
  glUseProgram(program);
  glUniform3fv(Ulight_direction, 1, &light.direction.coord[0] );
  glUniform4fv(Ulight_ambient, 1, &light.ambient.coord[0] );
  glUniform4fv(Ulight_diffuse, 1, &light.diffuse.coord[0] );
  glUniform4fv(Ulight_specular, 1, &light.specular.coord[0] );
  glUniform4fv(Umaterial_ambient, 1, &material.ambient.coord[0] );
  glUniform4fv(Umaterial_diffuse, 1, &material.ambient.coord[0] );
  glUniform4fv(Umaterial_specular, 1, &material.ambient.coord[0] );
  glUniform1f(Umaterial_ambient, material.shininess[0] );
  glUniform1i(Ucolor, 0);   glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *color);
  glUniform1i(Unormalmap, 0);  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *normal);
  glUniform1i(Uheight, 0);  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *height);
 	q.toAxisAngle(rotationAxis, rotationDegrees);
  Matrix4 modelMatrix = Matrix4::createRotate(rotationAxis, rotationDegrees);
  Matrix4 *temp=camera.viewMatrix.asMatrix4();
 	Matrix4 modelViewMatrix = modelMatrix * (*temp);
  glUniformMatrix4fv(Uprojection, 1, GL_FALSE, &projectionMatrix[0][0]);
  glUniformMatrix4fv(Umodel, 1, GL_FALSE, &modelMatrix[0][0] );
	 glUniformMatrix3fv(Unormal, 1, GL_FALSE, &normalMatrix[0][0] );
 	normalMatrix[0][0] = modelViewMatrix[0][0]; 	normalMatrix[0][1] = modelViewMatrix[0][1]; 	normalMatrix[0][2] = modelViewMatrix[0][2]; 
 	normalMatrix[1][0] = modelViewMatrix[1][0]; 	normalMatrix[1][1] = modelViewMatrix[1][1]; 	normalMatrix[1][2] = modelViewMatrix[1][2]; 
 	normalMatrix[2][0] = modelViewMatrix[2][0]; 	normalMatrix[2][1] = modelViewMatrix[2][1]; 	normalMatrix[2][2] = modelViewMatrix[2][2]; 
 	normalMatrix = normalMatrix.inverse().transpose();
	 glUniformMatrix4fv(Uview, 1, GL_FALSE, &(*temp)[0][0] ); // needs type optimization on this line

  delete temp;

	 glUniform2fv(UscaleBias, 1, scaleBias);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern ParallaxProtoShader parallaxprotoShader;

class ParallaxGlossProtoShader : public GLSLShader {
public:
 GLuint *color,*normal,*height,*gloss;
 GLuint 
  Ulight_direction, Ulight_ambient, Ulight_diffuse, Ulight_specular,
  Umaterial_ambient, Umaterial_diffuse, Umaterial_specular, Umaterial_shininess,
  Ucolor,Unormalmap,Uheight,Ugloss,
  Uprojection,Umodel,Uview,Unormal,
  UscaleBias;
 GLint posAttrib,texAttrib;
 gCamera camera;
 ImmediateLight light;
 GLMaterial material;
 float scaleBias[2];
 Matrix4 projectionMatrix;
 Matrix4 modelMatrix;
 Matrix3 normalMatrix;
 Quaternionf q;
 float rotationDegrees;
 float rotationSpeed;
 Vector3 rotationAxis;
 Vector3 up, target;
 void Perspective(float fovx, float aspect, float znear, float zfar) {
 	float e = 1.0f / tanf(dhpowareMath::degreesToRadians(fovx) / 2.0f);
 	float aspectInv = 1.0f / aspect;
 	float fovy = 2.0f * atanf(aspectInv / e);
 	float xScale = 1.0f / tanf(0.5f * fovy);
 	float yScale = xScale / aspectInv;
 	projectionMatrix[0][0] = xScale; 	projectionMatrix[0][1] = 0.0f;   	projectionMatrix[0][2] = 0.0f;                                  	 projectionMatrix[0][3] = 0.0f;
 	projectionMatrix[1][0] = 0.0f; 	  projectionMatrix[1][1] = yScale; 	projectionMatrix[1][2] = 0.0f;                                  	 projectionMatrix[1][3] = 0.0f;
 	projectionMatrix[2][0] = 0.0f; 	  projectionMatrix[2][1] = 0.0f;   	projectionMatrix[2][2] = (zfar + znear) / (znear - zfar); 	       projectionMatrix[2][3] = -1.0f;
 	projectionMatrix[3][0] = 0.0f; 	  projectionMatrix[3][1] = 0.0f;   	projectionMatrix[3][2] = (2.0f * zfar * znear) / (znear - zfar);  projectionMatrix[3][3] = 0.0f;
 }
 void Init() {
  color=gloss=normal=height=0;
  scaleBias[0]=0.04f;
  scaleBias[1]=-0.03f;
  up.set(0.0f,1.0f,0.0f);
  target.set(0.0f,0.0f,0.0f);
  rotationAxis.set(0.0f,0.0f,0.0f);
  rotationDegrees=0.0f;
  camera.Setup(display.w,display.h,&(camera.origin));
  light.direction.Set(0.0f, 0.0f, -1.0f, 0.0f);
  light.ambient.Set(1.0f, 1.0f, 1.0f, 1.0f);
  light.diffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular.Set(1.0f, 1.0f, 1.0f, 1.0f);
  material.ambient.Set(0.3f, 0.3f, 0.3f, 1.0f);
  material.diffuse.Set(0.7f, 0.7f, 0.7f, 1.0f);
  material.specular.Set(0.5f, 0.5f, 0.5f, 1.0f);
  material.shininess[0]=60.0f;
  OUTPUT("ParallaxGlossProtoShader::Loading\n");
  string vert=
   string("#version 130\n")+
   string("precision highp float;\n")+
   string("uniform struct DirectionalLight {\n")+
   string("	vec3 direction;\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("} DirectionalLight1;\n")+
   string("uniform struct Material {\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("	float shininess;\n")+
   string("} Material1;\n")+
   string("uniform mat4 ProjectionMatrix;\n")+
   string("uniform mat4 ViewMatrix;\n")+
   string("uniform mat4 ModelMatrix;\n")+
   string("uniform mat3 NormalMatrix;\n")+
   string("in vec3 VertexPosition;\n")+
   string("in vec3 VertexNormal;\n")+
   string("in vec2 VertexTexCoord;\n")+
   string("in vec4 VertexTangent;\n")+
   string("out vec3 LightDir;\n")+
   string("out vec3 ViewDir;\n")+
   string("out vec2 TexCoord;\n")+
   string("void main() {\n")+
   string("	vec3 n = normalize(NormalMatrix * VertexNormal);\n")+
   string(" vec3 t = normalize(NormalMatrix * VertexTangent.xyz);\n")+
   string(" vec3 b = cross(n, t) * VertexTangent.w;\n")+
   string(" mat3 tbnMatrix = mat3(t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z);\n")+
   string("	mat4 modelViewMatrix = ViewMatrix * ModelMatrix;\n")+
   string(" mat4 mvpMatrix = ProjectionMatrix * modelViewMatrix;\n")+
   string(" vec4 pos = vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);\n")+
   string(" gl_Position = mvpMatrix * pos;\n")+
   string(" pos = modelViewMatrix * pos;\n")+
   string("	ViewDir = -(pos.xyz / pos.w);\n")+
   string("	ViewDir = tbnMatrix * ViewDir;\n")+
   string("	LightDir = vec3(ViewMatrix * vec4(-DirectionalLight1.direction, 0.0f));\n")+
   string("	LightDir = tbnMatrix * LightDir;\n")+
   string(" TexCoord = VertexTexCoord;\n")+
   string("}\n")
  ;
  string frag=
   string("#version 130\n")+
   string("precision highp float;\n")+
   string("uniform struct DirectionalLight {\n")+
   string("	vec3 direction;\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("} DirectionalLight1;\n")+
   string("uniform struct Material {\n")+
   string("	vec4 ambient;\n")+
   string("	vec4 diffuse;\n")+
   string("	vec4 specular;\n")+
   string("	float shininess;\n")+
   string("} Material1;\n")+
   string("uniform sampler2D ColorMap;\n")+
   string("uniform sampler2D GlossMap;\n")+
   string("uniform sampler2D NormalMap;\n")+
   string("uniform sampler2D HeightMap;\n")+
   string("uniform vec2 ParallaxScaleBias;\n")+
   string("in vec3 LightDir;\n")+
   string("in vec3 ViewDir;\n")+
   string("in vec2 TexCoord;\n")+
   string("out vec4 FragColor;\n")+
   string("void main() {\n")+
   string("	vec3 L = normalize(LightDir);\n")+
   string("	vec3 V = normalize(ViewDir);\n")+
   string("	float height = texture(HeightMap, TexCoord).r;\n")+
   string("	height = height * ParallaxScaleBias.x + ParallaxScaleBias.y;\n")+
   string("	vec2 texCoord = TexCoord + (height * V.xy);\n")+
   string("	vec3 N = normalize(texture(NormalMap, texCoord.st).xyz * 2.0 - 1.0);\n")+
   string("	vec3 R = normalize(-reflect(L, N));\n")+
   string("	float nDotL = max(0.0, dot(N, L));\n")+
   string("	float rDotV = max(0.0, dot(R, V));\n")+
   string("	vec4 ambient = DirectionalLight1.ambient * Material1.ambient;\n")+
   string("	vec4 diffuse = DirectionalLight1.diffuse * Material1.diffuse * nDotL;\n")+
   string("	vec4 specular = DirectionalLight1.specular * Material1.specular * pow(rDotV, Material1.shininess);\n")+
   string("	vec4 texel = texture(ColorMap, texCoord);\n")+
   string("	float gloss = texture(GlossMap, texCoord).r;\n")+
   string(" FragColor = (ambient + diffuse + (gloss * specular)) * texel;\n")+
   string("}\n")
  ;
  Perspective(rad2deg((float)camera.fov), (float)(display.wd / display.hd), 0.1f, 20000.0f);
 	camera.LookAt(Vector3(camera.cPosition.x, camera.cPosition.y, camera.cPosition.z), target, up);
  Strings binds; binds.Add("FragColor");
  loadShaders(&binds,vert.c_str(),frag.c_str());
  InitUniforms();
 }
 void InitUniforms() {
  Ulight_direction = glGetUniformLocation(program,"DirectionalLight1.direction");
  Ulight_ambient = glGetUniformLocation(program,"DirectionalLight1.ambient");
  Ulight_diffuse = glGetUniformLocation(program,"DirectionalLight1.diffuse");
  Ulight_specular = glGetUniformLocation(program,"DirectionalLight1.specular");
  Umaterial_ambient = glGetUniformLocation(program,"Material1.ambient");
  Umaterial_diffuse = glGetUniformLocation(program,"Material1.diffuse");
  Umaterial_specular = glGetUniformLocation(program,"Material1.specular");
  Umaterial_shininess = glGetUniformLocation(program,"Material1.shininess");
  Ucolor = glGetUniformLocation(program,"ColorMap");
  Unormalmap = glGetUniformLocation(program,"NormalMap");
  Uheight = glGetUniformLocation(program,"HeightMap");
  Ugloss = glGetUniformLocation(program,"GlossMap");
  Uprojection = glGetUniformLocation(program,"ProjectionMatrix");
  Umodel = glGetUniformLocation(program,"ModelMatrix");
  Uview = glGetUniformLocation(program,"ViewMatrix");
  Unormal = glGetUniformLocation(program,"NormalMatrix");
  UscaleBias = glGetUniformLocation(program,"ParallaxScaleBias");
 }
 void UpdateUniforms() {
  glUseProgram(program);
  Perspective(rad2deg((float)camera.fov), (float)(display.wd / display.hd), 0.1f, 20000.0f);
 	camera.LookAt(Vector3(camera.cPosition.x, camera.cPosition.y, camera.cPosition.z), target, up);
  glUniform3fv(Ulight_direction, 1, &light.direction.coord[0] );
  glUniform4fv(Ulight_ambient, 1, &light.ambient.coord[0] );
  glUniform4fv(Ulight_diffuse, 1, &light.diffuse.coord[0] );
  glUniform4fv(Ulight_specular, 1, &light.specular.coord[0] );
  glUniform4fv(Umaterial_ambient, 1, &material.ambient.coord[0] );
  glUniform4fv(Umaterial_diffuse, 1, &material.ambient.coord[0] );
  glUniform4fv(Umaterial_specular, 1, &material.ambient.coord[0] );
  glUniform1f(Umaterial_ambient, material.shininess[0] );
  glUniform1i(Ucolor, 0);   glActiveTexture(GL_TEXTURE0);	 glBindTexture(GL_TEXTURE_2D, *color);
  glUniform1i(Unormalmap, 0);  glActiveTexture(GL_TEXTURE1);	 glBindTexture(GL_TEXTURE_2D, *normal);
  glUniform1i(Uheight, 0);  glActiveTexture(GL_TEXTURE2);	 glBindTexture(GL_TEXTURE_2D, *height);
  glUniform1i(Ugloss, 0);   glActiveTexture(GL_TEXTURE3);	 glBindTexture(GL_TEXTURE_2D, *gloss);
 	q.toAxisAngle(rotationAxis, rotationDegrees);
  Matrix4 modelMatrix = Matrix4::createRotate(rotationAxis, rotationDegrees);
//  glUniform3f(glGetUniformLocation(program->handle,"RotationAngles"), rotation->x, rotation->y, rotation->z );
//  glUniform3f(glGetUniformLocation(program->handle,"VerticesOffset"), position->x, position->y, position->z );
//  glUniform3f(glGetUniformLocation(program->handle,"CumulativeOffset"), offset->x, offset->y, offset->z );
//  glUniform3f(glGetUniformLocation(program->handle,"VerticesScale"), scale->x, scale->y, scale->z );
  Matrix4 *temp=camera.viewMatrix.asMatrix4();
 	Matrix4 modelViewMatrix = modelMatrix * (*temp);
  glUniformMatrix4fv(Uprojection, 1, GL_FALSE, &projectionMatrix[0][0]);
  glUniformMatrix4fv(Umodel, 1, GL_FALSE, &modelMatrix[0][0] );
	 glUniformMatrix3fv(Unormalmap, 1, GL_FALSE, &normalMatrix[0][0] );
 	normalMatrix[0][0] = modelViewMatrix[0][0]; 	normalMatrix[0][1] = modelViewMatrix[0][1]; 	normalMatrix[0][2] = modelViewMatrix[0][2]; 
 	normalMatrix[1][0] = modelViewMatrix[1][0]; 	normalMatrix[1][1] = modelViewMatrix[1][1]; 	normalMatrix[1][2] = modelViewMatrix[1][2]; 
 	normalMatrix[2][0] = modelViewMatrix[2][0]; 	normalMatrix[2][1] = modelViewMatrix[2][1]; 	normalMatrix[2][2] = modelViewMatrix[2][2]; 
 	normalMatrix = normalMatrix.inverse().transpose();
	 glUniformMatrix4fv(Uview, 1, GL_FALSE, &(*temp)[0][0] ); // needs type optimization on this line

  delete temp;

	 glUniform2fv(UscaleBias, 1, scaleBias);
 }

 void Disable() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
 }
};

extern ParallaxGlossProtoShader parallaxglossprotoShader;