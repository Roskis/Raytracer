#ifndef SCENES
#define SCENES

#include "Material.h"
#include "Sphere.h"
#include "Triangle.h"

std::tuple<Hitable*, Camera> basicModel(char* filename, int width, int height) {
  std::cout << "filename: " << filename << std::endl;
  std::cout << "reading model" << std::endl;
  std::unique_ptr<PLY_Model> model = Read_PLY_Model(filename);
  int faces = int(model->m_face_data.size()/3);
  int vertexes = int(model->m_vertex_data.size()/3);
  std::cout << "faces:\t" << faces << std::endl;
  std::cout << "verts:\t" << vertexes << std::endl;
  int numberOfSpheres = 2;
  int numOfObjects = faces + numberOfSpheres;
  Hitable** list = new Hitable*[numOfObjects];
  if (list == NULL) exit(1);
  list[0] = new Sphere(Vec3f(0, -99.95, 0), 100, new Lambertian(Vec3f(0.8, 0.8, 0.0)));
  list[1] = new Sphere(Vec3f(-0.05, 0.15, -0.15), 0.1, new Metal(Vec3f(0.2, 0.6, 0.8), 0.0));

  #pragma omp parallel for
  for (int i = 0; i < faces; i++) {
    std::tuple<int, int, int> indexes = getFace(model, i);
    Vec3f v0 = getVertex(model, std::get<0>(indexes));
    Vec3f v1 = getVertex(model, std::get<1>(indexes));
    Vec3f v2 = getVertex(model, std::get<2>(indexes));
    list[i + numberOfSpheres] = new Triangle(v0, v1, v2,
      new Lambertian(Vec3f(0.8, 0.2, 0.2)));
  }

  std::cout << "creating bounding volume hierarchy" << std::endl;
  Hitable* bvh = new BVHNode(list, numOfObjects);
  std::cout << "BVH done" << std::endl;
  delete[] list;

  Vec3f cameraOrigin(-0.2, 0.2, 0.25);
  Vec3f cameraDirection(0.0, 0.15, 0.0);
  Vec3f cameraUp(0, 1, 0);
  float fov = 60;
  float aspectRatio = float(width)/float(height);
  float distToFocus = 1.0;
  float aperture = 0.0;
  Camera cam(cameraOrigin, cameraDirection, cameraUp, fov, aspectRatio, aperture, distToFocus);
  return std::make_tuple(bvh, cam);
}

std::tuple<Hitable*, Camera> randomSpheres(int width, int height) {
  int n = 1000;
  Hitable** list = new Hitable*[n+1];
  list[0] = new Sphere(Vec3f(0, -1000, 0), 1000, new Lambertian(Vec3f(0.5, 0.5, 0.5)));
  int i = 1;
  for (int a = -15; a < 15; a++) {
    for (int b = -15; b < 15; b++) {
      float mat = rand01();
      Vec3f center(a + 0.9 * rand01(), 0.2, b * 0.9 * rand01());
      bool freeSpace = true;
      for (int h = 0; h < i; h++) {
        if (distance(center, list[h]->middlePoint()) < 0.4) {
          freeSpace = false;
          break;
        }
      }

      if (freeSpace) {
        if ((center - Vec3f(4, 0, 2.0)).length() > 0.9) {
          if (mat < 0.8) {
            list[i++] = new Sphere(center, 0.2,
              new Lambertian(Vec3f(rand01() * rand01(), rand01() * rand01(), rand01() * rand01())));
          } else if (mat < 0.95) {
            list[i++] = new Sphere(center, 0.2,
              new Metal(Vec3f(0.5 * (1 + rand01()), 0.5 * (1 + rand01()), 0.5 * (1 + rand01())), 0.5 * rand01()));
          } else {
            list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
          }
        }
      }
    }
  }
  list[i++] = new Sphere(Vec3f(0, 1, 0), 1.0, new Dielectric(1.5));
  list[i++] = new Sphere(Vec3f(-4, 1, 0), 1.0, new Lambertian(Vec3f(0.4, 0.2, 0.1)));
  list[i++] = new Sphere(Vec3f(4, 1, 0), 1.0, new Metal(Vec3f(0.7, 0.6, 0.5), 0.0));

  Hitable* bvh = new BVHNode(list, i);
  delete[] list;

  Vec3f cameraOrigin(13, 2, 3);
  Vec3f cameraDirection(0.0, 0.0, 0.0);
  Vec3f cameraUp(0, 1, 0);
  float fov = 30;
  float aspectRatio = float(width)/float(height);
  float distToFocus = 10.0;
  float aperture = 0.1;
  Camera cam(cameraOrigin, cameraDirection, cameraUp, fov, aspectRatio, aperture, distToFocus);
  return std::make_tuple(bvh, cam);
}

#endif
