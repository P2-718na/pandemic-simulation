#pragma once
#include <cstdlib>
#include <string>

using namespace std;

float randFloat(float min, float max) {
  return min + (float)rand() / (float)RAND_MAX/(max - min);
}

enum Sex {
  male,
  female
};

struct Entity {
  int age;
  Sex sex;
  float virusResistance;
  float virusSpreadChance;
  float infectionChance;
  string ai;
  pair<int, int> homeLocation{};
  pair<int, int> workLocation{};
};

struct OLD_MAN : Entity {
  int age{(int)randFloat(70, 95)};
  Sex sex{male};
  float virusResistance{randFloat(0, 100 - age) / 100.f};
  float virusSpreadChance{randFloat(.4, .6)};
  float infectionChance{randFloat(.5, .7)};
  string ai{"oldAi"};
};

struct OLD_WOMAN : Entity {
  int age{(int)randFloat(70, 100)};
  Sex sex{female};
  float virusResistance{randFloat(10, 110 - age) / 100.f};
  float virusSpreadChance{randFloat(.4, .6)};
  float infectionChance{randFloat(.4, .6)};
  string ai{"oldAi"};
};

struct MAN : Entity{
  int age{(int)randFloat(35, 70)};
  Sex sex{male};
  float virusResistance{randFloat(.4, .7)};
  float virusSpreadChance{randFloat(.4, .6)};
  float infectionChance{randFloat(.3, .5)};
  string ai{"manAi"};
};

struct WOMAN : Entity{
  int age{(int)randFloat(35, 70)};
  Sex sex{female};
  float virusResistance{randFloat(.45, .75)};
  float virusSpreadChance{randFloat(.4, .6)};
  float infectionChance{randFloat(.1, .2)};
  // We did it boys, we stopped gender inequality
  string ai{"manAi"};
};

struct GRADUATE : Entity{
  int age{(int)randFloat(25, 35)};
  float virusResistance{randFloat(.70, .80)};
  float virusSpreadChance{randFloat(.5, .6)};
  float infectionChance{randFloat(.4, .5)};
  string ai{"gradAi"};
};

struct UNI_STUDENT : Entity{
  int age{(int)randFloat(18, 25)};
  float virusResistance{randFloat(.80, .90)};
  float virusSpreadChance{randFloat(.55, .65)};
  float infectionChance{randFloat(.5, .6)};
  string ai{"uniAi"};
};

struct TEEN : Entity{
  int age{(int)randFloat(12, 18)};
  float virusResistance{randFloat(.8, .95)};
  float virusSpreadChance{randFloat(.6, .7)};
  float infectionChance{randFloat(.5, .6)};
  string ai{"teenAi"};
};

struct NOMASK : Entity{
  int age{(int)randFloat(35, 45)};
  float virusResistance{randFloat(.40, .70)};
  float virusSpreadChance{randFloat(.9, 1)};
  float infectionChance{randFloat(.7, .9)};
  string ai{"manAi"};
};