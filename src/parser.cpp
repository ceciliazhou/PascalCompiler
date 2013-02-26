#include "parser.h"
#include <iostream>
using namespace std;

const std::string Parser::NTstring[] =
{
	"Program",                   "IdList",                    "_IdList_",                  "Declarations",              "Type",
	"StandardType",              "SubprogramDeclarations",    "SubprogramDeclaration",     "SubprogramHead",            "Arguments",
	"ParameterList",             "_ParameterList_",           "CompoundStatement",         "OptionalStatements",        "StatementList",
	"_StatementList_",           "Statement",                 "RestOfStatement",           "ExpressionList",            "_ExpressionList_",
	"Expression",                "_Expression_",              "SimpleExpression",          "_SimpleExpression_",        "Term",
	"_Term_",                    "Factor",                    "_Factor_",                  "Sign",                      "relop",
	"addop",                     "mulop",                     "assignop"
};

const prod_item Parser::prod_array[] =
{
	/* Program ::= program id ( IdList ) ; Declarations  SubprogramDeclarations CompoundStatement . */
	{true,  tok_program},               {true,  tok_id},                    {true,  tok_op_prnth},
	{false, IdList},                    {true,  tok_cl_prnth},              {true,  tok_semicolon},
	{false, Declarations},              {false, SubprogramDeclarations},    {false, CompoundStatement},
	{true,  tok_period},

	/* IdList ::= id _IdList_ */
	{true,  tok_id},                    {false, _IdList_},

	/* _IdList_ ::=  , id _IdList_ */
	{true,  tok_comma},                 {true,  tok_id},                    {false, _IdList_},


	/* _IdList_ ::=  <nil> */


	/* Declarations ::= var IdList : Type ; Declarations */
	{true,  tok_var},                   {false, IdList},                    {true,  tok_colon},
	{false, Type},                      {true,  tok_semicolon},             {false, Declarations},


	/* Declarations ::= <nil> */


	/* Type ::= StandardType */
	{false, StandardType},

	/* Type ::= array [ num . . num ] of StandardType */
	{true,  tok_array},                 {true,  tok_op_brckt},              {true,  tok_number},
	{true,  tok_period},                {true,  tok_period},                {true,  tok_number},
	{true,  tok_cl_brckt},              {true,  tok_of},                    {false, StandardType},


	/* StandardType ::= integer */
	{true,  tok_integer},

	/* StandardType ::= real */
	{true,  tok_real},

	/* SubprogramDeclarations ::= SubprogramDeclaration ; SubprogramDeclarations */
	{false, SubprogramDeclaration},     {true,  tok_semicolon},             {false, SubprogramDeclarations},


	/* SubprogramDeclarations ::= <nil> */


	/* SubprogramDeclaration ::= SubprogramHead Declarations CompoundStatement */
	{false, SubprogramHead},            {false, Declarations},              {false, CompoundStatement},


	/* SubprogramHead ::= function id Arguments : StandardType ; */
	{true,  tok_function},              {true,  tok_id},                    {false, Arguments},
	{true,  tok_colon},                 {false, StandardType},              {true,  tok_semicolon},


	/* SubprogramHead ::=  procedure id Arguments ; */
	{true,  tok_procedure},             {true,  tok_id},                    {false, Arguments},
	{true,  tok_semicolon},

	/* Arguments ::= ( ParameterList ) */
	{true,  tok_op_prnth},              {false, ParameterList},             {true,  tok_cl_prnth},


	/* Arguments ::= <nil> */


	/* ParameterList ::= IdList : Type _ParameterList_ */
	{false, IdList},                    {true,  tok_colon},                 {false, Type},
	{false, _ParameterList_},

	/* _ParameterList_ ::=  ; IdList : Type _ParameterList_ */
	{true,  tok_semicolon},             {false, IdList},                    {true,  tok_colon},
	{false, Type},                      {false, _ParameterList_},

	/* _ParameterList_ ::= <nil> */


	/* CompoundStatement ::= begin OptionalStatements end */
	{true,  tok_begin},                 {false, OptionalStatements},        {true,  tok_end},


	/* OptionalStatements ::= StatementList */
	{false, StatementList},

	/* OptionalStatements ::= <nil> */


	/* StatementList ::= Statement _StatementList_ */
	{false, Statement},                 {false, _StatementList_},

	/* _StatementList_ ::= ; Statement _StatementList_ */
	{true,  tok_semicolon},             {false, Statement},                 {false, _StatementList_},


	/* _StatementList_ ::= <nil> */


	/* Statement ::= id RestOfStatement */
	{true,  tok_id},                    {false, RestOfStatement},

	/* Statement ::= CompoundStatement */
	{false, CompoundStatement},

	/* Statement ::= if Expression then Statement else Statement */
	{true,  tok_if},                    {false, Expression},                {true,  tok_then},
	{false, Statement},                 {true,  tok_else},                  {false, Statement},


	/* Statement ::= while Expression do Statement */
	{true,  tok_while},                 {false, Expression},                {true,  tok_do},
	{false, Statement},

	/* RestOfStatement ::=  ( ExpressionList ) */
	{true,  tok_op_prnth},              {false, ExpressionList},            {true,  tok_cl_prnth},


	/* RestOfStatement ::=  [ ExpressionList ] assignop Expression */
	{true,  tok_op_brckt},              {false, ExpressionList},            {true,  tok_cl_brckt},
	{false, assignop},                  {false, Expression},

	/* RestOfStatement ::=  assignop Expression */
	{false, assignop},                  {false, Expression},

	/* RestOfStatement ::=  <nil> */


	/* ExpressionList ::= Expression _ExpressionList_ */
	{false, Expression},                {false, _ExpressionList_},

	/* _ExpressionList_ ::= , Expression _ExpressionList_ */
	{true,  tok_comma},                 {false, Expression},                {false, _ExpressionList_},


	/* _ExpressionList_ ::= <nil> */


	/* Expression ::= SimpleExpression _Expression_ */
	{false, SimpleExpression},          {false, _Expression_},

	/* _Expression_ ::= <nil> */


	/* _Expression_ ::= relop SimpleExpression */
	{false, relop},                     {false, SimpleExpression},

	/* SimpleExpression ::= Term _SimpleExpression_ */
	{false, Term},                      {false, _SimpleExpression_},

	/* SimpleExpression ::= Sign Term _SimpleExpression_ */
	{false, Sign},                      {false, Term},                      {false, _SimpleExpression_},


	/* _SimpleExpression_ ::= addop Term _SimpleExpression_ */
	{false, addop},                     {false, Term},                      {false, _SimpleExpression_},


	/* _SimpleExpression_ ::= <nil> */


	/* Term ::= Factor _Term_ */
	{false, Factor},                    {false, _Term_},

	/* _Term_ ::= mulop Factor _Term_ */
	{false, mulop},                     {false, Factor},                    {false, _Term_},


	/* _Term_ ::= <nil> */


	/* Factor ::= id _Factor_ */
	{true,  tok_id},                    {false, _Factor_},

	/* Factor ::= num */
	{true,  tok_number},

	/* Factor ::= ( Expression ) */
	{true,  tok_op_prnth},              {false, Expression},                {true,  tok_cl_prnth},


	/* Factor ::= not Factor */
	{true,  tok_not},                   {false, Factor},

	/* _Factor_ ::= <nil> */


	/* _Factor_ ::= ( ExpressionList ) */
	{true,  tok_op_prnth},              {false, ExpressionList},            {true,  tok_cl_prnth},


	/* Sign ::= + */
	{true,  tok_add},

	/* Sign ::= - */
	{true,  tok_subtract},

	/* relop ::= = */
	{true,  tok_equal},

	/* relop ::= > */
	{true,  tok_gt},

	/* relop ::= < */
	{true,  tok_lt},

	/* relop ::= >= */
	{true,  tok_ge},

	/* relop ::= <= */
	{true,  tok_le},

	/* relop ::= <> */
	{true,  tok_ne},

	/* addop ::= + */
	{true,  tok_add},

	/* addop ::= - */
	{true,  tok_subtract},

	/* addop ::= OR */
	{true,  tok_or},

	/* mulop ::= * */
	{true,  tok_mul},

	/* mulop ::= / */
	{true,  tok_div},

	/* mulop ::= DIV */
	{true,  tok_int_div},

	/* mulop ::= MOD */
	{true,  tok_mod},

	/* mulop ::= AND */
	{true,  tok_and},

	/* assignop ::= := */
	{true,  tok_assign}

};

const prod_auxi_item Parser::prod_auxi_array[prod_number] =
{
	{0,   10},  {10,  2},   {12,  3},   {15,  0},   {15,  6},   {21,  0},   {21,  1},   {22,  9},
	{31,  1},   {32,  1},   {33,  3},   {36,  0},   {36,  3},   {39,  6},   {45,  4},   {49,  3},
	{52,  0},   {52,  4},   {56,  5},   {61,  0},   {61,  3},   {64,  1},   {65,  0},   {65,  2},
	{67,  3},   {70,  0},   {70,  2},   {72,  1},   {73,  6},   {79,  4},   {83,  3},   {86,  5},
	{91,  2},   {93,  0},   {93,  2},   {95,  3},   {98,  0},   {98,  2},   {100, 0},   {100, 2},
	{102, 2},   {104, 3},   {107, 3},   {110, 0},   {110, 2},   {112, 3},   {115, 0},   {115, 2},
	{117, 1},   {118, 3},   {121, 2},   {123, 0},   {123, 3},   {126, 1},   {127, 1},   {128, 1},
	{129, 1},   {130, 1},   {131, 1},   {132, 1},   {133, 1},   {134, 1},   {135, 1},   {136, 1},
	{137, 1},   {138, 1},   {139, 1},   {140, 1},   {141, 1},   {142, 1}
};

const int Parser::parse_table[non_term_number][term_number] =
{
	{0,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   2,  -1,  -1,  -1,   3,  -1,  -1,   3,  -1,  -1,  -1},
	{-1,   4,  -1,  -1,  -1,   5,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,   6,   6,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   7,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,   8,   9,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  10,  11,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  10,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  12,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  12,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  14,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  13,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  16,  -1,  15,  -1,  -1,  -1,  16,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  17,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  18,  -1,  -1,  19,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  20,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  21,  22,  21,  -1,  21,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  21,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  23,  -1,  23,  -1,  23,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  23,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  25,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  24,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  27,  -1,  28,  -1,  29,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  26,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  33,  -1,  -1,  -1,  -1,  33,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  32,  -1,  33,  -1,  30,  -1,  31,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  34,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  34,  34,  -1,  -1,  -1,  -1,  -1,  -1,  34,  -1,  -1,  -1,  -1,  34,  34,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  35,  -1,  -1,  -1,  36,  -1,  36,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  37,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  37,  37,  -1,  -1,  -1,  -1,  -1,  -1,  37,  -1,  -1,  -1,  -1,  37,  37,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  38,  -1,  38,  -1,  38,  38,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  39,  39,  39,  39,  39,  39,  -1,  -1,  -1,  -1,  -1,  38,  38,  -1,  -1,  38,  -1,  38,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  40,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  41,  41,  -1,  -1,  -1,  -1,  -1,  -1,  40,  -1,  -1,  -1,  -1,  40,  40,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  43,  -1,  43,  -1,  43,  43,  -1,  -1,  -1,  -1,  42,  -1,  -1,  -1,  -1,  43,  43,  43,  43,  43,  43,  42,  42,  -1,  -1,  -1,  43,  43,  -1,  -1,  43,  -1,  43,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  44,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  44,  -1,  -1,  -1,  -1,  44,  44,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  46,  -1,  46,  -1,  46,  46,  -1,  45,  45,  45,  46,  -1,  -1,  -1,  -1,  46,  46,  46,  46,  46,  46,  46,  46,  45,  45,  -1,  46,  46,  -1,  -1,  46,  -1,  46,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  50,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  49,  -1,  -1,  -1,  -1,  47,  48,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  51,  -1,  51,  -1,  51,  51,  -1,  51,  51,  51,  51,  -1,  -1,  -1,  -1,  51,  51,  51,  51,  51,  51,  51,  51,  51,  51,  -1,  51,  51,  -1,  52,  51,  -1,  51,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  53,  54,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  55,  60,  57,  59,  56,  58,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  63,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  61,  62,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  66,  67,  68,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  64,  65,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
	{-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  69,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}
};

Parser::Parser(const char* file_name):Scanner(file_name)
{

}

/*
 *   Place the start symbol in a node and push it onto the stack.
 *   Fetch a token
 *   REPEAT
 *       Pop a node from the stack
 *       IF it contains a terminal, match it to the current token (no 	match indicates a parsing error) and fetch another token
 *       ELSE IF it contains a nonterminal, look it up in the production table using the nontermina and the current token.  Place the variables in REVERSE order on the stack
 *   UNTIL the stack is empty
 */
void Parser::parse()
{
	parse_node root(false, Program, 1);
	parseStack.push(root);

    token cur_tok = next_token();
	while(!parseStack.empty())
	{
		parse_node cur_node = parseStack.top();
		parseStack.pop();

        print_parse_node(cur_node, cur_tok.lexeme);

		if (cur_node.pd_item.isTerm)
        {
            // raise error if the terminal got from the file doesn't match the one required in the parse table.
            if(cur_tok.type != cur_node.pd_item.type)
                cerr << "\nLine " << ln_num << ": "
                     << "Unexpected terminal: " << tok_name[cur_tok.type] << ": \"" << cur_tok.lexeme
                     << "\" is given, but " << tok_name[cur_node.pd_item.type] << " is expected."
                     << endl << endl;

            cur_tok = next_token();
        }
        else
            parseNT(cur_node, cur_tok);
	}
}

/*
 * look up in the parse table using the nontermina and the current token.  Place the variables in REVERSE order on the stack
 */
void Parser::parseNT(parse_node node, token &tok)
{
    int level = node.level + 1;

    int pnum = parse_table[node.pd_item.type][tok.type];
	if( pnum == -1)
	{
        cerr << "\nLine " << ln_num << ": "
             << "Mismatch nonterminal-token: " << NTstring[node.pd_item.type] << " and "<< tok_name[tok.type]
             << endl << endl;
	}

	if (prod_auxi_array[pnum].len != 0)
	{
		int last = prod_auxi_array[pnum].start + prod_auxi_array[pnum].len - 1;


		for( int ndx = last; ndx >= prod_auxi_array[pnum].start; ndx-- )
        {
            parse_node sub_node(prod_array[ndx].isTerm, prod_array[ndx].type, level);
            parseStack.push(sub_node);
		}
	}

}


void Parser::print_parse_node(const parse_node &node, const string& lexeme)
{
    for (int i = 0;  i < node.level;  i++)
		cout << "   ";

	cout << node.level << "    ";
	if(node.pd_item.isTerm)
        cout << tok_name[node.pd_item.type] << " --> \"" << lexeme << "\"" << endl;
    else
        cout << NTstring[node.pd_item.type] << endl;


}

