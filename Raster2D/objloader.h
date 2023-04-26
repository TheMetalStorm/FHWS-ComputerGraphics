//
// Created by arapo on 26.04.2023.
//

#ifndef FH_CG_OBJLOADER_H
#define FH_CG_OBJLOADER_H

#include "simple.h"
#include <iostream>
#include <fstream>
#include <string>
string eatToDelim(string *line, char c);
string eatToSpace(string *line);

using namespace std;

Obj loadFile(){
    Obj res = {};
    string line;
    ifstream myfile(R"(C:\Users\arapo\CLionProjects\FHWS-ComputerGraphics\Wavefront Datasets CG\datasets\cube.obj)");


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
                    int x0 = stoi(eatToDelim(&firstTrig, '/'));
                    int y0 = stoi(eatToDelim(&firstTrig, '/'));
                    int z0 = stoi(eatToDelim(&firstTrig, '/'));
                    string secondTrig = eatToSpace(&line);
                    int x1 = stoi(eatToDelim(&secondTrig, '/'));
                    int y1 = stoi(eatToDelim(&secondTrig, '/'));
                    int z1 = stoi(eatToDelim(&secondTrig, '/'));
                    string thirdTrig = eatToSpace(&line);
                    int x2 = stoi(eatToDelim(&thirdTrig, '/'));
                    int y2 = stoi(eatToDelim(&thirdTrig, '/'));
                    int z2 = stoi(eatToDelim(&thirdTrig, '/'));
                    res.trigs.emplace_back(vector<Vector3i>{{x0, y0, z0}, {x1, y1, z1}, {x2, y2, z2}});
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
