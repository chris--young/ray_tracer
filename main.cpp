#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
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
  return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
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

Scene* setupScene(int numSpheres) {
  Sphere* sphere1 = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
  Sphere* sphere2 = new Sphere(Vec3(0, 1, 0), 1, new Dielectric(1.5));
  Sphere* sphere3 = new Sphere(Vec3(-4, 1, 0), 1, new Lambertian(Vec3(0.4, 0.2, 0.1)));
  Sphere* sphere4 = new Sphere(Vec3(4, 1, 0), 1, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

  std::vector<Collidable*> spheres = { sphere1, sphere2, sphere3, sphere4 };

  for (int a = -numSpheres; a < numSpheres; ++a) {
    for (int b = -numSpheres; b < numSpheres; ++b) {
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

class Pixel {
  public:
    unsigned int x, y;

    Pixel(unsigned int x, unsigned int y) {
      this->x = x;
      this->y = y;
    }
};

int main() {
  const unsigned long start = now();

  const unsigned int imageWidth = 320; // 2560;
  const unsigned int imageHeight = 240; // 1440;
  const unsigned int samplesPerPixel = 100;
  const unsigned int totalSamples = imageWidth * imageHeight * samplesPerPixel;

  Camera camera(imageWidth, imageHeight, 20, 0.1, 10, Vec3(13, 2, 3), Vec3(0, 0, 0), Vec3(0, 1, 0));
  Scene* scene = setupScene(1);
  std::fstream imageFile("./image.ppm", std::fstream::out);
  unsigned int processedSamples = 0;

  imageFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
  std::cout << imageWidth << 'x' << imageHeight << " (" << samplesPerPixel << " samples per pixel)\n";

  std::vector<Pixel> pixels;

  for (int y = imageHeight - 1; y >= 0; --y) {
    for (unsigned int x = 0; x < imageWidth; ++x) {
      pixels.push_back(Pixel(x, (unsigned int)y));
    }
  }

  unsigned int threadCount = std::thread::hardware_concurrency();
  std::vector<std::vector<Pixel>> pixelChunks;
  unsigned int chunkSize = pixels.size() / threadCount;

  for (size_t x = 0; x < pixels.size(); x += chunkSize) {
    std::vector<Pixel> chunk;

    for (size_t y = 0; y < chunkSize; ++y)
      chunk.push_back(pixels[x + y]);

    pixelChunks.push_back(chunk);
  }

  std::vector<std::vector<Vec3>> pixelColors(pixelChunks.size());
  std::vector<std::thread> threads;
  std::mutex logMutex;

  for (size_t x = 0; x < pixelChunks.size(); ++x) {
    threads.push_back(std::thread([&, x]() {
      pixelColors[x] = std::vector<Vec3>(pixelChunks[x].size());

      for (size_t y = 0; y < pixelChunks[x].size(); ++y) {
        Vec3 color;

        for (unsigned int sample = 0; sample < samplesPerPixel; ++sample) {
          float u = ((float)pixelChunks[x][y].x + _random()) / (float)imageWidth;
          float v = ((float)pixelChunks[x][y].y + _random()) / (float)imageHeight;

          Ray ray = camera.getRay(u, v);
          color += trace(ray, scene, 0);
        }

        color /= samplesPerPixel;
        pixelColors[x][y] = Vec3(255.99 * sqrt(color[0]), 255.99 * sqrt(color[1]), 255.99 * sqrt(color[2]));

        logMutex.lock();
        std::cout << "\rprocessed " + std::to_string(processedSamples += samplesPerPixel) + " of " + std::to_string(totalSamples) + " samples";
        logMutex.unlock();
      }
    }));
  }

  for (size_t x = 0; x < threads.size(); ++x)
    threads[x].join();

  for (size_t x = 0; x < pixelColors.size(); ++x)
    for (size_t y = 0; y < pixelColors[x].size(); ++y)
      imageFile << pixelColors[x][y][0] << ' ' << pixelColors[x][y][1] << ' ' << pixelColors[x][y][2] << std::endl;

  imageFile.close();

  std::cout << "\nrendered in " << (now() - start) / 1000.0 << "s\n";

  return 0;
}
