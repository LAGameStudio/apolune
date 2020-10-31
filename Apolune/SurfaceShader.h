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
#include "Art.h"
#include "GLSL2Shader.h"

#if defined(NEVER)
// LGPL http://code.google.com/p/h3dsample/source/browse/trunk/Applications/Resources/Renderers/Shaders/?r=285

class PlasmaPixelShader {
 [vert]
 // Vertex -> Vertex Shader
attribute vec3 inTangent;
// for test
attribute vec3 inBinormal;
attribute vec3 inUserArray;

// Vertex Shader -> Fragment Shader
varying vec3 objPosition;
varying vec3 ecPosition;
varying vec3 eyePosition;
varying vec3 normal;
varying vec2 texCoord0;
varying vec3 testVector;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	texCoord0 = gl_MultiTexCoord0.xy;

	objPosition = vec3( gl_Vertex );
	ecPosition = vec3( gl_ModelViewMatrix * gl_Vertex );
	eyePosition = -ecPosition;
	
	testVector = inTangent;

	gl_Position = ftransform();
}
[frag]
// url : http://www.renderman.org/RMR/Shaders/RudyCShaders/index.html

varying vec3 objPosition;
varying vec3 ecPosition;
varying vec3 eyePosition;
varying vec3 normal;
varying vec2 texCoord0;
varying vec3 testVector;

uniform sampler2D DiffuseTextureID;
//! 랜덤 값을 위한 텍스쳐
uniform sampler1D RandomDataTextureID;
//! 수열 텍스쳐 (0~255 의 중복되지 숫자의 순서가 섞여서 저장되어 있다.)
uniform sampler1D PermutationTextureID;

//! Noise 의 크기를 256 으로 한다. 원래는 float 형이어야함
const float randomLength = 256.0;
const float MINFILTWIDTH = 1.0e-6;
const float PI = 3.141592;

//! 애니메이션을 위한 FRAME 변수
uniform float FRAME;

uniform float textFreq;
uniform float noiFreq; // 12
uniform float turbPower; // 4.0
uniform float additive;
uniform float rimEdge;

//float textFreq = 0.025;
//float noiFreq = 12.0; // 12
//float turbPower = 2.0; // 4.0
//float additive = 1.0;
//float rimEdge = 0.8;

//! 선형 보간(linear interpolation)
float lerp( float t, float x0, float x1 )
{
	return x0 + t * ( x1 - x0 );
}

//! 부드러운 보간(smooth interpolation/curve)
float s_curve( float t )
{
	return t * t * ( 3.0 - 2.0 * t );
	//return 6.0 * pow( t, 5.0 ) - 15.0 * pow( t, 4.0 ) + 10.0 * pow( t, 3.0 );
}

//! index 에 해당하는 수열 값을 수열 텍스쳐에서 얻어옴
float GetIndex( float index )
{
	//return mod( index, randomLength );
	//return texture1D( PermutationTextureID, index ).r;
	return texture1D( PermutationTextureID, mod( index, randomLength )/randomLength ).r * randomLength;
}

//! 노이즈 데이터를 얻어옴. 반환 값은 vec3 값이 된다.
vec3 GetNoiseData( float index )
{
	//return texture1D( RandomDataTextureID, index ).rgb;
	//return texture1D( RandomDataTextureID, mod( index, randomLength ) ).rgb;
	return 2.0 * texture1D( RandomDataTextureID, mod( index, randomLength )/randomLength ).rgb - vec3( 1.0 );
}

//! Spline 커브
//! 참고 : Computer Graphics : Principles and Practice
vec3 spline( float value,
		vec3 fn1, vec3 fn2, vec3 fn3, vec3 fn4, vec3 fn5, vec3 fn6, vec3 fn7, vec3 fn8, vec3 fn9, vec3 fn10, vec3 fn11 )
{
	const float CR00 = -0.5;
	const float CR01 = 1.5;
	const float CR02 = -1.5;
	const float CR03 = 0.5;
	const float CR10 = 1.0;
	const float CR11 = -2.5;
	const float CR12 = 2.0;
	const float CR13 = -0.5;
	const float CR20 = -0.5;
	const float CR21 = 0.0;
	const float CR22 = 0.5;
	const float CR23 = 0.0;
	const float CR30 = 0.0;
	const float CR31 = 1.0;
	const float CR32 = 0.0;
	const float CR33 = 0.0;
	
	vec3 c0, c1, c2, c3;
	
	int num = 11;
	value = clamp( value, 0.0, 1.0 ) * float(num);
	int span = int(value);

	if( span >= num - 3 )
		span = num - 3;
	value -= float(span);
	//step = span;

	if( span == 0 )
	{
		c3 = CR00 * fn1 + CR01 * fn2 + CR02 * fn3 + CR03 * fn4;
		c2 = CR10 * fn1 + CR11 * fn2 + CR12 * fn3 + CR13 * fn4;
		c1 = CR20 * fn1 + CR21 * fn2 + CR22 * fn3 + CR23 * fn4;
		c0 = CR30 * fn1 + CR31 * fn2 + CR32 * fn3 + CR33 * fn4;
	}
	if( span == 1 )
	{
		c3 = CR00 * fn2 + CR01 * fn3 + CR02 * fn4 + CR03 * fn5;
		c2 = CR10 * fn2 + CR11 * fn3 + CR12 * fn4 + CR13 * fn5;
		c1 = CR20 * fn2 + CR21 * fn3 + CR22 * fn4 + CR23 * fn5;
		c0 = CR30 * fn2 + CR31 * fn3 + CR32 * fn4 + CR33 * fn5;
	}
	if( span == 2 )
	{
		c3 = CR00 * fn3 + CR01 * fn4 + CR02 * fn5 + CR03 * fn6;
		c2 = CR10 * fn3 + CR11 * fn4 + CR12 * fn5 + CR13 * fn6;
		c1 = CR20 * fn3 + CR21 * fn4 + CR22 * fn5 + CR23 * fn6;
		c0 = CR30 * fn3 + CR31 * fn4 + CR32 * fn5 + CR33 * fn6;
	}
	if( span == 3 )
	{
		c3 = CR00 * fn4 + CR01 * fn5 + CR02 * fn6 + CR03 * fn7;
		c2 = CR10 * fn4 + CR11 * fn5 + CR12 * fn6 + CR13 * fn7;
		c1 = CR20 * fn4 + CR21 * fn5 + CR22 * fn6 + CR23 * fn7;
		c0 = CR30 * fn4 + CR31 * fn5 + CR32 * fn6 + CR33 * fn7;
	}
	if( span == 4 )
	{
		c3 = CR00 * fn5 + CR01 * fn6 + CR02 * fn7 + CR03 * fn8;
		c2 = CR10 * fn5 + CR11 * fn6 + CR12 * fn7 + CR13 * fn8;
		c1 = CR20 * fn5 + CR21 * fn6 + CR22 * fn7 + CR23 * fn8;
		c0 = CR30 * fn5 + CR31 * fn6 + CR32 * fn7 + CR33 * fn8;
	}
	if( span == 5 )
	{
		c3 = CR00 * fn6 + CR01 * fn7 + CR02 * fn8 + CR03 * fn9;
		c2 = CR10 * fn6 + CR11 * fn7 + CR12 * fn8 + CR13 * fn9;
		c1 = CR20 * fn6 + CR21 * fn7 + CR22 * fn8 + CR23 * fn9;
		c0 = CR30 * fn6 + CR31 * fn7 + CR32 * fn8 + CR33 * fn9;
	}
	if( span == 6 )
	{
		c3 = CR00 * fn7 + CR01 * fn8 + CR02 * fn9 + CR03 * fn10;
		c2 = CR10 * fn7 + CR11 * fn8 + CR12 * fn9 + CR13 * fn10;
		c1 = CR20 * fn7 + CR21 * fn8 + CR22 * fn9 + CR23 * fn10;
		c0 = CR30 * fn7 + CR31 * fn8 + CR32 * fn9 + CR33 * fn10;
	}
	if( span == 7 )
	{
		c3 = CR00 * fn8 + CR01 * fn9 + CR02 * fn10 + CR03 * fn11;
		c2 = CR10 * fn8 + CR11 * fn9 + CR12 * fn10 + CR13 * fn11;
		c1 = CR20 * fn8 + CR21 * fn9 + CR22 * fn10 + CR23 * fn11;
		c0 = CR30 * fn8 + CR31 * fn9 + CR32 * fn10 + CR33 * fn11;
	}
	if( span == 8 )
	{
		c3 = CR00 * fn9 + CR01 * fn10 + CR02 * fn11 + CR03 * fn11;
		c2 = CR10 * fn9 + CR11 * fn10 + CR12 * fn11 + CR13 * fn11;
		c1 = CR20 * fn9 + CR21 * fn10 + CR22 * fn11 + CR23 * fn11;
		c0 = CR30 * fn9 + CR31 * fn10 + CR32 * fn11 + CR33 * fn11;
	}
	return ((c3*value + c2)*value + c1)*value + c0;
}

float PerlinNoise( vec3 p )
{
	float rx0 = p.x - floor(p.x);
	float ry0 = p.y - floor(p.y);
	float rz0 = p.z - floor(p.z);

//	float rx1 = 1.0 - rx0;
//	float ry1 = 1.0 - ry0;
//	float rz1 = 1.0 - rz0;

	float rx1 = rx0 - 1.0;
	float ry1 = ry0 - 1.0;
	float rz1 = rz0 - 1.0;

	float bx0, bx1, by0, by1, bz0, bz1;
	float i, j, b00, b10, b01, b11;

	bx0 = mod( floor(p.x), randomLength );
	by0 = mod( floor(p.y), randomLength );
	bz0 = mod( floor(p.z), randomLength );

	bx1 = mod( bx0 + 1.0, randomLength );
	by1 = mod( by0 + 1.0, randomLength );
	bz1 = mod( bz0 + 1.0, randomLength );

	i = GetIndex( bx0 );
	j = GetIndex( bx1 );
	b00 = GetIndex( i + by0 );
	b10 = GetIndex( j + by0 );
	b01 = GetIndex( i + by1 );
	b11 = GetIndex( j + by1 );

	float sx = s_curve( rx0 );
	float sy = s_curve( ry0 );
	float sz = s_curve( rz0 );

	float a, b, c, d, u, v, result;
	u = dot( GetNoiseData( b00 + bz0 ), vec3( rx0, ry0, rz0 ) );
	v = dot( GetNoiseData( b10 + bz0 ), vec3( rx1, ry0, rz0 ) );
	a = lerp( sx, u, v );

	u = dot( GetNoiseData( b01 + bz0 ), vec3( rx0, ry1, rz0 ) );
	v = dot( GetNoiseData( b11 + bz0 ), vec3( rx1, ry1, rz0 ) );
	b = lerp( sx, u, v );

	c = lerp( sy, a, b );

	u = dot( GetNoiseData( b00 + bz1 ), vec3( rx0, ry0, rz1 ) );
	v = dot( GetNoiseData( b10 + bz1 ), vec3( rx1, ry0, rz1 ) );
	a = lerp( sx, u, v );

	u = dot( GetNoiseData( b01 + bz1 ), vec3( rx0, ry1, rz1 ) );
	v = dot( GetNoiseData( b11 + bz1 ), vec3( rx1, ry1, rz1 ) );
	b = lerp( sx, u, v );

	d = lerp( sy, a, b );

	result = lerp( sz, c, d ) * 1.5;
	result = 2.0 * result - 1.0;
	return result;
}

// returns -1.0 to 1.0?
float filteredsnoise( vec3 p, float width )
{
	float result = 0.0;
	result = PerlinNoise( p );
	return result * 0.25;
	//return PerlinNoise( p ) * ( 1.0 - smoothstep( 0.2, 0.75, width ) );
}

void main()
{
	/*float textFreq = 0.025;
	float noiFreq = 12.0; // 12
	float turbPower = 2.0; // 4.0
	float additive = 1.0;
	float rimEdge = 0.8;*/
	
	vec4 diffuseTexture = texture2D( DiffuseTextureID, texCoord0 ).rgba;

	vec4 ambient = vec4( 0.0, 0.0, 0.0, 1.0 );
	vec4 diffuse = vec4( 1.0, 1.0, 1.0, 1.0 );
	vec4 specular = vec4( 0.0, 0.0, 0.0, 1.0 );
	// Light
	vec4 materialColor = 
			ambient * gl_FrontMaterial.ambient + 
			diffuse * diffuseTexture + // replace
			specular * gl_FrontMaterial.specular;
	//materialColor = vec4( 0.9, 0.9, 1.0, 1.0 );
	//materialColor = vec4( 0.7, 0.8, 1.0, 1.0 );
	materialColor = vec4( 1.0, 0.3, 0.3, 1.0 );

	vec3 PP = objPosition;
	PP += vec3( 10.0 );
	PP = PP * textFreq;
	//PP = abs( PP );
	//p = vec3( p.x );
	float PPwidth = MINFILTWIDTH;
	//float PPwidth = 0.000001;
	float turb = 0.0;
	float freq = noiFreq;

// Layer Alpha
	//turb = abs( filteredsnoise( intensity, PPwidth ) );
	//turb = abs( filteredsnoise( PP + vec3( freq * PI + FRAME ), PPwidth ) );
	turb += abs( filteredsnoise( PP + freq * PI + vec3( FRAME ), PPwidth  ) );
	turb = pow( 1.0 - turb, turbPower );
	float alpha = turb;

// Layer Color
//	vec3 dar = materialColor.rgb * 0.15;
//	vec3 middar = materialColor.rgb * 0.25;
//	vec3 mid = materialColor.rgb * 0.5;
//	vec3 midhi = materialColor.rgb * 0.75;
//	vec3 hi = materialColor.rgb * 0.9;

	vec3 dar = materialColor.rgb * 0.15;
	vec3 middar = materialColor.rgb * 0.25;
	vec3 mid = materialColor.rgb * 0.5;
	vec3 midhi = materialColor.rgb * 0.75;
	vec3 hi = materialColor.rgb * 0.9;

	float noi = 0.0;
	//noi = PerlinNoise( PP * PI  + vec3( FRAME, 0.0, 0.0 ) );
	noi = PerlinNoise( PP * PI  + vec3( FRAME ) );
	noi = pow( noi, 2.0 );
	vec3 LayerColor = vec3( noi );
	// Spline
	//LayerColor = smoothstep( vec3( 0.0 ), vec3( 1.0, 0.0, 0.0 ), noi );
	//LayerColor = materialColor;
	
	LayerColor = spline( noi, dar, dar, middar, mid, midhi, hi, 
		midhi, mid, middar, dar, dar );
	
	//alpha += 0.0;
	//alpha *= pow( sqrt( dot( LayerColor, LayerColor ) ), 2.0 );
	alpha *= dot( LayerColor, LayerColor );
	//alpha *= sqrt( dot( LayerColor, LayerColor ) );
	
	//alpha = s_curve( alpha );
	
	//alpha = 1.0 - pow( noi, 2.0 );

// Composite Layers
	vec3 FinalColor;
	//FinalColor = mix( materialColor.rgb, LayerColor, alpha );
	//FinalColor = mix( materialColor.rgb, LayerColor, alpha );
	FinalColor = LayerColor;
	//FinalColor += (1.0 - alpha) * materialColor;

// Alpha w.r.t. Light
	vec3 LightDirection = normalize( vec3( gl_LightSource[0].position ) - ecPosition );
	//vec3 LightDirection = ecPosition;
	float NdotL = clamp( -dot( normal, LightDirection ), 0.0, 1.0 );
	//NdotL = 1.0;
	float alpha1 = 1.0 - smoothstep( 0.0, rimEdge, NdotL );
	//alpha1 = dot( normal, LightDirection );
	//if( dot( normal, LightDirection ) > 0 )
		alpha *= alpha1 + (alpha/1.5);
	//else
	//	alpha *= alpha1 + (alpha/5.5);
	alpha = (alpha/1.5);
	//alpha = 1.0;

// ?? Composite? 정체가 머냐!
	FinalColor = mix( FinalColor, LayerColor, alpha );
	FinalColor.r += alpha * additive;
	FinalColor.g += alpha * additive;
	FinalColor.b += alpha * additive;

	//alpha = 1.0;
	//gl_FragColor.a = alpha; // * input_alpha?
	gl_FragColor.a = pow( alpha, 1.0 );
	gl_FragColor.rgb = FinalColor.rgb + alpha;
	//gl_FragColor.rgb = FinalColor.rgb;

	// 2010. 4. 14. OK (집에서는 안되는듯?)
		// Diffuse Texture
		//LayerColor.rgb = texture2D( DiffuseTextureID, texCoord0.st ).rgb;
		// Noise Data Test OK
		//LayerColor.rgb = GetNoiseData( ecPosition.x ) + 1.0;
		// Permutation Data Test OK
//		LayerColor.r = GetIndex( ecPosition.x )/randomLength;
//		LayerColor.g = GetIndex( ecPosition.y )/randomLength;
//		LayerColor.b = GetIndex( ecPosition.z )/randomLength;

	//LayerColor.rgb = vec3( mod( texCoord0, 1.0 ), 0.0 );
	//LayerColor.rgb = testVector;
	
	// Test
	gl_FragColor.rgb = LayerColor; 
	//gl_FragColor.r = gl_FragDepth;
	//gl_FragColor.rgb = vec3( gl_FragDepth );
	//gl_FragColor.rgb = vec4( texture2D( DiffuseTextureID, texCoord0 ).rgb, 1.0 );
//	gl_FragColor.g = filteredsnoise( ecPosition, 10.0 );
	//gl_FragColor.b = 0.5;
//	gl_FragColor.g = additive;
	//gl_FragColor.a = 0.1;
	//gl_FragColor.b = additive;
}
};

class BumpShader {
 [vert]
/* Vertex shader */
attribute vec3 inTangent;

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;

varying vec3 LightDirectionTS;
varying vec3 EyeDirectionTS; // 

void main()
{
	EyeDirectionTS = normalize( vec3( gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space
	
	// Eye to Tangent Matrix
	vec3 normal, tangent, binormal; // Eye Space
	Normal = normal = normalize( gl_NormalMatrix * gl_Normal );
	//Normal = normal = normalize( gl_NormalMatrix * vec3( 0.0, 0.0, 1.0 ) );
	Tangent = tangent = normalize( gl_NormalMatrix * inTangent );
	//Tangent = tangent = normalize( gl_NormalMatrix * vec3( 1.0, 0.0, 0.0 ) );
	Binormal = binormal = cross( normal, tangent );
	
	vec3 v;
	LightDirectionTS = normalize( vec3( gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space
	v.x = dot( LightDirectionTS, tangent );
	v.y = dot( LightDirectionTS, binormal );
	v.z = dot( LightDirectionTS, normal );
	LightDirectionTS = normalize( v ); // Tangent Space
	
	EyeDirectionTS = -EyeDirectionTS;
	v.x = dot( EyeDirectionTS, tangent );
	v.y = dot( EyeDirectionTS, binormal );
	v.z = dot( EyeDirectionTS, normal );
	EyeDirectionTS = normalize( v ); // Tangent Space
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
[frag]
/* Fragment shader */
uniform sampler2D tex0; // ambient and diffuse map
uniform sampler2D tex1; // normal map

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;

varying vec3 LightDirectionTS;
varying vec3 EyeDirectionTS;

vec3 GetNormal( vec2 coord )
{
	vec3 ret = texture2D( tex1, coord ).xyz;
	ret.xy = ret.xy * 2.0 - 1.0;
	// 임시
	ret.y *= -1.0;
	return normalize( ret );
}

void main()
{
	vec4 colorMap = vec4( texture2D( tex0, gl_TexCoord[0].st ).xyz, 1.0 );
	vec3 normalMap = GetNormal( gl_TexCoord[0].st );
	
	vec3 lightDirection = normalize( LightDirectionTS );
	
	// Ambient
	vec4 ambient_color = (gl_FrontLightModelProduct.sceneColor) + 
		(colorMap * gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	
	// Diffuse
	float lambertTerm = max( dot( normalMap, lightDirection ), 0.0 );
	vec4 diffuse_color = colorMap * gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
	
	// Specular
	//vec3 reflectDirection = normalize( reflect( normalize( -LightDirectionTS ), normalMap ) ); 
	vec3 reflectDirection = 2.0 * dot( normalMap, lightDirection ) * normalMap - lightDirection; // in Tangen Space
	float specular = pow( max( dot(reflectDirection, EyeDirectionTS), 0.0 ), gl_FrontMaterial.shininess );

	vec4 specular_color = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;
	if( lambertTerm <= 0.0 )
	{
		specular_color = vec4( 0.0, 0.0, 0.0, 0.0 );
	}

	gl_FragColor = vec4( ambient_color.xyz + diffuse_color.xyz + specular_color.xyz, 1.0 );
}
};

void main()
{
	EyeDirectionTS = normalize( vec3( gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space
	
	// Eye to Tangent Matrix
	vec3 normal, tangent, binormal; // Eye Space
	Normal = normal = normalize( gl_NormalMatrix * gl_Normal );
	//Normal = normal = normalize( gl_NormalMatrix * vec3( 0.0, 0.0, 1.0 ) );
	Tangent = tangent = normalize( gl_NormalMatrix * inTangent );
	//Tangent = tangent = normalize( gl_NormalMatrix * vec3( 1.0, 0.0, 0.0 ) );
	Binormal = binormal = cross( normal, tangent );
	
	vec3 v;
	LightDirectionTS = normalize( vec3( gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space
	v.x = dot( LightDirectionTS, tangent );
	v.y = dot( LightDirectionTS, binormal );
	v.z = dot( LightDirectionTS, normal );
	LightDirectionTS = normalize( v ); // Tangent Space
	
	EyeDirectionTS = -EyeDirectionTS;
	v.x = dot( EyeDirectionTS, tangent );
	v.y = dot( EyeDirectionTS, binormal );
	v.z = dot( EyeDirectionTS, normal );
	EyeDirectionTS = normalize( v ); // Tangent Space
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
[frag]

};


class NonGDam {
 /* Vertex shader */
varying vec3 Normal;

varying vec3 LightDirection;
varying vec3 EyeDirection;

void main()
{
	EyeDirection = normalize( vec3( gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space (Eye to vertex)
	
	Normal = normalize( gl_NormalMatrix * gl_Normal );
	LightDirection = normalize( vec3( gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space
	EyeDirection = -EyeDirection; 
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
[frag]
/* Fragment shader */
/* diffuse */
uniform sampler2D tex0;

varying vec3 Normal;

varying vec3 LightDirection;
varying vec3 EyeDirection;

void main()
{
	vec2 startCoord = gl_TexCoord[0].st;
	
	// Ambient
	//vec4 ambient_color = (gl_FrontLightModelProduct.sceneColor) + 
	//	(colorMap * gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	
	// Diffuse
	float lambertTerm = max( dot( Normal, LightDirection ), 0.0 );
	//vec4 diffuse_color = colorMap * gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
	vec4 diffuse_color = (texture2D( tex0, vec2( 1.0 - lambertTerm, 0.0 ) )) * gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
	
	// Specular
	vec3 reflectDirection = 2.0 * dot( Normal, LightDirection ) * Normal - LightDirection; // in Tangen Space

	float specular = pow( max( dot(reflectDirection, EyeDirection), 0.0 ), gl_FrontMaterial.shininess );
	vec4 specular_color = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;

	gl_FragColor = vec4( diffuse_color.xyz + specular_color.xyz, 1.0 );
	//gl_FragColor = vec4( ambient_color.xyz + diffuse_color.xyz + specular_color.xyz, 1.0 );
}
};

class DynamicParallaxOcclusion {
 [vert]
 /* Vertex shader */
uniform float inHeightMapRange;

attribute vec3 inTangent;

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;

varying vec3 LightDirectionTS;
varying vec3 EyeDirectionTS;

varying float Depth;

varying vec3 testVector;


void main()
{
	Depth = 30.0/150.0;
	EyeDirectionTS = normalize( vec3( gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space (Eye to vertex)
	
	// Eye to Tangent Space Matrix
	vec3 normal, tangent, binormal; // Eye Space
	Normal = normal = normalize( gl_NormalMatrix * gl_Normal );
	//Normal = normal = normalize( gl_NormalMatrix * vec3( 0.0, 0.0, 1.0 ) );
	Tangent = tangent = normalize( gl_NormalMatrix * inTangent );
	//Tangent = tangent = normalize( gl_NormalMatrix * vec3( 1.0, 0.0, 0.0 ) );
	Binormal = binormal = cross( normal, tangent );
	
	testVector = tangent;
	
	vec3 v;
	LightDirectionTS = normalize( vec3( gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex ) ); // Eye Space
	v.x = dot( LightDirectionTS, tangent );
	v.y = dot( LightDirectionTS, binormal );
	v.z = dot( LightDirectionTS, normal );
	LightDirectionTS = normalize( v ); // Tangent Space
	
	testVector = vec3( gl_LightSource[0].position - gl_ModelViewMatrix * gl_Vertex );
	
	EyeDirectionTS = -EyeDirectionTS; 
	v.x = dot( EyeDirectionTS, tangent );
	v.y = dot( EyeDirectionTS, binormal );
	v.z = dot( EyeDirectionTS, normal );
	EyeDirectionTS = normalize( v ); // Vertex to Eye in Tangent space
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	//gl_TexCoord[0] = clamp( gl_MultiTexCoord0, 0.0, 1.0 );
	gl_Position = ftransform();
}
[frag]
/* Fragment shader */

/* diffuse */
uniform sampler2D tex0;
/* normal map and height map */
uniform sampler2D tex1;

varying vec3 Normal;
varying vec3 Tangent;
varying vec3 Binormal;

varying vec3 LightDirectionTS;
varying vec3 EyeDirectionTS;

uniform float inHeightScale;
varying float Depth;

varying vec3 testVector;

vec3 GetNormal( vec2 coord )
{
	vec3 ret = texture2D( tex1, coord ).xyz;
	ret.xy = ret.xy * 2.0 - 1.0;
	ret.y *= -1.0;
	return normalize( ret );
}

float GetHeight( vec2 coord )
{
	return texture2D( tex1, coord ).w;
}

void main()
{
	vec3 LightDirection = normalize( LightDirectionTS );
	vec3 EyeDirection = normalize( EyeDirectionTS );
	vec3 EyeRay = -EyeDirection;
	vec2 startCoord = gl_TexCoord[0].st;
	vec2 newCoord = startCoord;

	// Common for Parallax
	// EyeRay.z < 0 
	vec2 ParallaxXY = ( EyeRay ).xy/-EyeRay.z * Depth;
	vec3 ParallaxXYZ = normalize( vec3( ParallaxXY, -1.0 ) );

	// Steep Parallax
	float Step = 0.01;
	vec2 dt = ParallaxXY * Step;
	float StepXYlength = length( dt );
	float Height = 1.0;
	float oldHeight = 1.0;
	vec2 Coord = startCoord;
	vec2 oldCoord = Coord;
	float HeightMap = GetHeight( Coord );
	float oldHeightMap = HeightMap;
	
	float minBound = 0.004;
	float maxBound = 0.03;

	while( HeightMap < Height )
	{
		Step = minBound +
			abs( dot( GetNormal( Coord.st ), ParallaxXYZ ) ) * (maxBound - minBound);
		oldHeightMap = HeightMap;
		oldHeight = Height;
		oldCoord = Coord;

		Height += ( ParallaxXYZ.z * Step );
		Coord += ParallaxXYZ.xy * Step;
		HeightMap = GetHeight( Coord );
	}
	//if( Coord.s <= 0.0 || Coord.t <= 0.0 || Coord.s >= 1.0 || Coord.t >= 1.0 )
	//	discard;
	Coord = (Coord + oldCoord) * 0.5;
	if( Height < 0.0 )
	{
		Coord = oldCoord;
		Height = 0.0;
	}
	newCoord = Coord;

	vec4 colorMap = vec4( texture2D( tex0, newCoord ).xyz, 1.0 );
	float heightMap = GetHeight( newCoord );
	vec3 normalMap = GetNormal( newCoord );
	
//	// Self Shadowing
//	Step = 0.1;
//	vec2 LightXY = ( LightDirection ).xy/LightDirection.z * Depth;
//	dt = LightXY * Step;
//	StepXYlength = length( dt );
//	//Height = 0.0;
//	//oldHeight = 0.0;
//	Coord = newCoord;
//	oldCoord = Coord;
//	HeightMap = GetHeight( Coord );
//	oldHeightMap = HeightMap;
//	Height = HeightMap + 0.001;
//	//Height += Step;
//	//Coord += dt;
//	
//	do
//	{
//		if( Height > 1.0 )
//			break;
//		oldHeightMap = HeightMap;
//		oldHeight = Height;
//		oldCoord = Coord;
//		Height += Step;
//		Coord += dt;
//		HeightMap = GetHeight( Coord );
//	} while( HeightMap < Height );
//	if( Height > 1.0 )
//	{
//		Coord = oldCoord;
//		Height = 1.0;
//	}
	
	// Shadow Off
	Height = 1.0;
	
	// Ambient
	vec4 ambient_color = (gl_FrontLightModelProduct.sceneColor) + 
		(colorMap * gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
	
	// Diffuse
	float lambertTerm = max( dot( normalMap, LightDirection ), 0.0 );
	vec4 diffuse_color = colorMap * gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
	
	// Specular
	vec3 reflectDirection = 2.0 * dot( normalMap, LightDirection ) * normalMap - LightDirection; // in Tangen Space

	float specular = pow( max( dot(reflectDirection, EyeDirectionTS), 0.0 ), gl_FrontMaterial.shininess );
	vec4 specular_color = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;

//	if( newCoord.s < 0.0 || newCoord.t < 0.0 || newCoord.s > 1.0 || newCoord.t > 1.0 )
//	{
//		discard;
//	}

	if( Height >= 1.0 )
		gl_FragColor = vec4( ambient_color.xyz + diffuse_color.xyz + specular_color.xyz, 1.0 );
	else
		gl_FragColor = vec4( ambient_color.xyz, 1.0 );
	
//	if( textureCoord.s > 1.0 )
//		textureCoord.s = 1.0;
	//gl_FragColor = vec4( abs( gl_TexCoord[0].st ), 0.0, 1.0 );
	//gl_FragColor = vec4( LightXY.xy, 0.0, 1.0 );
	//gl_FragColor = vec4( 1.0, ambient_color.yz, 1.0 );
	//gl_FragColor = vec4( GetNormal( gl_TexCoord[0].st ), 1.0 );
	//gl_FragColor = vec4( ParallaxXY, 0.0, 1.0 );
	//gl_FragColor = vec4( heightMap, 0.0, 0.0, 1.0 );
}

};



class NormalHeightShader {
 [vert]
 uniform sampler2D NormalTextureID;
uniform sampler2D HeightTextureID;

attribute vec3 inTangent;

varying vec3 normal, tangent, binormal;
varying vec3 lightDir, eyeVec;
varying vec2 texCoord0;

uniform float HeightScale;

void main()
{
	// To Eye Space
	normal = gl_NormalMatrix * gl_Normal;
	tangent = normalize( gl_NormalMatrix * inTangent );
	binormal = cross( normal, tangent );

	texCoord0 = gl_MultiTexCoord0.xy;

	// Eye Space
	float Height = texture2D( HeightTextureID, texCoord0 ).x * HeightScale;
	vec3 Displacement = gl_Normal * 0.0;

	// world space? ???
	vec3 vVertex = vec3( gl_ModelViewMatrix * (gl_Vertex + vec4( Displacement, 0.0) ) );

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeVec = -vVertex;

	gl_Position = gl_ProjectionMatrix  * gl_ModelViewMatrix * (gl_Vertex + vec4( Displacement, 0.0) );
	//gl_Position = ftransform();
	//gl_Position.xyz += Displacement;
}
[frag]

};

class NormalTangent {
[vert]
attribute vec3 inTangent;

varying vec3 normal, tangent, binormal;
varying vec3 lightDir, eyeVec;
varying vec2 texCoord0;

varying vec3 tgLightDir;

varying vec3 tempVector;

void main()
{	
	// Worldspace To Eye Space : Eye Space to Tangent Space Matrix
	normal = normalize( gl_NormalMatrix * gl_Normal );
	tangent = normalize( gl_NormalMatrix * inTangent );
	binormal = cross( normal, tangent );

	// Eye Space
	vec3 vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );

	// Eye Space
	lightDir = vec3(gl_LightSource[0].position) - vVertex;
	eyeVec = -vVertex;
	
	/*tgLightDir.x = dot( tangent, lightDir );
	tgLightDir.y = dot( binormal, lightDir );
	tgLightDir.z = dot( normal, lightDir );*/

	texCoord0 = gl_MultiTexCoord0.xy;

	gl_Position = ftransform();		
}
};


class DiffuseNormalShader {
 uniform sampler2D DiffuseTextureID;
uniform sampler2D NormalTextureID;

varying vec3 normal, tangent, binormal;
varying vec3 lightDir, eyeVec;
varying vec2 texCoord0;

vec3 Normal, Tangent, Binormal;

// Eye Space to Tangent Space
vec3 EyeToTangent( vec3 inVec )
{
	vec3 outVec;
	outVec.x = dot( Tangent, inVec );
	outVec.y = dot( Binormal, inVec );
	outVec.z = dot( Normal, inVec );
	return outVec;
}

vec3 GetNormal( vec2 coord )
{
	vec3 ret = texture2D( NormalTextureID, coord ).xyz;
	ret.xy = ret.xy * 2.0 - 1.0;
	// 임시
	//ret.y *= -1.0;
	return normalize( ret );
}

void main (void)
{
	Normal = normalize( normal );
	Tangent = normalize( tangent );
	Binormal = normalize( binormal );
	
	vec4 diffuseMap = texture2D( DiffuseTextureID, texCoord0 );
	
	// Ambient
	vec4 AmbientColor = 
		(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
			 (diffuseMap * gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

	//vec3 N = normalize(normal);
	vec3 N = EyeToTangent( GetNormal( texCoord0 ) );
	vec3 L = normalize( EyeToTangent( lightDir ) );

	// Diffuse
	vec4 DiffuseColor = 
			gl_LightSource[0].diffuse * 
			gl_FrontMaterial.diffuse *
			diffuseMap * 
			max( dot(N,L), 0.0 );

	// Specular
	vec3 E = normalize( EyeToTangent( eyeVec ) );
	vec3 R = reflect(-L, N);
	float specular = pow( max(dot(R, E), 0.0), 
	                 gl_FrontMaterial.shininess );
	vec4 SpecularColor = gl_LightSource[0].specular * 
	               gl_FrontMaterial.specular * 
				   specular;
	
	vec4 final_color = AmbientColor + DiffuseColor + SpecularColor;

	//gl_FragData[0] = vec4( normalize(L), 1.0 );
	//gl_FragData[0] = vec4( diffuseMap.rgb, 1.0 );
	
	//gl_FragData[0] = vec4( vec3( normalize(N).length() * 0.5 ), 1.0 );
	//gl_FragData[0] = vec4( texCoord0, 0.0, 1.0 );
	//gl_FragData[0] = vec4( GetNormal( texCoord0 ), 1.0 );
	//gl_FragData[0] = vec4( N, 1.0 );
	
	gl_FragData[0] = vec4( final_color.rgb, 1.0 );

	gl_FragData[1] = vec4( 0.0, 0.0, 0.0, 1.0 );
	//gl_FragData[1] = vec4( clamp( final_color.rgb - 0.8, 0.0, 1.0 )*5.0, 1.0 );
}

};
#endif



class DiffuseSpecularNormalShader : public GLSLShader {
public:
 void Init() {
  string vert=
   string("#version 120\n")+
   string("attribute vec3 inTangent;\n")+
   string("varying vec3 normal, tangent, binormal;\n")+
   string("varying vec3 lightDir, eyeVec;\n")+
   string("varying vec2 texCoord0;\n")+
   string("varying vec3 tgLightDir;\n")+
   string("varying vec3 tempVector;\n")+
   string("void main() {	\n")+
   string("	// Worldspace To Eye Space : Eye Space to Tangent Space Matrix\n")+
   string(" normal = normalize( gl_NormalMatrix * gl_Normal );\n")+
   string(" tangent = normalize( gl_NormalMatrix * inTangent );\n")+
   string(" binormal = cross( normal, tangent );\n")+
   string("	// Eye Space\n")+
   string("	vec3 vVertex = vec3( gl_ModelViewMatrix * gl_Vertex );\n")+
   string(" lightDir = vec3(gl_LightSource[0].position) - vVertex;\n")+
   string(" eyeVec = -vVertex;\n")+
   string(" tgLightDir.x = dot( tangent, lightDir );\n")+
   string(" tgLightDir.y = dot( binormal, lightDir );\n")+
   string(" tgLightDir.z = dot( normal, lightDir );\n")+
   string(" texCoord0 = gl_MultiTexCoord0.xy;\n")+
   string("	gl_Position = ftransform();\n")+
   string("}\n")
  ;
  string frag=
   string("#version 120\n")+
   string("uniform sampler2D DiffuseTextureID;\n")+
   string("uniform sampler2D SpecularTextureID;\n")+
   string("uniform sampler2D NormalTextureID;\n")+
   string("varying vec3 normal, tangent, binormal;\n")+
   string("varying vec3 lightDir, eyeVec;\n")+
   string("varying vec2 texCoord0;\n")+
   string("vec3 Normal, Tangent, Binormal;\n")+
   string("// Eye Space to Tangent Space\n")+
   string("vec3 EyeToTangent( vec3 inVec ) {\n")+
   string("	vec3 outVec;\n")+
   string("	outVec.x = dot( Tangent, inVec );\n")+
   string("	outVec.y = dot( Binormal, inVec );\n")+
   string("	outVec.z = dot( Normal, inVec );\n")+
   string("	return outVec;\n")+
   string("}\n")+
   string("vec3 GetNormal( vec2 coord ) {\n")+
   string("	vec3 ret = texture2D( NormalTextureID, coord ).xyz;\n")+
   string("	ret.xy = ret.xy * 2.0 - 1.0;\n")+
   string("	//ret.y *= -1.0;\n")+
   string("	return normalize( ret );\n")+
   string("}\n")+
   string("void main () {\n")+
   string("	Normal = normalize( normal );\n")+
   string("	Tangent = normalize( tangent );\n")+
   string("	Binormal = normalize( binormal );\n")+
   string("	vec4 diffuseMap = texture2D( DiffuseTextureID, texCoord0 );\n")+
   string("	vec4 specularMap = texture2D( SpecularTextureID, texCoord0 );\n")+
   string("	// Ambient\n")+
   string("	vec4 AmbientColor = \n")+
   string("		(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + \n")+
   string("			 (diffuseMap * gl_LightSource[0].ambient * gl_FrontMaterial.ambient);\n")+
   string("	//vec3 N = normalize(normal);\n")+
   string("	vec3 N = EyeToTangent( GetNormal( texCoord0 ) );\n")+
   string("	vec3 L = normalize( EyeToTangent( lightDir ) );\n")+
   string("	// Diffuse\n")+
   string("	vec4 DiffuseColor = \n")+
   string("			gl_LightSource[0].diffuse * \n")+
   string("			gl_FrontMaterial.diffuse *\n")+
   string("			diffuseMap * \n")+
   string("			max( dot(N,L), 0.0 );\n")+
   string("	// Specular\n")+
   string("	vec3 E = normalize( EyeToTangent( eyeVec ) );\n")+
   string("	vec3 R = reflect(-L, N);\n")+
   string("	float specular = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess );\n")+
   string("	vec4 SpecularColor = gl_LightSource[0].specular * gl_FrontMaterial.specular * specularMap * specular;\n")+
   string("	vec4 final_color = AmbientColor + DiffuseColor + SpecularColor;\n")+
   string("	gl_FragData[0] = vec4( final_color.rgb, 1.0 );\n")+
   string("	//gl_FragData[0] = vec4( specularMap.rgb, 1.0 );\n")+
   string("	gl_FragData[1] = vec4( 0.0, 0.0, 0.0, 1.0 );\n")+
   string("}")
  ;
 }
};