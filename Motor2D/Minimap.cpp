#include "Minimap.h"
#include "j1App.h"
#include "j1Render.h"
SDL_Texture* Minimap::GetMinimap()
{
	SDL_Surface* manipulation = surface;

	for (std::list<dot>::iterator it = points.begin(); it != points.end(); it++)
	{
		SDL_Rect representation;

		representation.x = width / map_width;
		representation.y = height / map_height;
		representation.w = width / map_width;
		representation.h = height / map_height;

		SDL_FillRect(manipulation, &representation, (manipulation->format, it->color.r, it->color.g, it->color.b)); 
	}

	ret = SDL_CreateTextureFromSurface(App->render->renderer, manipulation);

	return ret;
}