#include "Vec3.h"
#include "Triangle.h"
#include "Mesh.h"
#include "Ray.h"

class AxisAlignedBoundingBox{
private:
  Vec3f min_point, max_point;
public:
  AxisAlignedBoundingBox(Vec3f min_point, Vec3f max_point) {}
};
