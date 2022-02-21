#pragma once
#include <SDL.h>
#include <SDL_main.h>
#include "Game.h"

class Entity
{
public:
	double xpos, ypos, health;  //also left upper hitbox edge
	double speed_x, speed_y, attack;
	double xhitbox, yhitbox; //right lower hitbox edge
	double max_health; //maximum health (used to draw health bar)
	SDL_Surface* texture;
	SDL_Texture* r_texture;
	bool membership; //1-player, 0-enemy
	int score, newest_bullet_id, magnitute;
	bool row;  //used to change magnitute
//public:
	friend class Game;
	friend class Healer;
	friend void CheckCollision(Entity* bullet, Entity& object, Entity& UpdateScore, static int time, Unlocker& unlock);
	Entity();
	Entity(Game& game, int x, int y, int health, const char* path, bool k);
	void Set(Game& game, Entity& object, bool k);
	void SetDistance(bool x, bool y, double value);
	void SetId(int value);
	int GetId();
	bool GetMembership();
	double GetSpeed(bool x, bool y);
	double GetX();
	double GetY();
	SDL_Surface* GetSurface();
	int GetScore();
	bool CheckRow();
	void UpdateScore(int value);
	void Render(Game& game, int x, int y, SDL_Rect* clip);
	void BulletOutside(Entity& object);
	bool Collision(Entity& object, bool& cooldown, Entity& UpdateScore);
	void RemoveFromWorld();
	~Entity();
};