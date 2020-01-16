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
#ifndef __LALR1_H
#define __LALR1_H
#include <algorithm>
#include "grammar.h"
#include "item.h"
#include "state.h"
#include "parser.h"
#include "LR1.h"

class LALR1 : public LR1
{

public:
  LALR1();
  LALR1(Grammar* grammar);
  ~LALR1();

public: 
  State* createState(State* newState);
  void createTransitionStates(State* state);
  void create_automata(void);
  std::set<Item*> getProductionOfItem(Item* item);
  void print_automata(void);
};

#endif