#ifndef __j1ENTITYCONTROLLER_H__
#define __j1ENTITYCONTROLLER_H__

#include "j1Module.h"
#include "Building.h"

#include <list>
#include <map>

class Squad;

class j1EntityController : public j1Module
{
public:
	j1EntityController();
	~j1EntityController();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	bool loadEntitiesDB(pugi::xml_node& data);
	
	void DeleteEntity(Entity* entity);
	bool DeleteDB() { return true; };

	Building* addBuilding(iPoint pos, buildingType type );

	void placingBuilding(buildingType type);
	std::vector<Entity*> CheckCollidingWith(Entity* entity);

	iPoint CameraToWorld(int x, int y);

public:

	std::list<Entity*> entities;
	std::list<Entity*> selected_entities;

	std::list<Entity*> entities_to_destroy;
	
	std::map<uint, Building*> buildingDB;

	bool debug = false;
	bool building = false;
	SDL_Rect selection_rect = { 0,0,0,0 };
};



#endif // !

