#include "PlayerSprite.h"
#include <Windows.h> // VK_SPACE için

SPRITEACTION PlayerSprite::Update()
{
    // Yerçekimini uygula
    //velocityY += gravity;

    // Mevcut velocity'yi al, sadece Y'yi değiştir
    POINT ptVel = GetVelocity();
    ptVel.y = static_cast<int>(velocityY);
    SetVelocity(ptVel);

    // Sprite'ı hareket ettir (1 kez)
    SPRITEACTION sa = Sprite::Update();

    // Oyuncu zemine değdi mi kontrol et
    RECT pos = GetPosition();

    if (isOnGround)
    {
        velocityY = 0;
	}
	
    else {
        velocityY += gravity;
		velocityY = min(velocityY, 5.0f); // Maksimum düşme hızı
    }
 
    isOnGround = false;

    return sa;
}


void PlayerSprite::HandleKeyDown(UINT key)
{
    if (key == VK_SPACE && isOnGround)
    {
        velocityY = -12.0f;   // zıplama gücü
        isJumping = true;
        isOnGround = false;
    }
}
