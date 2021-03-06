#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityController.h"
j1Scene::j1Scene() : j1Module() { name = "scene"; }

// Destructor
j1Scene::~j1Scene() {}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	return true;
}

// Called before the first frame
bool j1Scene::Start()
{
	int w = -1, h = -1;
	uchar* data = nullptr;

	App->map->Load_map("map1.tmx");

	wood = 200;
	gold = 400;
	
	minimap = new Minimap("base_image.png",200,200,4096,4096,64+32,64+32,-1,-1);
	minimap->window_position_x = App->win->width - minimap->width -5;
	minimap->window_position_y = App->win->height - minimap->height - 5;

	minimap->minimap_camera_x = 0;
	minimap->minimap_camera_y = 0;

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;

	App->render->MouseCameraMovement(dt);
	App->map->Draw();

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		int camx, camy;
		minimap->Mouse_to_map(camx,camy);

		if (camx != -1 && camy != -1)
		{
			App->render->camera.y = -camy + App->win->height / 2;
			App->render->camera.x = -camx + App->win->width / 2;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && minimap->minimap_camera_y > 0)
	{
		minimap->minimap_camera_y -= 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && minimap->minimap_camera_x > 0)
	{
		minimap->minimap_camera_x -= 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && minimap->minimap_camera_x < minimap->width - minimap->minimap_viewport_w)
	{
		minimap->minimap_camera_x += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && minimap->minimap_camera_y< minimap->height - minimap->minimap_viewport_h)
	{
		minimap->minimap_camera_y += 1;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	minimap->DrawMinimap();

	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	return true;
}
