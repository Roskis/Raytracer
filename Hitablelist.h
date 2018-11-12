#ifndef HITABLELIST
#define HITABLELIST

class HitableList: public Hitable {
  public:
    Hitable **list;
    int listSize;
    AABB bb;
    bool isBoundBox;
    HitableList() {};
    HitableList(Hitable **l, int n) {
      list = l;
      listSize = n;
      AABB tempBox;
      isBoundBox = true;
      if (!list[0]->boundingBox(tempBox)) {
        isBoundBox = false;
      } else {
        list[0]->boundingBox(bb);
      }
      for (int i = 1; i < listSize; i++) {
        if (list[i]->boundingBox(tempBox)) {
          bb.expand(tempBox.min);
          bb.expand(tempBox.max);
        } else {
          isBoundBox = false;
        }
      }
    };
    virtual bool hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const;
    virtual bool boundingBox(AABB& box) const;
};

bool HitableList::hit(const Ray& r, float tmin, float tmax, RaycastResult& res) const {
  RaycastResult tempRes;
  bool hitInList = false;
  float closest = tmax;
  for (int i = 0; i < listSize; i++) {
    if (list[i]->hit(r, tmin, closest, tempRes)) {
      hitInList = true;
      closest = tempRes.t;
      res = tempRes;
    }
  }
  return hitInList;
}

bool HitableList::boundingBox(AABB& box) const {
  box = bb;
  return isBoundBox;
}

#endif
