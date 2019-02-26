#pragma once

#include <math.h>
#include <stdlib.h>

#include "./Collidable.h"
#include "./Material.h"
#include "./Ray.h"
#include "./Vec3.h"

class Dielectric : public Material {
  private:
    float idx;

    bool refract(Vec3& v, Vec3& n, float niOverNt, Vec3& refracted) {
      Vec3 uv = v.unit();
      float dt = dot(uv, n);
      float d = 1 - niOverNt * niOverNt * (1 - dt * dt);

      if (d <= 0) {
        return false;
      } else {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(d);
        return true;
      }
    }

    float schlick(float cosine, float idx) {
      float r = (1 - idx) / (1 + idx);

      r = r * r;

      return r + (1 - r) * pow((1 - cosine), 5);
    }

    float random() {
      return (float)rand() / (float)RAND_MAX;
    }

  public:
    Dielectric(float idx) {
      this->idx = idx;
    }

    bool scatter(Ray& ray, Collision& collision, Vec3 &attentuation, Ray& scattered) override {
      Vec3 normal, refracted;
      Vec3 reflected = reflect(ray.B, collision.normal);
      float niOverNt, probability, cosine;

      attentuation = Vec3(1, 1, 1);

      if (dot(ray.B, collision.normal) > 0) {
        normal = -collision.normal;
        niOverNt = idx;
        cosine = idx * dot(ray.B, collision.normal) / ray.B.length();
      } else {
        normal = collision.normal;
        niOverNt = 1 / idx;
        cosine = -dot(ray.B, collision.normal) / ray.B.length();
      }

      if (refract(ray.B, normal, niOverNt, refracted)) {
        probability = schlick(cosine, idx);
      } else {
        scattered = Ray(collision.position, refracted);
        probability = 1;
      }

      if (random() < probability)
        scattered = Ray(collision.position, reflected);
      else
        scattered = Ray(collision.position, refracted);

      return true;
    }
};
