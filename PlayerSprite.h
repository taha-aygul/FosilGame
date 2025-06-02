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
    }  // Base sýnýf constructor'ýný çaðýrýr

    PlayerSprite(CustomBitmap* pBitmap, RECT& rcBounds,
        BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, rcBounds, baBoundsAction) {
    }

    PlayerSprite(CustomBitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
        RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, baBoundsAction) {
    }




};

