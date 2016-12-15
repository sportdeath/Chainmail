#include <iostream>
#include <random>

#include "sweater/RayTracing/Renderer.hpp"
#include "sweater/Video/Image.hpp"

const float Renderer::EPSILON = 0.05;

void Renderer::renderFrame() {
  int width = video.getWidth();
  int height = video.getHeight();

  Image image(width, height);
  Image normalImage(width, height);

  // Pixel size
  float xPixelRadius = 1./float(width - 1);
  float yPixelRadius = 1./float(height - 1);

  // random number generators for jitter
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> xJitter(-xPixelRadius, xPixelRadius);
  std::uniform_real_distribution<> yJitter(-yPixelRadius, yPixelRadius);
  
  // loop through all the pixels in the image
  // generate all the samples
  for (int x = 0; x < width; x++) {

    // Scene x in [-1, 1]
    float sceneX = 2 * x * xPixelRadius - 1;

    for (int y = 0; y < height; y++) {

      // Scene y in [-1, 1]
      float sceneY = 2. * y * yPixelRadius - 1;

      Color color(0,0,0);
      Eigen::Vector3f normal(0,0,0);
      for (int i = 0; i < ANTI_ALIASING; i++) {
        // Initialize this hit
        Hit hit;
        // Generate a ray
        // randomly jittered between [-pixelRadius, pixelRadius]
        Ray ray = camera -> 
          generateRay(sceneX + xJitter(gen), sceneY + yJitter(gen));
        // Trace the ray
        color += traceRay(ray, camera -> getTMin(), MAX_DEPTH, hit);
        normal += hit.getNormal();
      }
      color /= float(ANTI_ALIASING);
      normal.normalize();

      // Set the pixel

      image.setPixel(x, y, color);
      normalImage.setPixel(x, y, (normal.array() - 1.)/2.);
    }
  }

  // Record to video
  video.addFrame(image);
  normalVideo.addFrame(normalImage);
}

Color Renderer::traceRay(
    const Ray & ray,
    float tmin,
    int bounces,
    Hit & hit) {

  // Default color is black
  Color illuminance(0, 0, 0);

  // If an intersection occurs
  if (intersect(ray, tmin, hit)) {

    // The point of intersection
    Eigen::Vector3f pointOfIntersection = 
      ray.pointAtParameter(hit.getT());

    // Compute effect of lighting
    for (Light * light : lights) {

      // Compute lighting
      Eigen::Vector3f directionToLight;
      Color lightIntensity;
      float distanceToLight;
      light -> getIllumination(
          pointOfIntersection,
          directionToLight, 
          lightIntensity, 
          distanceToLight
          );

      // Check if the object is in shadow
      Ray shadowRay(
          pointOfIntersection,
          directionToLight);
      Hit shadowHit;
      bool isInShadow = intersect(shadowRay, EPSILON, shadowHit);

      isInShadow = isInShadow and
        (shadowHit.getT() <= distanceToLight/directionToLight.norm());

      if (not isInShadow) {
        illuminance +=
          hit.getMaterial() ->
          shade(ray, hit, directionToLight, lightIntensity);
      }
    }
    // END of lighting


    // Compute global illumination
    if (bounces > 0) {
      // Pick a random direction
      // Weight the direction by the probabity
      float dotProduct;
      Ray randomRay = Ray::randomRay(
          pointOfIntersection, 
          hit.getNormal(),
          dotProduct
          );

      Hit bounceHit;
      Color reflectedIlluminance 
        = traceRay(randomRay, 
            EPSILON, 
            bounces - 1, 
            bounceHit);

      illuminance += 
        dotProduct *
        ((hit.getMaterial() -> getColor())
        .cwiseProduct(reflectedIlluminance));
    }
  } 

  return illuminance;
}

