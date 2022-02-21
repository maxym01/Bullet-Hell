#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_main.h>
#include <ctime>
#include "Game.h"
#include "Entity.h"
#include "String.h"
#include "Healer.h"
#pragma warning(disable : 4996)  //allow printf function not to throw error

#define SCREEN_WIDTH	  840
#define SCREEN_HEIGHT	  680
#define LEVEL_WIDTH	      1280
#define LEVEL_HEIGHT      960
#define NUMBER_OF_BULLETS 10    //defines how many bullets can player and enemy have
#define PLAYER_X          640  //coords of player and enemy
#define PLAYER_Y		  480
#define ENEMY_X           360
#define ENEMY_Y			  300
#define PLAYER_HP		  100
#define ENEMY_HP		  100
#define ENEMY_HP_2        300 // ...2 are defined for enemy for harder level 2
#define PLAYER_ATTACK     10
#define ENEMY_ATTACK      10
#define ENEMY_ATTACK_2    20
#define PLAYER_SPEED	  100
#define BULLET_SPEED	  150
#define COOLDOWN_DURATION 2  //duration of invincibility frames
#define MAGNITUTE_VALUE   1  //defines a score multiplier 
#define SCORE_ADDITION    10 //defines the score that the player gets per hit
#define HEAL_AMOUNT		  25
#define IRON_RANGE        100 //define ranges for grades
#define GOLD_RANGE        300
#define DIAMOND_RANGE     600
#define NETHERITE_RANGE   1000


struct Unlocker //logical struct to hold initializing variables
{
	bool cooldown = 0, initiate_cooldown = 1, powerup_enemy = 0, menu_deactivate = 0, reset_game=0, quit=0;
};