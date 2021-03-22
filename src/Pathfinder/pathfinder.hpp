#pragma once
#include <vector>

class Pathfinder {
  int _startX{};
  int _startY{};
  int _endX{};
  int _endY{};
  int _step{-1};
  std::vector<std::pair<int, int>> _path{};

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

  std::vector<std::pair<int, int>> getPath() const;
  std::pair<int, int> step();
  bool isArrived();
};