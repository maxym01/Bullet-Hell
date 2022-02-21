#pragma once

class Healer
{
	double health, xpos, ypos;
	double xhitbox, yhitbox;
	SDL_Surface* texture;
	SDL_Texture* r_texture;
public:
	Healer(Game& game);
	void Heal(Entity& player);
	void Render(Game& game, int x, int y, SDL_Rect* clip);
	~Healer();
};