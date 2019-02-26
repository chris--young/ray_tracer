#pragma once

#include <stdlib.h>

#include "./Collidable.h"
#include "./Vec3.h"

class Material {
  private:
	float random() {
		return (float)rand() / (float)RAND_MAX;
	}

  protected:
    Vec3 randomPointOnSphere() {
      Vec3 point;
      Vec3 offset(1, 1, 1);

      do {
        point = 2 * Vec3(random(), random(), random()) - offset;
      } while (point.lengthSquared() >= 1);

      return point;
    }

  public:
    virtual bool scatter(Ray& ray, Collision& collision, Vec3& attentuation, Ray& scattered) = 0;
};

