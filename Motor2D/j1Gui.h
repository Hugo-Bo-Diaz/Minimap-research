#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_rect.h"
#include "Unit.h"
#include "Building.h"
#include "Nature.h"
#include <map>
#include <string>
#include <list>

struct _TTF_Font;
struct SDL_Texture;
class UI_element;
class Window;
class menu;
class Entity;

enum event_type
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_RIGHT_CLICK,
	MOUSE_RIGHT_RELEASE,
	MOUSE_LEFT_CLICK,
	MOUSE_LEFT_RELEASE,
	TIMER_ZERO,
	STOPWATCH_ALARM
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool checkMouseHovering(UI_element* element);

	void UIDebugDraw();
	// Gui creation functions
	const SDL_Texture* GetAtlas() const;
	const SDL_Texture* GetIconAtlas() const;
	//Type:
	//TEXT - 0
	//IMAGE - 1
	//BUTON - 2
	//WINDOW - 3
	//CHRONO - 4
	//PROGRESSBAR - 5
	UI_element* GetElement(int type, int id);

	void Load_UIElements(pugi::xml_node node, menu* menu, j1Module* callback = nullptr, UI_element* parent = nullptr);
	//NULL texture to use atlas
	Window* createWindow(pugi::xml_node node, j1Module* callback = nullptr, bool saveIntoGUI = true);

	void AddIconData(unitType type, pugi::xml_node node);
	void AddIconData(buildingType type, pugi::xml_node node);
	void AddIconData(resourceType type, pugi::xml_node node);
	SDL_Rect GetIconRect(Entity* entity);
	SDL_Rect GetLifeBarRect(std::string tag);


public:
	bool UI_Debug = false;
	int alpha_value = 255;
	uint button_click_fx = 0;
	bool clickedOnUI = false;

private:

	SDL_Texture* atlas=nullptr;
	SDL_Texture* icon_atlas = nullptr;
	std::string atlas_file_name;
	std::string icon_atlas_file_name;
	std::string	buttonFX;
	std::list<Window*> Windows;
	UI_element* draggingElement = nullptr;
	std::map<unitType, SDL_Rect> unitIconRect;
	std::map<buildingType, SDL_Rect> buildingIconRect;
	std::map<resourceType, SDL_Rect> resourceIconRect;

};

#endif // __j1GUI_H__