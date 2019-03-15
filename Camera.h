#pragma once

#include <math.h>
#include <random>

#include "./Vec3.h"
#include "./Ray.h"

#define M_PI 3.14159265358979323846

class Camera {
  private:
    float lenseRadius;

    Vec3 origin;
    Vec3 lookAt;
    Vec3 up;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 bottomLeft;
    Vec3 u, v, w;

    Vec3 randomInUnitDisk() {
      Vec3 point;

      do {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;

        point = 2.0 * Vec3(x, y, 0) - Vec3(1, 1, 0);
      } while (dot(point, point) >= 1);

      return point;
    }

  public:
    Camera(int width, int height, float fov, float aperture, float focusDistance, Vec3 origin, Vec3 lookAt, Vec3 up) {
      float aspect = (float)width / (float)height;
      float theta = fov * M_PI / 180;
      float halfHeight = tan(theta / 2);
      float halfWidth = aspect * halfHeight;

      this->origin = origin;
      this->lookAt = lookAt;
      this->up = up;

      w = (origin - lookAt).unit();
      u = cross(up, w).unit();
      v = cross(w, u);

      lenseRadius = aperture / 2;
      bottomLeft = origin - halfWidth * focusDistance * u - halfHeight * focusDistance * v - w * focusDistance;
      horizontal = 2 * halfWidth * focusDistance * u;
      vertical = 2 * halfHeight * focusDistance * v;
    }

    Ray getRay(float u, float v) {
      Vec3 r = lenseRadius * randomInUnitDisk();
      Vec3 offset = this->u * r[0] + this->v * r[1];

      return Ray(origin + offset, bottomLeft + horizontal * u + vertical * v - origin - offset);
    }
};
