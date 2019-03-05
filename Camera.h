#pragma once

#include <math.h>

#include "./Vec3.h"
#include "./Ray.h"

#define M_PI 3.14159265358979323846

class Camera {
  private:
    Vec3 origin;
    Vec3 lookAt;
    Vec3 up;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 bottomLeft;

  public:
    Camera(int width, int height, float fov, Vec3 origin, Vec3 lookAt, Vec3 up) {
      float aspect = (float)width / (float)height;
      float theta = fov * M_PI / 180;
      float halfHeight = tan(theta / 2);
      float halfWidth = aspect * halfHeight;

      this->origin = origin;
      this->lookAt = lookAt;
      this->up = up;

      Vec3 w = (origin - lookAt).unit();
      Vec3 u = cross(up, w).unit();
      Vec3 v = cross(w, u);

      bottomLeft = origin - halfWidth * u - halfHeight * v - w;;
      horizontal = 2 * halfWidth * u; 
      vertical = 2 * halfHeight * v;
    }

    Ray getRay(float u, float v) {
      return Ray(origin, bottomLeft + horizontal * u + vertical * v - origin);
    }
};

