<div align="center">
  <img alt="Alma Mater Studiorum Logo" src="assets/md/unibo-logo.png">
</div>

# pandemic-simulation
This is my attempt at making a parametric pandemic simulation. Given a _map_
(which is just a bitmap image with highlighted points of interest) and a list of
_people_, this program will attempt to simulate the evolution of a pandemic
among them.

The goal I set for this project was to try and simulate a human-like behaviour
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
- [SFML](http://https://www.sfml-dev.org/) (required)
- [CMake](https://cmake.org/) (recommended)

## Building
The preferred way to build this code is by using CMake. Although is recommended
to build the program in `Release` configuration for better performance, only the
`Debug` configuration satisfies all the requirements imposed by the project
assignment (`-g` and `-fsanitize="address"` flags are present only in the
latter).
```shell
# Clone the repo
git clone git@github.com:P2-718na/pandemic-simulation.git

# Create and cd to build directory
take pandemic-simulation/build

# Prepare build files. Use "Debug" instead of "Release" to build in debug mode.
cmake .. -DCMAKE_BUILD_TYPE=Release
```
This will configure all the needed files. Three executables will be generated
(see [Running](#running) for additional information on what they do).
```bash
pandemic  # Pandemic simulation executable

seed      # Tool to randomly generate a list of people for the simulation
          
test      # Run tests
```

## Running
([tl;dr](#tldr) at the end of this section for a quick, automatic running).

For the simulation to run, it requires a `background` and an `entities` file.
`background` is an image which contains "points of interest" (often referred as
_POIs_ in code and comments) such as houses, parks, shops and so on (see TODO, generate background).
`entities` is a text file, which contains specific information for every person
(or _entity_) in the simulation (see TODO). To keep things simpler, I already
included an example background file, called `background.sample.bmp`, but you can
go ahead and create your own if you prefer.

The following sections will teach you how to launch the simulation correctly.
Every command assumes that you have successfully build the program and are in
the build directory (see [Building](#building)).

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
The background file that the seeder needs to generate a list of entities is the
same that will be used later to launch the simulation. This is because
that the seeder needs to have a way to know where POIs are located, and the
background image contains precisely this information.

Even though this might seem redundant, I purposely chose this approach because
of many reasons. Most notably, I wanted to keep the _simulation engine_ and
_simulation data_ parts of my project separated. This way, one can have
different ways to generate the entities list for the same map.

### Launch the simulation
Once you have your `entities` and `map` files ready, you can run the simulation.
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
TODO

### tl;dr
Since there are many steps to running this program, I added an helper script to
quickly setup everything. After building, run  
```bash 
./quick-run.sh
```
from the CMake build directory. (Make sure the file is executable, if needed use
`chmod +x quick-run.sh`).

--------------------------------------------------------------------------------

## Directory structure and components
TODO

### Engine class
TODO

### World class
TODO

### Entity class
TODO

### Pathfinder class
TODO

### Config class
TODO

### Seeder
TODO (See readme.md in seeder)

### Tests
TODO (see readme.md in tests)

--------------------------------------------------------------------------------

## Additional notes
TODO (valgrind, clang tidy)
TODO giuseppe fitting

[1]: https://baltig.infn.it/giaco/pf2020/-/blob/master/progetto/progetto.md
[2]: https://github.com/P2-718na
