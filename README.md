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

![example](https://imgur.com/epGRLl0.png)

## Micro game vs Macro game

Micro game refers to the gameplay that is skill-based, like managing units or making the input for a shoryuken, this gameplay can be executed correctly without taking in consideration the larger scale of the game. To simplify, the manouverability a player has with the controls/inputs in the game.

Macro game, on the other hand, refers to the awareness of the player about his/her surroundings and what is happening on the game on a larger scale and how he/she reasons the information to get a decision. To simplify, the tactical and/or strategic decisions the player takes.

![](https://imgur.com/2cDB3Nr)

A minimap can help diferentiate these two types of gameplay to the player as well as making it easier to go from one to another, it will provide macro information but what is displayed on screen usually correspond to the micro game therefore it help balance them a little bit better than only giving the player the option to open a map which can be more difficult to the more inexperienced players.

## Does my game need a minimap?

It depends a lot on the game you are trying to develop, but I'll deliver general guidelines for when a game usually needs a minimap 

- Tactical genres

- Games with big open worlds

- Games that can be disorienting to the player

- You want to make a distinction between micro and macro gameplay

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

## Next section

```
code?
```

# Bibliography

http://www.gamemakers.com/micro-vs-macro-consolepc-vs-mobile-gaming/
##
