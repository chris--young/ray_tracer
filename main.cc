#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <stdlib.h>
#include <thread>
#include <vector>

#include "./Camera.h"
#include "./Collidable.h"
#include "./Dielectric.h"
#include "./Lambertian.h"
#include "./Metal.h"
#include "./Ray.h"
#include "./Scene.h"
#include "./Sphere.h"
#include "./Vec3.h"

float _random() {
  return (float)rand() / (float)RAND_MAX;
}

unsigned long now() {
  return std::chrono::system_clock::now().time_since_epoch().count();
}

Vec3 trace(Ray& ray, Scene* scene, unsigned int depth) {
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

unsigned int logStatus(unsigned int previousLength, unsigned int processed, unsigned int total) {
  std::string cls;

  for (int x = previousLength; x > 0; --x)
    cls += '\b';

  std::string log = "processed " + std::to_string(processed) + " of " + std::to_string(total) + " samples";

  std::cout << cls << log;

  return log.length();
}

Scene* setupScene() {
  Sphere* sphere1 = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
  Sphere* sphere2 = new Sphere(Vec3(0, 1, 0), 1, new Dielectric(1.5));
  Sphere* sphere3 = new Sphere(Vec3(-4, 1, 0), 1, new Lambertian(Vec3(0.4, 0.2, 0.1)));
  Sphere* sphere4 = new Sphere(Vec3(4, 1, 0), 1, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

  std::vector<Collidable*> spheres = { sphere1, sphere2, sphere3, sphere4 };

  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      Vec3 position(a + 0.9 * _random(), 0.2, b + 0.9 * _random()); 

      if ((position - Vec3(4, 0.2, 0)).length() > 0.9) { 
        float kind = _random();

        if (kind < 0.7)
          spheres.push_back(new Sphere(position, 0.2, new Lambertian(Vec3(_random(), _random(), _random()))));
        else if (kind < 0.9)
          spheres.push_back(new Sphere(position, 0.2, new Metal(Vec3(_random(), _random(), _random()), 0.5 * _random())));
        else
          spheres.push_back(new Sphere(position, 0.2, new Dielectric(1.5)));
      }
    }
  }

  return new Scene(spheres);
}

int main() {
  const unsigned long start = now();

  const unsigned int imageWidth = 2880;
  const unsigned int imageHeight = 1800;
  const unsigned int samplesPerPixel = 100;
  const unsigned int totalSamples = imageWidth * imageHeight * samplesPerPixel;

  Camera camera(imageWidth, imageHeight, 20, Vec3(13, 2, 3), Vec3(0, 0, 0), Vec3(0, 1, 0));
  Scene* scene = setupScene();

  std::vector<std::vector<Ray>> rays;

  for (int y = imageHeight - 1; y >= 0; --y) {
    for (unsigned int x = 0; x < imageWidth; ++x) {
      std::vector<Ray> samples;

      for (unsigned int sample = 0; sample < samplesPerPixel; ++sample) {
        float u = ((float)x + _random()) / (float)imageWidth;
        float v = ((float)y + _random()) / (float)imageHeight;

        samples.push_back(camera.getRay(u, v));
      }

      rays.push_back(samples);
    }
  }

  const unsigned int maxThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  std::vector<std::vector<unsigned int>> colors(imageWidth * imageHeight);

  unsigned int logLineLength = 100;
  unsigned int processedSamples = 0;

  for (unsigned int x = 0; x < maxThreads; ++x) {
    unsigned int start = rays.size() / maxThreads * x;
    unsigned int stop = rays.size() / maxThreads * (x + 1);

    //threads.push_back(std::thread([&]{
      for (unsigned int y = start; y < stop; ++y) {
        Vec3 color;

        logLineLength = logStatus(logLineLength, processedSamples += samplesPerPixel, totalSamples);

        for (unsigned int sample = 0; sample < samplesPerPixel; ++sample)
          color += trace(rays[y][sample], scene, 0);

        color /= samplesPerPixel;
        color = Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));

        colors[y] = std::vector<unsigned int>({
          (unsigned int)(255.99 * color[0]),
          (unsigned int)(255.99 * color[1]),
          (unsigned int)(255.99 * color[2])
        });
      }
    //}));
  }

  // for (unsigned int x = 0; x < threads.size(); ++x)
    // threads[x].join();

  std::fstream imageFile("./image.ppm", std::fstream::out);

  imageFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

  for (unsigned int x = 0; x < colors.size(); ++x)
    imageFile << colors[x][0] << ' ' << colors[x][1] << ' ' << colors[x][2] << std::endl;

  imageFile.close();

  std::cout << "\nrender time: " << (now() - start) / 1000.0 << " ms\n";

  return 0;
}

