#include <iostream>

#include "packfile.hpp"

/*
 * pak [options] <directory>
 * pak [options] <vpkfile>
 * pak [options] <command> <command arguments...>
 *
 * pak h || pak ? 			- Displays help info, and arguments
 * pak <directory> 			- Create a PAK from a directory structure
 * pak a <pakfile> <file1> <file2> ...	- Add File(s)
 * pak <pakfile>			- Extracts all files from PAK
 * pak x <pakfile> <file1> <file2> ...	- Extracts file(s)
 * pak l <pakfile>			- Lists contents of PAK
 * pak L <pakfile>			- Lists detailed contents of PAK
 */
int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cout << "Invalid Arguments: Provide a filename!\n";
		return -1;
	}
	PackFile Pack(argv[1]);

	std::cout << argv[1] << " has the size of " << Pack.GetSize() << std::endl;
	return 0;
}
