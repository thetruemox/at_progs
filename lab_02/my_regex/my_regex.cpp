#include "My_Regex.h"

My_Regex::My_Regex(std::string regex)
{
    DFA_Builder dfa_builder(regex);
    this->dfa_graph = dfa_builder.get_min_dfa_graph();
    this->dfa_graph_start = this->dfa_graph.front();
    this->dfa_recieves = dfa_builder.get_min_dfa_graph_recieves();

    dfa_builder.draw_min_dfa_graph("min_dfa.txt");
    dfa_builder.draw_dfa_graph("dfa.txt");
    dfa_builder.draw_nfa_graph("nfa.txt");
    dfa_builder.draw_syntax_tree("stree.txt");
}

int My_Regex::check_str(std::string str)
{
    DFA_Node* cur_node = this->dfa_graph_start;
    DFA_Node* next_node = nullptr;

    for (int i = 0; i < str.length(); i++)
    {
        next_node = cur_node->transition(str[i]);
        if (next_node != nullptr)
        {
            cur_node = next_node;
        }
        else 
        {
            std::cout << "There is no transition: " << str[i] << std::endl;
            return 0;
        }
    }
    
    if (cur_node->type == dfa_receiving)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

std::string My_Regex::restore_regex(std::string file_name)
{
    std::ofstream* out = new std::ofstream(file_name);

    int k = this->dfa_graph.size();
    int i = this->dfa_graph_start->id;
    int j;
    std::string regex;

    for (auto it = this->dfa_recieves.begin(); it != this->dfa_recieves.end(); it++)
    {
        j = (*it)->id;
        regex += _R(k-1, i, j) + "|" + _R(k - 1, i, k) + ".^|(" + _R(k - 1, k, k) + ")+." + _R(k - 1, k, j);
        regex += "|";
    }
    regex.erase(regex.size() - 1);
    
    *out << regex;

    return regex;
}

std::string My_Regex::_R(int k, int i, int j)
{
    std::string regex;

    if (k > 1)
    {
        regex += _R(k - 1, i, j) + "|" + _R(k - 1, i, k) + ".^|(" + _R(k - 1, k, k) + ")+." + _R(k - 1, k, j);
    } 
    else
    {
        regex += _null_R(i, j) + "|" + _null_R(i, k) + ".^|(" + _null_R(k, k) + ")+." + _null_R(k, j);
    }

    if (regex == "^|^.^|(^)+.^")
    {
        return "^";
    } else return regex;
}

std::string My_Regex::_null_R(int i, int j)
{
    std::string tmp_r;

    DFA_Node* i_node = get_node(i);
    DFA_Node* j_node = get_node(j);

    for (auto it = i_node->links.begin(); it != i_node->links.end(); it++)
    {
        if ((*it).first->id == j_node->id)
        {
            tmp_r += (*it).second + ".";
        }
    }

    if (tmp_r.size() == 0)
    {
        tmp_r = "^";
    }
    else
    {
        tmp_r.erase(tmp_r.size() - 1);
    }
        
    return tmp_r;
}

DFA_Node* My_Regex::get_node(int id)
{
    for (auto it = this->dfa_graph.begin(); it != this->dfa_graph.end(); it++)
    {
        if ((*it)->id == id) return (*it);
    }
    return nullptr;
}


