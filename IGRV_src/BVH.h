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
  BVH(AxisAlignedBoundingBox bbox, Triangle triangle): bbox(bbox), isLeaf(true), triangle(triangle) {}
  BVH(AxisAlignedBoundingBox bbox, BVH * l, BVH * r): bbox(bbox), leftChild(l), rightChild(r) {}
};
