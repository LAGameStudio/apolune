#ifndef _H_SKELETON2D
#define _H_SKELETON2D

#define AGK_BONE_ANIMATE			0x00000001
#define AGK_BONE_INHERIT_ROTATION	0x00000002
#define AGK_BONE_INHERIT_SCALE		0x00000004
#define AGK_BONE_ROOT				0x00000008

#define AGK_SKELETON_PLAYING        0x00000001
#define AGK_SKELETON_LOOPING        0x00000002
#define AGK_SKELETON_FLIPH			0x00000004
#define AGK_SKELETON_FLIPV			0x00000008

#define AGK_SLOT_ANIMATE            0x00000001

namespace AGK
{
	class cSprite;
	class Bone2D;

	// attachment key frames
	class Anim2DKeyFrameAttachment
	{
		public:
			float m_fTime;
			uString m_sSpriteName;
			cSprite *m_pSprite;
	};

	// color key frames
	class Anim2DKeyFrameColor
	{
		public:
			float m_fTime;
			unsigned char m_iRed;
			unsigned char m_iGreen;
			unsigned char m_iBlue;
			unsigned char m_iAlpha;

			virtual void Interpolate( Anim2DKeyFrameColor* pNext, float t, unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &alpha );
	};

	class Anim2DKeyFrameColorStepped : public Anim2DKeyFrameColor
	{
		public:
			virtual void Interpolate( Anim2DKeyFrameColor* pNext, float t, unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &alpha )
			{
				red = m_iRed;
				green = m_iGreen;
				blue = m_iBlue;
				alpha = m_iAlpha;
			}
	};

	class Anim2DKeyFrameColorCurved : public Anim2DKeyFrameColor
	{
		public:
			float c1, c2, c3, c4;

			virtual void Interpolate( Anim2DKeyFrameColor* pNext, float t, unsigned char &red, unsigned char &green, unsigned char &blue, unsigned char &alpha );
	};

	class Anim2DSlot
	{
		public:
			uString m_sSlotName;
			int m_iSlotIndex;

			UINT m_iNumColors;
			Anim2DKeyFrameColor **m_pColors;

			UINT m_iNumAttachments;
			Anim2DKeyFrameAttachment **m_pAttachments;

			Anim2DSlot();
			~Anim2DSlot();
	};

	class Slot2D
	{
		public:
			uString m_sName;
			Bone2D *m_pParent;
			UINT m_bFlags;

			cSprite *m_pSprite;
			UINT m_iColor;

			cSprite *m_pOrigSprite;
			UINT m_iOrigColor;

			Anim2DSlot *m_pPrevAnim;
			Anim2DSlot *m_pAnim;

			UINT m_iPrevFrameColor;
			UINT m_iPrevFrameAttachment;
			UINT m_iCurrFrameColor;
			UINT m_iCurrFrameAttachment;

			Slot2D();
			~Slot2D();

			void Tween( float prevtime, float currtime, float s );
			void Interpolate( float currtime );
	};

	// rotation keyframes
	class Anim2DKeyFrameRotation
	{
		public:
			float m_fTime;
			float m_fRotation;

			virtual void Interpolate( Anim2DKeyFrameRotation* pNext, float t, float &r )
			{
				if ( !pNext ) 
				{
					r = m_fRotation;
					return;
				}

				float oldR = m_fRotation;
				float newR = pNext->m_fRotation;

				if ( oldR < newR )
				{
					while ( newR - oldR > 180 ) oldR += 360;
				}
				else
				{
					while ( oldR - newR > 180 ) newR += 360;
				}

				r = oldR + t*(newR - oldR);				
			}
	};

	class Anim2DKeyFrameRotationStepped : public Anim2DKeyFrameRotation
	{
		public:
			virtual void Interpolate( Anim2DKeyFrameRotation* pNext, float t, float &r )
			{
				r = m_fRotation;
			}
	};

	class Anim2DKeyFrameRotationCurved : public Anim2DKeyFrameRotation
	{
		public:
			float c1, c2, c3, c4;

			virtual void Interpolate( Anim2DKeyFrameRotation* pNext, float t, float &r )
			{
				float guess = 0.5f;
				float newGuess = t;
				int iter = 0;

				// use newton raphson to find s for t (t=beizer x axis)
				do
				{
					guess = newGuess;
					newGuess = guess - ( (EvaluateBezier(c1, c3, guess) - t) / EvaluateBezierDt(c1, c3, guess) );
					iter++;
				} while ( fabs(newGuess-guess) > 0.0001f && iter < 10 );

				// get new t (from bezier y axis) for this s
				t = EvaluateBezier(c2, c4, newGuess);

				float oldR = m_fRotation;
				float newR = pNext->m_fRotation;

				if ( oldR < newR )
				{
					while ( newR - oldR > 180 ) oldR += 360;
				}
				else
				{
					while ( oldR - newR > 180 ) newR += 360;
				}

				r = oldR + t*(newR - oldR);	
			}
	};

	// position keyframes
	class Anim2DKeyFramePosition
	{
		public:
			float m_fTime;
			float m_fX;
			float m_fY;

			virtual void Interpolate( Anim2DKeyFramePosition* pNext, float t, float &x, float &y )
			{
				if ( !pNext ) 
				{
					x = m_fX;
					y = m_fY;
					return;
				}

				x = m_fX + t*(pNext->m_fX - m_fX);
				y = m_fY + t*(pNext->m_fY - m_fY);
			}
	};

	class Anim2DKeyFramePositionStepped : public Anim2DKeyFramePosition
	{
		public:
			virtual void Interpolate( Anim2DKeyFramePosition* pNext, float t, float &x, float &y )
			{
				x = m_fX;
				y = m_fY;
			}
	};

	class Anim2DKeyFramePositionCurved : public Anim2DKeyFramePosition
	{
		public:
			float c1, c2, c3, c4;

			virtual void Interpolate( Anim2DKeyFramePosition* pNext, float t, float &x, float &y )
			{
				float guess = 0.5f;
				float newGuess = t;
				int iter = 0;

				// use newton raphson to find s for t (t=beizer x axis)
				do
				{
					guess = newGuess;
					newGuess = guess - ( (EvaluateBezier(c1, c3, guess) - t) / EvaluateBezierDt(c1, c3, guess) );
					iter++;
				} while ( fabs(newGuess-guess) > 0.00001f && iter < 10 );

				// get new t (from bezier y axis) for this s
				t = EvaluateBezier(c2, c4, newGuess);

				x = m_fX + t*(pNext->m_fX - m_fX);
				y = m_fY + t*(pNext->m_fY - m_fY);
			}
	};

	// scale keyframes
	class Anim2DKeyFrameScale
	{
		public:
			float m_fTime;
			float m_fScaleX;
			float m_fScaleY;

			virtual void Interpolate( Anim2DKeyFrameScale* pNext, float t, float &x, float &y )
			{
				if ( !pNext ) 
				{
					x = m_fScaleX;
					y = m_fScaleY;
					return;
				}

				x = m_fScaleX + t*(pNext->m_fScaleX - m_fScaleX);
				y = m_fScaleY + t*(pNext->m_fScaleY - m_fScaleY);
			}
	};

	class Anim2DKeyFrameScaleStepped : public Anim2DKeyFrameScale
	{
		public:
			virtual void Interpolate( Anim2DKeyFrameScale* pNext, float t, float &x, float &y )
			{
				x = m_fScaleX;
				y = m_fScaleY;
			}
	};

	class Anim2DKeyFrameScaleCurved : public Anim2DKeyFrameScale
	{
		public:
			float c1, c2, c3, c4;

			virtual void Interpolate( Anim2DKeyFrameScale* pNext, float t, float &x, float &y )
			{
				float guess = 0.5f;
				float newGuess = t;
				int iter = 0;

				// use newton raphson to find s for t (t=beizer x axis)
				do
				{
					guess = newGuess;
					newGuess = guess - ( (EvaluateBezier(c1, c3, guess) - t) / EvaluateBezierDt(c1, c3, guess) );
					iter++;
				} while ( fabs(newGuess-guess) > 0.00001f && iter < 10 );

				// get new t (from bezier y axis) for this s
				t = EvaluateBezier(c2, c4, newGuess);

				x = m_fScaleX + t*(pNext->m_fScaleX - m_fScaleX);
				y = m_fScaleY + t*(pNext->m_fScaleY - m_fScaleY);
			}
	};

	// animation for 1 bone
	class Anim2DBone
	{
		public:
			uString m_sBoneName;
			int m_iBoneIndex;
			
			UINT m_iNumRotations;
			Anim2DKeyFrameRotation **m_pRotations;

			UINT m_iNumPositions;
			Anim2DKeyFramePosition **m_pPositions;

			UINT m_iNumScales;
			Anim2DKeyFrameScale **m_pScales;

			Anim2DBone();
			~Anim2DBone();
	};

	class Bone2D
	{
		public:
			uString m_sName;
			float length;
			float origX, origY, origAngle, origSX, origSY;
			float x, y, angle, sX, sY;
			float worldX, worldY, worldAngle, worldSX, worldSY;
			UINT m_bFlags;
			Bone2D *m_pParent;
			float m00, m01, m10, m11;

			Anim2DBone *m_pPrevAnim;
			Anim2DBone *m_pAnim;
			UINT m_iPrevFrameRotation;
			UINT m_iPrevFramePosition;
			UINT m_iPrevFrameScale;
			UINT m_iCurrFrameRotation;
			UINT m_iCurrFramePosition;
			UINT m_iCurrFrameScale;

			Bone2D();
			~Bone2D();

			void Tween( float prevtime, float currtime, float s );
			void Interpolate( float currtime );

			void UpdateWorldMatrix( int flipH, int flipV );
			void ResetToOrig();
	};

	// single animation for a set of bones, e.g. "walk"
	class Animation2D
	{
		public:
			uString m_sName;
			float m_fTime;
			
			UINT m_iNumBones;
			Anim2DBone *m_pBoneAnims;
			
			UINT m_iNumSlots;
			Anim2DSlot *m_pSlotAnims;

			Animation2D();
			~Animation2D();

			Anim2DBone* GetAnimForBone( const char* name );
			Anim2DSlot* GetAnimForSlot( const char* name );
	};

	class Skeleton2D
	{
			friend class agk;

		protected:
			UINT m_iNumBones;
			Bone2D *m_pBones;

			UINT m_iNumSprites;
			cSprite *m_pSprites;

			UINT m_iNumAnimations;
			Animation2D *m_pAnimations;

			UINT m_iNumSlots;
			Slot2D *m_pSlots;

			UINT m_iID;
			UINT m_bFlags;
			float m_fCurrTime;
			float m_fPrevTime;
			float m_fTweenTime;
			float m_fTotalTweenTime;
			float m_fSpeed;
			int m_iCurrAnimation;

			float m_fX;
			float m_fY;
			float m_fAngle;

			int m_iDepth;

		public:
			Skeleton2D();
			~Skeleton2D();

			void LoadFromSpine( const char* filename, float scale, cImage *pAtlas, int loadAnim );
			Bone2D* GetBone( const char* name );
			Bone2D* GetBone( UINT index );
			int GetBoneIndex( const char* name );
			Slot2D* GetSlot( const char* name );
			int GetSlotIndex( const char* name );
			cSprite* GetSprite( const char* name );
			int GetAnimation( const char* name );

			void SetPosition( float x, float y );
			void SetAngle( float angle );
			void SetDepth( int depth );
			int GetDepth() { return m_iDepth; }
			void FixToScreen( int mode );

			float GetX() { return m_fX; }
			float GetY() { return m_fY; }
			float GetAngle() { return m_fAngle; }

			void SetAnimationSpeed( float speed );
			void PlayAnimation( const char* anim, float starttime, int loop, float tweentime );
			void SetAnimationFrame( const char* anim, float time, float tweentime );
			void StopAnimation();
			int GetIsAnimating();
			int GetIsTweening();
			float GetCurrentTime();
			float GetAnimationTime( const char* anim );

			void SetFlipH( int flip );
			void SetFlipV( int flip );

			void Update( float time );
			void Draw();
			void DrawBones();
			void DrawBoneNames();
	};
}

#endif