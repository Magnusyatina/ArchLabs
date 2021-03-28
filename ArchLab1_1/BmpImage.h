#pragma once
#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1)
struct MYBITMAPFILEHEADER {
	unsigned short int bfType = 0;
	unsigned long int bfSize = 0;
	unsigned short int bfReserved1 = 0;
	unsigned short int bfReserved2 = 0;
	unsigned long int bfOffBits = 0;
};

struct MYBITMAPINFOHEADER {
	unsigned long int biSize = 0;
	unsigned long int biWidth = 0;
	unsigned long int biHeight = 0;
	unsigned short int biPlanes = 0;
	unsigned short int biBitCount = 0;
	unsigned long int biCompression = 0;
	unsigned long int biSizeImage = 0;
	unsigned long int biXPelsPerMeter = 0;
	unsigned long int biYPelsPerMeter = 0;
	unsigned long int biClrUsed = 0;
	unsigned long int biClrImportant = 0;
};

struct MYRGBQUADS {
	unsigned char rgbRed;
	unsigned char rgbGreen;
	unsigned char rgbBlue;
	unsigned char rgbReserved;
};
#pragma pack(pop)


class BmpImage {
private:
	MYBITMAPFILEHEADER fileHeader;
	MYBITMAPINFOHEADER infoHeader;
	MYRGBQUADS* rgbquads = nullptr;
	char** pixArray1 = nullptr;

public:
	void loadImage(std::ifstream& file) {
		std::cout << sizeof(MYBITMAPFILEHEADER) << std::endl;
		std::cout << sizeof(MYBITMAPINFOHEADER) << std::endl;

		file.read((char*)&fileHeader, sizeof(MYBITMAPFILEHEADER));
		file.read((char*)&infoHeader, sizeof(MYBITMAPINFOHEADER));
		std::cout << "Read bytes: " << file.tellg() << std::endl;
		int offset = ((infoHeader.biWidth * 3) % 4);
		int width = (infoHeader.biWidth + offset) * (infoHeader.biBitCount / 8);
		int height = infoHeader.biHeight;

		pixArray1 = new char* [height];
		for (int i = 0; i < height; i++) {
			*(pixArray1 + i) = new char[width];
			file.read(*(pixArray1 + i), width);
		}

	}

	void saveImage(std::ofstream& file) {
		file.write((char*)&fileHeader, sizeof(fileHeader));
		file.write((char*)&infoHeader, sizeof(infoHeader));

		int offset = ((infoHeader.biWidth * 3) % 4);
		int width = (infoHeader.biWidth + offset) * (infoHeader.biBitCount / 8);;
		int height = infoHeader.biHeight;

		for (int i = 0; i < height; i++) {
			file.write(*(pixArray1 + i), width);
		}
	}

	BmpImage image24_to_16() {
		BmpImage bmpImage;
		bmpImage.fileHeader = this->fileHeader;
		bmpImage.infoHeader = this->infoHeader;
		bmpImage.infoHeader.biBitCount = 16;
		int offset = ((bmpImage.infoHeader.biWidth * 3) % 4);
		int byteCountInRow = (bmpImage.infoHeader.biWidth + offset) * (bmpImage.infoHeader.biBitCount / 8);
		int rowCount = bmpImage.infoHeader.biHeight;
		bmpImage.fileHeader.bfSize = bmpImage.fileHeader.bfOffBits + (byteCountInRow * rowCount);

		char** nPixArray = new char* [rowCount];
		for (int i = 0; i < rowCount; i++) {
			*(nPixArray + i) = new char[byteCountInRow];
			for (int j = 0; j < bmpImage.infoHeader.biWidth + offset; j++) {
				char* srcBytes = *(pixArray1 + i) + j * 3;
				char* destBytes = *(nPixArray + i) + j * 2;
				destBytes[0] = destBytes[0] | ((srcBytes[0] >> 3) << 3);
				destBytes[0] = destBytes[0] | (srcBytes[1] >> 5);
				destBytes[1] = destBytes[1] | ((srcBytes[1] >> 3) << 6);
				destBytes[1] = destBytes[1] | (srcBytes[2] >> 2);
			}
		}
		bmpImage.pixArray1 = nPixArray;

		return bmpImage;
	}

	BmpImage image16_to_24() {
		BmpImage bmpImage;
		bmpImage.fileHeader = this->fileHeader;
		bmpImage.infoHeader = this->infoHeader;
		bmpImage.infoHeader.biBitCount = 24;
		int offset = ((bmpImage.infoHeader.biWidth * 3) % 4);
		int byteCountInRow = (bmpImage.infoHeader.biWidth + offset) * (bmpImage.infoHeader.biBitCount / 8);
		int rowCount = bmpImage.infoHeader.biHeight;
		bmpImage.fileHeader.bfSize = bmpImage.fileHeader.bfOffBits + (byteCountInRow * rowCount);

		char** nPixArray = new char* [rowCount];
		for (int i = 0; i < rowCount; i++) {
			*(nPixArray + i) = new char[byteCountInRow];
			for (int j = 0; j < bmpImage.infoHeader.biWidth + offset; j++) {
				char* srcBytes = *(pixArray1 + i) + j * 2;
				char* destBytes = *(nPixArray + i) + j * 3;
				destBytes[0] = destBytes[0] | ((srcBytes[0] >> 3) << 3);
				destBytes[1] = destBytes[1] | (srcBytes[0] << 5);
				destBytes[1] = destBytes[1] | ((srcBytes[1] >> 6) << 3);
				destBytes[2] = destBytes[2] | (srcBytes[1] << 2);
			}
		}
		bmpImage.pixArray1 = nPixArray;

		return bmpImage;
	}

	friend std::ostream& operator<<(std::ostream& stream, BmpImage image);
};

