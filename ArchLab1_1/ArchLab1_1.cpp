// ArchLab1_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <windows.h>
#include "BmpImage.h"

using namespace std;

int main() {
	//ifstream file("D:\\Others\\Учеба\\year_2\\architecture\\part_2\\labs\\lab_1\\image.bmp", ios::binary);
	ifstream file("C:\\Users\\serge\\Downloads\\oribmp24.bmp", ios::binary);
	//ofstream outFile("testimg.bmp", ios::binary);
	ofstream outFile2("testimg16bit.bmp", ios::binary);
	ofstream outFile3("testimg24bit.bmp", ios::binary);
	BmpImage image;
	cout << sizeof(DWORD);
	if (!file) {
		return 0;
	}
	image.loadImage(file);
	file.close();
	//image.saveImage(outFile);
	BmpImage image2 = image.image24_to_16();
	image2.saveImage(outFile2);

	ifstream file3("testimg16bit.bmp", ios::binary);
	BmpImage image16bit;
	image16bit.loadImage(file3);
	BmpImage image24bit = image16bit.image16_to_24();
	image24bit.saveImage(outFile3);
	return 0;
}

