#include "YellowFlower.h"
#include "ModuleObstacle.h"

YellowFlower::YellowFlower() 
{
	
	this->SetRect({ 0,0,7,8 });
	this->SetTexture("Assets/Images/Sprites/Environment_Sprites/Yellow_Flower.png");
	this->SetCollider(new Collider(this->getRect(), Collider::Type::DESTRUCTABLE_WALL, App->obstacles));
	this->SetType(Collider::Type::DESTRUCTABLE_WALL);
	this->SetDestructable(true);


	dieTexture = App->textures->Load("Assets/Images/Sprites/PowerUps_Sprites/ItemDestroyedSheet.png");

	idle.PushBack({ 9,8,16,16 });
	
	currentAnim = &idle;

	die.PushBack({ 3,2,26,27 });
	die.PushBack({ 35,2,26,27 });
	die.PushBack({ 67,4,26,27 });
	die.PushBack({ 3,34,26,25 });
	die.PushBack({ 35,34,26,25 });
	die.PushBack({ 67,34,26,25 });
	die.speed = 0.5f;
	die.loop = true;
}



void YellowFlower::PostUpdate()
{
	App->render->DrawTexture(texture, this->getPosition().x, this->getPosition().y, &(currentAnim->GetCurrentFrame()));
}

void YellowFlower::Die()
{
	currentAnim = &die;
}

