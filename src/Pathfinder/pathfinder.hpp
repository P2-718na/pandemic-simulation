#ifndef PATHFINDER_HPP
#define  PATHFINDER_HPP
#include <vector>
#include <list>
#include <string>

typedef std::pair<int, int> Coords;

struct aStarNode {
  Coords coords;

  int weight;

  // G is the movement cost to move from starting point to this point.
  int g{};

  // H is the estimated cost to move from this node to destination.
  int h{};

  // F is the sum of g and h.
  // todo maybe this could be a variable?
  int f() const {
    return this->g + this->h;
  }

  aStarNode* parent{};

  // todo make vector
  aStarNode* neigbors[8];

  aStarNode(const Coords& coords, int weight) : coords{coords}, weight{weight} {};
};

typedef std::list<aStarNode> aStarList;

class Pathfinder {
  // Full list of nodes. Must be initialized by World at the beginning
  // of the simulation.
  static aStarList aStarFullList_;

  aStarList aStarOpenList_{aStarFullList_.size()};
  aStarList aStarClosedList_{aStarFullList_.size()};

  Coords startCoords_;
  Coords endCoords_;
  int _step{-1};
  std::vector<Coords> _path{};

  auto aStarFindLowestF_(const aStarList& list);
  auto nodeWithLowerFInList_(const aStarNode& node, const aStarList& list) ;
  int aStarComputeHeuristics_(const Coords& nodeCoords) const;

 public:
  //todo Pathfinder();

  static void loadMap(std::string mapImagePath);

  // Reset open and closed list. (clear open list, copy full list to closed list).
  void reset();

  // Set start and end positions. Calls reset() method.
  void init(const Coords& startCoords, const Coords& endCoords);

  // Create graph for A* and save results to vector.
  aStarNode* generateTree();

  void computeAStar();

  // todo Coords step();

  // todo bool isArrived();
};

#endif // define PATHFINDER_HPP
