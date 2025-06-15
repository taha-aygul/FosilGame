#include "GameBitmaps.h"

// Tüm statik pointer'larý nullptr olarak baþlatýyoruz.
CustomBitmap* GameBitmaps::Desert = nullptr;
CustomBitmap* GameBitmaps::Car = nullptr;
CustomBitmap* GameBitmaps::SmCar = nullptr;
CustomBitmap* GameBitmaps::Missile = nullptr;
CustomBitmap* GameBitmaps::Blobbo = nullptr;
CustomBitmap* GameBitmaps::BMissile = nullptr;
CustomBitmap* GameBitmaps::Jelly = nullptr;
CustomBitmap* GameBitmaps::JMissile = nullptr;
CustomBitmap* GameBitmaps::Timmy = nullptr;
CustomBitmap* GameBitmaps::TMissile = nullptr;
CustomBitmap* GameBitmaps::SmExplosion = nullptr;
CustomBitmap* GameBitmaps::LgExplosion = nullptr;
CustomBitmap* GameBitmaps::Ground = nullptr;
CustomBitmap* GameBitmaps::Egg = nullptr;
CustomBitmap* GameBitmaps::Ladder = nullptr;
CustomBitmap* GameBitmaps::GameOver = nullptr;

void GameBitmaps::Init(HDC hDC, HINSTANCE hInstance)
{
    Desert = new CustomBitmap(hDC, IDB_DESERT, hInstance);
    Car = new CustomBitmap(hDC, IDB_LADDER, hInstance); 
    SmCar = new CustomBitmap(hDC, IDB_SMCAR, hInstance);
    Missile = new CustomBitmap(hDC, IDB_MISSILE, hInstance);
    Blobbo = new CustomBitmap(hDC, IDB_BLOBBO, hInstance);
    BMissile = new CustomBitmap(hDC, IDB_BMISSILE, hInstance);
    Jelly = new CustomBitmap(hDC, IDB_JELLY, hInstance);
    JMissile = new CustomBitmap(hDC, IDB_JMISSILE, hInstance);
    Timmy = new CustomBitmap(hDC, IDB_TIMMY, hInstance);
    TMissile = new CustomBitmap(hDC, IDB_TMISSILE, hInstance);
    SmExplosion = new CustomBitmap(hDC, IDB_SMEXPLOSION, hInstance);
    LgExplosion = new CustomBitmap(hDC, IDB_LGEXPLOSION, hInstance);
    GameOver = new CustomBitmap(hDC, IDB_GAMEOVER, hInstance);
    Ground = new CustomBitmap(hDC, IDB_REDBLOCK, hInstance);
    Egg = new CustomBitmap(hDC, IDB_EGG, hInstance);
    Ladder = new CustomBitmap(hDC, IDB_GREENBLOCK, hInstance);
}

void GameBitmaps::Shutdown()
{
    // Oluþturulan tüm bitmap'leri siliyoruz.
    delete Desert; delete Car; delete SmCar; delete Missile;
    delete Blobbo; delete BMissile; delete Jelly; delete JMissile;
    delete Timmy; delete TMissile; delete SmExplosion; delete LgExplosion;
    delete Ground; delete Egg; delete Ladder; delete GameOver;
}