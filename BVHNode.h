#ifndef BVHNODE
#define BVHNODE

#include "Hitable.h"

class BVHNode: public Hitable {
  public:
    AABB box;
    Hitable* left;
    Hitable* right;
    BVHNode() {};
    BVHNode(Hitable** l, int n);
    virtual bool hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const;
    virtual bool boundingBox(AABB& box) const;
};

bool BVHNode::boundingBox(AABB& bb) const {
  bb = box;
  return true;
}

bool BVHNode::hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const {
  if (box.hit(r, tmin, tmax)) {
    RaycastResult leftRes, rightRes;
    bool leftHit = left->hit(r, tmin, tmax, leftRes);
    bool rightHit = right->hit(r, tmin, tmax, rightRes);
    if (leftHit && rightHit) {
      if (leftRes.t < rightRes.t) {
        res = leftRes;
      } else {
        res = rightRes;
      }
      return true;
    } else if (leftHit) {
      res = leftRes;
      return true;
    } else if (rightHit) {
      res = rightRes;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

int xComp(const void* a, const void* b) {
  AABB leftBox, rightBox;
  Hitable* ah = *(Hitable**)a;
  Hitable* bh = *(Hitable**)b;
  if (!ah->boundingBox(leftBox) || !bh->boundingBox(rightBox)) {
    std::cout << "no bounding box in bvh constructor" << std::endl;
  }
  if (leftBox.min.x - rightBox.min.x < 0.0) {
    return -1;
  } else {
    return 1;
  }
}

int yComp(const void* a, const void* b) {
  AABB leftBox, rightBox;
  Hitable* ah = *(Hitable**)a;
  Hitable* bh = *(Hitable**)b;
  if (!ah->boundingBox(leftBox) || !bh->boundingBox(rightBox)) {
    std::cout << "no bounding box in bvh constructor" << std::endl;
  }
  if (leftBox.min.y - rightBox.min.y < 0.0) {
    return -1;
  } else {
    return 1;
  }
}

int zComp(const void* a, const void* b) {
  AABB leftBox, rightBox;
  Hitable* ah = *(Hitable**)a;
  Hitable* bh = *(Hitable**)b;
  if (!ah->boundingBox(leftBox) || !bh->boundingBox(rightBox)) {
    std::cout << "no bounding box in bvh constructor" << std::endl;
  }
  if (leftBox.min.z - rightBox.min.z < 0.0) {
    return -1;
  } else {
    return 1;
  }
}

BVHNode::BVHNode(Hitable** l, int n) {
  AABB bb;
  AABB* boxes = new AABB[n];
  float* leftArea = new float[n];
  float* rightArea = new float[n];
  l[0]->boundingBox(bb);
  for (int i = 1; i < n; i++) {
    AABB temp;
    l[i]->boundingBox(temp);
    bb.expand(temp.min, temp.max);
  }
  int axis = bb.longestAxis();
  if (axis == X_AXIS) {
    qsort(l, n, sizeof(Hitable*), xComp);
  } else if (axis == Y_AXIS) {
    qsort(l, n, sizeof(Hitable*), yComp);
  } else {
    qsort(l, n, sizeof(Hitable*), zComp);
  }
  for (int i = 0; i < n; i++) {
    l[i]->boundingBox(boxes[i]);
  }
  leftArea[0] = boxes[0].area();
  AABB leftBox = boxes[0];
  for (int i = 1; i < n-1; i++) {
    leftBox.expand(boxes[i].min, boxes[i].max);
    leftArea[i] = leftBox.area();
  }
  rightArea[n-1] = boxes[n-1].area();
  AABB rightBox = boxes[n-1];
  for (int i = n-2; i > 0; i--) {
    rightBox.expand(boxes[i].min, boxes[i].max);
    rightArea[i] = rightBox.area();
  }
  float bestSAH = MAXFLOAT;
  int bestId = 0;
  for (int i = 0; i < n-1; i++) {
    float SAH = i*leftArea[i] + (n-i-1)*rightArea[i+1];
    if (SAH < bestSAH) {
      bestId = i;
      bestSAH = SAH;
    }
  }

  delete[] boxes;
  delete[] leftArea;
  delete[] rightArea;

  if (bestId == 0) {
    left = l[0];
  } else {
    left = new BVHNode(l, bestId + 1);
  }
  if (bestId == n-2) {
    right = l[bestId+1];
  } else {
    right = new BVHNode(l + bestId + 1, n - bestId - 1);
  }
  box = bb;
}

#endif
