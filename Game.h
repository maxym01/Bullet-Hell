#pragma once
#include <SDL.h>
#include <SDL_main.h>

struct Unlocker;

class Entity;

class Game
{
	SDL_Surface* background, *grade, *gameover, *menu, *gamewon;
	SDL_Surface* dirt, *iron, *gold, *diamond, *netherite;
	SDL_Texture* r_background, *r_gameover, *r_menu, *r_gamewon; 
	SDL_Texture* scrtex;
	SDL_Window* window;
	int rc;
	SDL_Surface* screen;
	SDL_Renderer* renderer;
public:
	friend class Entity;
	friend class Healer;
	int black, green, blue, red;
	Game();
	void Menu();
	SDL_Surface* GetScreen();
	void Grade(int x, int y, Entity& player);
	void Health(Entity& player, Entity& enemy);
	void DrawPixel(int x, int y, Uint32 color);
	void DrawLine(int x, int y, int l, int dx, int dy, Uint32 color);
	void DrawRectangle(int x, int y, int l, int k,
		Uint32 outlineColor, Uint32 fillColor);
	void RenderBG(int x, int y, SDL_Rect* clip);
	void RenderString();
	void Refresh();
	void Event_Handling(SDL_Event& event, Entity& object, Entity* player_bullet, Unlocker& unlock);
	bool Save(Entity& object);
	void NewGame(Entity& player, Entity& enemy, Entity* player_bullet, Entity* enemy_bullet, bool& powerup_enemy);
	void End(Entity& player, Entity& enemy, Entity* enemy_bullet);
	~Game();
};