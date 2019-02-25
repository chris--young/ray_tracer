#pragma once

#include "./Collidable.h"
#include "./Vec3.h"

class Material {
  protected:
    Vec3 randomPointOnSphere() {
      Vec3 point;
      Vec3 offset(1, 1, 1);

      do {
        point = 2 * Vec3(drand48(), drand48(), drand48()) - offset;
      } while (point.lengthSquared() >= 1);

      return point;
    }

  public:
    virtual bool scatter(Ray& ray, Collision& collision, Vec3& attentuation, Ray& scattered) = 0;
};

