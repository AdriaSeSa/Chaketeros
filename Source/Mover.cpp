#include "Mover.h"

Mover::Mover(iPoint spawnPos, iPoint* playerPos, Tile* level1Tile)
{
	position.x = spawnPos.x;
	position.y = spawnPos.y;
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 16;
	bounds.h = 16;
	this->playerPos = playerPos;
	this->level1Tile = level1Tile;

	#pragma region Init Anim

	// Anim Down
	downAnim.PushBack({ 5,166,23,30 });//IDLE
	downAnim.PushBack({ 37,166,23,30 });
	downAnim.PushBack({ 5,166,23,30 });//IDLE
	downAnim.PushBack({ 37,166,23,30 });
	downAnim.speed = defaultEnemySpeed;

	//Animation UP
	upAnim.PushBack({ 69,166,23,30 });//IDLE
	upAnim.PushBack({ 101,166,23,30 });
	upAnim.PushBack({ 69,166,23,30 });//IDLE
	upAnim.PushBack({ 101,166,23,30 });
	upAnim.speed = defaultEnemySpeed;

	//Animation RIGHT
	rightAnim.PushBack({ 135,166,23,30 });//IDLE
	rightAnim.PushBack({ 168,166,23,30 });
	rightAnim.PushBack({ 135,166,23,30 });//IDLE
	rightAnim.PushBack({ 201,166,23,30 });
	rightAnim.speed = defaultEnemySpeed;

	//Animation LEFT
	leftAnim.PushBack({ 135,166,23,30 });//IDLE
	leftAnim.PushBack({ 168,166,23,30 });
	leftAnim.PushBack({ 135,166,23,30 });//IDLE
	leftAnim.PushBack({ 201,166,23,30 });
	leftAnim.speed = defaultEnemySpeed;

	#pragma endregion
	currentAnimation = &downAnim;

	// Init destroyed particle
	dieParticle = new Particle(500.0f, 0.05f, texture);
	dieParticle->anim.PushBack({ 232,166,23,30 });

}

Mover::~Mover() 
{
	delete dieParticle;
	dieParticle = nullptr;
}

bool Mover::Start() 
{
	LOG("LOADING ENEMY MOVER");

	texture = App->textures->Load("Assets/Images/Sprites/Enemies_Sprites/Enemies.png");

	col = App->collisions->AddCollider(bounds, Type::ENEMY, App->scene);

	moverTimer = Timer::Instance();

	return true;
}

UpdateResult Mover::PreUpdate()
{
	iPoint tilePos =  level1Tile->getTilePos(position);
	iPoint centerTile = level1Tile->getWorldPos(tilePos);

	if (position == centerTile)
	{
		moveDirIndex = AStar();
	}

	return UpdateResult::UPDATE_CONTINUE;
}

UpdateResult Mover::Update() 
{
	col->SetPos(this->position.x, this->position.y);

	moverTimer->Update();

	if (moverTimer->getDeltaTime() >= 0.2f)
	{
		if (moveDirIndex != -1)
		{
			FixedUpdate();
		}		
		moverTimer->Reset();
	}

	return UpdateResult::UPDATE_CONTINUE;
}

void Mover::FixedUpdate()
{
	position += moveDir[moveDirIndex];
}

UpdateResult Mover::PostUpdate() {

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	iPoint tempPos = position;
	tempPos += {-4, -14};

	if (isFlip)
	{
		App->render->DrawRotateTexture(texture, tempPos, &rect, false, 180);
	}
	else
	{
		App->render->DrawTexture(texture, tempPos, &rect);
	}

	return UpdateResult::UPDATE_CONTINUE;
}

void Mover::RandomMov() {
	
	if ((position.x - 8) % (32) == 0 && position.y % 32 == 0) 
	{
		randomDir = rand() % 4;
	}

	switch (randomDir)
	{
	case 0: //UP
		isFlip = false;
		currentAnimation = &upAnim;
		currentAnimation->hasIdle = false;
		position.y -= speed;
		break;
	case 1: //DOWN
		isFlip = false;
		currentAnimation = &downAnim;
		currentAnimation->hasIdle = false;
		position.y += speed;
		break;
	case 2: //RIGHT
		isFlip = false;
		currentAnimation = &rightAnim;
		currentAnimation->hasIdle = false;
		position.x += speed;
		break;
	case 3: //LEFT
		isFlip = false;
		currentAnimation = &leftAnim;
		currentAnimation->hasIdle = false;
		position.x -= speed;
		break;
	}

	//currentAnimation = &downAnim;
	col->SetPos(position);
	currentAnimation->Update();

	bounds.x = position.x;
	bounds.y = position.y;

	pivotPoint = { position.x + 8, position.y + 8 };
}

int Mover::AStar()
{
	// �ҵĳ�ʼ���ӵ����� // mi posicion (tile)
	iPoint myTilePos = level1Tile->getTilePos(position);	
	myTilePos.y--;
	// Ŀ��ĸ��ӵ����� // posicion de destinatario (tile)
	iPoint playerTilePos = level1Tile->getTilePos(*playerPos);
	playerTilePos.y--;

	// �ҵ�Ŀ��ľ��� // distancia entre yo y destinatario
	int distance = position.DistanceManhattan(myTilePos, playerTilePos);

	// �ҵ�ǰ�ڵĸ��� // grid que estoy ubicado
	PathNode currentGrid;
	// ��ʼ���ӵ��ﵱǰ���ӵĻ��� // el coste de grid que estaba en principio hasta este grid
	currentGrid.g_cost = 0;
	// ��ǰ���ӵ���Ŀ����ӵĻ��� // el coste del grid que estoy hasta el destinatatio
	currentGrid.h_cost = distance;
	// �������������� // suma de los dos = coste de este grid
	currentGrid.total_cost = currentGrid.g_cost + currentGrid.h_cost;
	// Inicial la posicion del primer grid
	currentGrid.pos.x = myTilePos.x;
	currentGrid.pos.y = myTilePos.y;
	// el primer indice es -1
	currentGrid.lastIndex = -1;

	// ���浱ǰ��⵽����û�ߵĸ��� // guarda los grids que detectadas 
	vector<PathNode> openGrid;
	// �����Ѿ��߹��ĸ��� // guarda los grids que esta usada
	vector<PathNode> closeGrid;	
	// ����ʵ���Ӵ��뵽������� // guardar el grid que estamos ahora dentro de grid detectada
	openGrid.push_back(currentGrid);

	// �жϵ�ǰ���ĸ����Ƿ��Ǹ���Ч�ĸ��� // detectar si es un grid util o no
	bool pass = false;

	// �����⵽�ĸ��ӻ�û���� // si nos quedan grids para detectar
	while (!openGrid.empty())
	{	
		// ����˵ĸ��� // grid que tiene menos coste
		int lessGrid[2] = { 0, openGrid[0].total_cost };

		for (int i = 0; i < openGrid.size(); i++)
		{
			if (openGrid[i].total_cost < openGrid[lessGrid[0]].total_cost)
			{
				// ˢ������˵ĸ��� // actuaizar el grid que tiene menos coste
				lessGrid[0] = i;
				lessGrid[1] = openGrid[i].total_cost;		
			}
		}

		// ��������˵ĸ���Ϊ��ǰ����ĸ��� // guardar el grid que tiene menos coste como el grid acutual
		PathNode lessNode = openGrid[lessGrid[0]];
		// ɾ��ԭ�еĸ��� // eleminarlo del openGrid
		openGrid.erase(openGrid.begin() + lessGrid[0]);

		// �����ǰ���������ĸ��� // si el grid actual es el grid de destinatario
		if (lessNode.h_cost == 0)
		{
			closeGrid.push_back(lessNode);
		
			vector <int> instruction;

			for (int i = closeGrid.size() -1 ; i >= 0; i = closeGrid[i].lastIndex)
			{
				instruction.push_back(closeGrid[i].dir);
			}

			return instruction[instruction.size() - 2];
		}
		
		// ��ȡ�ĸ�����ĸ��� // obtener grid de 4 direcciones alrededor del grid actual
		iPoint dir[4] =
		{{ lessNode.pos.x + 1, lessNode.pos.y}, // Right
		{ lessNode.pos.x - 1, lessNode.pos.y }, // Left
		{ lessNode.pos.x , lessNode.pos.y - 1 }, // Up
		{ lessNode.pos.x , lessNode.pos.y + 1}}; // Down
		
		// �ж��ĸ�����ĸ����Ƿ���Ч // detectar si puedes avanzar en alguna de las 4 direcciones
		for (int i = 0; i < 4; ++i)
		{
			pass = false;

			// �����ǰ�������ϰ��� // si el grid que vamos a ir no es 0 o 4
			if(level1Tile->Level1TileMap[dir[i].y][dir[i].x] != 0 && level1Tile->Level1TileMap[dir[i].y][dir[i].x] != 4)
			{
				// ������ǰ���� // ignoramos este grid
				pass = true;
				continue;
			}

			for (int j = 0; j < closeGrid.size(); ++j)
			{
				// �����ǰ�����Ѿ�������closeGrid���� // si ya existe en close grid
				if (closeGrid[j].pos == dir[i])
				{
					// ������ǰ����
					pass = true;
					break;
				}
			}

			for (int k = 0; k < openGrid.size(); ++k)
			{
				// �����ǰ�����Ѿ�������openGrid���� // si ya existe en opengrid
				if (openGrid[k].pos == dir[i])
				{
					// ������ǰ����
					pass = true;
					break;
				}
			}

			// �����Ч������뵽���ĸ����� // si es un grid valido
			if(!pass)
			{
				// creamos un nuevo nodo para el grid
				PathNode node;
				node.lastIndex = closeGrid.size();
				node.dir = i;
				node.pos.x = dir[i].x;
				node.pos.y = dir[i].y;
				node.h_cost = position.DistanceManhattan(dir[i], playerTilePos);
				node.g_cost = position.DistanceManhattan(dir[i], myTilePos);
				node.total_cost = node.g_cost + node.h_cost;

				// metemos dentro del openGrid
				openGrid.push_back(node);
			}
		}

		// metemos el grid que estamos gestionando dentro del closeGrid
		closeGrid.push_back(lessNode);
	}

	// si no ha encontrado ningun camino, devolvemos un -1
	return { -1 };
}

void Mover::OnCollision(Collider* col) 
{
	if (col->type == Type::EXPLOSION) 
	{
		die();
	}
}

void Mover::die()
{
	isDead = false;

	col->pendingToDelete = true;

	App->particle->AddParticle(*dieParticle, position, Type::NONE, true, 0, 0);

	moverTimer->Release();

	delete dieParticle;
	dieParticle = nullptr;
}