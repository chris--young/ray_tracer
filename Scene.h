#pragma once

#include <vector>

#include "./Collidable.h"
#include "./Sphere.h"

class Scene : Collidable {
  private:
    std::vector<Collidable*> bodies;

  public:
    Scene(std::vector<Collidable*> bodies) {
      this->bodies = bodies;
    }

    bool checkCollision(Ray ray, float min, float max, Collision& collision) override {
      Collision c;
      bool collided = false;
      float closest = max;

      for (int x = 0; x < bodies.size(); ++x) {
        if (bodies[x]->checkCollision(ray, min, closest, c)) {
          collided = true;
          closest = c.t;
          collision = c;
          c.material = bodies[x]->material;
        }
      }

      return collided;
    }
};

