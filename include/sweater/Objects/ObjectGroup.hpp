#ifndef OBJECT_GROUP_HPP
#define OBJECT_GROUP_HPP

#include <initializer_list>
#include <vector>

#include "sweater/Objects/Object.hpp"

class ObjectGroup {
  public:
    virtual void computeSelfInteraction() = 0;

    virtual std::vector<Object*>::iterator begin() = 0;
    virtual std::vector<Object*>::iterator end() = 0;

    virtual void update() = 0;

    bool intersect(
        const Ray & r,
        float tmin,
        Hit & h);
};

class ObjectVector : public ObjectGroup {
  public:
    ObjectVector(std::initializer_list<Object*> _objects) {
      objects.assign(_objects.begin(), _objects.end());
    }

    virtual std::vector<Object*>::iterator begin() override {
      return objects.begin();
    }
    virtual std::vector<Object*>::iterator end() override {
      return objects.end();
    }

    void addObject(Object * object) {objects.push_back(object);}

    virtual void computeSelfInteraction() override;
    virtual void update() override {};

  private:
    std::vector<Object*> objects;
};

#endif
