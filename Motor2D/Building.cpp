#include "Building.h"
#include "j1Render.h"
#include "j1Scene.h"

Building::Building(iPoint pos, Building& building)
{
	name							= building.name;
	texture							= building.texture;
	type							= building.type;

	current_HP						= building.current_HP;
	max_HP							= building.max_HP;
	villagers_inside				= building.villagers_inside;
	cooldown_time					= building.cooldown_time;
	defense							= building.defense;
	
	entity_type						= BUILDING;
	position.x = pos.x, position.y	= pos.y;
}

Building::~Building()
{

}

bool Building::Update(float dt)
{
	App->render->Blit(texture, position.x, position.y, section);

	SDL_Color c;
	c.a = 255;
	c.r = 0;
	c.b = 0;
	c.g = 255;
	App->scene->minimap->Addpoint({(int)position.x, (int)position.y,96,95 }, c);

	return true;
}

void Building::Draw()
{

}