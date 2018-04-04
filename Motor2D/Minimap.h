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

			int rmask;
			int gmask;
			int bmask;
			int amask;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
					rmask = 0xff000000;
					gmask = 0x00ff0000;
					bmask = 0x0000ff00;
					amask = 0x000000ff;
		#else
					rmask = 0x000000ff;
					gmask = 0x0000ff00;
					bmask = 0x00ff0000;
					amask = 0xff000000;
		#endif
					surface = SDL_CreateRGBSurface(0, tex_width, tex_height, 32, rmask, gmask, bmask, amask);
		}

		map_width = _width;
		map_height = _height;
		width = surface->w;
		height = surface->h;
	
	}
	
	SDL_Texture* GetMinimap();

};


#endif 
