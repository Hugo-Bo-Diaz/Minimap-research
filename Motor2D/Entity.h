#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "p2Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"

enum entityType
{
	NONE_ENTITY,
	UNIT,
	BUILDING,
	NATURE,

	// IMPORTANT: this MUST be used only inside j1EntityController::getSelectedType
	UNIT_AND_BUILDING
};

class Entity
{
public:
	fPoint position = { 0.0f, 0.0f };
	SDL_Rect collider = { 0,0,0,0 };
	SDL_Texture* texture = nullptr;
	bool isActive = true;
	std::string name;
	entityType entity_type = NONE_ENTITY;

public:
	virtual ~Entity();

	iPoint getSize();
	void setActive(bool active) { isActive = active; };
	void selectedDraw();
	bool Collides(Entity& other);

	virtual bool Update(float dt)	{ return true; };
	virtual void Draw(float dt)		{};

	virtual bool Save()		{ return true; };
	virtual bool Load()		{ return true; };
};

#endif