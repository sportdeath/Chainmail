#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <string>

#include <opencv2/videoio.hpp>

#include "sweater/Video/Image.hpp"

class Video {
  private:
    cv::VideoWriter writer;
    int width;
    int height;

  public:
    Video(
        const std::string & filename,
        int _width, 
        int _height, 
        double fps
        );

    int getWidth() const;
    int getHeight() const;

    void addFrame(const Image & image);

    void release();
};

#endif
