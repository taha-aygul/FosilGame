#pragma once

#include <windows.h>
#include "CustomBitmap.h"
#include "resource.h"

// Tüm oyun bitmap'lerini tutan merkezi ve statik depo sýnýfý
class GameBitmaps
{
public:
    // Oyun Dünyasý
    static CustomBitmap* Desert;

    // Oyuncu ve Ýlgili Objeler
    static CustomBitmap* Car;
    static CustomBitmap* SmCar;
    static CustomBitmap* Missile;

    // Düþmanlar ve Mermileri
    static CustomBitmap* Blobbo;
    static CustomBitmap* BMissile;
    static CustomBitmap* Jelly;
    static CustomBitmap* JMissile;
    static CustomBitmap* Timmy;
    static CustomBitmap* TMissile;

    // Efektler
    static CustomBitmap* SmExplosion;
    static CustomBitmap* LgExplosion;

    // Level Objeleri
    static CustomBitmap* Ground;
    static CustomBitmap* Egg;
    static CustomBitmap* Ladder;

    // UI
    static CustomBitmap* GameOver;

public:
    static void Init(HDC hDC, HINSTANCE hInstance);
    static void Shutdown();

private:
    GameBitmaps() = delete;
    ~GameBitmaps() = delete;
};