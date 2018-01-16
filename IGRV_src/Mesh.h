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
#include "Vec3.h"
#include "Triangle.h"

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


private:
    std::vector<Vec3f> m_positions;
    std::vector<Vec3f> m_normals;
    std::vector<Triangle> m_triangles;
    std::string loaded_filename;

    std::pair<int, int> pair_maker(int a, int b);
    float getCotan(Vec3f v1, Vec3f v2);
};
