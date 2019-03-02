#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "./Vec3.h"
#include "./Ray.h"

class Camera {
  private:
    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 bottomLeft;

  public:
    Camera(int width, int height, float fov) {
      float aspect = (float)width / (float)height;
      float theta = fov * M_PI / 180;
      float halfHeight = tan(theta / 2);
      float halfWidth = aspect * halfHeight;

      origin = Vec3(0, 0, 1);
      horizontal = Vec3(2 * halfWidth, 0, 0);
      vertical = Vec3(0, 2 * halfHeight, 0);
      bottomLeft = Vec3(-halfWidth, -halfHeight, -1);
    }

    Ray getRay(float u, float v) {
      return Ray(origin, horizontal * u + vertical * v + bottomLeft);
    }
};

