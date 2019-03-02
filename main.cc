#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <limits>
#include <stdlib.h>

#include "./Camera.h"
#include "./Collidable.h"
#include "./Dielectric.h"
#include "./Lambertian.h"
#include "./Metal.h"
#include "./Ray.h"
#include "./Scene.h"
#include "./Sphere.h"
#include "./Vec3.h"

long random() {
  return (float)rand() / (float)RAND_MAX;
}

unsigned long now() {
  return std::chrono::system_clock::now().time_since_epoch().count();
}

Vec3 trace(Ray& ray, Scene* scene, int depth) {
  Collision collision;

  if (scene->checkCollision(ray, 0.001, std::numeric_limits<float>::max(), collision)) {
    Ray scattered;
    Vec3 attentuation;

    if (depth < 50 && collision.material->scatter(ray, collision, attentuation, scattered))
      return attentuation * trace(scattered, scene, depth + 1); 

    return Vec3(0, 0, 0);
  }

  Vec3 unit = ray.B.unit();
  float t = 0.5 * (unit[1] + 1);

  return Vec3(1, 1, 1) * (1 - t) + Vec3(0.5, 0.7, 1.0) * t;
}

int logStatus(int previousLength, int processed, int total) {
  std::string cls;

  for (int x = previousLength; x > 0; --x)
    cls += '\b';

  std::string log = "processed " + std::to_string(processed) + " of " + std::to_string(total) + " samples";

  std::cout << cls << log;

  return log.length();
}

int main() {
  unsigned long start = now();

  const int imageWidth = 400;
  const int imageHeight = 200;
  const int samplesPerPixel = 100;
  const int totalSamples = imageWidth * imageHeight * samplesPerPixel;
  int processedSamples = 0;
  int logLineLength = 100;

  Camera camera(imageWidth, imageHeight, 45, Vec3(-2, 2, 2), Vec3(0, 0, -1), Vec3(0, 1, 0));
  Sphere* sphere1 = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));
  Sphere* sphere2 = new Sphere(Vec3(-1, -100.5, -1.0), 100.0, new Lambertian(Vec3(0.8, 0.8, 0.0)));
  Sphere* sphere3 = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.1));
  Sphere* sphere4 = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
  std::vector<Collidable*> spheres = { sphere1, sphere2, sphere3, sphere4 };
  Scene* scene = new Scene(spheres);
  std::fstream imageFile("./image.ppm", std::fstream::out);

  imageFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  std::cout << "processed " << processedSamples << " of " << totalSamples << " samples";

  for (int y = imageHeight - 1; y >= 0; --y) {
    for (int x = 0; x < imageWidth; ++x) {
      Vec3 color;

      for (int sample = 0; sample < samplesPerPixel; ++sample) {
        float u = ((float)x + random()) / (float)imageWidth;
        float v = ((float)y + random()) / (float)imageHeight;

        Ray ray = camera.getRay(u, v);
        Vec3 sampleColor = trace(ray, scene, 0);

        color += sampleColor;
      }

      logLineLength = logStatus(logLineLength, processedSamples += samplesPerPixel, totalSamples);

      color /= samplesPerPixel;
      color = Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

      int red = int(255.99 * color[0]);
      int green = int(255.99 * color[1]);
      int blue = int(255.99 * color[2]);

      imageFile << red << ' ' << green << ' ' << blue << std::endl;
    }
  }

  imageFile.close();

  std::cout << "\nrender time: " << (now() - start) / 1000.0 << " ms\n";

  return 0;
}

