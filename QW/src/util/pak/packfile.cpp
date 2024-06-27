#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <new>

#include "packfile.hpp"


PackFile::PackFile(char* pakname) {
	FILE* fh = fopen(pakname, "r");
	fread(&Header, sizeof(PackHeader_t), 1, fh);
	if(strncmp(Header.magic, "PACK", 4)) {
		fclose(fh);
		return;
	}
	if(fseek(fh, Header.offset, SEEK_SET) != 0) {
		std::cout << "Failed to seek to start of file table!\n";
	}
	long orig_offset = 0;
	while(SEEK_CUR != SEEK_END) {
		PackEntry_t* Entry = new PackEntry_t;
		fread(&Entry->filename, 56, 1, fh);
		fread(&Entry->offset, 4, 1, fh);
		fread(&Entry->size, 4, 1, fh);
		std::cout << "Entry name is " << Entry->filename << "\n";
		std::cout << "Entry size is " << Entry->size << "\n";
		std::cout << "Entry offset is " << Entry->offset << "\n";
		//void* filecontents = malloc(Entry->size);
		void* filecontents = new void*[Entry->size];
		if(!filecontents) { std::cout << "Failed to allocate!\n"; }
		orig_offset = ftell(fh);
		fseek(fh, Entry->offset, 0);
		fread(&filecontents, Entry->size, 1, fh);
		//PackEntries.insert(std::make_pair(*Entry, *filecontents));
		PackEntries.insert(std::pair<PackEntry_t*, void*>(Entry, filecontents));
		fseek(fh, orig_offset + 1, 0);
	}
	fclose(fh);
}

PackFile::~PackFile() {
}

void* PackFile::ExtractFile(char* filename) {
}

int PackFile::AppendFile(char* filename) {
}

int PackFile::GetSize(char* filename) {
	if(!filename) {
		return Header.offset;
	} else {
		for(const auto& Entry : PackEntries) {
			if(!strcmp(filename, Entry.first->filename)) {
				return Entry.first->size;
			}
		}
		return -1;
	}
}
