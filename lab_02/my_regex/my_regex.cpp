#include "My_Regex.h"

My_Regex::My_Regex(std::string regex)
{
    this->dfa_builder = new DFA_Builder(regex);

    this->CG = dfa_builder->get_CG();
    this->dfa_graph = dfa_builder->get_min_dfa_graph();
    this->dfa_graph_start = this->dfa_graph.front();
    this->dfa_recieves = dfa_builder->get_min_dfa_graph_recieves();

    /*
    dfa_builder->draw_min_dfa_graph("min_dfa.txt");
    dfa_builder->draw_dfa_graph("dfa.txt");
    dfa_builder->draw_nfa_graph("nfa.txt");
    dfa_builder->draw_syntax_tree("stree.txt");
    */
}

My_Regex::My_Regex(DFA_Builder* dfa_builder)
{
    this->dfa_builder = dfa_builder;
    this->CG = dfa_builder->get_CG();
    this->dfa_graph = dfa_builder->get_min_dfa_graph();
    this->dfa_graph_start = this->dfa_graph.front();
    this->dfa_recieves = dfa_builder->get_min_dfa_graph_recieves();
}

My_Regex* My_Regex::inversion()
{
    Syntax_Tree* tmp_stree = new Syntax_Tree(this->restore_regex());
    _flip_stree(tmp_stree->get_root());
    DFA_Builder* dfa_builder = new DFA_Builder(new NFA_Builder(tmp_stree));
    My_Regex* my_regex = new My_Regex(dfa_builder);

    /*
    dfa->draw_min_dfa_graph("flip_dfa.txt");
    */
    return my_regex;
}


void My_Regex::_flip_stree(ST_Node* node)
{
    ST_Node* tmp_node;
    if (node->left_ptr != nullptr && node->right_ptr != nullptr)
    {
        tmp_node = node->left_ptr;
        node->left_ptr = node->right_ptr;
        node->right_ptr = tmp_node;
    }

    if (node->left_ptr != nullptr) _flip_stree(node->left_ptr);
    if (node->right_ptr != nullptr) _flip_stree(node->right_ptr);
}

int My_Regex::check_str(std::string str)
{
    DFA_Node* cur_node = this->dfa_graph_start;
    DFA_Node* next_node = nullptr;
    //CG_Unit* last_unit = &(this->CG->units.front());

    for (int i = 0; i < str.length(); i++)
    {
        next_node = cur_node->transition(str[i]);
        if (next_node != nullptr)
        {
            //CG->capture(cur_node->id, next_node->id, str[i], last_unit);
            cur_node = next_node;
        }
        else 
        {
            std::cout << "There is no transition: " << str[i] << std::endl;
            return 0;
        }
    }
    
    /*for (auto u_it = this->CG->units.begin(); u_it != this->CG->units.end(); u_it++)
    {
        std::cout << (*u_it).cap_str << " ";
    }*/

    if (cur_node->type == dfa_receiving)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int My_Regex::check_str(std::string str, std::map<int, std::string> &capture)
{
    DFA_Node* cur_node = this->dfa_graph_start;
    DFA_Node* next_node = nullptr;
    CG_Unit* last_unit = &(this->CG->units.front());

    for (int i = 0; i < str.length(); i++)
    {
        next_node = cur_node->transition(str[i]);
        if (next_node != nullptr)
        {
            CG->capture(cur_node->id, next_node->id, str[i], last_unit);

            cur_node = next_node;
        }
        else
        {
            std::cout << "There is no transition: " << str[i] << std::endl;
            return 0;
        }
    }

    for (auto u_it = this->CG->units.begin(); u_it != this->CG->units.end(); u_it++)
    {
        capture[(*u_it).CG_num] = (*u_it).cap_str;
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

std::string My_Regex::restore_regex()
{
    int k = this->dfa_graph.size();
    int i = this->dfa_graph_start->id;
    int j;
    std::string regex;

    for (auto it = this->dfa_recieves.begin(); it != this->dfa_recieves.end(); it++)
    {
        j = (*it)->id;

        regex += _R(k, i, j);
        regex += "|";
    }
    regex.erase(regex.size() - 1);

    return regex;
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

        regex += _R(k, i, j);
        regex += "|";
    }
    regex.erase(regex.size() - 1);
    
    *out << regex;
    
    return regex;
}

std::string My_Regex::_R(int k, int i, int j)
{
    std::string regex;
    std::string tmp_ij, tmp_ik, tmp_kk, tmp_kj;

    if (k > 1)
    {
        tmp_ij = _R(k - 1, i, j);
        tmp_ik = _R(k - 1, i, k);
        if (tmp_ik.size() == 0)
        {
            regex = tmp_ij;
            return regex;
        }
        tmp_kk = _R(k - 1, k, k);
        if (tmp_kk.size() == 0)
        {
            regex = tmp_ij;
            return regex;
        }
        tmp_kj = _R(k - 1, k, j);
        if (tmp_kj.size() == 0)
        {
            regex = tmp_ij;
            return regex;
        }
    } 
    else
    {   //todo пустое множество, то нет конкатенации.
        tmp_ij = _null_R(i, j);
        tmp_ik = _null_R(i, k);
        tmp_kk = _null_R(k, k);
        tmp_kj = _null_R(k, j);
    }

    if (tmp_ij == (tmp_ik + tmp_kk + tmp_kj))
    {
        regex = tmp_ij;
        if (regex.size() > 1) regex = "(" + regex + ")";
        return regex;
    }

    if (tmp_ij.size())
    {
        regex += tmp_ij + "|";
    }

    if (!tmp_ik.size() || !tmp_kk.size() || !tmp_kj.size())
    {
        if (regex.size() && (regex[regex.size() - 1] == '.' || regex[regex.size() - 1] == '|'))
        {
            regex.erase(regex.size() - 1);
        }
        return regex;
    }

    if (tmp_ik.size())
    {
        regex += tmp_ik + ".";
    }

    if (tmp_kk.size())
    {
        if (tmp_kk == "^")
        {
            regex += "^.";
        } else regex += "(^|" + tmp_kk + "+).";
       
    }

    if (tmp_kj.size())
    {
        regex += tmp_kj;
    }
    
    if (regex.size() && (regex[regex.size() - 1] == '.' || regex[regex.size() - 1] == '|'))
    {
        regex.erase(regex.size() - 1);
    }
    
    if (regex.size()) regex = "(" + regex + ")";

    return regex;
}

std::string My_Regex::_null_R(int i, int j)
{
    if (i == j) return "^";

    std::string tmp_r;

    DFA_Node* i_node = get_node(i);
    DFA_Node* j_node = get_node(j);

    for (auto it = i_node->links.begin(); it != i_node->links.end(); it++)
    {
        if ((*it).first->id == j_node->id)
        {
            tmp_r += (*it).second + "|";
        }
    }
    //если переход i в i то всегда подходит ^
    //если нет перехода, то не возвращай ^
    if (tmp_r.size() == 0)
    {
        tmp_r = "";
    }
    else
    {
        tmp_r.erase(tmp_r.size() - 1);
    }
        
    if (tmp_r.size() > 1)
    {
        tmp_r = "(" + tmp_r + ")";
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


