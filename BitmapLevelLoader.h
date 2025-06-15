    #pragma once
    #include <windows.h>
    #include <objidl.h>
    #include <gdiplus.h>
    using namespace Gdiplus;

    // ÖNEMLÝ: SpaceOut projenizdeki GameEngine, Sprite, CustomBitmap gibi sýnýflarýnýzý kullanacaðýz.
    // Aþaðýda “CreateGroundTile”, “CreateLadderTile” vb. fonksiyonlarýn imzalarýný yazýyoruz.
    // Siz kendi projenizde bunlara karþýlýk gelen fonksiyonlarý tanýmlamalýsýnýz.

    /// <summary>
    /// LevelLoader, bir BMP (tilemap) dosyasýný okuyup pixele göre
    /// oyun sahnesine zemini (ground), merdiveni (ladder), düþmaný (enemy) vb. otomatik yerleþtirir.
    /// </summary>
    class BitmapLevelLoader
    {
    public:
        BitmapLevelLoader();
        ~BitmapLevelLoader();

    public:
        static HDC hDC; // GDI+ iþlemleri için HDC
        static HINSTANCE hInstance; // GDI+ iþlemleri için HINSTANCE

        static void InitGDIPlus();

        /// <summary>
        /// GDI+ kapatma iþlemi (oyun kapanýrken bir kez çaðýrýlmalý)
        /// </summary>
        static void ShutdownGDIPlus();



        /// <summary>
        /// Kaynak ID’sine göre BMP’i yükler ve tarama yaparak oyunu inþa eder.
        /// levelResourceID: resource.h’da tanýmlý IDB_LEVEL01 (örneðin 4000)
        /// tileSize: Her bir kap blok/pikselin dünya koordinatýnda kaç piksele denk geldiði (örneðin 16).
        /// </summary>
        static void GenerateLevelFromBitmap(int levelResourceID, int tileSize);

    private:
        /// <summary>
        /// Renk koduna göre ground (zemin) bloðunu oluþturur.
        /// </summary>
        static void CreateGroundTile(int pixelX, int pixelY, int tileSizee);

        /// <summary>
        /// Renk koduna göre ladder (merdiven) bloðunu oluþturur.
        /// </summary>
        static void CreateLadderTile(int pixelX, int pixelY, int tileSize);

        /// <summary>
        /// Renk koduna göre enemy (düþman) nesnesi oluþturur.
        /// </summary>
        static void CreateEnemyAt(int pixelX, int pixelY, int tileSize);
        static void CreateEgg(int pixelX, int pixelY, int tileSize);
        static void CreateInvisibleEdge(int pixelX, int pixelY, int tileSize);
        static void CreateGreenEnemy(int pixelX, int pixelY, int tileSize);
        static void CreateChaserEnemy(int pixelX, int pixelY, int tileSize);

        // Eðer ihtiyacýnýz varsa baþka tile türleri de ekleyin:
        // static void CreateWallTile(int pixelX, int pixelY, int tileSize);
        // static void CreateItemAt(int pixelX, int pixelY, int tileSize);
        // vb.
	    static void CreateTile(UINT szResourceName, int pixelX, int pixelY, int tileSize);
    };
