#include "Syntax_Tree.h"

Syntax_Tree::Syntax_Tree(std::string regex)
{
    std::string reg = this->rep_parse(regex);

    reg = "(" + reg + ")";
    int first, last;

    do
    {
        first = -1;
        last = -1;

        for (int i = 0; i < reg.length(); i++)
        {
            if (reg[i] == '#') i+=2;

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
            if (reg[i] != '+' && reg[i] != '|' && reg[i] != ' ' && reg[i] != '.')
            {
                if (reg[i] == '#')
                {
                    create_node(reg[i + 1], i + 1, a_node);
                    reg[i] = ' ';
                    reg[i + 1] = ' ';
                    ++i;
                }
                else
                {
                    create_node(reg[i], i, a_node);
                    reg[i] = ' ';
                }
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '+')
            {
                create_node(reg[i], i, plus_node);
                add_children(i, i - 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '.')
            {
                create_node(reg[i], i, cat_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

        for (int i = first + 1; i < last; i++)
        {
            if (reg[i] == '|')
            {
                create_node(reg[i], i, or_node);
                add_children(i, i - 1);
                add_children(i, i + 1);
                reg[i] = ' ';
            }
        }

    } while (first != -1 && last != -1);
   
    this->root = _get_root();
}

ST_Node* Syntax_Tree::get_root()
{
    return this->root;
}

void Syntax_Tree::draw_syntax_tree(std::string file_name)
{
    /*std::cout << "root is " << "i = " << this->root->index << "; val = " << this->root->value << ";" << std::endl;
 
    for (auto it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        std::cout << "i = " << (*it)->index << "; val = " << (*it)->value << ";\tchildren: ";

        if ((*it)->left_ptr != nullptr)
        {
            std::cout << "i = " << (*it)->left_ptr->index << "; val = " << (*it)->left_ptr->value << ";\t";
        }

        if ((*it)->right_ptr != nullptr)
        {
            std::cout << "i = " << (*it)->right_ptr->index << "; val = " << (*it)->right_ptr->value << ";\t";
        }

        std::cout << std::endl;
    }*/
    if (this->root == nullptr) return;
    std::ofstream* out = new std::ofstream(file_name);

    *out << "digraph G {" << std::endl;

    if ((this->root)->left_ptr == nullptr && (this->root)->right_ptr == nullptr)
    {
        *out << "\"" << "id:" << (this->root)->index << " " << (this->root)->value << "\"" << std::endl;
        *out << "}";
        return;
    }

    for (auto it = this->nodes.begin(); it != this->nodes.end(); it++)
    {
        if ((*it)->left_ptr != nullptr)
        {
            *out << "\"" << "id:" << (*it)->index << " " << (*it)->value << "\"" << " -> " << "\"" << "id:" << (*it)->left_ptr->index << " " << (*it)->left_ptr->value << "\"" << std::endl;
        }
        if ((*it)->right_ptr != nullptr)
        {
            *out << "\"" << "id:" << (*it)->index << " " << (*it)->value << "\"" << " -> " << "\"" << "id:" << (*it)->right_ptr->index << " " << (*it)->right_ptr->value << "\"" << std::endl;
        }
    }

    *out << "}";
}

void Syntax_Tree::add_brackets(int open, int close)
{
	std::pair<int, bracket_type> t1 = { open, BT_open };
	std::pair<int, bracket_type> t2 = { close, BT_close };
	this->brackets.push_back(t1);
	this->brackets.push_back(t2);
}

int Syntax_Tree::is_this_bracket(int index)
{
	for (auto iter = this->brackets.begin(); iter != this->brackets.end(); iter++)
	{
		if ((*iter).first == index) return 1;
	}
	return 0;
}

std::string Syntax_Tree::rep_parse(std::string reg)
{
    int op_br_count;
    int cl_br_count;
    int r_size;

    std::string new_reg;
    std::string r;

    std::string* s_ptr;
    std::string s_low, s_up;
    int low, up;
    int comm;
    int t_i;

    for (int i = 0; i < reg.size(); i++)
    {
        if (reg[i] == '{')
        {
            //Поиск r
            t_i = i - 1;
            op_br_count = 0;
            cl_br_count = 0;
            r_size = 0;
            if (reg[t_i] == ')')
            {
                ++cl_br_count;
                ++r_size;
                --t_i;

                while (op_br_count != cl_br_count)
                {
                    if (reg[t_i] == ')')
                    {
                        ++cl_br_count;
                    }
                    else if (reg[t_i] == '(')
                    {
                        ++op_br_count;
                    }
                    ++r_size;
                    --t_i;
                }

                r = reg.substr(i - r_size, r_size);
            }
            else
            {
                r_size = 1;
                r = reg[t_i];
            }

            //Поиск s_low, s_up
            t_i = i + 1;
            comm = 0;
            s_ptr = &s_low;
            while (reg[t_i] != '}')
            {
                if (reg[t_i] == ',')
                {
                    s_ptr = &s_up;
                    comm = 1;
                    ++t_i;
                    continue;
                }

                (*s_ptr) += reg[t_i];
                ++t_i;
            }

            //Пропуск части с {...} внешним счетчиком
            i += (2 + s_low.size() + s_up.size() + comm) - 1;

            low = std::stoi(s_low);

            new_reg.erase(new_reg.size() - r_size, r_size);

            new_reg += '(';

            if (low != 0)
            {
                new_reg += '(';
                new_reg += r;
                for (int i = 0; i < low - 1; i++)
                {
                    new_reg += '.';
                    new_reg += r;
                }
                new_reg += ')';
            }
            else
            {
                new_reg += "(^)";
            }

            if (s_low.size() != 0 && s_up.size() != 0) //from s_low to s_up
            {
                up = std::stoi(s_up);
                for (int ds = 0; ds < (up - low); ds++)
                {
                    new_reg += "|(";
                    new_reg += r;
                    for (int t_i = 0; t_i < low + ds; t_i++)
                    {
                        new_reg += '.';
                        new_reg += r;
                    }
                    new_reg += ')';
                }
            }
            else if (s_low.size() != 0 && comm == 1 && s_up.size() == 0) //from s_low to inf (^|r+)
            {
                new_reg += ".(^|";
                new_reg += r;
                new_reg += "+)";
            }
            else if (s_low.size() != 0 && comm == 0 && s_up.size() == 0) //from s_low to s_low 
            {
                //По сути, этот случай изначально уже обработан
            }
            else
            {
                //Ошибка
            }

            new_reg += ')';
        }
        else
        {
            new_reg += reg[i];
        }
    }

    return new_reg;
}

bracket_type Syntax_Tree::_get_bracket_type(int index)
{
	for (auto iter = this->brackets.begin(); iter != this->brackets.end(); iter++)
	{
		if ((*iter).first == index) return (*iter).second;
	}
	return bt_error;
}

ST_Node* Syntax_Tree::_get_node(int index)
{
	for (auto iter = this->nodes.begin(); iter != this->nodes.end(); iter++)
	{
		if ((*iter)->index == index) return *iter;
	}
	return nullptr;
}

stn_type Syntax_Tree::_get_type(int index)
{
	for (auto iter = this->nodes.begin(); iter != this->nodes.end(); iter++)
	{
		if ((*iter)->index == index) return (*iter)->type;
	}
	return stn_error;
}

void Syntax_Tree::create_node(char value, int index, stn_type type)
{
    std::string tmp_str;
    tmp_str = value;
    if (value == '^') tmp_str = "eps";

	this->nodes.push_back(new ST_Node(tmp_str, index, type));
}

void Syntax_Tree::add_children(int parent_i, int child_i)
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

ST_Node* Syntax_Tree::_get_root()
{
	ST_Node* temp = this->nodes.back();

	while (temp->parent != nullptr)
	{
		temp = temp->parent;
	}

	return temp;
}
