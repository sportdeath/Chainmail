#include <iostream>

#include "sweater/video.hpp"

int main() {

  std::string file = "./video_test.avi";
  int width = 640;
  int height = 360;
  double fps = 24;

  Video video(file, width, height, fps);

  Screen picture(height, width);

  int numFrames = 100;

  for (int i = 0; i < numFrames; i++) {

    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        
        if (col/double(width) < i/double(numFrames)) {
          // red increases as i increases
          picture(row, col) = Color(0.7,0,0);
        } else {
          // green
          picture(row, col) = Color(0.4,0.6,0);
        }
      }

    }

    video.addFrame(picture);
  }
}
