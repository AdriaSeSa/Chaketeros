#include "ModuleEnemy.h"

ModuleEnemy::ModuleEnemy()
{

};

ModuleEnemy::ModuleEnemy(int x, int y) 
{
	position.x = x;
	position.y = y;
};

ModuleEnemy::~ModuleEnemy() 
{
};

bool ModuleEnemy::Start() 
{
	return true;
};

UpdateResult ModuleEnemy::Update() 
{
	return UpdateResult::UPDATE_CONTINUE;
};
UpdateResult ModuleEnemy::PostUpdate() 
{
	return UpdateResult::UPDATE_CONTINUE;
};
void ModuleEnemy::OnCollision(Collider* otherCol) 
{

};

