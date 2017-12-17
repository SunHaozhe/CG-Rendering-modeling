#include "AxisAlignedBoundingBox.h"

class BVH{
private:
  AxisAlignedBoundingBox bbox;
  BVH * leftChild = nullptr;
  BVH * rightChild = nullptr;
  bool isLeaf = false;
  Triangle triangle;

  static void drawCube(const Vec3f & min_p, const Vec3f & max_p);
  static void drawCube(BVH * const bvh);

public:
  static std::vector<Vec3f> bvh_positions;
  static std::vector<unsigned int> bvh_indices;

  static unsigned int deepToNumberOfNodes(unsigned int currentDeep);

  BVH(AxisAlignedBoundingBox bbox, Triangle triangle): bbox(bbox), isLeaf(true), triangle(triangle) {}
  BVH(AxisAlignedBoundingBox bbox, BVH * l, BVH * r): bbox(bbox), leftChild(l), rightChild(r) {}
  BVH * getLeftChild() {return leftChild;}
  BVH * getRightChild() {return rightChild;}
  AxisAlignedBoundingBox getAABB() {return bbox;}
  bool isALeaf() {return isLeaf;}
  Triangle getTriangle() {return triangle;}
  void drawBVH(unsigned int numberOfNodes);
};
