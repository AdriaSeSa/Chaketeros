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

UpdateResult ModuleEnemy::PreUpdate()
{
	return UpdateResult::UPDATE_CONTINUE;
}

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
	if (otherCol->type == Type::EXPLOSION && !protect)
	{
		protect = true;
		if (--life == 0)
		{
			Die();
			App->scene->playerSettings->playerScore += points;
			App->scene->DrawPoints(points, this->col->getPos());
		}
	}
};
