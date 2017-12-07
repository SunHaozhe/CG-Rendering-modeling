#include "Vec3.h"
#include "Mesh.h"

class Ray{
  private:
    Vec3f origin;
    Vec3f direction;
  public:
    Ray(){}
    Ray(Vec3f origin, Vec3f direction): origin(origin), direction(direction){}
    ~Ray(){}
    bool isIntersected(Mesh mesh) const;
};
