#include <string>
#include <vector>
#include <map>


std::set<string> non_term_tp;

struct production
{
    string non_term;
    std::vector<string> rh_items;
}

std::vector<production> prod_lst;

typedef std::pair<string, string> nt_t_pair;
std::map<nt_t_pair, int> parse_table;

