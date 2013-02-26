#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <fstream>
#include <vector>
#include "symTable.h"


struct token
{
    std::string      lexeme;
	tok_type         type;
	std::string      name;

	token(){ type = tok_unknown; };
	token(std::string lex, std::string nm, tok_type tp = tok_unknown):lexeme(lex), type(tp), name(nm){ }
};

class Scanner
{
/* interface */
public:

	Scanner(const char* file_name);
	virtual ~Scanner()
	{
		if(prog_file.is_open())
			prog_file.close();
	}

	/*
	 * look through the whole file and extract all the lexemes and corresponding tokens.
	 * The lexemes and tokens will be stored into two vectors as the scanning procedes.
	 */
	void scan();

	/*
	 * Print all the lexemes and tokens, one pair a line.
	 */
	void print_lexeme_tokens() const;

	/*
	 * look for the next lexeme,
     * push the lexeme and the responding token to the vectors,
	 * and the next token is returned.
	 */
protected:
	token next_token();
    int         ln_num;
    //keywords_num = 21, op_num = 11, delimiters_num = 8, other_tokens_num = 4, semantic_type_num = 5, data_type_num = 4
    static const std::string tok_name[keywords_num + op_num + delimiters_num + other_tokens_num];
    static const std::string  st_name[semantic_type_num];
    static const std::string  dt_name[data_type_num];

private:
	/*
	 * data members
	 */
	std::ifstream                   prog_file;
	SymTable                        sym_table;
	std::vector<std::string>        lexemes;
	std::vector<attr_table_entry>   attrs;

	/*
     * skip comments and white spaces, find and return the next valid character
	 */
	char next_char();

	/*
     * scan the program return the next word
	 */
	std::string next_word(char c);

	/*
     * scan the program return the next number as a string type
	 */
	std::string next_number(char c);

	/*
     * scan the program return the next number as a string type
	 */
	std::string next_operator(char c);
};

#endif

