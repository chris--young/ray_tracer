#pragma once

#include "./Vec3.h"

class Ray {
  public:
    Vec3 A, B; 

    Ray(Vec3 &A, Vec3 &B) {
      this->A = A; // origin 
      this->B = B; // direction 
    }

    Vec3 point_at(float t) {
      // return A + t * B;
      Vec3 p = B.multiply(t);
      return A.add(p);
    }
};

