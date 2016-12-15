#include <cassert>
#include <iostream>
#include <algorithm>

#include "sweater/RayTracing/Color.hpp"
#include "sweater/Video/Video.hpp"
#include "sweater/Video/Image.hpp"

int Video::getWidth() const {
  return width;
}

int Video::getHeight() const {
  return height;
}

Video::Video(
    const std::string & filename,
    int _width,
    int _height,
    double fps) :
    width(_width),
    height(_height) {

  int fourcc =
    //cv::VideoWriter::fourcc('F', 'F', 'V', '1');
    //cv::VideoWriter::fourcc('P', 'I', 'M', '1');
    cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    //cv::VideoWriter::fourcc('M', 'J', '2', 'C');
    //cv::VideoWriter::fourcc('X', '2', '6', '4');
    //cv::VideoWriter::fourcc('H', 'F', 'Y', 'U');
    
  cv::Size size(width, height);

  writer.open(
      filename + ".avi", 
      fourcc,
      fps,
      size
      );

  //writer.set(cv::VIDEOWRITER_PROP_QUALITY, 100);

  assert(writer.isOpened());
}

void Video::addFrame(const Image & image) {

  assert(height == image.getHeight());
  assert(width == image.getWidth());

  cv::Mat img(height, width, CV_8UC3);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      Color color = image.getPixel(width - col - 1, height - row - 1);

      img.at<cv::Vec3b>(row, col) = 
        cv::Vec3b(
            255.*std::min(color[2],1.f),
            255.*std::min(color[1],1.f),
            255.*std::min(color[0],1.f));
    }
  }

  writer.write(img);
}

void Video::release() {
  writer.release();
  assert(not writer.isOpened());
}
