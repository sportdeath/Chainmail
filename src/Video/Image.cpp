#include <eigen3/Eigen/Dense>

#include "sweater/Video/Image.hpp"

int Image::getWidth() const {
  return screen.cols();
}

int Image::getHeight() const {
  return screen.rows();
}

// (0, 0) is bottom right corner
void Image::setPixel(int x, int y, const Color & color) {
  screen(y, x) = color;
}

const Color & Image::getPixel(int x, int y) const {
  return screen(y, x);
}
