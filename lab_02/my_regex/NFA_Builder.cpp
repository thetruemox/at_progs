#include "NFA_Builder.h"

NFA_Builder::NFA_Builder(std::string regex)
{
	this->start = nullptr;
	this->recieve = nullptr;
	this->nfa_nodes_id = 0;

	this->stree = new Syntax_Tree(regex);
	this->CG = stree->get_CG();

	build(this->stree->get_root());

	//Заполнение групп захвата
	NFA_Graph* temp_graph;
	for (auto u_it = this->CG->units.begin(); u_it != this->CG->units.end(); u_it++)
	{
		temp_graph = this->find_graph((*u_it).stree_node_id);
		this->cg_around_graph(temp_graph->enter_node, temp_graph->exit_node, &(*u_it));
	}
}

void NFA_Builder::build(ST_Node* node)
{
	if (this->stree->get_root()->is_checked == 1) return;

	if (node->left_ptr == nullptr && node->right_ptr == nullptr)
	{
		if (node->value != "eps") ++(this->abc)[node->value];	

		create_graph(node);
		node->is_checked = 1;

		build(node->parent);
	}

	if (node->left_ptr != nullptr && node->right_ptr != nullptr)
	{
		if (node->left_ptr->is_checked && node->right_ptr->is_checked)
		{
			create_graph(node);
			node->is_checked = 1;

			build(node->parent);
		}

		if (!node->left_ptr->is_checked) build(node->left_ptr);
		if (!node->right_ptr->is_checked) build(node->right_ptr);
	}

	if (node->left_ptr != nullptr && node->right_ptr == nullptr)
	{
		if (node->left_ptr->is_checked)
		{
			create_graph(node);
			node->is_checked = 1;

			build(node->parent);
		}
		else build(node->left_ptr);
	}
}

void NFA_Builder::create_graph(ST_Node* node)
{
	NFA_Graph* new_graph;
	
	NFA_Node* node_A;
	NFA_Node* node_B;

	NFA_Graph* graph_A;
	NFA_Graph* graph_B;

	switch (node->type)
	{
	case a_node:
		new_graph = new NFA_Graph(node->index);

		node_A = new NFA_Node(nfa_start, this->nfa_nodes_id++);
		node_B = new NFA_Node(nfa_receiving, this->nfa_nodes_id++);

		if (node->value == "eps")
		{
			node_A->make_link(node_B, "eps");
		}
		else
		{ 
			node_A->make_link(node_B, node->value);
		}

		new_graph->add_node(node_A);
		new_graph->add_node(node_B);

		new_graph->enter_node = node_A;
		new_graph->exit_node = node_B;

		this->start = node_A;
		this->recieve = node_B;
		this->graphs.push_back(new_graph);
		break;
	case or_node:
		new_graph = new NFA_Graph(node->index);

		node_A = new NFA_Node(nfa_start, this->nfa_nodes_id++);
		node_B = new NFA_Node(nfa_receiving, this->nfa_nodes_id++);

		graph_A = find_graph(node->left_ptr->index);
		graph_B = find_graph(node->right_ptr->index);

		graph_A->enter_node->type = nfa_normal;
		graph_A->exit_node->type = nfa_normal;
		graph_B->enter_node->type = nfa_normal;
		graph_B->exit_node->type = nfa_normal;

		node_A->make_link(graph_A->enter_node, "eps");
		node_A->make_link(graph_B->enter_node, "eps");

		graph_A->exit_node->make_link(node_B, "eps");
		graph_B->exit_node->make_link(node_B, "eps");

		new_graph->enter_node = node_A;
		new_graph->exit_node = node_B;

		this->start = node_A;
		this->recieve = node_B;
		this->graphs.push_back(new_graph);
		break;
	case cat_node:
		new_graph = new NFA_Graph(node->index);

		graph_A = find_graph(node->left_ptr->index);
		graph_B = find_graph(node->right_ptr->index);

		graph_A->enter_node->type = nfa_start;
		graph_A->exit_node->type = nfa_normal;
		graph_B->enter_node->type = nfa_normal;
		graph_B->exit_node->type = nfa_receiving;

		graph_A->exit_node->links = graph_B->enter_node->links;
		graph_B->delete_node(graph_B->enter_node);
		graph_B->enter_node = graph_A->exit_node;

		new_graph->enter_node = graph_A->enter_node;
		new_graph->exit_node = graph_B->exit_node;

		this->start = graph_A->enter_node;
		this->recieve = graph_B->exit_node;
		this->graphs.push_back(new_graph);
		break;
	case plus_node:
		new_graph = new NFA_Graph(node->index);

		node_A = new NFA_Node(nfa_start, this->nfa_nodes_id++);
		node_B = new NFA_Node(nfa_receiving, this->nfa_nodes_id++);

		graph_A = find_graph(node->left_ptr->index);
		
		graph_A->enter_node->type = nfa_normal;
		graph_A->exit_node->type = nfa_normal;

		node_A->make_link(graph_A->enter_node, "eps");
		graph_A->exit_node->make_link(node_B, "eps");
		graph_A->exit_node->make_link(graph_A->enter_node, "eps");
		
		new_graph->enter_node = node_A;
		new_graph->exit_node = node_B;

		this->start = node_A;
		this->recieve = node_B;
		this->graphs.push_back(new_graph);
		break;
	default:
		throw("there is no such type!");
		break;
	}
}



void NFA_Builder::draw_syntax_tree(std::string file_name)
{
	this->stree->draw_syntax_tree(file_name);
}

NFA_Node* NFA_Builder::get_start()
{
	return this->start;
}

NFA_Node* NFA_Builder::get_recieve()
{
	return this->recieve;
}

Capture_Groups* NFA_Builder::get_CG()
{
	return this->CG;
}

std::map<std::string, int> NFA_Builder::get_abc()
{
	return this->abc;
}

void NFA_Builder::draw_nfa_graph(std::string file_name)
{
	if (this->start == nullptr) return;

	std::ofstream* out = new std::ofstream(file_name);
	*out << "digraph G {" << std::endl;

	*out << this->start->id << " [label=\"" << this->start->id << "\\nstart\"];" << std::endl;
	*out << this->recieve->id << " [label=\"" << this->recieve->id << "\\nrecieve\"];" << std::endl;

	_recursive_drawing(this->start, out);

	*out << "}";
}

void NFA_Builder::_recursive_drawing(NFA_Node* node, std::ofstream* out)
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

NFA_Graph* NFA_Builder::find_graph(int index)
{
	for (auto it = this->graphs.begin(); it != this->graphs.end(); it++)
	{
		if ((*it)->index == index) return (*it);
	}

	return nullptr;
}

void NFA_Builder::cg_around_graph(NFA_Node* A, NFA_Node* B, CG_Unit* unit)
{
	if (A == B)
	{
		if (!unit->saw_it(B->id)) unit->NFA_arr.push_back(B->id);
		return;
	}

	if (!unit->saw_it(A->id))
	{
		unit->NFA_arr.push_back(A->id);

		for (auto it = A->links.begin(); it != A->links.end(); it++)
		{
			cg_around_graph((*it).first, B, unit);
		}
	}
	
	return;
}

