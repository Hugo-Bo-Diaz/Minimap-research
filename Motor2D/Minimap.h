#ifndef __MINIMAP_H__

#include "SDL\include\SDL.h"
#include "SDL_image\include\SDL_image.h"
#include <list>
#include <map>


struct dot
{
	SDL_Rect rect;//relation to the real map
	SDL_Color color;

};

class Minimap
{
public:
	SDL_Texture* ret;
	SDL_Surface* surface;
	std::list<dot> points;

	int width;
	int height;

	int map_width;
	int map_height;

	Minimap()
	{}

	Minimap(const char* base_texture_path ,int _width, int _height, int tex_width = -1, int tex_height = -1)
	{
		if (tex_width == -1 && tex_height == -1)
		{
			surface = IMG_Load(base_texture_path);
		}
		else
		{

			SDL_Rect r = { 0,0,tex_width,tex_height };
			surface = SDL_CreateRGBSurface(0, tex_width, tex_height, 32, 0, 0, 0, 0);
			SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));


		}

		map_width = _width;
		map_height = _height;
		width = surface->w;
		height = surface->h;
	
	}
	
	SDL_Texture* GetMinimap();

	void DrawMinimap(int x, int y);

	void Addpoint (SDL_Rect rect, SDL_Color color);

};


#endif 
