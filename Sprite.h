//-----------------------------------------------------------------
// Sprite Object
// C++ Header - Sprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "CustomBitmap.h"

//-----------------------------------------------------------------
// Custom Data Types
//-----------------------------------------------------------------
typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE = 0x0000L,
SA_KILL = 0x0001L,
SA_ADDSPRITE = 0x0002L;

typedef WORD        BOUNDSACTION;
const BOUNDSACTION  BA_STOP = 0,
BA_WRAP = 1,
BA_BOUNCE = 2,
BA_DIE = 3;

//-----------------------------------------------------------------
// Sprite Class
//-----------------------------------------------------------------
class Sprite
{
protected:
	// Member Variables
	CustomBitmap* m_pBitmap;
	int           m_iNumFrames, m_iCurFrame;
	int           m_iFrameDelay, m_iFrameTrigger;
	RECT          m_rcPosition,
		m_rcCollision;
	POINT         m_ptVelocity;
	int           m_iZOrder;
	RECT          m_rcBounds;
	BOUNDSACTION  m_baBoundsAction;
	BOOL          m_bHidden;
	BOOL          m_bDying;
	BOOL          m_bOneCycle;
	BOOL          m_bFlipped;
	BOOL          m_bFlippable;
	// Helper Methods
	void          UpdateFrame();
	virtual void  CalcCollisionRect();


public:
	// Constructor(s)/Destructor
	Sprite(CustomBitmap* pBitmap);
	Sprite(CustomBitmap* pBitmap, RECT& rcBounds,
		BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(CustomBitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
		RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();

	// General Methods
	virtual SPRITEACTION  Update();
	virtual Sprite* AddSprite();
	void                  Draw(HDC hDC);
	BOOL                  IsPointInside(int x, int y);
	BOOL                  TestCollision(Sprite* pTestSprite);
	void                  Kill() { m_bDying = TRUE; };

	// Accessor Methods
	CustomBitmap* GetBitmap() { return m_pBitmap; };
	void    SetNumFrames(int iNumFrames, BOOL bOneCycle = FALSE, BOOL flippable = false);
	void    SetFrameDelay(int iFrameDelay) { m_iFrameDelay = iFrameDelay; };
	RECT& GetPosition() { return m_rcPosition; };
	void    SetPosition(int x, int y);
	void    SetPosition(POINT ptPosition);
	void    SetPosition(RECT& rcPosition);
	void    OffsetPosition(int x, int y);
	RECT& GetCollision() { return m_rcCollision; };
	POINT   GetVelocity() { return m_ptVelocity; };
	void    SetVelocity(int x, int y);
	void    SetVelocity(POINT ptVelocity);
	BOOL    GetZOrder() { return m_iZOrder; };
	void    SetZOrder(int iZOrder) { m_iZOrder = iZOrder; };
	void    SetBounds(RECT& rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
	void    SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
	BOOL    IsHidden() { return m_bHidden; };
	void    SetHidden(BOOL bHidden) { m_bHidden = bHidden; };

	// Sprite.h i�inde
	// Sprite.h i�inde
	inline int Sprite::GetWidth()
	{
		 if (m_pBitmap == NULL || m_iNumFrames <= 0)
        return 0;

    int iTotalFramesInBitmap = m_bFlippable ? (m_iNumFrames * 2) : m_iNumFrames;
    
    return m_pBitmap->GetWidth() / iTotalFramesInBitmap;
	}
	int     GetHeight() { return m_pBitmap->GetHeight(); };

	void SetFlipped(BOOL bFlipped);
};

//-----------------------------------------------------------------
// Sprite Inline Helper Methods
//-----------------------------------------------------------------
// Sprite.h i�inde
inline void Sprite::UpdateFrame()
{
	if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0))
	{
		// Frame tetikleyicisini s�f�rla
		m_iFrameTrigger = m_iFrameDelay;

		// Varsay�lan animasyon aral���
		int iStartFrame = 0;
		int iEndFrame = m_iNumFrames;

		// SADECE sprite hem "flippable" hem de "flipped" ise ikinci animasyon setini kullan
		if (m_bFlippable && m_bFlipped)
		{
			iStartFrame = m_iNumFrames;
			iEndFrame = 2 * m_iNumFrames;
		}

		// Frame'i art�r
		if (++m_iCurFrame >= iEndFrame)
		{
			// E�er tek d�ng�l�k bir animasyon ise sprite'� yok et
			if (m_bOneCycle)
				m_bDying = TRUE;
			else
				// Mevcut animasyonun ba��na d�n
				m_iCurFrame = iStartFrame;
		}
	}
}

inline void Sprite::CalcCollisionRect()
{
	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
	CopyRect(&m_rcCollision, &m_rcPosition);
	InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

//-----------------------------------------------------------------
// Sprite Inline General Methods
//-----------------------------------------------------------------
inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
	RECT& rcTest = pTestSprite->GetCollision();
	return m_rcCollision.left <= rcTest.right &&
		rcTest.left <= m_rcCollision.right &&
		m_rcCollision.top <= rcTest.bottom &&
		rcTest.top <= m_rcCollision.bottom;
}

inline BOOL Sprite::IsPointInside(int x, int y)
{
	POINT ptPoint;
	ptPoint.x = x;
	ptPoint.y = y;
	return PtInRect(&m_rcPosition, ptPoint);
}

//-----------------------------------------------------------------
// Sprite Inline Accessor Methods
//-----------------------------------------------------------------
inline void Sprite::SetNumFrames(int iNumFrames, BOOL bOneCycle, BOOL flippable)
{
	// Frame say�s�n� ve d�ng� ayar�n� yap
	m_iNumFrames = iNumFrames;
	m_bOneCycle = bOneCycle;
	m_bFlippable = flippable;
	// Pozisyonu yeniden hesapla
	RECT rect = GetPosition();
	// Sprite'�n geni�li�i art�k tek bir karenin geni�li�idir.
	rect.right = rect.left + GetWidth();
	SetPosition(rect);
}


// Sprite.h i�inde
inline void Sprite::SetFlipped(BOOL bFlipped)
{
	// Sadece "flippable" olarak i�aretlenmi� spritelar�n y�n� de�i�tirilebilir.
	if (!m_bFlippable)
		return;

	// Sadece durum de�i�iyorsa i�lem yap
	if (m_bFlipped != bFlipped)
	{
		m_bFlipped = bFlipped;

		// Mevcut frame'i yeni animasyonun ba�lang�� karesine ayarla
		m_iCurFrame = m_bFlipped ? m_iNumFrames : 0;
	}
}

inline void Sprite::SetPosition(int x, int y)
{
	OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
	CalcCollisionRect();
}

inline void Sprite::SetPosition(POINT ptPosition)
{
	OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
		ptPosition.y - m_rcPosition.top);
	CalcCollisionRect();
}

inline void Sprite::SetPosition(RECT& rcPosition)
{
	CopyRect(&m_rcPosition, &rcPosition);
	CalcCollisionRect();
}

inline void Sprite::OffsetPosition(int x, int y)
{
	OffsetRect(&m_rcPosition, x, y);
	CalcCollisionRect();
}

inline void Sprite::SetVelocity(int x, int y)
{
	m_ptVelocity.x = x;
	m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
	m_ptVelocity.x = ptVelocity.x;
	m_ptVelocity.y = ptVelocity.y;
}


