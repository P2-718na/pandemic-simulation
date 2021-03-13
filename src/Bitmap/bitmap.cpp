#include "bitmap.hpp"
#include <fstream>

Bitmap::Bitmap(const std::string &fileName) {
  // Open file for reading
  std::ifstream ifs(fileName);

  if (!_readHeaders(ifs)) {
    throw std::runtime_error("Can't open bitmap file.");
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
  this->_fileHeader.bfOffBits = *((DWORD*)&buffer[6]);

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

}

int Bitmap::width() const {
  return (int)this->_infoHeader.biWidth;
}

int Bitmap::height() const {
  const LONG height = this->_infoHeader.biHeight;
  return  (int)(height > 0 ? height : -height);
}