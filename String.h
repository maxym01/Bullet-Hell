#pragma once
#include <SDL.h>
#include <SDL_main.h>
class String
{
	SDL_Surface* charset;
	int x, y;
public:
	String(int x, int y);
	void DrawString(SDL_Surface* screen, const char* text);
	~String();
};