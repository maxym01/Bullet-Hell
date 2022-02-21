#include "Llibrary.h"

Game::Game()
{
	background = SDL_LoadBMP("./bg.bmp");
	dirt = SDL_LoadBMP("./dirt.bmp");
	iron = SDL_LoadBMP("./iron.bmp");
	gold = SDL_LoadBMP("./gold.bmp");
	diamond = SDL_LoadBMP("./diamond.bmp");
	netherite = SDL_LoadBMP("./netherite.bmp");
	gameover = SDL_LoadBMP("./gameover.bmp");
	menu = SDL_LoadBMP("./menu.bmp");
	gamewon = SDL_LoadBMP("./gamewon.bmp");
	grade = 0;

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  //Set texture filtering to linear
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);  
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Bullet Hell");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000); 

	scrtex = SDL_CreateTextureFromSurface(renderer, screen);
	black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	r_background = SDL_CreateTextureFromSurface(renderer, background);
	r_gameover = SDL_CreateTextureFromSurface(renderer, gameover);
	r_menu = SDL_CreateTextureFromSurface(renderer, menu);
	r_gamewon = SDL_CreateTextureFromSurface(renderer, gamewon);

}

void Game::Menu()
{
	SDL_Rect renderQuad = { 0, 0, menu->w, menu->h };

	renderQuad.w = SCREEN_WIDTH;
	renderQuad.h = SCREEN_HEIGHT;

	SDL_RenderCopy(renderer, r_menu, NULL, &renderQuad);
}

SDL_Surface* Game::GetScreen()
{
	return screen;
}


void Game::Grade(int x, int y, Entity& player)
{
	if(player.score<IRON_RANGE)
		grade = dirt;
	else if(player.score < GOLD_RANGE && player.score >= IRON_RANGE)
		grade = iron;
	else if (player.score < DIAMOND_RANGE && player.score >= GOLD_RANGE)
		grade = gold;
	else if (player.score < NETHERITE_RANGE && player.score >= DIAMOND_RANGE)
		grade = diamond;
	else 
		grade = netherite;
	SDL_Rect dest;
	dest.x = x - grade->w / 2;
	dest.y = y - grade->h / 2;
	dest.w = grade->w;
	dest.h = grade->h;
	SDL_BlitSurface(grade, NULL, screen, &dest);
}

void Game::Health(Entity& player, Entity& enemy)  //displays health bar of player and enemy
{
	Game::DrawRectangle(4, 50, 10, PLAYER_HP, red, red);
	Game::DrawRectangle(4, 50, 10, player.health, green, green);

	Game::DrawRectangle(SCREEN_WIDTH-30, 50, 10, enemy.max_health, red, red);
	Game::DrawRectangle(SCREEN_WIDTH - 30, 50, 10, enemy.health, green, green);
}

// draw a single pixel
void Game::DrawPixel(int x, int y, Uint32 color) {
	int bpp = screen->format->BytesPerPixel;
	Uint8* p = (Uint8*)screen->pixels + y * screen->pitch + x * bpp;
	*(Uint32*)p = color;
};


// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void Game::DrawLine(int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(x, y, color);
		x += dx;
		y += dy;
	};
};


// draw a rectangle of size l by k
void Game::DrawRectangle(int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(x, y, k, 0, 1, outlineColor);
	DrawLine(x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(x, y, l, 1, 0, outlineColor);
	DrawLine(x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(x + 1, i, l - 2, 1, 0, fillColor);
};

void Game::RenderBG(int x, int y, SDL_Rect* clip)  //Renders blue n white background
{
	
	SDL_Rect renderQuad = { x, y, background->w, background->h };

		renderQuad.w = clip->w;
		renderQuad.h = clip->h;

	SDL_RenderCopy(renderer, r_background, clip, &renderQuad);
}

void Game::RenderString()
{
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
}

void Game::Refresh()
{
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);  //Updates rendering states
	SDL_RenderPresent(renderer);
}

void Game::NewGame(Entity& player, Entity& enemy, Entity* player_bullet, Entity* enemy_bullet, bool& powerup_enemy) //reset all the states
{
	player.xpos = PLAYER_X;
	player.ypos = PLAYER_Y;
	player.health = PLAYER_HP;
	player.score = 0;
	enemy.xpos = ENEMY_X;
	enemy.ypos = ENEMY_Y;
	if (powerup_enemy)
	{
		enemy.health = ENEMY_HP_2;
		for(int i=0; i<NUMBER_OF_BULLETS;i++)
		enemy_bullet[i].attack = ENEMY_ATTACK_2;
	}
	else
		enemy.health = ENEMY_HP;
	for (int i = 0; i < NUMBER_OF_BULLETS; i++)
	{
		player_bullet[i].xpos = player.xpos + player.texture->w / 2;
		player_bullet[i].ypos = player.ypos;
		enemy_bullet[i].xpos = enemy.xpos + enemy.texture->w / 2;
		enemy_bullet[i].ypos = enemy.ypos + enemy.texture->h / 2;
	}
}

void Game::Event_Handling(SDL_Event& event, Entity& object, Entity* player_bullet, Unlocker& unlock)
{
	// handling of events (if there were any)
	int id = player_bullet->GetId();
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) unlock.quit = 1;
				else if (event.key.keysym.sym == SDLK_UP) object.speed_y = -PLAYER_SPEED;
				else if (event.key.keysym.sym == SDLK_DOWN) object.speed_y = PLAYER_SPEED;
				else if (event.key.keysym.sym == SDLK_RIGHT) object.speed_x = PLAYER_SPEED;
				else if (event.key.keysym.sym == SDLK_LEFT) object.speed_x = -PLAYER_SPEED;
				else if (event.key.keysym.sym == SDLK_n) unlock.reset_game = 1;
				else if (event.key.keysym.sym == SDLK_w)
				{
					if (id < NUMBER_OF_BULLETS)
					{
						player_bullet[id].xpos = object.xpos + object.texture->w / 2;
						player_bullet[id].ypos = object.ypos;
						player_bullet[id].speed_y = -BULLET_SPEED;
						player_bullet[id].row = 0;
						player_bullet->SetId(id+1);
					}
					else
					{
						for (int i = 0; i < NUMBER_OF_BULLETS; i++)
						{
							player_bullet[i].xpos = object.xpos + object.texture->w / 2;
							player_bullet[i].ypos = object.ypos;
						}
						player_bullet->SetId(0);
					}
				}
				else if (event.key.keysym.sym == SDLK_f || event.key.keysym.sym == SDLK_2)
				{
					unlock.reset_game = 1;
					unlock.powerup_enemy = 1;
					unlock.menu_deactivate = 1;
				}
				else if (event.key.keysym.sym == SDLK_1)
				{
					unlock.reset_game = 1;
					unlock.menu_deactivate = 1;
					unlock.powerup_enemy = 0;
				}
				else if (event.key.keysym.sym == SDLK_m)
				{
					unlock.reset_game = 1;
					unlock.menu_deactivate = 0;
				}
				else if (event.key.keysym.sym == SDLK_s)
					Game::Save(object);
			break;
		case SDL_KEYUP:
			object.speed_y = 0;
			object.speed_x = 0;
			break;
		case SDL_QUIT:
			unlock.quit = 1;
			break;
		};
	};
}

bool Game::Save(Entity& object)  //saves the score 
{
	FILE* save;
	unsigned long long pot2 = 1;

	save = fopen("score.txt", "w");
	if (save == NULL) {
		printf("Failed to open a file\n");
		return 1;
	}
	fprintf(save, "%d\n", object.score);

	fclose(save);
}

void Game::End(Entity& player, Entity& enemy, Entity* enemy_bullet) //renders either game over or winning screen
{
	SDL_Rect renderQuad = { 0, 0, gameover->w, gameover->h };

	renderQuad.w = SCREEN_WIDTH;
	renderQuad.h = SCREEN_HEIGHT;

	if (enemy.health <= 0)
		SDL_RenderCopy(renderer, r_gamewon, NULL, &renderQuad);
	else if(player.health <= 0)
		SDL_RenderCopy(renderer, r_gameover, NULL, &renderQuad);

}

Game::~Game() //release all SDL stuff
{
	SDL_DestroyTexture(scrtex);
	SDL_DestroyTexture(r_background);
	SDL_DestroyTexture(r_menu);
	SDL_DestroyTexture(r_gameover);
	SDL_DestroyTexture(r_gamewon);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(background);
	SDL_FreeSurface(dirt);
	SDL_FreeSurface(iron);
	SDL_FreeSurface(gold);
	SDL_FreeSurface(diamond);
	SDL_FreeSurface(netherite);
	SDL_FreeSurface(menu);
	SDL_FreeSurface(gameover);
	SDL_FreeSurface(gamewon);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}