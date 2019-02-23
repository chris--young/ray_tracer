#pragma once

#include <math.h>
#include <iostream>

class Vec3 {
  private:
    float e[3];

  public:
    Vec3() {
      e[0] = 0.0;
      e[1] = 0.0;
      e[2] = 0.0;
    }

    Vec3(float e0, float e1, float e2) {
      e[0] = e0;
      e[1] = e1;
      e[2] = e2;
    }

    float& operator[](int i) {
      return e[i];
    }

    Vec3& operator+=(Vec3 v) {
      e[0] += v[0];
      e[1] += v[1];
      e[2] += v[2];

      return *this;
    }

    Vec3 unit() {
      float l = length();

      return Vec3(e[0] / l, e[1] / l, e[2] / l);
    }

    float length() {
      return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }
};

Vec3 operator*(Vec3 v, float s) {
  return Vec3(v[0] * s, v[1] * s, v[2] * s);
}

Vec3 operator*(float s, Vec3 v) {
  return Vec3(v[0] * s, v[1] * s, v[2] * s);
}

Vec3 operator/(Vec3 v, float s) {
  return Vec3(v[0] / s, v[1] / s, v[2] / s);
}

Vec3 operator/(float s, Vec3 v) {
  return Vec3(v[0] / s, v[1] / s, v[2] / s);
}

Vec3 operator+(Vec3 v1, Vec3 v2) {
  return Vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vec3 operator-(Vec3 v1, Vec3 v2) {
  return Vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

std::ostream& operator<<(std::ostream &os, Vec3 v) {
  os << '[' << v[0] << ',' << v[1] << ',' << v[2] << ']';

  return os;
}

