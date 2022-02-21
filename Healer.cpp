#include "Llibrary.h"

Healer::Healer(Game& game) //generate bonus health points randomly on surface
{
	srand(time(NULL));  
	xpos = rand() % (LEVEL_WIDTH + 1);
	ypos = rand() % (LEVEL_HEIGHT + 1);
	health = HEAL_AMOUNT;
	texture = SDL_LoadBMP("./healer.bmp");
	xhitbox = xpos + texture->w;
	yhitbox = ypos + texture->h;
	r_texture = SDL_CreateTextureFromSurface(game.renderer, texture);
}

void Healer::Heal(Entity& player)
{
	if ((xpos > player.xpos && xpos<player.xhitbox && ypos>player.ypos && ypos < player.yhitbox)
		|| (xhitbox > player.xpos && xpos<player.xhitbox && ypos>player.ypos && yhitbox < player.yhitbox)) //
	{
		srand(time(NULL));
		if(player.health<=PLAYER_HP-HEAL_AMOUNT)   //teleport away once obtained
			player.health += health;
		xpos = rand() % (LEVEL_WIDTH + 1);
		ypos = rand() % (LEVEL_HEIGHT + 1);
	}

}

void Healer::Render(Game& game, int x, int y, SDL_Rect* clip)
{
	xhitbox = xpos + texture->w;
	yhitbox = ypos + texture->h;

	SDL_Rect renderQuad = { xpos - x, ypos - y, texture->w, texture->h };

	//Render to screen
	SDL_RenderCopy(game.renderer, r_texture, clip, &renderQuad);
}

Healer::~Healer()
{
	SDL_FreeSurface(texture);
	SDL_DestroyTexture(r_texture);
}