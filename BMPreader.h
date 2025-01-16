#pragma once

#include <Windows.h>
#include <fstream>

class BMPreader {
private:
	tagBITMAPFILEHEADER fileHeader;
	tagBITMAPINFOHEADER infoHeader;
	tagRGBQUAD palette[256];
	uint8_t* data;
	std::ifstream inputFile;

	void openBMP(const std::string& fileName);
	void closeBMP();
public:
	BMPreader(char* fileName);
	void displayBMP();
	~BMPreader();
};

