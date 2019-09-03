/*
    This file is part of Ellerre

    Ellerre is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ellerre is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Ellerre. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __STATE_H
#define __STATE_H
#include "rule.h"
#include "item.h"
#include <set>

class State
{
private:
    char* id;
    std::set<Item*> item_set;
    std::set<Symbol*, char*> transitions;

public:
	State();
    State(char* id, std::set<Item*> item_set, std::set<Symbol*, char*> transitions);
	~State();
};

std::ostream &operator<< (std::ostream &output, const State &state);

#endif