#include "Minimap.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

Minimap::Minimap(const char* base_texture_path, int _window_position_x, int _window_position_y, int _width, int _height, int tex_width, int tex_height)
{
	if (tex_width == -1 && tex_height == -1)
	{
		base_image = IMG_Load(base_texture_path);
	}
	else
	{
		base_image = SDL_CreateRGBSurface(0, tex_width, tex_height, 32, 0, 0, 0, 0);
		SDL_FillRect(base_image, NULL, SDL_MapRGB(base_image->format, 255, 0, 0));
	}

	map_width = _width;
	map_height = _height;
	width = base_image->w;
	height = base_image->h;

	window_position_x = _window_position_x;
	window_position_y = _window_position_y;


	float temp_w = width;
	float temp_mw = map_width;
	float temp_h = height;
	float temp_mh = map_height;

	ratio_x = temp_w / temp_mw;
	ratio_y = temp_h / temp_mh;
}

Minimap::~Minimap()
{
	if (ret != nullptr)
	{
		SDL_DestroyTexture(ret);
		ret != nullptr;
	}
	if (base_image != nullptr)
	{
		SDL_FreeSurface(base_image);
		base_image = nullptr;
	}
	points.clear();
	queue.clear();
}


SDL_Texture* Minimap::GetMinimap()
{
	SDL_Surface* manipulation = base_image;

	for (std::list<dot>::iterator it = points.begin(); it != points.end(); it++)
	{
		SDL_Rect representation;

		representation.x = ratio_x* it->rect.x;
		representation.y = ratio_y* it->rect.y;
		representation.w = ratio_x* it->rect.w;
		representation.h = ratio_y* it->rect.h;

		SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, it->color.b, it->color.g, it->color.r));

	}

	ret = SDL_CreateTextureFromSurface(App->render->renderer, manipulation);
	return ret;
}

void Minimap::DrawMinimap()
{
	//this square is the background so that it looks clear
	App->render->DrawQuad({ -App->render->camera.x + window_position_x - 5,-App->render->camera.y + window_position_y - 5,width + 10,height + 10 }, Grey);
	//thi is the texture we will be blitting, a copy of the actual base, so that way we are not modifying it
	SDL_Surface* manipulation = new SDL_Surface();
	manipulation = SDL_ConvertSurface(base_image, base_image->format, SDL_SWSURFACE);

	//we first blit the sprites that the player asked for to be in the map for this frame
	for (std::list<sprite_to_blit>::iterator it = queue.begin(); it != queue.end(); it++)
	{
		SDL_Rect r = { it->section.x*ratio_x, it->section.y *ratio_y, it->section.w, it->section.h };
		SDL_BlitSurface(it->sprite, NULL, manipulation, &r);
	}
	queue.clear();

	//now we begin with the points
	for (std::list<dot>::iterator it = points.begin(); it != points.end(); it++)
	{
		SDL_Rect representation;
		// we multiply the rectangle by the ratio so that it gets reduced to minimap size
		representation.x = ratio_x * it->rect.x;
		representation.y = ratio_y * it->rect.y;
		representation.w = ratio_x * it->rect.w;
		representation.h = ratio_y * it->rect.h;

		//now we make it fit inside the modifiable texture
		SDL_FillRect(manipulation, &representation, SDL_MapRGB(manipulation->format, it->color.r, it->color.g, it->color.b));
	}

	points.clear();

	//now we will blit the viewport representation on the minimap
	SDL_Rect up = {-App->render->camera.x * ratio_x,-App->render->camera.y * ratio_y ,App->render->camera.w * ratio_x, 1};
	SDL_FillRect(manipulation, &up, SDL_MapRGB(manipulation->format, 255, 255, 255));

	SDL_Rect down = {-App->render->camera.x * ratio_x,-(App->render->camera.y-App->render->camera.h) * ratio_y -1 ,App->render->camera.w * ratio_x, 1 };
	SDL_FillRect(manipulation, &down, SDL_MapRGB(manipulation->format, 255, 255, 255));

	SDL_Rect left = {-App->render->camera.x * ratio_x,-App->render->camera.y * ratio_y ,1 , App->render->camera.h * ratio_y };
	SDL_FillRect(manipulation, &left, SDL_MapRGB(manipulation->format, 255, 255, 255));

	SDL_Rect right = {-(App->render->camera.x -App->render->camera.w) * ratio_x - 1 , - App->render->camera.y * ratio_y ,1, App->render->camera.h* ratio_y };
	SDL_FillRect(manipulation, &right, SDL_MapRGB(manipulation->format, 255, 255, 255));
	
	//we create the texture
	ret = SDL_CreateTextureFromSurface(App->render->renderer, manipulation);
	// we blit it
	App->render->Blit(ret,window_position_x - App->render->camera.x, window_position_y - App->render->camera.y);
	//free everything to avoid leaks
	SDL_DestroyTexture(ret);
	SDL_FreeSurface(manipulation);
	manipulation = nullptr;
	ret = nullptr;

}
void Minimap::Addpoint(SDL_Rect rect, SDL_Color color) 
{
	dot p;
	p.rect = rect;
	p.color = color;

	points.push_back(p);
}

void Minimap::Mouse_to_map(int& map_x, int& map_y)// returns -1 if unsuccesfull
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

void Minimap::Draw_Sprite(SDL_Surface* img, int pos_x, int pos_y)
{

	sprite_to_blit _sprite;

	_sprite.sprite = img;
	_sprite.section.x = pos_x;
	_sprite.section.y = pos_y;
	_sprite.section.w = img->w;
	_sprite.section.h = img->h;

	queue.push_back(_sprite);

}