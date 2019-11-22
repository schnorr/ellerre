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
#ifndef __LR1_H
#define __LR1_H
#include <algorithm>
#include "grammar.h"
#include "item.h"
#include "state.h"
#include "parser.h"

class LR1 : public Parser
{

public:
  LR1();
  LR1(Grammar* grammar);
  ~LR1();

public:
  void create_item_set(void);
  void create_automata(void);
  void createTransitionStates(State* state);
  bool isLookaheadInFirst(Symbol* head, Symbol* lookahead);
  std::set<Item*> closure(std::set<Item*> kernel);
  State* createState(State* state);
  std::set<Item*> getProductionOfItem(Item* item);
  Item* getNextItem(Item* i);
};

#endif