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
CustomBitmap*           _pPlayerBitmap;
CustomBitmap*           _pLgExplosionBitmap;
CustomBitmap*           _pGameOverBitmap;
CustomBitmap*			_chaserEnemyBitmap;

StarryBackground* _pBackground;
PlayerSprite*           _pPlayerSprite;
BOOL              _bGameOver;
INT     _iScore;
//-----------------------------------------------------------------
// Function Declarations
//-----------------------------------------------------------------
void NewGame();
void CollectEgg(Sprite* pEgg);
void LoadCurrentLevel();
