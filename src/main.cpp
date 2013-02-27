#include <iostream>
#include "parser.h"
using namespace std;

int main(int argc, char *argv[])
{

	if(argc < 2)
	{
		cerr << "Usage: " << argv[0] << " pascal_file_name.\n";
		return 1;
	}


    Parser my_parser(argv[1]);
    my_parser.parse();

	return 0;
}
