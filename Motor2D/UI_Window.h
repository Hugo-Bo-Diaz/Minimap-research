#ifndef __UI_WINDOW__
#define __UI_WINDOW__

#include "UI_element.h"

class Window : public UI_element
{
public:

	Window()
	{}

	Window(SDL_Texture* texture, int x, int y, SDL_Rect section, j1Module* callback): UI_element(x, y, element_type::WINDOW, section, callback, texture)
	{}

	~Window()
	{}

	void BlitElement();

};

#endif // !__UI_WINDOW__