#ifndef SEEDER_PEOPLE_HPP
#define SEEDER_PEOPLE_HPP
// This file contains all possible entity configurations (aka people).
// It can be expanded with new entities, which can be used in houses.hpp

#include <string>

#include "../seeder.hpp"
#include "types.hpp"

namespace seeder {

/* clang-format off */
inline seederEntity OLD_MAN() {
  int age = Seeder::randInt(70, 95);

  return {
    age,
    Seeder::randFloat(0, 100 - age) / 100.f,
    Seeder::randFloat(.02, .03),
    Seeder::randFloat(.6, .7),
    "oldAI"
  };
}

inline seederEntity OLD_WOMAN() {
  int age = Seeder::randInt(70, 100);

  return {
    age,
    Seeder::randFloat(10, 110 - age) / 100.f,
    Seeder::randFloat(.04, .06),
    Seeder::randFloat(.5, .6),
    "oldAI"
  };
}

inline seederEntity MAN() {
  return {
    Seeder::randInt(35, 70),
    Seeder::randFloat(.6, .8),
    Seeder::randFloat(.02, .03),
    Seeder::randFloat(.6, .7),
    "manAI"
  };
}

inline seederEntity WOMAN() {
  return {
    Seeder::randInt(35, 70),
    Seeder::randFloat(.65, .85),
    Seeder::randFloat(.02, .03),
    Seeder::randFloat(.3, .4),
    "manAI"
  };
}

inline seederEntity GRADUATE() {
  return {
    Seeder::randInt(25, 35),
    Seeder::randFloat(.70, .90),
    Seeder::randFloat(.03, .05),
    Seeder::randFloat(.7, .8),
    "gradAI"
  };
}

inline seederEntity UNI_STUDENT() {
  return {
    Seeder::randInt(18, 25),
    Seeder::randFloat(.80, .95),
    Seeder::randFloat(.025, .035),
    Seeder::randFloat(.5, .6),
    "uniAI"
  };
}

inline seederEntity TEEN() {
  return {
    Seeder::randInt(12, 18),
    Seeder::randFloat(.95, 1.f),
    Seeder::randFloat(.01, .02),
    Seeder::randFloat(.5, .6),
    "teenAI"
  };
}

inline seederEntity NOMASK() {
  return {
    Seeder::randInt(35, 45),
    Seeder::randFloat(.40, .70),
    Seeder::randFloat(.8, 1.f),
    Seeder::randFloat(.7, .9),
    "manAI"
  };
}
/* clang-format on */

}  // namespace seeder

#endif  // SEEDER_PEOPLE_HPP
