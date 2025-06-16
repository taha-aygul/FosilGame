//-----------------------------------------------------------------
// Space Out Application
// C++ Source - SpaceOut.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "SpaceOut.h"
#include "CustomBitmap.h"
#include "BitmapLevelLoader.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
  // Create the game engine

  _pGame = new GameEngine(hInstance, TEXT("Space Out"),
    TEXT("Space Out"), IDI_SPACEOUT, IDI_SPACEOUT_SM, 600, 450);
  if (_pGame == NULL)
    return FALSE;

  // Set the frame rate
  _pGame->SetFrameRate(30);

  // Store the instance handle
  _hInstance = hInstance;

  return TRUE;
}

void GameStart(HWND hWindow)
{
  // Seed the random number generator
  srand(GetTickCount());

  // Initialize GDI+ for bitmap loading
  HDC hDC = GetDC(hWindow);
  BitmapLevelLoader::InitGDIPlus();
  BitmapLevelLoader::hDC = hDC;
  BitmapLevelLoader::hInstance = _hInstance;
  // Create the offscreen device context and bitmap
  _hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
  _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
    _pGame->GetWidth(), _pGame->GetHeight());
  SelectObject(_hOffscreenDC, _hOffscreenBitmap);

  // Create and load the bitmaps
  _pDesertBitmap = new CustomBitmap(hDC, IDB_DESERT, _hInstance);
  _pCarBitmap = new CustomBitmap(hDC, IDB_LADDER, _hInstance);
  _pSmCarBitmap = new CustomBitmap(hDC, IDB_SMCAR, _hInstance);
  _pMissileBitmap = new CustomBitmap(hDC, IDB_MISSILE, _hInstance);
  _pBlobboBitmap = new CustomBitmap(hDC, IDB_BLOBBO, _hInstance);
  _pBMissileBitmap = new CustomBitmap(hDC, IDB_BMISSILE, _hInstance);
  _pJellyBitmap = new CustomBitmap(hDC, IDB_JELLY, _hInstance);
  _pJMissileBitmap = new CustomBitmap(hDC, IDB_JMISSILE, _hInstance);
  _pTimmyBitmap = new CustomBitmap(hDC, IDB_TIMMY, _hInstance);
  _pTMissileBitmap = new CustomBitmap(hDC, IDB_TMISSILE, _hInstance);
  _pSmExplosionBitmap = new CustomBitmap(hDC, IDB_SMEXPLOSION, _hInstance);
  _pLgExplosionBitmap = new CustomBitmap(hDC, IDB_LGEXPLOSION, _hInstance);
  _pGameOverBitmap = new CustomBitmap(hDC, IDB_GAMEOVER, _hInstance);

  _pGame->_groundBitmap = new CustomBitmap(hDC, IDB_REDBLOCK, _hInstance);
  _pGame->_eggBitmap = new CustomBitmap(hDC, IDB_EGG, _hInstance);
  _pGame->_ladderBitmap = new CustomBitmap(hDC, IDB_GREENBLOCK, _hInstance);
  _pGame-> _invisivbleEdgeBitmap = new CustomBitmap(hDC, IDB_INVISIBLEEDGE, _hInstance);
  _pGame->_greenEnemyBitmap = new CustomBitmap(hDC, IDB_BLOBBO, _hInstance);
  _pGame->_chaserEnemyBitmap = new CustomBitmap(hDC, IDB_JELLY, _hInstance);
  _pGame->_stalkerEnemyBitmap = new CustomBitmap(hDC, IDB_TIMMY, _hInstance);

  // Create the starry background
  _pBackground = new StarryBackground(600, 450);

  // Play the background music
  //_pGame->PlayMIDISong(TEXT("Music.mid"));

  // Start the game
  NewGame();
}

int _currentLevel = 1;
const int _maxLevel = 3; // Set this to your total number of levels
int _eggsCollected = 0;
int _eggsInLevel = 0; // Number of eggs in the current level

void NewGame()
{
    _currentLevel = 1;

    // Clear the sprites
    _pGame->CleanupSprites();

	BitmapLevelLoader::GenerateLevelFromBitmap(IDB_LEVEL01, 10);

    // Create the car sprite
    RECT rcBounds = { 0, 0, 600, 450 };
    _pCarSprite = new PlayerSprite(_pCarBitmap, rcBounds, BA_BOUNCE);
    _pCarSprite->SetPosition(200, 200);
    _pGame->AddSprite(_pCarSprite);
	_pCarSprite->SetNumFrames(9, FALSE, TRUE); // Set number of frames and flippable

	

    // Initialize the game variables
    _iFireInputDelay = 0;
    _iScore = 0;
    _iNumLives = 3;
    _iDifficulty = 80;
    _bGameOver = FALSE;

    // Play the background music
    _pGame->PlayMIDISong();
}

void GameEnd()
{
  // Close the MIDI player for the background music
  _pGame->CloseMIDIPlayer();
  // Shutdown GDI+
  BitmapLevelLoader::ShutdownGDIPlus();

  // Cleanup the offscreen device context and bitmap
  DeleteObject(_hOffscreenBitmap);
  DeleteDC(_hOffscreenDC);  

  // Cleanup the bitmaps
  delete _pDesertBitmap;
  delete _pCarBitmap;
  delete _pSmCarBitmap;
  delete _pMissileBitmap;
  delete _pBlobboBitmap;
  delete _pBMissileBitmap;
  delete _pJellyBitmap;
  delete _pJMissileBitmap;
  delete _pTimmyBitmap;
  delete _pTMissileBitmap;
  delete _pSmExplosionBitmap;
  delete _pLgExplosionBitmap;
  delete _pGameOverBitmap;
  delete _pGame->_groundBitmap;
  delete _pGame->_eggBitmap;
  delete _pGame->_invisivbleEdgeBitmap;
  delete _pGame->_ladderBitmap;
  delete _pGame->_greenEnemyBitmap;
  delete _pGame->_chaserEnemyBitmap;
  // Cleanup the background
  delete _pBackground;

  // Cleanup the sprites
  _pGame->CleanupSprites();

  // Cleanup the game engine
  delete _pGame;
}

void GameActivate(HWND hWindow)
{
  // Resume the background music
  _pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
  // Pause the background music
  _pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
    // Draw the background
    _pBackground->Draw(hDC);

    // Draw the desert bitmap
    _pDesertBitmap->Draw(hDC, 0, 371);

    // Draw the sprites
    _pGame->DrawSprites(hDC);

    // Draw the egg icon next to the eggs collected/total text
    int eggIconX = 10;
    int eggIconY = 10;
    int eggIconWidth = _pGame->_eggBitmap->GetWidth();
    int eggIconHeight = _pGame->_eggBitmap->GetHeight();
    _pGame->_eggBitmap->Draw(hDC, eggIconX, eggIconY, TRUE);

    // Draw the eggs collected / total eggs text next to the icon
    TCHAR szEggs[64];
    RECT rectEggs = { eggIconX + eggIconWidth + 8, eggIconY, eggIconX + eggIconWidth + 120, eggIconY + eggIconHeight };
    wsprintf(szEggs, "%d / %d", _eggsCollected, _eggsInLevel);
    SetBkMode(hDC, TRANSPARENT);
    SetTextColor(hDC, RGB(255, 255, 255));
    DrawText(hDC, szEggs, -1, &rectEggs, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

    // Draw the number of remaining lives (cars)
    // for (int i = 0; i < _iNumLives; i++)
    //   _pSmCarBitmap->Draw(hDC, 520 + (_pSmCarBitmap->GetWidth() * i), 10, TRUE);

    // Draw the game over message, if necessary
    if (_bGameOver)
        _pGameOverBitmap->Draw(hDC, 190, 149, TRUE);
}

void NextLevel()
{
    _currentLevel++;
    if (_currentLevel > _maxLevel)
    {
        // Game completed, show game over or victory screen
        _bGameOver = TRUE;
        // Optionally: Show a "You Win" bitmap or message
        return;
    }

    LoadCurrentLevel();

    // Reset or update other variables as needed
}
void LoadCurrentLevel() {
    // Reset for new level
    _eggsCollected = 0;
    _eggsInLevel = 0;

    // Clear sprites and load the next level bitmap
    _pGame->CleanupSprites();

    // Example: Level bitmap resource IDs are sequential (IDB_LEVEL01, IDB_LEVEL02, ...)
    int nextLevelResId = IDB_LEVEL01 + (_currentLevel - 1);

    BitmapLevelLoader::GenerateLevelFromBitmap(nextLevelResId, 10);

    // Re-create player sprite, reset variables, etc.
    RECT rcBounds = { 0, 0, 600, 450 };
    _pCarSprite = new PlayerSprite(_pCarBitmap, rcBounds, BA_WRAP);
    _pCarSprite->SetPosition(10, 10);
	_pCarSprite->SetNumFrames(9, FALSE, TRUE); // Set number of frames and flippable
    _pGame->AddSprite(_pCarSprite);

    _bGameOver = false;
}



bool AllEggsCollected()
{
    return _eggsCollected >= _eggsInLevel;
}

    

void GameCycle()
{
    if (_bGameOver) {


        LoadCurrentLevel();
        return;
    }


  if (!_bGameOver)
  {
          // Check for level completion (example: all eggs collected)
    if (AllEggsCollected()) // Implement this function as needed
    {
      NextLevel();
      return;
    }

    // Randomly add aliens
    if ((rand() % _iDifficulty) == 0)
      //AddAlien();

    // Update the background
    _pBackground->Update();

    // Update the sprites
    //_pCarSprite->isOnGround = false;

    _pGame->UpdateSprites();

    // Obtain a device context for repainting the game
    HWND  hWindow = _pGame->GetWindow();
    HDC   hDC = GetDC(hWindow);

    // Paint the game to the offscreen device context
    GamePaint(_hOffscreenDC);

    // Blit the offscreen bitmap to the game screen
    BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
      _hOffscreenDC, 0, 0, SRCCOPY);

    // Cleanup
    ReleaseDC(hWindow, hDC);
  }
}

void HandleKeys()
{
  if (!_bGameOver)
  {
    // Move the car based upon left/right key presses
    POINT ptVelocity = _pCarSprite->GetVelocity();

    //  DAMPING EKLEND�
    const float dampingFactor = 0.85f;   // daha çok yavaşlatır
    const float maxSpeed = 4.0f;         // daha düşük hız

    if (GetAsyncKeyState(VK_LEFT) < 0)
    {
      ptVelocity.x = max(ptVelocity.x - 1, -maxSpeed);
    }
    else if (GetAsyncKeyState(VK_RIGHT) < 0)
    {
      // Move right
      ptVelocity.x = min(ptVelocity.x + 2, maxSpeed);
    }
    else
    {
        // Stop the car
        ptVelocity.x = static_cast<int>(ptVelocity.x * dampingFactor);
        // K���k h�zlar� s�f�ra �ek (s�r�nmesin)
        if (abs(ptVelocity.x) < 1) ptVelocity.x = 0;
    }
      // --- Zıplama (Space tuşu ile) ---
    if (GetAsyncKeyState(VK_SPACE) < 0)
    {
      _pCarSprite->HandleKeyDown(VK_SPACE);  // Yerçekimine göre zıplama
    }
  

    if (GetAsyncKeyState(VK_UP) < 0 && _pCarSprite->isCollidingWithLadder)
    {
        _pCarSprite->velocityY = max(ptVelocity.y - 1, -maxSpeed); // Merdivende yukarı çıkma hızı
    }
    else if (GetAsyncKeyState(VK_DOWN) < 0 && _pCarSprite->isCollidingWithLadder)
    {
        _pCarSprite->velocityY = min(ptVelocity.y + 1, maxSpeed); // Merdivende yukarı çıkma hızı
    }
    else
    {
        // Stop the car
        ptVelocity.y = static_cast<int>(ptVelocity.y * dampingFactor);
        if (abs(ptVelocity.y) < 1) ptVelocity.y = 0;
    }
     
    _pCarSprite->SetVelocity(ptVelocity);


   
  }

  // Start a new game based upon an Enter (Return) key press
  if (_bGameOver && (GetAsyncKeyState(VK_RETURN) < 0))
    // Start a new game
    NewGame();
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    



    CustomBitmap* _groundBitmap = GameEngine::GetEngine()->_eggBitmap;
    CustomBitmap* _ladderBitmap = GameEngine::GetEngine()->_ladderBitmap;
    CustomBitmap* _invisivbleEdgeBitmap = GameEngine::GetEngine()->_invisivbleEdgeBitmap;
    CustomBitmap* _greenEnemyBitmap = GameEngine::GetEngine()->_greenEnemyBitmap;
    CustomBitmap* _stalkerEnemyBitmap = GameEngine::GetEngine()->_stalkerEnemyBitmap;

  // See if a player missile and an alien have collided
    CustomBitmap* pHitter = pSpriteHitter->GetBitmap();
    CustomBitmap* pHittee = pSpriteHittee->GetBitmap();

    

    if ((pHitter == _invisivbleEdgeBitmap && (pHittee == _greenEnemyBitmap))) {
        pSpriteHittee->SetVelocity(-pSpriteHittee->GetVelocity().x, 0);
    }
    if (pHittee == _invisivbleEdgeBitmap && (pHitter == _greenEnemyBitmap)) {
        pSpriteHitter->SetVelocity(-pSpriteHitter->GetVelocity().x, 0);
    }
    if ((pHitter == _invisivbleEdgeBitmap && (pHittee == _stalkerEnemyBitmap))) {
        RECT rcPlayer = _pCarSprite->GetPosition();
        RECT rcAlien = pSpriteHittee->GetPosition();
        RECT rcinvisibleEdge = pSpriteHitter->GetPosition();
        // Merkezden merkeze uzaklığı hesapla
        int dx = (rcPlayer.left + _pCarSprite->GetWidth() / 2) - (rcAlien.left + pSpriteHittee->GetWidth() / 2);
        int dx1 = (rcinvisibleEdge.left + pSpriteHitter->GetWidth() / 2) - (rcAlien.left + pSpriteHittee->GetWidth() / 2);
        if (dx * dx1 >= 0)
        {
            // dx ve dy aynı işarete sahip (veya en az biri sıfır).
            // Örneğin, her ikisi de kuzeydoğu veya güneybatı yönüne gidiyor.
            pSpriteHittee->SetVelocity(0, 0);
        }
        

        
    }
    if (pHittee == _invisivbleEdgeBitmap && (pHitter == _stalkerEnemyBitmap)) {
        RECT rcPlayer = _pCarSprite->GetPosition();
        RECT rcAlien = pSpriteHitter->GetPosition();
        RECT rcinvisibleEdge = pSpriteHittee->GetPosition();
        // Merkezden merkeze uzaklığı hesapla
        int dx = (rcPlayer.left + _pCarSprite->GetWidth() / 2) - (rcAlien.left + pSpriteHitter->GetWidth() / 2);
        int dx1 = (rcinvisibleEdge.left + pSpriteHittee->GetWidth() / 2) - (rcAlien.left + pSpriteHitter->GetWidth() / 2);
        if (dx * dx1 >= 0)
        {
            // dx ve dy aynı işarete sahip (veya en az biri sıfır).
            // Örneğin, her ikisi de kuzeydoğu veya güneybatı yönüne gidiyor.
            pSpriteHitter->SetVelocity(0, 0);
        }
    }

    if (pHitter == _pCarBitmap) {
        if (pHittee == _pGame->_greenEnemyBitmap || pHittee == _pGame->_chaserEnemyBitmap || pHittee == _pGame->_stalkerEnemyBitmap)
        {
            PlaySound((LPCSTR)IDW_GAMEOVER, _hInstance, SND_ASYNC |
                SND_RESOURCE);
            _bGameOver = true;
        }
    }
  if (pHittee == _pCarBitmap) {


      if (pHitter == _pGame->_greenEnemyBitmap || pHitter == _pGame->_chaserEnemyBitmap || pHitter == _pGame->_stalkerEnemyBitmap)
      {
          PlaySound((LPCSTR)IDW_GAMEOVER, _hInstance, SND_ASYNC |
              SND_RESOURCE);
          _bGameOver = true;
      }


      if (pHitter == _pGame->_ladderBitmap)
      {
          _pCarSprite->isCollidingWithLadder = true;
      }

      if (pHitter == _pGame->_eggBitmap)
      {
          CollectEgg(pSpriteHitter);
      }

      if (pHitter == _pGame->_groundBitmap)
      {
          //_pCarSprite->isOnGround = true;

          RECT rcPlayer = _pCarSprite->GetPosition();
          RECT rcGround = pSpriteHitter->GetPosition();
          POINT ptVelocity = _pCarSprite->GetVelocity();

          // Use a small tolerance value. This allows the check to succeed even if
          // the player has slightly passed through the ground's surface due to its velocity.
          const int tolerance = 10;

          // Check two conditions:
          // 1. The player is moving downwards (or is stationary vertically). This prevents
          //    the check from triggering while jumping up through a platform from below.
          // 2. The player's bottom edge is at or slightly below the ground's top edge.
          if (ptVelocity.y >= 0
              && rcPlayer.bottom <= rcGround.top + tolerance
              && rcPlayer.bottom >= rcGround.top - tolerance
              )
          {
              // Set the flag to indicate the player is on the ground.
              // You can now use this flag in your player control logic (e.g., to allow jumping).
              _pCarSprite->isOnGround = true; if (pHitter == _pGame->_ladderBitmap)
              {
                  _pCarSprite->isCollidingWithLadder = true;
              }

              // To prevent the player from sinking into the ground, it's good practice
              // to "snap" their position so they sit exactly on top of the ground.
              _pCarSprite->SetPosition(rcPlayer.left, rcGround.top - _pCarSprite->GetHeight() + 4);

          }
      }
  }
 

  return FALSE;
}

void SpriteDying(Sprite* pSprite)
{
  // See if an alien missile sprite is dying
  if (pSprite->GetBitmap() == _pBMissileBitmap ||
    pSprite->GetBitmap() == _pJMissileBitmap ||
    pSprite->GetBitmap() == _pTMissileBitmap)
  {
    // Play the small explosion sound
    PlaySound((LPCSTR)IDW_SMEXPLODE, _hInstance, SND_ASYNC |
      SND_RESOURCE | SND_NOSTOP);

    // Create a small explosion sprite at the missile's position
    RECT rcBounds = { 0, 0, 600, 450 };
    RECT rcPos = pSprite->GetPosition();
    Sprite* pSprite = new Sprite(_pSmExplosionBitmap, rcBounds);
    pSprite->SetNumFrames(8, TRUE);
    pSprite->SetPosition(rcPos.left, rcPos.top);
    _pGame->AddSprite(pSprite);
  }
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------


void AddAlien()
{
  // Create a new random alien sprite
  RECT          rcBounds = { 0, 0, 600, 410 };
  AlienSprite*  pSprite;
  
  // Rastgele alien türü seç (3 tür artık)
  switch(rand() % 3)  
  {
  case 0:
    // Blobbo 
    pSprite = new AlienSprite(_pBlobboBitmap, rcBounds, BA_BOUNCE);
    pSprite->SetNumFrames(8);
    pSprite->SetPosition(rand() % 600, rand() % 370);
    pSprite->SetVelocity((rand() % 5) - 2, (rand() % 5) + 3);
    break;
  case 1:
    // Chaser (Takipçi) - Jelly bitmap'ini kullanıyoruz
    pSprite = new AlienSprite(_pJellyBitmap, rcBounds, BA_BOUNCE);
    pSprite->SetNumFrames(8);
    pSprite->SetPosition(rand() % 600, rand() % 370);
    pSprite->SetVelocity(0, 0);  // Başlangıç hızı 0
    pSprite->SetChaser(true);    // Takipçi olarak işaretle
    break;
  case 2:
    // Timmy
    pSprite = new AlienSprite(_pTimmyBitmap, rcBounds, BA_WRAP);
    pSprite->SetNumFrames(8);
    pSprite->SetPosition(rand() % 600, rand() % 370);
    pSprite->SetVelocity((rand() % 7) + 3, 0);
    break;
  }

  // Add the alien sprite
  _pGame->AddSprite(pSprite);
}

void CollectEgg(Sprite* pEgg)
{
	pEgg->Kill();
	_iScore += 1;
	_iDifficulty = max(80 - (_iScore / 20), 20);

    _eggsCollected++; // Increment collected eggs

    RECT rcBounds = { 0, 0, 600, 450 };
    RECT rcPos;
        rcPos = pEgg->GetPosition();
    Sprite* pSprite = new Sprite(_pLgExplosionBitmap, rcBounds);
    pSprite->SetNumFrames(8, TRUE);
    pSprite->SetPosition(rcPos.left, rcPos.top);
    _pGame->AddSprite(pSprite);
}

