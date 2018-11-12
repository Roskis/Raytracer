#ifndef RAY
#define RAY

#include "Vec3f.h"

class Ray{
  public:
    Vec3f A, B;
    Ray() {}
    Ray(const Vec3f& a, const Vec3f& b) {
      A = a;
      B = b;
    }
    Vec3f pointAtParameter(float t) const {
      return A + t*B;
    }
    inline Vec3f origin() const { return A; }
    inline Vec3f direction() const { return B; }

    inline Vec3f orig() const { return origin(); }
    inline Vec3f dir() const { return direction(); }
    inline Vec3f o() const { return origin(); }
    inline Vec3f d() const { return direction(); }
    inline Vec3f p(float t) const { return pointAtParameter(t); }
};

#endif
