#include "j1EntityController.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "Entity.h"
#include "PugiXml/src/pugixml.hpp"
#include "j1Textures.h"
#include "j1Input.h"

j1EntityController::j1EntityController() { name = "entitycontroller"; }

j1EntityController::~j1EntityController() {}

bool j1EntityController::Start()
{
	pugi::xml_document doc;
	pugi::xml_node gameData;

	gameData = App->LoadFile(doc, "GameData.xml");

	loadEntitiesDB(gameData);

	//squad_units_test.push_back(addUnit(iPoint(900, 1000), FOOTMAN));
	//squad_units_test.push_back(addUnit(iPoint(1000, 800), FOOTMAN));
	//squad_units_test.push_back(addUnit(iPoint(1000, 1000), FOOTMAN));
	//squad_units_test.push_back(addUnit(iPoint(900, 800), FOOTMAN));

	//addUnit(iPoint(900, 700), HERO_1);

	//addBuilding(iPoint(700, 700), BARRACKS);

	return true;
}

bool j1EntityController::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { debug = !debug; App->map->debug = debug; };

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if ((*it)->isActive)
		{
			if (App->render->CullingCam((*it)->position))
			{
				(*it)->Draw(dt);
				if (debug) App->render->DrawQuad((*it)->collider, Green);
			}
			if (!(*it)->Update(dt))	return false;
			SDL_Color c;
			c.a = 255;
			c.r = 0;
			c.b = 0;
			c.g = 255;
			App->scene->minimap->Addpoint({ (int)(*it)->position.x, (int)(*it)->position.y,96,95 },c);

		}
	}

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN && !building)
	{
		building = true;
	}

	else if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN && building)
	{
		
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint pos1 = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
		iPoint pos2 = App->map->MapToWorld(pos1.x, pos1.y);
		
		addBuilding(pos2, BARRACKS);

		SDL_Color coloring;
		coloring.r = 255;
		coloring.g = 255;
		coloring.b = 0;

		App->scene->minimap->Addpoint({ pos2.x,pos2.y,96,95 },coloring);

		building = false;
	}

	if (building)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint pos1 = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
		iPoint pos2 = App->map->MapToWorld(pos1.x, pos1.y);

		App->render->DrawQuad({ pos2.x,pos2.y,buildingDB[1]->section->w,buildingDB[1]->section->h }, Green);
		
	}


	return true;
}

bool j1EntityController::PostUpdate()
{
	for (std::list<Entity*>::iterator it = entities_to_destroy.begin(); it != entities_to_destroy.end(); it++)
	{
		entities.remove(*it);
		selected_entities.remove(*it);
		
		//if((*it)->collider)  DestroyCollider

		delete *it;
	}

	entities_to_destroy.clear();

	return true;
}

bool j1EntityController::CleanUp()
{
	if (!DeleteDB()) return false;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)	RELEASE(*it);

	entities.clear();
	selected_entities.clear();

	return true;
}

void j1EntityController::DeleteEntity(Entity* entity)
{
	entities.remove(entity);
	selected_entities.remove(entity);
}


Building* j1EntityController::addBuilding(iPoint pos, buildingType type)
{
	Building* building = new Building(pos, *(buildingDB[type]));
	entities.push_back(building);

	return building;
}



std::vector<Entity*> j1EntityController::CheckCollidingWith(Entity* entity)
{
	std::vector<Entity*> ret;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if (*it != entity)
			if (SDL_HasIntersection(&entity->collider, &(*it)->collider)) ret.push_back(*it);
	}

	return ret;
}

iPoint j1EntityController::CameraToWorld(int x, int y)
{
	iPoint ret;

	ret.x = x - App->render->camera.x;
	ret.y = y - App->render->camera.y;

	return ret;
}

bool j1EntityController::loadEntitiesDB(pugi::xml_node& data)
{
	pugi::xml_node NodeInfo;

	for (NodeInfo = data.child("Buildings").child("Building"); NodeInfo; NodeInfo = NodeInfo.next_sibling("Building")) {

		Building* buildingTemplate = new Building();
		buildingTemplate->type = (buildingType)NodeInfo.child("type").attribute("value").as_int(0);

		buildingTemplate->name = NodeInfo.child("name").attribute("value").as_string("error");
		buildingTemplate->texture = App->tex->Load(NodeInfo.child("texture").attribute("value").as_string("error"));

		buildingTemplate->current_HP = buildingTemplate->max_HP = NodeInfo.child("Stats").child("life").attribute("value").as_int(0);
		buildingTemplate->villagers_inside = NodeInfo.child("Stats").child("villagers").attribute("value").as_int(0);
		buildingTemplate->cooldown_time = NodeInfo.child("Stats").child("cooldown").attribute("value").as_int(0);
		buildingTemplate->defense = NodeInfo.child("Stats").child("defense").attribute("value").as_int(0);
		
		// TODO building cost outside the DB so it's not unnecessarily repeated on every unit

		buildingDB.insert(std::pair<int, Building*>(buildingTemplate->type, buildingTemplate));
	}

	return true;
}

bool j1EntityController::Save(pugi::xml_node & config) const
{
	return true;
}

bool j1EntityController::Load(pugi::xml_node & config)
{
	return true;
}