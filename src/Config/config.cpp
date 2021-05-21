#include "config.hpp"

int Config::A_STAR_WEIGHT_BY_COLOR(const sf::Color& color) const noexcept {
  // Return correct weight based on color.
  if (color == PARK_COLOR) {
    return PARK_COLOR_WEIGHT;
  }
  if (color == SHOP_COLOR) {
    return SHOP_COLOR_WEIGHT;
  }
  if (color == PARTY_COLOR) {
    return PARTY_COLOR_WEIGHT;
  }
  if (color == ROAD_COLOR) {
    return ROAD_COLOR_WEIGHT;
  }

  // Todo this will need to be replaced with "return -1", but for now
  //  this will do. Need to add additional logic in A*
  return -1;
}
