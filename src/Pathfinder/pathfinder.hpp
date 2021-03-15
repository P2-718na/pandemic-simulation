#pragma once
#include <vector>

class Pathfinder {
  int _startX{};
  int _startY{};
  int _endX{};
  int _endY{};
  int _step{-1};
  std::vector<std::vector<int>> _path;

  void _init(
    const int &startX,
    const int &startY,
    const int &endX,
    const int &endY
  );
  bool _calcPath();

 public:
  // todo fratm riscrivi sta roba 🤡
  Pathfinder();
  Pathfinder(
    const int &startX,
    const int &startY,
    const int &endX,
    const int &endY
  );

  void init(
    const int &startX,
    const int &startY,
    const int &endX,
    const int &endY
  );

  std::vector<std::vector<int>> getPath() const;
  std::vector<int> step();
  bool isArrived();
};