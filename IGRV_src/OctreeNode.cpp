#include "OctreeNode.h"

using namespace std;

OctreeNode * OctreeNode::buildOctree(Vec3f min_point, Vec3f max_point, vector<unsigned int> ind, vector<Vec3f> & positions, unsigned int n){
  if(ind.size() > n){
    OctreeNode * octreeNode = new OctreeNode(min_point, max_point, n);
    vector<OctreeNode *> c(8);
    spatialSplit(min_point, max_point, ind, c, positions, n);
    octreeNode->getChildren( c );
    return octreeNode;
  }else{
    return new OctreeNode(ind, min_point, max_point, n);
  }
}

void OctreeNode::spatialSplit(Vec3f min_point, Vec3f max_point,  std::vector<unsigned int> & ind, vector<OctreeNode *> & c, vector<Vec3f> & positions, unsigned int n){
  Vec3f middle_point = (min_point + max_point) / 2.0f;
  c[0] = new OctreeNode(min_point, middle_point, n);
  c[1] = new OctreeNode(Vec3f(middle_point[0], min_point[1], min_point[2]), Vec3f(max_point[0], middle_point[1], middle_point[2]), n);
  c[2] = new OctreeNode(Vec3f(min_point[0], middle_point[1], min_point[2]), Vec3f(middle_point[0], max_point[1], middle_point[2]), n);
  c[3] = new OctreeNode(Vec3f(middle_point[0], middle_point[1], min_point[2]), Vec3f(max_point[0], max_point[1], middle_point[2]), n);
  c[4] = new OctreeNode(Vec3f(min_point[0], min_point[1], middle_point[2]), Vec3f(middle_point[0], middle_point[1], max_point[2]), n);
  c[5] = new OctreeNode(Vec3f(middle_point[0], min_point[1], middle_point[2]), Vec3f(max_point[0], middle_point[1], max_point[2]), n);
  c[6] = new OctreeNode(Vec3f(min_point[0], middle_point[1], middle_point[2]), Vec3f(middle_point[0], max_point[1], max_point[2]), n);
  c[7] = new OctreeNode(middle_point, max_point, n);

  for(unsigned int i = 0; i < ind.size(); i++){
    Vec3f point = positions[ind[i]];
    unsigned int x = (unsigned int) floor((point[0] - min_point[0]) / ((max_point[0] - min_point[0]) / 2.0f));
    unsigned int y = (unsigned int) floor((point[1] - min_point[1]) / ((max_point[1] - min_point[1]) / 2.0f));
    unsigned int z = (unsigned int) floor((point[2] - min_point[2]) / ((max_point[2] - min_point[2]) / 2.0f));
    if(x == 2) x = 1;
    if(y == 2) y = 1;
    if(z == 2) z = 1;
    c[x + y * 2 + z * 4]->addIndex(ind[i]);
  }

  for(unsigned int i = 0; i < 8; i++){
    if( c[i]->getIndices().size() <= n ){
      c[i]->becomesALeaf();
    }
  }
}
