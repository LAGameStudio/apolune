#ifndef _H_CAMERA_
#define _H_CAMERA_

// Common includes
#include "Common.h"
#include "3DMath.h"

// Namespace
namespace AGK
{
	class cCamera
	{
		protected:
			AGKVector m_position;
			AGKQuaternion m_rotation;
			float m_fFOV;

		public:
			cCamera();
			~cCamera() {}

			float GetX() { return m_position.x; }
			float GetY() { return m_position.y; }
			float GetZ() { return m_position.z; }

			float GetEulerX() { return m_rotation.GetEulerX(); }
			float GetEulerY() { return m_rotation.GetEulerY(); }
			float GetEulerZ() { return m_rotation.GetEulerZ(); }

			const AGKVector& GetPosition() { return m_position; }
			const AGKQuaternion& GetRotation() { return m_rotation; }
			
			void SetPosition( float x, float y, float z ) { m_position.x = x; m_position.y = y; m_position.z = z; }
			void SetRotationQuat( const AGKQuaternion &rot ) { m_rotation = rot; }
			void SetRotationEuler( float angX, float angY, float angZ ) { m_rotation.MakeFromEulerYXZ( angX, angY, angZ ); }

			void MoveLocalX( float amount );
			void MoveLocalY( float amount );
			void MoveLocalZ( float amount );

			void RotateLocalX( float amount ) { m_rotation.AddLocalRotation( 0, amount ); }
			void RotateLocalY( float amount ) { m_rotation.AddLocalRotation( 1, amount ); }
			void RotateLocalZ( float amount ) { m_rotation.AddLocalRotation( 2, amount ); }

			void RotateGlobalX( float amount ) { m_rotation.AddGlobalRotation( 0, amount ); }
			void RotateGlobalY( float amount ) { m_rotation.AddGlobalRotation( 1, amount ); }
			void RotateGlobalZ( float amount ) { m_rotation.AddGlobalRotation( 2, amount ); }

			void LookAt( float x, float y, float z, float roll );
			//void SetRange( float near, float far );
			void SetFOV( float FOV );
			
			void GetViewMatrix( AGKMatrix4 &matOut ) { matOut.MakeInvWorld( m_rotation, m_position, AGKVector(1,1,1) ); }
			void GetViewNormalMatrix( AGKMatrix3 &matOut ) { matOut.MakeWorld( m_rotation, -AGKVector(1,1,1) ); }
	};
}

#endif