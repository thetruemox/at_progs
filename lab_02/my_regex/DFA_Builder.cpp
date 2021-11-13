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
    while (1)
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

