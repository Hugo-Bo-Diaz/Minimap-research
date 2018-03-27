#ifndef __UI_PROGRESSBAR__
#define _UI_PROGRESSBAR__

#include "UI_element.h"

enum bar_type
{
	INCREASING,
	DECREASING
};

class ProgressBar : public UI_element
{
public:

	ProgressBar(int x, int y, SDL_Texture* texture, SDL_Rect empty, SDL_Rect full, SDL_Rect head, float max_value, j1Module* callback);

	~ProgressBar()
	{}

	void setProgress(float newProgress);
	float getProgress() const;
	void enterCurrentValue(float current_value);
	void BlitElement(bool use_camera = false);

public:

	SDL_Rect full;
	float max_value = 0.0f;
	float progress = 1.0f;
	SDL_Rect head;
	iPoint head_pos;
	bar_type type;
};

#endif // !__UI_PROGRESSBAR__