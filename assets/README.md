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

--------------------------------------------------------------------------------

## `entities` file
A `entities` file is a text file which contains a list of entities and their
properties.

Below is an example file with explanation (note that comments render the file
invalid).

```
[count]     <-- [count] keyword must appear only once at the beginning
1               of the file, followed by the number of entities that are
                present in this file.
              

[entity]    <-- Each entity starts with a [entity] keyword.
[infected]  <-- If the keyword [infected] appears within the entity body,
                the entity will be infected at the start of the simulation.
                
uid=0       <-- These properties are required for every entities.
homex=435
homey=530
workx=456
worky=497   <-- End of required properties.

symptoms_resistance=0.3    <-- These properties are optional (a default value
virus_spread_chance=0.02       will be assigned).
infection_resistance=0.8
ai=manAI                   <-- End of optional properties.
[/entity]   <-- Each entity ends with a [/entity] keyword.
```

I also provided a [sample `entities` file][A]. An explanation on how each
property affects an entity is found on [Entity] class code comments.

--------------------------------------------------------------------------------

[World]: ../README.md#world
[Seeder]: ../README.md#seeder
[Entity]: ../README.md#entity

[home]: md/POI/home.png
[work]: md/POI/work.png
[school]: md/POI/school.png
[uni]: md/POI/uni.png
[party]: md/POI/party.png
[shop]: md/POI/shop.png
[walk]: md/POI/walk.png
[road]: md/POI/road.png

[A]: entities.sample.txt
