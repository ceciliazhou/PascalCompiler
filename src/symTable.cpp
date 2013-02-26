#include "symTable.h"
using namespace std;

const string rsv_word[] = { /* --------------keywords (only a subset of pascal reserved words)-------------- */
                            "PROGRAM",      "VAR",      "INTEGER",      "REAL",     "PROCEDURE",
                            "BEGIN",        "END",      "IF",           "THEN",     "WHILE",
                            "DO",           "ELSE",     "NOT",          "DIV",      "MOD",
                            "AND",          "OR",       "CONST",        "ARRAY",    "FUNCTION",
                            "OF",

                            /* --------------operators (only a subset of pascal operators)-------------- */
                            "=",    "<>",   "<",    "<=",   ">",    ">=",
                            "+",    "-",    "*",    "/",    ":=",

                            /* --------------delimiters-------------- */
                            ",",     ";",    ".",    "(",    ")",    "[",    "]",
                            ":"  };


SymTable::SymTable()
{
	// Install the keywords, operators and delimiters in the name table and
	// set the token type in attribute table.
	for( int i = 0;  i < keywords_num + op_num + delimiters_num;  i++ )
	{
		int ndx_attr = install_name( rsv_word[i] );
		attr_table_entry new_attr( 0, (tok_type)i, st_keyword, dt_none );
		set_attribute( ndx_attr, new_attr );
	}
}

int SymTable::install_name(string name)
{
    int name_ndx = name_table.size();
    name_table_entry new_name( name );
    name_table.push_back( new_name );

    int attr_ndx = install_attribute( name_ndx );
    name_table.at(name_ndx).ndx_attr = attr_ndx;

    return attr_ndx;
}

bool SymTable::is_present(string name, int& ndx_name, int& ndx_attr) const
{
    int ndx = 0;
    vector< name_table_entry >::const_iterator it = name_table.begin();
    for (; it != name_table.end(); ++it, ++ndx )
    {
        if( name == it->name )
        {
            ndx_name = ndx;
            ndx_attr = name_table.at(ndx).ndx_attr;
            return true;
        }
    }

    return false;
}

int	SymTable::install_attribute(int ndx_name)
{
    int attr_ndx = attr_table.size();
    attr_table_entry new_attr(ndx_name);
    attr_table.push_back(new_attr);

    return attr_ndx;
}

void SymTable::set_attribute(int ndx_attr, const attr_table_entry& new_attr)
{
    attr_table.at(ndx_attr).tok = new_attr.tok;
    attr_table.at(ndx_attr).st = new_attr.st;
    attr_table.at(ndx_attr).dt = new_attr.dt;
    attr_table.at(ndx_attr).value = new_attr.value;
    attr_table.at(ndx_attr).scope = new_attr.scope;
}

attr_table_entry SymTable::get_attr( int ndx_attr )
{
    return attr_table.at( ndx_attr );
}
