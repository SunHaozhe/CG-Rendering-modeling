
class BVH{
private:
  AxisAlignedBoundingBox bbox;
  BVH * leftChild;
  BVH * rightChild;
public:
  BVH(const Mesh & mesh);
};
