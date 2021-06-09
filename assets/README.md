## `background` file
A `background` (or `map`, for short) file can be any type of bitmap image.
Each pixel on the image represent a space on the simulation world. [World] and
[Seeder] will parse the image to look for specific colors, which represent
special locations, called _points of interest_ or _POIs_ for short.

What follows is a list of every specific color and its meaning. For a
`background` file to be considered valid, it needs to contain at least one of
each of these colors. Additional colors are ignored.

|   Color   |   RGB code   |   meaning   |
|-----------|--------------|-------------|
| ![home]   | `#FFFFFF`    | House       |
| ![work]   | `#0000CC`    | Work        |
| ![school] | `#FFAA00`    | School      |
| ![uni]    | `#FF5500`    | University  |
| ![party]  | `#FF00FF`    | Party       |
| ![shop]   | `#FFFF00`    | Shop        |
| ![walk]   | `#00FF00`    | Park        |
| ![road]   | `#EEEEEE`    | Road        |

Note that _Road_ is currently unused, but is reserved for future possible
upgrades.

## `entities` file
TODO

[World]: ../README.md#world
[Seeder]: ../README.md#seeder

[home]: md/POI/home.png
[work]: md/POI/work.png
[school]: md/POI/school.png
[uni]: md/POI/uni.png
[party]: md/POI/party.png
[shop]: md/POI/shop.png
[walk]: md/POI/walk.png
[road]: md/POI/road.png
