// --------------------------------------------------------------------------
// Copyright(C) 2009-2016
// Tamy Boubekeur
//
// Permission granted to use this code only for teaching projects and
// private practice.
//
// Do not distribute this code outside the teaching assignements.
// All rights reserved.
// --------------------------------------------------------------------------

#pragma once
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include "Vec3.h"
#include "Triangle.h"

struct UGridCell{
  Vec3f meanPosition;
  unsigned int count;
  std::set<unsigned int> indices;

  UGridCell(): meanPosition(0.0f, 0.0f, 0.0f), count(0) {}

  void addNewVertex(unsigned int index, Vec3f new_vertex){
    meanPosition = meanPosition * count + new_vertex;
    count++;
    meanPosition /= count;
    indices.insert(index);
  }
};

struct UGrid{
  unsigned int nx, ny, nz;
  std::vector<UGridCell> cells;

  UGrid(unsigned int x_, unsigned int y_, unsigned int z_): nx(x_), ny(y_), nz(z_) {
    cells.clear();
    cells.resize(nx * ny * nz);
  }

  UGridCell & getCell(unsigned int x, unsigned int y, unsigned int z){
    return cells[z + nz * y + nz * ny * x];
  }
};

/// A Mesh class, storing a list of vertices and a list of triangles indexed over it.
class Mesh {
public:
    inline Mesh () {}
    inline virtual ~Mesh () {}

    inline std::vector<Vec3f> & positions () { return m_positions; }
    inline const std::vector<Vec3f> & positions () const { return m_positions; }
    inline  std::vector<Vec3f> & normals () { return m_normals; }
    inline const std::vector<Vec3f> & normals () const { return m_normals; }
    inline std::vector<Triangle> & triangles () { return m_triangles; }
    inline const std::vector<Triangle> & triangles () const { return m_triangles; }

    /// Empty the positions, normals and triangles arrays.
    void clear ();

	/// Loads the mesh from a <file>.off
	void loadOFF (const std::string & filename);

    /// Compute smooth per-vertex normals
    void recomputeNormals ();

    /// scale to the unit cube and center at original
    void centerAndScaleToUnit ();

    // Topological laplace operator
    void topologicalLaplacianFilter (float laplace_alpha);

    void reloadOFF();

    // Geometric laplace operator
    void geometricLaplacianFilter (float laplace_alpha);

    // OCS simplification
    void simplify(unsigned int resolution);


private:
    std::vector<Vec3f> m_positions;
    std::vector<Vec3f> m_normals;
    std::vector<Triangle> m_triangles;
    std::string loaded_filename;

    std::pair<int, int> pair_maker(int a, int b);
    float getCotan(Vec3f v1, Vec3f v2);
    void calculateMinMax(Vec3f & min_p, Vec3f & max_p);
    void make_cubes(float & x_scale, float & y_scale, float & z_scale, unsigned int resolution, Vec3f & min_p);
    void push_vertices_into_cubes(UGrid & grid, unsigned int resolution,
      float x_scale, float y_scale, float z_scale, std::vector<unsigned int> & index_map, Vec3f & min_p);
    void calculate_new_positions(std::vector<Vec3f> & new_positions, UGrid & grid);
    void reindex(const std::vector<unsigned int> & index_map, std::vector<Triangle> & new_triangles);
};
