#ifndef CAMERA
#define CAMERA

class Camera {
  public:
    Vec3f lowerLeft, horizontal, vertical, origin, u, v, w;
    float lens_radius;
    Camera() {}
    Camera(Vec3f from, Vec3f to, Vec3f up, float fov, float aspect, float aperture, float focus_dist) {
      lens_radius = aperture / 2;
      float theta = fov * M_PI / 180;
      float halfHeight = tan(theta/2);
      float halfWidth = aspect * halfHeight;
      origin = from;
      w = unitVector(from - to);
      u = unitVector(cross(up, w));
      v = cross(w, u);
      lowerLeft = origin - halfWidth * focus_dist * u - halfHeight * focus_dist * v - focus_dist * w;
      horizontal = 2.0 * halfWidth * focus_dist * u;
      vertical = 2.0 * halfHeight * focus_dist * v;
    }
    Ray getRay(float s, float t) {
      Vec3f rd = lens_radius * rejectionSample();
      Vec3f offset = u * rd.x + v * rd.y;
      return Ray(origin + offset, lowerLeft + s * horizontal + t * vertical - origin - offset);
    }
};

#endif
