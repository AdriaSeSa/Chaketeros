#include "ModuleCollisions.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleInput.h"

#include "External/SDL/include/SDL_Scancode.h"

ModuleCollisions::ModuleCollisions()
{
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}	

	//TRUE -> Can be detected BY FIRST OBJECT // FALSE -> CAN'T BE DETECTED BY FIRST OBJECT
	// EX-> matrix[uint(Type::WALL)][uint(Type::PLAYER)] = false;
	// WALL CAN DETECT PLAYER

	matrix[uint(Type::WALL)][uint(Type::WALL)] = false;
	matrix[uint(Type::WALL)][uint(Type::PLAYER)] = false;
	matrix[uint(Type::WALL)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::WALL)][uint(Type::BOMB)] = false;
	matrix[uint(Type::WALL)][uint(Type::EXPLOSION)] = false;
	matrix[uint(Type::WALL)][uint(Type::DESTRUCTABLE_WALL)] = false;
	matrix[uint(Type::WALL)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::WALL)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::WALL)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::WALL)][uint(Type::COIN)] = false;

	matrix[uint(Type::PLAYER)][uint(Type::WALL)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::PLAYER)] = false;
	matrix[uint(Type::PLAYER)][uint(Type::ENEMY)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::BOMB)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::DESTRUCTABLE_WALL)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::FIREPOWER)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::BOMBPOWER)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::INVINCIBLEPOWER)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::COIN)] = true;

	matrix[uint(Type::ENEMY)][uint(Type::WALL)] = true;
	matrix[uint(Type::ENEMY)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::ENEMY)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::ENEMY)][uint(Type::BOMB)] = true;
	matrix[uint(Type::ENEMY)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::ENEMY)][uint(Type::DESTRUCTABLE_WALL)] = true;
	matrix[uint(Type::ENEMY)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::ENEMY)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::ENEMY)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::ENEMY)][uint(Type::COIN)] = false;
	matrix[uint(Type::ENEMY)][uint(Type::SARUSHOT)] = true;

	matrix[uint(Type::BOMB)][uint(Type::WALL)] = true;
	matrix[uint(Type::BOMB)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::BOMB)][uint(Type::ENEMY)] = true;
	matrix[uint(Type::BOMB)][uint(Type::BOMB)] = false;
	matrix[uint(Type::BOMB)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::BOMB)][uint(Type::DESTRUCTABLE_WALL)] = true;
	matrix[uint(Type::BOMB)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::BOMB)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::BOMB)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::BOMB)][uint(Type::COIN)] = false;

	matrix[uint(Type::EXPLOSION)][uint(Type::WALL)] = true;
	matrix[uint(Type::EXPLOSION)][uint(Type::PLAYER)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::BOMB)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::EXPLOSION)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::DESTRUCTABLE_WALL)] = true;
	matrix[uint(Type::EXPLOSION)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::BANANA)] = false;
	matrix[uint(Type::EXPLOSION)][uint(Type::COIN)] = false;

	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::WALL)] = true;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::ENEMY)] = true;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::BOMB)] = true;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::DESTRUCTABLE_WALL)] = false;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::DESTRUCTABLE_WALL)][uint(Type::COIN)] = false;

	matrix[uint(Type::FIREPOWER)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::FIREPOWER)][uint(Type::WALL)] = false;
	matrix[uint(Type::FIREPOWER)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::FIREPOWER)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::FIREPOWER)][uint(Type::BOMB)] = false;
	matrix[uint(Type::FIREPOWER)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::FIREPOWER)][uint(Type::DESTRUCTABLE_WALL)] = false;
	matrix[uint(Type::FIREPOWER)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::FIREPOWER)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::FIREPOWER)][uint(Type::COIN)] = false;

	matrix[uint(Type::BOMBPOWER)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::BOMBPOWER)][uint(Type::WALL)] = false;
	matrix[uint(Type::BOMBPOWER)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::BOMBPOWER)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::BOMBPOWER)][uint(Type::BOMB)] = false;
	matrix[uint(Type::BOMBPOWER)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::BOMBPOWER)][uint(Type::DESTRUCTABLE_WALL)] = false;
	matrix[uint(Type::BOMBPOWER)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::BOMBPOWER)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::BOMBPOWER)][uint(Type::COIN)] = false;

	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::WALL)] = false;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::BOMB)] = false;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::DESTRUCTABLE_WALL)] = false;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::INVINCIBLEPOWER)][uint(Type::COIN)] = false;

	matrix[uint(Type::COIN)][uint(Type::WALL)] = false;
	matrix[uint(Type::COIN)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::COIN)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::COIN)][uint(Type::BOMB)] = true;
	matrix[uint(Type::COIN)][uint(Type::EXPLOSION)] = true;
	matrix[uint(Type::COIN)][uint(Type::DESTRUCTABLE_WALL)] = false;
	matrix[uint(Type::COIN)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::COIN)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::COIN)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::COIN)][uint(Type::COIN)] = false;
	matrix[uint(Type::COIN)][uint(Type::SARUSHOT)] = true;

	matrix[uint(Type::SARUSHOT)][uint(Type::WALL)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::PLAYER)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::BOMB)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::EXPLOSION)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::DESTRUCTABLE_WALL)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::FIREPOWER)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::BOMBPOWER)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::INVINCIBLEPOWER)] = false;
	matrix[uint(Type::SARUSHOT)][uint(Type::BANANA)] = true;
	matrix[uint(Type::SARUSHOT)][uint(Type::SARUSHOT)] = false;

	matrix[uint(Type::BANANA)][uint(Type::EXPLOSION)] = false;
	matrix[uint(Type::BANANA)][uint(Type::SARUSHOT)] = true;
}

// Destructor
ModuleCollisions::~ModuleCollisions()
{

}

bool ModuleCollisions::Start()
{
	return true;
}

UpdateResult ModuleCollisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// Skip empty colliders
		if(colliders[i] == nullptr)
		{
			continue;
		}
			
		c1 = colliders[i];

		// Avoid checking collisions already checked
		for(uint k = i+1; k < MAX_COLLIDERS; ++k)
		{
			// Skip empty colliders
			if(colliders[k] == nullptr)
			{
				continue;
			}
				
			c2 = colliders[k];

			if (c1->WillIntersects(c2->rect))
			{
				if (matrix[uint(c1->type)][uint(c2->type)] && c1->listener)
					c1->listener->WillCollision(c1, c2);

				if (matrix[uint(c2->type)][uint(c1->type)] && c2->listener)
					c2->listener->WillCollision(c2, c1);
			}

			if(c1->Intersects(c2->rect))
			{
				if(matrix[uint(c1->type)][uint(c2->type)] && c1->listener)
					c1->listener->OnCollision(c1, c2);
				
				if(matrix[uint(c2->type)][uint(c1->type)] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}
		}
	}

	return UpdateResult::UPDATE_CONTINUE;
}

UpdateResult ModuleCollisions::Update()
{
	return UpdateResult::UPDATE_CONTINUE;
}

UpdateResult ModuleCollisions::PostUpdate()
{
	if (App->debug->debugColBox)
	{
		DebugDraw();
	}	

	return UpdateResult::UPDATE_CONTINUE;
}

void ModuleCollisions::DebugDraw()
{
	Uint8 alpha = 80;
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
			continue;
		
		switch(colliders[i]->type)
		{
			case Type::NONE: // white
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 255, 255, 255, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 255, 255, 255, alpha });
			break;
			case Type::WALL: // blue
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 0, 0, 255, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 0, 0, 255, alpha });
			break;
			case Type::PLAYER: // green
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 0, 255, 0, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 0, 255, 0, alpha });
			break;
			case Type::ENEMY: // red
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 255, 0, 0, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 255, 0, 0, alpha });
			break;
			case Type::BOMB: // yellow
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 255, 255, 0, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 255, 255, 0, alpha });
			break;
			case Type::EXPLOSION: // cyan
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 0, 255, 255, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 0, 255, 255, alpha });
			break;
			case Type::DESTRUCTABLE_WALL: // magenta
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 255, 0, 255, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 255, 0, 255, alpha });
			break;
			case Type::FIREPOWER: // black
			case Type::BOMBPOWER:
			case Type::INVINCIBLEPOWER:
			//App->render->DrawRectangle(colliders[i]->rect, SDL_Color{ 0, 0, 0, alpha });
			App->render->AddRectRenderQueue(colliders[i]->rect, SDL_Color{ 0, 0, 0, alpha });
			break;
		}
	}
}

// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

void ModuleCollisions::CleanUpScene()
{
	LOG("Freeing all colliders in Scene");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Type type, Module* listener)
{
	Collider* ret = nullptr;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}
