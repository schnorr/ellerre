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
#ifndef __LR0_H
#define __LR0_H
#include <algorithm>
#include "grammar.h"
#include "item.h"
#include "state.h"
#include "parser.h"

class LR0 : public Parser
{

public:
  LR0();
  LR0(Grammar* grammar);
  ~LR0();

public:
  void create_item_set(void);
  void create_automata(void);
  void createTransitionStates(State* state);
  std::set<Item*> closure(std::set<Item*> kernel);
  State* createState(State* state);
  std::set<Item*> getProductionOfItem(Item* item);
  Item* getNextItem(Item* i);
};

#endif