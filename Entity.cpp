#include "Llibrary.h"

Entity::Entity()  //constructor for bullets, -1 means the variable won't be used
{
	texture = SDL_LoadBMP("./dot.bmp");
	attack = PLAYER_ATTACK;
	score = -1;
	row = 0;
}

Entity::Entity(Game& game, int x, int y, int hp, const char* path, bool k) //constructor for objects that are either player or enemy
{
	xpos = x;
	ypos = y;
	texture = SDL_LoadBMP(path);
	r_texture = SDL_CreateTextureFromSurface(game.renderer, texture);
	health = hp;
	speed_x = 0;
	speed_y = 0;
	attack = 0;
	membership = k;
	if (membership == 1)
		max_health = PLAYER_HP;
	else if (membership == 0)
		max_health = ENEMY_HP;
	xhitbox = x + texture->w;
	yhitbox = y + texture->h;
	score = 0;
	magnitute = 1;
}

void Entity::Set(Game& game, Entity& object, bool k)  //sets parameters for bullets
{
	r_texture = SDL_CreateTextureFromSurface(game.renderer, texture);
	membership = k;
	xpos = object.xpos + object.texture->w / 2;
	speed_y = 0;
	if (membership)
	{
		ypos = object.ypos;
		newest_bullet_id = 0;
	}
	else
	{
		speed_y = BULLET_SPEED;
		ypos = object.ypos + object.texture->h;
		newest_bullet_id = 1;
	}
	speed_x = 0;
	ypos = object.ypos;
}

void Entity::SetDistance(bool x, bool y, double value)
{
	if (x)
		xpos += value;
	else if (y)
		ypos += value;
}

void Entity::SetId(int value)
{
	newest_bullet_id = value;
}

int Entity::GetId()
{
	return newest_bullet_id;
}

bool Entity::GetMembership()
{
	return membership;
}

double Entity::GetSpeed(bool x, bool y)
{
	if (x)
		return speed_x;
	else if (y)
		return speed_y;
}


double Entity::GetX()
{
	return xpos;
}

double Entity::GetY()
{
	return ypos;
}

SDL_Surface* Entity::GetSurface()
{
	return texture;
}

int Entity::GetScore()
{
	return score;
}

void Entity::UpdateScore(int value)
{
	score += value;
}

bool Entity::CheckRow()
{
	return row;
}

void Entity::Render(Game& game, int x, int y, SDL_Rect* clip)
{
	xhitbox = xpos + texture->w;
	yhitbox = ypos + texture->h;

	if ((xpos < 0) || (xpos > LEVEL_WIDTH - texture->w))  //keep the object within the level surface
	{
		xpos -= speed_x / 100;
	}
	if ((ypos < 0) || (ypos > LEVEL_HEIGHT - texture->h))
	{
		ypos -= speed_y / 100;
	}
	SDL_Rect renderQuad = { xpos - x, ypos - y, texture->w, texture->h };

	//Render to screen
	SDL_RenderCopy(game.renderer, r_texture, clip, &renderQuad);
}

void Entity::BulletOutside(Entity& object)
{
	if ((xpos < 0) || (xpos > LEVEL_WIDTH - texture->w) || (ypos < 0) || (ypos > LEVEL_HEIGHT - texture->h))
	{
		if (membership)  //remove the bullets, once outside the borders (for player)
			Entity::RemoveFromWorld();
		else
		{
			xpos = object.xpos + object.texture->w / 2;  //reuse the bullets for enemy once outside the borders
			ypos = object.ypos + object.texture->h/2;
		}
	}
}

bool Entity::Collision(Entity& object, bool& cooldown, Entity& UpdateScore)  //checks whether the bullet hit an object
{
	if ((xpos > object.xpos && xpos<object.xhitbox && ypos>object.ypos && ypos < object.yhitbox)
		|| (xhitbox > object.xpos && xpos<object.xhitbox && ypos>object.ypos && yhitbox < object.yhitbox))  //if describing the hitbox
	{
		object.health -= attack;
		cooldown = 1;
		if (membership)
		{
			UpdateScore.score += SCORE_ADDITION *UpdateScore.magnitute;  //update players score and remove bullets
			Entity::RemoveFromWorld();
		}
		else
		{
			if(object.score>= SCORE_ADDITION) //substract the score once hit by an enemy, teleport back the bullet
			object.score -=SCORE_ADDITION;
			xpos = ENEMY_X + object.texture->w / 2;
			ypos = ENEMY_Y;
			object.magnitute = 1;
		}
		row = 1;
		return true;
	}
	else
		return false;
}

void Entity::RemoveFromWorld()
{
	xpos = -100;
	speed_x = 0;
	ypos = -100;
	speed_y = 0;
}


Entity::~Entity()
{
	SDL_FreeSurface(texture);
	SDL_DestroyTexture(r_texture);
}