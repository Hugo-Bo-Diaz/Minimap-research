#ifndef __MINIMAP_H__

#include "SDL\include\SDL.h"
#include "SDL_image\include\SDL_image.h"
#include <list>
#include <map>


struct dot
{
	SDL_Rect rect;//real map space that wants to be represented
	SDL_Color color;// color we want the point on the minimap
};

class Minimap
{
public:

	SDL_Texture* ret;
	SDL_Surface* surface;// the texture we are using will be saved here
	std::list<dot> points;// this is the list of points we want on the minimap

	int window_position_x;// position of the minimap on the window
	int window_position_y;

	int width;//width and height of the minimap on the window
	int height;

	float ratio_x = 1;//relation to the map it is representing
	float ratio_y = 1;

	int map_width;//total map width and height
	int map_height;

	Minimap(const char* base_texture_path, int _window_position_x, int _window_position_y, int _width, int _height, int tex_width = -1, int tex_height = -1);

	//this function returns a texture of the map
	SDL_Texture* GetMinimap();

	//and this one draws it directly
	void DrawMinimap();

	void Addpoint (SDL_Rect rect, SDL_Color color);

	void Mouse_to_map(int& map_x, int& map_y);

};


#endif 
