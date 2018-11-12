#ifndef SPHERE
#define SPHERE

class Sphere: public Hitable {
  public:
    Vec3f center;
    float radius;
    Material* mat_ptr;
    AABB bb;
    Sphere() {};
    Sphere(Vec3f c, float r, Material* m): center(c), radius(r), mat_ptr(m) {
      bb = AABB(center - Vec3f(radius), center + Vec3f(radius));
    };
    virtual bool hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const;
    virtual Vec3f middlePoint() const;
    virtual bool boundingBox(AABB& box) const;
};

Vec3f Sphere::middlePoint() const {
  return center;
}

bool Sphere::hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const {
  Vec3f oc = r.o() - center;
  float a = dot(r.d(), r.d());
  float b = dot(oc, r.d());
  float c = dot(oc, oc) - radius*radius;
  float dis = b*b - a*c;
  if (dis > 0) {
    float temp = (-b - sqrt(b*b - a*c)) / a;
    if (temp < tmax && temp > tmin) {
      res.t = temp;
      res.p = r.p(res.t);
      res.normal = (res.p - center) / radius;
      res.mat_ptr = mat_ptr;
      return true;
    }
    temp = (-b + sqrt(b*b - a*c)) / a;
    if (temp < tmax && temp > tmax) {
      res.t = temp;
      res.p = r.p(res.t);
      res.normal = (res.p - center) / radius;
      res.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}

bool Sphere::boundingBox(AABB& box) const {
  box = bb;
  return true;
}

#endif
