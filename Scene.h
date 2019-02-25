#pragma once

#include <vector>

#include "./Sphere.h"

class Scene {
  private:
    std::vector<Sphere> spheres;

  public:
    Scene(std::vector<Sphere> spheres) {
      this->spheres = spheres;
    }

    bool checkCollision(Ray ray, float min, float max, Collision &collision) {
      Collision c;
      bool collided = false;
      float closest = max;

      for (int x = 0; x < spheres.size(); ++x) {
        if (spheres[x].checkCollision(ray, min, closest, c)) {
          collided = true;
          closest = c.t;
          collision = c;
        }
      }

      return collided;
    }
};

