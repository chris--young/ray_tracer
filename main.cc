#include <iostream>
#include <random>

#include "./Vec3.h"
#include "./Ray.h"
#include "./Camera.h"

float dot(Vec3 &v1, Vec3 &v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float check_collision(Vec3 sphere_position, float sphere_radius, Ray &ray) {
  Vec3 offset = ray.A - sphere_position;

  float a = dot(ray.B, ray.B);
  float b = 2.0 * dot(offset, ray.B);
  float c = dot(offset, offset) - sphere_radius * sphere_radius;
  float d = b * b - 4.0 * a * c;

  return (d < 0.0) ? -1.0 : -b - sqrt(d) / 2.0 * a;
}

Vec3 trace(Ray &ray) {
  float t = check_collision(Vec3(0.0, 0.0, -1.0), 0.5, ray);

  if (t > 0.0) {
    Vec3 normal = (ray.point_at(t) - Vec3(0.0, 0.0, -1.0)).unit();

    return Vec3(normal[0] + 1.0, normal[1] + 1.0, normal[2] + 1.0) * 0.5;
  }

  Vec3 unit = ray.B.unit();

  t = 0.5 * (unit[1] + 1.0);

  return Vec3(1.0, 1.0, 1.0) * (1.0 - t) + Vec3(0.5, 0.7, 1.0) * t;
}

int main() {
  int image_width = 200;
  int image_height = 100;
  int anti_aliasing_samples = 100;

  Camera camera;

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int y = image_height - 1; y >= 0; --y) {
    for (int x = 0; x < image_width; ++x) {
      Vec3 color;

      for (int sample = 0; sample < anti_aliasing_samples; ++sample) {
        float u = ((float)x + drand48()) / (float)image_width;
        float v = ((float)y + drand48()) / (float)image_height;

        Ray ray = camera.get_ray(u, v);
        Vec3 sample_color = trace(ray);

        color += sample_color;
      }

      color = color / anti_aliasing_samples;

      int red = int(255.99 * color[0]);
      int green = int(255.99 * color[1]);
      int blue = int(255.99 * color[2]);

      std::cout << red << ' ' << green << ' ' << blue << std::endl;
    }
  }

  return 0;
}
