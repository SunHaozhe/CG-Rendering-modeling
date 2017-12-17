#include <queue>
#include <cmath>
#include "BVH.h"

void BVH::drawBVH(unsigned int numberOfNodes){
  unsigned int k = 0;
  bvh_positions.clear();
  bvh_indices.clear();

  if( numberOfNodes == 0 ) return;

  //BFS
  std::queue<BVH *> visited, unvisited;
  BVH * current;
  unvisited.push(this);
  while ( ! unvisited.empty() ){
    current = unvisited.front();
    if (current->getLeftChild()  != nullptr) unvisited.push( current->getLeftChild()  );
    if (current->getRightChild() != nullptr) unvisited.push( current->getRightChild() );
    visited.push(current);
    drawCube(current);
    unvisited.pop();
    k++;
    if( k >= numberOfNodes ) break;
  }
}

void BVH::drawCube(BVH * const bvh){
  Vec3f min_p = ( bvh->getAABB() ).getMinPoint();
	Vec3f max_p = ( bvh->getAABB() ).getMaxPoint();
	drawCube(min_p, max_p);
}

unsigned int BVH::deepToNumberOfNodes(unsigned int currentDeep){
  unsigned int sum = 0;
  for(unsigned int i = 1; i <= currentDeep; i++){
    sum += pow(2, i - 1);
  }
  return sum;
}

void BVH::drawCube(const Vec3f & min_p, const Vec3f & max_p){
	int first_index = bvh_positions.size() - 1;
	Vec3f p1 = Vec3f(min_p[0], max_p[1], max_p[2]);
	Vec3f p2 = Vec3f(min_p[0], max_p[1], min_p[2]);
	Vec3f p3 = Vec3f(max_p[0], max_p[1], min_p[2]);
	Vec3f p4 = Vec3f(max_p[0], max_p[1], max_p[2]);
  Vec3f p5 = Vec3f(min_p[0], min_p[1], max_p[2]);
  Vec3f p6 = Vec3f(min_p[0], min_p[1], min_p[2]);
  Vec3f p7 = Vec3f(max_p[0], min_p[1], min_p[2]);
  Vec3f p8 = Vec3f(max_p[0], min_p[1], max_p[2]);

	bvh_positions.push_back(p1);
	bvh_positions.push_back(p2);
	bvh_positions.push_back(p3);
	bvh_positions.push_back(p4);
  bvh_positions.push_back(p5);
  bvh_positions.push_back(p6);
  bvh_positions.push_back(p7);
  bvh_positions.push_back(p8);

	bvh_indices.push_back(first_index + 1);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 1);

  bvh_indices.push_back(first_index + 5);
	bvh_indices.push_back(first_index + 6);
	bvh_indices.push_back(first_index + 6);
	bvh_indices.push_back(first_index + 7);
	bvh_indices.push_back(first_index + 7);
	bvh_indices.push_back(first_index + 8);
	bvh_indices.push_back(first_index + 8);
	bvh_indices.push_back(first_index + 5);

  bvh_indices.push_back(first_index + 1);
	bvh_indices.push_back(first_index + 5);
	bvh_indices.push_back(first_index + 2);
	bvh_indices.push_back(first_index + 6);
	bvh_indices.push_back(first_index + 3);
	bvh_indices.push_back(first_index + 7);
	bvh_indices.push_back(first_index + 4);
	bvh_indices.push_back(first_index + 8);
}
