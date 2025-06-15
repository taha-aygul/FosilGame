    #pragma once
    #include <windows.h>
    #include <objidl.h>
    #include <gdiplus.h>
    using namespace Gdiplus;

    // �NEML�: SpaceOut projenizdeki GameEngine, Sprite, CustomBitmap gibi s�n�flar�n�z� kullanaca��z.
    // A�a��da �CreateGroundTile�, �CreateLadderTile� vb. fonksiyonlar�n imzalar�n� yaz�yoruz.
    // Siz kendi projenizde bunlara kar��l�k gelen fonksiyonlar� tan�mlamal�s�n�z.

    /// <summary>
    /// LevelLoader, bir BMP (tilemap) dosyas�n� okuyup pixele g�re
    /// oyun sahnesine zemini (ground), merdiveni (ladder), d��man� (enemy) vb. otomatik yerle�tirir.
    /// </summary>
    class BitmapLevelLoader
    {
    public:
        BitmapLevelLoader();
        ~BitmapLevelLoader();

    public:
        static HDC hDC; // GDI+ i�lemleri i�in HDC
        static HINSTANCE hInstance; // GDI+ i�lemleri i�in HINSTANCE

        static void InitGDIPlus();

        /// <summary>
        /// GDI+ kapatma i�lemi (oyun kapan�rken bir kez �a��r�lmal�)
        /// </summary>
        static void ShutdownGDIPlus();



        /// <summary>
        /// Kaynak ID�sine g�re BMP�i y�kler ve tarama yaparak oyunu in�a eder.
        /// levelResourceID: resource.h�da tan�ml� IDB_LEVEL01 (�rne�in 4000)
        /// tileSize: Her bir kap blok/pikselin d�nya koordinat�nda ka� piksele denk geldi�i (�rne�in 16).
        /// </summary>
        static void GenerateLevelFromBitmap(int levelResourceID, int tileSize);

    private:
        /// <summary>
        /// Renk koduna g�re ground (zemin) blo�unu olu�turur.
        /// </summary>
        static void CreateGroundTile(int pixelX, int pixelY, int tileSizee);

        /// <summary>
        /// Renk koduna g�re ladder (merdiven) blo�unu olu�turur.
        /// </summary>
        static void CreateLadderTile(int pixelX, int pixelY, int tileSize);

        /// <summary>
        /// Renk koduna g�re enemy (d��man) nesnesi olu�turur.
        /// </summary>
        static void CreateEnemyAt(int pixelX, int pixelY, int tileSize);
        static void CreateEgg(int pixelX, int pixelY, int tileSize);
        static void CreateInvisibleEdge(int pixelX, int pixelY, int tileSize);
        static void CreateGreenEnemy(int pixelX, int pixelY, int tileSize);
        static void CreateChaserEnemy(int pixelX, int pixelY, int tileSize);

        // E�er ihtiyac�n�z varsa ba�ka tile t�rleri de ekleyin:
        // static void CreateWallTile(int pixelX, int pixelY, int tileSize);
        // static void CreateItemAt(int pixelX, int pixelY, int tileSize);
        // vb.
	    static void CreateTile(UINT szResourceName, int pixelX, int pixelY, int tileSize);
    };
