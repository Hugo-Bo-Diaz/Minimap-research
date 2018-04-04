#include "Minimap.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Log.h"
SDL_Texture* Minimap::GetMinimap()
{
	SDL_Surface* manipulation = surface;

	for (std::list<dot>::iterator it = points.begin(); it != points.end(); it++)
	{
		SDL_Rect representation;

		float temp_w = width;
		float temp_mw = map_width;
		float temp_h = height;
		float temp_mh = map_height;

		float ratio_x = temp_w / temp_mw;
		float ratio_y = temp_h / temp_mh;
		representation.x = ratio_x* it->rect.x;
		representation.y = ratio_y* it->rect.y;
		representation.w = ratio_x* it->rect.w;
		representation.h = ratio_y* it->rect.h;

		SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, it->color.b, it->color.g, it->color.r));
//		SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, 255, 0, 0));

	}

	ret = SDL_CreateTextureFromSurface(App->render->renderer, manipulation);

	return ret;
}

void Minimap::DrawMinimap(int x, int y)
{
	SDL_Surface* manipulation = new SDL_Surface();
	SDL_Rect r = {0,0,surface->w,surface->h};
	SDL_BlitSurface(surface, NULL, manipulation, NULL);
	
	LOG("%s",SDL_GetError());

	for (std::list<dot>::iterator it = points.begin(); it != points.end(); it++)
	{
		SDL_Rect representation;

		float temp_w = width;
		float temp_mw = map_width;
		float temp_h = height;
		float temp_mh = map_height;

		float ratio_x = temp_w / temp_mw;
		float ratio_y = temp_h / temp_mh;
		representation.x = ratio_x * it->rect.x;
		representation.y = ratio_y * it->rect.y;
		representation.w = ratio_x * it->rect.w;
		representation.h = ratio_y * it->rect.h;

		SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, it->color.b, it->color.g, it->color.r));
		//		SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, 255, 0, 0));

	}
	ret = SDL_CreateTextureFromSurface(App->render->renderer, manipulation);
	App->render->Blit(ret,x,y);
	SDL_DestroyTexture(ret);

}
void Minimap::Addpoint(SDL_Rect rect, SDL_Color color) 
{
	dot p;
	p.rect = rect;
	p.color = color;

	points.push_back(p
);
}
