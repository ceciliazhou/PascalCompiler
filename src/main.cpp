#include <iostream>
#include "parser.h"
using namespace std;

int main(int argc, char *argv[])
{

	if(argc < 2)
	{
		cerr << "Usage: lex_test pascal_file_name.\n";
		return 1;
	}


/*
	Scanner my_scanner(argv[1]);
	my_scanner.scan();
	my_scanner.print_lexeme_tokens();
*/

    Parser my_parser(argv[1]);
    my_parser.parse();

	return 0;
}
