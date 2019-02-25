#pragma once

#include "./Material.h"
#include "./Vec3.h"

class Lambertian : public Material {
  private:
    Vec3 albedo;

    Vec3 randomPointOnSphere() {
      Vec3 point;
      Vec3 offset(1.0, 1.0, 1.0);

      do {
        point = 2.0 * Vec3(drand48(), drand48(), drand48()) - offset;
      } while (point.lengthSquared() >= 1.0);

      return point;
    }

  public:
    Lambertian(Vec3 albedo) {
      this->albedo = albedo;
    }

    bool scatter(Ray &ray, Collision &collision, Vec3 &attentuation, Ray &scattered) override {
      Vec3 target = collision.position + collision.normal + randomPointOnSphere();

      scattered = Ray(collision.position, target - collision.position);
      attentuation = albedo;

      return true;
    }
};

