#pragma once

#include "./Vec3.h"

class Ray {
  public:
    Vec3 A, B; 

    Ray(Vec3 A, Vec3 B) {
      this->A = A; // origin 
      this->B = B; // direction 
    }

    Vec3 pointAt(float t) {
      return A + t * B;
    }
};

