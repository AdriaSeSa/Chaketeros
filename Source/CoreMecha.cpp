#include "CoreMecha.h"

CoreMecha::CoreMecha()
{
	LOG("Constructor yellowFlower");
}

CoreMecha::CoreMecha(iPoint pos, SDL_Texture* tex, SDL_Texture* texDie) : Obstacle({ pos.x, pos.y, 16, 16 }, true, App->collisions->AddCollider({ pos.x, pos.y, 16, 16 }, Type::DESTRUCTABLE_WALL, App->scene), tex)
{
	// Flow tienen sprites en diferentes sprite sheet, por eso necesita una textura aparte para guardar la animacion de morir
	this->texDie = texDie;

	renderRect = { 9, 8, 16, 16 };

	//idle.PushBack({ 9,8,16,16 });

	//currentAnim = &idle;

	//die.PushBack({ 3,2,26,27 });
	//die.PushBack({ 35,2,26,27 });
	//die.PushBack({ 67,4,26,27 });
	//die.PushBack({ 3,34,26,25 });
	//die.PushBack({ 35,34,26,25 });
	//die.PushBack({ 67,34,26,25 });
	//die.speed = 0.5f;
	//die.loop = true;
}

void CoreMecha::Die()
{
	//currentAnim = &die;
}

void CoreMecha::PostUpdate()
{
	//LOG("FlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\nFlowerUpdate\n");
	App->render->DrawTexture(texture, this->getPosition().x, this->getPosition().y, &renderRect);
}