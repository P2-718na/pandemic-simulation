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

struct seederEntity {
  int age;
  float symptomsResistance;
  float virusSpreadChance;
  float infectionResistance;
  string ai;
  pair<int, int> homeLocation{};
  pair<int, int> workLocation{};
};

seederEntity OLD_MAN() {
  int age = (int)randFloat(70, 95);

  return {
    age,
    randFloat(0, 100 - age) / 100.f,
    randFloat(.02, .03),
    randFloat(.6, .7),
    "oldAI"
  };
}

seederEntity OLD_WOMAN() {
  int age = (int)randFloat(70, 100);

  return {
    (int)randFloat(70, 100),
    randFloat(10, 110 - age) / 100.f,
    randFloat(.04, .06),
    randFloat(.5, .6),
    "oldAI"
  };
}

seederEntity MAN() {
  return {
    (int)randFloat(35, 70),
    randFloat(.6, .8),
    randFloat(.02, .03),
    randFloat(.6, .7),
    "manAI"
  };
}

seederEntity WOMAN() {
  return {
    (int)randFloat(35, 70),
    randFloat(.65, .85),
    randFloat(.02, .03),
    randFloat(.3, .4),
    // We did it boys, we stopped gender inequality
    "manAI"
  };
}

seederEntity GRADUATE() {
  return {
    (int)randFloat(25, 35),
    randFloat(.70, .90),
    randFloat(.03, .05),
    randFloat(.7, .8),
    "gradAI"
  };
}

seederEntity UNI_STUDENT() {
  return {
    (int)randFloat(18, 25),
    randFloat(.80, .95),
    randFloat(.025, .035),
    randFloat(.5, .6),
    "uniAI"
  };
};

seederEntity TEEN() {
  return {
    (int)randFloat(12, 18),
    randFloat(.95, 1.f),
    randFloat(.01, .02),
    randFloat(.5, .6),
    "teenAI"
  };
}

seederEntity NOMASK() {
  return {
    (int)randFloat(35, 45),
    randFloat(.40, .70),
    randFloat(.8, 1),
    randFloat(.7, .9),
    "manAI"
  };
}

// Houses //////////////////////////////////////////////////////////////////////
struct House {
  vector<seederEntity> inhabs{};
};

House FAMILY1() {
  return House{{
    MAN(),
    WOMAN(),
    TEEN(),
    TEEN()
  }};
}

House FAMILY2() {
  return House{{
    MAN(),
    WOMAN(),
    TEEN(),
    TEEN(),
    TEEN()
  }};
}

House FAMILY3() {
  return House{{
    MAN(),
    WOMAN(),
    TEEN()
  }};
}

House COUPLE() {
  return House{{
    MAN(),
    WOMAN()
  }};
}

House OLDIES() {
  return House{{
    OLD_MAN(),
    OLD_MAN()
  }};
}

House SINGLE() {
  return House{{
    GRADUATE()
  }};
}

House CON() {
  return House{{
    NOMASK(),
    OLD_WOMAN()
  }};
}

House UNI1() {
  return House{{
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT()
  }};
}

House UNI2() {
  return House{{
    UNI_STUDENT(),
    UNI_STUDENT(),
    UNI_STUDENT()
  }};
}
