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

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <limits>

using namespace std;

void Mesh::clear () {
    m_positions.clear ();
    m_normals.clear ();
    m_triangles.clear ();
}

void Mesh::loadOFF (const std::string & filename) {
    clear ();
	ifstream in (filename.c_str ());
    if (!in)
        exit (1);
	string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    m_positions.resize (sizeV);
    m_triangles.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> m_positions[i];
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> m_triangles[i][j];
    }
    in.close ();
    centerAndScaleToUnit ();
    recomputeNormals ();
    loaded_filename = filename;
}

void Mesh::recomputeNormals () {
    m_normals.clear ();
    m_normals.resize (m_positions.size (), Vec3f (0.f, 0.f, 0.f));
    for (unsigned int i = 0; i < m_triangles.size (); i++) {
        Vec3f e01 = m_positions[m_triangles[i][1]] -  m_positions[m_triangles[i][0]];
        Vec3f e02 = m_positions[m_triangles[i][2]] -  m_positions[m_triangles[i][0]];
        Vec3f n = cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            m_normals[m_triangles[i][j]] += n;
    }
    for (unsigned int i = 0; i < m_normals.size (); i++)
        m_normals[i].normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3f c;
    for  (unsigned int i = 0; i < m_positions.size (); i++)
        c += m_positions[i];
    c /= m_positions.size ();
    float maxD = dist (m_positions[0], c);
    for (unsigned int i = 0; i < m_positions.size (); i++){
        float m = dist (m_positions[i], c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < m_positions.size (); i++)
        m_positions[i] = (m_positions[i] - c) / maxD;
}

void Mesh::topologicalLaplacianFilter (float laplace_alpha){
  vector<Vec3f> new_positions(m_positions.size(), Vec3f(0.0f, 0.0f, 0.0f) );
  vector<Vec3f> delta(m_positions.size(), Vec3f(0.0f, 0.0f, 0.0f) );
  vector<unsigned int> counts_neighbers(m_positions.size(), 0);
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      unsigned int edge_index_i = m_triangles[index][i];
      unsigned int edge_index_j = (i == 2) ? m_triangles[index][0] : m_triangles[index][i + 1];
      new_positions[edge_index_j] += m_positions[edge_index_i];
      counts_neighbers[edge_index_j] += 1;
    }
  }
  for(unsigned int i = 0; i < m_positions.size(); i++) new_positions[i] /= counts_neighbers[i];
  for(unsigned int i = 0; i < m_positions.size(); i++) delta[i] = m_positions[i] - new_positions[i];
  for(unsigned int i = 0; i < m_positions.size(); i++) m_positions[i] -= laplace_alpha * delta[i];
  recomputeNormals();
}

void Mesh::reloadOFF(){
  loadOFF(loaded_filename);
}

void Mesh::geometricLaplacianFilter (float laplace_alpha){
  vector<Vec3f> new_positions(m_positions.size(), Vec3f(0.0f, 0.0f, 0.0f) );
  vector<Vec3f> delta(m_positions.size(), Vec3f(0.0f, 0.0f, 0.0f) );
  vector<float> counts_neighbers(m_positions.size(), 0.0f);
  map<pair<int, int>, float> weights;

  //To build a map for weights
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      unsigned int edge_index_i = m_triangles[index][i];
      unsigned int edge_index_j = (i == 2) ? m_triangles[index][0] : m_triangles[index][i + 1];
      unsigned int third_vertex_index = (i == 0) ? m_triangles[index][2] : m_triangles[index][i - 1];
      Vec3f edge1 = m_positions[edge_index_i] - m_positions[third_vertex_index];
      Vec3f edge2 = m_positions[edge_index_j] - m_positions[third_vertex_index];
      weights[pair_maker(edge_index_i, edge_index_j)] += 0.5f * getCotan(edge1, edge2);
    }
  }
  //To calculate new_positions
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      unsigned int edge_index_i = m_triangles[index][i];
      unsigned int edge_index_j = (i == 2) ? m_triangles[index][0] : m_triangles[index][i + 1];
      new_positions[edge_index_j] += weights[pair_maker(edge_index_i, edge_index_j)] * m_positions[edge_index_i];
      counts_neighbers[edge_index_j] += weights[pair_maker(edge_index_i, edge_index_j)];
    }
  }
  for(unsigned int i = 0; i < m_positions.size(); i++) new_positions[i] /= counts_neighbers[i];
  for(unsigned int i = 0; i < m_positions.size(); i++) delta[i] = m_positions[i] - new_positions[i];
  for(unsigned int i = 0; i < m_positions.size(); i++) m_positions[i] -= laplace_alpha * delta[i];
  recomputeNormals();
}

pair<int, int> Mesh::pair_maker(int a, int b){
    if ( a < b ) return pair<int, int>(a,b);
    else return pair<int, int>(b,a);
}

float Mesh::getCotan(Vec3f v1, Vec3f v2){
  float angle = acosf(dot(v1, v2) / sqrt(length(v1) * length(v2)));
  return cos(angle) / sin(angle);
}

void Mesh::calculateMinMax(Vec3f & min_p, Vec3f & max_p){
	for(unsigned int i = 0; i < m_triangles.size(); i++){
		if( max_p[0] < m_positions[ m_triangles[i][0] ][0] ) max_p[0] = m_positions[ m_triangles[i][0] ][0];
		if( max_p[1] < m_positions[ m_triangles[i][0] ][1] ) max_p[1] = m_positions[ m_triangles[i][0] ][1];
		if( max_p[2] < m_positions[ m_triangles[i][0] ][2] ) max_p[2] = m_positions[ m_triangles[i][0] ][2];
		if( min_p[0] > m_positions[ m_triangles[i][0] ][0] ) min_p[0] = m_positions[ m_triangles[i][0] ][0];
		if( min_p[1] > m_positions[ m_triangles[i][0] ][1] ) min_p[1] = m_positions[ m_triangles[i][0] ][1];
		if( min_p[2] > m_positions[ m_triangles[i][0] ][2] ) min_p[2] = m_positions[ m_triangles[i][0] ][2];

		if( max_p[0] < m_positions[ m_triangles[i][1] ][0] ) max_p[0] = m_positions[ m_triangles[i][1] ][0];
		if( max_p[1] < m_positions[ m_triangles[i][1] ][1] ) max_p[1] = m_positions[ m_triangles[i][1] ][1];
		if( max_p[2] < m_positions[ m_triangles[i][1] ][2] ) max_p[2] = m_positions[ m_triangles[i][1] ][2];
		if( min_p[0] > m_positions[ m_triangles[i][1] ][0] ) min_p[0] = m_positions[ m_triangles[i][1] ][0];
		if( min_p[1] > m_positions[ m_triangles[i][1] ][1] ) min_p[1] = m_positions[ m_triangles[i][1] ][1];
		if( min_p[2] > m_positions[ m_triangles[i][1] ][2] ) min_p[2] = m_positions[ m_triangles[i][1] ][2];

		if( max_p[0] < m_positions[ m_triangles[i][2] ][0] ) max_p[0] = m_positions[ m_triangles[i][2] ][0];
		if( max_p[1] < m_positions[ m_triangles[i][2] ][1] ) max_p[1] = m_positions[ m_triangles[i][2] ][1];
		if( max_p[2] < m_positions[ m_triangles[i][2] ][2] ) max_p[2] = m_positions[ m_triangles[i][2] ][2];
		if( min_p[0] > m_positions[ m_triangles[i][2] ][0] ) min_p[0] = m_positions[ m_triangles[i][2] ][0];
		if( min_p[1] > m_positions[ m_triangles[i][2] ][1] ) min_p[1] = m_positions[ m_triangles[i][2] ][1];
		if( min_p[2] > m_positions[ m_triangles[i][2] ][2] ) min_p[2] = m_positions[ m_triangles[i][2] ][2];
	}
}

void Mesh::make_cubes(float & x_scale, float & y_scale, float & z_scale, unsigned int resolution, Vec3f & min_p){
  float max_float = numeric_limits<float>::max();
	float min_float =  - ( numeric_limits<float>::max() - 1);
	Vec3f max_p = Vec3f(min_float, min_float, min_float);
	min_p = Vec3f(max_float, max_float, max_float);
	calculateMinMax(min_p, max_p);
  min_p -= (max_p - min_p) * 0.05f;
  max_p += (max_p - min_p) * 0.05f;
  x_scale = (max_p[0] - min_p[0]) / (float) resolution;
  y_scale = (max_p[1] - min_p[1]) / (float) resolution;
  z_scale = (max_p[2] - min_p[2]) / (float) resolution;
}

void Mesh::push_vertices_into_cubes(UGrid & grid, unsigned int resolution, float x_scale, float y_scale, float z_scale, vector<unsigned int> & index_map, Vec3f & min_p){
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      Vec3f vertex_position = m_positions[ m_triangles[index][i] ];
      unsigned int x = (vertex_position[0] - min_p[0]) / x_scale;
      unsigned int y = (vertex_position[1] - min_p[1]) / y_scale;
      unsigned int z = (vertex_position[2] - min_p[2]) / z_scale;
      grid.getCell(x, y, z).addNewVertex(m_triangles[index][i], vertex_position, m_normals[ m_triangles[index][i] ]);
      index_map[m_triangles[index][i]] = z + y * resolution + x * resolution * resolution;
    }
  }
}

void Mesh::calculate_new_positions(vector<Vec3f> & new_positions, vector<Vec3f> & new_normals, UGrid & grid){
  for(unsigned int x = 0; x < grid.nx; x++){
    for(unsigned int y = 0; y < grid.ny; y++){
      for(unsigned int z = 0; z < grid.nz; z++){
        new_positions.push_back(grid.getCell(x, y, z).meanPosition);
        new_normals.push_back(grid.getCell(x, y, z).meanNormal);
      }
    }
  }
}

void Mesh::reindex(const vector<unsigned int> & index_map, vector<Triangle> & new_triangles){
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    if(index_map[m_triangles[index][0]] != index_map[m_triangles[index][1]] &&
       index_map[m_triangles[index][1]] != index_map[m_triangles[index][2]] &&
       index_map[m_triangles[index][0]] != index_map[m_triangles[index][2]] ){
       new_triangles.push_back(Triangle(index_map[m_triangles[index][0]], index_map[m_triangles[index][1]], index_map[m_triangles[index][2]] ) );
    }
  }
}

void Mesh::simplify(unsigned int resolution){
  vector<Vec3f> new_positions;
  vector<Triangle> new_triangles;
  vector<Vec3f> new_normals;
  vector<unsigned int> index_map(3 * m_triangles.size());
  UGrid grid(resolution, resolution, resolution);
  Vec3f min_p;
	float x_scale, y_scale, z_scale;
  make_cubes(x_scale, y_scale, z_scale, resolution, min_p);
  push_vertices_into_cubes(grid, resolution, x_scale, y_scale, z_scale, index_map, min_p);
  calculate_new_positions(new_positions, new_normals, grid);
  for(unsigned int i = 0; i < new_normals.size(); i++) new_normals[i].normalize();
  reindex(index_map, new_triangles);

  new_positions.swap(m_positions);
  new_triangles.swap(m_triangles);
  new_normals.swap(m_normals);
}

void Mesh::addOddVertices(map<pair<int, int>, Vec3f> & oddVertices, map<pair<int, int>, unsigned int> & oddVertexIndices, vector<Vec3f> & new_positions){
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      unsigned int edge_index_i = m_triangles[index][i];
      unsigned int edge_index_j = (i == 2) ? m_triangles[index][0] : m_triangles[index][i + 1];
      unsigned int third_vertex_index = (i == 0) ? m_triangles[index][2] : m_triangles[index][i - 1];
      oddVertices[pair_maker(edge_index_i, edge_index_j)] += (1.0f / 8.0f) * m_positions[third_vertex_index] + (3.0f / 16.0f) * (m_positions[edge_index_i] + m_positions[edge_index_j]);
    }
  }
  for(auto it : oddVertices){
    oddVertexIndices[it.first] = new_positions.size();
    new_positions.push_back(it.second);
  }
}

void Mesh::updateEvenVertices(vector<unsigned int> & valence, vector<Vec3f> & new_positions){
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      unsigned int edge_index_j = (i == 2) ? m_triangles[index][0] : m_triangles[index][i + 1];
      valence[edge_index_j] += 1;
    }
  }
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      unsigned int edge_index_j = (i == 2) ? m_triangles[index][0] : m_triangles[index][i + 1];
      unsigned int n = valence[edge_index_j];
      if( length(new_positions[edge_index_j] - m_positions[edge_index_j]) < length(m_positions[edge_index_j]) * 0.01f ) new_positions[edge_index_j] *= (1.0f - getAlpha(n));
    }
  }
  for(unsigned int index = 0; index < m_triangles.size(); index++){
    for(unsigned int i = 0; i < 3; i++){
      unsigned int edge_index_i = m_triangles[index][i];
      unsigned int edge_index_j = (i == 2) ? m_triangles[index][0] : m_triangles[index][i + 1];
      unsigned int n = valence[edge_index_j];
      new_positions[edge_index_j] += m_positions[edge_index_i] * (getAlpha(n) / (float) n);
    }
  }
}

void Mesh::reindex_subdivision(map<pair<int, int>, unsigned int> & oddVertexIndices, vector<Triangle> & new_triangles){
  for(unsigned int i = 0; i < m_triangles.size(); i++){
    unsigned int index0 = m_triangles[i][0];
    unsigned int index1 = m_triangles[i][1];
    unsigned int index2 = m_triangles[i][2];
    unsigned int odd0 = oddVertexIndices[pair_maker(index0, index1)];
    unsigned int odd1 = oddVertexIndices[pair_maker(index1, index2)];
    unsigned int odd2 = oddVertexIndices[pair_maker(index2, index0)];
    new_triangles.push_back(Triangle(index0, odd0, odd2));
    new_triangles.push_back(Triangle(odd0, index1, odd1));
    new_triangles.push_back(Triangle(odd2, odd1, index2));
    new_triangles.push_back(Triangle(odd0, odd1, odd2));
  }
}

void Mesh::subdivide(){
  map<pair<int, int>, Vec3f> oddVertices;
  map<pair<int, int>, unsigned int> oddVertexIndices;
  vector<Vec3f> new_positions;
  vector<Triangle> new_triangles;
  vector<unsigned int> valence(m_positions.size(), 0);

  new_positions = m_positions;

  addOddVertices(oddVertices, oddVertexIndices, new_positions);
  updateEvenVertices(valence, new_positions);
  reindex_subdivision(oddVertexIndices, new_triangles);

  new_positions.swap(m_positions);
  new_triangles.swap(m_triangles);
  recomputeNormals();
}
