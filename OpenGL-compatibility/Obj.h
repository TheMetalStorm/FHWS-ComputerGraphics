//
// Created by arapo on 26.04.2023.
//

#ifndef FH_CG_OBJ_H
#define FH_CG_OBJ_H

#include "simple.h"
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "glm/geometric.hpp"
using namespace std;

class Obj {

public:

    explicit Obj(string filename) {     // Constructor
        this->loadFile(std::move(filename));
    }

    ~Obj(){
        if(vertexData){
            delete[] vertexData;
        }

    }

    int getIndicesCount();
    int getVertexCount() const;
    void init();

    void render(bool drawPolygon);

private:
    string eatToDelim(string *line, char c);
    string eatToSpace(string *line);

    void loadFile(string filename);
    int indicesCount = 0;
    int vertexCount = 0;
    //v
    vector<glm::vec3> verts = {};
    //vt
    vector<glm::vec2> vertTex= {};
    //vn
    vector<glm::vec3> vertNormals= {};
    //f
    vector<unsigned int> indices = {};
    float * generateVertexDataFromModel() ;
    float *vertexData;

    GLuint vbo, vao = 0;

};
#endif //FH_CG_OBJ_H
