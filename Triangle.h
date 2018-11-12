#ifndef TRIANGLE
#define TRIANGLE

class Triangle: public Hitable {
  public:
    Vec3f v0, v1, v2, normal;
    Material* mat_ptr;
    AABB bb;
    Triangle() {};
    Triangle(Vec3f vertex1, Vec3f vertex2, Vec3f vertex3, Material* m):
      v0(vertex1), v1(vertex2), v2(vertex3), mat_ptr(m) {
        normal = cross((v1 - v0), (v2 - v0)).normalize();
        bb = AABB(v0);
        bb.expand(v1);
        bb.expand(v2);
      };
    virtual bool hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const;
    virtual bool boundingBox(AABB& box) const;
};

bool Triangle::hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const {
  Vec3f v1v0 = v1 - v0;
  Vec3f v2v0 = v2 - v0;
  Vec3f rov0 = r.o() - v0;

  Vec3f n = cross(v1v0, v2v0);
  Vec3f q = cross(rov0, r.d());
  float d = 1.0 / dot(r.d(), n);
  float u = d * dot(-q, v2v0);
  float v = d * dot(q, v1v0);
  float t = d * dot(-n, rov0);

  t = std::min(u, std::min(v, std::min(1.0f - u - v, t)));

  if (t < tmax && t > tmin) {
    res.t = t;
    res.p = r.p(t);
    res.normal = normal;
    res.mat_ptr = mat_ptr;
    return true;
  }
  return false;
}

bool Triangle::boundingBox(AABB& box) const {
  box = bb;
  return true;
}

#endif
