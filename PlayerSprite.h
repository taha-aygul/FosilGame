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
    PlayerSprite(CustomBitmap* pBitmap)
        : Sprite(pBitmap) {
    }  // Base s�n�f constructor'�n� �a��r�r

    PlayerSprite(CustomBitmap* pBitmap, RECT& rcBounds,
        BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, rcBounds, baBoundsAction) {
    }

    PlayerSprite(CustomBitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
        RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, baBoundsAction) {
    }




};

