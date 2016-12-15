#include <iostream>

#include "sweater/Objects/Chainmail.hpp"

Chainmail::Chainmail(
    Eigen::Vector3f topLeftCorner,
    float majorRadius,
    float minorRadius,
    float mass,
    int _rows,
    int _columns,
    Material * material
    ) :
  upperObject(Eigen::Vector3f::UnitX(), 0, NULL), 
  lowerObject(Eigen::Vector3f::UnitX(), 0, NULL),
  rows(_rows),
  columns(_columns)
{
  Eigen::Vector3f columnStart, rowStart = topLeftCorner;
  Eigen::Matrix3f rotationMatrix = Eigen::Matrix3f::Identity();
  Eigen::Matrix3f rotate90 = 
    Eigen::AngleAxisf(M_PI/2.,Eigen::Vector3f::UnitX()).toRotationMatrix();
  float shift = majorRadius + minorRadius;

  for (int r = 0; r < rows; r++) {
    // if row is
    columnStart = rowStart;
    for (int c = 0; c < columns; c++) {
      // make torus
      Torus torus(
          columnStart,
          rotationMatrix,
          majorRadius,
          minorRadius,
          mass,
          material
          );
      chainmail.push_back(torus);

      columnStart[0] += 2* shift; // in x direction
    }


    // rotate rotation matrix by 90 degrees
    // along x axis
    rotationMatrix *= rotate90;

    // shift row down 
    rowStart[1] -= minorRadius;
    rowStart[2] -= majorRadius - 2 * minorRadius;

    if (r%2 == 0) {
      rowStart[0] += shift; // in x direction
      columns -= 1;
    } else {
      rowStart[0] -= shift;
      columns += 1;
    }
  }
  columns = _columns;


  searchRadiusFloat = 2 * (minorRadius + majorRadius);
  searchRadius = 
    Eigen::Vector3f(searchRadiusFloat, searchRadiusFloat, searchRadiusFloat);

  //rangeTree = nullptr;
  update();
}

void Chainmail::computeSelfInteraction() {
  for (auto it1 = begin(); it1 < end(); it1++) {
    //Eigen::Vector3f position =
      //(*it) -> trialState.position;

    //lowerObject.trialState.position =
      //position - searchRadius;
    //upperObject.trialState.position =
      //position + searchRadius;


    //std::vector<Object *> collisions;
    //rangeTree -> find(
        //&lowerObject,
        //&upperObject,
        //collisions);

    //for (Object * otherObject : collisions) {
    for (auto it2 = begin(); it2 < end(); it2++) {
      if (*it2 != *it1) {
        float distanceToObject = 
          ((*it1) -> trialState.position - 
          (*it2) -> trialState.position).norm();
        if (distanceToObject < searchRadiusFloat) {
          (*it1) -> collide(*it2); 
        }
      }
    }
  }

  // pin top two corners
  chainmail[0].force = Eigen::Vector3f(0,0,0);
  chainmail[0].torque = Eigen::Vector3f(0,0,0);

  chainmail[columns - 1].force = Eigen::Vector3f(0,0,0);
  chainmail[columns - 1].torque = Eigen::Vector3f(0,0,0);

}
