#include <iostream>
#include <iomanip> // for setw
#include <cctype>  // for isalpha, toupper, etc
#include "scanner.h"
using namespace std;

const std::string Scanner::tok_name[] = { /* --------------keywords-------------- */
                            "tok_program",      "tok_var",          "tok_integer",      "tok_real",         "tok_procedure",
                            "tok_begin",        "tok_end",          "tok_if",           "tok_then",         "tok_while",
                            "tok_do",           "tok_else",         "tok_not",          "tok_int_div",      "tok_mod",
                            "tok_and",          "tok_or",           "tok_const",        "tok_array",        "tok_function",
                            "tok_of",

                            /* --------------operators-------------- */
                            "tok_equal",        "tok_ne",           "tok_lt",           "tok_le",           "tok_gt",       "tok_ge",
                            "tok_plus",         "tok_minus",        "tok_mul",          "tok_div",          "tok_assign",

                            /* --------------delimiters-------------- */
                            "tok_comma",        "tok_semicolon",    "tok_period",       "tok_op_prnth",     "tok_cl_prnth",
                            "tok_op_brckt",     "tok_cl_brckt",     "tok_colon",

                            /* --------------others-------------- */
                            "tok_identifier",   "tok_number",       "tok_eof",          "tok_unknown"        };

const std::string  Scanner::st_name[] = { "st_unknown",       "st_keyword",       "st_variable",      "st_literal",       "st_operator"   };
const std::string  Scanner::dt_name[] = { "dt_unknown",       "dt_none",          "dt_integer",       "dt_real"    };


Scanner::Scanner(const char* file_name):ln_num(1), prog_file(file_name), sym_table()
{

}


void Scanner::scan()
{
	token next_tok = next_token();
	while( next_tok.type != tok_eof )
	{
		next_tok = next_token();
	}
}

#define PT(EX) cout << setw(15) << left << ##EX ;

void Scanner::print_lexeme_tokens() const
{
	vector<attr_table_entry>::const_iterator it_att = attrs.begin();
	vector<string>::const_iterator it_lex = lexemes.begin();

	cout << setw(15) << right << "LEXEME\t\t|\t";
	cout << setw(15) << left << "TOKEN" \
         << setw(15) << left << "SEMANTIC_TYPE" \
	     << setw(15) << left << "DATA_TYPE" \
	     << setw(15) << left << "VALUE" << endl;
	cout << "-------------------------------------------------------------------------------------------------" << endl;

	for(; it_lex != lexemes.end() && it_att != attrs.end(); it_att++, it_lex++)
	{
		cout << setw(15) << right << *it_lex << "\t\t|\t" ;
		cout << setw(15) << left << tok_name[it_att->tok];
		cout << setw(15) << left << st_name[it_att->st];
        cout << setw(15) << left << dt_name[it_att->dt];
        cout << setw(15) << left << it_att->value << endl;
	}
}

token Scanner::next_token()
{
	char next_c = next_char();
	string next_lexeme;
	attr_table_entry new_attr;

	if( isalpha(next_c) )
	{
		next_lexeme = next_word(next_c);

		//in case the lexeme is not found in reserved words.
        new_attr.tok = tok_id;
        new_attr.st = st_variable;
	}
	else if( isdigit(next_c) )
	{
		next_lexeme = next_number(next_c);
        new_attr.tok = tok_number;
        new_attr.st = st_literal;
        new_attr.value = next_lexeme;
        if( next_lexeme.find_first_of(".Ee") != string::npos)
            new_attr.dt = dt_real;
        else
            new_attr.dt = dt_integer;

	}
	else if (next_c == '\0')
	{
	    next_lexeme = '\0';
	    new_attr.tok = tok_eof;
	}
	else
	{
	    next_lexeme = next_operator(next_c);
	    new_attr.tok = tok_unknown; //in case the lexeme is not found in intrinsic operators.
	}


    int name_ndx = -1, attr_ndx = -1;
    if( sym_table.is_present( next_lexeme, name_ndx, attr_ndx ) )
    {
        new_attr = sym_table.get_attr(attr_ndx);
    }
    else
    {
        int attr_ndx = sym_table.install_name( next_lexeme );
		sym_table.set_attribute( attr_ndx, new_attr );
    }

	lexemes.push_back(next_lexeme);

	token next_tok(next_lexeme, tok_name[new_attr.tok], new_attr.tok);
	attrs.push_back(new_attr);

	return next_tok;
}

char Scanner::next_char()
{
	char rt_char = '\0';
	while( !prog_file.eof() )
	{
	    prog_file.get(rt_char);
		while( !prog_file.eof() && (rt_char == ' ' || rt_char == '\t' || rt_char == '\r' || rt_char == '\n'))
		{
		    if(rt_char == '\n')
                ln_num++;
			prog_file.get(rt_char);
		}

		if( rt_char == '{' )
		{
			while( rt_char != '}' && !prog_file.eof())
			{
			    prog_file.get(rt_char);
			    if(rt_char == '\n')
                    ln_num++;
			}
			continue;
		}
		else
			break;
	}

	if( prog_file.eof() && (rt_char == ' ' || rt_char == '\t' || rt_char == '\r' || rt_char == '\n'))
		return '\0';
	else
		return toupper(rt_char);
}

string Scanner::next_word(char c)
{
	string rt_word;
	rt_word += c;
	char ch;
	while( !prog_file.eof() )
	{
		prog_file.get(ch);
        if(!isalnum(ch))
		{
			prog_file.unget();
			break;
		}
		else
			rt_word += toupper(ch);
	}

	return rt_word;
}

/*
 *   "=",    "<>",   "<",    "<=",   ">",    ">=",
 *   "+",    "-",    "*",    "/",    ":=",
 *   ";",    ".",
 */
string Scanner::next_operator(char c)
{
    string rt_op;
	rt_op += c;
	const int op_2c_sz = 4;
	const string op_2c[op_2c_sz] = {"<>", "<=", ">=", ":="};

    // if the operator starts with ':', '<' or '>', it could be two-character operator
    if (( c == '<' || c == '>' || c == ':') && !prog_file.eof())
    {
        prog_file.get(c);
        string temp = rt_op + c;
        int i = 0;
		for(; i < op_2c_sz; i++)
		{
		    if(temp == op_2c[i])
                break;
		}
		if(i == op_2c_sz)
		{
		    prog_file.unget();
		    if(c == '\n')
                ln_num--;
		}
        else
            rt_op = temp;
    }

    return rt_op;

}

/*
 * sign: + | - | empty
 * digits: digit digit*
 * optional_fraction: . digits | empty
 * optional_exponent: (E (+ | - | empty ) digits )| empty
 * num: sign digits optional_fraction optional_exponent
 */
string Scanner::next_number(char c)
{
	string rt_word;
	rt_word += c;
	char ch;

	// digits
	while(isdigit(prog_file.peek()))
	{
		prog_file.get(ch);
		rt_word += ch;
	}

	// optional_fraction
	if( prog_file.peek() == '.')
	{
		prog_file.get(ch);
		rt_word += ch;
		while(isdigit(prog_file.peek()))
		{
			prog_file.get(ch);
			rt_word += ch;
		}
	}

	// optional_exponent
	if(prog_file.peek() == 'E')
	{
		prog_file.get(ch);
		string exp;
		exp += ch;
		ch = (char) prog_file.peek();
		if (ch == '+' || ch == '-' || isdigit(ch))
		{
		    prog_file.get(ch);
		    exp += ch;
            while(isdigit(prog_file.peek()))
			{
				prog_file.get(ch);
				exp += ch;
			}

			if(!isdigit(ch)) // exponent must end with digit.
				prog_file.unget();
			else
				rt_word += exp;
		}
		else
			prog_file.unget();
	}

	return rt_word;
}
