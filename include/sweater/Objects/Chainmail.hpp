#include <eigen3/Eigen/Dense>

#include "sweater/RangeTree/ObjectRangeTree.hpp"
#include "sweater/Objects/ObjectGroup.hpp"
#include "sweater/Objects/Torus.hpp"
#include "sweater/Objects/Plane.hpp"
#include "sweater/RayTracing/Material.hpp"

class Chainmail : public ObjectGroup {
  public:
    Chainmail(
        Eigen::Vector3f topLeftCorner,
        float majorRadius,
        float minorRadius,
        float mass,
        int rows,
        int columns,
        Material * material
        );

    virtual std::vector<Object*>::iterator begin() override {
      //return rangeTree -> begin();
      return allObjects.begin();
    }
    virtual std::vector<Object*>::iterator end() override {
      //return rangeTree -> end();
      return allObjects.end();
    }

    virtual void computeSelfInteraction() override;

    virtual void update() override {
      //delete rangeTree;

      allObjects = std::vector<Object*>(chainmail.size());

      for (int i = 0; i < chainmail.size(); i++) {
        allObjects[i] = &chainmail[i];
      }

      //rangeTree = new ObjectRangeTreeX(
          //allObjects.begin(),
          //allObjects.end()
          //);
    }

  private:
    int rows;
    int columns;
    std::vector<Torus> chainmail;
    std::vector<Object*> allObjects;
    //ObjectRangeTreeX * rangeTree;
    float searchRadiusFloat;
    Eigen::Vector3f searchRadius;
    Plane upperObject, lowerObject;
};
