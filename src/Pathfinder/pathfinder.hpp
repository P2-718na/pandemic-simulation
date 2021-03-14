#pragma once
#include <vector>

class Pathfinder {
  int _startX{};
  int _startY{};
  int _endX{};
  int _endY{};

  int _step{};
  std::vector<int*> _path;

  bool _calcPath();

 public:
  //Pathfinder();
  Pathfinder(int startX, int startY, int endX, int endY);
  ~Pathfinder();

  std::vector<int*> getPath() const;
  int* step();
};