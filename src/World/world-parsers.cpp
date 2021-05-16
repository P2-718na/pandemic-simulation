#include <fstream>

#include "world.hpp"

bool World::parseEntitiesFromFile_(
  const std::string &entitiesFile, std::vector<Entity> &entities) {
  if (!entities.empty()) {
    return false;
  }

  int uid, homex, homey, workx, worky;
  float virus_resistance, virus_spread_chance, infection_chance;
  std::string ai;

  std::ifstream ifs(entitiesFile);
  std::string line;

  int count = 0;

  while(std::getline(ifs, line)) {
    if (line == "[count]") {
      if (count != 0) {
        return false;
      }

      ifs >> count;
      entities.reserve(count);
      continue;
    }

    if (line == "[entity]") {
      entities.emplace_back((IWorld*)this, uid, homex, homey, parseEntityAi(ai));

      Entity& entity = entities.back();
      entity.homeLocation = {homex, homey};
      entity.workLocation = {workx, worky};
      entity.symptomsResistance = virus_resistance;
      entity.virusSpreadChance = virus_spread_chance;
      entity.infectionResistance = infection_chance;

      continue;
    }

    if (line == "[infected]") {
      entities.back().infective(true);
      continue;
    }

    auto delim = line.find('=');
    if (delim == std::string::npos) {
      continue;
    }

    std::string key = line.substr(0, delim);
    std::string value = line.substr(delim + 1, std::string::npos);
    Entity &currentEntity = entities[entities.size() - 1];

    if (key == "uid") {
      uid = atoi(value.c_str());
      continue;
    }
    if (key == "homex") {
      homex = atoi(value.c_str());
      continue;
    }
    if (key == "homey") {
      homey = atoi(value.c_str());
      continue;
    }
    if (key == "workx") {
      workx = atoi(value.c_str());
      continue;
    }
    if (key == "worky") {
      worky = atoi(value.c_str());
      continue;
    }
    if (key == "virus_resistance") {
      virus_resistance = atof(value.c_str());
      continue;
    }
    if (key == "virus_spread_chance") {
      virus_spread_chance = atof(value.c_str());
      continue;
    }
    if (key == "infection_chance") {
      infection_chance = atof(value.c_str());
      continue;
    }
    if (key == "ai") {
      ai = value;
      continue;
    }
  }
  return true;
}

entityAi World::parseEntityAi(const std::string &value) {
  if (value == "nullAI") {
    return AI::nullAI;
  }
  if (value == "randomAI") {
    return AI::randomAI;
  }
  if (value == "testAI") {
    return AI::testAI;
  }

  return AI::testAI;
}
