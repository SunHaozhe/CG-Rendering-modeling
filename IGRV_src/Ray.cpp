#include <cmath>
#include <vector>
#include "Triangle.h"
#include "Ray.h"

using namespace std;

//https://www.scratchapixel.com/code.php?id=13&origin=/lessons/3d-basic-rendering/introduction-to-shading
//http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/

bool Ray::isIntersected(Mesh mesh) const {
  float epsilon = 0.0001;
  bool intersected = false;

  vector<Triangle> triangle_indices = mesh.triangles ();
  vector<Vec3f> vertex_list = mesh.positions ();

  for(unsigned int i = 0; i < triangle_indices.size(); i++){
    Triangle threeIndices = triangle_indices[i];
    Vec3f v0 = vertex_list[threeIndices[0]];
    Vec3f v1 = vertex_list[threeIndices[1]];
    Vec3f v2 = vertex_list[threeIndices[2]];

    Vec3f v0v1 = v1 - v0;
    Vec3f v0v2 = v2 - v0;
    Vec3f pvec = cross(direction, v0v2);
    float det = dot(v0v1, pvec);

    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < epsilon) continue;

    float invDet = 1.0 / det;

    Vec3f tvec = origin - v0;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0.0 || u > 1.0) continue;

    Vec3f qvec = cross(tvec, v0v1);
    float v = dot(direction, qvec) * invDet;
    if (v < 0.0 || u + v > 1.0) continue;

    float t = dot(v0v2, qvec) * invDet;

    if(t > 0.0){
      intersected = true;
      break;
    }
  }
  
  return intersected;
}
