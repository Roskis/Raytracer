#ifndef AABB_
#define AABB_

#include "Ray.h"
#include "Utils.h"

class AABB {
  public:
    Vec3f min, max;
    AABB() {};
    AABB(const Vec3f& a) {min = a; max = a;};
    AABB(const Vec3f& a, const Vec3f& b) {min = a; max = b;};
    bool hit(const Ray& r, float tmin, float tmax) const {
      for (int a = 0; a < 3; a++) {
        float invD = 1.0f / r.d()[a];
        float t0 = (min[a] - r.o()[a]) * invD;
        float t1 = (max[a] - r.o()[a]) * invD;
        if (invD < 0.0f) {
          std::swap(t0, t1);
        }
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin) {
          return false;
        }
      }
      return true;
    }

    inline float area() const {
      Vec3f d(max - min);
      return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
    }

    inline void expand(Vec3f p) {
      if (p.x < min.x) min.x = p.x;
      if (p.y < min.y) min.y = p.y;
      if (p.z < min.z) min.z = p.z;
      if (p.x > max.x) max.x = p.x;
      if (p.y > max.y) max.y = p.y;
      if (p.z > max.z) max.z = p.z;
    }

    inline void expand(Vec3f newMin, Vec3f newMax) {
      if (newMin.x < min.x) min.x = newMin.x;
      if (newMin.y < min.y) min.y = newMin.y;
      if (newMin.z < min.z) min.z = newMin.z;
      if (newMax.x > max.x) max.x = newMax.x;
      if (newMax.y > max.y) max.y = newMax.y;
      if (newMax.z > max.z) max.z = newMax.z;
    }
    inline int longestAxis() const {
      float x = max.x - min.x;
      float y = max.y - min.y;
      float z = max.z - min.z;
      (void) x; (void) y; (void) z;
      if (x > y && x > z) {
        return X_AXIS;
      } else if (y > z) {
        return Y_AXIS;
      } else {
        return Z_AXIS;
      }
    }
};

#endif
