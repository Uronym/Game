# Game

Yes, that's actually the name of the project, for now. A better name is a top priority once the project is large enough to justify one!

## Controls

    Key | Effect
-------:|:-------
 Arrows | Move/attack
      G | Pick up item underfoot
      I | Wield first item in inventory
      Q | Quit/exit program
      U | Use first item in inventory

## Compile

#### Linux (g++):

```
g++ *.cpp *.h -o Game.out $(pkg-config --cflags --libs allegro-5.0 allegro_audio-5.0 allegro_acodec-5.0 allegro_image-5.0 allegro_primitives-5.0) -std=c++0x -Wall
```
