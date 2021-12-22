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

void My_Regex::draw_dfa_graph(std::list<DFA_Node*> dfa_graph, std::string file_name)
{
    std::ofstream* out = new std::ofstream(file_name);
    *out << "digraph G {" << std::endl;

    *out << dfa_graph.front()->id << " [label=\"" << dfa_graph.front()->id << "\\nstart\"];" << std::endl;

    for (auto it_mg = dfa_graph.begin(); it_mg != dfa_graph.end(); it_mg++)
    {
        if ((*it_mg)->type == dfa_receiving)
        {
            *out << (*it_mg)->id << " [label=\"" << (*it_mg)->id << "\\nrecieve\"];" << std::endl;
        }
        else if ((*it_mg)->type == dfa_start_and_receiving)
        {
            *out << (*it_mg)->id << " [label=\"" << (*it_mg)->id << "\\nstart and recieve\"];" << std::endl;
        }

        for (auto it_nl = (*it_mg)->links.begin(); it_nl != (*it_mg)->links.end(); it_nl++)
        {
            *out << (*it_mg)->id << " -> " << (*it_nl).first->id << "[label=\"" << (*it_nl).second << "\"];" << std::endl;
        }

    }

    *out << "}";
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

My_Regex* My_Regex::addition(std::string regex)
{
    std::map<std::string, int> abc = this->dfa_builder->get_abc();
    std::string new_reg = "(";

    for (auto it = abc.begin(); it != abc.end(); it++)
    {
        new_reg += (*it).first + "|";
    }
    new_reg[new_reg.length() - 1] = ')';
    new_reg += "+";

    DFA_Builder* sigma_kleene = new DFA_Builder(new_reg);
    DFA_Builder* l_graph = new DFA_Builder(regex);
    std::list<DFA_Node*> subtract = this->_multiply(sigma_kleene->get_min_dfa_graph(), l_graph->get_min_dfa_graph(), abc);

    this->draw_dfa_graph(subtract, "addon.txt");

    return nullptr;
}

std::list<DFA_Node*> My_Regex::_multiply(std::list<DFA_Node*> graph_A, std::list<DFA_Node*> graph_B, std::map<std::string, int> abc)
{
    std::list<DFA_Node*> graph_mul;
    int id = 0;

    for (auto it_A = graph_A.begin(); it_A != graph_A.end(); it_A++)
    {
        for (auto it_B = graph_B.begin(); it_B != graph_B.end(); it_B++)
        {
            graph_mul.push_back(new DFA_Node(id++, (*it_A)->id, (*it_B)->id));
        }
    }

    graph_mul.front()->type = dfa_start;

    DFA_Node* tr_node_A, * tr_node_B;
    for (auto it_gm = graph_mul.begin(); it_gm != graph_mul.end(); it_gm++)
    {
        for (auto it_abc = abc.begin(); it_abc != abc.end(); it_abc++)
        {
            tr_node_A = get_node((*it_gm)->mul_id_f, graph_A)->transition((*it_abc).first);
            tr_node_B = get_node((*it_gm)->mul_id_s, graph_B)->transition((*it_abc).first);
            if (tr_node_A != nullptr && tr_node_B != nullptr)
            {
                (*it_gm)->make_link(_find_mul_node(graph_mul, tr_node_A->id, tr_node_B->id), (*it_abc).first);
            }
        }

        //Часть с назначанием принимающего состояния, возможно стоит перенести для большей универсальности функции умножения автоматов
        if ((get_node((*it_gm)->mul_id_f, graph_A)->type == dfa_receiving || get_node((*it_gm)->mul_id_f, graph_A)->type == dfa_start_and_receiving) && (get_node((*it_gm)->mul_id_s, graph_B)->type != dfa_receiving || get_node((*it_gm)->mul_id_s, graph_B)->type != dfa_start_and_receiving))
        {
            if ((*it_gm)->type == dfa_start)
            {
                (*it_gm)->type = dfa_start_and_receiving;
            } else (*it_gm)->type = dfa_receiving;
        }
    }

    return graph_mul;
}

DFA_Node* My_Regex::_find_mul_node(std::list<DFA_Node*> graph, int id1, int id2)
{
    for (auto it = graph.begin(); it != graph.end(); it++)
    {
        if ((*it)->mul_id_f == id1 && (*it)->mul_id_s == id2)
        {
            return (*it);
        }
    }
    return nullptr;
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

    if (cur_node->type == dfa_receiving || cur_node->type == dfa_start_and_receiving)
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

    if (cur_node->type == dfa_receiving || cur_node->type == dfa_start_and_receiving)
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

DFA_Node* My_Regex::get_node(int id, std::list<DFA_Node*> node_list)
{
    for (auto it = node_list.begin(); it != node_list.end(); it++)
    {
        if ((*it)->id == id) return (*it);
    }
    return nullptr;
}


