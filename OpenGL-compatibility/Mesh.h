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
#include <map>

#include "glm/geometric.hpp"
using namespace std;

class Mesh {

public:

    explicit Mesh(const string& dataSource, bool fromFile, unsigned char* texData,  bool generatSmoothNormals) {
        this->texData = texData;
        if(fromFile){
            ifstream myfile(dataSource);
            this->loadFromObj(myfile, generatSmoothNormals);
        }
        else{
            this->loadFromStringData(dataSource, generatSmoothNormals);
        }
    }

    ~Mesh(){
        delete[] vertexData;
    }

    int getIndicesCount();
    int getVertexCount() const;
    void init();
    void transform(unsigned int prg, glm::mat4x4 look, glm::mat4x4 proj);
    void render(GLuint program, bool drawPolygon) const;
    tblock tblock{};

private:
    void loadFromObj(ifstream& myfile, bool generateSmoothNormals);
    void loadFromStringData(const string& filedata, bool generateSmoothNormals);
    static string eatToDelim(string *line, char c);
    string eatToSpace(string *line);
    float * generateVertexDataFromModel() ;
    const int floatsPerVertex = 3+2+3;
    int indicesCount = 0;
    int vertexCount = 0;
    float *vertexData{};
    unsigned char* texData = {};
    GLuint texture = 0;
    GLuint sampler = 0;
    GLuint vbo = 0, vao = 0;

    //v
    vector<glm::vec3> verts = {};
    //vt
    vector<glm::vec2> vertTex= {};
    //vn
    vector<glm::vec3> vertNormals= {};
    //f
    vector<unsigned int> indices = {};


    static glm::vec<3, float> calcVertNormal(pair<multimap<int, glm::vec<3, float>>::iterator, multimap<int, glm::vec<3, float>>::iterator> pair1);

    void fillVertexAndIndexVectors(const string& filedata, bool generateSmoothNormals);
};
#endif //FH_CG_MESH_H
