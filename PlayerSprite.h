#pragma once
#include "Sprite.h"

class PlayerSprite : public Sprite
{
public:
    bool isCollidingWithLadder;
    bool isOnGround;
    float velocityY;      // Dikey hız

private:
    float gravity;        // Yerçekimi ivmesi
    bool isJumping;       // Zıplama kontrolü

public:
    // Constructor (basic)
    PlayerSprite(CustomBitmap* pBitmap)
        : Sprite(pBitmap), gravity(1.3f), velocityY(0.0f), isJumping(false), isOnGround(false) {} 

    // Constructor with bounds
    PlayerSprite(CustomBitmap* pBitmap, RECT& rcBounds,
        BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, rcBounds, baBoundsAction), gravity(1.3f), velocityY(0.0f), isJumping(false), isOnGround(false) {}

    // Full constructor
    PlayerSprite(CustomBitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
        RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP)
        : Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, baBoundsAction),
          gravity(1.3f), velocityY(0.0f), isJumping(false), isOnGround(false) {}

    // Update fonksiyonu override
    virtual SPRITEACTION Update() override;

    // Space tuşu gibi input'u işleyen fonksiyon
    void HandleKeyDown(UINT key);
};
