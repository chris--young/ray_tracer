#include <iostream>
#include <random>

#include "./Vec3.h"
#include "./Ray.h"

float dot(Vec3 &v1, Vec3 &v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float hit(Vec3 pos, float r, Ray &ray) {
  Vec3 o = ray.A.minus(pos); 

  float a = dot(ray.B, ray.B);
  float b = 2.0 * dot(o, ray.B);
  float c = dot(o, o) - r * r;
  float d = b * b - 4.0 * a * c;

  if (d < 0.0)
    return -1.0;
  else
    return -b - sqrt(d) / 2.0 * a;
}

Vec3 trace(Ray &r) {
  float t = hit(Vec3(0.0, 0.0, -1.0), 0.5, r);

  if (t > 0.0) {
    Vec3 v = Vec3(0.0, 0.0, -1.0);
    Vec3 n = r.point_at(t).minus(v).unit();
    Vec3 v2 = Vec3(n[0] + 1.0, n[1] + 1.0, n[2] + 1.0);
    return v2.multiply(0.5);
  }

  Vec3 u = r.B.unit();

  t = 0.5 * (u[1] + 1.0);

  Vec3 s = Vec3(1.0, 1.0, 1.0);
  Vec3 e = Vec3(0.5, 0.7, 1.0);

  Vec3 p1 = s.multiply(1.0 - t);
  Vec3 p2 = e.multiply(t);

  return p1.add(p2);

  // return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

int main() {
  int w = 400;
  int h = 200;
  int aas = 100;

  Vec3 bottom_left(-2.0, -1.0, -1.0);
  Vec3 horizontal(4.0, 0.0, 0.0);
  Vec3 vertical(0.0, 2.0, 0.0);
  Vec3 origin(0.0, 0.0, 0.0);

  std::cout << "P3\n" << w << ' ' << h << "\n255\n";

  for (int y = h - 1; y >= 0; --y) {
    for (int x = 0; x < w; ++x) {
      Vec3 c;

      for (int s = 0; s < aas; ++s) {
        float u = ((float)x + drand48()) / (float)w;
        float v = ((float)y + drand48()) / (float)h;

        // Ray ray(origin, bottom_left + u * horizontal + v * vertical);
        Vec3 p1 = horizontal.multiply(u);
        Vec3 p2 = vertical.multiply(v);
        Vec3 p3 = p1.add(p2);
        Vec3 p4 = p3.add(bottom_left);

        Ray ray(origin, p4);

        Vec3 sc = trace(ray);
        c = c.add(sc);
      }

      c = c.divide(aas);

      int r = int(255.99 * c[0]);
      int g = int(255.99 * c[1]);
      int b = int(255.99 * c[2]);

      std::cout << r << ' ' << g << ' ' << b << std::endl;
    }
  }

  return 0;
}
