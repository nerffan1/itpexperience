#include "SrcMain.h"
#include <string>
#include "RleFile.h"
#include <cstring>

void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Process argc and argv, and use to compress/decompress files
	if (argc == 2)
	{
		RleFile rleFile;
		bool compOrDecomp;

		bool one = argv[1][strlen(argv[1]) - 1] == '1' ;
		bool l = argv[1][strlen(argv[1]) - 2] == 'l' ;
		bool r = argv[1][strlen(argv[1]) - 3] == 'r' ;

		if (one && l && r)
		{
			rleFile.ExtractArchive(argv[1]);
		}
		else
		{
			rleFile.CreateArchive(argv[1]);
		}
	}
}
