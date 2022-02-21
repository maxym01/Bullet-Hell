#include "Llibrary.h"

void CameraControll(SDL_Rect& camera) //keep the camera within the border
{
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > LEVEL_WIDTH - camera.w)
		camera.x = LEVEL_WIDTH - camera.w;
	if (camera.y > LEVEL_HEIGHT - camera.h)
		camera.y = LEVEL_HEIGHT - camera.h;
}

void BulletDistance(Entity* bullet, double delta)  //update bullets position
{
	for (int i = 0; i <bullet->GetId(); i++)
	{
		bullet[i].SetDistance(0, 1, bullet[i].GetSpeed(0, 1) * delta);
	}
}

void Text(Game& game, double worldTime, double fps, int player_newset_bullet_id, Entity& player) //show text
{
	char text[128];
	sprintf(text, "Esc - exit, arrows - movement, n - new game, w - shoot");
	String instruction(game.GetScreen()->w / 2 - strlen(text) * 8 / 2, 10);
	instruction.DrawString(game.GetScreen(), text);

	sprintf(text, "Level time = %.1lf s  %.0lf fps  Amo left = %d  Score = %d", worldTime, fps, 
		NUMBER_OF_BULLETS - player_newset_bullet_id, player.GetScore());
	String stats(game.GetScreen()->w / 2 - strlen(text) * 8 / 2, 26);
	stats.DrawString(game.GetScreen(), text);

	sprintf(text, "Your Health");
	String health_bar(8, 20);
	health_bar.DrawString(game.GetScreen(), text);

	sprintf(text, "Boss Health");
	String boss_bar(SCREEN_WIDTH-100, 20);
	boss_bar.DrawString(game.GetScreen(), text);

	sprintf(text, "Grade");
	String score(SCREEN_WIDTH - 90, 570);
	score.DrawString(game.GetScreen(), text);
}

void ShowBullets(Entity* bullet, Entity& object, Game& game, SDL_Rect& camera)
{
	for (int i = 0; i < bullet->GetId(); i++)
	{
		bullet[i].BulletOutside(object);
		bullet[i].Render(game, camera.x, camera.y, NULL);
	}
}

void Display_Board(Game& game, SDL_Rect& camera, Entity& player, Entity& enemy)
{
	SDL_ShowCursor(SDL_DISABLE);
	CameraControll(camera);
	game.Refresh();
	game.RenderBG(0, 0, &camera);
	game.Grade(SCREEN_WIDTH-50, 620, player);
	game.Health(player, enemy);
}

void Display_Entities(Game& game, Entity& player, Entity& enemy, Healer& healer, SDL_Rect& camera)
{
	healer.Render(game, camera.x, camera.y, NULL);
	player.Render(game, camera.x, camera.y, NULL);
	enemy.Render(game, camera.x, camera.y, NULL);
	game.RenderString();
}

void fps_counter(double& fpsTimer, double& delta, double& fps, int& frames)
{
	fpsTimer += delta;
	if (fpsTimer > 0.5) {
		fps = frames * 2;
		frames = 0;
		fpsTimer -= 0.5;
	};
}

void CheckCollision(Entity* bullet, Entity& object, Entity& UpdateScore, int time, Unlocker& unlock)
{
	static int temp;
	if (!unlock.cooldown || object.GetMembership()==0)
	{
		time = 0;
		for (int i = 0; i < NUMBER_OF_BULLETS; i++)
		{
			bool bonus = bullet[i].Collision(object, unlock.cooldown, UpdateScore);
			if (object.GetMembership() == 0 && i > 0 && bullet[i - 1].CheckRow() && bonus)
				UpdateScore.magnitute += MAGNITUTE_VALUE;			
		}
	}
	else
	{
		if (unlock.initiate_cooldown)
		{
			temp = time;
			unlock.initiate_cooldown = 0;
		}
		if (time - temp == COOLDOWN_DURATION)
		{
			unlock.initiate_cooldown = 1;
			unlock.cooldown = 0;
		}
	}
	
	
}

void SetBullets(Game& game, Entity& object, Entity* bullet, bool membership)
{
	for (int i = 0; i < NUMBER_OF_BULLETS; i++)
		bullet[i].Set(game, object, membership);
}

bool Check_SDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return -1;
	}
	return 0;
}

void SetWorldTime(int& t1, int& t2, double& delta, double& worldTime)
{
	t2 = SDL_GetTicks();
	delta = (t2 - t1) * 0.001;
	t1 = t2;
	worldTime += delta;
}

// main
int main(int argc, char** argv) {
	int t1, t2, quit, frames;
	double delta, worldTime, fpsTimer, fps;
	Unlocker unlock;
	SDL_Event event;
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Game game;
	Entity player(game, PLAYER_X, PLAYER_Y, PLAYER_HP, "./Good.bmp", 1);
	Entity enemy(game, ENEMY_X, ENEMY_Y, ENEMY_HP, "./Evil.bmp", 0);
	Entity* enemy_bullet = new Entity[NUMBER_OF_BULLETS];
	Entity* player_bullet = new Entity[NUMBER_OF_BULLETS];
	Healer healer(game);

	SetBullets(game, player, player_bullet, 1);
	SetBullets(game, enemy, enemy_bullet, 0);

	Check_SDL();  

	t1 = SDL_GetTicks();

	frames = 0, fpsTimer = 0, fps = 0, worldTime = 0;	

	while (!unlock.quit)
	{
		if (unlock.reset_game)
		{
			game.NewGame(player, enemy, player_bullet, enemy_bullet, unlock.powerup_enemy);
			unlock.reset_game = 0, frames = 0, fpsTimer = 0, fps = 0, worldTime = 0, player_bullet->SetId(0);
		}

		SetWorldTime(t1, t2, delta, worldTime);
	
		player.SetDistance(1, 0, player.GetSpeed(1, 0) * delta);  //update player coords
		player.SetDistance(0, 1, player.GetSpeed(0, 1) * delta);

		if (worldTime <= NUMBER_OF_BULLETS)
			enemy_bullet->SetId((int)worldTime); 

		BulletDistance(enemy_bullet, delta);  //update bullets coords
		BulletDistance(player_bullet, delta);

		fps_counter(fpsTimer, delta, fps, frames);

		game.DrawRectangle(4, 4, SCREEN_WIDTH - 8, 36, game.red, game.blue);  //draw rectangle as a background for text

		Text(game, worldTime, fps, player_bullet->GetId(), player);

		camera.x = (player.GetX() + player.GetSurface()->w / 2) - SCREEN_WIDTH / 2; //update camera position according to the players one
		camera.y = (player.GetY() + player.GetSurface()->h / 2) - SCREEN_HEIGHT / 2;

		Display_Board(game, camera, player, enemy);     //displays the game + checks if bullets hit object
		ShowBullets(enemy_bullet, enemy, game, camera);
		ShowBullets(player_bullet, player, game, camera);  
		CheckCollision(player_bullet, enemy, player, (int)worldTime, unlock);
		CheckCollision(enemy_bullet, player, enemy, (int)worldTime, unlock);
		Display_Entities(game, player, enemy, healer, camera);

		game.Event_Handling(event, player, player_bullet, unlock);
		healer.Heal(player);
		game.End(player, enemy, enemy_bullet);

		if (!unlock.menu_deactivate)  
			game.Menu();
		frames++;
	};
	delete[] enemy_bullet;
	delete[] player_bullet;
	SDL_Quit();
	return 0;
};
