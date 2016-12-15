#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <eigen3/Eigen/Dense>

#include "sweater/RayTracing/Color.hpp"

class Image {
  public:
    Image(int width, int height)
      : screen(height, width) {}

    int getWidth() const;
    int getHeight() const;

    void setPixel(int x, int y, const Color & color);
    const Color & getPixel(int x, int y) const;

  private:
    Eigen::Matrix<Color, Eigen::Dynamic, Eigen::Dynamic> screen;
};

#endif
