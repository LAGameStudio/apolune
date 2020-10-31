#ifndef _H_3D_MATH_
#define _H_3D_MATH_

#define SMALL_FLOAT 0.000001f

// Common includes
#include "Common.h"

// Namespace
namespace AGK
{
	class AGKQuaternion;
	class AGKMatrix3;
	class AGKMatrix4;

	float EvaluateBezier( float c1, float c2, float t );
	float EvaluateBezierDt( float c1, float c2, float t );
	
	class AGKVector
	{
		public:
			float x, y, z;

			AGKVector() { x = 0; y = 0; z = 0; }
			AGKVector( float vx, float vy, float vz ) { x = vx; y = vy; z = vz; }
			AGKVector( const AGKVector& v ) { x = v.x; y = v.y; z = v.z; }
			~AGKVector() {}

			bool IsUniform() const { return (x == y) && (y == z); }

			void Set( float vx, float vy, float vz ) { x = vx; y = vy; z = vz; }
			void Mult( float s ) { x *= s; y *= s; z *= s; }
			void Scale( float sx, float sy, float sz ) { x *= sx; y *= sy; z *= sz; }

			AGKVector& Mult( const AGKMatrix3 &mat );
			AGKVector& Mult( const AGKMatrix4 &mat );
			AGKVector& Mult( const AGKQuaternion &rot );
			AGKVector& MultX( const AGKQuaternion &rot ); // optimization if you know vector Y=0 and Z=0
			AGKVector& MultY( const AGKQuaternion &rot ); // optimization if you know vector X=0 and Z=0
			AGKVector& MultZ( const AGKQuaternion &rot ); // optimization if you know vector X=0 and Y=0

			void Normalize();
			float Length() const;
			float SqrLength() const { return x*x + y*y + z*z; }
			float Dot( const AGKVector &v ) const { return x*v.x + y*v.y + z*v.z; }
			AGKVector& Cross( const AGKVector &u, const AGKVector &v );
			void FlattenToCrossVector( const AGKVector &u, const AGKVector &v );
			AGKVector ClosestPoint( const AGKVector &start, const AGKVector &object ) const;
			AGKVector ClosestNormalisedPoint( const AGKVector &start, const AGKVector &finish, const AGKVector &object ) const;
			float GetSqrDist( const AGKVector &v ) const { return (v.x-x)*(v.x-x) + (v.y-y)*(v.y-y) + (v.z-z)*(v.z-z); }
			float GetDist( const AGKVector &v ) const;

			inline AGKVector& operator =( const AGKVector& o ) { x=o.x; y=o.y; z=o.z; return *this; }
			inline AGKVector& operator +=( const AGKVector& o ) { x+=o.x; y+=o.y; z+=o.z; return *this; }
			inline AGKVector& operator -=( const AGKVector& o ) { x-=o.x; y-=o.y; z-=o.z; return *this; }
			inline AGKVector operator +( const AGKVector& o ) const { AGKVector result(x+o.x, y+o.y, z+o.z); return result; }
			inline AGKVector operator -( const AGKVector& o ) const { AGKVector result(x-o.x, y-o.y, z-o.z); return result; }
			inline AGKVector operator -() const { AGKVector result(-x, -y, -z); return result; }
			AGKVector GetInverse() const { AGKVector result(1/x, 1/y, 1/z); return result; }
			
			inline bool operator ==(const AGKVector& o) const { return x==o.x && y==o.y && z==o.z; }
			inline bool operator !=(const AGKVector& o) const { return x!=o.x || y!=o.y || z!=o.z; }
	};

	class AGKVector4
	{
		public:
			AGKVector vec;
			float w;

			AGKVector4() : vec(0,0,0) { w = 0; }
			AGKVector4( float vx, float vy, float vz, float vw ) : vec(vx,vy,vz) { w = vw; }
			AGKVector4( const AGKVector& v ) : vec(v) { w = 0; }
			~AGKVector4() {}

			AGKVector4& Mult( const AGKMatrix4 &mat );
			
			inline bool operator ==(const AGKVector4& o) const { return vec==o.vec && w==o.w; }
			inline bool operator !=(const AGKVector4& o) const { return vec!=o.vec || w!=o.w; }
	};

	class AGKQuaternion
	{
		public:
			float w, x, y, z;

			AGKQuaternion() { Identity(); }
			AGKQuaternion( float qw, float qx, float qy, float qz ) { w = qw; x = qx; y = qy; z = qz; }
			AGKQuaternion( const AGKQuaternion& q ) { w = q.w; x = q.x; y = q.y; z = q.z; }
			~AGKQuaternion() {}

			bool IsIdentity() const { return w==1; }

			void Set( float qw, float qx, float qy, float qz ) { w = qw; x = qx; y = qy; z = qz; }

			void Identity() { w = 1; x = 0; y = 0; z = 0; }
			void Normalize();
			void NormalizeFast(); // when the quaternion is already near length 1 (+-0.5), useful for interpolated values
			void Invert() { x = -x; y = -y; z = -z; }
			void Mult( const AGKQuaternion &other ); // Transforms this quaternion by the other quaternion
			void AddLocalRotation( int axis, float ang );
			void AddGlobalRotation( int axis, float ang );
			void Slerp( const AGKQuaternion &from, const AGKQuaternion &to, float t );
			void Lerp( const AGKQuaternion &from, const AGKQuaternion &to, float t );

			float GetEulerX();
			float GetEulerY();
			float GetEulerZ();

			void LookAt( float vx, float vy, float vz, float roll );
			void MakeFromEulerYXZ( float xang, float yang, float zang ); 
			void MakeFromEulerYX( float xang, float yang ); 
			void MakeFromAxisAngle( float vx, float vy, float vz, float ang ); 
			void MakeFromMatrix( const AGKMatrix3 &rot ); 

			inline AGKQuaternion& operator =( const AGKQuaternion& o ) { w=o.w; x=o.x; y=o.y; z=o.z; return *this; }
			inline AGKQuaternion operator *( const AGKQuaternion& o ) const { AGKQuaternion result(o); result.Mult(*this); return result; }
			inline AGKVector operator *( const AGKVector& v ) const { AGKVector result(v); result.Mult( *this ); return result; }
			inline AGKQuaternion operator -() const { AGKQuaternion result(w,-x,-y,-z); return result; }

			inline bool operator ==(const AGKQuaternion& o) const { return w==o.w && x==o.x && y==o.y && z==o.z; }
			inline bool operator !=(const AGKQuaternion& o) const { return w!=o.w || x!=o.x || y!=o.y || z!=o.z; }
	};

	class AGKMatrix3
	{
		public:
			float mat[3][3]; // matrix is [Column][Row]

			AGKMatrix3() { Identity(); }
			AGKMatrix3( const AGKMatrix3 &o ) { *this = o; }
			~AGKMatrix3() {}

			float* GetFloatPtr() { return &mat[0][0]; }
			
			void Identity();
			bool IsIdentity() const;
			void Set( const float* f ) { memcpy(mat,f,sizeof(float)*9); }

			AGKMatrix3& Mult( const AGKMatrix3& other ); // Transforms this matrix by the other matrix
			AGKMatrix3& Mult( const AGKMatrix4& other );
			AGKMatrix3& Transpose();

			float GetEulerX();
			float GetEulerY();
			float GetEulerZ();

			void MakeRotationX( float ang );
			void MakeRotationY( float ang );
			void MakeRotationZ( float ang );

			void MakeFromEulerYXZ( float xang, float yang, float zang );
			void MakeFromAxisAngle( float x, float y, float z, float ang );
			void MakeFromQuaternion( float w, float x, float y, float z );

			void MakeWorld( const AGKQuaternion &rot, const AGKVector &scale );
			void MakeInvWorld( const AGKQuaternion &rot, const AGKVector &scale );

			inline AGKMatrix3& operator =( const AGKMatrix3& o ) { memcpy(mat,o.mat,sizeof(float)*9); return *this; }
			inline AGKVector operator *( const AGKVector& v ) const { AGKVector result(v); result.Mult( *this ); return result; }
			
			inline bool operator ==( const AGKMatrix3& o ) 
			{ 
				return mat[0][0]==o.mat[0][0] && mat[0][1]==o.mat[0][1] && mat[0][2]==o.mat[0][2]
			        && mat[1][0]==o.mat[1][0] && mat[1][1]==o.mat[1][1] && mat[1][2]==o.mat[1][2]
					&& mat[2][0]==o.mat[2][0] && mat[2][1]==o.mat[2][1] && mat[2][2]==o.mat[2][2];
			}
			inline bool operator !=( const AGKMatrix3& o ) 
			{ 
				return mat[0][0]!=o.mat[0][0] || mat[0][1]!=o.mat[0][1] || mat[0][2]!=o.mat[0][2]
			        || mat[1][0]!=o.mat[1][0] || mat[1][1]!=o.mat[1][1] || mat[1][2]!=o.mat[1][2]
					|| mat[2][0]!=o.mat[2][0] || mat[2][1]!=o.mat[2][1] || mat[2][2]!=o.mat[2][2];
			}
	};

	class AGKMatrix4
	{
		public:
			float mat[4][4]; // matrix is [Column][Row]

			AGKMatrix4() { Identity(); }
			AGKMatrix4( const AGKMatrix3 &o ) { *this = o; }
			AGKMatrix4( const AGKMatrix4 &o ) { *this = o; }
			~AGKMatrix4() {}

			float* GetFloatPtr() { return &mat[0][0]; }
			inline AGKMatrix3 GetMat3() const { AGKMatrix3 r; memcpy(r.mat[0],mat[0],12); memcpy(r.mat[1],mat[1],12); memcpy(r.mat[2],mat[2],12); return r; }
			
			void Identity(); 
			bool IsIdentity() const;
			void Set( const float* f ) { memcpy(mat,f,sizeof(float)*16); }

			AGKMatrix4& Mult( const AGKMatrix4& other ); // Transforms this matrix by the other matrix
			
			void MakeWorld( const AGKMatrix3 &rot, const AGKVector &pos, const AGKVector &scale );
			void MakeWorld( const AGKQuaternion &rot, const AGKVector &pos, const AGKVector &scale );
			void MakeInvWorld( const AGKMatrix3 &rot, const AGKVector &pos, const AGKVector &scale );
			void MakeInvWorld( const AGKQuaternion &rot, const AGKVector &pos, const AGKVector &scale );
			void MakeProj( float fFov, float fAspect, float fNear, float fFar, bool righthand=false );
			void MakeOrtho( float l, float r, float b, float t, float n, float f );

			inline AGKMatrix4& operator =( const AGKMatrix4& o ) { memcpy(mat,o.mat,sizeof(float)*16); return *this; }
			AGKMatrix4& operator =( const AGKMatrix3& o );
			inline AGKVector operator *( const AGKVector& v ) const { AGKVector result(v); result.Mult( *this ); return result; }

			inline bool operator ==( const AGKMatrix4& o ) 
			{ 
				return mat[0][0]==o.mat[0][0] && mat[0][1]==o.mat[0][1] && mat[0][2]==o.mat[0][2] && mat[0][3]==o.mat[0][3]
			        && mat[1][0]==o.mat[1][0] && mat[1][1]==o.mat[1][1] && mat[1][2]==o.mat[1][2] && mat[1][3]==o.mat[1][3]
					&& mat[2][0]==o.mat[2][0] && mat[2][1]==o.mat[2][1] && mat[2][2]==o.mat[2][2] && mat[2][3]==o.mat[2][3]
					&& mat[3][0]==o.mat[3][0] && mat[3][1]==o.mat[3][1] && mat[3][2]==o.mat[3][2] && mat[3][3]==o.mat[3][3];
			}
			inline bool operator !=( const AGKMatrix4& o ) 
			{ 
				return mat[0][0]!=o.mat[0][0] || mat[0][1]!=o.mat[0][1] || mat[0][2]!=o.mat[0][2] || mat[0][3]!=o.mat[0][3]
			        || mat[1][0]!=o.mat[1][0] || mat[1][1]!=o.mat[1][1] || mat[1][2]!=o.mat[1][2] || mat[1][3]!=o.mat[1][3]
					|| mat[2][0]!=o.mat[2][0] || mat[2][1]!=o.mat[2][1] || mat[2][2]!=o.mat[2][2] || mat[2][3]!=o.mat[2][3]
					|| mat[3][0]!=o.mat[3][0] || mat[3][1]!=o.mat[3][1] || mat[3][2]!=o.mat[3][2] || mat[3][3]!=o.mat[3][3];
			}
			inline bool operator ==( const float* o ) 
			{ 
				return mat[0][0]==o[0] && mat[0][1]==o[1] && mat[0][2]==o[2] && mat[0][3]==o[3]
			        && mat[1][0]==o[4] && mat[1][1]==o[5] && mat[1][2]==o[6] && mat[1][3]==o[7]
					&& mat[2][0]==o[8] && mat[2][1]==o[9] && mat[2][2]==o[10] && mat[2][3]==o[11]
					&& mat[3][0]==o[12] && mat[3][1]==o[13] && mat[3][2]==o[14] && mat[3][3]==o[15];
			}
			inline bool operator !=( const float* o ) 
			{ 
				return mat[0][0]!=o[0] || mat[0][1]!=o[1] || mat[0][2]!=o[2] || mat[0][3]!=o[3]
			        || mat[1][0]!=o[4] || mat[1][1]!=o[5] || mat[1][2]!=o[6] || mat[1][3]!=o[7]
					|| mat[2][0]!=o[8] || mat[2][1]!=o[9] || mat[2][2]!=o[10] || mat[2][3]!=o[11]
					|| mat[3][0]!=o[12] || mat[3][1]!=o[13] || mat[3][2]!=o[14] || mat[3][3]!=o[15];
			}
	};
}

#endif