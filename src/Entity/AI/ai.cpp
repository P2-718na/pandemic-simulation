#include "ai.hpp"
#include "config.hpp"
#include "entity.hpp"
#include "world.hpp"

namespace pandemic {

AI::AI(Entity* parent)
  : parent_{parent}, lockdownFreeDay_{
                       Config::randInt(0, parent->config().DAYS_IN_A_WEEK())} {}

AI::~AI() noexcept = default;

bool AI::stayHome_(int day) const noexcept {
  if (parent_->quarantined()) {
    return true;
  }

  if (parent_->world_->lockdown() && day != lockdownFreeDay_) {
    return true;
  }

  return false;
}

} // namespace pandemic
