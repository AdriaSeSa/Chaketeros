#include "Particle.h"

#include "External/SDL/include/SDL_timer.h"

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed),
										frameCount(p.frameCount), lifetime(p.lifetime), renderTex(p.renderTex)
{

}

Particle::Particle(float lifetime, float animSpeed, SDL_Texture* tex, bool flipHor, float rotation, iPoint speed)
{
	position.SetToZero();
	anim.loop = false;
	anim.hasIdle = false;
	isAlive = true;

	anim.speed = animSpeed;
	renderTex = tex;
	this->speed = speed;
	this->lifetime = lifetime;
	this->flipHor = flipHor;
	this->rotation = rotation;
}

Particle::~Particle()
{
	if ( col != nullptr)
	{
		col->pendingToDelete = true;
	}	
}

bool Particle::Update()
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the spawnTime is reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive && frameCount >= 0)
	{
		anim.Update();
		
		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= (int)lifetime) {
				ret = false;
			}
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.HasFinished())
			ret = false;

		// Update the position in the screen
		position.x += speed.x;
		position.y += speed.y;

		if ( col != nullptr)
		{
			col->SetPos(position.x, position.y);
		}		
	}

	return ret;
}

void Particle::InitParticle(float lifetime, float animSpeed, SDL_Texture* tex, bool flipHor, float rotation, iPoint speed)
{
	position.SetToZero();
	anim.loop = false;
	anim.hasIdle = false;
	isAlive = true;

	anim.speed = animSpeed;
	renderTex = tex;
	this->speed = speed;
	this->lifetime = lifetime;
	this->flipHor = flipHor;
	this->rotation = rotation;
}
