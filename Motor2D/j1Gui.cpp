#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Entity.h"
#include "UI_element.h"
#include "UI_Window.h"
#include "j1Window.h"
#include "j1UIScene.h"
#include "j1EntityController.h"

j1Gui::j1Gui() : j1Module()
{
	name = "gui";
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	buttonFX = conf.child("buttonFX").attribute("source").as_string("");
	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	icon_atlas_file_name = conf.child("icon_atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.c_str());
	icon_atlas = App->tex->Load(icon_atlas_file_name.c_str());

	pugi::xml_document doc;
	pugi::xml_node gameData;

	gameData = App->LoadFile(doc, "GameData.xml");


	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;
	
	//SDL_SetTextureAlphaMod(atlas, alpha_value);

	UI_element* element = nullptr;

	//Get element to interact with
	if (draggingElement != nullptr)
		element = draggingElement;
	else
	{
		for (std::list<menu*>::reverse_iterator it_m = App->uiscene->menus.rbegin(); it_m != App->uiscene->menus.rend(); it_m++) //Go through menus
		{
			if ((*it_m) == nullptr) break;
			if ((*it_m)->active == false) continue;
			for (std::list<UI_element*>::iterator it_e = (*it_m)->elements.begin(); it_e != (*it_m)->elements.end(); it_e++) //Go through elements
			{
				if (checkMouseHovering((*it_e)))
					element = (*it_e);
				for (std::list<UI_element*>::iterator it_c = (*it_e)->childs.begin(); it_c != (*it_e)->childs.end(); it_c++)
					if (checkMouseHovering((*it_c)))
						element = (*it_c);
			}
		}
	}

	//Send events related to UI elements
	if (element != nullptr)
	{
		if (!element->hovering)
		{
			element->hovering = true;
			if (element->callback != nullptr)
				element->callback->OnUIEvent(element, MOUSE_ENTER);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			clickedOnUI = true;
			if (element->callback != nullptr)
			{
				ret = element->callback->OnUIEvent(element, MOUSE_LEFT_CLICK);
			}

			if (element->dragable)
			{
				element->Start_Drag();
				draggingElement = element;
			}
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			if (element->callback != nullptr)
			{
				element->callback->OnUIEvent(element, MOUSE_LEFT_RELEASE);
			}
			if (element->dragable)
			{
				element->End_Drag();
				draggingElement = nullptr;
			}
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			if (element->callback != nullptr)
				ret = element->callback->OnUIEvent(element, MOUSE_RIGHT_CLICK);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			if (element->callback != nullptr)
				element->callback->OnUIEvent(element, MOUSE_RIGHT_RELEASE);
		}
	}

	return ret;
}

bool j1Gui::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		UI_Debug = !UI_Debug;

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		clickedOnUI = false;

	//Draw selection quads
	for (std::list<Entity*>::iterator it_e = App->entitycontroller->selected_entities.begin(); it_e != App->entitycontroller->selected_entities.end(); it_e++)
		App->render->DrawQuad((*it_e)->collider, Green, false);
	
	//Draw elements of active menus
	for (std::list<menu*>::iterator it_m = App->uiscene->menus.begin(); it_m != App->uiscene->menus.end(); it_m++)
	{
		if ((*it_m) == nullptr) break;
		if (!(*it_m)->active) continue;
		for (std::list<UI_element*>::iterator it_e = (*it_m)->elements.begin(); it_e != (*it_m)->elements.end(); it_e++)
		{
			if ((*it_e)->moving)
			{
				(*it_e)->Mouse_Drag();
				(*it_e)->state = CLICKED;
			}

			if ((*it_e)->parent == nullptr) //If it has a parent, the parent will be responsible for drawing it
				(*it_e)->BlitElement();
		}
	}
	//Draw Debug
	if (UI_Debug)
		UIDebugDraw();
	
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	if (atlas != nullptr)
	{
		App->tex->UnLoad(atlas);
		atlas = nullptr;
	}
	if (icon_atlas != nullptr)
	{
		App->tex->UnLoad(icon_atlas);
		icon_atlas = nullptr;
	}


	//Windows
	std::list<Window*>::iterator it_w;
	it_w = Windows.begin();
	while ((*it_w) != nullptr && it_w != Windows.end())
	{
		RELEASE((*it_w));
		it_w++;
	}
	Windows.clear();

	return true;
}

bool j1Gui::checkMouseHovering(UI_element* element)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	int scale = App->win->GetScale();
	bool ret = false;

	iPoint globalPos = element->calculateAbsolutePosition();
	if (x >= globalPos.x && x <= globalPos.x + element->section.w / scale && y >= globalPos.y && y <= globalPos.y + element->section.h / scale && element->interactive)
	{
		ret = true;
	}
	else if (element->hovering)
	{
		element->hovering = false;
		if (element->callback != nullptr)
			element->callback->OnUIEvent(element, MOUSE_LEAVE);
	}

	return ret;
}

void j1Gui::UIDebugDraw()
{
	for (std::list<menu*>::iterator it_m = App->uiscene->menus.begin(); it_m != App->uiscene->menus.end(); it_m++)
	{
		if ((*it_m) == nullptr) break;
		if (!(*it_m)->active) continue;
		for (std::list<UI_element*>::iterator it_e = (*it_m)->elements.begin(); it_e != (*it_m)->elements.end(); it_e++)
		{
			SDL_Rect box;
			int scale = App->win->GetScale();
			box.x = (*it_e)->calculateAbsolutePosition().x * scale;
			box.y = (*it_e)->calculateAbsolutePosition().y * scale;
			box.w = (*it_e)->section.w;
			box.h = (*it_e)->section.h;
			App->render->DrawQuad(box, Red, false, false);
		}
	}

}
// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

const SDL_Texture* j1Gui::GetIconAtlas() const
{
	return icon_atlas;
}

UI_element* j1Gui::GetElement(int type, int id)
{
	UI_element* ret = nullptr;
	switch (type)
	{
	case WINDOW:
		ret = (*std::next(Windows.begin(), id));
		break;
	}

	return ret;
}

void j1Gui::Load_UIElements(pugi::xml_node node, menu* menu, j1Module* callback, UI_element* parent)
{
	BROFILER_CATEGORY("Load UI elements", Profiler::Color::Chocolate);

	pugi::xml_node tmp;

	tmp = node.first_child();
	UI_element* element = nullptr;
	for (; tmp; tmp = tmp.next_sibling())
	{
		std::string type = tmp.name();

		if (type == "window")
			element = createWindow(tmp, callback);


		pugi::xml_node childs = tmp.child("childs");
		if(childs)
		{ 
			Load_UIElements(childs, nullptr, callback, element);
		}
		
		if (parent != nullptr)
		{
			parent->appendChild(element, tmp.attribute("center").as_bool());
		}

		if (menu != nullptr)
			menu->elements.push_back(element);
	}
}

Window* j1Gui::createWindow(pugi::xml_node node, j1Module * callback, bool saveIntoGUI)
{
	SDL_Texture* texture = nullptr;
	if (node.attribute("path"))
		texture = App->tex->Load(node.attribute("path").as_string());
	else
		texture = atlas;

	int x = node.child("position").attribute("x").as_int();
	int y = node.child("position").attribute("y").as_int();
	SDL_Rect section = { node.child("section").attribute("x").as_int(), node.child("section").attribute("y").as_int(), node.child("section").attribute("w").as_int(), node.child("section").attribute("h").as_int() };

	Window* ret = new Window(texture, x, y, section, callback);

	if (saveIntoGUI)
		Windows.push_back(ret);

	return ret;
}

void j1Gui::AddIconData(buildingType type, pugi::xml_node node)
{
	SDL_Rect rect = { node.attribute("x").as_int(), node.attribute("y").as_int(), node.attribute("w").as_int(), node.attribute("h").as_int() };
	buildingIconRect.insert(std::pair<buildingType, SDL_Rect>(type, rect));
}

SDL_Rect j1Gui::GetIconRect(Entity* entity)
{
	switch (entity->entity_type)
	{
	case BUILDING:
		return buildingIconRect.at(((Building*)entity)->type);
		break;
	default:
		return { 0,0,0,0 };
		break;
	}
}