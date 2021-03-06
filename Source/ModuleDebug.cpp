#include "ModuleDebug.h"

#include "ModuleInput.h"
#include "Application.h"
#include "Player.h"
#include "Tile.h"
#include "Obstacle.h"

#include <iostream>
using namespace std;

ModuleDebug::ModuleDebug()
{
	pauseIgnore = true;
}

ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Start()
{
	return true;
}

UpdateResult ModuleDebug::Update()
{
	// Toggle Fullscreen
	if (App->input->keys[SDL_SCANCODE_F] == KEY_DOWN)
	{
		App->FullScreenDesktop = !App->FullScreenDesktop;

		if (App->FullScreenDesktop)
		{
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			SDL_ShowCursor(SDL_DISABLE);
			App->ScreenSize = 1;
		}
		else
		{
			SDL_SetWindowFullscreen(App->window->window, 0);
			SDL_ShowCursor(SDL_ENABLE);
			App->ScreenSize = 3;
		}
	}

	GamePad& pad = App->input->pads[0];
	//Pause logic
	if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_DOWN || pad.start == KEY_DOWN)
	{
		PauseOnOff();
	}

	#pragma region Camera movement

	if(!App->isPaused)
	{
		if (App->input->keys[SDL_SCANCODE_F7] == KEY_DOWN)
		{
			debugCamera = !debugCamera;
			if (debugCamera)
			{
				cameraTempPos.x = App->render->camera.x;
				cameraTempPos.y = App->render->camera.y;
			}
			if (!debugCamera)
			{
				App->render->camera.y = cameraTempPos.x;
				App->render->camera.x = cameraTempPos.y;
			}
		}

		if (debugCamera)
		{
			// Handle positive vertical movement
			if (App->input->keys[SDL_SCANCODE_UP] == KEY_REPEAT) App->render->camera.y -= App->render->cameraSpeed;

			// Handle negative vertical movement
			if (App->input->keys[SDL_SCANCODE_DOWN] == KEY_REPEAT) App->render->camera.y += App->render->cameraSpeed;

			if (App->input->keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT) App->render->camera.x += App->render->cameraSpeed;

			if (App->input->keys[SDL_SCANCODE_LEFT] == KEY_REPEAT) App->render->camera.x -= App->render->cameraSpeed;
		}
	}

	#pragma endregion

	// Collision box
	if (App->input->keys[SDL_SCANCODE_F2] == KEY_DOWN)
	{
		App->debug->debugColBox = !App->debug->debugColBox;
	}

	if (App->isPaused)
	{
		ConstructMode();
	}

	#pragma region Escape Button

	if (App->input->keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)
	{
		return UpdateResult::UPDATE_STOP;
	}

	#pragma endregion

	return UpdateResult::UPDATE_CONTINUE;
}

UpdateResult ModuleDebug::PostUpdate()
{
	if(App->isPaused)
	{
		if (!createObject)
		{
			App->render->AddTextureRenderQueue(texPause, { 0 ,0 }, nullptr, 2, 100);

			App->render->AddTextureRenderQueue(texArrow, arrowPos[arrowPosPointer], nullptr, 2, 101);
		}
		else
		{
			SDL_Color renderColor;

			iPoint temp = currentTile->getTilePos(spawnPoint);
			temp.y--;
			int currentGrid = currentTile->LevelsTileMaps[App->scene->currentLevel][temp.y][temp.x];

			if (currentGrid == 0 || currentGrid == 4)
			{
				renderColor = { 0, 255, 0, 200 };
			}
			else
			{
				renderColor = { 255, 0, 0, 200 };
			}

			// Return button
			App->render->AddTextureRenderQueue(texPause, { 168 ,50 }, &returnButton, 3, 100);
			App->render->AddTextureRenderQueue(texPause, { 172 ,61 }, &enterButton, 3, 100);

			// Black bg
			App->render->AddRectRenderQueue({ App->render->camera.x, App->render->camera.y, SCREEN_WIDTH ,SCREEN_HEIGHT }, { 0, 0, 0, 191 });

			// Select pointer
			App->render->AddRectRenderQueue({ spawnPoint.x, spawnPoint.y, 16 , 16 }, renderColor);
		}
	}
	return UpdateResult::UPDATE_CONTINUE;
}

void ModuleDebug::InitDebug(Obstacle** obstacles, Tile* tile, Player* player)
{
	this->obstacles = obstacles;

	this->player = player;

	currentTile = tile;

	texArrow = App->textures->Load("Assets/Images/Sprites/My_Sprites/Arrow.png");
	texPause = App->textures->Load("Assets/Images/Sprites/My_Sprites/Pause.png");
	texPowerUp = App->textures->Load("Assets/Images/Sprites/PowerUps_Sprites/Powerups.png");

	createErrorSFX = App->audio->LoadSound("Assets/Audio/SFX/In_Game_Sounds/Miscellaneous_Sounds/G_CollisionWithBombInSound.wav");
	selectOptionSFX = App->audio->LoadSound("Assets/Audio/SFX/General_Sounds/MM_SelectSound.wav");

	pauseTimeOffset = 0;
}

void ModuleDebug::ConstructMode()
{
	GamePad& pad = App->input->pads[0];
	if(createObject)
	{
		if (App->input->keys[SDL_SCANCODE_LEFT] == KEY_DOWN || pad.left == KEY_DOWN)
		{
			spawnPoint.x -= 16;
			if (spawnPoint.x < currentTile->limitMin[App->scene->currentLevel].x)
			{
				spawnPoint.x = currentTile->limitMin[App->scene->currentLevel].x;
			}
		}
		if (App->input->keys[SDL_SCANCODE_RIGHT] == KEY_DOWN || pad.right == KEY_DOWN)
		{
			spawnPoint.x += 16;
			if (spawnPoint.x > currentTile->limitMax[App->scene->currentLevel].x)
			{
				spawnPoint.x = currentTile->limitMax[App->scene->currentLevel].x;
			}
		}
		if (App->input->keys[SDL_SCANCODE_UP] == KEY_DOWN || pad.up == KEY_DOWN)
		{
			spawnPoint.y -= 16;
			if (spawnPoint.y < currentTile->limitMin[App->scene->currentLevel].y)
			{
				spawnPoint.y = currentTile->limitMin[App->scene->currentLevel].y;
			}
		}
		if (App->input->keys[SDL_SCANCODE_DOWN] == KEY_DOWN || pad.down == KEY_DOWN)
		{
			spawnPoint.y += 16;
			if (spawnPoint.y > currentTile->limitMax[App->scene->currentLevel].y)
			{
				spawnPoint.y = currentTile->limitMax[App->scene->currentLevel].y;
			}
		}

		// Si esta en nivel 2, la camara se mueve con el cuadrado
		if (App->scene->currentScene->getID() == 5)
		{
			iPoint tempPos = { spawnPoint.x, 0 };

			App->render->CameraMove(tempPos);
		}
		
		if (App->input->keys[SDL_SCANCODE_BACKSPACE] == KEY_DOWN || pad.b == KEY_DOWN)
		{
			createObject = false;
		}
		if (App->input->keys[SDL_SCANCODE_RETURN] == KEY_DOWN || pad.a == KEY_DOWN)
		{
			iPoint temp = currentTile->getTilePos(spawnPoint);
			temp.y--;
			int currentGrid = currentTile->LevelsTileMaps[App->scene->currentLevel][temp.y][temp.x];

			if (currentGrid == 0 || currentGrid == 4)
			{
				if(App->scene->currentScene->Spawn(spawnPoint, arrowPosPointer))
				{
					PauseOnOff();
					App->audio->PlaySoundA(selectOptionSFX, 0);
				}
				else
				{
					cout << "Create Error" << endl;
					App->audio->PlaySoundA(createErrorSFX, 0);
				}
			}
			else
			{
				cout << "Create Error" << endl;
				App->audio->PlaySoundA(createErrorSFX, 0);
			}
		}
	}
	else
	{
		if (App->input->keys[SDL_SCANCODE_LEFT] == KEY_DOWN || pad.left == KEY_DOWN)
		{
			if (arrowPosPointer <= 0) arrowPosPointer = 2;

			else arrowPosPointer--;
		}

		if (App->input->keys[SDL_SCANCODE_RIGHT] == KEY_DOWN || pad.right == KEY_DOWN)
		{
			if (arrowPosPointer >= 2) arrowPosPointer = 0;

			else arrowPosPointer++;
		}

		if (App->input->keys[SDL_SCANCODE_RETURN] == KEY_DOWN || pad.a == KEY_DOWN)
		{
			createObject = true;
			spawnPoint = player->getCurrentTileWorldPos();
			//spawnPoint = currentTile->getWorldPos({ 1, 2 });
		}

		// temp debug
		if (App->input->keys[SDL_SCANCODE_H] == KEY_DOWN)
		{
			cout << "x: " << spawnPoint.x << endl;
			cout << "y: " << spawnPoint.y << endl;
		}
	}
}

void ModuleDebug::CalPauseTimeOffset()
{
	if (App->isPaused)
	{
		pauseTimeOffset = (SDL_GetTicks() * 0.001f) - pauseTimeOffset;
	}
	else
	{
		pauseTimeOffset = (SDL_GetTicks() * 0.001f) - pauseTimeOffset;
	}
}

void ModuleDebug::AddUpFlame()
{
	App->scene->playerSettings->powerUpFlame++;
}

void ModuleDebug::PowerUpPosition()
{
	debugPowerUpPosition = !debugPowerUpPosition;
}

void ModuleDebug::PlayerGodMod()
{
	player->godMode = !player->godMode;
}

void ModuleDebug::PlayerPosInConsole()
{
	player->posMode = !player->posMode;

	if (player->posMode)
	{
		player->lastTilePos = player->getCurrentTilePos();
		player->tileMap->LevelsTileMaps[App->scene->currentLevel][player->lastTilePos.y - 1][player->lastTilePos.x] = -1;
	}
	else
	{
		player->tileMap->LevelsTileMaps[App->scene->currentLevel][player->tilePos.y - 1][player->tilePos.x] = 0;
	}
}

void ModuleDebug::DrawMapInConsole(Tile* tile, int gridX, int gridY)
{
	system("cls");
	cout << endl;

	for (int i = 0, k = 0; i < gridY; ++i)
	{
		for (int j = 0; j < gridX; ++j)
		{
			switch (tile->LevelsTileMaps[App->scene->currentLevel][i][j])
			{
			case -1: cout << "P,"; break;
			case 10: cout << "G,"; break;
			default: cout << tile->LevelsTileMaps[App->scene->currentLevel][i][j] << ","; break;
			}
		}
		cout << endl;
	}
}

void ModuleDebug::PrintDebugInformation()
{
	cout << endl;
	cout << "F1: On/Off GodMod" << endl;
	cout << "F2: On/Off Collision box" << endl;
	cout << "F3: Instant lose" << endl;
	cout << "F4: Instant win" << endl;
	cout << "F5: On/Off PowerUp position" << endl;
	cout << "F6: On/Off Mover A* path" << endl;
	cout << "F7: On/Off Camera (move with dirArrown)" << endl;
	cout << "F10: On/Off Draw player pos in console map (use with Q)" << endl;
	cout << "Q: Update console tileMap" << endl;
	cout << "Z: bomb flame powerUp" << endl;
}

void ModuleDebug::Win(iPoint winPos, int cameraX)
{
	for (int i = 0; i < SCENE_OBSTACLES_NUM; ++i)
	{
		if (obstacles[i] != nullptr && obstacles[i]->CompareName("CoreMecha"))
		{
			obstacles[i]->Die();
		}
	}
	player->position = winPos;
	App->render->CameraMove({ cameraX,0 });
}

void ModuleDebug::PauseOnOff()
{
	#pragma region Quit Other debug

	debugColBox = false;

	if(debugCamera)
	{
		debugCamera = false;
		App->render->camera.y = cameraTempPos.y;
		App->render->camera.x = cameraTempPos.x;
	}
	
	#pragma endregion

	// Just can be pause in gameScene
	int sceneID = App->scene->currentScene->getID();

	if (sceneID == 4 || sceneID == 5 || sceneID == 6)
	{
		App->isPaused = !App->isPaused;

		createObject = false;

		CalPauseTimeOffset();
	}
}

void ModuleDebug::GameOver()
{
	App->scene->ChangeCurrentScene(SCENE_GAMEOVER, 90);
}

void ModuleDebug::DrawPowerUpPosition()
{
	// Draw powerUpPos
	for (int i = 0; i < SCENE_OBSTACLES_NUM; i++)
	{
		if (debugPowerUpPosition == true && obstacles[i] != nullptr && obstacles[i]->powerUp != 0)
		{
			App->render->AddRectRenderQueue({ obstacles[i]->getPosition().x + 2,obstacles[i]->getPosition().y + 2,12,12 }, { 0,0,255,255 });
		}
	}
}
