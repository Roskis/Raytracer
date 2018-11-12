#include <sstream>

#include "BVHNode.h"
#include "Camera.h"
#include "Scenes.h"

Vec3f color(const Ray& r, Hitable* world, int depth) {
  RaycastResult res;
  if (world->hit(r, EPSILON, MAXFLOAT, res)) {
    Ray scattered;
    Vec3f attenuation;
    if (depth < 50 && res.mat_ptr->scatter(r, res, attenuation, scattered)) {
      return attenuation * color(scattered, world, depth + 1);
    } else {
      return Vec3f(0, 0, 0);
    }
  } else {
    Vec3f unitDir = unitVector(r.d());
    float t = 0.5 * (unitDir.y + 1.0);
    return (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
  }
}

int main(int argc, char *argv[]) {
  (void) argc; (void) argv;
  clock_t pre, start, end, post;
  pre = clock();
  char cCurrentPath[FILENAME_MAX], filename[FILENAME_MAX];
  if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {return errno;}
  strcpy(filename, cCurrentPath); strcat(filename, "/");

  unsigned int width = 1920, height = 1080, samples = 50;
  int percent = int(height/100);
  int totalPixels = width * height;
  int finishedPixels = 0;
  char* image = new char[width*height*3];
  bool antiAlias = true;

  Hitable* world;
  Camera mainCamera;
  if (argc > 1) {
    strcat(filename, argv[1]);
    std::tie(world, mainCamera) = basicModel(filename, width, height);
  } else {
    std::tie(world, mainCamera) = randomSpheres(width, height);
  }

  std::cout << "rendering image" << std::endl;
  std::cout << " " << int(100.0 * finishedPixels / totalPixels) << "\t%\r";
  std::cout.flush();

  start = clock();
  #pragma omp parallel for
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      Vec3f c(0.0);
      if (antiAlias) {
        for (unsigned int s = 0; s < samples; s++) {
          float u = float((x + rand01())) / float(width);
          float v = float((y + rand01())) / float(height);
          Ray r = mainCamera.getRay(u, v);
          c += color(r, world, 0);
        }
        c /= float(samples);
      } else {
        float u = (0.5 + x) / float(width);
        float v = (0.5 + y) / float(height);
        Ray r = mainCamera.getRay(u, v);
        c = color(r, world, 0);
      }

      //gamma correction
      c = Vec3f(sqrt(c.x), sqrt(c.y), sqrt(c.z));
      //tga handles colours in order blue, green, red
      image[x*3 + y*width*3] = floatToHex(c.z);
      image[x*3 + y*width*3+1] = floatToHex(c.y);
      image[x*3 + y*width*3+2] = floatToHex(c.x);
      finishedPixels += 1; // Possible datarace (only affects displayed progress)
    }
    if (y%percent == 0) {
      std::cout << " " << int(100.0 * finishedPixels / totalPixels) << "\t%\r";
      std::cout.flush(); // These might print on top of each other, but it doesn't really matter
    }
  }
  end = clock();

  std::cout << "100\t%\r" << std::endl;
  saveTgaImage(cCurrentPath, width, height, image);
  delete [] image;
  delete [] world;
  post = clock();
  std::cout << "pre&post time:\t" << start - pre + post - end << std:: endl;
  std::cout << "loop time:\t" << end - start << std::endl;
  std::cout << "total time:\t" << post - pre << std::endl;
}
