#ifndef SEEDER_HOUSES_HPP
#define SEEDER_HOUSES_HPP
// This file contains all possible housing configurations. It can be expanded,
// but be sure to add the new configurations in seeder.cpp accordingly

#include <vector>

#include "seeder.hpp"
#include "people.hpp"

namespace seeder {

/* clang-format off */
inline House FAMILY1() {
  return House{
    MAN(),
    WOMAN(),
    TEEN(),
    TEEN()
  };
}

inline House FAMILY2() {
  return House{
    MAN(),
    WOMAN(),
    TEEN(),
    TEEN(),
    TEEN()
  };
}

inline House FAMILY3() {
  return House{
    MAN(),
    WOMAN(),
    TEEN()
  };
}

inline House COUPLE() {
  return House{
    MAN(),
    WOMAN()
  };
}

inline House OLDIES() {
  return House{
    OLD_MAN(),
    OLD_MAN()
  };
}

inline House SINGLE() {
  return House{
    GRADUATE()
  };
}

inline House CON() {
  return House{
    NOMASK(),
    OLD_WOMAN()
  };
}

inline House UNI1() {
  return House{
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT()
  };
}

inline House UNI2() {
  return House{
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT()
  };
}
/* clang-format on */

}  // namespace seeder

#endif  // define SEEDER_HOUSES_HPP
