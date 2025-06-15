#include "BitmapLevelLoader.h"
#include "resource.h"       
#include "GameEngine.h"    
#include "Sprite.h"
#include "CustomBitmap.h"
#include <cassert>

#pragma comment(lib, "gdiplus.lib")

// GDI+ token
static ULONG_PTR gdiPlusToken = 0;
HDC BitmapLevelLoader::hDC = nullptr;
HINSTANCE BitmapLevelLoader::hInstance = nullptr;


// ---------------------------------------------------------
BitmapLevelLoader::BitmapLevelLoader() {}
BitmapLevelLoader::~BitmapLevelLoader() {}

// ---------------------------------------------------------
// GDI+ baþlatma ve kapatma
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
//  levelResourceID: resource.h içinde tanýmlý BMP ID’si (örneðin IDB_LEVEL01)
//  tileSize: her pixelin oyun dünyasýndaki gerçek geniþliði (ör.:16)
// ---------------------------------------------------------

void BitmapLevelLoader::GenerateLevelFromBitmap(int levelResourceID, int tileSize)
{
    // 1) Bellekteki resource’tan HBITMAP elde et
    HBITMAP hBmp = nullptr;
    {
        // Win32 LoadBitmap yerine GDI+ ile doðrudan kaynak ID’sinden yükleme:
        //  - Önce CustomBitmap::FromResource kullanacaðýz (GDI+). Alternatif olarak CreateDIBSection + LoadImage de kullanýlabilir.
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

    // 2) HBITMAP’i GDI+ Bitmap’e dönüþtür
    Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromHBITMAP(hBmp, nullptr);
    assert(bmp != nullptr);

    // 3) Boyutlarý oku (80×60 bekleniyor, ama dinamik olarak da çalýþýr)
    UINT width = bmp->GetWidth();
    UINT height = bmp->GetHeight();

    // 4) Her pikseli tarayýp renge göre nesne oluþtur
    for (UINT y = 0; y < height; ++y)
    {
        for (UINT x = 0; x < width; ++x)
        {
            Color pixelColor;
            bmp->GetPixel(x, y, &pixelColor);
            BYTE r = pixelColor.GetR();
            BYTE g = pixelColor.GetG();
            BYTE b = pixelColor.GetB();

            // Her piksel pozisyonunu tileSize ile dünya koordinatýna çeviriyoruz:
            // pixelXCoord = x * tileSize;  pixelYCoord = y * tileSize;
            if (r == 255 && g == 0 && b == 0)
            {
                // Kýrmýzý: Ground (Zemin) bloðu
                CreateGroundTile((int)x, (int)y, tileSize);
            }
            else if (r == 0 && g == 255 && b == 0)
            {
                // Yeþil: Ladder (Merdiven)
                CreateLadderTile((int)x, (int)y, tileSize);
            }
            else if (r == 0 && g == 0 && b == 255)
            {
                // Mavi: Enemy
                CreateEnemyAt((int)x, (int)y, tileSize);
            }
            else if (r == 0 && g == 60 && b == 255) {

                // Mavi: Tile end
                CreateTileEndMark((int)x, (int)y, tileSize);
            }
<<<<<<< Updated upstream
=======
            else if (r == 0 && g == 0 && b == 0) {
                CreateInvisibleEdge((int)x, (int)y, tileSize);
            }
            else
>>>>>>> Stashed changes
            {
                // Siyah veya baþka renkler: Boþ (hiçbir þey yapma)
                // ya da baþka bir renge göre baþka nesne eklemek isterseniz buraya ekleyin:
                // else if (r==255 && g==255 && b==0) { CreateGold((int)x,(int)y,tileSize); }
            }
        }
    }

    // 5) Belleði temizle
    delete bmp;
    DeleteObject(hBmp);
}

// ---------------------------------------------------------
// CreateGroundTile
//   pixelX, pixelY: tilemap üzerindeki koordinat (0..79, 0..59 gibi).
//   tileSize: her pikseli kaç piksel geniþliðe dönüþtüreceðiz (ör:16).
// ---------------------------------------------------------
void BitmapLevelLoader::CreateGroundTile(int pixelX, int pixelY, int tileSize)
{

    // 1) Dünya koordinatýný hesapla
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    // 2) Ground’ýn kendine ait bir Bitmap kaynaðý olmalý. Örneðin IDC_GROUND bitmap'i ek, resource.h’da:
    //    #define IDB_GROUND  2013
    //    SpaceOut.rc: IDB_GROUND BITMAP "res\\ground.bmp"
    // Burada, GroundSprite adýnda yeni bir sýnýf yoksa, Sprite sýnýfýný da kullanabilirsiniz:
    //
    CustomBitmap* pGroundBmp = new CustomBitmap(hDC, IDB_REDBLOCK, hInstance);
  
    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* ground = new Sprite(GameEngine::GetEngine()->_groundBitmap,rcBounds,BA_WRAP);

    // 3) Sprite’ýn pozisyonunu ayarla:
    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    ground->SetPosition(pt);


    // 4) Engine’e ekle
    GameEngine::GetEngine()->AddSprite(ground);
// 5) Eðer isterseniz, ground için bir ses de ekleyebilirsiniz:
// PlaySound((LPCSTR)IDW_GROUNDTILE, hInstance, SND_ASYNC | SND_RESOURCE);
}

// ---------------------------------------------------------
// CreateLadderTile
// ---------------------------------------------------------
void BitmapLevelLoader::CreateLadderTile(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    // Ladder için bir CustomBitmap kaynaðýnýz olmasý lazým. Örneðin:
    //    resource.h:  #define IDB_LADDER 2014
    //    SpaceOut.rc: IDB_LADDER BITMAP "res\\ladder.bmp"
    //
    
   
    CustomBitmap* pLadderBmp = new CustomBitmap(hDC, IDB_LADDER, hInstance);
    Sprite* ladder = new Sprite(pLadderBmp);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    ladder->SetPosition(pt);

    GameEngine::GetEngine()->AddSprite(ladder);
}



// ---------------------------------------------------------
// CreateEnemyAt
// ---------------------------------------------------------
void BitmapLevelLoader::CreateEnemyAt(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    // Enemy için de ayrý bir CustomBitmap kaynaðý olmalý. Örnek:
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


// ---------------------------------------------------------
// CreateEndMarkAt
// ---------------------------------------------------------
void BitmapLevelLoader::CreateTileEndMark(int pixelX, int pixelY, int tileSize) {

    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    //    resource.h:  #define IDB_ENDMARKER		2017
    //    SpaceOut.rc: IDB_ENEMY BITMAP "res\\enemy.bmp"

    CustomBitmap* pMarkerBmp = new CustomBitmap(hDC, IDB_TIMMY, hInstance);
    Sprite* marker = new Sprite(pMarkerBmp);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    marker->SetPosition(pt);

    GameEngine::GetEngine()->AddSprite(marker);

}
<<<<<<< Updated upstream
=======


void BitmapLevelLoader::CreateEgg(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* egg = new Sprite(GameEngine::GetEngine()->_eggBitmap, rcBounds, BA_WRAP);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    // 4) Engine’e ekle
    GameEngine::GetEngine()->AddSprite(egg);
}

void BitmapLevelLoader::CreateInvisibleEdge(int pixelX, int pixelY, int tileSize)
{
    float worldX = (float)(pixelX * tileSize);
    float worldY = (float)(pixelY * tileSize);

    RECT rcBounds = { 0, 0, 600, 450 };
    Sprite* egg = new Sprite(GameEngine::GetEngine()->_invisibleEdgeBitmap, rcBounds, BA_WRAP);

    POINT pt;
    pt.x = (LONG)worldX;
    pt.y = (LONG)worldY;
    egg->SetPosition(pt);

    // 4) Engine’e ekle
    GameEngine::GetEngine()->AddSprite(egg);
}


>>>>>>> Stashed changes
