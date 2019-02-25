#pragma once

#include "./Vec3.h"
#include "./Ray.h"

class Camera {
  private:
    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 bottomLeft;

  public:
    Camera() {
      origin = Vec3(0, 0, 0);
      horizontal = Vec3(4, 0, 0);
      vertical = Vec3(0, 2, 0);
      bottomLeft = Vec3(-2, -1, -1);
    }

    Ray getRay(float u, float v) {
      return Ray(origin, horizontal * u + vertical * v + bottomLeft);
    }
};

