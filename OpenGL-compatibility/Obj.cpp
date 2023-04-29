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
                        //TODO
                        int vert1 = stoi(eatToSpace(&line));
                        int vert2 = stoi(eatToSpace(&line));
                        int vert3 = stoi(eatToSpace(&line));
                    }
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


float *Obj::getVertexDataFromModel() {
    //FIXME: could give problems when we read in less indices
    const auto vertexDataPoints = indices.size() * ( 2 + 3);
    auto* vertexData = new float[vertexDataPoints];

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
