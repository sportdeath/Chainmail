#include <array>
#include <iostream>

#include "sweater/Physics/TimeStepper.hpp"
#include "sweater/Objects/Object.hpp"
#include "sweater/Objects/ObjectGroup.hpp"

const std::array<float, 4> RK4::TIMESTEP_CONSTANT = {1, 2, 2, 1};

/**
 * An implimentation of Runge Kutta
 */
void RK4::takeStep(
    const std::vector<ObjectGroup *> & objectGroups,
    float stepSize
    ) {

  for (int i = 0; i < 4; i++) {
    for (ObjectGroup * objects : objectGroups) {
      for (auto it = objects -> begin(); 
          it < objects -> end(); 
          it++) {
        if (i == 0) { 
          // first one is just state
          (*it) -> trialState = (*it) -> state;
        } else {
          // estimate trial state
          (*it) -> evaluateDerivative(
              (*it) -> state,
              (*it) -> derivatives[i - 1],
              // C[1] = 0.5, C[2] = 0.5, C[3] = 1
              stepSize / TIMESTEP_CONSTANT[i], 
              (*it) -> trialState
              );
        }
      }
    }

    // Calculate forces in current state
    for (ObjectGroup * objects : objectGroups) {
      objects -> update();

      for (auto it = objects -> begin(); 
          it < objects -> end(); 
          it++) {
        (*it) -> clearForceAndTorque();
        (*it) -> computeInternalForcesAndConstraints();
      }

      objects -> computeSelfInteraction();
    }

    // Compute collisions
    for (ObjectGroup * objects1 : objectGroups) {
      for (ObjectGroup * objects2 : objectGroups) {
        if (objects1 != objects2) {
          for (auto it1 = objects1 -> begin(); 
            it1 < objects1 -> end(); 
            it1++) {
            for (auto it2 = objects2 -> begin(); 
                it2 < objects2 -> end(); 
                it2++) {
              (*it1) -> collide(*it2);
            }
          }
        }
      }
    }

    // Update numerical integrator
    for (ObjectGroup * objects : objectGroups) {
      for (auto it = objects -> begin(); 
          it < objects -> end(); 
          it++) {

        // perform differentiation
        (*it) -> differentiateState(
            (*it) -> trialState, // input state
            (*it) -> derivatives[i] // output derivative
            ); 

      }
    }
  }

  for (ObjectGroup * objects : objectGroups) {
    for (auto it = objects -> begin(); 
        it < objects -> end(); 
        it++) {
      // Multiply by time constants
      for (int i = 0; i < 4; i++) {
        Object::multiplyDerivative(
            (*it) -> derivatives[i],
            TIMESTEP_CONSTANT[i]
            );
      }

      // Add!
      for (int i = 3; i > 0; i--) {
        Object::addDerivative(
            (*it) -> derivatives[i - 1],
            (*it) -> derivatives[i]
            );
      }

      // Compute new state
      (*it) -> evaluateDerivative(
          (*it) -> state,
          (*it) -> derivatives[0],
          stepSize/6.,
          (*it) -> state);
    }
  }
}

