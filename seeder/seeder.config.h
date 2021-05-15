#pragma once
#include <cstdlib>
#include <string>

using namespace std;

float randFloat(float a, float b) {
  if (b > a) {
    return randFloat(b, a);
  }

  float random = ((float) rand()) / (float) RAND_MAX;
  float diff = b - a;
  float r = random * diff;
  return a + r;
}
enum Sex {
  male,
  female
};

struct Entity {
  int age;
  float symptomsResistance;
  float virusSpreadChance;
  float infectionResistance;
  string ai;
  pair<int, int> homeLocation{};
  pair<int, int> workLocation{};
};

Entity OLD_MAN() {
  int age = (int)randFloat(70, 95);

  return {
    age,
    randFloat(0, 100 - age) / 100.f,
    randFloat(.04, .06),
    randFloat(.5, .7),
    "oldAi"
  };
};

Entity OLD_WOMAN() {
  int age = (int)randFloat(70, 100);

  return {
    (int)randFloat(70, 100),
    randFloat(10, 110 - age) / 100.f,
    randFloat(.04, .06),
    randFloat(.4, .6),
    "oldAi"
  };
};

Entity MAN() {
  return {
    (int)randFloat(35, 70),
    randFloat(.4, .7),
    randFloat(.04, .06),
    randFloat(.3, .5),
    "manAi"
  };
};

Entity WOMAN() {
  return {
    (int)randFloat(35, 70),
    randFloat(.45, .75),
    randFloat(.04, .06),
    randFloat(.1, .2),
    // We did it boys, we stopped gender inequality
    "manAi"
  };
};

Entity GRADUATE() {
  return {
    (int)randFloat(25, 35),
    randFloat(.70, .80),
    randFloat(.05, .06),
    randFloat(.4, .5),
    "gradAi"
  };
};

Entity UNI_STUDENT() {
  return {
    (int)randFloat(18, 25),
    randFloat(.80, .90),
    randFloat(.055, .065),
    randFloat(.5, .6),
    "uniAi"
  };
};

Entity TEEN() {
  return {
    (int)randFloat(12, 18),
    randFloat(.8, .95),
    randFloat(.06, .07),
    randFloat(.5, .6),
    "teenAi"
  };
};

Entity NOMASK() {
  return {
    (int)randFloat(35, 45),
    randFloat(.40, .70),
    randFloat(.1, .2),
    randFloat(.7, .9),
    "manAi"
  };
};

// Houses //////////////////////////////////////////////////////////////////////
struct House {
  vector<Entity> inhabs{};
  int posx{};
  int posy{};
};

House FAMILY1() {
  return House{{
    MAN(),
    WOMAN(),
    TEEN(),
    TEEN()
  }};
};

House FAMILY2() {
  return House{{
    MAN(),
    WOMAN(),
    TEEN(),
    TEEN(),
    TEEN()
  }};
};

House FAMILY3() {
  return House{{
    MAN(),
    WOMAN(),
    TEEN()
  }};
};

House COUPLE() {
  return House{{
    MAN(),
    WOMAN()
  }};
};

House OLDIES() {
  return House{{
    OLD_MAN(),
    OLD_MAN()
  }};
};

House SINGLE() {
  return House{{
    GRADUATE()
  }};
};

House CON() {
  return House{{
    NOMASK(),
    OLD_WOMAN()
  }};
};

House UNI1() {
  return House{{
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT()
  }};
};

House UNI2() {
  return House{{
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT()
  }};
};
