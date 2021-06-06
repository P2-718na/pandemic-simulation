#ifndef SEEDER_SEEDER_HPP
#define SEEDER_SEEDER_HPP

#include <random>
#include <string>

#include "types.hpp"

namespace seeder {

class Seeder {
  sf::Image background{};

  std::vector<pandemic::Coords> houseLocations{};
  std::vector<pandemic::Coords> workLocations{};
  std::vector<pandemic::Coords> schoolLocations{};
  std::vector<pandemic::Coords> uniLocations{};

  // Return a random element from array of coordinates
  static const pandemic::Coords& randomLocation_(const std::vector<pandemic::Coords> &list) noexcept;

  // RNG functions
  // (See comments in config.hpp, they are the same
  static std::default_random_engine& generator_() noexcept;
  static float randFloat_(float a, float b) noexcept;
  static float randInt_(int a, int b) noexcept;

 public:
  // Constructors //////////////////////////////////////////////////////////////
  Seeder() noexcept;
  explicit Seeder(const std::string& backgroundPath);
};

}  // namespace seeder

#endif  // define SEEDER_SEEDER_HPP
