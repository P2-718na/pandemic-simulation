#! /bin/bash
# Helper script to setup and run the simulation quickly, using default values.

# Inform the user in case of error.
function onError {
  echo "Oops, something went wrong." >> /dev/stderr;
  echo "Make sure to run this script from the build directory, after running cmake." >> /dev/stderr;

  exit 1;
}

# Seed entities
echo "Generating entities file..."
./seed -b background.sample.bmp -t 1000 -i 5 -o entities.txt || onError

# Start simulation
echo "Running pandemic..."
./pandemic -b background.sample.bmp -e entities.txt || onError
