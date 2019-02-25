#pragma once

#include "./Collidable.h"
#include "./Vec3.h"

class Material {
  public:
    virtual bool scatter(Ray &ray, Collision &collision, Vec3 &attentuation, Ray &scattered) = 0;
};

