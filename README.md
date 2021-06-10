<div align="center">
  <img alt="Alma Mater Studiorum Logo" src="assets/md/unibo-logo.png">
</div>

# pandemic-simulation
[This][0] is my attempt at making a parametric pandemic simulation. Given a
_map_ (a bitmap image with highlighted points of interest) and a list of
_people_, this program will attempt to simulate the evolution of a pandemic.

The goal I set for this project was to try and emulate a human-like behaviour
for the people in my simulation, instead of relying on a random-based approach.
I also wrote my code with expandability in mind: adding additional behaviour
rules or simulation features is easy and it can be done by working directly on
the existing code.

This is the second part of my coding exam [final project][1] at University of
Bologna, year one physics course. All the code in this project
(excluding external libraries) is written entirely by [me (Matteo Bonacini)][2].

This is a GIF of this program running with `1000` entities in the city of
Bologna (click to enlarge).
<div align="center">
  <img alt="Program running" src="assets/md/running.gif" width="400">
</div>

--------------------------------------------------------------------------------

## Dependencies
- [Lyra](https://github.com/bfgroup/Lyra) (bundled)
- [Doctest](https://github.com/onqtam/doctest) (bundled)
- [SFML](http://www.sfml-dev.org/) (required)
- [CMake](https://cmake.org/) (recommended)

## Building
Make sure to install all the required dependencies before continuing.

The preferred way to build this code is by using CMake. Although it is
recommended to build the program in `Release` configuration for better
performance, only the `Debug` configuration satisfies all the requirements
imposed by the project assignment (`-g` and `-fsanitize="address"` flags are
only present in the latter).

```shell
# Clone the repo
git clone git@github.com:P2-718na/pandemic-simulation.git

# Create and cd to build directory
take pandemic-simulation/build

# Prepare build files. Use "Debug" instead of "Release" to build in debug mode.
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build everything
make
```

This will configure all the needed files. Three executables will be generated
(see [Running] for additional information on what they do).

```bash
pandemic  # Pandemic simulation executable

seed      # Tool to randomly generate a list of people for the simulation
          
test      # Run tests
```

## Running
([tl;dr] at the end of this section for a quick, automatic setup).

For the simulation to run, it requires a `background` and a `entities` file.
`background` (or `map`) is an image which contains _points of interest_ (often
referred as _POIs_ in code and comments) such as houses, parks and shops.
`entities` is a text file, which contains specific information for every person
(or _entity_) in the simulation. To keep things simpler, I already
included an example background file, but you can go ahead and create your own if
you prefer. You do not need to create a `entities` file, since it will be
generated automatically (see below).

You can learn how to create a `background` and an `entities` file manually
by looking at [`background` file] and [`entities` file].

The following sections will teach you how to launch the simulation correctly.
Every command assumes that you have successfully built the program and are in
the build directory (see [Building]).

This code was tested and running on _Mac OS X 10.14.6 Mojave_ and
_Ubuntu 20.04 LTS (running on WSL with X server enabled)_.

### Generate a list of people
The `entities` file can be written manually, extrapolated from real world data
or generated randomly. For this project, I included an utility (_seeder_) that
can generate an arbitrary number of entities to use in the simulation.

```bash
# Show help on how to run the seeder
./seed --help

# Example command (generate 1000 entities, based on background.sample.bmp)
./seed -b background.sample.bmp -t 1000 -i 5 -o entities.txt
```

The `background` file that the seeder requires is the same that will be used
later to launch the simulation. The reason for this is that the seeder
needs to have a way to know where POIs are located, and the background
image contains precisely this information.

Even though this might seem redundant, I purposely chose this approach because
of many reasons. Most notably, I wanted to keep the _simulation engine_ and
_simulation data_ parts of my project separated. This way, one can have
different ways to generate the entities list for the same map.

### Launch the simulation
Once you have your `entities` and `background` files ready, you can run the
simulation.

```bash
./pandemic --background background.sample.bmp --entities entities.txt
```

Make sure to check the console for additional output and to have an X server
available to display the graphics.

Once the simulation is running, you can input some additional commands by
typing on your keyboard while the windows is in focus:
- `L` key toggles _lockdown_ status. While lockdown is active, parks will be
      closed and entities will go outside less frequently.
- `D` key cycles window framerate. An higher framerate will use more CPU time
      to draw on the screen, but a lower framerate will skip some simulation
      loops between each frame.
- `K` key toggles daylight cycle.

### Testing
To run tests, run
```bash
./test
```

### tl;dr
Since there are many steps to running this program, I added an helper script to
quickly setup everything. After building, run  
 
```bash 
./quick-run.sh
```

from the CMake build directory. (Make sure the file is executable, if needed use
`chmod +x quick-run.sh`).

--------------------------------------------------------------------------------

## Components
What follows is a quick overview of every component of this project. More
information can be found in code comments. I didn't include a detailed
description of every class method, since the comments in the code do a
sufficiently good job at explaining just that.

### Engine
Class that handles drawing the simulation to screen, input and output. This is
responsible for:

- Printing output to the console
- Creating and updating the rendering window
- Drawing the daylight cycle tint
- Stepping forward the simulation
- Handling user input

For now, this is mostly just a wrapper for SFML-related code, but it can be
expanded with more features. Some possible upgrades are:

- Print message directly on the screen instead of the console
- Add a proper GUI instead of relying on keyboard-based input

Follows a small legend for the entities display color.

|   Color   |   Meaning   |
|-----------|-------------|
| ![purple] | Alive       |
| ![red]    | Infected    |
| ![black]  | Dead        |
| ![cyan]   | Immune      |


### World
Class that handles the _simulation world_. This class contains the list of
entities in the simulation, and has access to the map image. Entities can query
the world to get POI coordinates, current time of day and current day of the
week. This class also handles virus spread.

The simulation can be stepped forward by calling the `World::loop()` method,
which will advance the time, call the `Entity::loop()` method for every entity
and spread the virus. Every `World::loop()` call will advance the time by
one minute (arbitrary unit). After a configurable number of minutes, a day
passes.

At the end of each day, some additional calculations are performed: `World` will
call `Entity::dayLoop()` method for each entity and it will handle quarantine
logic. Infected entities will be put into quarantine, and quarantined entities
that defeated the virus will be put out of quarantine.

### Entity
Each simulation entity represents a _person_. This class handles everything
related to that. Each `Entity` instance needs to be created inside a `World`
object and holds a pointer to it.

One person needs to have a home, a _work location_ and a set of behaviour rules
(referred to as _AI_ in the code). The _work location_ can actually refer to 
three different things:

1. Actual work location
2. University location
3. School location

and each entity AI variant will handle this accordingly. I chose this approach
because one person cannot be both a student or a working person at the same
time, so there is no conflict in having a single variable for all of these
POIs. In case this was not clear, I provided some examples:  
  
1. A person with an _university student_ AI will have its _work location_ set
   to an university location. Its AI will call the `goWork()` method when the
   person needs to go to university.
2. A person with a _working man_ AI will have its _work location_ set to a
   generic working location. Its AI will call the `goWork()` method when the
   person needs to go to work.

Every entity also contains some additional virus-related stats
(virus spread chance, symptoms resistance, infection resistance, infected
and infective status).

### Pathfinder
Class that handles pathfinding for entities. By pathfinding, I mean generating
the list of steps an entity needs to take to get from a point to another point
in the map. Each entity has its own pathfinder instance which can be used to
query this data.

Right now, the pathfinding algorithm is very simple: the entities can move
horizontally, vertically or diagonally, and they go directly to the destination.
There are neither obstacles, nor preferred paths.  

The original idea for this component was to implement an actual pathfinding
algorithm, but I found it unfeasible to develop in reasonable time. I wrote an
unfinished implementation using the A* algorithm (see [#5]), but it was too
computationally intensive for even a small number of entities. This goal can
certainly be achieved with another method, but I had to discard it for now.

### Config
Class that holds simulation-related parameters and handles RNG. Every `World`
object needs a reference to a `Config` object.

As of right now, there is no way to change the configuration values besides
editing the code directly, but this class could be expanded by allowing the
user to edit the configuration values before the simulation starts, or even
while it is already running.

### Seeder
This is an helper program that generates a `entities` file, given a
`background` file. See [Generate a list of people] for usage and
[`background` file] for `background` file specifications.

In order to generate the entities, the program looks for all the available 
housing and working spots on the map. Then, for every house, it selects a 
[random house configuration][A] and generates people accordingly. In the end,
it selects people at random until there are no more people left or until the
target number of people is reached. All of the selected people are written in
the `entities` file.

### Tests
I used two methods of testing for this project:

1. Doctest unit tests
2. `assert` statements

To run unit tests, see [Testing]. `asserts` statements are active only while
running in `Debug` mode.

--------------------------------------------------------------------------------

## Additional notes

### On code correctness
I used several tools to make sure that my code is correct, clean and consistent.
Namely:

1. Clang-Format to check code formatting ([.clang-format][B]).
2. Clang-Tidy to check for common bad-practice warnings
   ([CLion default configuration][3]).
3. cpplint.py to check for some additional details that Clang-Tidy could not
   pick up ([Google styleguide][4]).

I also run the code through _Valgrind Memcheck_ and I made sure that there are
no memory-related errors in my code.

### SIR model fitting
My colleague [Giuseppe][5] developed an interesting project for this same
assignment. His program can, given some data, find out which [SIR Model][6]
parameters best fit said data. We thought it would be interesting to see how the
data generated by my program would suit this particular model. This is the
result we got (click to enlarge):

<div align="center">
  <img alt="Program running" src="assets/md/fitting.png" width="600">
</div>

This graph contains the number of susceptible, infected and removed
(immune or dead) people for each day of the simulation. The dots represent the
data obtained from my program, and the lines are the best possible fit of my
data using the SIR model.

Given that the SIR model has already been proven to work for real-world pandemic
data, I can confidently state that my simulation can mimic well a real-world
pandemic (given the right circumstances).

See Giuseppe's work [here][7].

--------------------------------------------------------------------------------

[0]: https://github.com/P2-718na/pandemic-simulation
[1]: https://baltig.infn.it/giaco/pf2020/-/blob/master/progetto/progetto.md
[2]: https://github.com/P2-718na
[3]: https://confluence.jetbrains.com/display/CLION/Clang-Tidy+in+CLion%3A+default+configuration?_ga=2.184137826.59717557.1623227743-1021145942.1623227743
[4]: https://google.github.io/styleguide/cppguide.html
[5]: https://github.com/KaldarrostaJazz
[6]: https://github.com/P2-718na/SIR-model-simulation
[7]: https://github.com/KaldarrostaJazz/pandemic_models/blob/main/logistic_model_forecasts/sati.md

[A]: seeder/Seeder/configurations/houses.hpp
[B]: .clang-format

[#5]: https://github.com/P2-718na/pandemic-simulation/pull/5

[Building]: #building
[Running]: #running
[Generate a list of people]: #generate-a-list-of-people
[Testing]: #testing
[tl;dr]: #tldr
[`background` file]: assets/README.md#background-file
[`entities` file]: assets/README.md#entities-file

[purple]: assets/md/entities/purple.png
[red]: assets/md/entities/red.png
[cyan]: assets/md/entities/cyan.png
[black]: assets/md/entities/black.png
