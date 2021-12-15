#pragma once
#include "CG_Unit.h"
#include <list>

class Capture_Groups
{
public:
	void add_unit(int CG_num, int stree_min_id, int stree_max_id);
	void capture(int id1, int id2, char cond, CG_Unit* last_unit);

	std::list<CG_Unit> units;

};

