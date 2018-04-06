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

struct sprite_to_blit
{
	SDL_Surface* sprite;//sprite or image to blit
	SDL_Rect section;
};

class Minimap
{
public:
	SDL_Texture * ret;

	std::list<dot> points;// this is the list of points we want on the minimap on one frame
	std::list<sprite_to_blit> queue;// this is the list of sprites we want in one frame

	// position of the minimap on the window
	int window_position_x;
	int window_position_y;

	//width and height of the minimap on the window
	int width;
	int height;

private:

	SDL_Surface* base_image;// the base texture we are using will be saved here

	//relation to the map it is representing
	float ratio_x = 1;
	float ratio_y = 1;

	//total map width and height
	int map_width;
	int map_height;

public:
	//constructor
	Minimap(const char* base_texture_path, int _window_position_x, int _window_position_y, int _width, int _height, int tex_width = -1, int tex_height = -1);

	//destructor
	~Minimap();

	//this function returns a texture of the minimap
	SDL_Texture* GetMinimap();

	//and this one draws it directly
	void DrawMinimap();
	
	//used to add a point to the map
	void Addpoint (SDL_Rect rect, SDL_Color color);

	// used to add a sprite to the map, the sprite must be loaded in whoever calls this function
	void Draw_Sprite(SDL_Surface* img, int x, int y);

	//transforms a coordinate clicked in the minimap into a real map one
	void Mouse_to_map(int& map_x, int& map_y);
};


#endif 
