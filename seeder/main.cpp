#include <fstream>
#include <lyra/lyra.hpp>
#include <string>

#include "seeder.hpp"

void parseArgs(
  int argc, char* argv[], int& target, int& infectedCount,
  std::string& backgroundPath, std::string& outputPath) {
  bool showHelp{ false };

  /* clang-format off */
  auto cli = lyra::help(showHelp)
             | lyra::opt( target, "target entity count" )
             ["--target"]["-t"]
               ("Try to generate the specified number of entities.")
               .required()
             | lyra::opt( infectedCount, "infected entity count" )
             ["--infected-count"]["-i"]
               ("Number of currently infected entities")
             | lyra::opt( backgroundPath, "background_ image" )
             ["--background_"]["-b"]
               ("Background image (needed to find valid entity POIs).")
               .required()
             | lyra::opt( outputPath, "output file" )
             ["--output"]["-o"]
               ("Output file location.")
               .required();
  /* clang-format on */

  auto result = cli.parse({ argc, argv });

  // CLion doesn't recognize that lyra parser can set showHelp to true.
  // Ignore "condition is always false" warning.
  if (showHelp || argc == 1) {
    std::cout << cli;
    exit(EXIT_SUCCESS);
  }

  if (!result) {
    std::cerr << "Error in arguments: " << result.errorMessage() << std::endl;
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char* argv[]) {
  int target{};
  int infectedCount{ 0 };
  std::string backgroundPath{};
  std::string outputPath{};

  parseArgs(argc, argv, target, infectedCount, backgroundPath, outputPath);

  std::cout << "Generating entities from \"" << backgroundPath << "\"..."
            << std::endl;

  seeder::Seeder seeder;
  try {
    seeder = seeder::Seeder{ backgroundPath };

    seeder.generateEntities(target, infectedCount);

    std::ofstream os{ outputPath };

    os << seeder.printableEntities();
  } catch (std::runtime_error& err) {
    std::cerr << "An error occurred:" << std::endl << err.what() << std::endl;
    exit(EXIT_FAILURE);
  } catch (...) {
    std::cerr << "Something went horribly wrong! Terminating..." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Print result
  std::cout << "Done. Wrote ";
  std::cout << seeder.entityCount();
  std::cout << " entities." << std::endl;
}
