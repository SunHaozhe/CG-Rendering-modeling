#include <vector>

#include "Vec3.h"

class OctreeNode{
private:
  std::vector<OctreeNode *> children;
  std::vector<unsigned int> indices;
  Vec3f min_point, max_point;
  bool leafStatus = false;
  unsigned int n;   // max number of vertices per leaf

  OctreeNode(): leafStatus(false), n(0) {
    children.clear();
    indices.clear();
  }

  OctreeNode(std::vector<unsigned int> indices, Vec3f min_point, Vec3f max_point, unsigned int n): indices(indices), min_point(min_point), max_point(max_point), leafStatus(true), n(n) {children.clear();}

  OctreeNode(Vec3f min_point, Vec3f max_point, unsigned int n): min_point(min_point), max_point(max_point), leafStatus(false), n(n) {
    children.resize(8);
    indices.clear();
  }

  static void spatialSplit(Vec3f min_point, Vec3f max_point,  std::vector<unsigned int> & ind, std::vector<OctreeNode *> & c, std::vector<Vec3f> & positions, unsigned int n);
  void getChildren(std::vector<OctreeNode *> c){
    if(isLeaf()) std::cout << "???" << '\n';
    else children = c;
  }
  void addIndex(unsigned int index){indices.push_back(index);}


public:
  static OctreeNode * buildOctree(Vec3f min_point, Vec3f max_point, std::vector<unsigned int> ind, std::vector<Vec3f> & positions, unsigned int n);
  bool isLeaf(){return leafStatus;}
  std::vector<unsigned int> getIndices(){return indices;}
  std::vector<OctreeNode *> getChildren(){return children;}
  void becomesALeaf(){leafStatus = true;}
};
