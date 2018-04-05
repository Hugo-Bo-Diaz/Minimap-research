#include "Minimap.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

Minimap::Minimap(const char* base_texture_path, int _window_position_x, int _window_position_y, int _width, int _height, int tex_width, int tex_height)
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

	window_position_x = _window_position_x;
	window_position_y = _window_position_y;

	float temp_w = width;
	float temp_mw = map_width;
	float temp_h = height;
	float temp_mh = map_height;

	ratio_x = temp_w / temp_mw;
	ratio_y = temp_h / temp_mh;

}

SDL_Texture* Minimap::GetMinimap()
{
	SDL_Surface* manipulation = surface;

	for (std::list<dot>::iterator it = points.begin(); it != points.end(); it++)
	{
		SDL_Rect representation;

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

void Minimap::DrawMinimap()
{
	SDL_Surface* manipulation = new SDL_Surface();
	//SDL_Rect r = {0,0,surface->w,surface->h};
	//SDL_BlitSurface(surface, NULL, manipulation, NULL);

	manipulation = SDL_ConvertSurface(surface, surface->format, SDL_SWSURFACE);

	//LOG("%s",SDL_GetError());

	for (std::list<dot>::iterator it = points.begin(); it != points.end(); it++)
	{
		SDL_Rect representation;

		representation.x = ratio_x * it->rect.x;
		representation.y = ratio_y * it->rect.y;
		representation.w = ratio_x * it->rect.w;
		representation.h = ratio_y * it->rect.h;

		SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, it->color.b, it->color.g, it->color.r));
		//SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, 255, 0, 0));

	}

	points.clear();

	//now we will blit the map borders
	SDL_Rect up = {-App->render->camera.x * ratio_x,-App->render->camera.y * ratio_y ,App->render->camera.w * ratio_x, 1};
	SDL_FillRect(manipulation, &up, SDL_MapRGB(manipulation->format, 255, 255, 255));

	SDL_Rect down = {-App->render->camera.x * ratio_x,-(App->render->camera.y-App->render->camera.h) * ratio_y -1 ,App->render->camera.w * ratio_x, 1 };
	SDL_FillRect(manipulation, &down, SDL_MapRGB(manipulation->format, 255, 255, 255));

	SDL_Rect left = {-App->render->camera.x * ratio_x,-App->render->camera.y * ratio_y ,1 , App->render->camera.h * ratio_y };
	SDL_FillRect(manipulation, &left, SDL_MapRGB(manipulation->format, 255, 255, 255));

	SDL_Rect right = {-(App->render->camera.x -App->render->camera.w) * ratio_x - 1 , - App->render->camera.y * ratio_y ,1, App->render->camera.h* ratio_y };
	SDL_FillRect(manipulation, &right, SDL_MapRGB(manipulation->format, 255, 255, 255));

	
	ret = SDL_CreateTextureFromSurface(App->render->renderer, manipulation);
	App->render->Blit(ret,window_position_x - App->render->camera.x, window_position_y - App->render->camera.y);
	SDL_DestroyTexture(ret);
	SDL_FreeSurface(manipulation);

}
void Minimap::Addpoint(SDL_Rect rect, SDL_Color color) 
{
	dot p;
	p.rect = rect;
	p.color = color;

	points.push_back(p);
}

void Minimap::Mouse_to_map(int& map_x, int& map_y)
{
	int mouse_x, mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);

	if (	mouse_x > window_position_x && mouse_x < window_position_x + width
		&&	mouse_y > window_position_y && mouse_y < window_position_y + height)
	{
		map_x = (mouse_x - window_position_x)/ ratio_x;
		map_y = (mouse_y - window_position_y) / ratio_y;
	}
	else
	{
		map_x = -1;
		map_y = -1;
	}
}