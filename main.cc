#include <iostream>

#include "./Vec3.h"
#include "./Ray.h"

Vec3 trace(Ray &r) {
  Vec3 unit = unit_vector(r.B);

  float t = 0.5 * (unit[1] + 1.0);

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

  Vec3 bottom_left(-2.0, -1.0, -1.0);
  Vec3 horizontal(4.0, 0.0, 0.0);
  Vec3 vertical(0.0, 2.0, 0.0);
  Vec3 origin(0.0, 0.0, 0.0);

  std::cout << "P3\n" << w << ' ' << h << "\n255\n";

  for (int y = h - 1; y >= 0; --y) {
    for (int x = 0; x < w; ++x) {
      float u = (float)x / (float)w;
      float v = (float)y / (float)h;

      // Ray ray(origin, bottom_left + u * horizontal + v * vertical);
      Vec3 p1 = horizontal.multiply(u);
      Vec3 p2 = vertical.multiply(v);
      Vec3 p3 = p1.add(p2);
      Vec3 p4 = p3.add(bottom_left);
      Ray ray(origin, p4);

      Vec3 c = trace(ray);

      int r = int(255.99 * c[0]);
      int g = int(255.99 * c[1]);
      int b = int(255.99 * c[2]);

      std::cout << r << ' ' << g << ' ' << b << std::endl;
    }
  }

  return 0;
}
