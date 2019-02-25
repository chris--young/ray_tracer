#pragma once

#include <vector>

#include "./Collidable.h"
#include "./Sphere.h"

class Scene : Collidable {
  private:
    std::vector<Sphere> spheres;

  public:
    Scene(std::vector<Sphere> spheres) {
      this->spheres = spheres;
    }

    bool checkCollision(Ray ray, float min, float max, Collision &collision) override {
      Collision c;
      bool collided = false;
      float closest = max;

      for (int x = 0; x < spheres.size(); ++x) {
        if (spheres[x].checkCollision(ray, min, closest, c)) {
          collided = true;
          closest = c.t;
          collision = c;
          c.material = spheres[x].material;
        }
      }

      return collided;
    }
};

