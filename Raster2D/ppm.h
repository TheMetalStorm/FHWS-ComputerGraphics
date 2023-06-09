#pragma once
#include <iostream>
#include <fstream>
#include "simple.h"

const int maxColorVal = 65535;

//Todo fix
void framebufferToPPM(const RGBPixel framebuffer[RESOLUTION][RESOLUTION], std::string filename) {

	ofstream out(filename + ".ppm");

	out << "P3" << endl;
	out << RESOLUTION << " " << RESOLUTION << endl;
	out << maxColorVal << endl;

	for (int y = RESOLUTION - 1; y >= 0; y--) {
		for (int x = 0; x < RESOLUTION; x++) {
			const RGBPixel current = framebuffer[x][y];
			out << current.x() * maxColorVal << " " << current.y() * maxColorVal << " " << current.z() * maxColorVal << " ";
		}
		out << endl;
	}

	out.close();
}
