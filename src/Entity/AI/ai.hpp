#ifndef AI_HPP
#define AI_HPP

class Entity;

// fixme ask if this is ok
namespace AI {

// Entity*, points to current entity, int are current minute and current
// day of the week.
typedef void (*entityAI)(Entity*, int, int);

// Default AI //////////////////////////////////////////////////////////////////
void nullAI(Entity* _this, int time = 0, int day = 0);
void randomAI(Entity* entity, int time = 0, int day = 0);

// Variants ////////////////////////////////////////////////////////////////////
void manAI(Entity* entity, int time = 0, int day = 0);
void oldAI(Entity* entity, int time = 0, int day = 0);

};

// Include AI variants
// fixme ask if this is ok
#include "variants/man.hpp"
#include "variants/old.hpp"
#include "variants/grad.hpp"
#include "variants/uni.hpp"
#include "variants/teen.hpp"

#endif // define AI_HPP
