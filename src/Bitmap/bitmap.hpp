#pragma once
#include "wintypes.hpp"

#include <iostream>
#include <string>

#define BITMAPFILEHEADER_SIZE_BYTES 14
#define BITMAPINFOHEADER_SIZE_BYTES 40

enum {
  BI_RGB = 0,
  BI_RLE8 = 1,
  BI_RLE4 = 2,
  BI_BITFIELDS = 3,
  BI_JPEG = 4,
  BI_PNG = 5
};

typedef struct tagBITMAPFILEHEADER {
  WORD  bfType{};
  DWORD bfSize{};
  WORD  bfReserved1{};
  WORD  bfReserved2{};
  DWORD bfOffBits{};
} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
  DWORD biSize{};
  LONG  biWidth{};
  LONG  biHeight{};
  WORD  biPlanes{};
  WORD  biBitCount{};
  DWORD biCompression{};
  DWORD biSizeImage{};
  LONG  biXPelsPerMeter{};
  LONG  biYPelsPerMeter{};
  DWORD biClrUsed{};
  DWORD biClrImportant{};
} BITMAPINFOHEADER;

struct Pixel {
  int b{};
  int g{};
  int r{};
};

std::ostream& operator<<(std::ostream& lhs, const Pixel& rhs);

class Bitmap {
  BITMAPFILEHEADER _fileHeader;
  BITMAPINFOHEADER _infoHeader;

  // todo maybe rename this to smth like rgbField
  BYTE* _bitField;

  bool _readHeaders(std::ifstream &ifs);
  bool _readBitfield(std::ifstream &ifs);

  public:
  explicit Bitmap(const std::string &fileName);
  ~Bitmap();

  int width() const;
  int height() const;

  Pixel at(const int &x, const int &y) const;
};