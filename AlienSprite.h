//-----------------------------------------------------------------
// Alien Sprite Object
// C++ Header - AlienSprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Sprite.h"

//-----------------------------------------------------------------
// AlienSprite Class
//-----------------------------------------------------------------
class AlienSprite : public Sprite
{
public:
  // Constructor(s)/Destructor
          AlienSprite(CustomBitmap* pBitmap, RECT& rcBounds,
            BOUNDSACTION baBoundsAction = BA_STOP);
  virtual ~AlienSprite();

  // General Methods
  virtual SPRITEACTION  Update();

  // Yeni özellikler
  bool isStalker;
  bool isChaser;  // Bu alien'ın takipçi olup olmadığını belirten flag
  void SetChaser(bool value) { isChaser = value; }
  void SetStalker(bool value) { isStalker = value; }
};
