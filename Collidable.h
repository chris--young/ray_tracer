#pragma once

class Material;

struct Collision {
  float t;
  Vec3 position;
  Vec3 normal;
  Material *material;
};

class Collidable {
  public:
    virtual bool checkCollision(Ray ray, float min, float max, Collision &collision) = 0;
};

