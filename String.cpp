#include "String.h"
#include <SDL.h>
#include <SDL_main.h>


String::String(int x, int y)
{
	charset = SDL_LoadBMP("./cs8x8.bmp");
	this->x = x;
	this->y = y;
	SDL_SetColorKey(charset, true, 0x000000);
}

// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void String::DrawString(SDL_Surface* screen, const char* text) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


String::~String()
{
	SDL_FreeSurface(charset);
}