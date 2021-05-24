#ifndef MAP_HPP
#define MAP_HPP
#include <micropather.h>
#include <SFML/Graphics/Image.hpp>

namespace mp = micropather;

typedef void* State;
typedef std::vector<MP_VECTOR<mp::StateCost>> Column;
typedef std::vector<Column> CostGrid;
typedef std::pair<int, int> Coords;

class Config;

class Map : public mp::Graph {
  // Micropather instance.
  mp::MicroPather pather_{this};

  // Width and height of the map.
  static int width_;
  static int height_;

  // Grid containing the adjacent states and the rispective weights for each
  // state.
  CostGrid grid_;

  // MicroPather ///////////////////////////////////////////////////////////////
  // Heuristic. For now, use euclidean distance. Will be changed to diagonal
  // distance.
  float LeastCostEstimate(void* stateStart, void* stateEnd) override;

  // Cost of adjacent states for a given state.
  void AdjacentCost(void* state, MP_VECTOR<mp::StateCost>* adjacent) override;

  // Debug function. Prints state info.
  void PrintStateInfo(void* state) override;

 public:
  // Load State map.
  void init(const Config& config, const sf::Image& map);

  // Convert to and from MicroPather state.
  static State coordsToState(const Coords& coords) noexcept;
  static Coords stateToCoords(State state) noexcept;

  // Accessors /////////////////////////////////////////////////////////////////
  static int width() noexcept;
  static int height() noexcept;
  mp::MicroPather& pather() noexcept;
};

#endif // define MAP_HPP
