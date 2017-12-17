#include "BVH.h"

void BVH::drawBVH(){
	Vec3f min_p = ( this->getAABB() ).getMinPoint();
	Vec3f max_p = ( this->getAABB() ).getMaxPoint();
  std::cout << "min_p " << min_p << '\n';
  std::cout << "max_p " << max_p << '\n';
	drawCube(min_p, max_p);
}

void BVH::drawCube(const Vec3f & min_p, const Vec3f & max_p){
	int first_index = bvh_positions.size() - 1;
	Vec3f p1 = Vec3f(min_p[0], max_p[1], max_p[2]);
	Vec3f p2 = Vec3f(min_p[0], max_p[1], min_p[2]);
	Vec3f p3 = Vec3f(max_p[0], max_p[1], min_p[2]);
	Vec3f p4 = Vec3f(max_p[0], max_p[1], max_p[2]);
	bvh_positions.push_back(p1);
	bvh_positions.push_back(p2);
	bvh_positions.push_back(p3);
	bvh_positions.push_back(p4);
	bvh_indices.push_back(first_index + 1);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 1);
}
