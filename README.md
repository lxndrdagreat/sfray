# sfray
Old-school FPS raycast engine written in C++11 and using SFML 2.3.

Please note that this is _not_ a complete game or even really a game
at all. This project focuses on the rendering engine for the raycasting.
You can build off of this to add movement, combat and the other things
that make a real game. The included example showcases how to do a few
of these things.

# Features

- Ability to render walls, floors and ceilings as textures or colors
- Rendering of sprite objects (`Entity` class)

# Roadmap
These are things I would like to explore and try to figure out, when
I have more time available to do so.

- Walls with different heights
- Animated entities
- Animated walls and floors/ceilings
- More performance increases
- More rendering methods and options to give more choices to the
developer as to where to cut corners

# Resources

Check out my random Youtube videos that show sort of snapshots of the
development of the engine:

- [Simple Colored Walls](https://youtu.be/U3x_njx409E)
- [Textured Walls and Floors](https://youtu.be/DpW7sF1EK_g)
- [BSP Map Generation and Floor Casting Performance Boost. First Sprite.](https://youtu.be/ZMAJb8Aw1-0)
- [Engine codebase restructure](https://youtu.be/NEHo7yX6vns)
- [Major performance increase](https://youtu.be/_0zJWetBIL8)

And [here is probably the absolute best tutorial](http://lodev.org/cgtutor/raycasting.html)
for a starting place. Note: this tutorial was *not* done in SFML, but the
basic idea is how this is done and what I based my version off of.

# Example Program
The example program (located in `main.cpp`) shows basic usage. I've tried to
clean it up and add comments.

## Demo Features

- Floor, ceiling and wall rendering methods
- Toggle entity drawing on and off
- Camera movement and collision
- Overlay "gun" example (defaults to hidden, see keybindings to key to toggle
it to visible).
- Frames-per-second output to help debug performance gains/losses.

## Keybindings

- `Arrow Keys`: movement/rotation
- `Z`: Cycle floor rendering method
- `X`: Cycle ceiling rendering method
- `C`: Cycle wall rendering method
- `V`: Cycle entities on and off
- `B`: Cycle overlay example on and off

# Raycaster

## Rendering Options
There are different rendering "options" for walls, floor, ceiling and entities.

### Walls

- None
- Colored
- Textured

### Floor

- None
- Colored
- Textured

### Ceiling

- None
- Colored
- Textured

### Entities

- None
- Textured

# License
The MIT License (MIT). See LICENSE for more info.

*Art assets are not my creation nor do I claim any ownership.*