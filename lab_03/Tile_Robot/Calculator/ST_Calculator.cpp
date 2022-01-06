#include "ST_Calculator.h"

ST_Calculator::ST_Calculator(std::string math_expr)
{
    std::string reg = "(" + math_expr + ")";
    reg = a_parse(reg);

    int first, last;

    do
    {
        first = -1;
        last = -1;

        for (int i = 0; i < reg.length(); i++)
        {
            if (reg[i] == '(')
            {
                first = i;
            }
            if (reg[i] == ')')
            {
                last = i;
                break;
            }
        }

        if (first == -1 && last == -1) break;

        reg[first] = ' ';
        reg[last] = ' ';
        add_brackets(first, last);

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '@')
            {
                create_node(reg[i], i, a_node);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '*')
            {
                create_node(reg[i], i, mult_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '/')
            {
                create_node(reg[i], i, div_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '%')
            {
                create_node(reg[i], i, mod_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '+')
            {
                create_node(reg[i], i, plus_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '-')
            {
                create_node(reg[i], i, sub_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '=')
            {
                create_node(reg[i], i, even_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '>')
            {
                create_node(reg[i], i, more_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '<')
            {
                create_node(reg[i], i, less_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

    } while (first != -1 && last != -1);
   
    this->root = _get_root();
}

ST_Node* ST_Calculator::get_root()
{
    return this->root;
}

void ST_Calculator::draw_syntax_tree(std::string file_name)
{
    if (this->root == nullptr) return;
    std::ofstream* out = new std::ofstream(file_name);

    *out << "digraph G {" << std::endl;

    if ((this->root)->left_ptr == nullptr && (this->root)->right_ptr == nullptr)
    {
        *out << "\"" << "id:" << (this->root)->index << " " << (this->root)->sign << "\"" << std::endl;
        *out << "}";
        return;
    }

    for (auto it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        if ((*it)->left_ptr != nullptr)
        {
            *out << "\"" << "id:" << (*it)->index << " " << (*it)->sign << "\"" << " -> " << "\"" << "id:" << (*it)->left_ptr->index << " " << (*it)->left_ptr->sign << "\"" << std::endl;
        }
        if ((*it)->right_ptr != nullptr)
        {
            *out << "\"" << "id:" << (*it)->index << " " << (*it)->sign << "\"" << " -> " << "\"" << "id:" << (*it)->right_ptr->index << " " << (*it)->right_ptr->sign << "\"" << std::endl;
        }
    }

    *out << "}";
}

void ST_Calculator::calculate(ST_Node* cur_node, var_type calc_type, Function* cur_fun)
{

}

void ST_Calculator::add_brackets(int open, int close)
{
	std::pair<int, bracket_type> t1 = { open, BT_open };
	std::pair<int, bracket_type> t2 = { close, BT_close };
	this->brackets.push_back(t1);
	this->brackets.push_back(t2);
}

int ST_Calculator::is_this_bracket(int index)
{
	for (auto iter = this->brackets.begin(); iter != this->brackets.end(); iter++)
	{
		if ((*iter).first == index) return 1;
	}
	return 0;
}

bracket_type ST_Calculator::_get_bracket_type(int index)
{
	for (auto iter = this->brackets.begin(); iter != this->brackets.end(); iter++)
	{
		if ((*iter).first == index) return (*iter).second;
	}
	return bt_error;
}

ST_Node* ST_Calculator::_get_node(int index)
{
	for (auto iter = this->nodes.begin(); iter != this->nodes.end(); iter++)
	{
		if ((*iter)->index == index) return *iter;
	}
	return nullptr;
}

stn_type ST_Calculator::_get_type(int index)
{
	for (auto iter = this->nodes.begin(); iter != this->nodes.end(); iter++)
	{
		if ((*iter)->index == index) return (*iter)->type;
	}
	return stn_error;
}

void ST_Calculator::create_node(char value, int index, stn_type type)
{
	this->nodes.push_back(new ST_Node(value, index, type));
}

void ST_Calculator::add_children(int parent_i, int child_i)
{
	ST_Node* parent = _get_node(parent_i);
	
    while(is_this_bracket(child_i)) 
    {
	    if (is_this_bracket(child_i) && _get_bracket_type(child_i) == BT_open)
	    {
            ++child_i;
	    }
	    else if (is_this_bracket(child_i) && _get_bracket_type(child_i) == BT_close)
	    {
            --child_i;
	    }
    }

    if (this->_get_type(child_i) == stn_error) ++child_i;

	ST_Node* child = _get_node(child_i);

	while (child->parent != nullptr)
	{
		child = child->parent;
	}

    if (parent->left_ptr == nullptr)
    {
        parent->left_ptr = child;
    }
    else parent->right_ptr = child;

	child->parent = parent;
}

ST_Node* ST_Calculator::_get_root()
{
	ST_Node* temp = this->nodes.back();

	while (temp->parent != nullptr)
	{
		temp = temp->parent;
	}

	return temp;
}

std::string ST_Calculator::a_parse(std::string reg)
{
    std::regex sign("[*]|[/]|[%]|[+]|[-]|[=]|[>]|[<]|[(]|[)]");
    std::string parsed = "";
    std::string t_value = "";

    for (int i = 0; i < reg.length(); i++)
    {
        if (!regex_match(std::string(1, reg[i]), sign))
        {
            t_value += reg[i];
        } 
        else
        {      
            if (t_value.size() != 0)
            {
                parsed += "@";
                this->node_values[parsed.size() - 1] = t_value;           
            }
            parsed += std::string(1, reg[i]);
            t_value = "";
        }
    }

    if (t_value.size() != 0)
    {
        parsed += "@";
        this->node_values[parsed.size() - 1] = t_value;
    }

    return parsed;
}
