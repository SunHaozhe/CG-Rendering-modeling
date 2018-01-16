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
