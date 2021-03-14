#pragma once
#include <vector>

class Pathfinder {
  int _startX{};
  int _startY{};
  int _endX{};
  int _endY{};
  int _step{-1};
  std::vector<int*> _path;

  void _init(
    const int &startX,
    const int &startY,
    const int &endX,
    const int &endY
  );
  bool _calcPath();

 public:
  Pathfinder();
  Pathfinder(
    const int &startX,
    const int &startY,
    const int &endX,
    const int &endY
  );
  ~Pathfinder();

  void init(
    const int &startX,
    const int &startY,
    const int &endX,
    const int &endY
  );
  std::vector<int*> getPath() const;
  int* step();
  bool isArrived();
};