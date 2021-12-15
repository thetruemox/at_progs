#include "Capture_Groups.h"

void Capture_Groups::add_unit(int CG_num, int stree_min_id, int stree_max_id)
{
	CG_Unit new_unit(CG_num, stree_min_id, stree_max_id);
	this->units.push_back(new_unit);
}

void Capture_Groups::capture(int id1, int id2, char cond, CG_Unit* last_unit)
{
	for (auto u_it = this->units.begin(); u_it != this->units.end(); u_it++)
	{
		if ((*u_it).contains(2, id1) && (*u_it).contains(2, id2))
		{
			(*u_it).cap_str += cond;
		}
	}
}
