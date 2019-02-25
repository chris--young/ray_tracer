#pragma once

#include "./Material.h"
#include "./Ray.h"
#include "./Vec3.h"

class Metal : public Material {
  private:
    Vec3 albedo;
    float blur;

    Vec3 reflect(Vec3 v, Vec3 n) {
      return v - 2 * dot(v, n) * n;
    }

  public:
    Metal(Vec3 albedo, float blur) {
      this->albedo = albedo;
      this->blur = (blur < 1) ? blur : 1;
    }

    bool scatter(Ray& ray, Collision& collision, Vec3& attentuation, Ray& scattered) override {
      Vec3 reflected = reflect(ray.B.unit(), collision.normal);

      scattered = Ray(collision.position, reflected + blur * randomPointOnSphere());
      attentuation = albedo;

      return (dot(scattered.B, collision.normal) > 0);
    };
};

