#include "bitmap.hpp"
#include <fstream>

std::ostream& operator<<(std::ostream& lhs, const Pixel& rhs) {
  lhs << "(" << rhs.r << ", " << rhs.g << ", " << rhs.b << ")";
  return lhs;
}

Bitmap::Bitmap(const std::string &fileName) {
  // Open file for reading
  std::ifstream ifs(fileName);

  if (!ifs) {
    throw std::runtime_error("Can't open bmp file.");
  }
  if (!_readHeaders(ifs)) {
    throw std::runtime_error("Wrong bmp file headers.");
  };
  if (!_readBitfield(ifs)) {
    throw std::runtime_error("Error reading bitfield.");
  };
}

bool Bitmap::_readHeaders(std::ifstream &ifs) {
  // Allocate buffer for BITMAPFILEHEADER. Size is fixed, 14 bytes
  char* buffer = new char[BITMAPFILEHEADER_SIZE_BYTES];
  if (!ifs.read(buffer, BITMAPFILEHEADER_SIZE_BYTES)) {
     return false;
  }

  // *((WORD*)&buffer[0]);
  // Cast &buffer to WORD pointer and dereference as WORD value.

  // File type. Must be BM 0x4D42
  this->_fileHeader.bfType = *((WORD*)&buffer[0]);
  if (this->_fileHeader.bfType != 0x4D42) {
    return false;
  }

  // File size in bytes
  this->_fileHeader.bfSize = *((DWORD*)&buffer[2]);

  // Reserved, must be zero
  this->_fileHeader.bfReserved1 = *((WORD*)&buffer[6]);
  this->_fileHeader.bfReserved2 = *((WORD*)&buffer[8]);
  if (
    this->_fileHeader.bfReserved1 != 0 ||
    this->_fileHeader.bfReserved2 != 0
  ) {
    return false;
  }

  // The offset, in bytes, from the beginning of the BITMAPFILEHEADER structure
  // to the bitmap bits.
  this->_fileHeader.bfOffBits = *((DWORD*)&buffer[10]);

  // Delete old buffer and allocate new one. File is variable. 40 For Windows NT
  delete[] buffer;
  buffer = new char[BITMAPINFOHEADER_SIZE_BYTES] ;
  if (!ifs.read(buffer, BITMAPINFOHEADER_SIZE_BYTES)) {
    return false;
  }

  // The number of bytes in header. Only 40Bit headers are supported.
  this->_infoHeader.biSize = *((DWORD*)&buffer[0]);
  if (this->_infoHeader.biSize != 40) {
    return false;
  }

  // Width and height of file
  // If biHeight is positive, the bitmap is a bottom-up DIB
  // and its origin is the lower-left corner. If biHeight is
  // negative, the bitmap is a top-down DIB and its origin
  // is the upper-left corner.
  this->_infoHeader.biWidth = *((LONG*)&buffer[4]);
  this->_infoHeader.biHeight = *((LONG*)&buffer[8]);

  // The number of planes for the target device. Must be set 1.
  this->_infoHeader.biPlanes = *((WORD*)&buffer[12]);
  if (this->_infoHeader.biPlanes != 1) {
    return false;
  }

  // The number of bits-per-pixel. Only 24 is supported.
  this->_infoHeader.biBitCount = *((WORD*)&buffer[14]);
  if (this->_infoHeader.biBitCount != 24) {
    return false;
  }

  // Type of compression (see enum). Only BI_RGB is supported.
  this->_infoHeader.biCompression = *((DWORD*)&buffer[16]);
  if (this->_infoHeader.biCompression != BI_RGB) {
    return false;
  }

  // The size, in bytes, of the image. May be set to zero for BI_RGB bitmaps.
  this->_infoHeader.biSizeImage = *((DWORD*)&buffer[20]);

  // Horizontal and vertical resolution in pixels per meter.
  this->_infoHeader.biXPelsPerMeter = *((LONG*)&buffer[24]);
  this->_infoHeader.biYPelsPerMeter = *((LONG*)&buffer[28]);

  // Color index stuff. Unused.
  this->_infoHeader.biClrUsed = *((DWORD*)&buffer[32]);
  this->_infoHeader.biClrImportant = *((DWORD*)&buffer[36]);

  // Free memory
  delete[] buffer;

  return true;
}

bool Bitmap::_readBitfield(std::ifstream &ifs) {
  const int width_ = this->width();
  const int height_ = this->height();

  this->_bitField = new BYTE[width_ * height_ * 3];

  //Skip to begin of bitfield
  ifs.seekg(this->_fileHeader.bfOffBits);

  // todo check negative height or remove from support.
  // todo (maybe) change and use pixel struct. Prolly not needed since
  //  pointer arithmetic is fast

  // If byte width is not a multiple of 4, rows get padded with null bytes
  const int paddingBytes = 4 - (this->width() * 3) % 4;

  // Read data and discard padding bytes
  for (int i = 0; i < height_; i++) {
    if (!ifs.read((char*)this->_bitField + (i * width_) * 3, width_ * 3)) {
      return false;
    }

    ifs.ignore(paddingBytes);
  }

  return true;
}

int Bitmap::width() const {
  return (int)this->_infoHeader.biWidth;
}
int Bitmap::height() const {
  const LONG height = this->_infoHeader.biHeight;
  return  (int)(height > 0 ? height : -height);
}

Pixel Bitmap::at(const int &x, const int &y) const {
  if (x >= this->width() || y >= this->height()) {
    throw std::runtime_error("Trying to access invalid pixel in bmp.");
  }

  // Todo check if image is top-down or bottom-up. Assuming bottom-up for now.
  const int y_ = this->height() - 1 - y;

  // Save pixels to array. Note that R and B are switched.
  BYTE* pixel = this->_bitField + (x + this->width() * y_) * 3;
  return {*pixel, *(pixel + 1), *(pixel + 2)};
}

Bitmap::~Bitmap() {
  delete[] this->_bitField;
}