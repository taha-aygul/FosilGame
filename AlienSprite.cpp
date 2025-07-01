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
extern int     _iDifficulty;
extern PlayerSprite* _pPlayerSprite;

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
  if (isChaser && _pPlayerSprite != NULL)
  {
    // Oyuncunun pozisyonunu al
    RECT rcPlayer = _pPlayerSprite->GetPosition();
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
  else if (isStalker && _pPlayerSprite != NULL)
  {
      RECT rcPlayer = _pPlayerSprite->GetPosition();
      RECT rcAlien = GetPosition();
      POINT ptVelocity = GetVelocity();

      // Merkezden merkeze uzaklığı hesapla
      int dx = (rcPlayer.left + _pPlayerSprite->GetWidth() / 2) - (rcAlien.left + GetWidth() / 2);
      int dy = (rcPlayer.top + _pPlayerSprite->GetHeight() / 2) - (rcAlien.top + GetHeight() / 2);
      float distance = sqrtf(static_cast<float>(dx * dx + dy * dy));

      // Yakınsa X ekseninde hareket et
      if (distance <200.0f)
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

