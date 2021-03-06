#include "Tile.h"

Tile::Tile()
{

}


iPoint Tile::getWorldPos(iPoint pos)
{
	iPoint ret = { 0,0 };

	ret.x += 8;	//Ajuste para que encajen las Tiles

	/*ret.x += pivotPoint.x;	//Ajuste para que se alinee con el Pivot Point
	ret.y += pivotPoint.y;*/

	ret.x += pos.x * 16;
	ret.y += pos.y * 16;

	return ret;
}

iPoint Tile::getWorldPos(int x, int y)
{
	iPoint ret = { 0,0 };

	ret.x += 8;	//Ajuste para que encajen las Tiles

	ret.x += x * 16;
	ret.y += y * 16;

	return ret;
}

iPoint Tile::getTilePos(iPoint pos)	//EXAMPLE :31, 87	
{	
	pos.x -= 8;	// Restamos 8 mposiciones para compensar por el tamanyo del mapa				//16

	pos.x -= pos.x % r.w;	//Calculamos el resto de cada eje	16
	pos.y -= pos.y % r.w;	// 80

	pos.x /= r.w;	//Encontrar el indice de LevelsTileMaps[App->scene->currentLevel] 1
	pos.y /= r.w;	// 5

	return {pos.x, pos.y};
}

iPoint Tile::getTilePos(int x, int y)
{
	x -= 8;	// Restamos 8 mposiciones para compensar por el tamanyo del mapa				//16

	x -= (x % r.w);	//Calculamos el resto de cada eje	16
	y -= (y % r.w);	// 80

	x /= r.w;	//Encontrar el indice de LevelsTileMaps[App->scene->currentLevel] 1
	y /= r.w;	// 5

	return { x, y};
}

void Tile::setTile(iPoint tile, int num)
{
	LevelsTileMaps[App->scene->currentLevel][tile.x][tile.y] = num;

}

void Tile::setTile(int tileX, int tileY, int num)
{
	LevelsTileMaps[App->scene->currentLevel][tileX][tileY] = num;
}

void Tile::Reset()
{
	for (int i = 0, k = 0; i < 13; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			if (LevelsTileMaps[App->scene->currentLevel][i][j] == 5)
			{
				LevelsTileMaps[App->scene->currentLevel][i][j] = 0;
			}
		}
	}
}
