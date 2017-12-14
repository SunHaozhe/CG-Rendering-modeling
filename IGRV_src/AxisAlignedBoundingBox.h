class AxisAlignedBoundingBox{
private:
  Vec3f position;
  Vec3f normal;
  Vec3f average_color;
  Ray ray;
public:
  AxisAlignedBoundingBox();
  ~AxisAlignedBoundingBox();
};
