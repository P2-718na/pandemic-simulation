## `background` file
A `background` (or `map`, for short) file can be any type of bitmap image.
Each pixel on the image represent a space on the simulation world. [World] and
[Seeder] will parse the image to look for specific colors, which represent
special locations, called _points of interest_ or _POIs_ for short.

What follows is a list of every specific color and its meaning. For a
`background` file to be considered valid, it needs to contain at least one of
each of these colors. Additional colors are ignored.

<span style="color:blue">some *blue* text</span>. asd

## `entities` file
TODO

[World]: ../README.md#world
[Seeder]: ../README.md#seeder
