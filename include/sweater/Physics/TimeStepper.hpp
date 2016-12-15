#ifndef TIME_STEPPER_HPP
#define TIME_STEPPER_HPP

#include <array>
#include <vector>

#include "sweater/Objects/ObjectGroup.hpp"

class RK4 {
  public:
    static void takeStep(const std::vector<ObjectGroup *> & objectGroups, float stepSize);
  private:
    static const std::array<float, 4> TIMESTEP_CONSTANT;
};

#endif
