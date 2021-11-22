#include "My_Regex.h"

My_Regex::My_Regex(std::string regex)
{
    this->dfa_graph_start = (new DFA_Builder(regex))->get_min_dfa_graph();
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


