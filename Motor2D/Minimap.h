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

	int window_position_x;
	int window_position_y;

	int width;
	int height;

	float ratio_x = 1;
	float ratio_y = 1;

	int map_width;
	int map_height;

	Minimap(const char* base_texture_path, int _window_position_x, int _window_position_y, int _width, int _height, int tex_width = -1, int tex_height = -1);

	SDL_Texture* GetMinimap();

	void DrawMinimap(int x, int y);

	void Addpoint (SDL_Rect rect, SDL_Color color);

	void Mouse_to_map(int& map_x, int& map_y);

};


#endif 
