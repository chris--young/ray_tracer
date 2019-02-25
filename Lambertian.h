#pragma once

#include "./Material.h"
#include "./Vec3.h"

class Lambertian : public Material {
  private:
    Vec3 albedo;

  public:
    Lambertian(Vec3 albedo) {
      this->albedo = albedo;
    }

    bool scatter(Ray& ray, Collision& collision, Vec3& attentuation, Ray& scattered) override {
      Vec3 target = collision.position + collision.normal + randomPointOnSphere();

      scattered = Ray(collision.position, target - collision.position);
      attentuation = albedo;

      return true;
    }
};

