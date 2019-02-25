#include <iostream>
#include <fstream>
#include <random>
#include <vector>

#include "./Camera.h"
#include "./Ray.h"
#include "./Scene.h"
#include "./Sphere.h"
#include "./Vec3.h"

Vec3 randomPointOnSphere() {
  Vec3 point;
  Vec3 offset(1.0, 1.0, 1.0);

  do {
    point = 2.0 * Vec3(drand48(), drand48(), drand48()) - offset;
  } while (point.lengthSquared() >= 1.0);

  return point;
}

Vec3 trace(Ray &ray, Scene scene) {
  Collision collision;

  if (scene.checkCollision(ray, 0.001, MAXFLOAT, collision)) {
    Vec3 target = collision.position + collision.normal + randomPointOnSphere();
    Ray scatter(collision.position, target - collision.position);

    return 0.5 * trace(scatter, scene);
  }

  Vec3 unit = ray.B.unit();
  float t = 0.5 * (unit[1] + 1.0);

  return Vec3(1.0, 1.0, 1.0) * (1.0 - t) + Vec3(0.5, 0.7, 1.0) * t;
}

int main() {
  const int imageWidth = 800;
  const int imageHeight = 400;
  const int samplesPerPixel = 100;

  Camera camera;
  Sphere sphere1(Vec3(0.0, 0.0, -1.0), 0.5);
  Sphere sphere2(Vec3(0.0, -100.5, -1.0), 100.0);
  std::vector<Sphere> spheres = { sphere1, sphere2 };
  Scene scene(spheres);
  std::fstream imageFile("./image.ppm", std::fstream::out);

  imageFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (int y = imageHeight - 1; y >= 0; --y) {
    for (int x = 0; x < imageWidth; ++x) {
      Vec3 color;

      for (int sample = 0; sample < samplesPerPixel; ++sample) {
        float u = ((float)x + drand48()) / (float)imageWidth;
        float v = ((float)y + drand48()) / (float)imageHeight;

        Ray ray = camera.getRay(u, v);
        Vec3 sampleColor = trace(ray, scene);

        color += sampleColor;
      }

      color /= samplesPerPixel;
      color = Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

      int red = int(255.99 * color[0]);
      int green = int(255.99 * color[1]);
      int blue = int(255.99 * color[2]);

      imageFile << red << ' ' << green << ' ' << blue << std::endl;
    }
  }

  imageFile.close();

  return 0;
}
