#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#include "sweater/Objects/Object.hpp"
#include "sweater/Objects/ObjectGroup.hpp"
#include "sweater/Lights/Light.hpp"
#include "sweater/RayTracing/Camera.hpp"
#include "sweater/RayTracing/Ray.hpp"
#include "sweater/RayTracing/Hit.hpp"
#include "sweater/Video/Video.hpp"

class Renderer {
  public:
    Renderer(
        std::string filename,
        int width,
        int height,
        int fps,
        std::vector<ObjectGroup *> _objectGroups,
        std::vector<Light *> & _lights,
        Camera * _camera
        ) :
      video(filename, width, height, fps),
      normalVideo(filename + "-normal", width, height, fps),
      objectGroups(_objectGroups),
      lights(_lights),
      camera(_camera)
    {}

    void renderFrame();
    //void updateFrame();

  private:
    Color traceRay(
        const Ray & ray,
        float tmin,
        int bounces,
        Hit & hit
        );

    bool intersect(
        const Ray & ray,
        float tmin,
        Hit & hit
        ) {
      bool doesIntersect = false;
      for (ObjectGroup * objects : objectGroups) {
        doesIntersect |= 
          objects -> intersect(ray, tmin, hit);
      }
      return doesIntersect;
    }

    static const int MAX_DEPTH = 0;
    static const int ANTI_ALIASING = 4;
    static const float EPSILON;

    Video video;
    Video normalVideo;
    std::vector<ObjectGroup *> objectGroups;
    std::vector<Light *> lights;
    Camera * camera;
};

#endif
