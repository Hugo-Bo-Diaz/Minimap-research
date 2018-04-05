#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1UIScene.h"
#include "j1EntityController.h"
#include "j1Gui.h"
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

	debug_tex = App->tex->Load("maps/Navigable.png");

	pugi::xml_document	Gui_config_file;
	pugi::xml_node		guiconfig;

	guiconfig = App->LoadFile(Gui_config_file, "Gui_config.xml");
	guiconfig = guiconfig.child("scene");

	wood = 200;
	gold = 400;
	
	minimap_test = new Minimap("base.png",200,200,4096,4096, -1,-1);
	//minimap_test = new Minimap("", 4096, 4096, 198, 198);

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene update", Profiler::Color::Black);

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;

	App->render->MouseCameraMovement(dt);
	App->map->Draw();


	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		int camx, camy;
		minimap_test->Mouse_to_map(camx,camy);

		if (camx != -1 && camy != -1)
		{
			App->render->camera.y = -camy + App->win->height / 2;
			App->render->camera.x = -camx + App->win->width / 2;
		}
	}

	lmao += 10;

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{

	SDL_Color c;
	c.r = 255;
	c.b = 0;
	c.a = 255;
	c.g = 0;

	minimap_test->Addpoint({ lmao,lmao,100,100 }, c);
	minimap_test->DrawMinimap();

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
