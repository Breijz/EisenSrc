#pragma once

#include <iostream>
#include <map>

struct PackHeader_t {
	char magic[4];
	int offset;
	int size;
};

struct PackEntry_t {
	char filename[56];
	int offset;
	int size;
};

class PackFile {
	PackHeader_t Header;
	std::map<PackEntry_t*, void*> PackEntries;
public:
	PackFile(char* filename);
	~PackFile();

	void* ExtractFile(char* filename);
	int AppendFile(char* filename);
	int GetSize(char* filename = nullptr);
};
