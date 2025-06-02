#pragma once
#include "Sprite.h"

class PlayerSprite :
    public Sprite
{
public:
    bool isCollidingWithLadder;
    bool isOnGround;




public:
    // Constructor
    PlayerSprite(Bitmap* pBitmap)
        : Sprite(pBitmap) {
    }  // Base s�n�f constructor'�n� �a��r�r

    PlayerSprite(Bitmap* pBitmap, RECT& rcBounds,
        BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, rcBounds, baBoundsAction) {
    }

    PlayerSprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
        RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, baBoundsAction) {
    }




};

