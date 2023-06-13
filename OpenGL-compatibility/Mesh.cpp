// Created by arapo on 29.04.2023.
//
#include "Mesh.h"
#include <glm/gtx/normal.hpp>

string Mesh::eatToDelim(string *line, char c) {

    if (line->empty()) return "none";

    for (int i = 0; i < line->size(); i++) {
        if (line->at(i) == c) {
            string res = line->substr(0, i);
            line->erase(0, i + 1);
            return res;
        } else if (i == line->size() - 1) {
            return *line;
        }
    }
}

string Mesh::eatToSpace(string *line) {
    return eatToDelim(line, ' ');
}

glm::vec<3, float> getFaceNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    //for normals in other dir: switch ab and ac in cross func
    glm::vec3 trigNormal = glm::cross(ab, ac);
    return trigNormal / glm::length(trigNormal);
}
void Mesh::loadFromStringData(const string& filedata, bool generateSmoothNormals) {

    fillVertexAndIndexVectors(filedata, generateSmoothNormals);

}

//TODO?: Maybe rewrite mit sscanf
void Mesh::loadFromObj(ifstream& myfile, bool generateSmoothNormals) {

    if (myfile.is_open()) {
        std::stringstream buffer;
        buffer << myfile.rdbuf();
        std::string filedata = buffer.str();

        fillVertexAndIndexVectors(filedata, generateSmoothNormals);

        myfile.close();

    }
    else {
        cout << "Unable to open file\n";
    }



}

void Mesh::fillVertexAndIndexVectors(const string& filedata, bool generateSmoothNormals) {
    std::stringstream filestream(filedata);
    std::string line;
    vector<glm::ivec3> tempFaces = {};
    while (std::getline(filestream, line)) {
        string type = eatToSpace(&line);
        if (type != "none" && type != "#") {
            if (type == "v") {
                float x = stof(eatToSpace(&line));
                float y = stof(eatToSpace(&line));
                float z = stof(eatToSpace(&line));
                verts.emplace_back(x, y, z);
            } else if (type == "vt") {
                float u = stof(eatToSpace(&line));
                float v = stof(eatToSpace(&line));
                vertTex.emplace_back(u, v);
            } else if (type == "vn") {
                float x = stof(eatToSpace(&line));
                float y = stof(eatToSpace(&line));
                float z = stof(eatToSpace(&line));
                vertNormals.emplace_back(x, y, z);
            } else if (type == "f") {
                if (line.find('/') != string::npos) {
                    string firstTrig = eatToSpace(&line);
                    int vert = stoi(eatToDelim(&firstTrig, '/'));
                    int uv = stoi(eatToDelim(&firstTrig, '/'));
                    int norm = stoi(eatToDelim(&firstTrig, '/'));
                    indices.emplace_back(vert);
                    indices.emplace_back(uv);
                    indices.emplace_back(norm);
                    string secondTrig = eatToSpace(&line);
                    vert = stoi(eatToDelim(&secondTrig, '/'));
                    uv = stoi(eatToDelim(&secondTrig, '/'));
                    norm = stoi(eatToDelim(&secondTrig, '/'));
                    indices.emplace_back(vert);
                    indices.emplace_back(uv);
                    indices.emplace_back(norm);
                    string thirdTrig = eatToSpace(&line);
                    vert = stoi(eatToDelim(&thirdTrig, '/'));
                    uv = stoi(eatToDelim(&thirdTrig, '/'));
                    norm = stoi(eatToDelim(&thirdTrig, '/'));
                    indices.emplace_back(vert);
                    indices.emplace_back(uv);
                    indices.emplace_back(norm);
                } else {
                    if(generateSmoothNormals){
                        int vert1 = stoi(eatToSpace(&line));
                        int vert2 = stoi(eatToSpace(&line));
                        int vert3 = stoi(eatToSpace(&line));
                        tempFaces.emplace_back(vert1, vert2, vert3);
                    }
                    else {
                        int vert1 = stoi(eatToSpace(&line));
                        int vert2 = stoi(eatToSpace(&line));
                        int vert3 = stoi(eatToSpace(&line));
                        glm::vec3 a = verts[vert1 - 1];
                        glm::vec3 b = verts[vert2 - 1];
                        glm::vec3 c = verts[vert3 - 1];
                        auto normal = getFaceNormal(a,b,c);
                        vertNormals.emplace_back(normal);
                        int normalIndex = vertNormals.size();
                        //dummy uv:
                        vertTex.emplace_back(0, 0);
                        int uvIndex = vertTex.size();
                        indices.emplace_back(vert1);
                        indices.emplace_back(uvIndex);
                        indices.emplace_back(normalIndex);
                        indices.emplace_back(vert2);
                        indices.emplace_back(uvIndex);
                        indices.emplace_back(normalIndex);
                        indices.emplace_back(vert3);
                        indices.emplace_back(uvIndex);
                        indices.emplace_back(normalIndex);

                    }
                }
                //TODO: change if we load f with more values
                vertexCount +=3;
            }


        }

    }
    if(generateSmoothNormals){
        std::vector<glm::vec3> normals(verts.size(), glm::vec3(0.0f, 0.0f, 0.0f));

        for (const auto &item: tempFaces){
            int x = item.x - 1;
            int y = item.y - 1;
            int z = item.z - 1;
            glm::vec3 a = verts[x];
            glm::vec3 b = verts[y];
            glm::vec3 c = verts[z];
            glm::vec3 normal = getFaceNormal(a,b,c);

            normals[x] += normal;
            normals[y] += normal;
            normals[z] += normal;
        }

        for (auto& normal : normals) {
            normal = glm::normalize(normal);
        }

        for (const auto &item: tempFaces){
            vertTex.emplace_back(0, 0);
            int uvIndex1 = vertTex.size();

            vertTex.emplace_back(0, 0);
            int uvIndex2 = vertTex.size();

            vertTex.emplace_back(0, 0);
            int uvIndex3 = vertTex.size();

            indices.emplace_back(item.x);
            indices.emplace_back(uvIndex1);
            auto normalX = normals[item.x-1];
            vertNormals.emplace_back(normalX);
            int normalIndexX = vertNormals.size();
            indices.emplace_back(normalIndexX);

            indices.emplace_back(item.y);
            indices.emplace_back(uvIndex2);
            auto normalY = normals[item.y-1];
            vertNormals.emplace_back(normalY);
            int normalIndexY = vertNormals.size();
            indices.emplace_back(normalIndexY);

            indices.emplace_back(item.z);
            indices.emplace_back(uvIndex3);
            auto normalZ = normals[item.z-1];
            vertNormals.emplace_back(normalZ);
            int normalIndexZ = vertNormals.size();
            indices.emplace_back(normalIndexZ);

        }
    }
}

int Mesh::getIndicesCount(){
    return indices.size();
}

int Mesh::getVertexCount() const{
    return vertexCount;
}


float *Mesh::generateVertexDataFromModel() {
    const auto vertexDataPoints = getVertexCount()  * floatsPerVertex;
    auto *vertexData = new float[vertexDataPoints];

    int index = 0;
    for (int i = 0; i < indices.size(); i++) {
        //pos
        vertexData[index] = verts[indices[i] - 1].x;
        index++;
        vertexData[index] = verts[indices[i] - 1].y;
        index++;
        vertexData[index] = verts[indices[i] - 1].z;
        index++;

        i++;

        //tex
        vertexData[index] = vertTex[indices[i] - 1].x;
        index++;
        vertexData[index] = vertTex[indices[i] - 1].y;
        index++;

        i++;

        //normals
        vertexData[index] = vertNormals[indices[i] - 1].x;
        index++;
        vertexData[index] = vertNormals[indices[i] - 1].y;
        index++;
        vertexData[index] = vertNormals[indices[i] - 1].z;
        index++;



    }
    return vertexData;
}

void Mesh::init()
{
    vertexData = generateVertexDataFromModel();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* getVertexCount() * floatsPerVertex, vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // siehe Shader: „location 0“
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8,
                          (const GLvoid*)(sizeof(GLfloat)*3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*8,
                          (const GLvoid*)(sizeof(GLfloat)*5));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::transform(unsigned int prg, glm::mat4x4 look, glm::mat4x4 proj) {

    tblock.look = look;
    tblock.proj = proj;
    GLuint blockIndex = glGetUniformBlockIndex(prg, "TBlock");
    GLuint uBuf;
    glGenBuffers(1, &uBuf);
    glBindBuffer(GL_UNIFORM_BUFFER, uBuf);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(tblock), &tblock, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uBuf);
}


void Mesh::render(bool drawPolygon) const {


    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    if(drawPolygon){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glDrawArrays(GL_TRIANGLES, 0, getVertexCount());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::vec<3, float> Mesh::calcVertNormal(
        pair<multimap<int, glm::vec<3, float>>::iterator, multimap<int, glm::vec<3, float>>::iterator> range) {

    glm::vec3 sum(0.0f);

    for(auto it = range.first; it != range.second; ++it){
        sum += it->second;
    }
    int rangeSize = std::distance(range.first, range.second);

    return sum / static_cast<float>(rangeSize);

}

