//
// Created by arapo on 26.04.2023.
//

#ifndef FH_CG_MESH_H
#define FH_CG_MESH_H

#include "simple.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "glm/geometric.hpp"
using namespace std;

class Mesh {

public:

    explicit Mesh(string filename) {     // Constructor
        this->loadFromObj(std::move(filename));
    }

    ~Mesh(){
        if(vertexData){
            delete[] vertexData;
        }

    }

    int getIndicesCount();
    int getVertexCount() const;
    void init();

    void render(bool drawPolygon);

private:
    void loadFromObj(string filename);
    string eatToDelim(string *line, char c);
    string eatToSpace(string *line);
    float * generateVertexDataFromModel() ;

    int floatsPerVertex = 3+2+3;
    int indicesCount = 0;
    int vertexCount = 0;
    float *vertexData;
    GLuint vbo, vao = 0;

    //v
    vector<glm::vec3> verts = {};
    //vt
    vector<glm::vec2> vertTex= {};
    //vn
    vector<glm::vec3> vertNormals= {};
    //f
    vector<unsigned int> indices = {};


};
#endif //FH_CG_MESH_H
