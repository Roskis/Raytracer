#ifndef VEC3F
#define VEC3F

#include <cmath>

class Vec3f {
  public:
    float x, y, z;
    Vec3f(): x(0), y(0), z(0) {}
    Vec3f(float xx): x(xx), y(xx), z(xx) {}
    Vec3f(float xx, float yy, float zz): x(xx), y(yy), z(zz) {}

    inline Vec3f operator* (const Vec3f &v) const {
      return Vec3f(x * v.x, y * v.y, z * v.z);
    }

    inline Vec3f& operator*= (const Vec3f &v) {
      x *= v.x, y *= v.y, z *= v.z; return *this;
    }

    inline Vec3f operator/ (const Vec3f &v) const {
      return Vec3f(x / v.x, y / v.y, z / v.z);
    }

    inline Vec3f& operator/= (const Vec3f &v) {
      x /= v.x, y /= v.y, z /= v.z; return *this;
    }

    inline Vec3f operator+ (const Vec3f &v) const {
      return Vec3f(x + v.x, y + v.y, z + v.z);
    }

    inline Vec3f& operator+= (const Vec3f &v) {
      x += v.x, y += v.y, z += v.z; return *this;
    }

    inline Vec3f operator- (const Vec3f &v) const {
      return Vec3f(x - v.x, y - v.y, z - v.z);
    }

    inline Vec3f operator- () const {
      return Vec3f(-x, -y, -z);
    }

    inline float operator[] (const int axis) const {
      if (axis == 2) {
        return z;
      }
      if (axis == 1) {
        return y;
      }
      return x;
    }

    inline float& operator[] (const int axis) {
      if (axis == 2) {
        return z;
      }
      if (axis == 1) {
        return y;
      }
      return x;
    }

    inline float length() const {
      return sqrt(length2());
    }

    inline float length2() const {
      return x*x + y*y + z*z;
    }

    inline float dot(const Vec3f &v) const {
      return x * v.x + y * v.y + z * v.z;
    }

    inline Vec3f& normalize() {
      float inverseLength = 1 / sqrt(length2());
      x *= inverseLength; y *= inverseLength; z *= inverseLength;
      return *this;
    }

    inline friend std::istream& operator >> (std::istream &is, Vec3f &v) {
      is >> v.x >> v.y >> v.z;
      return is;
    }

    inline friend std::ostream& operator << (std::ostream &os, const Vec3f &v) {
      os << "[x: " << v.x << " y: " << v.y << " z: " << v.z << "]";
      return os;
    }
};

inline Vec3f operator* (const Vec3f &v, const float &f) {
  return Vec3f(f * v.x, f * v.y, f * v.z);
}

inline Vec3f operator* (const float &f, const Vec3f &v) {
  return Vec3f(f * v.x, f * v.y, f * v.z);
}

inline Vec3f operator/ (const Vec3f &v, const float &f) {
  float inv = 1.0 / f;
  return Vec3f(v.x * inv, v.y * inv, v.z * inv);
}

inline Vec3f operator+ (const Vec3f &v, const float &f) {
  return Vec3f(f + v.x, f + v.y, f + v.z);
}
inline Vec3f operator+ (const float &f, const Vec3f &v) {
  return Vec3f(f + v.x, f + v.y, f + v.z);
}

inline Vec3f operator- (const Vec3f &v, const float &f) {
  return Vec3f(f - v.x, f - v.y, f - v.z);
}
inline Vec3f operator- (const float &f, const Vec3f &v) {
  return Vec3f(f - v.x, f - v.y, f - v.z);
}

inline float dot(const Vec3f &v1, const Vec3f &v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vec3f cross(const Vec3f &a, const Vec3f &b) {
  return Vec3f(a.y * b.z - a.z * b.y,
               a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x);
}

inline Vec3f unitVector(Vec3f v) {
  return v / v.length();
}

inline float distance(const Vec3f& a, const Vec3f& b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

#endif
