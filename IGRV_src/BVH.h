#include "AxisAlignedBoundingBox.h"

class BVH{
private:
  AxisAlignedBoundingBox bbox;
  BVH * leftChild = nullptr;
  BVH * rightChild = nullptr;
  bool isLeaf = false;
  Triangle triangle;

  //AxisAlignedBoundingBox calculateBoundingBox();
  //void redistributeTriangles();
public:
  BVH(AxisAlignedBoundingBox bbox): bbox(bbox) {}
  ~BVH();
};
