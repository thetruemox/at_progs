#include "Syntax_Tree.h"

Syntax_Tree::Syntax_Tree(std::string regex)
{
    std::string reg = "(" + regex + ")";
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
	this->nodes.push_back(new ST_Node(value, index, type));
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
