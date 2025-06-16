//-----------------------------------------------------------------
// Alien Sprite Object
// C++ Source - AlienSprite.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "AlienSprite.h"
#include "PlayerSprite.h"
#include <cmath>
//-----------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------
extern CustomBitmap* _pBlobboBitmap;
extern CustomBitmap* _pBMissileBitmap;
extern CustomBitmap* _pJellyBitmap;
extern CustomBitmap* _pJMissileBitmap;
extern CustomBitmap* _pTimmyBitmap;
extern CustomBitmap* _pTMissileBitmap;
extern int     _iDifficulty;
extern PlayerSprite* _pCarSprite;

//-----------------------------------------------------------------
// AlienSprite Constructor(s)/Destructor
//-----------------------------------------------------------------
AlienSprite::AlienSprite(CustomBitmap* pBitmap, RECT& rcBounds,
  BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
  baBoundsAction)
{
  isChaser = false;  // Varsayılan olarak takipçi değil
  isStalker = false;
}

AlienSprite::~AlienSprite()
{
}

//-----------------------------------------------------------------
// AlienSprite General Methods
//-----------------------------------------------------------------
SPRITEACTION AlienSprite::Update()
{
  // Call the base sprite Update() method
  SPRITEACTION saSpriteAction;
  saSpriteAction = Sprite::Update();

  // Eğer bu bir takipçi alien ise
  if (isChaser && _pCarSprite != NULL)
  {
    // Oyuncunun pozisyonunu al
    RECT rcPlayer = _pCarSprite->GetPosition();
    RECT rcAlien = GetPosition();
    
    // Oyuncuya doğru hareket et
    POINT ptVelocity = GetVelocity();
    
    // Yatay hareket
    if (rcPlayer.left < rcAlien.left)
      ptVelocity.x = -1;  // Sola hareket
    else if (rcPlayer.left > rcAlien.left)
      ptVelocity.x = 1;   // Sağa hareket
      
    // Dikey hareket
    if (rcPlayer.top < rcAlien.top)
      ptVelocity.y = -1;  // Yukarı hareket
    else if (rcPlayer.top > rcAlien.top)
      ptVelocity.y = 1;   // Aşağı hareket
      
    SetVelocity(ptVelocity);
  }
  else if (isStalker && _pCarSprite != NULL)
  {
      RECT rcPlayer = _pCarSprite->GetPosition();
      RECT rcAlien = GetPosition();
      POINT ptVelocity = GetVelocity();

      // Merkezden merkeze uzaklığı hesapla
      int dx = (rcPlayer.left + _pCarSprite->GetWidth() / 2) - (rcAlien.left + GetWidth() / 2);
      int dy = (rcPlayer.top + _pCarSprite->GetHeight() / 2) - (rcAlien.top + GetHeight() / 2);
      float distance = sqrtf(static_cast<float>(dx * dx + dy * dy));

      // Yakınsa X ekseninde hareket et
      if (distance < 99999.0f)
      {
          if (abs(dx) > 2)
              ptVelocity.x = (dx > 0) ? 1 : -1;
          else
              ptVelocity.x = 0; // Yaklaşınca dur
      }
      else
      {
          ptVelocity.x = 0; // uzaksa sabit
      }

      ptVelocity.y = 0; // Y ekseni sabit kalsın
      SetVelocity(ptVelocity);
  }


  return saSpriteAction;
}

Sprite* AlienSprite::AddSprite()
{
  // Create a new missile sprite
  RECT    rcBounds = { 0, 0, 640, 410 };
  RECT    rcPos = GetPosition();
  Sprite* pSprite = NULL;
  if (GetBitmap() == _pBlobboBitmap)
  {
    // Blobbo missile
    pSprite = new Sprite(_pBMissileBitmap, rcBounds, BA_DIE);
    pSprite->SetVelocity(0, 7);
  }
  else if (GetBitmap() == _pJellyBitmap)
  {
    // Jelly missile
    pSprite = new Sprite(_pJMissileBitmap, rcBounds, BA_DIE);
    pSprite->SetVelocity(0, 5);
  }
  else
  {
    // Timmy missile
    pSprite = new Sprite(_pTMissileBitmap, rcBounds, BA_DIE);
    pSprite->SetVelocity(0, 3);
  }

  // Set the missile sprite's position and return it
  pSprite->SetPosition(rcPos.left + (GetWidth() / 2), rcPos.bottom);
  return pSprite;
}
