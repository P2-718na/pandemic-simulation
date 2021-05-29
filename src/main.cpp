#include <iostream>

#include "engine.hpp"

int main() {

  try {
    pandemic::Engine engine("./background.sample.bmp", "entities");

    pandemic::Engine::printMessage("Starting simulation...");
    engine.startSimulation();
  } catch (std::runtime_error& err) {
    std::cerr << "An error occurred while initializing the simulation:"
              << std::endl << err.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Something went horribly wrong! Terminating..." << std::endl;
    return EXIT_FAILURE;
  }
}
