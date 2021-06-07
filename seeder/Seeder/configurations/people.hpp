#ifndef SEEDER_PEOPLE_HPP
#define SEEDER_PEOPLE_HPP
// This file contains all possible entity configurations (aka people).
// It can be expanded with new entities, which can be used in houses.hpp

#include <string>

#include "seeder.hpp"
#include "types.hpp"

namespace seeder {

inline seederEntity OLD_MAN() {
  int age = (70, 95);

  return {age, Seeder::randInt(0, 100 - age) / 100.f, randFloat(.02, .03),
    randFloat(.6, .7), "oldAI"};
}

inline seederEntity OLD_WOMAN() {
  int age = (int)randFloat(70, 100);

  return {age, randFloat(10, 110 - age) / 100.f, randFloat(.04, .06),
    randFloat(.5, .6), "oldAI"};
}

inline seederEntity MAN() {
  return {(int)randFloat(35, 70), randFloat(.6, .8), randFloat(.02, .03),
    randFloat(.6, .7), "manAI"};
}

inline seederEntity WOMAN() {
  return {(int)randFloat(35, 70), randFloat(.65, .85), randFloat(.02, .03),
    randFloat(.3, .4),
    // We did it boys, we stopped gender inequality
    "manAI"};
}

inline seederEntity GRADUATE() {
  return {(int)randFloat(25, 35), randFloat(.70, .90), randFloat(.03, .05),
    randFloat(.7, .8), "gradAI"};
}

inline seederEntity UNI_STUDENT() {
  return {(int)randFloat(18, 25), randFloat(.80, .95), randFloat(.025, .035),
    randFloat(.5, .6), "uniAI"};
};

inline seederEntity TEEN() {
  return {(int)randFloat(12, 18), randFloat(.95, 1.f), randFloat(.01, .02),
    randFloat(.5, .6), "teenAI"};
}

inline seederEntity NOMASK() {
  return {(int)randFloat(35, 45), randFloat(.40, .70), randFloat(.8, 1),
    randFloat(.7, .9), "manAI"};
}

}  // namespace seeder

#endif  // SEEDER_PEOPLE_HPP
