#pragma once

#include "./Material.h"
#include "./Ray.h"
#include "./Vec3.h"

class Metal : public Material {
  private:
    Vec3 albedo;
    float blur;

    float dot(Vec3 &v1, Vec3 &v2) {
      return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    }

    Vec3 reflect(Vec3 v, Vec3 n) {
      return v - 2.0 * dot(v, n) * n;
    }

    Vec3 randomPointOnSphere() {
      Vec3 point;
      Vec3 offset(1.0, 1.0, 1.0);

      do {
        point = 2.0 * Vec3(drand48(), drand48(), drand48()) - offset;
      } while (point.lengthSquared() >= 1.0);

      return point;
    }

  public:
    Metal(Vec3 albedo, float blur) {
      this->albedo = albedo;
      this->blur = (blur < 1.0) ? blur : 1.0;
    }

    bool scatter(Ray &ray, Collision &collision, Vec3 &attentuation, Ray &scattered) override {
      Vec3 reflected = reflect(ray.B.unit(), collision.normal);

      scattered = Ray(collision.position, reflected + blur * randomPointOnSphere());
      attentuation = albedo;

      return (dot(scattered.B, collision.normal) > 0.0);
    };
};

