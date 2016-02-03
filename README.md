# sfray
Old-school FPS raycast engine written in C++11 and using SFML 2.3.

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
clean it up and add comments. You can move with the arrow keys, and cycle the
different rendering modes for the walls, floor, ceiling and entities using
the Z, X and C keys. Escape quits the program.

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