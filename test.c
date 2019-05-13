#include "pch.h"
#include <iostream>
#include <windows.h>
#include <direct.h>
#include <string>

using namespace std;

void convert(string fname, string path);

int main()
{
	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	TCHAR tempPath[255];

	GetCurrentDirectory(255, tempPath);

	string path = tempPath;

	path += "\\*.2dx";

	hFind = FindFirstFile((LPCSTR)path.c_str(), &FindData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do convert(FindData.cFileName, tempPath);
		while (FindNextFile(hFind, &FindData));
	}
	else {
		cout << "No such File" << endl;
		system("pause");
		FindClose(hFind);
		return 0;
	}
	cout << "Done." << endl;
	system("pause");
	FindClose(hFind);
	return 0;
}

void convert(string fname, string path) {
	char buffer[5] = { 0 };
	int length = 0;

	string outName = fname;
	outName = outName.replace(outName.length() - 3, 3, "wav");
	string fullOutName = path + "\\WAV_converted";
	_mkdir(fullOutName.c_str());
	fullOutName += "\\" + outName;

	FILE *input, *output;
	input = fopen(fname.c_str(), "rb+");

	while (!feof(input)) {
		if (FILE *tmp = fopen(fullOutName.c_str(), "r")) {
			fclose(tmp);
			break;
		}
		while (fread(buffer, 4, 1, input) == 1) {
			if (strcmp("RIFF", buffer) == 0) {
				fread(&length, 4, 1, input);
				char* mem = (char *)malloc(length);
				fread(mem, length, 1, input);
				output = fopen(fullOutName.c_str(), "wb+");
				fwrite("RIFF", 4, 1, output);
				fwrite(&length, 4, 1, output);
				fwrite(mem, length, 1, output);
				cout << outName << " saved. size : " << length << endl;
				fclose(output);
				free(mem);
				break;
			}
		}
	}
	fclose(input);
}