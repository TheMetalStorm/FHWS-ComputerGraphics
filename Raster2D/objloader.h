//
// Created by arapo on 26.04.2023.
//

#ifndef FH_CG_OBJLOADER_H
#define FH_CG_OBJLOADER_H

#include "simple.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string eatToDelim(string *line, char c);
string eatToSpace(string *line);

Obj loadFile(string filename){
    Obj res = {};
    string line;
    ifstream myfile(filename);


    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            string type = eatToSpace(&line);
            if(type != "none" && type != "#"){
                if(type == "v"){
                    float x = stof(eatToSpace(&line));
                    float y = stof(eatToSpace(&line));
                    float z = stof(eatToSpace(&line));
                    res.verts.emplace_back(x, y, z);
                }
                else if(type == "vt"){
                    float u = stof(eatToSpace(&line));
                    float v = stof(eatToSpace(&line));
                    cout << u << " " << v << endl;
                    res.vertTex.emplace_back(u, v);
                }
                else if(type == "vn"){
                    float x = stof(eatToSpace(&line));
                    float y = stof(eatToSpace(&line));
                    float z = stof(eatToSpace(&line));
                    res.vertNormals.emplace_back(x, y, z);
                }
                else if(type == "f"){
                    string firstTrig = eatToSpace(&line);
                    int vert = stoi(eatToDelim(&firstTrig, '/'));
                    int uv = stoi(eatToDelim(&firstTrig, '/'));
                    int norm = stoi(eatToDelim(&firstTrig, '/'));
                    res.vertexIndices.emplace_back(vert);
                    res.uvIndices.emplace_back(uv);
                    res.normalIndices.emplace_back(norm);
                    string secondTrig = eatToSpace(&line);
                    vert = stoi(eatToDelim(&secondTrig, '/'));
                    uv = stoi(eatToDelim(&secondTrig, '/'));
                    norm = stoi(eatToDelim(&secondTrig, '/'));
                    res.vertexIndices.emplace_back(vert);
                    res.uvIndices.emplace_back(uv);
                    res.normalIndices.emplace_back(norm);
                    string thirdTrig = eatToSpace(&line);
                    vert = stoi(eatToDelim(&thirdTrig, '/'));
                    uv = stoi(eatToDelim(&thirdTrig, '/'));
                    norm = stoi(eatToDelim(&thirdTrig, '/'));
                    res.vertexIndices.emplace_back(vert);
                    res.uvIndices.emplace_back(uv);
                    res.normalIndices.emplace_back(norm);
                }

            }

        }
        myfile.close();
    } else {
        cout << "Unable to open file\n";
    }

    return res;
}


string eatToDelim(string *line, char c) {

    if(line->empty()) return "none";

    for (int i = 0; i<line->size();i++){
        if(line->at(i) == c){
            string res = line->substr(0, i);
            line->erase(0, i+1);
            return res;
        }
        else if(i == line->size()-1){
            return *line;
        }
    }

}

string eatToSpace(string *line) {
    return eatToDelim(line, ' ');
}
#endif //FH_CG_OBJLOADER_H
