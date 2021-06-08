<div align="center">
  <img alt="Alma Mater Studiorum Logo" src="assets/md/unibo-logo.png">
</div>

# pandemic-simulation
This is my attempt at making a parametric pandemic simulation. Given a "map"
(which is just a bitmap image with highlighted points of interest) and a list of
"people", this program will attempt to simulate the evolution of a pandemic
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

seed      # Helper program to randomly generate a list of people for the
          # simulation
          
test      # Run tests
```

## Running
TODO

### Generate list of people
TODO

### Launch the simulation
TODO

### Testing
TODO

### TL;DR
TODO

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
