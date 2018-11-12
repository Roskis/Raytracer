#ifndef UTILS
#define UTILS

#include <ctime>
#include <iostream>

#include "ply_reader.h"
#include "tga_saver.h"
#include "Vec3f.h"

#define X_AXIS 1
#define Y_AXIS 2
#define Z_AXIS 3

constexpr float EPSILON = 1e-8;

#ifdef _WIN32
  //#define NOMINMAX // use this when on VS
  #include <algorithm>
  #include <direct.h>
  #include <tuple>
  #include <windows.h>
  #define GetCurrentDir _getcwd
  static const std::string slash="\\";
#else
  #include <limits>
  #include <string.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #define GetCurrentDir getcwd
  static const std::string slash="/";
#endif

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

#ifndef MAXFLOAT
  constexpr float MAXFLOAT = std::numeric_limits<float>::max();
#endif

class Material;

struct RaycastResult {
  float t;
  Vec3f p;
  Vec3f normal;
  Material *mat_ptr;
};

char floatToHex(float f) {
  if (f <= 0) f = 0;
  if (f >= 1) f = 1;
  uint32_t i = int(255.99*f);
  char* pf = reinterpret_cast<char*>(&i);
  return pf[0];
}

char intToHex(uint32_t i) {
  if (i <= 0) i = 0;
  if (i >= 256) i = 256;
  char* pf = reinterpret_cast<char*>(&i);
  return pf[0];
}

inline float rand01() {
  return float(rand()) / RAND_MAX;
}

Vec3f rejectionSample() {
  Vec3f p;
  do {
    p = Vec3f(rand01(), rand01(), rand01());
    p = 2*p - 1.0;
  } while (p.length2() >= 1.0);
  return p;
}

std::tuple<int, int, int> getFace(std::unique_ptr<PLY_Model>& model, int index) {
  float i0 = model->m_face_data[3*index];
  float i1 = model->m_face_data[3*index + 1];
  float i2 = model->m_face_data[3*index + 2];
  return std::make_tuple(i0, i1, i2);
}

Vec3f getVertex(std::unique_ptr<PLY_Model>& model, int index) {
  float x = model->m_vertex_data[3*index];
  float y = model->m_vertex_data[3*index + 1];
  float z = model->m_vertex_data[3*index + 2];
  return Vec3f(x, y, z);
}

void saveTgaImage(char *currentPath, int width, int height, char* image) {
  char imageFilename[FILENAME_MAX];
  strcpy(imageFilename, currentPath);
  strcat(imageFilename, "/images/");

  #if defined(_WIN32)
    if (CreateDirectory(imageFilename, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
  #else
    if (mkdir(imageFilename, 0777)) {
  #endif
      std::stringstream ss;
      ss << time(0);
      strcat(imageFilename, ss.str().c_str());
      strcat(imageFilename, ".tga");
      Write_Tga(imageFilename, width, height, image);
      std::cout << "output image: " << imageFilename << std::endl;
    } else {
  #if defined(_WIN32)
      std::cout << "Failed to create directory: " << imageFilename << std::endl;
      std::cout << "error: " << ERROR_ALREADY_EXISTS << std::endl;
    }
  #else
      std::cout << "Failed to create directory: " << imageFilename << std::endl;
    }
  #endif
}

#endif
