#pragma once

#include "./Vec3.h"
#include "./Ray.h"

class Camera {
  private:
    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 bottom_left;

  public:
    Camera() {
      origin = Vec3(0.0, 0.0, 0.0);
      horizontal = Vec3(4.0, 0.0, 0.0);
      vertical = Vec3(0.0, 2.0, 0.0);
      bottom_left = Vec3(-2.0, -1.0, -1.0);
    }

    Ray get_ray(float u, float v) {
      return Ray(origin, horizontal * u + vertical * v + bottom_left);
    }
};

