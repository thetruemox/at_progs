#include "DFA_Builder.h"

DFA_Builder::DFA_Builder(std::string regex)
{
    this->nfa_builder = new NFA_Builder(regex);
    this->abc = nfa_builder->get_abc();

    int id = 0;
    this->abc = abc;

    //Проверка стартового состояния
	DFA_Agent* agent = new DFA_Agent(id++);
    agent->set_of_states.push_back(this->nfa_builder->get_start());
    for (auto it = agent->set_of_states.begin(); it != agent->set_of_states.end(); it++)
    {
        z_eps(agent, (*it));
    }
	this->agents.push_back(agent);
    this->graph.push_back(new DFA_Node(agent->id));
    //По итогу имеем 1-го агента, с которого и начнем обработку

    DFA_Agent* new_agent;
    while (agent != nullptr)
    {
        agent = nullptr;
        //Ищем необработанного агента
        for (auto it = this->agents.begin(); it != this->agents.end(); it++)
        {
            if (!(*it)->checked)
            {
                agent = (*it);
                break;
            }
        }
        if (agent == nullptr) break;

        for (auto abc_it = this->abc.begin(); abc_it != this->abc.end(); abc_it++)
        {
            new_agent = new DFA_Agent(id++);

            //m_abc
            for (auto set_it = agent->set_of_states.begin(); set_it != agent->set_of_states.end(); set_it++)
            {
                for (auto link_it = (*set_it)->links.begin(); link_it != (*set_it)->links.end(); link_it++)
                {
                    if ((*link_it).second == (*abc_it).first)
                    {
                        new_agent->set_of_states.push_back((*link_it).first);
                    }
                }
            }

            //z_eps
            for (auto set_it = new_agent->set_of_states.begin(); set_it != new_agent->set_of_states.end(); set_it++)
            {
                z_eps(new_agent, (*set_it));
            }

            //Если new_agent уникальный, то делаем связку его с agent по условию (*abc_it).first
            if (is_unique(new_agent, agent, (*abc_it).first))
            {
                this->agents.push_back(new_agent);
                this->graph.push_back(new DFA_Node(new_agent->id));
                get_node(agent->id)->make_link(get_node(new_agent->id), (*abc_it).first);
            }
            else
            {
                delete new_agent;
            }

            agent->checked = 1;
        }

    }
    
    this->graph.front()->type = dfa_start;

    for (auto ag_it = this->agents.begin(); ag_it != this->agents.end(); ag_it++)
    {
        for (auto set_it = (*ag_it)->set_of_states.begin(); set_it != (*ag_it)->set_of_states.end(); set_it++)
        {
            if ((*set_it)->id == this->nfa_builder->get_recieve()->id)
            {
                this->get_node((*ag_it)->id)->type = dfa_receiving;
            }
        }
    }

    this->minimize();
}

int DFA_Builder::is_unique(DFA_Agent* new_agent, DFA_Agent* agent, std::string cond)
{
    int ans = 1;

    for (auto ags_it = this->agents.begin(); ags_it != this->agents.end(); ags_it++)
    {
        if (new_agent->set_of_states.size() == (*ags_it)->set_of_states.size())
        {
            ans = 0;
            for (auto set_it = new_agent->set_of_states.begin(); set_it != new_agent->set_of_states.end(); set_it++)
            {
                if (!(*ags_it)->saw_it((*set_it))) 
                { 
                    ans = 1;
                    break;
                }            
            }

            if (ans == 0)
            {
                get_node(agent->id)->make_link(get_node((*ags_it)->id), cond); //ошибка здесь
                return ans;
            }
        }
    }

    return ans;
}

void DFA_Builder::z_eps(DFA_Agent* agent, NFA_Node* node)
{
    for (auto it = node->links.begin(); it != node->links.end(); it++)
    {
        if ((*it).second == "eps" && !agent->saw_it((*it).first))
        {       
            agent->set_of_states.push_back((*it).first);
            z_eps(agent, (*it).first);          
        }
    }
}

DFA_Node* DFA_Builder::get_node(int id)
{
    DFA_Node* node = nullptr;

    for (auto it = this->graph.begin(); it != this->graph.end(); it++)
    {
        if ((*it)->id == id)
        {
            node = (*it);
        }
    }

    return node;
}

DFA_Node* DFA_Builder::get_front()
{
    return this->graph.front();
}

void DFA_Builder::draw_min_dfa_graph(std::string file_name)
{
    std::ofstream* out = new std::ofstream(file_name);
    *out << "digraph G {" << std::endl;

    *out << this->min_graph.front()->id << " [label=\"" << this->min_graph.front()->id << "\\nstart\"];" << std::endl;

    for (auto it_mg = this->min_graph.begin(); it_mg != this->min_graph.end(); it_mg++)
    {
        if ((*it_mg)->type == dfa_receiving)
        {
            *out << (*it_mg)->id << " [label=\"" << (*it_mg)->id << "\\nrecieve\"];" << std::endl;
        }

        for (auto it_nl = (*it_mg)->links.begin(); it_nl != (*it_mg)->links.end(); it_nl++)
        {
            *out << (*it_mg)->id << " -> " << (*it_nl).first->id << "[label=\"" << (*it_nl).second << "\"];" << std::endl;
        }

    }

    *out << "}";
}

void DFA_Builder::draw_dfa_graph(std::string file_name)
{
    if (get_front() == nullptr) return;

    std::ofstream* out = new std::ofstream(file_name);
    *out << "digraph G {" << std::endl;

    *out << get_front()->id << " [label=\"" << get_front()->id << "\\nstart\"];" << std::endl;
    
    for (auto it = this->graph.begin(); it != this->graph.end(); it++)
    {
        if ((*it)->type == dfa_receiving)
        {
            *out << (*it)->id << " [label=\"" << (*it)->id << "\\nrecieve\"];" << std::endl;
        }
    }

    _recursive_drawing(get_front(), out);

    *out << "}";
}

void DFA_Builder::draw_nfa_graph(std::string file_name)
{
    this->nfa_builder->draw_nfa_graph(file_name);
}

void DFA_Builder::draw_syntax_tree(std::string file_name)
{
    this->nfa_builder->draw_syntax_tree(file_name);
}

void DFA_Builder::_recursive_drawing(DFA_Node* node, std::ofstream* out)
{
    if (node->checked) return;

    for (auto it = node->links.begin(); it != node->links.end(); it++)
    {
        *out << node->id << " -> " << (*it).first->id << "[label=\"" << (*it).second << "\"];" << std::endl;
    }
    node->checked = 1;

    for (auto it = node->links.begin(); it != node->links.end(); it++)
    {
        _recursive_drawing((*it).first, out);
    }
}

void DFA_Builder::minimize()
{
    int g_id = 0;
    std::list<std::pair<int, std::list<int>*>> splits;

    splits.push_back({ g_id++, new std::list<int> });
    for (auto it = this->graph.begin(); it != this->graph.end(); it++)
    {
        if ((*it)->type == dfa_normal || (*it)->type == dfa_start)
        {
            splits.back().second->push_back((*it)->id);
        }
    }

    splits.push_back({ g_id++, new std::list<int> });
    for (auto it = this->graph.begin(); it != this->graph.end(); it++)
    {
        if ((*it)->type == dfa_receiving)
        {
            splits.back().second->push_back((*it)->id);
        }
    }

    int prev_group, cur_group;
    int changed = 1;
    while (changed)
    {
        changed = 0;
        for (auto it_s = splits.begin(); it_s != splits.end(); it_s++)
        {
            if ((*it_s).second->size() == 1) continue;

            for (auto it_abc = this->abc.begin(); it_abc != this->abc.end(); it_abc++)
            {
                prev_group = -1;
                for (auto it_g = (*it_s).second->begin(); it_g != (*it_s).second->end(); it_g++) //it_g == id текущего члена группы
                {
                    if (prev_group == -1) //Первый элемент в группе
                    {
                        prev_group = _find_group(this->get_node((*it_g))->transition((*it_abc).first)->id, &splits); 
                    }
                    else
                    {
                        cur_group = _find_group(this->get_node((*it_g))->transition((*it_abc).first)->id, &splits);

                        if (cur_group != prev_group) //Нашли элемент с отличной группой перехода, создаем новую для него, из старой убираем
                        {
                            splits.push_back({ g_id++, new std::list<int> });
                            splits.back().second->push_back((*it_g));

                            (*it_s).second->erase(it_g);

                            changed = 1;
                            break;
                        }
                    }                       
                }
                if (changed) break;
            }
            if (changed) break;
        }
        if (!changed) break;
    }

    //Отладка
    /*for (auto it_s = splits.begin(); it_s != splits.end(); it_s++)
    {
        std::cout << "Group " << (*it_s).first << " {";
        for (auto it_g = (*it_s).second->begin(); it_g != (*it_s).second->end(); it_g++)
        {
            std::cout << (*it_g) << " ";
        }
        std::cout << "}" << std::endl;
    } */

    //Построение графа
    for (auto it_s = splits.begin(); it_s != splits.end(); it_s++)
    {
        min_graph.push_back(new DFA_Node((*it_s).first));
    }

    auto it_mg = min_graph.begin();
    DFA_Node* t_node;
    for (auto it_s = splits.begin(); it_s != splits.end(); it_s++)
    {
        t_node = this->get_node((*it_s).second->front());
        for (auto it_nl = t_node->links.begin(); it_nl != t_node->links.end(); it_nl++)
        {
            (*it_mg)->make_link(this->get_mg_node(this->_find_group((*it_nl).first->id, &splits)), (*it_nl).second);      
            (*it_mg)->type = t_node->type;
        }   
        ++it_mg;
    }
}

int DFA_Builder::_find_group(int id, std::list<std::pair<int, std::list<int>*>>* splits)
{
    for (auto it_s = splits->begin(); it_s != splits->end(); it_s++)
    {
        for (auto it_g = (*it_s).second->begin(); it_g != (*it_s).second->end(); it_g++)
        {
            if ((*it_g) == id) return (*it_s).first;
        }
    }
    throw("there is no such id!");
    return -1;
}

DFA_Node* DFA_Builder::get_mg_node(int g_id)
{
    for (auto it = this->min_graph.begin(); it != this->min_graph.end(); it++)
    {
        if ((*it)->id == g_id) return (*it);
    }

    return nullptr;
}

