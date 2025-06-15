//-----------------------------------------------------------------
// Space Out Application
// C++ Header - SpaceOut.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "CustomBitmap.h"
#include "Sprite.h"
#include "Background.h"
#include "AlienSprite.h"
#include "PlayerSprite.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE         _hInstance;
GameEngine*       _pGame;
HDC               _hOffscreenDC;
HBITMAP           _hOffscreenBitmap;
CustomBitmap*           _pDesertBitmap;
CustomBitmap*           _pCarBitmap;
CustomBitmap*           _pSmCarBitmap;
CustomBitmap*           _pMissileBitmap;
CustomBitmap*           _pBlobboBitmap;
CustomBitmap*           _pBMissileBitmap;
CustomBitmap*           _pJellyBitmap;
CustomBitmap*           _pJMissileBitmap;
CustomBitmap*           _pTimmyBitmap;
CustomBitmap*           _pTMissileBitmap;
CustomBitmap*           _pSmExplosionBitmap;
CustomBitmap*           _pLgExplosionBitmap;
CustomBitmap*           _pGameOverBitmap;

CustomBitmap* deneme2Bitmap;

StarryBackground* _pBackground;
PlayerSprite*           _pCarSprite;
int               _iFireInputDelay;
int               _iNumLives, _iScore, _iDifficulty;
BOOL              _bGameOver;

//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void NewGame();
void AddAlien();
void CollectEgg(Sprite* pEgg);
