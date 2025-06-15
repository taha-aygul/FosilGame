#include "BitmapLevelLoader.h"
#include "resource.h"       
#include "GameEngine.h"    
#include "Sprite.h"
#include "CustomBitmap.h"
#include <cassert>
#include <random>
#include <iostream>
#include "AlienSprite.h"

#pragma comment(lib, "gdiplus.lib")

extern int _eggsInLevel; // Add this at the top of the file

// GDI+ token
static ULONG_PTR gdiPlusToken = 0;
HDC BitmapLevelLoader::hDC = nullptr;
HINSTANCE BitmapLevelLoader::hInstance = nullptr;


// ---------------------------------------------------------
BitmapLevelLoader::BitmapLevelLoader() {}
BitmapLevelLoader::~BitmapLevelLoader() {}

// ---------------------------------------------------------
// GDI+ başlatma ve kapatma
// ---------------------------------------------------------
void BitmapLevelLoader::InitGDIPlus()
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, nullptr);
}

void BitmapLevelLoader::ShutdownGDIPlus()
{
    if (gdiPlusToken != 0)
    {
        GdiplusShutdown(gdiPlusToken);
        gdiPlusToken = 0;
    }
}

// ---------------------------------------------------------
// GenerateLevelFromBitmap
//  levelResourceID: resource.h içinde tanımlı BMP ID’si (örneğin IDB_LEVEL01)
//  tileSize: her pixelin oyun dünyasındaki gerçek genişliği (ör.:16)
// ---------------------------------------------------------

void BitmapLevelLoader::GenerateLevelFromBitmap(int levelResourceID, int tileSize)
{
    // 1) Bellekteki resource’tan HBITMAP elde et
    HBITMAP hBmp = nullptr;
    {
        // Win32 LoadBitmap yerine GDI+ ile doğrudan kaynak ID’sinden yükleme:
        //  - Önce CustomBitmap::FromResource kullanacağız (GDI+). Alternatif olarak CreateDIBSection + LoadImage de kullanılabilir.
        GameEngine* engInst = GameEngine::GetEngine();
		HINSTANCE hInst = engInst->GetInstance();
        hBmp = (HBITMAP)LoadImage(
            hInst,
            MAKEINTRESOURCE(levelResourceID),
            IMAGE_BITMAP,
            0, 0,
            LR_CREATEDIBSECTION);

        if (hBmp == nullptr)
        {
            MessageBoxW(nullptr, L"Level BMP yüklenemedi!", L"Hata", MB_OK | MB_ICONERROR);
            return;
        }
    }

    // 2) HBITMAP’i GDI+ Bitmap’e dönüştür
    Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromHBITMAP(hBmp, nullptr);
    assert(bmp != nullptr);

    // 3) Boyutları oku (80×60 bekleniyor, ama dinamik olarak da çalışır)
    UINT width = bmp->GetWidth();
    UINT height = bmp->GetHeight();

    // 4) Her pikseli tarayıp renge göre nesne oluştur
    for (UINT y = 0; y < height; ++y)
    {
        for (UINT x = 0; x < width; ++x)
        {
            Color pixelColor;
            bmp->GetPixel(x, y, &pixelColor);
            BYTE r = pixelColor.GetR();
            BYTE g = pixelColor.GetG();
            BYTE b = pixelColor.GetB();

            // Her piksel pozisyonunu tileSize ile dünya koordinatına çeviriyoruz:
            // pixelXCoord = x * tileSize;  pixelYCoord = y * tileSize;
            if (r == 255 && g == 0 && b == 0)
            {
                // Kırmızı: Ground (Zemin) bloğu
                CreateGroundTile((int)x, (int)y, tileSize);
            }
            else if (r == 0 && g == 255 && b == 0)
            {
                // Yeşil: Ladder (Merdiven)
                CreateLadderTile((int)x, (int)y, tileSize);
            }
            else if (r == 0 && g == 0 && b == 255)
            {
                // Mavi: Enemy
                CreateEnemyAt((int)x, (int)y, tileSize);
            }
            else if (r == 255 && g == 255 && b == 0) {
				CreateEgg((int)x, (int)y, tileSize);
            }
            else if (r == 0 && g == 0 && b == 0) {
                CreateInvisibleEdge((int)x, (int)y, tileSize);
            }
            else if (r == 0 && g == 128 && b == 0) {
                CreateGreenEnemy((int)x, (int)y, tileSize);
            }
            else if (r == 128 && g == 0 && b == 128) {
                CreateChaserEnemy((int)x, (int)y, tileSize);
            }
            else if (r == 128 && g == 0 && b == 0) {
                CreateStalkerEnemy((int)x, (int)y, tileSize);
            }
            else
            {
                // Siyah veya başka renkler: Boş (hiçbir şey yapma)
                // ya da başka bir renge göre başka nesne eklemek isterseniz buraya ekleyin:
                // else if (r==255 && g==255 && b==0) { CreateGold((int)x,(int)y,tileSize); }
            }
        }
    }

    // 5) Belleği temizle
    delete bmp;
    DeleteObject(hBmp);
}

// ---------------------------------------------------------
// CreateGroundTile
//   pixelX, pixelY: tilemap üzerindeki koordinat (0..79, 0..59 gibi).
//   tileSize: her pikseli kaç piksel genişliğe dönüştüreceğiz (ör:16).
// ---------------------------------------------------------
void BitmapLevelLoader::CreateGroundTile(int pixelX, int pixelY, int tileSize)
{

    // 1) Dünya koordinatını hesapla
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    // 2) Ground’ın kendine ait bir Bitmap kaynağı olmalı. Örneğin IDC_GROUND bitmap'i ek, resource.h’da:
    //    #define IDB_GROUND  2013
    //    SpaceOut.rc: IDB_GROUND BITMAP "res\\ground.bmp"
    // Burada, GroundSprite adında yeni bir sınıf yoksa, Sprite sınıfını da kullanabilirsiniz:
    //
    
    CustomBitmap* pGroundBmp = GameEngine::GetEngine()->_groundBitmap;     // new CustomBitmap(hDC, IDB_REDBLOCK, hInstance);

    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* ground = new Sprite(pGroundBmp,rcBounds,BA_WRAP);

    // 3) Sprite’ın pozisyonunu ayarla:
    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    ground->SetPosition(pt);


    // 4) Engine’e ekle
    GameEngine::GetEngine()->AddSprite(ground);
// 5) Eğer isterseniz, ground için bir ses de ekleyebilirsiniz:
// PlaySound((LPCSTR)IDW_GROUNDTILE, hInstance, SND_ASYNC | SND_RESOURCE);
}

// ---------------------------------------------------------
// CreateLadderTile
// ---------------------------------------------------------
void BitmapLevelLoader::CreateLadderTile(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    // Ladder için bir CustomBitmap kaynağınız olması lazım. Örneğin:
    //    resource.h:  #define IDB_LADDER 2014
    //    SpaceOut.rc: IDB_LADDER BITMAP "res\\ladder.bmp"
    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* egg = new Sprite(GameEngine::GetEngine()->_ladderBitmap, rcBounds, BA_WRAP);

    

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    GameEngine::GetEngine()->AddSprite(egg);
}

// ---------------------------------------------------------
// CreateEnemyAt
// ---------------------------------------------------------
void BitmapLevelLoader::CreateEnemyAt(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    // Enemy için de ayrı bir CustomBitmap kaynağı olmalı. Örnek:
    //    resource.h:  #define IDB_ENEMY  2015
    //    SpaceOut.rc: IDB_ENEMY BITMAP "res\\enemy.bmp"
    
    CustomBitmap* pEnemyBmp = new CustomBitmap(hDC, IDB_REDBLOCK, hInstance);
    
    
    Sprite* enemy = new Sprite(pEnemyBmp);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    enemy->SetPosition(pt);

    GameEngine::GetEngine()->AddSprite(enemy);
}



static void CreateTile(UINT uiResID, int pixelX, int pixelY, int tileSize, HDC hDC, HINSTANCE hInstance) {


    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);


    CustomBitmap* pEnemyBmp = new CustomBitmap(hDC, uiResID, hInstance);
    Sprite* enemy = new Sprite(pEnemyBmp);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    enemy->SetPosition(pt);

    GameEngine::GetEngine()->AddSprite(enemy);

}


void BitmapLevelLoader::CreateEgg(int pixelX, int pixelY, int tileSize)
{
    _eggsInLevel++; // Count the egg

    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* egg = new Sprite(GameEngine::GetEngine()->_eggBitmap, rcBounds, BA_WRAP);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    GameEngine::GetEngine()->AddSprite(egg);
}

void BitmapLevelLoader::CreateChaserEnemy(int pixelX, int pixelY, int tileSize)
{

    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    RECT rcBounds = { 0, 0, 600, 450 };
    AlienSprite* egg = new AlienSprite(GameEngine::GetEngine()->_chaserEnemyBitmap, rcBounds, BA_BOUNCE);
    egg->SetNumFrames(8);
    egg->SetChaser(true);


    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    // 4) Engine�e ekle
    GameEngine::GetEngine()->AddSprite(egg);
}

void BitmapLevelLoader::CreateGreenEnemy(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* egg = new Sprite(GameEngine::GetEngine()->_greenEnemyBitmap, rcBounds, BA_BOUNCE);
    egg->SetNumFrames(8);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1); // returns 0 or 1

    int a = 3;
    int sign = dist(gen) == 0 ? 1 : -1;
    int result = a * sign;
    egg->SetVelocity(result, 0);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    // 4) Engine�e ekle
    GameEngine::GetEngine()->AddSprite(egg);
}

void BitmapLevelLoader::CreateStalkerEnemy(int pixelX, int pixelY, int tileSize)
{

    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    RECT rcBounds = { 0, 0, 600, 450 };
    AlienSprite* egg = new AlienSprite(GameEngine::GetEngine()->_stalkerEnemyBitmap, rcBounds, BA_BOUNCE);
    egg->SetNumFrames(8);
    egg->SetStalker(true);


    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    // 4) Engine�e ekle
    GameEngine::GetEngine()->AddSprite(egg);
}



void BitmapLevelLoader::CreateInvisibleEdge(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* egg = new Sprite(GameEngine::GetEngine()->_invisivbleEdgeBitmap, rcBounds, BA_WRAP);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    // 4) Engine�e ekle
    GameEngine::GetEngine()->AddSprite(egg);
}


