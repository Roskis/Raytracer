#ifndef HITABLE
#define HITABLE

#include "AABB.h"

class Hitable {
  public:
    virtual bool hit(const Ray& r, float t_min, float t_max, RaycastResult& res) const = 0;
    virtual Vec3f middlePoint() const;
    virtual bool boundingBox(AABB& box) const = 0;
};

Vec3f Hitable::middlePoint() const {
  return Vec3f(0);
}

#endif
