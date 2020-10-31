#ifndef _H_AGK_TWEEN
#define _H_AGK_TWEEN

#define AGK_TWEEN_TYPE_NULL 0
#define AGK_TWEEN_TYPE_SPRITE 1
#define AGK_TWEEN_TYPE_TEXT 2
#define AGK_TWEEN_TYPE_CHAR 3
#define AGK_TWEEN_TYPE_OBJECT 4
#define AGK_TWEEN_TYPE_CAMERA 5

#define AGK_TWEEN_INTERP_LINEAR 0
#define AGK_TWEEN_INTERP_SMOOTH1 1
#define AGK_TWEEN_INTERP_SMOOTH2 2
#define AGK_TWEEN_INTERP_EASEIN1 3
#define AGK_TWEEN_INTERP_EASEIN2 4
#define AGK_TWEEN_INTERP_EASEOUT1 5
#define AGK_TWEEN_INTERP_EASEOUT2 6
#define AGK_TWEEN_INTERP_BOUNCE 7
#define AGK_TWEEN_INTERP_OVERSHOOT 8

#define AGK_TWEEN_CHAIN_PLAYING 0x01
#define AGK_TWEEN_CHAIN_PAUSED 0x02

namespace AGK
{
	class Tween
	{
		public:
			float m_fDuration;
						
			Tween() { m_fDuration = 0; }
			virtual ~Tween() {}

			virtual int GetType() = 0;
			virtual void Update( float fTime, void *target, int subtarget=-1 ) = 0;

			static float InterpLinear( float start, float end, float t );
			static float InterpSmooth1( float start, float end, float t );
			static float InterpSmooth2( float start, float end, float t );
			static float InterpEaseIn1( float start, float end, float t );
			static float InterpEaseIn2( float start, float end, float t );
			static float InterpEaseOut1( float start, float end, float t );
			static float InterpEaseOut2( float start, float end, float t );
			static float InterpBounce( float start, float end, float t );
			static float InterpOvershoot( float start, float end, float t );
	};

	class TweenSprite : public Tween
	{
		public:

			float m_fBeginX, m_fEndX; int m_iInterpX;
			float m_fBeginY, m_fEndY; int m_iInterpY;
			float m_fBeginAngle, m_fEndAngle; int m_iInterpAngle;
			float m_fBeginSizeX, m_fEndSizeX; int m_iInterpSizeX;
			float m_fBeginSizeY, m_fEndSizeY; int m_iInterpSizeY;
			int m_iBeginRed, m_iEndRed; int m_iInterpRed;
			int m_iBeginGreen, m_iEndGreen; int m_iInterpGreen;
			int m_iBeginBlue, m_iEndBlue; int m_iInterpBlue;
			int m_iBeginAlpha, m_iEndAlpha; int m_iInterpAlpha;
			
			TweenSprite();
			~TweenSprite() {}

			int GetType() { return AGK_TWEEN_TYPE_SPRITE; }
			void Update( float fTime, void *target, int subtarget );
	};

	class TweenText : public Tween
	{
		public:

			float m_fBeginX, m_fEndX; int m_iInterpX;
			float m_fBeginY, m_fEndY; int m_iInterpY;
			float m_fBeginAngle, m_fEndAngle; int m_iInterpAngle;
			float m_fBeginSize, m_fEndSize; int m_iInterpSize;
			float m_fBeginSpacing, m_fEndSpacing; int m_iInterpSpacing;
			float m_fBeginLineSpacing, m_fEndLineSpacing; int m_iInterpLineSpacing;
			int m_iBeginRed, m_iEndRed; int m_iInterpRed;
			int m_iBeginGreen, m_iEndGreen; int m_iInterpGreen;
			int m_iBeginBlue, m_iEndBlue; int m_iInterpBlue;
			int m_iBeginAlpha, m_iEndAlpha; int m_iInterpAlpha;
			
			TweenText();
			~TweenText() {}

			int GetType() { return AGK_TWEEN_TYPE_TEXT; }
			void Update( float fTime, void *target, int subtarget );
	};

	class TweenChar : public Tween
	{
		public:

			float m_fBeginX, m_fEndX; int m_iInterpX;
			float m_fBeginY, m_fEndY; int m_iInterpY;
			float m_fBeginAngle, m_fEndAngle; int m_iInterpAngle;
			int m_iBeginRed, m_iEndRed; int m_iInterpRed;
			int m_iBeginGreen, m_iEndGreen; int m_iInterpGreen;
			int m_iBeginBlue, m_iEndBlue; int m_iInterpBlue;
			int m_iBeginAlpha, m_iEndAlpha; int m_iInterpAlpha;
			
			TweenChar();
			~TweenChar() {}

			int GetType() { return AGK_TWEEN_TYPE_CHAR; }
			void Update( float fTime, void *target, int subtarget );
	};

	class TweenObject : public Tween
	{
		public:

			float m_fBeginX, m_fEndX; int m_iInterpX;
			float m_fBeginY, m_fEndY; int m_iInterpY;
			float m_fBeginZ, m_fEndZ; int m_iInterpZ;
			float m_fBeginAngleX, m_fEndAngleX; int m_iInterpAngleX;
			float m_fBeginAngleY, m_fEndAngleY; int m_iInterpAngleY;
			float m_fBeginAngleZ, m_fEndAngleZ; int m_iInterpAngleZ;
			float m_fBeginScaleX, m_fEndScaleX; int m_iInterpScaleX;
			float m_fBeginScaleY, m_fEndScaleY; int m_iInterpScaleY;
			float m_fBeginScaleZ, m_fEndScaleZ; int m_iInterpScaleZ;
			int m_iBeginRed, m_iEndRed; int m_iInterpRed;
			int m_iBeginGreen, m_iEndGreen; int m_iInterpGreen;
			int m_iBeginBlue, m_iEndBlue; int m_iInterpBlue;
			int m_iBeginAlpha, m_iEndAlpha; int m_iInterpAlpha;
			
			TweenObject();
			~TweenObject() {}

			int GetType() { return AGK_TWEEN_TYPE_OBJECT; }
			void Update( float fTime, void *target, int subtarget );
	};

	class TweenCamera : public Tween
	{
		public:

			float m_fBeginX, m_fEndX; int m_iInterpX;
			float m_fBeginY, m_fEndY; int m_iInterpY;
			float m_fBeginZ, m_fEndZ; int m_iInterpZ;
			float m_fBeginAngleX, m_fEndAngleX; int m_iInterpAngleX;
			float m_fBeginAngleY, m_fEndAngleY; int m_iInterpAngleY;
			float m_fBeginAngleZ, m_fEndAngleZ; int m_iInterpAngleZ;
			float m_fBeginFOV, m_fEndFOV; int m_iInterpFOV;
			
			TweenCamera();
			~TweenCamera() {}

			int GetType() { return AGK_TWEEN_TYPE_CAMERA; }
			void Update( float fTime, void *target, int subtarget );
	};

	class TweenInstance
	{
		public:
			Tween *m_pTween;
			TweenInstance *m_pNext;
			float m_fCurrentTime;
			float m_fBeginTime;
			float m_fDelay;
			void *m_pTarget;
			int m_iSubTarget;

			static TweenInstance *g_pInstances;
			static void DeleteTween( Tween* tween );
			static void DeleteInstance( Tween* tween, void* target, int subtarget=-1 );
			static TweenInstance* GetInstance( Tween* tween, void* target, int subtarget=-1 );

			TweenInstance();
			~TweenInstance() {}

			void Update( float fTime );
			void UpdateFromChain( float fTime );
	};

	class TweenChain
	{
		public:
			TweenInstance *m_pTweenList;
			TweenInstance *m_pLastTween;
			TweenInstance *m_pCurrTween;
			float m_fCurrTime;
			float m_fTotalTime;
			UINT m_bFlags;

			TweenChain();
			~TweenChain();
			
			void UpdateInstanceTimes();
			void DeleteTween( Tween* m_pTween );

			void ClearAll();
			void Play();
			void Pause();
			void Stop();

			void AddTweenForSprite( Tween *pTween, cSprite *pSprite, float delay );
			void AddTweenForText( Tween *pTween, cText *pText, float delay );
			void AddTweenForChar( Tween *pTween, cText *pText, int subtarget, float delay );
			void AddTweenForObject( Tween *pTween, cObject3D *pObject, float delay );
			void AddTweenForCamera( Tween *pTween, cCamera *pCamera, float delay );

			int IsPlaying() { return (m_bFlags & AGK_TWEEN_CHAIN_PLAYING) ? 1 : 0; }
			int IsPaused() { return (m_bFlags & AGK_TWEEN_CHAIN_PAUSED) ? 1 : 0; }

			void Update( float fTime );

	};
}

#endif