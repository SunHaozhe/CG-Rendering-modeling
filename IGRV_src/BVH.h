
class BVH{
private:
  AxisAlignedBoundingBox bbox;
  BVH * leftChild;
  BVH * rightChild;

  AxisAlignedBoundingBox calculateBoundingBox();
  void redistributeTriangles();
public:
  BVH(const Mesh & mesh);
  ~BVH();
};
