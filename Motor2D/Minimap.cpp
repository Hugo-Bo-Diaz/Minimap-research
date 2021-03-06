#include "Minimap.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

Minimap::Minimap(const char* base_texture_path, int _window_position_x, int _window_position_y, int _width, int _height,  int _minimap_viewport_w , int _minimap_viewport_h , int tex_width, int tex_height)
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

	if (_minimap_viewport_w != -1 && _minimap_viewport_h != -1)
	{
		minimap_viewport_w = _minimap_viewport_w;
		minimap_viewport_h = _minimap_viewport_h;
	}
	else
	{
		minimap_viewport_w = width;
		minimap_viewport_h = height;
	}

	window_position_x = _window_position_x;
	window_position_y = _window_position_y;

	ratio_x = (float)width	/ (float)map_width;
	ratio_y = (float)height	/ (float)map_height;
}

Minimap::~Minimap()
{
	if (base_image != nullptr)
	{
		SDL_FreeSurface(base_image);
		base_image = nullptr;
	}
	point_queue.clear();
	sprite_queue.clear();
}


void Minimap::DrawMinimap()
{
	//this square is the background so that it looks clear(OPTIONAL)
	App->render->DrawQuad({ -App->render->camera.x + window_position_x - 5,-App->render->camera.y + window_position_y - 5,width + 10,height + 10 }, Grey);
	
	//this is the texture we will be blitting, a copy of the actual base, so that way we are not modifying it
	SDL_Surface* manipulable = new SDL_Surface();
	manipulable = SDL_ConvertSurface(base_image, base_image->format, SDL_SWSURFACE);

	//we first blit the sprites that the player asked for to be in the map for this frame
	for (std::list<sprite>::iterator it = sprite_queue.begin(); it != sprite_queue.end(); it++)
	{
		SDL_Rect r = { it->section.x*ratio_x, it->section.y *ratio_y, it->section.w, it->section.h };
		SDL_BlitSurface(it->sprite_surface, NULL, manipulable, &r);
	}
	sprite_queue.clear();

	//now we begin with the points
	for (std::list<point>::iterator it = point_queue.begin(); it != point_queue.end(); it++)
	{
		SDL_Rect representation;
		// we multiply the rectangle by the ratio so that it gets reduced to minimap size
		representation.x = ratio_x * it->rect.x;
		representation.y = ratio_y * it->rect.y;
		representation.w = ratio_x * it->rect.w;
		representation.h = ratio_y * it->rect.h;

		//now we make it fit inside the modifiable texture
		SDL_FillRect(manipulable, &representation, SDL_MapRGB(manipulable->format, it->color.r, it->color.g, it->color.b));
	}

	point_queue.clear();

	//now we will blit the viewport representation on the minimap
	SDL_Rect up = {-App->render->camera.x * ratio_x,-App->render->camera.y * ratio_y ,App->render->camera.w * ratio_x, 1};
	SDL_FillRect(manipulable, &up, SDL_MapRGB(manipulable->format, 255, 255, 255));

	SDL_Rect down = {-App->render->camera.x * ratio_x,-(App->render->camera.y-App->render->camera.h) * ratio_y -1 ,App->render->camera.w * ratio_x, 1 };
	SDL_FillRect(manipulable, &down, SDL_MapRGB(manipulable->format, 255, 255, 255));

	SDL_Rect left = {-App->render->camera.x * ratio_x,-App->render->camera.y * ratio_y ,1 , App->render->camera.h * ratio_y };
	SDL_FillRect(manipulable, &left, SDL_MapRGB(manipulable->format, 255, 255, 255));

	SDL_Rect right = {-(App->render->camera.x -App->render->camera.w) * ratio_x - 1 , - App->render->camera.y * ratio_y ,1, App->render->camera.h* ratio_y };
	SDL_FillRect(manipulable, &right, SDL_MapRGB(manipulable->format, 255, 255, 255));
	
	//we create the texture
	SDL_Texture* texture_to_blit = SDL_CreateTextureFromSurface(App->render->renderer, manipulable);

	// we blit it
	SDL_Rect rec = { minimap_camera_x, minimap_camera_y,minimap_viewport_w,minimap_viewport_h };

	App->render->Blit(texture_to_blit, window_position_x - App->render->camera.x, window_position_y - App->render->camera.y, &rec );
	//free everything to avoid leaks
	SDL_DestroyTexture(texture_to_blit);
	SDL_FreeSurface(manipulable);
	manipulable = nullptr;
}
void Minimap::Addpoint(SDL_Rect rect, SDL_Color color) 
{
	point p;
	p.rect = rect;
	p.color = color;

	point_queue.push_back(p);
}

void Minimap::Draw_Sprite(SDL_Surface* img, int pos_x, int pos_y)
{

	sprite _sprite;

	_sprite.sprite_surface = img;
	_sprite.section.x = pos_x;
	_sprite.section.y = pos_y;
	_sprite.section.w = img->w;
	_sprite.section.h = img->h;

	sprite_queue.push_back(_sprite);

}

void Minimap::Mouse_to_map(int& map_x, int& map_y)// returns -1 in the variables if unsuccesfull
{
	int mouse_x, mouse_y;
	App->input->GetMousePosition(mouse_x, mouse_y);

	if (	mouse_x > window_position_x && mouse_x < window_position_x + width
		&&	mouse_y > window_position_y && mouse_y < window_position_y + height)
	{
		map_x = (mouse_x - window_position_x)/ ratio_x + minimap_camera_x/ratio_x;
		map_y = (mouse_y - window_position_y) / ratio_y + minimap_camera_y/ratio_y;
	}
	else
	{
		map_x = -1;
		map_y = -1;
	}
}
