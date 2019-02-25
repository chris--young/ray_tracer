#pragma once

#include <math.h>

#include "./Collidable.h"
#include "./Material.h"
#include "./Vec3.h"

class Sphere : public Collidable {
  private:
    Vec3 position;
    float radius;

  public:
    Sphere(Vec3 position, float radius, Material *material) {
      this->position = position;
      this->radius = radius;
      this->material = material;
    }

    bool checkCollision(Ray ray, float min, float max, Collision &collision) override {
      Vec3 offset = ray.A - position;

      float a = dot(ray.B, ray.B);
      float b = dot(offset, ray.B);
      float c = dot(offset, offset) - radius * radius;
      float d = b * b - a * c;

      if (d > 0) {
        float t = (-b - sqrt(d)) / a;

        if (t > min && t < max) {
          collision.t = t;
          collision.position = ray.pointAt(collision.t);
          collision.normal = (collision.position - position) / radius;
          collision.material = material;
          return true;
        }

        t = (-b + sqrt(d)) / a;

        if (t > min && t < max) {
          collision.t = t;
          collision.position = ray.pointAt(collision.t);
          collision.normal = (collision.position - position) / radius;
          collision.material = material;
          return true;
        }
      }

      return false;
    }
};

