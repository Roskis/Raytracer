#ifndef MATERIAL
#define MATERIAL

struct RaycastResult;

Vec3f reflect(const Vec3f& v, const Vec3f& n) {
  return v - 2 * dot(v, n) * n;
}

bool refract(const Vec3f& vin, const Vec3f& n, float ni_over_nt, Vec3f& vout) {
  Vec3f uv = unitVector(vin);
  float dt = dot(uv, n);
  float dis = 1.0 - ni_over_nt * ni_over_nt * (1 - dt*dt);
  if (dis > 0) {
    vout = ni_over_nt * (uv - n*dt) - n * sqrt(dis);
    return true;
  } else {
    return false;
  }
}

float schlick(float cos, float ref_idx) {
  float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cos), 5);
}

class Material {
  public:
    virtual bool scatter(const Ray& rin, const RaycastResult& res, Vec3f& attenuation, Ray& rout) const = 0;
};

class Lambertian: public Material {
  public:
    Vec3f albedo;
    Lambertian(const Vec3f& a): albedo(a) {}
    virtual bool scatter(const Ray& rin, const RaycastResult& res, Vec3f& attenuation, Ray& rout) const {
      (void) rin;
      Vec3f target = res.p + res.normal + rejectionSample();
      rout = Ray(res.p, target - res.p);
      attenuation = albedo;
      return true;
    }
};

class Metal: public Material {
  public:
    Vec3f albedo;
    float fuzz;
    Metal(const Vec3f& a, float f): albedo(a) {if (f < 1) fuzz = f; else fuzz = 1;}
    virtual bool scatter(const Ray& rin, const RaycastResult& res, Vec3f& attenuation, Ray& rout) const {
      Vec3f reflected = reflect(unitVector(rin.d()), res.normal);
      rout = Ray(res.p, reflected + fuzz * rejectionSample());
      attenuation = albedo;
      return (dot(rout.d(), res.normal) > 0);
    }
};

class Dielectric: public Material {
  public:
    float ref_idx;
    Dielectric(float ri): ref_idx(ri) {}
    virtual bool scatter(const Ray& rin, const RaycastResult& res, Vec3f& attenuation, Ray& rout) const {
      Vec3f outNormal;
      Vec3f reflected = reflect(rin.d(), res.normal);
      float ni_over_nt;
      attenuation = Vec3f(1.0, 1.0, 1.0);
      Vec3f refracted;
      float reflect_prob;
      float cos;
      if (dot(rin.d(), res.normal) > 0) {
        outNormal = -res.normal;
        ni_over_nt = ref_idx;
        cos = dot(rin.d(), res.normal) / rin.d().length();
        cos = sqrt(1 - ref_idx * ref_idx * (1 - cos*cos));
      } else {
        outNormal = res.normal;
        ni_over_nt = 1.0 / ref_idx;
        cos = -dot(rin.d(), res.normal) / rin.d().length();
      }
      if (refract(rin.d(), outNormal, ni_over_nt, refracted)) {
        reflect_prob = schlick(cos, ref_idx);
      } else {
        reflect_prob = 1.0;
      }
      if (rand01() < reflect_prob) {
        rout = Ray(res.p, reflected);
      } else {
        rout = Ray(res.p, refracted);
      }
      return true;
    }
};

#endif
