# WorkInProgress

I am [Hugo Bó Díaz](https://www.linkedin.com/in/hugo-b%C3%B3-d%C3%ADaz-415628146/), student of [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/). This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu).

# Theory about minimaps

## What is a minimap?

A minimap is a representation of the part where the player is on the game universe that is part of th HUD. It usually takes a top-down perspective, unless the game requires otherwise.

![example](https://imgur.com/epGRLl0.png)

## Minimap vs map

The main difference between a minimap and a map is whether they are the main focus of the player when they are on screen, a map is designed usually to make the player observe the world around them before making a decision, therefore not only are usually bigger but centered so that is catches the atention of the player:

![example](https://imgur.com/nSEaRJI.png)

"expresion to make it look like i know english", A minimap is an element that should not take the center of the screen as they are more like a constant hint of what is going on the whole map of the game so that the player can always be orientated/aware rather than something the player should be looking with full attention.

![example](https://imgur.com/Laowfq0.png)

## Micro game vs Macro game

Micro game refers to the gameplay that is skill-based, like managing units or making the input for a shoryuken, this gameplay can be executed correctly without taking in consideration the larger scale of the game. To simplify, the manouverability a player has with the controls/inputs in the game.

Macro game, on the other hand, refers to the awareness of the player about his/her surroundings and what is happening on the game on a larger scale and how he/she reasons the information to get a decision. To simplify, the tactical and/or strategic decisions the player takes.

![](https://imgur.com/2cDB3Nr.png)

A minimap can help diferentiate these two types of gameplay to the player as well as making it easier to go from one to another, it will provide macro information but what is displayed on screen usually correspond to the micro game therefore it help balance them a little bit better than only giving the player the option to open a map which can be more difficult to the more inexperienced players.

## Does my game need a minimap?

It depends a lot on the game you are trying to develop, but I'll deliver general guidelines for when a game usually needs a minimap 

- Tactical genres

- Games with big open worlds

- Games that can be disorienting to the player

- You want to make a distinction between micro and macro gameplay

##Parts of the minimap

IMAGE HERE WITH PARTS DETAILED

# Implementation

Now that we know all the theory of on what is a minimap and its function we can create our own

## Basics in the code

A minimap and a map are based on the same code concepts, which are comparable to what a regular map does: it shrinks the size of things to represent them in their relative position.

For example we have a map that is 100 units long and we want to represent it in a minimap that is 10 units long. So if we had an object in the coordinate 50 in the real map it would mean that it is halfway because 50/100 = 1/2 of the total map lenght. This means that in our minimap it should also be represented halfway of the minimap: 10*(1/2) = 5.

So our item in the minimap will be in the 5 position. let's see the operation we did here and try to make them easier for a computer:

We divided our world position in the total map distance and that result was multiplied by the total lenght of our minimap. In our code as long as the minimap is active the total map distance and the measures of our minimap are not going to change:

![](https://imgur.com/H49wFHR.png)

so we can calculate a ratio to how things in our minimap measure in the world by dividing our minimap lenght by our map distance.

Now whenever we multiply any real world measure by that ratio it gets transformed into coordinates that we can use on our minimap.

## Coding the minimap

For my example and the excercises I'll be using c++ and the SDL 2.0 library

Our minimap in this example will be a class.

First we need our base image as it is the texture we will be working over, where will our minimap will be on the window and how big is the map we want to represent. Those will be the arguments our constructor will recieve. We are going to store the values needed and load the image first

```
Minimap(const char* _base_texture_path, int _win_pos_x, int _win_pos_y, int _width, int height)
{
  base_image = IMG_Load(_base_texture_path);
  map_width = _width;
  map_height = _height;
  window_position_x = _win_pos_x;
  window_position_y = _win_pos_y;
 ```

now because the SDL_surface has the width and height of the image as a variable we will be getting how big our minimap is from there

```
  width = base_image->w;
  height = base_image->h;
```

the last thing we want to do in this piece of code is calculate the ratios because we already have all the data and is something that'll remain constant in the minimap


```
	ratio_x = (float)width	/ (float)map_width;
	ratio_y = (float)height	/ (float)map_height;
}
```

we cast everything to float because we need the decimals (usually the ratios are a fraction and lower than 1)

Now we need something to represent on our minimap, someone has to give the minimap the points, and that is why this function exists

```
void Minimap::Addpoint(SDL_Rect rect, SDL_Color color) 
{
	point p;
  
  ```
  point is a struct that I created to have everything easily accesible, it has a SDL_Rect and a SDL_Color
  ```
	p.rect = rect;
	p.color = color;
  ```
  the point_queue is a std::list so that we can access easliy all the points to blit them into the surface
  ```
	point_queue.push_back(p);
}
```
Now that we have that basis covered lets start drawing the map:

The first thing we need is to duplicate our base image because if we edited the original we would lose the original form and we wouldn't be able to revert the changes, I called this copy manipulable
```
void Draw_Minimap()
{
  SDL_Surface* manipulable = new SDL_Surface();
	manipulable = SDL_ConvertSurface(base_image, base_image->format, SDL_SWSURFACE);
```

now we want to change this texture to what we want our player to see lets add the points our player has requested

```
	for (std::list<point>::iterator it = point_queue.begin(); it != point_queue.end(); it++)
	{
```
We now reduce the points we have been given to for the minimap

```
		SDL_Rect representation;
		representation.x = ratio_x * it->rect.x;
		representation.y = ratio_y * it->rect.y;
		representation.w = ratio_x * it->rect.w;
		representation.h = ratio_y * it->rect.h;
```
And here we put them in the texture with SDL_FillRect
```
    SDL_FillRect(manipulable, &representation, SDL_MapRGB(manipulable->format, it->color.r, it->color.g, it->color.b));
	}
 
```
Once the points have been transfered into the minimap texture we can proceed to blit this manipulated texture to screen
```
	SDL_Texture* texture_to_blit = SDL_CreateTextureFromSurface(App->render->renderer, manipulable);
	App->render->Blit(texture_to_blit,window_position_x - App->render->camera.x, window_position_y - App->render->camera.y);

```
We now free everything to avoid leaks, this is very important as it would be a considerable leak for each frame!
```
	SDL_DestroyTexture(texture_to_blit);
	SDL_FreeSurface(manipulable);
}

```

The minimap in the repository has some more things like also accepting sprites to blit to the minimap and also is interactive with the mouse, I suggest looking the repository or the release for excercises if you are interested in learning more

# Bibliography

http://www.gamemakers.com/micro-vs-macro-consolepc-vs-mobile-gaming/
##
