#include "Bomb.h"

#include <iostream>
using namespace std;

Bomb::Bomb()
{
}

Bomb::Bomb(Player* player, SDL_Texture* tex, Tile* tile, uint explosionSFX)
:Obstacle({ player->getCurrentTileWorldPos().x, player->getCurrentTileWorldPos().y, 16, 16 }, true, App->collisions->AddCollider({ player->getCurrentTileWorldPos().x, player->getCurrentTileWorldPos().y, 16, 16 }, Type::BOMB, App->scene), tex)
{
	name = "Bomb";

 	this->player = player;

	lv1Tile = tile;

	myTilePos = lv1Tile->getTilePos(getPosition());

	myTilePos.y--;

	lv1Tile->LevelsTileMaps[App->scene->currentLevel][myTilePos.y][myTilePos.x] = 11;

	#pragma region Init explotion particle

	explosionCenter.InitParticle(500.0f, 0.4f, tex);
	explosionMiddle.InitParticle(500.0f, 0.4f, tex);
	explosionEnd.InitParticle(500.0f, 0.4f, tex);

	// ExplosionCenter particle
	explosionCenter.anim.PushBack({ 21, 2, 16, 16 });
	explosionCenter.anim.PushBack({ 21, 21, 16, 16 });
	explosionCenter.anim.PushBack({ 21, 40, 16, 16 });
	explosionCenter.anim.PushBack({ 21, 21, 16, 16 });
	explosionCenter.anim.PushBack({ 21, 2, 16, 16 });

	// ExplosionMiddle particle
	explosionMiddle.anim.PushBack({ 42, 2, 16, 16 });
	explosionMiddle.anim.PushBack({ 42, 21, 16, 16 });
	explosionMiddle.anim.PushBack({ 42, 40, 16, 16 });
	explosionMiddle.anim.PushBack({ 42, 21, 16, 16 });
	explosionMiddle.anim.PushBack({ 42, 2, 16, 16 });

	// ExplosionEnd particle
	explosionEnd.anim.PushBack({ 62, 2, 16, 16 });
	explosionEnd.anim.PushBack({ 62, 21, 16, 16 });
	explosionEnd.anim.PushBack({ 62, 40, 16, 16 });
	explosionEnd.anim.PushBack({ 62, 21, 16, 16 });
	explosionEnd.anim.PushBack({ 62, 2, 16, 16 });

	#pragma endregion

	// Inicializar animacion prestablecida de la bomba
	#pragma region Init bomb anim

	defaultAnim.hasIdle = false;
	defaultAnim.speed = 0.07f;
	defaultAnim.loop = true;
	defaultAnim.PushBack({ 1,1,16,16 });  //small
	defaultAnim.PushBack({ 1,21,16,16 }); //midle
	defaultAnim.PushBack({ 1,39,16,16 }); //big
	defaultAnim.PushBack({ 1,21,16,16 }); //midle

	#pragma endregion

	// Assignar anamacion prestablecida a currentAnim
	currentAnim = &defaultAnim;

	// Init explotionRange
	explotionRange += App->scene->playerSettings->powerUpFlame;

	// Init TimeCount
	startCountTime = SDL_GetTicks() - (App->debug->pauseTimeOffset * 1000);

	// Init sounds
	this->explosionSFX = explosionSFX;
}

Bomb::~Bomb()
{
	
}

void Bomb::Update()
{
	//ColUpdate();
	currentAnim->Update();

	// Count down
	if (!pendingToDelete)
	{
		// Cuenta Atras para que la bomba se explota
		float currentCountTime = SDL_GetTicks() - (App->debug->pauseTimeOffset * 1000);

		// para compabilidad de pausa, se hace esta forma
		if (((currentCountTime - startCountTime) * 0.001f) >= 0.1f)
		{		
			startCountTime = currentCountTime;
			explotionTime -= 0.1f;
			if (explotionTime <= 0)
			{
				Die();
			}	
		}
	}
}

void Bomb::PostUpdate()
{
	if(!pendingToDelete)
	{
		//App->render->DrawTexture(texture, getPosition(), &currentAnim->GetCurrentFrame());
		App->render->AddTextureRenderQueue(texture, getPosition(), &currentAnim->GetCurrentFrame(), 1, getPosition().y);
	}
}

void Bomb::OnCollision(Collider* col)
{
	if (col->type == Type::EXPLOSION && !pendingToDelete)
	{
		Die();
	}
}

void Bomb::Die()
{
	App->audio->PlaySound(explosionSFX, 0);

	pendingToDelete = true;

	App->scene->playerSettings->RemainBomb(true);

	getCollider()->pendingToDelete = true;

	// Create explotion center
	App->particle->AddParticle(explosionCenter, getPosition(), Type::EXPLOSION);

	#pragma region Calculate spawn number for dir

	int explotionNum[4] = { 0,0,0,0 };
	int tileX, tileY;
	tileX = lv1Tile->getTilePos(getPosition()).x;
	tileY = (lv1Tile->getTilePos(getPosition()).y) - 1;
	iPoint dirSpawn[4] = { {1,0},{0,1},{-1,0},{0,-1} };

	// Change number of tileMao when destroy
	lv1Tile->LevelsTileMaps[App->scene->currentLevel][myTilePos.y][myTilePos.x] = 0;	

	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j <= explotionRange; j++)
		{
			int temp = lv1Tile->LevelsTileMaps[App->scene->currentLevel][tileY + (dirSpawn[i].y) * j][tileX + (dirSpawn[i].x) * j];
			if (temp == 0 || temp == 4 || temp == -1 || temp == 11)
			{
				explotionNum[i]++;
			}
			else
			{
				if(temp !=1 && temp != 2 && temp  != 7 && temp != 10 && temp != 13 && temp != 12)
				{
					explotionNum[i]++;
				}
				break;
			}
		}
	}

	#pragma endregion

	#pragma region Create explotion

	for (int i = 1; i < explotionRange; i++)
	{
		// 4 direction for explotion
		iPoint dir[4] = {
		{ (i) * 16, 0 }, // Right
		{ 0, (i) * 16 }, // Down
		{ (i) * -16, 0 },// Left
		{ 0, (i) * -16 } // Up
		};

		// 4 rotation for explotion
		float rotation[4] = { 0, 270, 180, 90 };
		bool flipHor = true;

		for (int j = 0; j < 4; ++j)
		{
			if (explotionNum[j]-- > 0)
			{
				flipHor = true;
				// Explosopn End
				if (i == explotionRange - 1)
				{
					//++i; BUGGGGG
					if (j == 2)
					{
						flipHor = false;
					}
					App->particle->AddParticle(explosionEnd, getPosition() + dir[j], Type::EXPLOSION, flipHor, rotation[j]);
					//--i; BUGGGG y no se porque!!!!!!!!!!!!!!!!!!!!
				}
				else
				{
					// Explosion Middle
					App->particle->AddParticle(explosionMiddle, (getPosition() + dir[j]), Type::EXPLOSION, flipHor, rotation[j]);
				}
			}			
		}
	}

	#pragma endregion
}

void Bomb::CleanUp()
{
	App->scene->playerSettings->RemainBomb(true);
}
