#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string>
#include <vector>


enum tok_type { /* --------------keywords-------------- */
                tok_program,    tok_var,    tok_integer,    tok_real,       tok_procedure,
                tok_begin,      tok_end,    tok_if,         tok_then,       tok_while,
                tok_do,         tok_else,   tok_not,        tok_int_div,    tok_mod,
                tok_and,        tok_or,     tok_const,      tok_array,      tok_function,
                tok_of,

                /* --------------operators-------------- */
                tok_equal,      tok_ne,         tok_lt,         tok_le,     tok_gt,         tok_ge,
                tok_add,        tok_subtract,   tok_mul,        tok_div,    tok_assign,

                /* --------------delimiters or separators-------------- */
                tok_comma,      tok_semicolon,  tok_period,     tok_op_prnth,   tok_cl_prnth,   tok_op_brckt,   tok_cl_brckt,
                tok_colon,

                /* --------------others-------------- */
                tok_id,     tok_number,     tok_eof,        tok_unknown  };

enum semantic_type	{   st_unknown,      st_keyword,        st_variable,     st_literal,      st_operator   };
enum data_type      {   dt_unknown,      dt_none,           dt_integer,      dt_real    };

enum { keywords_num = 21, op_num = 11, delimiters_num = 8, other_tokens_num = 4, semantic_type_num = 5, data_type_num = 4 };

struct name_table_entry
{
     std::string        name;
     int                ndx_attr;

     name_table_entry(std::string nm, int attr = -1):name(nm), ndx_attr(attr){}
};

struct	attr_table_entry
{
     int			ndx_name;
     tok_type		tok;
     semantic_type  st;
     data_type      dt;
     std::string    value;
     int            scope; //not implemented yet.

     attr_table_entry( int ndx = -1,\
                       tok_type token = tok_unknown, \
                       semantic_type stype = st_unknown, \
                       data_type dtype = dt_unknown, \
                       std::string val = "0",
                       int scp = 0 )
    {
        tok = token;
        st = stype;
        dt = dtype;
        value = val;
        scope  = scp;
        ndx_name = ndx;
    };
};


class	SymTable	{

/* interface */
public:
	SymTable();
	~SymTable(){}


	/*
	 *	Return true if the name exists; -1 otherwise.
	 */
	bool is_present(std::string name, int& ndx_name, int& ndx_attr) const;

	/*
	 * Install a new name if the new name doesn't exist yet.
	 * Install attribute entry if there's no attribute entry pointed by the name.
	 * And set up the link between name and attribute.
	 * Return the attribute index.
	 */
    int install_name(std::string name);

    /*
	 *	set the attributes in the attribute table.
	 *  So far only token class will be set, extensible interface.
	 */
	void set_attribute(int ndx_attr, const attr_table_entry& new_attr);

	tok_type get_tok(int ndx_attr) const { return attr_table.at(ndx_attr).tok; }
	attr_table_entry get_attr( int ndx_attr );

private:
    std::vector< name_table_entry > name_table;
    std::vector< attr_table_entry > attr_table;

	/*
	 *	Add a new attribute table entry and return the index.
	 */
	int	install_attribute(int ndx_name);
};

#endif
