#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stack>
#include "scanner.h"

enum NTtype
{
	Program,                    IdList,                     _IdList_,                   Declarations,               Type,
	StandardType,               SubprogramDeclarations,     SubprogramDeclaration,      SubprogramHead,             Arguments,
	ParameterList,              _ParameterList_,            CompoundStatement,          OptionalStatements,         StatementList,
	_StatementList_,            Statement,                  RestOfStatement,            ExpressionList,             _ExpressionList_,
	Expression,                 _Expression_,               SimpleExpression,           _SimpleExpression_,         Term,
	_Term_,                     Factor,                     _Factor_,                   Sign,                       relop,
	addop,                      mulop,                      assignop
};

struct prod_item
{
	bool      isTerm;
	int       type;
};

struct prod_auxi_item
{
	int       start;
	int       len;
};

struct parse_node
{
	int       level;
	prod_item pd_item;
    parse_node(bool isT, int tp, int lv=0):level(lv){ pd_item.isTerm = isT; pd_item.type = tp; }
};

class Parser: public Scanner
{
public:
    /*
     * The pascal program file name is required to construct a parser.
     */
    Parser(const char* file_name);

    ~Parser(){}
    void parse();

private:
    /*
     * following enum types are just used as constant, specifying the size of different arrays.
     */
    enum size{ non_term_number = 33, term_number = 43, prod_number = 70 };

    /*
     * parse a nonterminal by pushing terminals/nonterminals on the righthand side of the production into the stack in reverse order.
     */
    static const std::string NTstring[non_term_number];
    static const prod_item prod_array[];
    static const prod_auxi_item prod_auxi_array[ prod_number ];
    static const int parse_table[non_term_number][term_number];

    /*
     * a stack to push and pop the nonterminals while parsing
     */
    std::stack<parse_node> parseStack;

    /*
     * parse a nonterminal by pushing terminals/nonterminals on the righthand side of the production into the stack in reverse order.
     */
    void parseNT(parse_node node, token &tok);

    /*
     * print the token type or nonterminal type of the node being parsed, (if it's a terminal, the lexeme will also be printed)
     */
    void print_parse_node(const parse_node &node, const std::string& lexeme="" );

};

#endif
