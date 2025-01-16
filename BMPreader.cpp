#include <iostream>

#include "BMPreader.h"

void BMPreader::openBMP(const std::string& fileName) {
	inputFile.open(fileName, std::ios::binary);

	if (!inputFile) {
		throw std::runtime_error("File not found");
	}
	if (!inputFile.read((char*)&fileHeader, sizeof(fileHeader))) {
		throw std::runtime_error("Can't read fileHeader");
	}
	if (fileHeader.bfType != 19778) {
		throw std::runtime_error("Incorrect type of file");
	}
	if (!inputFile.read((char*)&infoHeader, sizeof(infoHeader))) {
		throw std::runtime_error("Can't read infoHeader");
	}
	if (fileHeader.bfOffBits > sizeof(fileHeader) + sizeof(infoHeader)) {
		if (!inputFile.read((char*)&palette, fileHeader.bfOffBits - (sizeof(fileHeader) + sizeof(infoHeader)))) {
			throw std::runtime_error("Can't read palette");
		}
	}

	uint32_t dataSize = infoHeader.biWidth * infoHeader.biHeight * infoHeader.biBitCount / 8;

	data = new uint8_t[dataSize];
	if (infoHeader.biBitCount == 32 || infoHeader.biWidth % 4 == 0) {
		if (!inputFile.read((char*)data, dataSize)) {
			throw std::runtime_error("Can't read data of pixels");
		}
	}
	else {
		uint32_t rowSize = infoHeader.biWidth * infoHeader.biBitCount / 8;
		uint8_t extraBytesSize = 4 - ((rowSize) % 4);
		uint8_t extraBytes[4];
		for (int i = 0; i < infoHeader.biHeight; ++i) {
			if (!(inputFile.read((char*)(data + rowSize * i), rowSize) && inputFile.read((char*)extraBytes, extraBytesSize))) {
				throw std::runtime_error("Can't read data of pixels");
			}
		}
	}
}
void BMPreader::closeBMP() {
	inputFile.close();
	delete[] data;
	data = nullptr;
}
BMPreader::BMPreader(char* fileName) {
	data = nullptr;
	std::string name(fileName);
	openBMP(name);
}
void BMPreader::displayBMP() {
	if (!data) {
		throw std::runtime_error("Data is empty");
	}

	uint8_t pixelSize = infoHeader.biBitCount / 8;
	for (int i = infoHeader.biHeight - 1; i >= 0; --i) {
		uint32_t offset = i * infoHeader.biWidth * pixelSize;
		for (int j = 0; j < infoHeader.biWidth * pixelSize; j += pixelSize) {
			if (data[offset + j] && data[offset + j + 1] && data[offset + j + 2] == 255) {
				std::cout << 'W';
			}
			else if (!(data[offset + j] || data[offset + j + 1] || data[offset + j + 2])) {
				std::cout << 'B';
			}
			else {
				throw std::runtime_error("Picture contain other colors");
			}
		}
		std::cout << '\n';
	}
}
BMPreader::~BMPreader() {
	closeBMP();
}

