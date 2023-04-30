//
// Created by arapo on 29.04.2023.
//
#include "Obj.h"

string Obj::eatToDelim(string *line, char c) {

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

string Obj::eatToSpace(string *line) {
    return eatToDelim(line, ' ');
}

//TODO?: Maybe rewrite mit sscanf
void Obj::loadFile(string filename) {
    string line;
    ifstream myfile(filename);


    if (myfile.is_open()) {
        while (getline(myfile, line)) {
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
                        int vert1 = stoi(eatToSpace(&line));
                        int vert2 = stoi(eatToSpace(&line));
                        int vert3 = stoi(eatToSpace(&line));

                        glm::vec3 a = verts[vert1-1];
                        glm::vec3 b = verts[vert2-1];
                        glm::vec3 c = verts[vert3-1];

                        glm::vec3 ab = b-a;
                        glm::vec3 ac = c-a;

                        //for normals in other dir: switch ab and ac in cross func
                        glm::vec3 trigNormal = glm::cross(ab,ac);
                        vertNormals.emplace_back(trigNormal / glm::length(trigNormal));
                        int normalIndex = vertNormals.size();

                        //dummy uv:
                        vertTex.emplace_back(0,0);
                        int uvIndex = vertNormals.size();

                        indices.emplace_back(vert1);
                        indices.emplace_back(normalIndex);
                        indices.emplace_back(uvIndex);

                        indices.emplace_back(vert2);
                        indices.emplace_back(normalIndex);
                        indices.emplace_back(uvIndex);

                        indices.emplace_back(vert3);
                        indices.emplace_back(normalIndex);
                        indices.emplace_back(uvIndex);
                    }
                    //TODO: change if we laod f with more values
                    vertexCount+=3;
                }

            }

        }
        myfile.close();
    } else {
        cout << "Unable to open file\n";
    }
}

int Obj::getIndicesCount(){
    return indices.size();
}

int Obj::getVertexCount() const{
    return vertexCount;
}


float *Obj::generateVertexDataFromModel() {
    const auto vertexDataPoints = getVertexCount()  * (3 + 2 + 3);
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

void Obj::init()
{
    vertexData = generateVertexDataFromModel();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)* getVertexCount() * (3+ 2 + 3), vertexData, GL_STATIC_DRAW);

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
    glFrontFace(GL_CCW);
}
void Obj::render(bool drawPolygon) {


    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnable(GL_CULL_FACE);

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
